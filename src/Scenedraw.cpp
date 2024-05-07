#include "Scenedraw.h"
#include "Coord.h"
#include "globals.h"
#include "things.h"

void drawHiddenBuffer(Coord thing) {
    glDisable(GL_LIGHTING);

    //blue cube: - plays and pauses the video
    glColor3f(0, 0, 255);
    glPushMatrix();
    glTranslatef(0, 10, -10);
    glScalef(2, 2, 2);
    cubeOfPlanes(Coord(-12.0, 0, 0), Coord(12, 6.5, 0), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //red; red box - toggles the fire
    glColor3f(255, 0, 0);
    glPushMatrix();
    // glTranslatef(17.6, 1, -0.5f);
    cubeOfPlanes(Coord(-10, 0, -10), Coord(10, 3, 10), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

if(!hasCrystal) {
    glColor3f(0, 255, 0);
    glPushMatrix();

    Coord adfs = Coord(2,2,2);
    // thing = thing * adfs;
    cubeOfPlanes(thing-adfs, thing + adfs, 20, OUTSIDEOUT, ALL_FACE);

    glPopMatrix();
}


    glEnable(GL_LIGHTING);
}
