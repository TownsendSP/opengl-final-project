#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "src/globals.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <functional>
#include <map>
#include <fstream>

//My Imports and Defines

#include "models/crystal.h"
#include "models/hat.h"
#include "src/Coord.h"
#include "src/Camera.h"
#include "src/things.h"
#include "src/ColorData.h"

#include "src/lighting.h"
#include "src/testingFunctions.h"
#include "src/LeftVP.h"
#include "models/obiwan.h"
#include "models/lightsaber.h"


#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

//section Globals
#ifndef FOLDING_REGION_Vport
int totalWidth = 1000;
int lVportW = 150;
int rVportW = 800;

int height = 701;


#endif


#ifndef FOLDING_REGION_Global_Objects
Camera cam = Camera();
std::vector<Debug3Dx> debugXes;
Blinds windowBlinds;


#endif

#ifndef FOLDING_REGION_Global_Variables

float fov = 90.0;
float animThing = 0;
//Global Variables
bool useMouse = false;
bool started = false;


float sensitivity = 0.01f; // camera movement and mouse sensitivity
float blindAnimSpeed = 0.05;
bool showInfoViewport = true;
bool drawDebugPoints = false;
bool focusPoint = false;
bool drawAllPoints = false;
int currPointDraw = 0;
//function pointer to infoVP addDebugString

std::map<int, std::string> debugMap;

int alt, ctrl, modifiers, shift;
bool showKeybinds = true;
std::vector<std::string> instructionVec = {
    "======Keybinds======",
    "F1: Toggle Information Panel",
    "F2: Shrink Console",
    "F3: Grow Console",
    "F5: Load Camera States from disk",
    "F9: Save Camera states to disk",
    "F12: Print test bitmap Alphabet",
    "1-5: Load Camera state from memory slot",
    "!-%: Save Camera state to memory slot",
    "PGUP, PGDN: Pitch camera",
    "HOME: FOV++",
    "END: FOV --",
    "Up/DArrow: Move forward/back in the XZ plane",
    "L/RArrow: Rotate Clockwise/Counterclockwise about Y",
    "W/D: Move forward/back relative to camera in XZ",
    "A/S: Strafe Left/Right in XZ",
    "F/C: Ascend/Descend",
    "SPACE: Toggle mouse Camera Control",
    "?: View/Hide Keybindings (return to console)"
};

//mid-dark grey, kinda like blender's default background
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};
ColorData rVPColorData = ColorData(0.2, 0.2, 0.2, 1.0);
ColorData solarizedBG = ColorData(0.02745, 0.21176, 0.25882, 1.0);
ColorData solarizedText = ColorData(0.71373, 0.58039, 0.58824, 1.0);


DebugLevel defaultDebug = WEAK;
bool detachSpotlight = false;
//variables for FPS Counter:


#endif

#ifndef FOLDING_REGION_MATERIALS

//section materials and lights
Light fakeSun;

int frame = 0;
auto prevTime = std::chrono::high_resolution_clock::now();

void calculateFPS() {
    frame++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - prevTime);
    if (duration.count() >= 1) {
        debugMap[60 - 3] = " FPS: " + std::to_string(frame);
        frame = 0;
        prevTime = currentTime;
    }
}
#endif

#ifndef FOLDING_REGION_Draw

void getID(int x, int y) {
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    //printed only for demonstration
    glout << "Pixel at (" << x << ", " << y << "): R: "
            << (int) pixel[0] << " G: " << (int) pixel[1] << " B: " << (int) pixel[2] << std::endl;
}

Coord rotToVec(float rotRadX, float rotRadY) {
    return Coord(cos(rotRadX) * sin(rotRadY), sin(rotRadX), cos(rotRadX) * cos(rotRadY));
}

void updateSpotlight() {
    headLamp.setup();
    headLamp.lightPos = ColorData(0, 0, 0, 1.0f);
    headLamp.spotDir = Coord(0, 0, -1);
}

