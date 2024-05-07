//
// Created by tgsp on 4/9/2024.
//

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

//My Imports and Defines
#include "Coord.h"
#include "Camera.h"
#include "things.h"
#include "ColorData.h"
#include "globals.h"
#include "testingFunctions.h"

//
// uint8_t animInfo = 0b000;
// Coord coneRotAnim = Coord(0, 0, 0);
// float coneHeight = 3;
// float coneRotSpeed = 5;
// extern std::map<int, std::string> debugMap;


void testConeA(float height, ColorData colorThing) {
    float baseR = height / 2;
    glPushMatrix();
    glTranslatef(0, 0, -height);
    glColor4f(colorThing.R, colorThing.G, colorThing.B, colorThing.A);
    glutSolidCone(baseR, height, 20, 20);
    glColor3f(0, 0, 0);
    glLineWidth(2.0f);
    glutWireCone(baseR, height, 20, 20);
    glLineWidth(1.0f);
    glPopMatrix();
}

void testConeArot(Coord angle) {
    glPushMatrix();
    glRotatef(angle.X, 1, 0, 0);
    glRotatef(angle.Y, 0, 1, 0);
    glRotatef(angle.Z, 0, 0, 1);
    testConeA(coneHeight);
    glPopMatrix();
}

void testConeCrot(Coord angle, ColorData coloraaaa) {  //add info to debugMap
    glPushMatrix();
    glRotatef(angle.Y, 0, 1, 0);
    glRotatef(angle.X, 1, 0, 0);
    // glRotatef(-angle.Z, 0, 0, 1);
    testConeA(10, coloraaaa);
    glPopMatrix();
}

void testInRightPlace(Camera cam) { // tests the cone originating from the camera
    glPushMatrix();
    glScalef(1, 1, -1);
    glTranslatef(0, 5,0);
    testConeCrot(Coord(D(cam.ang.X), D(cam.ang.Y-PI/2), 0),
        ColorData(0.0f, 1.0f, 0.0f, 0.3f));
    glPopMatrix();
}

void testDrawingPlanes() {
    glColor4f(0, 0, 1, 0.3f);  //ZX
    drawPlane(Coord(0, 0, 0) + Coord(0, 0, -5), Coord(1, 0, 1) + Coord(0, 0, -5), Coord(0, 1, 0), 10);

    glColor4f(0, 1, 0, 0.3f); //XY
    drawPlane(Coord(0, 0, 0) + Coord(0, 0, -5), Coord(1, 1, 0) + Coord(0, 0, -5), Coord(0, 0, 1), 10);

    glColor4f(1, 0, 0, 0.3f);
    drawPlane(Coord(0, 0, 0) + Coord(0, 0, -5), Coord(0, 1, 1) + Coord(0, 0, -5), Coord(1, 0, 0), 10);

    glColor4f(0.5, 0.5, 1, 0.3f);
    drawPlane(Coord(0, 0, 0), Coord(1, 0, 1), Coord(0, 1, 0), 10);

    glColor4f(0.5, 1, 0.5, 0.3f);
    drawPlane(Coord(0, 0, 0), Coord(1, 1, 0), Coord(0, 0, 1), 10);

    glColor4f(1, 0.5, 0.5, 0.3f);
    drawPlane(Coord(0, 0, 0), Coord(0, 1, 1), Coord(1, 0, 0), 10);

    glColor4f(1, 1, 0.5, 0.3f);
    drawPlane(Coord(0, 0, 0) + Coord(-4, 0, 0), Coord(1, 0, 1) + Coord(-4, 0, 0), Coord(0, 1, 0), 10);

    glColor4f(1, 0.5, 1, 0.3f);
    drawPlane(Coord(0, 0, 0) + Coord(-4, 0, 0), Coord(1, 1, 0) + Coord(-4, 0, 0), Coord(0, 0, 1), 10);

    glColor4f(0.5, 1, 1, 0.3f);
    drawPlane(Coord(0, 0, 0) + Coord(-4, 0, 0), Coord(0, 1, 1) + Coord(-4, 0, 0), Coord(1, 0, 0), 10);
}

void testDrawingCubes() {
    glEnable(GL_BLEND);
    glColor4f(1, 0, 0, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b100100); //front and back

    glColor4f(0, 0, 1, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b010010); //top and bottom

    glColor4f(0, 1,0, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b001001); //Left and rightX
    glDisable(GL_BLEND);

}



void windowTest() {
    Coord wall1a = Coord(0, 0, 6);
    Coord wall1B = Coord(7, 6, 6.5);
    ColorData wall1Color = ColorData(0x6446AD,  1.0);

    Coord wall2a = Coord(1, 0, 4    );
    Coord wall2B = Coord(6, 5, 4.5);
    ColorData wall2Color = ColorData(0x2AAD8F, 1.0);

    Coord window1a = Coord(2.5, 1.5, 3.99);
    Coord window1b = Coord(3.5, 2.5, 4.501);
    Coord window1Color = ColorData(0x02BCEA, 0.0);

    Coord window2a = Coord(2.5, 1.5, 4.01);
    Coord window2b = Coord(3.5, 2.5, 4.502);

    Coord window2Color = ColorData(0x02BCEA, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    //wallMat 1 should be untouched
    glColor4fv(wall1Color);
    cubeGLfrom2Points(wall1a, wall1B);



    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //window 1 will use its transparency to cut a hole in the wallMat
    glColor4fv(window1Color);
    cubeGLfrom2Points(window1a, window1b);


    //usng a different blend function to make the object show up in the hole:

    //wallMat 2 should have a hole through it
    glColor4fv(wall2Color);
    cubeGLfrom2Points(wall2a, wall2B);

    glBlendFunc(GL_ONE, GL_ZERO);
    //and this should be a translucent blue object within the hole in wallMat 2
     glColor4fv(window2Color);
     cubeGLfrom2Points(window2a, window2b);

//    glDisable(GL_BLEND);

}





















