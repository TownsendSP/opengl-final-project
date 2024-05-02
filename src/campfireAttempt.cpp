//
// Created by tgsp on 4/29/2024.
//

#include "campfireAttempt.h"
#include "textureLoader.h"
#include "lighting.h"


// the flame is a bicubicSplineSurfaceLitTextured, but heavily modified
#include <cstdlib>
#include <iostream>
#include <fstream>


#include <GL/gl.h>
#include <GL/glu.h>
#include <map>
#include <string>


void Flame::draw() {
    //lighting:
    brightRed.setup();
    brightRed.enable();

    //material:
    shinyRed.apply();


    glBindTexture(GL_TEXTURE_2D, texture_24[textureMap_24["flame24"]]);
    gluBeginSurface(nurbsObject);
    gluNurbsSurface(nurbsObject, 19, uknots, 14, vknots,
                    30, 3, controlPoints[0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluNurbsSurface(nurbsObject, 4, uTextureknots, 4, vTextureknots,
                    4, 2, texturePoints[0][0], 2, 2, GL_MAP2_TEXTURE_COORD_2);
    gluEndSurface(nurbsObject);

    glDisable(GL_LIGHTING); // Disable lighting.
    glDisable(GL_TEXTURE_2D); // Disable texturing.



}


//bind bitmap to nurbs surface
void bindBitmapTextures_24(std::string name) {
    unsigned int textureID = texture_24[textureMap_24[name]];
}


// Reset control points.
void Flame::resetControlPoints(void) {
    int i, j;

    for (i = 0; i < 15; i++)
        for (j = 0; j < 10; j++) {
            controlPoints[i][j][0] = -2.5 + j * 0.5;
            controlPoints[i][j][1] = 0.0;
            controlPoints[i][j][2] = 6.0 - i;
        }
    rowCount = columnCount = 0;
}

Flame::Flame(Coord bottomLoca) {
    this->bottomLoc = bottomLoca;
    this ->nurbsObject = gluNewNurbsRenderer();
    gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
    gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 100.0);
    gluNurbsProperty(nurbsObject, GLU_DISPLAY_MODE, GLU_FILL);

    resetControlPoints(); // Fill control points array for real spline surface.

//    glEnable(GL_AUTO_NORMAL); // Enable automatic normal calculation.
}