void setupRight() {
    int effectiveWidth = showInfoViewport ? totalWidth - lVportW : totalWidth;

    if (showInfoViewport) {
        // glViewport(lVportW, 0, rVportW, height);
        glViewport(lVportW, 0, effectiveWidth, height);

    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, totalWidth, height);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, static_cast<float>(effectiveWidth) / height, 0.0001, 500.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    if (cardRotState == CARD_ROT_REL) {
        cardRotPercent = std::clamp((100.0f - ((cardDist - 2.0f) / 8.0f * 100.0f)) * cardRotSpeed, 0.0f, 100.0f);

        //cardRotPercent = std::clamp(cardRotSpeed * ((cardDist + 1.0f) / 8.0f) * 100.0f, 0.0f, 100.0f);

        //rdRotPercent = (std::clamp((((cardDist+1)9)*100.0f), 0.0f, 100.0f));
    }
    updateSpotlight();
    headLamp.setup();

    cam.lookAt();
    // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}




void drawModel(ModelVertex modelVertices[], unsigned int modelIndices[], int numVertices, int numIndices) {
    //enabling drawing with the drawing the loadedArrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //give it the pointers
    glVertexPointer(3, GL_FLOAT, sizeof(ModelVertex), &modelVertices[0].position[0]);
    glNormalPointer(GL_FLOAT, sizeof(ModelVertex), &modelVertices[0].normal[0]);

    //draw it using those pointers
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, modelIndices);

    //exit the vertex and normal mode
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void drawLitShapes() {
    glEnable(GL_LIGHTING);
    // sunLight.enable();
    float lightAmb[] = {0.5, 0.5, 0.5, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 20.0, 0.0, 0.0}; // Position remains the same
    // Cool global ambient light

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);
    headLamp.disable();
    // sunLight.enable();
    // glEnable(GL_LIGHT_MODEL_AMBIENT);
    //smooth shading
    glShadeModel(GL_SMOOTH);


  tableMat.apply();
  glPushMatrix();
  glTranslatef(10, 1, 10);
  drawModel(hatVertices, hatIndices, 36, 126);
  glPopMatrix();

   wallMat.apply();
  glPushMatrix();
  glTranslatef(0, 0, 20);
  drawModel(obiwanVertices, obiwanIndices, 37460, 37467);
  glPopMatrix();


    glShadeModel(GL_FLAT);
    //verts: 14805
    //idxs: 70848
    castIronMat.apply();
    glPushMatrix();
    // glTranslatef(-10, 0, -10);
    drawModel(crystalVertices, crystalIndices, 11815, 70848);
    glPopMatrix();

   // headLamp.enable();
    //
    // ceilingMat.apply();
    // glPushMatrix();
    // glTranslatef(0, 10, 0);
    // glRotatef(90, 1, 0, 0);
    // glutSolidTorus(25, 50, 150, 100);
    // glPopMatrix();
    // glPushMatrix();
    // glTranslatef(0, 10, 0);
    // glRotatef(90, 0, 1, 0);
    // glutSolidTorus(25, 50, 250, 100);
    // glPopMatrix();
    // glPushMatrix();
    // glTranslatef(0, 10, 0);
    // glutSolidTorus(25, 50, 250, 100);
    // glPopMatrix();

    glDisable(GL_LIGHTING);
    // glDisable(headLamp);
}

void drawUnlitShapes() {
    glDisable(GL_LIGHTING);
    if (defaultDebug != NONE) {
        glPushMatrix();
        drawXZxGridlines(50);
        glPopMatrix();
        glPushMatrix();
        for (Debug3Dx debug_x: debugXes) {
            debug_x.draw();
        }

        glPopMatrix();
    }

    // if (bufferPeeking) {
    //     drawHiddenBuffer();
    // }













    // testConeArot();


    // glColor3f(1, 0, 1);
    // glPushMatrix();
    // glutSolidCone(1, 1, 20, 20);
    // glPopMatrix();

    // testCamBindings();

    // testInRightPlace(cam);


    // testDrawingCubes();
    //
    // windowTest();
    glEnable(GL_LIGHTING);
}

void drawWindow() {
    if (showInfoViewport) {
        setupLeft();
        calculateFPS();
        drawLeft();
    }
    setupRight();

    drawUnlitShapes();

    // drawMoreShapes();

    // headLamp.enable();

    drawLitShapes();


    glutSwapBuffers();
}
//folding-region draw
#endif

#ifndef FOLDING_REGION_UTILITY_AND_SETUP
//section setup
bool shownKeybinds = false;
// fps function

void showKeybindings() {
    glout << NOPREFIX;
    glout << CLEARALL;
    glout << MAX;
    for (const std::string &i: instructionVec) {
        glout << i << '\n';
    }
}

void myPause(int pauseLength = 100) {
    int tStart = clock();
    while (clock() - tStart < pauseLength) { ; }
}

