//
// Created by tgsp on 4/29/2024.
//

#ifndef CAMPFIREATTEMPT_H
#define CAMPFIREATTEMPT_H
#include "Coord.h"

class Flame {
private:
    // Control points for a real bicubic spline surface.
    static float controlPoints[15][10][3];

    // Control points for a spline surface in texture space.
    static float texturePoints[2][2][2] =
    {
        {{0.0, 0.0}, {0.0, 5.0}},
        {{5.0, 0.0}, {5.0, 5.0}}
    };

    // Standard knot vector along the u-parameter for the real spline surface.
    static float uknots[19] =
    {
        0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
        7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 12.0, 12.0, 12.0
    };

    // Standard knot vector along the v-parameter for the real spline surface.
    static float vknots[14] =
    {
        0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
        7.0, 7.0, 7.0, 7.0
    };

    // Standard knot vector along the u-parameter for the spline surface in texture space.
    static float uTextureknots[4] =
            {0.0, 0.0, 12.0, 12.0};

    // Standard knot vector along the v-parameter for the spline surface in texture space.
    static float vTextureknots[4] =
            {0.0, 0.0, 7.0, 7.0};
    // End globals.
public:
    Coord bottomLoc;
    GLUnurbsObj *nurbsObject;
    int rowCount;
    int columnCount;
    static float controlPoints[8][5][3];


    void draw();

    void resetControlPoints();

    Flame(Coord bottomLoca);
};

class campfireAttempt {

};



#endif //CAMPFIREATTEMPT_H
