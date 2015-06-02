//
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
#include <string>
#include <stdlib.h>
#include <glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif
#define RESOURCE_FOLDER "resources/"


#include <sstream>
#include <iomanip>
#include "Projectile.h"
#include "Cannon.h"
#include "Ship.h"
#include "Walker.h"
#include "RenderText.h"
#include "Rectangle.h"
#include "Camera.h"
#include "math3d.h"
#include "TextureReader.h"
#include "Sound.h"
#include "MeshLoaderJwm.h"
#include "MeshLoaderObj.h"


using namespace std;

#define CAM_GUN 0
#define CAM_OVERHEAD 1

#define ID_GROUND_PLANE 300
#define ID_TREE_BARK 301
#define ID_TIE_FIGHTER_IMG 302
#define ID_METAL 303
#define ID_WALKER 304

#define CANNON_PROJECTILE_SCALE 0.8
#define WALKER_PROJECTILE_SCALE 0.9

#define LIGHT_MOVE_AMOUNT 250
#define ROTATION_SPEED_SLOW_MS 90
#define ROTATION_SPEED_MED_MS 40
#define ROTATION_SPEED_FAST_MS 4
#define STATUS_BAR_HEIGHT 25

#define INITIAL_AIR_ZOOM_DIST 500
#define INITIAL_AIR_V_ANGLE 280.0
#define INITIAL_AIR_H_ANGLE -180.0

#define INITIAL_CAN_ZOOM_DIST 150.0
#define INITIAL_CAN_V_ANGLE 90.0
#define INITIAL_CAN_H_ANGLE -180.0



int _animationStepCurrent, _animationStepsTotal;
int _rotationAnimationTimeMs = ROTATION_SPEED_MED_MS;
int _fireProjectileTimeMs = 50;
GLint windowWidth, windowHeight;
int rotationKey = 0;
bool runRotationTimer = false;
int _currentCamera;

Part * surface;
Cannon * cannon;
vector<Part*> parts;
vector<Projectile*> cannonProjectiles;
vector<Projectile*> walkerProjectiles;
vector<Projectile*> shipsProjectiles;
vector<Ship *> ships;
vector<Walker *> walkers;

Camera * _arialCam;
Camera * _cannonCam;
GLint viewportHeight, viewportWidth;
GLfloat currentRotationAngle = 0.0;
RenderText * statusBarPtr;
RectangleJM statusBarBorder(GL_POLYGON);


#define WORLD_SZ 20000
#define WALKER_START_Z -WORLD_SZ

#define CANNON_FIRE_LIGHT_Y 350
#define BACK_ROW_Y 5000
#define FRONT_ROW_Y 500

#define CANNON_FIRE_Z -1500
#define BACK_ROW_Z 7500
#define FRONT_ROW_Z -15000

GLfloat light_position0[] = {0, BACK_ROW_Y, BACK_ROW_Z, 0 };
GLfloat light_position1[] = {0, FRONT_ROW_Y,  FRONT_ROW_Z, 0 };
GLfloat light_position2_spot[] = {0, CANNON_FIRE_LIGHT_Y,  CANNON_FIRE_Z, 1.0 };
GLfloat light_position2_dir[] = { 0.0, -0.5, 0.5 };

