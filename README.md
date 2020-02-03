# Rebel Defence

## UPDATE Feb 2020

Working in Visual Studio 2019.

If you get an error about glut32.dll missing, copy it from RebelDefence_WIN\glut-3.7.6-bin into c:\Windows\SysWOW64.

## Command-line make (not recently tested)

The following should work if you have make and g++ installed:

```shell
git clone https://github.com/jeffwmair/rebel_defence.git
cd rebel_defence/src
make
./rebeldefence
```

## Gameplay

Gameplay is straight-forward.  Use the mouse to point the cannon, click (or spacebar) to fire.  Kill the AT-ST's before they
can walk past you (the cannon).  Shoot down the tie fighters too.  Though they don't actually shoot at you.

![](/documentation/rebel-defence-gameplay.gif)

