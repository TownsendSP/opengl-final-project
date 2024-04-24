//
// Created by tgsp on 4/3/2024.
//

#ifndef TGSOUTHAHW4_THINGS_H
#define TGSOUTHAHW4_THINGS_H

#include <map>

# include "ColorData.h"
# include "Coord.h"
# include "globals.h"



// class things {
//
// };

void drawBMPStr(std::string str, void* font = GLUT_BITMAP_8_BY_13);

class Debug3Dx {
public:
    float size;
    float weight;
    Coord position;

    explicit Debug3Dx(Coord* position, float size = 1.0, float weight = 1.0);

    explicit Debug3Dx(Coord position, float size = 1.0, float weight = 1.0);

   Debug3Dx(float inStuff[5]);

    // Debug3Dx(float size = 1.0, float weight = 1.0, Coord position);

    void draw() const;
    void draw(Coord pos, float length = 0.5, float weight = 0.5) const;
};

void drawXZxGridlines(float range);



class Blinds{
    float width;
    float height;
    float depth;
    float pitchAngle;
    std::map<int, std::string>* debug_string_add_map_ = nullptr;

    int numBlades;
    float bladeHeight = 0.01;

    float closedFactor;
    void drawBlade() const;

    void drawDbgPoints(DebugLevel dbg = NONE) const;

    bool opening = false;

public:
    void setDebugStringAdd(std::map<int, std::string>* mapPtr) {
        this->debug_string_add_map_ = mapPtr;
    }
    float* matSpecBlinds;
    float matShineBlinds[1]{};
    float* matAmbAndDifBlinds;
    Blinds(float width, float height, float depth, float pitchAngle, float closedFactor = 0.0);
    void draw(DebugLevel dbg = NONE) const;

    std::vector<std::string> toStrings(int prec);

    void open(float amt);
    void close(float amt);
    void animate(float amt);

    Blinds() = default;
};


class Wall {
    // assuming +x is far , -x is near, +z is right, -z is left
    Coord nearBottomLeft;
    Coord farTopRight;
    float* matSpecWall;
    float matShineWall[1]{};
    float* matAmbAndDifWall;




};

void drawPlane(Coord corner1, Coord corner2, Coord normalVec, int numSubDivisions);
void drawFlatPlaneXZ(Coord corner1, Coord corner2, int numSubDivisions);
void cubeOfPlanes(Coord bnl, Coord tfr, int numSubDiv, int insideOut = 1, uint8_t whichFaces = ALL_FACE);
void cubeGLfrom2Points(Coord bnl, Coord tfr, uint8_t mode = SOLID);



#endif //TGSOUTHAHW4_THINGS_H