GLfloat light_back_diffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat light_back_specular[] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_front_diffuse[] = {0.20, 0.2, 0.23, 1.0};
GLfloat light_front_specular[] = {0.2, 0.2, 0.2, 1.0};
GLfloat light_cannon_fire_diffuse[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light_cannon_fire_specular[] = {0.1, 0.1, 0.1, 1.0};

GLfloat model_ambient[]  = { 0.4, 0.4, 0.4, 1.0};

GLfloat mat_plane_ambient[]    = { 0.4, 0.4, 0.4, 1.0 };
GLfloat mat_plane_specular[]   = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_plane_diffuse[]   = { 0.35, 0.35, 0.4, 1.0 };
GLfloat mat_plane_shininess[]  = { 0.3 };

GLfloat light_level_during_gameover[] = { 0, 0, 0, 1.0};
GLfloat light_level_while_damaged[] = { 0, 0, 0, 1.0 };

/* function declarations */
GLuint loadTexture(const char *filename, int width, int height, GLuint textureId);
void keypress(unsigned char key, int x, int y);
void mouseMotion(int x, int y);
void mouseButtonPress(int button, int state, int xMouse, int yMouse);
void addCircleAtLocation(GLfloat x, GLfloat y);
void removeCircleAtLocation(GLfloat x, GLfloat y);
void specialKeyPress(int key, int x, int y);
void rotateTimer(int key);
void mainGameTimer(int x);

void loadObjectTimer(int remainingSlices);
void pointObjectWithRotations(float xAngle, float yAngle, Part * part);
void updateRotationSpeedOnStatusBar();
void updateLightPositionOnStatusBar();
void drawBottomViewportContent();
void drawHierarchicalObjects(std::vector<Shape*> shapes);
void cameraToWorldCoordinates(GLint wvLeft, GLint wvBottom, GLfloat xcam, GLfloat ycam, GLfloat *xw, GLfloat *yw);
void reshapeWindow(int w, int h);
void display();
void displaySplitScreen();
void createStatusbar();

/* END OF DECLARATIONS */


int _left, _right, _top, _bottom;
int _randomSeed;
int _gameRound;
int _statusBarTextRemoveCounter;
int _gameOverCounter;
int _damageCounter;

// need 2*NumVerticies; there are 4 verticies per face
GLfloat textureMapCubeWalkerHead[96];

// 10 faces per slice on spheres; 24 slices
// so 240 faces -> 240*4 = 960 verticies; -->> so
GLfloat textureMapDeathStar[1920];
GLfloat textureMapSphere1[1920];

// 1 face on the cylinder, 10 slices
// --> 10 * 4 vertices per quad = 40, then 2x for UV
GLfloat textureMapCylinder[80];

// 4 faces, 20 slices
// 80 * 4v = 320 x 2 = 640 UV components
GLfloat textureMapGroundPlane[640];

// 9 faces, 36 slices
// 324 * 4v = 1296 x 2 =
GLfloat textureMapCannon[2592];

RGBpixmap textureArray[6];

void setupTextureMap(GLfloat * map, bool textureOnlySides, int totalSides, int facesPerSide, float textureScaleFactor) {
  
    /*
     * This starting face is based on my knowledge of the order of verticies
     in the faces and the order of faces in the cube.  Verticies go counter-clockwise
     in the faces, and the faces go from top of the cube to the bottom.
     */
 
    int verticiesPerSide = facesPerSide * NUM_VERTICIES_PER_QUAD;
    int numVertices = totalSides * verticiesPerSide;
    // point us to the first vertex that we can start texturing from on this last side
    int verticeTextureIndex = (numVertices - 3) - (4*(facesPerSide-1));

    int numFacesPerSideToTexture = facesPerSide;
    if (textureOnlySides) {
        numFacesPerSideToTexture = (facesPerSide + 1) / 3; // just divide up by three to roughly get the middle -- works for cubes and good enough for non-cubes I think?
        if (numFacesPerSideToTexture == 0) numFacesPerSideToTexture = 1;
        verticeTextureIndex += (facesPerSide - numFacesPerSideToTexture - 1) * NUM_VERTICIES_PER_QUAD;
    }

    
    float sideXMapAmount = (1.0f / totalSides) * textureScaleFactor;
    float sideYMapAmount = (1.0f / numFacesPerSideToTexture) * textureScaleFactor;
    
    int numUvComponents = numVertices * 2;
    for(int i = 0; i < numUvComponents; i++) map[i] = 0.0f;
    
    float leftPos = 0.0f;
    float rightPos = sideXMapAmount;
    float topPos = 1.0f;
    float bottomPos = 1.0f - sideYMapAmount;
    
    
    int index = (verticeTextureIndex * 2) - 2;
    for(int i = 0; i < totalSides; i++) {
        
        for(int j = 0; j < numFacesPerSideToTexture; j++) {
            
            // BOTTOM-LEFT
            map[index++] = leftPos;
            map[index++] = bottomPos;
            // BOTTOM-RIGHT
            map[index++] = rightPos;
            map[index++] = bottomPos;
            // TOP-RIGHT
            map[index++] = rightPos;
            map[index++] = topPos;
            // TOP-LEFT
            map[index++] = leftPos;
            map[index++] = topPos;
            
            verticeTextureIndex += NUM_VERTICIES_PER_QUAD;
            index = (verticeTextureIndex * 2) - 2;
            bottomPos -= sideYMapAmount;
            topPos -= sideYMapAmount;
        }
        
        // need to rever the face start position back to the usual start place since it was incremented in the inner loop
        verticeTextureIndex -= numFacesPerSideToTexture*NUM_VERTICIES_PER_QUAD;
        
        // now jump to the next side
        verticeTextureIndex -= verticiesPerSide;
        index = (verticeTextureIndex * 2) - 2;
        
        topPos = 1.0f;
        bottomPos = 1.0f - sideYMapAmount;
        
        leftPos += sideXMapAmount;
        rightPos += sideXMapAmount;

    }
    
}

void drawParts(vector<Part*> Parts) {
    
    
    for(int i = 0; i < Parts.size(); i++) {
        Part *p = Parts[i];
        
        if (p->isVisible) {
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, p->getMaterialAmbient());
            glMaterialfv(GL_FRONT, GL_SPECULAR, p->getMaterialSpecular());
            glMaterialfv(GL_FRONT, GL_DIFFUSE, p->getMaterialDiffuse());
            glMaterialfv(GL_FRONT, GL_SHININESS, p->getMaterialShininess());
            
            glPushMatrix();
            
            
            for (std::vector<PartTransformation>::reverse_iterator rit = p->Transformations.rbegin(); rit != p->Transformations.rend(); ++rit)
            {
                PartTransformation op = *rit;
                switch(op.getOpType()) {
                    case OP_ROTATE:
                        glRotatef(op.getValues()[0], op.getValues()[1], op.getValues()[2], op.getValues()[3]);
                        break;
                        
                    case OP_SCALE:
                        glScalef(op.getValues()[0], op.getValues()[1], op.getValues()[2]);
                        break;
                        
                    case OP_TRANSLATE:
                        glTranslatef(op.getValues()[0], op.getValues()[1], op.getValues()[2]);
                        break;
                }

            }
            
            // start texturing if necessary
            if (p->isTextured()) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, p->getTextureId());
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2, GL_FLOAT, 0, p->getTextureMap());
            }
            
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            
      
            glNormalPointer(GL_FLOAT, 0, p->getMesh()->getNormals());
            glVertexPointer(NUM_POINTS_PER_VERTEX, GL_FLOAT, 0, p->getMesh()->getVerticies());
            glDrawArrays(GL_QUADS, 0, p->getMesh()->getNumVerticiesTotal());
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

            // clean up texturing if we started it
            if (p->isTextured()) {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisable(GL_TEXTURE_2D);
            }
            
            if (p->getChildCount() > 0) {
                drawParts(p->getChildren());
            }
            
            glPopMatrix();
            
        }
        
    }
}