//section  setupObjects() {
void setupObjects() {
    cam = Camera(Coord(1, 2, 0), Coord(2, 2, 0), Coord(0, 1, 0));
    debugXes.emplace_back(Coord(0, 0, 0), 100, 2);

    windowBlinds = Blinds(2, 2, 0.1, 30);


    // roomLight = Light(

    headLamp.enable();

    // headLamp.setup();

    //giving them access to the debugging info map
    cam.setDebugStringAdd(&debugMap);
    windowBlinds.setDebugStringAdd(&debugMap);

    //setup lvp class:
}

//section  setupLights() {
void setupLights() {
    float lightAmb[] = {0.2, 0.2, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 20.0, 0.0, 0.0}; // Position remains the same
    // Cool global ambient light

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Spotlight headlamp = Spotlight(Light::lightNum(GL_LIGHT2), ColorData(cam.pos, 0.0f), ColorData(0.2, 0.2, 0.2, 1.0), ColorData(1.0, 1.0, 1.0, 1.0),
    //                                ColorData(1.0, 1.0, 1.0, 1.0), cam.tgt, 30.0, 1.0);

    //enabling global ambient light:
    //
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    //setup the sun using a positional light:
    sunLight.setup();
    sunLight.enable();
    glEnable(GL_LIGHT_MODEL_AMBIENT);
    headLamp.enable();
}

void setup() {
    winner = useTimeToSeedRandomToSetWinner();
    roomBnlF[0] = 2 * hallBnlF[0];
    roomBnlF[1] = hallBnlF[1];
    roomBnlF[2] = 3.0f * hallBnlF[2];

    roomtfrF[0] = 2 * halltfrF[0];
    roomtfrF[1] = halltfrF[1];
    roomtfrF[2] = 3 * halltfrF[2];
    // Light property vectors.


    float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 7.0, 0.0, 1.0}; // Position remains the same;

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Spotlight headlamp = Spotlight(Light::lightNum(GL_LIGHT2), ColorData(cam.pos, 0.0f), ColorData(0.2, 0.2, 0.2, 1.0), ColorData(1.0, 1.0, 1.0, 1.0),
    //                                ColorData(1.0, 1.0, 1.0, 1.0), cam.tgt, 30.0, 1.0);

    //enabling global ambient light:
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.


    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.
    glShadeModel(GL_SMOOTH);
    //check if light0 is enabled with a printStatement:
    //glListallEnabled:

    setupObjects();
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}

void resize(int w, int h) {
    totalWidth = w;
    height = h;

    lVportW = 0.2 * totalWidth; // 20% of total width
    rVportW = totalWidth - lVportW;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float) w / (float) h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}
#endif
//endsection setup
//control
#ifndef FOLDING_REGION_Control

void mouseControl(int button, int state, int x, int y) {
    //check to see if the window is focused
    if (state == GLUT_DOWN && button == GLUT_LEFT) {
        selecting = true;
        selectLock = selectLockingEnabled;
        // if selectLock, set
        glutSetCursor(selectLock? GLUT_CURSOR_LEFT_ARROW: GLUT_CURSOR_FULL_CROSSHAIR);
        if(!selectLock) {
            xClick = x;
            yClick = height - y;
        }//for screen vs mouse coordinates}}
        glutPostRedisplay();
    }
    if (state == GLUT_UP && button == GLUT_LEFT) {
        selectLock = false;
        //reset cursor to default:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        xClick=x;
        yClick=height-y;
    }
}

static bool firstMouse = true;

void mouse(int x, int y) {
    static int lastX = 0, lastY = 0;

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    int deltaX = x - lastX;
    int deltaY = lastY - y;
    lastX = x;
    lastY = y;

    cam.relRot(Coord(deltaY * sensitivity, deltaX * sensitivity, 0));

    glutPostRedisplay();
}

void toggleMouse() {
    useMouse = !useMouse;
    if (useMouse) {
        glutPassiveMotionFunc(mouse);
    } else {
        glutPassiveMotionFunc(NULL);
    }
}


