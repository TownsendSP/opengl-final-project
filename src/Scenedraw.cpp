



#include "Scenedraw.h"
#include "Coord.h"

#include "globals.h"
#include "things.h"

#include "Camera.h"


extern std::map<int, std::string> debugMap;
extern Camera cam;
extern Blinds windowBlinds;

void lastHiddenCubeMirrored() {
    glColor3f(0,1,0);
    glPushMatrix();
    glTranslatef(15,-5.5 , 2);
    cubeOfPlanes(Coord(), Coord(1, -3, 1), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();
}
void drawHiddenBuffer(Coord thing) {
    glDisable(GL_LIGHTING);

    //blue cube: - plays and pauses the video
    glColor3f(0, 0, 255);
    glPushMatrix();
    glTranslatef(12, 10, -5);
    cubeOfPlanes(Coord(-12.0, -6.5, 0), Coord(-12, -6.5, 0), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //red; red box - toggles the fire
    glColor3f(255, 0, 0);
    glPushMatrix();
    // glTranslatef(17.6, 1, -0.5f);
    cubeOfPlanes(Coord(-10, 0, -10), Coord(10, 3, 10), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //green cube: -  selects the crystal
    glColor3f(0, 255, 0);
    glPushMatrix();
    Coord adfs = Coord(2,2,2);
    cubeOfPlanes(thing-adfs, thing + adfs, 20, OUTSIDEOUT, ALL_FACE);

    glPopMatrix();

    //magenta cube:
    glColor3f(255, 0, 255);
    glPushMatrix();
    // cubeOfPlanes(hallBnl + Coord(-1.25, 1.5f, 3.5f),hallBnl + Coord(-0.25, 2.5f, 4.5f), 10, OUTSIDEOUT, ALL_FACE);
    glTranslatef(10, 2, -3);
    glutSolidCube(1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}