void drawLightSourceDebug() {
    glLoadIdentity();
    glTranslatef(light_position0[0], light_position0[1], light_position0[2]);
    glutSolidSphere(50, 10, 10);
    glLoadIdentity();
    glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
    glutSolidSphere(50, 10, 10);
    glLoadIdentity();
    glTranslatef(light_position2_spot[0], light_position2_spot[1], light_position2_spot[2]);
    glutSolidSphere(20, 10, 10);
}

void drawTopViewportContent() {
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
//    drawLightSourceDebug();
    glLoadIdentity();
    drawParts(parts);
    
}

float getRandFloat01() {
    return (rand() % 100) / 100.0f;
}

void setTexture(RGBpixmap *p, GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p->nCols, p->nRows, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, p->pixel);
}

Mesh * cannonBodyMesh;
Mesh * cannonSightMesh;
Mesh * walkerHeadMesh;
Mesh * projectileMesh;
Mesh * cylMesh;
Mesh * cubeMesh;
Mesh * sphereMesh;
Mesh * shipWingConnector;
Mesh * shipWing;
Mesh * planeMesh;
Mesh * starMesh;
Mesh * treeTopMesh;

#define WALKER_START_X_OFF -800
#define WALKER_SPREAD 1500

Walker * addWalker() {
    
    int numWalkers = walkers.size();
    int xOffsetMultiplier = (numWalkers % 2 == 0) ? (1*(numWalkers+1)) : (-1*(numWalkers));
    xOffsetMultiplier *= WALKER_SPREAD;
    
    Cannon * walkerGun = new Cannon(cannonBodyMesh, cannonSightMesh, false);
    Walker * walker = new Walker(ID_WALKER,
                                 ID_METAL,
                                 walkerHeadMesh,
                                 cylMesh,
                                 cubeMesh,
                                 textureMapCubeWalkerHead,
                                 textureMapCylinder,
                                 walkerGun);
    
    GLfloat xStart = xOffsetMultiplier + WALKER_START_X_OFF;
    GLfloat yStart = WALKER_BODY_HEIGHT;
    GLfloat zStart = WALKER_START_Z;
    
    walker->setStartPosition(xStart, yStart, zStart);
    walker->getBody()->setTransformationValueWithId(ID_BODY_TRANSLATION, xStart, yStart, zStart, 0);
    
    // projectile for the walker
    Projectile * p = new Projectile(true, projectileMesh, WALKER_PROJECTILE_SCALE);
    p->getPart()->setMaterialRedProjectile();
    walker->setProjectile(p);
    walkerProjectiles.push_back(p);
    parts.push_back(p->getPart());
    
    //
    walkers.push_back(walker);
    parts.push_back(walker->getBody());
    walker->start();
    return walker;
    
}

void addSurfacePlane() {
    surface = new Part(ID_GROUND_PLANE, textureMapGroundPlane, planeMesh, -1);
    surface->addRotation(-90, 0, 0, 1.0);
    surface->addScale(100.0, 0.1, 100.0);
    surface->addTranslation(-9000, 0.0, -6000);
    surface->setMaterialAmbient(mat_plane_ambient[0], mat_plane_ambient[1],mat_plane_ambient[2],mat_plane_ambient[3]);
    surface->setMaterialDiffuse(mat_plane_diffuse[0], mat_plane_diffuse[1], mat_plane_diffuse[2], mat_plane_diffuse[3]);
    surface->setMaterialSpecular(mat_plane_specular[0], mat_plane_specular[1], mat_plane_specular[2], mat_plane_specular[3]);
    surface->setMaterialShininess(mat_plane_shininess[0]);
    parts.push_back(surface);
}

