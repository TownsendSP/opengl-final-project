//
// Created by tgsp on 4/29/2024.
//

#ifndef CAMPFIREATTEMPT_H
#define CAMPFIREATTEMPT_H
#include "Coord.h"
extern std::map<int, std::string> debugMap;extern std::map<int, std::string> debugMap;

class Flame {
public:
    Flame() = default;
    Coord relLoc;
    float controlPoints[15][10][3];
    float targetY;
    float currY;
    float targetX;
    float currX;
    Coord scale;
    int age;


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




    //GLUnurbsObj *nurbsObject;
    // static float controlPoints[8][5][3];


    void draw();

    float taperX(float z);

    float funcFlameY(float z);

    float shiftX(float z);

    void resetControlPoints();

    void animate();

    Flame(Coord bottomLoca, Coord scalea);


};




class Campfire {
public:
    bool exists = true;
    std::vector<Flame> flames;
    int numFlames;
    float radius;
    float bottomRad;

    Flame genFlame(float bottomRad);

    Campfire(int numFlames, float bottomRad);

    void fetchFlame(int i);

    void draw();
    void animate();






    Campfire() = default;
};



#endif //CAMPFIREATTEMPT_H
