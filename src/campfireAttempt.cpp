//
// Created by tgsp on 4/29/2024.
//

#include "campfireAttempt.h"
#include "textureLoader.h"
#include "lighting.h"

// the flame is a bicubicSplineSurfaceLitTextured, but heavily modified


#include <GL/gl.h>
#include <GL/glu.h>
#include <map>
#include <string>


float Flame::texturePoints[2][2][2] =
{
    {{0.0, 0.0}, {0.0, 5.0}},
    {{5.0, 0.0}, {5.0, 5.0}}
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



    glDisable(GL_LIGHTING); // Disable lighting.

    glDisable(GL_AUTO_NORMAL);

    glDisable(GL_TEXTURE_2D); // Disable texturing.
}

// void drawFlame(Flame input) {
//     //lighting:
//     glDisable(GL_LIGHTING);
//     glEnable(GL_AUTO_NORMAL);
//     glEnable(GL_TEXTURE_2D);
//
//     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
//     glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//     brightRed.setup();
//     brightRed.enable();
//
//     //material:
//     shinyRed.apply();
//     glEnable(GL_COLOR_MATERIAL);
//     glColor3i(255, 0, 255);
//
//
//     glPushMatrix();
//     glBindTexture(GL_TEXTURE_2D, texture_24[textureMap_24["flame24"]]);
//     gluBeginSurface(nurbsObject);
//     gluNurbsSurface(nurbsObject, 19, uknots, 14, vknots,
//                     30, 3, controlPoints[0][0], 4, 4, GL_MAP2_VERTEX_3);
//     gluNurbsSurface(nurbsObject, 4, uTextureknots, 4, vTextureknots,
//                     4, 2, texturePoints[0][0], 2, 2, GL_MAP2_TEXTURE_COORD_2);
//     gluEndSurface(nurbsObject);
//     glPopMatrix();
//
//
//     glDisable(GL_COLOR_MATERIAL);
//     glDisable(GL_LIGHTING); // Disable lighting.
//
//     glDisable(GL_AUTO_NORMAL);
//
//     glDisable(GL_TEXTURE_2D); // Disable texturing.
// }

// void createChessboard(void) {
//     int i, j;
//     for (i = 0; i < 64; i++)
//         for (j = 0; j < 64; j++)
//             if ((((i / 8) % 2) && ((j / 8) % 2)) || (!((i / 8) % 2) && !((j / 8) % 2))) {
//                 chessboard[i][j][0] = 0xFF;
//                 chessboard[i][j][1] = 0x00;
//                 chessboard[i][j][2] = 0x00;
//             } else {
//                 chessboard[i][j][0] = 0xFF;
//                 chessboard[i][j][1] = 0xFF;
//                 chessboard[i][j][2] = 0xFF;
//             }
// }

// void loadProceduralTextures() {
//     // Bind chessboard image to texture index[0].
//     glBindTexture(GL_TEXTURE_2D, texture_24[0]);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, chessboard);
// }

float taperX(float z) {
    return -0.571428571429 * z * z + 15;
}

float funcFlameY(float z) {
    return z * z; //0.571428571429 *
}

float shiftX(float z) {
    return 0.571428571429 * z * z;
}


void Flame::updatePts(void){ // sets the x of all
    int i, j;
    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++) {
            controlPoints[i][j][0] = -2.5 + taperX(j) * 0.5;
            controlPoints[i][j][1] = 0.0;
            controlPoints[i][j][2] = 6.0 - i;
        }
    rowCount = columnCount = 0;
}


// Reset control points.
void Flame::resetControlPoints(void) {
    int i, j;

    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++) {
            controlPoints[i][j][0] = -2.5 + taperX(j) * 0.5;
            controlPoints[i][j][1] = funcFlameY(j);
            controlPoints[i][j][2] = 6.0 - i;
        }
    rowCount = columnCount = 0;
}

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
}

Flame:: Flame(Coord bottomLoca) {
    this->bottomLoc = bottomLoca;
    this->nurbsObject = gluNewNurbsRenderer();
    //instantiate the control points:

    gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
    gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 100.0);
    gluNurbsProperty(nurbsObject, GLU_DISPLAY_MODE, GLU_FILL);

    resetControlPoints(); // Fill control points array for real spline surface.
    this -> targetY = srnd(-1, 1);
    this -> targetX = srnd(-1, 1);
    this -> currY = 0;
    this -> currX = 0.0f;
    // createChessboard();
    // loadProceduralTextures();

    //    glEnable(GL_AUTO_NORMAL); // Enable automatic normal calculation.
}