void addCannonProjectiles() {
    for(int i = 0; i < 8; i++) {
        Projectile * p = new Projectile(false, projectileMesh,
                                        CANNON_PROJECTILE_SCALE);
        parts.push_back(p->getPart());
        p->getPart()->setMaterialGreenProjectile();
        cannonProjectiles.push_back(p);
    }
}
void addNightStars() {
    for(int i = 0; i < 150; i++) {
        Part * star = new Part(starMesh, -1);
        int area = WORLD_SZ * 0.5;
        int xLoc = (rand() % area) - area/2;
        int zLoc = (rand() % area) - area;
        int yLoc = (rand() % 5000) + 1000;
        float starScale = 0.25*getRandFloat01();
        star->addScale(starScale, starScale, starScale);
        star->addTranslation(xLoc, yLoc, zLoc);
        star->setMaterialAmbient(getRandFloat01(), getRandFloat01(), getRandFloat01(), 1.0);
        star->setMaterialDiffuse(getRandFloat01(), getRandFloat01(), getRandFloat01(), 1.0);
        star->setMaterialSpecular(1.0, 1.0, 1.0, 1.0);
        star->setMaterialShininess(1.0);
        parts.push_back(star);
    }
}
void addTrees() {
    for(int i = 0; i < 150; i++) {
        Part * trunk = new Part(ID_TREE_BARK, textureMapCylinder, cylMesh, -1);
        Part * top = new Part(treeTopMesh, -1);
        parts.push_back(trunk);
        trunk->addChild(top);
        
        int area = WORLD_SZ;
        int xLoc = (rand() % area*2) - area;
        int zLoc = (rand() % area * 0.2) - area;
        int yLoc = -10;
        
        float scaleRand = getRandFloat01() - 0.3;
        
        float scale1 = 1.75f + scaleRand;
        float scale2 = scale1*3;
        trunk->addScale(scale1/4.0, scale1*3, scale1/4.0);
        trunk->addTranslation(xLoc, yLoc, zLoc);
        float trunkG = 0.05;
        float trunkR = 0.1;
        trunk->setMaterialAmbient(trunkR, trunkG, 0.0, 1.0);
        trunk->setMaterialDiffuse(trunkR, trunkG, 0.0, 1.0);
        trunk->setMaterialSpecular(0, 0, 0, 1.0);
        trunk->setMaterialShininess(0);
        
        float topR = 0.05;
        float topG = 0.2;
        float topB = 0.05;
        top->addScale(scale2, scale2/4, scale2);
        top->addTranslation(0, 150, 0);
        top->setMaterialAmbient(topR, topG, topB, 1.0);
        top->setMaterialDiffuse(topR, topG, topB, 1.0);
        top->setMaterialSpecular(0, 0, 0, 1.0);
        top->setMaterialShininess(0);
    }
}
void addDeathStar() {
    Part * deathStar = new Part(ID_METAL, textureMapSphere1, sphereMesh, -1);
    int dsScale = 15;
    
    float amb = 1.0;
    float diff = 1.0;
    float spec = 1.0;
    deathStar->addScale(dsScale, dsScale, dsScale);
    deathStar->addTranslation(-7000, 4000, -19250);
    deathStar->setMaterialAmbient(amb, amb, amb, 1.0);
    deathStar->setMaterialDiffuse(diff, diff, diff, 1.0);
    deathStar->setMaterialSpecular(spec, spec, spec, 1.0);
    deathStar->setMaterialShininess(1.0);
    parts.push_back(deathStar);
}

void addMoon() {
    Part * moon = new Part(sphereMesh, -1);
    int scale = 4;
    
    float amb = 1.0;
    float diff = 0.9;
    float spec = 1.0;
    moon->addScale(0.75 * scale, scale, scale);
    moon->addTranslation(7000, 5000, -18000);
    moon->setMaterialAmbient(amb, amb, amb*0.3, 1.0);
    moon->setMaterialDiffuse(diff, diff, diff, 1.0);
    moon->setMaterialSpecular(spec, spec, spec, 1.0);
    moon->setMaterialShininess(1.0);
    parts.push_back(moon);
    
}

Ship * addShip(bool fullSize) {
    Ship * ship = new Ship(fullSize, ID_TIE_FIGHTER_IMG, sphereMesh, shipWingConnector, shipWing, textureMapSphere1);
    
    // projectile
    Projectile * p = new Projectile(true, projectileMesh, WALKER_PROJECTILE_SCALE);
    p->getPart()->setMaterialRedProjectile();
    ship->setProjectile(p);
    shipsProjectiles.push_back(p);
    parts.push_back(p->getPart());
    
    ships.push_back(ship);
    parts.push_back(ship->getBody());
 	return ship;
}

void setStatusBarText(std::string text) {
    _statusBarTextRemoveCounter = 0;
    std::stringstream ss;
    ss << "Round: " << _gameRound << " " << text << endl;
    statusBarPtr->setText(ss.str());
}

void setupInitialGameLighting() {
    // Setup lighting and material props.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_back_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_back_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_front_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_front_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_cannon_fire_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_cannon_fire_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2_spot);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_position2_dir);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
}

void newGame() {
    
    _damageCounter = 0;
    _gameRound = 1;
    _gameOverCounter = 0;
    
    setupInitialGameLighting();
    
    /* TODO: delete objects instead of just abandoning them; we have a memory leak here forsure */
    ships.clear();
    shipsProjectiles.clear();
    walkers.clear();
    walkerProjectiles.clear();
    cannonProjectiles.clear();
    parts.clear();
    
    /* some ships */
    addShip(false);
    addShip(false);
    addShip(false);
    addShip(false);
    addShip(false);
    
    // some walkers
    addWalker();
    addWalker();
    addWalker();
    
    cannon = new Cannon(cannonBodyMesh, cannonSightMesh, true);
    parts.push_back(cannon->getBody());
    
    // add the set pieces
    addSurfacePlane();
    addCannonProjectiles();
    addNightStars();
    addTrees();
    addDeathStar();
    addMoon();
    
    setStatusBarText("Started");
}




