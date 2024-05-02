//
// Created by tgsp on 4/29/2024.
//

#ifndef CAMPFIREATTEMPT_H
#define CAMPFIREATTEMPT_H
#include "Coord.h"


class Flame {
public:
    Flame() = default;
    float controlPoints[15][10][3];
    float targetY;
    float currY;
    float targetX;
    float currX;


    // Control points for a real bicubic spline surface.

    // Control points for a spline surface in texture space.
    static float texturePoints[2][2][2];

    // Standard knot vector along the u-parameter for the real spline surface.
    static float uknots[19];

    // Standard knot vector along the v-parameter for the real spline surface.
    static float vknots[14];

    // Standard knot vector along the u-parameter for the spline surface in texture space.
    static float uTextureknots[4];

    // Standard knot vector along the v-parameter for the spline surface in texture space.
    static float vTextureknots[4];

    // End globals.


    static unsigned int texture[1]; // bicubicSplineSurfaceLitTextured.cpp
    static unsigned char chessboard[64][64][3]; // Storage for chessboard image.

    GLUnurbsObj *nurbsObject; // Pointer to NURBS object.



    Coord bottomLoc;
    //GLUnurbsObj *nurbsObject;
    int rowCount;
    int columnCount;
    // static float controlPoints[8][5][3];


    void draw();

    void resetControlPoints();

    void animate();

    Flame(Coord bottomLoca);
};




class campfireAttempt {

};



#endif //CAMPFIREATTEMPT_H
