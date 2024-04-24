//
// Created by tgsp on 4/9/2024.
//

#ifndef TESTINGFUNCTIONS_H
#define TESTINGFUNCTIONS_H
#include "Coord.h"
#include "Camera.h"
#include "ColorData.h"
#include "globals.h"




class testingFunctions {

};

// uint8_t animInfo;
// Coord coneRotAnim;
// float coneHeight;
// float coneRotSpeed;
inline uint8_t animInfo = 0b000;
inline Coord coneRotAnim = Coord(0, 0, 0);
inline float coneHeight = 3;
inline float coneRotSpeed = 5;
extern std::map<int, std::string> debugMap;


void testConeArot(Coord angle = coneRotAnim);
void testConeA(float height, ColorData colorThing = ColorData(1.0f, 0.0f, 1.0f, 1.0f));
void testConeCrot(Coord angle, ColorData coloraaaa) ;
void testInRightPlace(Camera cam);

void testInRightPlace();
void testDrawingPlanes();
void testDrawingCubes();
void windowTest();
#endif //TESTINGFUNCTIONS_H