int main(int argc, char * argv[])
{
    
//    MeshLoaderObj loader;
//    loader.loadMesh("tire.obj");
//    return 1;
    string windowTitle = "CPS511 - A3 Rebel Defence";
    
    // needed for randomizing game elements
    srand(time(NULL));
    
    _statusBarTextRemoveCounter = 0;
    
    windowWidth = 900;
    windowHeight = 900;
    _left = 0;
    _right = 200;
    _top = 200;
    _bottom = 0;
    _currentCamera = CAM_GUN;
    
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GL_FLOAT | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow (windowTitle.c_str());
    glutDisplayFunc(display);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(specialKeyPress);
    glutReshapeFunc(reshapeWindow);
    glutMouseFunc(mouseButtonPress);
    glutPassiveMotionFunc(mouseMotion);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutTimerFunc(0, mainGameTimer, 0);
    GLfloat background[4];
    
    // background color
    background[0] = 0.1f;
    background[1] = 0.2f;
    background[2] = 0.3f;
    background[3] = 1.0f;
    
//    background[0] = 0.5f;
//    background[1] = 0.6f;
//    background[2] = 0.7f;
//    background[3] = 1.0f;
    
    
    glClearColor(background[0], background[1], background[2], background[3]);
    
    // fog
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, background);
    glFogf(GL_FOG_START, (WORLD_SZ)-2500.0f);    // how far away does the fog start
    glFogf(GL_FOG_END, WORLD_SZ);    // how far does it go
    glFogi(GL_FOG_MODE, GL_LINEAR); //which eq
        
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	
	string path = RESOURCE_FOLDER;
	readBMPFile(&textureArray[0], (path + "Snow_01_UV_H_CM_1.bmp").c_str());
    setTexture(&textureArray[0], ID_GROUND_PLANE);
	readBMPFile(&textureArray[1], (path + "nature_bark_0007_01_s.bmp").c_str());
    setTexture(&textureArray[1], ID_TREE_BARK);
    readBMPFile(&textureArray[2], (path + "TieFighterTexture.bmp").c_str());
    setTexture(&textureArray[2], ID_TIE_FIGHTER_IMG);
    readBMPFile(&textureArray[3], (path + "DeathStarTexture.bmp").c_str());
    setTexture(&textureArray[3], ID_METAL);
    readBMPFile(&textureArray[4], (path + "AtStTexture.bmp").c_str());
    setTexture(&textureArray[4], ID_WALKER);
    
    setupTextureMap(textureMapCubeWalkerHead, false, 4, 3, 1.0f);
    setupTextureMap(textureMapSphere1, false, 24, 10, 1.0f);
    setupTextureMap(textureMapCylinder, false, 10, 1, 1.0f);
    setupTextureMap(textureMapGroundPlane, false, 20, 5, 500.0f);
    
    createStatusbar();
    
    MeshLoaderJwm meshLoader;
    
    cannonBodyMesh = meshLoader.loadMesh((path + "CannonBody.txt").c_str());
    cannonSightMesh = meshLoader.loadMesh((path + "CannonSight.txt").c_str());
    projectileMesh = meshLoader.loadMesh((path + "Projectile-Blaster.txt").c_str());
    sphereMesh = meshLoader.loadMesh((path + "Sphereish.txt").c_str());
    shipWingConnector = meshLoader.loadMesh((path + "ShipWingConnector.txt").c_str());
    shipWing = meshLoader.loadMesh((path + "ShipWing.txt").c_str());
    planeMesh = meshLoader.loadMesh((path + "Plane.txt").c_str());
    starMesh = meshLoader.loadMesh((path + "Star.txt").c_str());
    cylMesh = meshLoader.loadMesh((path + "Cylinder.txt").c_str());
    treeTopMesh = meshLoader.loadMesh((path + "TreeTop.txt").c_str());
    cubeMesh = meshLoader.loadMesh((path + "Cube.txt").c_str());
    walkerHeadMesh = meshLoader.loadMesh((path + "CubeB.txt").c_str());
    
    newGame();
    
	_arialCam = new Camera(INITIAL_AIR_ZOOM_DIST, INITIAL_AIR_V_ANGLE, INITIAL_AIR_H_ANGLE);
	_cannonCam = new Camera(INITIAL_CAN_ZOOM_DIST, INITIAL_CAN_V_ANGLE, INITIAL_CAN_H_ANGLE);

    glutMainLoop();
}

void specialKeyPress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT ||
        key == GLUT_KEY_DOWN || key == GLUT_KEY_UP) {
        if (rotationKey != key) {
            rotationKey = key;
            runRotationTimer = true;
            rotateTimer(key);
        }
        else {
            rotationKey = -1;
            runRotationTimer = false;
        }
    }
}

#define CANNON_FIRE_FLASH_DURATION 10
#define CANNON_FIRE_FLASH_BRIGHTNESS 0.8
int fireCannonFlashCounter = -1;
void fireCannonFlashLight() {
    if (fireCannonFlashCounter == 0) {
        glEnable(GL_LIGHT2);
        fireCannonFlashCounter++;
    }
    else if (fireCannonFlashCounter > 0 && fireCannonFlashCounter <= (CANNON_FIRE_FLASH_DURATION/2)) {
        //make it brighter
        fireCannonFlashCounter++;
        light_cannon_fire_diffuse[1] += ( 1 / (CANNON_FIRE_FLASH_DURATION / CANNON_FIRE_FLASH_BRIGHTNESS));
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_cannon_fire_diffuse);
    }
    else if (fireCannonFlashCounter > (CANNON_FIRE_FLASH_DURATION/2) && fireCannonFlashCounter <= CANNON_FIRE_FLASH_DURATION) {
        // make it darker
        fireCannonFlashCounter++;
        light_cannon_fire_diffuse[1] -= ( 1 / (CANNON_FIRE_FLASH_DURATION / CANNON_FIRE_FLASH_BRIGHTNESS));
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_cannon_fire_diffuse);
    }
    else {
        glDisable(GL_LIGHT2);
    }
}


