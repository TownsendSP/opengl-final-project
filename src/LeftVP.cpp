//
// Created by tgsp on 4/2/2024.
//
#include "globals.h"
#include "LeftVP.h"

#include <iostream>
#include <map>

#include "ColorData.h"

extern float rVPColor[];
extern ColorData rVPColorData;
extern ColorData solarizedBG;
extern ColorData solarizedText;
float conHeightPercent = 0.3;
extern std::map<int, std::string> debugMap;
extern int lVportW;
extern int height;


// void processConsoleMsgs() {
//     numConsoleLines = conHeightPercent * height / 15;
//     for(int i = 0; i < (consoleMsgs.size()<(numConsoleLines-1))?consoleMsgs.size():(numConsoleLines-1); i++) {
//         glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
//         glRasterPos3f(3.0, 0.3 * height - i * 15, -1.0);
//         std::for_each(consoleMsgs[i].msg.begin(), consoleMsgs[i].msg.end(),
//                       [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
//
//         consoleMsgs[i].time -= timeElapsedMillis;
//         if(consoleMsgs[i].time <= 0) {
//             consoleMsgs.erase(consoleMsgs.begin() + i);
//         }
//     }
// }

int mNum = 0;
float rposX = 3;

void processConsoleMsgs() {
    for (int i = glConsole.size() - 1; i >= 0; --i) {
        int messageHeight = 3 + (glConsole.size() - i) * 15;

        if (messageHeight > conHeightPercent * height) {
            glConsole.rm(i);
            // std::cout << "Removed message " << i << "because messageHeight " << messageHeight << " > maxHeight " <<
            //         static_cast<int>(conHeightPercent * height) << std::endl;
        } else {
            glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
            glRasterPos3f(rposX, messageHeight, -1.0);

            for (int8_t c: glConsole[i]) {
                if (c <= 0x7f && c != 0x00 && c != 0x0a) {
                    //just don't print the rest of the line if there is an illegal character
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
                }
            }
        }
    }
}

void drawLeft() {
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    //draw background with bkgColor; must be a rectangle, not glclearcolor
    glColor4f(solarizedBG.R, solarizedBG.G, solarizedBG.B, solarizedBG.A);
    // vertex array for the background
    GLint vertices[] = {
        0, 0,
        lVportW, 0,
        lVportW, height,
        0, height
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    //draw thick solarized orange line along the right edge
    glLineWidth(5.0f);
    glColor4f(0.86275, 0.19608, 0.18431, 1.0);
    glBegin(GL_LINES);
    glVertex2i(lVportW, 0);
    glVertex2i(lVportW, height);
    glEnd();

    //draw thick horizontal solarized orange line 20% from the bottom
    glLineWidth(2.50f);
    glBegin(GL_LINES);
    glVertex2i(0, conHeightPercent * height);
    glVertex2i(lVportW, conHeightPercent * height);
    glEnd();
    glLineWidth(1.0f);

    glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
    for (const auto &pair: debugMap) {
        glRasterPos3f(3.0, (pair.first + 1) * 15, -1.0);
        std::for_each(pair.second.begin(), pair.second.end(),
                      [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
    }
    processConsoleMsgs();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void setupLeft() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glViewport(0, 0, lVportW, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, lVportW, 0, height, -10, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //add message to
}
