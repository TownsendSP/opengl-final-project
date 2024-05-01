#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#define PI 3.1415926

#include "globals.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <functional>
#include <map>
#include <fstream>

//
// Created by tgsp on 4/29/2024.
//

#include "fSceneObjs.h"

#include "Coord.h"
#include "modelLoader.h"
#include "testingFunctions.h"


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

void drawTexEgs() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // glDisable(GL_LIGHTING);

    // std::map<std::string, int > it = textureMap_24;

    glBindTexture(GL_TEXTURE_2D, texture_24[textureMap_24["hat"]]);
    glPushMatrix();
    glTranslatef(10, 2, 10);
    drawHatUV();
    glPopMatrix();
    // glDisable(GL_TEXTURE_2D);
    // // Activate a texture_24.
    // // std::cout << currFrame << std::endl;
    // glEnable(GL_TEXTURE_2D);
    //
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    unsigned int* texturesLocal = texture_24;
    int thing = currFrame;
    int startext = textureMap_24["star"];
    int hatTx = textureMap_24["hat"];
    int baTest = textureMap_24["ba"];

    debugMap[60-1] = "str: " + std::to_string(startext) + " hat: " + std::to_string(hatTx) + " ba: " + std::to_string(baTest);


    testTexturedPlane(Coord(30, 10, -5), startext);
    testTexturedPlane(Coord(0, 10, -5), hatTx);
    testTexturedPlane(Coord(0, 30, -5), currFrame);
    testTexturedPlane(Coord(-30, 10, -5), baTest);
    glDisable(GL_TEXTURE_2D);
}



// void drawCampfire(int numLogs, int numFlames, int numRocks) {
//
// }
//