void mainGameTimer(int x) {
    
    
#define MAX_DAMAGE 10
    
   
    for(int i = 0; i < ships.size(); i++) {
        Ship * s = ships[i];
        if (s->getIsHit()) {
            bool isStillDying = s->animateDeath();
            if (isStillDying) {
                // leaving this unfinished for the ships for now
            }
        }
        else {
            s->move();
        }
    }
    
    bool allWalkersAreInactive = true; // assume true unless otherwise proven
    
    for(int i = 0; i < walkers.size(); i++) {
        Walker * w = walkers[i];
        if (w->getIsInactive()) {
            // do nothing
        }
        else if (w->getIsDying()) {
            setStatusBarText("You've taken down a walker!");
            bool isStillDying = w->animateDeath();
            if (isStillDying) {
                allWalkersAreInactive = false;
            }
        }
        else {
            w->walk(0);
            allWalkersAreInactive = false;
            
            #define MAX_GAMEOVER_COUNTER 700
            if (w->getPositionOfHead()[2] > 0 && _gameOverCounter == 0) {
                setStatusBarText("You've let a walker get through.  GAME OVER");
                for(int i = 0; i < 3; i++) light_level_during_gameover[i] = 0.5;
                _gameOverCounter++;
            }
        }
    }
    
    if(_gameOverCounter == 0 && _damageCounter > MAX_DAMAGE) {
        setStatusBarText("A walker has killed you... GAME OVER");
        _gameOverCounter = 1;
    }
    
    if (_gameOverCounter > 0) {
        // fade out the lights
        for(int i = 0; i<3; i++) light_level_during_gameover[i] -= 0.001;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_level_during_gameover);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_level_during_gameover);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_level_during_gameover);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_level_during_gameover);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_level_during_gameover);
        _gameOverCounter++;
        if (_gameOverCounter > MAX_GAMEOVER_COUNTER) newGame();

    }
    
    /* if we killed them all, then go to the next round */
    if (allWalkersAreInactive) {
        for(int i = 0; i < walkers.size(); i++) {
            Walker * w = walkers[i];
            w->resetToStartPosition();
            w->start();
        }
        _gameRound++;
        _damageCounter = 0; // RESET DAMAGE EACH ROUND
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient); // reset the ambient light
        if (_gameRound < 10) addWalker(); // don't add too many enemies or it will be impossible
    }
    
    for(std::vector<Projectile*>::iterator itr = walkerProjectiles.begin(); itr != walkerProjectiles.end(); ++itr) {
        Projectile * p = *itr;
        if (p->fireContinued()) {
            if (p->isColliding(cannon->getPosition())) {
                
                stringstream s;
                s << "You've been hit!!! Your damage level is " << _damageCounter << " out of " << MAX_DAMAGE << endl;
                setStatusBarText(s.str());
                _damageCounter++;
                p->setIsFiring(false);
#define DAMAGE_COLOR_CHANGE 0.05
                
                if (_damageCounter == 1) {
                    // reset the temp ambient light array so we can use it to fade to red
                    for(int i = 0; i < 3; i++) light_level_while_damaged[i] = model_ambient[i];
                }
                else {
                    light_level_while_damaged[0] += DAMAGE_COLOR_CHANGE;
                    light_level_while_damaged[1] -= DAMAGE_COLOR_CHANGE*0.5;
                    light_level_while_damaged[2] -= DAMAGE_COLOR_CHANGE*0.5;
                    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_level_while_damaged);
                }

            }
        }
    }
    
    for(std::vector<Projectile*>::iterator itr = shipsProjectiles.begin(); itr != shipsProjectiles.end(); ++itr) {
        Projectile * p = *itr;
        if (p->fireContinued()) {
            // ships won't hit anything
        }
    }
    
    for(std::vector<Projectile*>::iterator itr = cannonProjectiles.begin(); itr != cannonProjectiles.end(); ++itr) {
        Projectile * p = *itr;
        if (p->fireContinued()) {
            
            for(int i = 0; i < walkers.size(); i++) {
                Walker * w = walkers[i];
                if (!w->getIsInactive() && p->isColliding(w->getPositionOfHead())) {
                    walkers[i]->setIsHit();
                    p->setIsFiring(false);
                    setStatusBarText("You've hit a walker!  Watch out, he's firing at you!");
                    break;  // can only hit one at a time really
                }
            }
            for(int i = 0; i < ships.size(); i++) {
                GLfloat shipX, shipY, shipZ;
                ships[i]->getPosition(&shipX, &shipY, &shipZ);
                GLfloat pos[3];
                pos[0] = shipX;
                pos[1] = shipY;
                pos[2] = shipZ;
                if (p->isColliding(pos)) {
                    ships[i]->setIsHit();
                    p->setIsFiring(false);
                    setStatusBarText("You've taken down a tie fighter!");
                    break;  // can only hit one at a time really
                }
            }
            
        }
        
    }
    
    cannon->recoilContinue();
#define STATUS_BAR_DURATION 300
    fireCannonFlashLight();
    _statusBarTextRemoveCounter++;
    if (_statusBarTextRemoveCounter > STATUS_BAR_DURATION) {
        setStatusBarText("");
    }
    
    glutPostRedisplay();
    glutTimerFunc(10, mainGameTimer, 0);
}


