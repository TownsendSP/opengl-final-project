#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#define PI 3.1415926

#include "globals.h"
#include <random>
#include <string>


//
// Created by tgsp on 4/29/2024.
//

#include "fSceneObjs.h"

#include "Coord.h"
#include "modelLoader.h"
#include "lighting.h"
#include "things.h"


void testTexturedPlane(Coord locPostTrans, int properTexture) {
    glBindTexture(GL_TEXTURE_2D, texture_24[properTexture]);

    // Map the texture_24 onto a square polygon.
    glPushMatrix();
    glTranslatefv(locPostTrans);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-10.0, -10.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(10.0, -10.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(10.0, 10.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-10.0, 10.0, 0.0);
    glEnd();
    glPopMatrix();
}

void makeGround() {
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-50.0, 0.0, -50.0);
    glTexCoord2f(50.0, 0.0);
    glVertex3f(50.0, 0.0, -50.0);
    glTexCoord2f(50.0, 50.0);
    glVertex3f(50.0, 0.0, 50.0);
    glTexCoord2f(0.0, 50.0);
    glVertex3f(-50.0, 0.0, 50.0);
    glEnd();
    glPopMatrix();
}


void hat() {
    castIronMat.apply();
    glPushMatrix();
    glTranslatef(20, 5, 20);
    glBindTexture(GL_TEXTURE_2D, texture_24[textureMap_24["hat"]]);
    drawHatUV();
    glPopMatrix();
}

void drawTexEgs() {
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    unsigned int* texturesLocal = texture_24;
    int thing = currFrame;
    int startext = textureMap_24["star"];
    int hatTx = textureMap_24["hat"];
    int baTest = textureMap_24["grass"];



    // testTexturedPlane(Coord(30, 10, -5), startext);
    glBindTexture(GL_TEXTURE_2D, texture_24[baTest]);

    grassGreen.apply();
    glPushMatrix();
    makeGround();
    glPopMatrix();

    hat();

    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, texture_24[currFrame]);

    glPushMatrix();
    glTranslatef(0, 14, -10);
    glScalef(2, 2, 2);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-12.0, -6.5, 0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(12.0, -6.5, 0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(12.0, 6.5, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-12.0, 6.5, 0);
    glEnd();
    glPopMatrix();
    // testTexturedPlane(Coord(0, 30, -5), currFrame);
    glDisable(GL_TEXTURE_2D);

}
