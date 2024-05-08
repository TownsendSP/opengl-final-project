//
// Created by tgsp on 4/3/2024.
//

#include "globals.h"
#include "things.h"
#include "Coord.h"
#include <map>

#include <iostream>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif


extern std::map<int, std::string> debugMap;


//Window Blinds Routines
#ifndef FOLDING_REGION_BLINDS


#endif


//Drawing a 3D Debug Axis
#ifndef FOLDING_REGION_DEBUG3DX

Debug3Dx::Debug3Dx(Coord *position, float size, float weight) {
    this->size = size;
    this->weight = weight;
    this->position = *position;
}

Debug3Dx::Debug3Dx(Coord position, float size, float weight) {
    this->position = position;
    this->size = size;
    this->weight = weight;
}

Debug3Dx::Debug3Dx(float inStuff[5]) {
    this->position = {inStuff[0], inStuff[1], inStuff[2]};
    this->size = inStuff[3];
    this->weight = inStuff[4];
}

void Debug3Dx::draw() const {
    //set line width to weight
    glLineWidth(weight);
    glPushMatrix();

    // X axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X - size, position.Y, position.Z);
    glVertex3f(position.X + size, position.Y, position.Z);
    glEnd();

    //y axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y - size, position.Z);
    glVertex3f(position.X, position.Y + size, position.Z);
    glEnd();

    //z axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y, position.Z - size);
    glVertex3f(position.X, position.Y, position.Z + size);
    glEnd();

    glPopMatrix();


    glLineWidth(1);
}

void Debug3Dx::draw(Coord pos, float length, float weight) const {
    //create a new Debug3Dx object and draw it, then delete it
    Debug3Dx(pos, length, weight).draw();
    //delete it when done
}

void drawXZxGridlines(float range) {
    glColor3f(0.7f, 0.7f, 0.7f); // Set color to light gray
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0404);
    for (int i = -range; i <= range; ++i) { // NOLINT(*-narrowing-conversions)
        glBegin(GL_LINES);
        // x-axis
        glVertex3f(-range, 0.0f, static_cast<float>(i));
        glVertex3f(range, 0.0f, static_cast<float>(i));
        // z-axis
        glVertex3f(static_cast<float>(i), 0.0f, -range);
        glVertex3f(static_cast<float>(i), 0.0f, range);
        glEnd();
    }

    glDisable(GL_LINE_STIPPLE);
}

#endif

void drawBMPStr(std::string str, void *font) {
    for (int8_t c: str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void drawFlatPlaneYZ(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the YZ plane
    float yStep = (corner2.Y - corner1.Y) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X, corner1.Y + i * yStep, corner1.Z + j * zStep);
            glVertex3f(corner1.X, corner1.Y + (i + 1) * yStep, corner1.Z + j * zStep);
        }
        glEnd();
    }
}

void drawFlatPlaneXY(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the XY plane
    float xStep = (corner2.X - corner1.X) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float yStep = (corner2.Y - corner1.Y) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X + i * xStep, corner1.Y + j * yStep, corner1.Z);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y + j * yStep, corner1.Z);
        }
        glEnd();
    }
}

void drawFlatPlaneXZ(Coord corner1, Coord corner2, int numSubDivisions, GLuint textureId) { //plane in the XZ plane
    float xStep = (corner2.X - corner1.X) / numSubDivisions;
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // Specify material properties

    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            float s = i / (float)numSubDivisions;
            float t = j / (float)numSubDivisions;
            glTexCoord2f(s, t);
            glVertex3f(corner1.X + i * xStep, corner1.Y, corner1.Z + j * zStep);
            s = (i + 1) / (float)numSubDivisions;

            glTexCoord2f(s, t);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y, corner1.Z + j * zStep);
        }
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
void drawFlatPlaneXZ(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the XZ plane
    float xStep = (corner2.X - corner1.X) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips

    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X + i * xStep, corner1.Y, corner1.Z + j * zStep);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y, corner1.Z + j * zStep);
        }
        glEnd();
    }
}

void drawPlane(Coord c1, Coord c2, Coord normalVec, int numSubDivisions) {
    //draw the flat plane with the correct dimensions and location

    //figure out how the final plane will need to be rotated based on the corners;
    float debuga[3] =  {c1.X, c1.Y, c1.Z};
    float debugb[3] = {c2.X, c2.Y, c2.Z};
    float dbgn[3] = {normalVec.X, normalVec.Y, normalVec.Z};

    // XZ plane: CORRECT!
    if (abs(c1.Y - c2.Y) <= 0.001) {

        glPushMatrix();
        glNormal3fvd(normalVec);
        // glTranslatefv(c1);
        // drawFlatPlaneXZ(Coord(), c2 - c1, numSubDivisions);
        drawFlatPlaneXZ(c1, c2, numSubDivisions);
        glPopMatrix();
    }

    // XY plane:x
    if (abs(c1.Z - c2.Z) <= 0.001) {
        //here originally
        glPushMatrix();
        glNormal3fvd(normalVec);
        // glTranslatefv(c1);
        // drawFlatPlaneXY(Coord(), c2 - c1, numSubDivisions);
        drawFlatPlaneXY(c1, c2, numSubDivisions);
        glPopMatrix();
    }

    //YZ plane: rotate 90 degrees about the y axis
    if (abs(c1.X - c2.X) <= 0.001) {
        //here originally
        glPushMatrix();
        // glTranslatefv(c1);
        glNormal3fvd(normalVec);
        drawFlatPlaneYZ(c1, c2, numSubDivisions);
        glPopMatrix();
    }
    if(dbgNormals!=0) {
        glEnable(GL_LIGHTING);
    }
}