void userCannonFire() {

    
    bool allProjectilesAreFiring = true;
    int freeProjectileNumber = 0;
    for(int i = 0; i < cannonProjectiles.size(); i++) {
        if (!cannonProjectiles[i]->isFiring()) {
            allProjectilesAreFiring = false;
            freeProjectileNumber = i;
            break;
        }
    }
    
    if (cannon->isRecoiling() || allProjectilesAreFiring)
        return;
    
    fireCannonFlashCounter = 0; // starts the light 'flash' process
    Sound::playCannonFire();
    cannon->recoilBegin(_cannonCam->getCamX(), _cannonCam->getCamY(), _cannonCam->getCamZ());
   
    Projectile * projectileToFire = cannonProjectiles[freeProjectileNumber];
    float cannonPosX = cannon->getPosition()[0];
    float cannonPosY = cannon->getPosition()[1];
    float cannonPosZ = cannon->getPosition()[2];
    projectileToFire->fireBegin(_cannonCam->getHAngle(), _cannonCam->getVAngle(), cannonPosX, cannonPosY, cannonPosZ);
    
}

void rotateTimer(int unused) {
    Camera * cam = _cannonCam;
    if (_currentCamera == CAM_OVERHEAD) {
        cam = _arialCam;
    }
    switch (rotationKey) {
    	case GLUT_KEY_LEFT:
            cam->adjustHorizontalAngle(2);
            break;
        case GLUT_KEY_UP:
            cam->adjustVerticalAngle(-2);
    		break;
        case GLUT_KEY_RIGHT:
            cam->adjustHorizontalAngle(-2);
    		break;
        case GLUT_KEY_DOWN:
            cam->adjustVerticalAngle(2);
            break;
        default:
    		break;
    }
    
//    std::stringstream ss;
//    ss << "H:" << cam->getHAngle() << " V:" << cam->getVAngle() << endl;
//    statusBarPtr->setText(ss.str());
    
    if (runRotationTimer) {
        glutTimerFunc(_rotationAnimationTimeMs, rotateTimer, rotationKey);
    }
}
void keypress(unsigned char key, int x, int y)
{
    switch(key) {
        case ' ':
            userCannonFire();
            break;
        case 'a':
            setStatusBarText("Zoomed in");
            _arialCam->zoomIn();
            break;
        case 'z':
            setStatusBarText("Zoomed out");
            _arialCam->zoomOut();
            break;
        case 'p':
            _currentCamera +=1;
            _currentCamera = _currentCamera % 2; // controls how many cameras there are
            if (_currentCamera == CAM_GUN) {
                setStatusBarText("Switched to Cannon Cam");
            }
            else if (_currentCamera == CAM_OVERHEAD) {
                setStatusBarText("Switched to Aerial Cam");
            }
            glutPostRedisplay();
            break;
        case 't':
            if (!glIsEnabled(GL_LIGHT2)) {
                glEnable(GL_LIGHT2);
                cout << " enabled " << endl;
            }
            else {
                glDisable(GL_LIGHT2);
            }
            glutPostRedisplay();
            break;
        case 'd':
            light_position0[0] += LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case 'c':
            light_position0[0] -= LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case 'f':
            light_position0[1] += LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case 'v':
            light_position0[1] -= LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case 'g':
            light_position0[2] += LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case 'b':
            light_position0[2] -= LIGHT_MOVE_AMOUNT;
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            glutPostRedisplay();
            updateLightPositionOnStatusBar();
            break;
        case '1':
            _rotationAnimationTimeMs = ROTATION_SPEED_SLOW_MS;
            updateRotationSpeedOnStatusBar();
            break;
        case '2':
            _rotationAnimationTimeMs = ROTATION_SPEED_MED_MS;
            updateRotationSpeedOnStatusBar();
            break;
        case '3':
            _rotationAnimationTimeMs = ROTATION_SPEED_FAST_MS;
            updateRotationSpeedOnStatusBar();
            break;
            
    }
}

void updateLightPositionOnStatusBar() {
    stringstream s;
    s << "Light source x:" << light_position0[0] << " y:" << light_position0[1] << " z:" << light_position0[2];
    setStatusBarText(s.str());
}

void updateRotationSpeedOnStatusBar() {
    switch (_rotationAnimationTimeMs) {
        case ROTATION_SPEED_SLOW_MS:
            setStatusBarText("Rotation speed set to slow");
            break;
        case ROTATION_SPEED_MED_MS:
            setStatusBarText("Rotation speed set to medium");
            break;
        case ROTATION_SPEED_FAST_MS:
            setStatusBarText("Rotation speed set to fast");
            break;
        default:
            break;
    }
    glutPostRedisplay();
    
}

void mouseButtonPress(int button, int state, int xMouse, int yMouse)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        switch (state)
        {
            case GLUT_DOWN:
                userCannonFire();
    			break;
        }
    }
}

void pointObjectWithRotations(float xAngle, float yAngle, Part * part) {
    part->setTransformationValueWithId(ID_ROTATION_Y, -yAngle, 1.0, 0.0, 0.0);
    part->setTransformationValueWithId(ID_ROTATION_X, 180+xAngle, 0.0, 1.0, 0.0);
}

void mouseMotion(int x, int y) {
    
    Camera * cam = _cannonCam;
    
    if (x < 0 || x > viewportWidth || y < 0 || y > viewportHeight) {
        return;
    }
    
    float midX = viewportWidth / 2.0;
    float midY = viewportHeight / 2.0;
    float xPosPct = (midX - x) / viewportWidth;
    float yPosPct = (y - midY) / viewportHeight;
    
    /* move the cannon flash light horizontally (could be improved to follow the end of the cannon, but we're getting down to the wire here */
    
    light_position2_spot[0] = -1000*xPosPct;
    
    float xAngle = xPosPct * 180.0 + INITIAL_CAN_H_ANGLE;
    float yAngle = yPosPct * 120.0 + INITIAL_CAN_V_ANGLE;
    
    if (yAngle > 90) yAngle = 90;
    
    cam->setHAngle(xAngle);
    cam->setVAngle(yAngle);
    
    // cannon has to rotate if we are on cannon cam
    // cannon cam -- the cannon has to move too
    pointObjectWithRotations(xAngle, yAngle, cannon->getBody());
    
//    std::stringstream ss;
//    ss << "H:" << cam->getHAngle() << " V:" << cam->getVAngle() << endl;
//    statusBarPtr->setText(ss.str());
    
    glutPostRedisplay();
    
}

