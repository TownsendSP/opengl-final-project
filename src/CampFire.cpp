//
// Created by tgsp on 4/29/2024.
//

#include "CampFire.h"
#include "globals.h"
#include "textureLoader.h"
#include "lighting.h"
#include "testingFunctions.h"

// the flame is a bicubicSplineSurfaceLitTextured, but heavily modified


#include <GL/gl.h>
#include <GL/glu.h>
#include <string>





float Flame::texturePoints[2][2][2] =
{
    {{0.0, 0.0}, {0.0, 1.0}},
    {{1.0, 0.0}, {1.0, 1.0}}
};
float Flame::uknots[19] =
{
    0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
    7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 12.0, 12.0, 12.0
};

float Flame::vknots[14] =
{
    0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
    7.0, 7.0, 7.0, 7.0
};

float Flame::vTextureknots[4] = {0.0, 0.0, 7.0, 7.0};

float Flame::uTextureknots[4] ={0.0, 0.0, 12.0, 12.0};
//
// void Flame::modControlPoints() {
//
// }

void Flame::draw() {
    //lighting:

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    brightRed.setup();
    brightRed.enable();

    //material:
    shinyRed.apply();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_24[textureMap_24["flame24"]]);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f); // Set alpha to 0.5
    gluBeginSurface(nurbsObject);
    gluNurbsSurface(nurbsObject, 19, uknots, 14, vknots,
                    30, 3, controlPoints[0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluNurbsSurface(nurbsObject, 4, uTextureknots, 4, vTextureknots,
                    4, 2, texturePoints[0][0], 2, 2, GL_MAP2_TEXTURE_COORD_2);
    gluEndSurface(nurbsObject);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Set alpha to 0.5

    glPopMatrix();

    glDisable(GL_AUTO_NORMAL);

    glDisable(GL_TEXTURE_2D); // Disable texturing.
}


float Flame::taperX(float z) {
    return (-0.571428571429 * z * z) + 15;
}

float Flame::funcFlameY(float z) {
    return (currX/2) * 0.571428571429 * z * z; //0.571428571429 *
}

float Flame::shiftX(float z) {
    return (currX/2) * 0.571428571429 * z * z;
}


void Flame::resetControlPoints(void){ // sets the x of all
    int i, j;
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 10; j++) {
            //og -2.5 + j * 0.5;
            if(j>4) {
                controlPoints[i][j][0] = -2.5 + taperX(j)/10 * 0.5 + shiftX(i)/10;
            } else {
                controlPoints[i][j][0] = -2.5 + taperX(j)/10 * 0.5 + shiftX(i)/10;
            }
            // controlPoints[i][j][0] = j * 0.5 + taperX(i);
            controlPoints[i][j][1] = funcFlameY(i)/15;
            // controlPoints[i][j][1] = 0.0;
            controlPoints[i][j][2] = 6.0 - i;
        }
    }
}


// Reset control points.
// void Flame::resetControlPoints(void) {
//     int i, j;
//
//     for (i = 0; i < 15; i++)
//         for (j = 0; j < 10; j++) {
//             controlPoints[i][j][0] = -2.5 + taperX(j) * 0.5 + shiftX(i);
//             controlPoints[i][j][1] = funcFlameY(j);
//             controlPoints[i][j][2] = 6.0 - i;
//         }
//     rowCount = columnCount = 0;
// }

void Flame::animate() {
    //move currY towards targetY
    currY += (targetY - currY) * 0.1;
    //if currY is close enough to targetY, set a new targetY
    if (fabs(targetY - currY) < 0.1) {
        targetY = srnd(-1, 1);
    }

    currX += (targetX - currX) * 0.1;
    //if currY is close enough to targetY, set a new targetY
    if (fabs(targetX - currX) < 0.1) {
        targetX = srnd(-1, 1);
    }

    age += 1;
}



Flame::Flame(Coord bottomLoca, Coord scalea) {
    this->relLoc = bottomLoca;
    this -> scale = scalea;
    this -> age = 0;
    this->nurbsObject = gluNewNurbsRenderer();
    //instantiate the control points:

    gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
    gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 100.0);
    gluNurbsProperty(nurbsObject, GLU_DISPLAY_MODE, GLU_FILL);

    resetControlPoints(); // Fill control points array for real spline surface.
    this -> targetY = srnd(-1, 1);
    this -> targetX = srnd(-1, 1);
    this -> currY = 0.0f;
    this -> currX = 0.0f;
    // createChessboard();
    // loadProceduralTextures();

    //    glEnable(GL_AUTO_NORMAL); // Enable automatic normal calculation.
}


Flame Campfire::genFlame(float bottomRad) {
    float sqrt1 = sqrt(srnd(-bottomRad, bottomRad) * srnd(-bottomRad, bottomRad) + srnd(-bottomRad, bottomRad) * srnd(-bottomRad, bottomRad));
    Flame ta(Coord((srnd(-bottomRad, bottomRad))/2+3, 5, srnd(-bottomRad, bottomRad)/2+3),
    Coord(1, 1, 1));
            // Coord(sqrt1,sqrt1,sqrt1));
    return ta;
}

Campfire::Campfire(int numFlames, float bottomRada) {
    // add a numFlames to the vector
    this -> bottomRad = bottomRada;
    for ( int i = 0; i < numFlames; i++) {
        flames.push_back(genFlame(bottomRad));
    }
}

void makeLog(float rad, float len, GLUquadricObj *quadric ) {
    tableMat.apply();
    glPushMatrix();
    gluCylinder(quadric, rad/0.7, rad, len, 20, 10);
    glPopMatrix();
}

void Campfire::fetchFlame(int i) {
    glPushMatrix();
    glTranslatefv(flames[i].relLoc);
    glScalefv(flames[i].scale);
    glRotatef(90, 1, 0, 0);
    flames[i].draw();
    glPopMatrix();
}

void Campfire::draw() {
    hallLight.enable();
    brightRed.enable();
    if (makeflames) {
        glEnable(GL_BLEND);

        for (int i = 0; i < flames.size(); i++) {
            glPushMatrix();
            // glScalefv(flames[i].scale);
            debugMap[30] = "Flame: " + std::to_string(i);

            fetchFlame(i);

            glPopMatrix();
        }
                glDisable(GL_BLEND);


    }

    // draw 10 cylinders, each rotated 72 degrees
    GLUquadricObj *quadric = gluNewQuadric();
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        glRotatef(36 * i, 0, 1, 0);
        glTranslatef(0, 0, 0.5);
        makeLog(1, bottomRad, quadric);
        glPopMatrix();
    }
    gluDeleteQuadric(quadric);
}

void Campfire::animate() {
    // just call animate on each of the flames
    for (int i = 0; i < flames.size(); i++) {
        if(flames[i].age * srnd(0, 1) > 1000) {
            flames.erase(flames.begin() + i);
            flames[i] = genFlame(bottomRad);
        } else {
            flames[i].animate();
            flames[i].resetControlPoints();
        }
    }
}