void cubeGLfrom2Points(Coord bnl, Coord tfr, uint8_t mode) {
    //draw the cube with the correct dimensions and location
    glPushMatrix();
    glTranslatefv((tfr-bnl)/2+bnl);
    glScalefv(tfr-bnl);
if(mode == WiREFRAME)
    glutWireCube(1.0);
else if(mode == SOLID)
    glutSolidCube(1.0);
    glPopMatrix();
}


//manually building the coordinate vectors for normalse:


Coord farXpositive = Coord(1.0f, 0.0f, 0.0f);//Coord farXpositive = Coord(1, 0, 0);
Coord topYpositive = Coord(0.0f, 1.0f, 0.0f);//Coord topYpositive = Coord(0, 1, 0);
Coord rightZpositive = Coord(0.0f, 0.0f, 1.0f);//Coord rightZpositive = Coord(0, 0, 1);
Coord nearXnegative = Coord(-1.0f, 0.0f, 0.0f);//farXpositive * -1;            //Coord(0, 0, 0);
Coord bottomYnegative = Coord(0.0f, -1.0f, 0.0f);//topYpositive * -1;          //Coord(0, 0, 0);
Coord leftZnegative = Coord(0.0f, 0.0f, -1.0f);//rightZpositive * -1;          //Coord(0, 0, 0);

void drawInsideOutCube(Coord bnl, Coord tfr, int numSubDiv, uint8_t whichFaces) {
    float debuga[3] =  {bnl.X, bnl.Y, bnl.Z};
    float debugb[3] = {tfr.X, tfr.Y, tfr.Z};
    if (whichFaces >> 5 & 1 && enabledFaces >> 5 & 1){ //far +X
        // int a = whichFaces >> 5 & 1;
        drawPlane(Coord(tfr.X, bnl.Y, bnl.Z), tfr, nearXnegative, numSubDiv);
    }
    if (whichFaces >> 4 & 1  && enabledFaces >> 4 & 1){ //top +Y
        drawPlane(Coord(bnl.X, tfr.Y, bnl.Z), tfr, bottomYnegative, numSubDiv);
    }
    if (whichFaces >> 3 & 1 && enabledFaces >> 3 & 1){ //right +Z
        drawPlane(Coord(bnl.X, bnl.Y, tfr.Z), tfr, leftZnegative, numSubDiv);
    }
    if (whichFaces >> 2 & 1 && enabledFaces >> 2 & 1){ //near -X
        drawPlane(bnl, Coord(bnl.X, tfr.Y, tfr.Z), farXpositive, numSubDiv);
    }
    if (whichFaces >> 1 & 1 && enabledFaces >> 1 & 1){ //bottom
        drawPlane(bnl, Coord(tfr.X, bnl.Y, tfr.Z), topYpositive, numSubDiv);
    }
    if (whichFaces & 1 && enabledFaces & 1){ //left
        drawPlane(bnl, Coord(tfr.X, tfr.Y, bnl.Z), rightZpositive, numSubDiv);
    }


}


void cubeOfPlanes(Coord bnl, Coord tfr, int numSubDiv, int insideOut, uint8_t whichFaces) {
    /*draw the cube of planes with the correct dimensions and location
    whichFaces is a bitfield that determines which faces to draw; 1 is draw, 0 is don't draw
    1: front (YZ - X+), 2: top (XZ - Y+), 3: right (XY - Z+)
    4: back (YZ - X-), 5: bottom (XZ - Y-), 6: left (XY - Z-)*/
    //flat shading
    if(insideOut == -1) {
        drawInsideOutCube(bnl, tfr, numSubDiv, whichFaces);
        return;
    }

    // glPushMatrix();
    if (whichFaces >> 5 & 1 && enabledFaces >> 5 & 1){ //far +X
        // int a = whichFaces >> 5 & 1;
            glNormal3fvd(farXpositive);
        	glPushMatrix();
        	drawPlane(Coord(tfr.X, bnl.Y, bnl.Z), tfr, farXpositive, numSubDiv); //Coord(-insideOut, 0, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 4 & 1  && enabledFaces >> 4 & 1){ //top +Y
            // glNormal3fvd(topYpositive);
            glPushMatrix();
            drawPlane(Coord(bnl.X, tfr.Y, bnl.Z), tfr, topYpositive, numSubDiv); //Coord(0, -insideOut, 0),  * insideOut
            glPopMatrix();
    }
    if (whichFaces >> 3 & 1 && enabledFaces >> 3 & 1){ //right +Z
            // glNormal3fvd(rightZpositive);
        	glPushMatrix();
        	drawPlane(Coord(bnl.X, bnl.Y, tfr.Z), tfr, rightZpositive, numSubDiv); //Coord(0, 0, -insideOut),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 2 & 1 && enabledFaces >> 2 & 1){ //near -X
            // glNormal3fvd(nearXnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(bnl.X, tfr.Y, tfr.Z), nearXnegative, numSubDiv); //Coord(-insideOut, 0, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 1 & 1 && enabledFaces >> 1 & 1){ //bottom -Y
            // glNormal3fvd(bottomYnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(tfr.X, bnl.Y, tfr.Z), bottomYnegative, numSubDiv); //Coord(0, -insideOut, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces & 1 && enabledFaces & 1){ //left -Z
            // glNormal3fvd(leftZnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(tfr.X, tfr.Y, bnl.Z), leftZnegative, numSubDiv); //Coord(0, 0, -insideOut),  * insideOut
        	glPopMatrix();
    }

    // glPopMatrix();
    if(dbgNormals!=0) {
        glEnable(GL_LIGHTING);
    }

}