void createStatusbar() {
    
    float totalWidth = (_right - _left);
    
    /* status bar */
    statusBarBorder.setScaling(totalWidth, STATUS_BAR_BOX_HEIGHT, 0);
    statusBarBorder.setOffset(_left, 50, 0);
    statusBarBorder.setColor(0.8f,0.9f,0.8f);
    statusBarPtr = new RenderText("Start!", STATUS_BAR_TEXT_SCALE);
    statusBarPtr->setOffset(5, 5, 1);
    statusBarPtr->setScaling(0.025, 0.75, 1);
    statusBarPtr->setColor(0.9f, 0.9f, 0.9f);
    statusBarBorder.addRenderText(statusBarPtr);
}

void reshapeWindow(int w, int h) {
    if (h == 0) h = 1;
    viewportHeight = h;
    viewportWidth = w;
    
}

void displaySplitScreen() {
    
    // two viewports requires float the setup of everything
    for (int i = 0 ; i < 2; i++) {
        
        if (i == 0) {
            glViewport(0, 0, viewportWidth, 25);
        }
        else {
            glViewport(0, 25, viewportWidth, viewportHeight-25);
        }
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        if (i == 0) {
            gluOrtho2D(_left, _right, _bottom, _top);
        }
        else if (i == 1) {
            gluPerspective(60.0, CAM_ASPECT_RATIO_X, CAM_MIN_VIEW_DISTANCE, CAM_MAX_VIEW_DISTANCE);
            if (_currentCamera == CAM_GUN) {
                float zPos = 130; // must be less than INITIAL_CAN_ZOOM_DIST
                float yPos = sqrt(pow(INITIAL_CAN_ZOOM_DIST,2) - pow(zPos,2));
                gluLookAt(0.0, yPos, zPos,
                          _cannonCam->getCamX(),
                          _cannonCam->getCamY(),
                          _cannonCam->getCamZ(),
                          0.0, 1.0, 0.0);
            }
            else if (_currentCamera == CAM_OVERHEAD) {
                gluLookAt(_arialCam->getCamX(),
                          _arialCam->getCamY(),
                          _arialCam->getCamZ(),
                          0.0, 100.0, 0.0,
                          0.0,
                          _arialCam->getCamUpY(),
                          _arialCam->getCamUpZ());
            }
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        if (i == 0) {
            drawBottomViewportContent();
        }
        else {
            drawTopViewportContent();
        }
    }
}

void display() {
    
    // clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    displaySplitScreen();
    
    // flush the drawing commands and swap
    glutSwapBuffers();
}

void drawHierarchicalObjects(std::vector<Shape*> shapes) {
    
    if (shapes.size() == 0) return;
    
    glPushMatrix();
    for(std::vector<Shape*>::iterator itr = shapes.begin(); itr != shapes.end(); ++itr) {
        Shape * s = *itr;
        
        glPushMatrix();
        glTranslated(s->getOffset()[0], s->getOffset()[1], 0);    // children should take on the parent's position, not the scale
        
        /* draw any render text in the shape */
        std::vector<RenderText*> renderTexts = s->getRenderTexts();
        for (std::vector<RenderText*>::iterator itrText = renderTexts.begin(); itrText != renderTexts.end(); itrText++) {
            RenderText * rt = *itrText;
            glColor3f(rt->getColor()[0], rt->getColor()[1], rt->getColor()[2]);
            glPushMatrix();
            
            glTranslated(rt->getOffset()[0], rt->getOffset()[1], rt->getOffset()[2]);
            glScaled(rt->getScaling()[0], rt->getScaling()[1], rt->getScaling()[2]);
            
            for(int i = 0; i < rt->getText().size(); i++) {
                glTranslated(10, 0.0, 0.0); // extra space between the letters so its more readable
                glutStrokeCharacter(GLUT_STROKE_ROMAN,  rt->getText().at(i));
            }
            glPopMatrix();
            
            glPushMatrix();
            drawHierarchicalObjects(rt->getChildren());
            glPopMatrix();
            
        }
        
        glPushMatrix();
        glScaled(s->getScaling()[0], s->getScaling()[1], 0);
        glColor3f(s->getColor()[0], s->getColor()[1], s->getColor()[2]);
        glBegin(s->shapeType);
        
        std::vector<Vertex3> vertices = s->getVerticies();
        for(std::vector<Vertex3>::iterator iterVertices = vertices.begin();
            iterVertices != vertices.end();
            ++iterVertices)
        {
            Vertex3 v = *iterVertices;
            glVertex3f(v.getX(), v.getY(), v.getZ());
        }
        glEnd();
        glPopMatrix();
        
        drawHierarchicalObjects(s->getChildren());
        glPopMatrix();
        
    }
    
    glPopMatrix();
    
}

void drawBottomViewportContent() {
    
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    
    vector<Shape *> statusBar;
    statusBar.push_back(&statusBarBorder);
    drawHierarchicalObjects(statusBar);
}



