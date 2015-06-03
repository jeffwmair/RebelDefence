CC=g++
UNAME=$(shell uname)
ifeq ($(UNAME), Darwin)
	FLAGS=-framework OpenGL -framework GLUT -framework AudioToolbox -framework CoreServices -lc -lm
	SOUND_IMPLEMENTATION=Sound.cpp
endif
ifeq ($(UNAME), Linux)
	FLAGS=-lGL -lglut -lGLU -lc -lm
	SOUND_IMPLEMENTATION=SoundLinux.cpp
endif
PROGRAM_NAME=rebeldefence

all:
	$(CC) MyMath.h math3d.h Mesh.h Mesh.cpp MeshLoaderJwm.h MeshLoaderJwm.cpp MeshLoaderObj.h MeshLoaderObj.cpp Vertex.h Vertex.cpp Walker.h Walker.cpp Camera.h Camera.cpp Cannon.h Cannon.cpp PartTransformation.h PartTransformation.cpp Part.h Part.cpp Projectile.h Projectile.cpp Quad.h Quad.cpp Rectangle.h Rectangle.cpp RenderText.h RenderText.cpp Shape.h Shape.cpp Ship.h Ship.cpp Slice.h Slice.cpp Sound.h $(SOUND_IMPLEMENTATION) main.cpp -o $(PROGRAM_NAME) $(FLAGS)
clean:
	rm $(PROGRAM_NAME)
	rm *.o 
	rm *.gch