void activateDoor() {
    switch (animateDoor) {
        case DOOR_CLOSED_STOPPED:
            glout << "Door Opening" << '\n';
            animateDoor = DOOR_OPENING;
            break;
        case DOOR_OPENED_STOPPED:
            glout << "Door Closing" << '\n';
            animateDoor = DOOR_CLOSING;
            break;
        case DOOR_OPENING:
            animateDoor = DOOR_OPENED_STOPPED;
            glout << "Door Opened" << '\n';
            break;
        case DOOR_CLOSING:
            glout << "Door Closed" << '\n';
            animateDoor = DOOR_CLOSED_STOPPED;
            break;
        default:
            break;
    }
}

bool useCaps = false;


void hallLightAction() {
    std::string hallLightState;
    roomLight.lightswitch();
    hallLight.lightswitch();
    hallLightState = roomLight.enabled ? "On" : "Off";
    glout << "Room Light switched " << hallLightState << std::endl;
}

void keyboard(unsigned char key, int x, int y) {
    modifiers = glutGetModifiers();
    std::string hallLightState;

    switch (key) {
        case 'W': //CAMERA FORWARD
            cam.moveCamWithColl(Coord(1 * moveSpeed, 0, 0));
            break;
        case 'S': //CAMERA BACKWARD
            cam.moveCamWithColl(Coord(-1 * moveSpeed, 0, 0));
            break;
        case 'A': //CAMERA LEFT
            cam.moveCamWithColl(Coord(0, 0, -1 * moveSpeed));
            break;
        case 'D': //CAMERA RIGHT
            if (modifiers & GLUT_ACTIVE_ALT) {
                hallLightAction();
            } else {
                cam.moveCamWithColl(Coord(0, 0, 1 * moveSpeed));
            }
            break;
        case 'C': //CAMERA DOWN
            cam.moveCamWithColl(Coord(0, -1 * moveSpeed, 0));
            break;
        case 'F': //CAMERA UP
            cam.moveCamWithColl(Coord(0, 1 * moveSpeed, 0));
            break;
        case 'd': //reset all but the camera
            if (modifiers & GLUT_ACTIVE_ALT) {
                activateDoor();
            } else {
                hallLightAction();
                activateDoor();
            }
            break;
        case 'R': //reset all
            glout << DEFAULT;
        // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

            break;
        case ' ': //Toggle Mouse control of Camera
            if (modifiers & GLUT_ACTIVE_SHIFT) {
                useCollision = !useCollision;
                glout << "Collision: " << (useCollision ? "Enabled" : "Disabled") << '\n';
            } else {
                toggleMouse();
                glout << "Mouse Control: " << (useMouse ? "On" : "Off; Use <->  and PGUP/DN") << '\n';
            }
            break;

        case '1':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ FRONT_FACE;
                glout << "FRONT_FACE: " << (enabledFaces & FRONT_FACE ? "Enabled" : "Disabled") << '\n';
            } else {
                cam.restoreState(0);
                glout << "Camera State 1 Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << "Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '2':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ TOP_FACE;
                glout << "TOP_FACE: " << (enabledFaces & TOP_FACE ? "Enabled" : "Disabled") << '\n';
            } else {
                cam.restoreState(1);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '3':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ RIGHT_FACE;
                glout << "RIGHT_FACE: " << (enabledFaces & RIGHT_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(2);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '4':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ BACK_FACE;
                glout << "BACK_FACE : " << (enabledFaces & BACK_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(3);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '5':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ BOTTOM_FACE;
                glout << "BOTTOM_FACE : " << (enabledFaces & BOTTOM_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(4);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '6':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ LEFT_FACE;
                glout << "LEFT_FACE Face: " << (enabledFaces & LEFT_FACE ? "Enabled" : "Disabled") << std::endl;
            }
        case '!': cam.storeState(0);
            glout << "State1:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '@': cam.storeState(1);
            glout << "State2:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '#': cam.storeState(2);
            glout << "State3:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '$': cam.storeState(3);
            glout << "State4:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '%': cam.storeState(4);
            glout << "State5:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '`':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces | ALL_FACE;
                glout << "All Faces Enabled" << '\n';
            } else {
                dbgNormals = nextDbgState();
                glout << "NormalColorization: " << dbgNormMap[dbgNormals] << '\n';
            }
            break;
        case 'G':
            globAmb[0] = clmp(globAmb[0] + 0.01, 0.0, 1.0);
            globAmb[1] = clmp(globAmb[1] + 0.01, 0.0, 1.0);
            globAmb[2] = clmp(globAmb[2] + 0.01, 0.0, 1.0);
            glout << "Global Ambient Light: " << globAmb[0] << ", " << globAmb[1] << ", " << globAmb[2] << '\n';
            break;
        case 'g':
            globAmb[0] = clmp(globAmb[0] - 0.01, 0.0, 1.0);
            globAmb[1] = clmp(globAmb[1] - 0.01, 0.0, 1.0);
            globAmb[2] = clmp(globAmb[2] - 0.01, 0.0, 1.0);
            glout << "Global Ambient Light: " << globAmb[0] << ", " << globAmb[1] << ", " << globAmb[2] << '\n';


            break;
        case '?': //print keybinds:
            showKeybindings();
            break;

        case 27: //Escape Key: Exit
        default:
            break;
    }
    glutPostRedisplay();
}

int counter = 0;

void testCharacterPrinting() {
    for (unsigned int i = counter * 5; i <= 255; i++) {
        glout << std::hex << std::setw(2) << std::setfill('0') << i << ": ";
        glout << (char) i << " ";
        if (i % 5 == 4) {
            glout << '\n';
            counter++;
            break;
        }
        if (counter >= 0xFE || i >= 0xFE)
            counter = 0;
    }
}


Coord angle = Coord(0, 0.0872665, 0); //5 degrees


void specialKeyboard(int key, int x, int y) {
    //modifiers:
    modifiers = glutGetModifiers();
    alt = modifiers & GLUT_ACTIVE_ALT;
    shift = modifiers & GLUT_ACTIVE_SHIFT;

    switch (key) {
        case GLUT_KEY_F1:
            showInfoViewport = !showInfoViewport;
            resize(totalWidth, height);
            break;
        case GLUT_KEY_F2:
            conHeightPercent = clmp(conHeightPercent - 0.02, 0.0, 0.52);
            glout << "Shrank console to " << conHeightPercent << '\n';
            break;

        case GLUT_KEY_F3:
            conHeightPercent = clmp(conHeightPercent + 0.02, 0.0, 0.52);
            glout << "Grew console to " << conHeightPercent << '\n';
            break;
        case GLUT_KEY_F4:
            headLamp.lightswitch();
            glout << "Headlamp switched " << headLamp.enabled ? "On\n" : "Off\n";
            debugMap[60 - 20] = "Headlamp: " + headLamp.enabled ? "On" : "Off";

            break;
        case GLUT_KEY_F5:
            cam.loadFromFile(cameraSaveFile);

            glout << "Camera Profiles loaded from " << cameraSaveFile << '\n';
            glout << "Available States: " << '\n';
            for (int i = 0; i < 5; i++) {
                glout << i + 1 << ": " << "Pos:" << std::get<0>(cam.storedStates[i]).toString(0) <<
                        " Tgt:" << std::get<1>(cam.storedStates[i]).toString(0) << '\n';
            }
            break;

        case GLUT_KEY_F6:
            activateDoor();
            break;
        case GLUT_KEY_F7:
            //if shift
            if (modifiers & GLUT_ACTIVE_SHIFT) {
                winner = 1;
                glout << "Win-Cheat: " << retWinner() << "\n";
            } else if (modifiers & GLUT_ACTIVE_ALT) {
                winner = 0;
                glout << "Win-Cheat: " << retWinner() << "\n";;
            } else {
                winner = useTimeToSeedRandomToSetWinner();
                glout << "Win-Rand: " << retWinner() << "\n";;
            }
            break;

        case GLUT_KEY_F8:
            if (modifiers & GLUT_ACTIVE_ALT) {
                cardRotState = cardRotState == CARD_ROT_REL ? CARD_ROT_UNDO : CARD_ROT_REL;
                glout << "Interactive Card: " << (cardRotState == CARD_ROT_REL ? "Enabled" : "Disabled") << '\n';
            } else {
                if (cardRotState == CARD_ROT_NONE) {
                    cardRotState = CARD_ROT_NOW;
                } else if (cardRotState == CARD_ROT_NOW || CARD_ROT_COMPLETE) {
                    cardRotState = CARD_ROT_UNDO;
                    glout << "CardUndo" << std::endl;
                }
            }
            break;
        case GLUT_KEY_F9: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
            cam.saveToFile(cameraSaveFile);
            glout << "Camera states saved to " << cameraSaveFile << '\n';
            break;

        case GLUT_KEY_F12: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
            glout << CONTROLOFF;
            testCharacterPrinting();
            glout << CONTROLON;
            break;
        case GLUT_KEY_UP: // up arrow does windowBlind.open()
            windowBlinds.open(blindAnimSpeed);
            glout << "Blinds Opened" << '\n';
            break;
        case // up arrow does windowBlind.open()
        GLUT_KEY_DOWN:
            windowBlinds.close(blindAnimSpeed);
            glout << "Blinds Closed" << '\n';
            break;
        case GLUT_KEY_RIGHT:
            // Coord angle = Coord(0, 0.0349066, 0); //2 degrees
            // Coord angle = Coord(0, 0.0872665, 0); //5 degrees
            // Coord radianAngle = angle.degToRad()
            cam.relRot(angle);
            break;
        case GLUT_KEY_LEFT:
            cam.relRot(Coord(0, -5, 0).degToRad());
            break;
        case GLUT_KEY_PAGE_UP:
            cam.relRot(Coord(5, 0, 0).degToRad());
            break;
        case GLUT_KEY_PAGE_DOWN:
            cam.relRot(Coord(-5, 0, 0).degToRad());
            break;
        case GLUT_KEY_HOME:
            fov += 5;
            glout << "FOV increased to: " << fov << '\n';
            break;
        case GLUT_KEY_END:
            fov -= 5;
            glout << "FOV decreased to: " << fov << '\n';
            break;
        case GLUT_KEY_F11:
            if (modifiers & GLUT_ACTIVE_ALT) {
                selectLockingEnabled = !selectLockingEnabled;
                glout << "Select Locking: " << (selectLockingEnabled ? "Enabled" : "Disabled") << '\n';
            } else {
                bufferPeeking = true;
            }

            break;


        default:
            break;
    }
    glutPostRedisplay();
}
#endif

#ifndef FOLDING_REGION_ANIMATION
//section ANIMATION
float cardDistFun() {
    cardDist = cam.pos.dist(Coord(18.6000004, 1.84333336, 0.25));
    return cardDist;
}

void doorAnimate() {
    if (animateDoor == DOOR_OPENING) {
        doorOpenPercent += 1;
        if (doorOpenPercent >= 100) {
            animateDoor = DOOR_OPENED_STOPPED;
            doorOpenPercent = 100;
        }
    } else if (animateDoor == DOOR_CLOSING) {
        doorOpenPercent -= 1;
        if (doorOpenPercent <= 0) {
            animateDoor = DOOR_CLOSED_STOPPED;
            doorOpenPercent = 0;
        }
    }
}

void cardAnimate() {
    if (cardRotState == CARD_ROT_NOW) {
        // cardRotState++;
        cardRotPercent++;
        if (cardRotPercent >= 100 * cardRotSpeed) {
            cardRotState = CARD_ROT_COMPLETE;
            // glout << "Card Done!" << std::endl;
        }
    } else if (cardRotState == CARD_ROT_UNDO) {
        cardRotPercent--;
        if (cardRotPercent <= 0) {
            cardRotState = CARD_ROT_NONE;
            winner = useTimeToSeedRandomToSetWinner();
            // glout << "Card Done!" << std::endl;
        }
    }
}

int peek = 0;
int numPeeks = 1000;

void peekHiddenBuffer() {
    //peek the hidden buffer for numPeeks iterations of the animation function:
    if (peek < numPeeks) {
        bufferPeeking = true;
        peek++;
        debugMap[27] = "Peeking Buffer; " + std::to_string(peek) + "/" + std::to_string(numPeeks);
    } else {
        bufferPeeking = false;
        peek = 0;
        debugMap[27] = "";
    }
}

void animate(int value) {
    // add value to the debugMap
    debugMap[60 - 5] = "MysteryValue: " + std::to_string(value);

    doorAnimate();
    if (cardRotState == CARD_ROT_NOW || CARD_ROT_UNDO) {
        cardAnimate();
    }

    if (bufferPeeking) {
        peekHiddenBuffer();
    }


    glutTimerFunc(5, animate, 1);
    glutPostRedisplay();
}
#endif


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(totalWidth, height);
    glutInitWindowPosition(10, 100);
    glutCreateWindow("Scene Display Window");
    setup();
    // setupObjects();
    glutDisplayFunc(drawWindow);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(NULL);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseControl);

    glutTimerFunc(5, animate, 1);

    for (const std::string &i: instructionVec) {
        std::cout << i << '\n';
    }

    glutMainLoop();
    return 0;
}
