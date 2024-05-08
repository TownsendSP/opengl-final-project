
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
#include <map>
#include <filesystem>
#include "src/Coord.h"
#include "src/Camera.h"
#include "src/things.h"
#include "src/ColorData.h"
#include "src/lighting.h"
#include "src/LeftVP.h"
#include "src/Scenedraw.h"
#include "src/textureLoader.h"
#include "src/fSceneObjs.h"
#include "src/CampFire.h"
#include "src/modelLoader.h"


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



#endif

#ifndef FOLDING_REGION_Global_Variables

float fov = 90.0;
float animThing = 0;
//Global Variables
bool useMouse = false;
bool started = false;


float sensitivity = 0.01f;



std::map<int, std::string> debugMap;

int alt, ctrl, modifiers, shift;

void printinstructionsfromtxt() {
    std::ifstream file(RESOURCEDIR "/instructions.txt");
    if (!file) {return;}
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

std::vector<std::string> gameplayVec = {
        "     Gameplay: ",
        "Find the blue crystal",
        "Pick it up by clicking it",
        "Bring it to the campfire",
        "while holding the crystal, light the campfire",
        "	by clicking the campfire",
        "Now, you can play the movie onscreen"
};

//mid-dark grey, kinda like blender's default background
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};
ColorData rVPColorData = ColorData(0.2, 0.2, 0.2, 1.0);
ColorData solarizedBG = ColorData(0.02745, 0.21176, 0.25882, 1.0);
ColorData solarizedText = ColorData(0.71373, 0.58039, 0.58824, 1.0);

//flame
// Flame testflame;
Campfire testcampfire;

DebugLevel defaultDebug = WEAK;
//variables for FPS Counter:

bool animClip = false;

#endif

#ifndef FOLDING_REGION_MATERIALS

int frame = 0;
auto prevTime = std::chrono::high_resolution_clock::now();
int fps;
void calculateFPS() {
    frame++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - prevTime);
    if (duration.count() >= 1) {
        debugMap[60 - 3] = " FPS: " + std::to_string(frame);
        fps = frame;
        frame = 0;
        prevTime = currentTime;
    }
}
#endif

#ifndef FOLDING_REGION_Draw


Coord crystalPos;

void crystalPlace() {
    //randomize crystalPos
    crystalPos = Coord(srnd(-20, 20), 4, srnd(-20, 20));
}
bool activated = false;
void crystalFlameInteraction() {
    if(hasCrystal) {
        flamenoanim = false;
        hasCrystal = false;
        makeflames = true;
        crystalPlace();
        activated = true;
        glout << "The fire is lit, enjoy the movie" << '\n';
    }
    else {
        if(activated) {
            makeflames = false;
            flamenoanim = true;
            activated = false;
            glout << "The fire is out" << '\n';
        }
        else{
            glout << "You need the crystal to light the fire" << '\n';
        }
    }
}

void clipAction() {
    if(activated) {
        animClip = !animClip;
        glout << "Movie " << (animClip ? "Playing" : "Paused") << '\n';
    }
    else {
        if(animClip){
            glout << "Movie stopped. You need to light the fire again" << '\n';
        } if(hasCrystal) {
            glout << "You need to light the fire first" << '\n';
        } else {
            glout << "Find the crystal first" << '\n';
        }
        animClip = false;
    }
}

void getID(int x, int y) {
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    int pixeli[3] = {(int) pixel[0], (int) pixel[1], (int) pixel[2]};

    int pixela = pixeli[0] << 16 | pixeli[1] << 8 | pixeli[2];
    switch (pixela) {

        case (255 << 16) | (0 << 8) | 0:
            crystalFlameInteraction();

        break;
        case(0 << 16) | (255 << 8) | 0:
            hasCrystal = !hasCrystal;
            glout << "Crystal " << (hasCrystal ? "Picked Up" : "Dropped") << '\n';

        break;
        case (0 << 16) | (0 << 8) | 255:
            clipAction();

        break;
        case (255 << 16) | (0 << 8) | 255:
            glout << "That's the Hat!" << '\n';
            loadSingleTextures24();
        break;
        default:
            return;
    }
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


    updateSpotlight();
    headLamp.setup();

    cam.lookAt();
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}


void finalCrystal() {
    shinyBlue.apply();
    glPushMatrix();
    glShadeModel(GL_FLAT);
    if(hasCrystal) {
        glTranslatefv(crystalPos);
        glScalef(0.2, 0.2, 0.2);
    }
    else {
        glTranslatefv(crystalPos);
        glScalef(2, 2, 2);
    }
    drawCrystal();
    glPopMatrix();
}

void drawLitShapes() {
    glEnable(GL_LIGHTING);
    sunLight.enable();
    float lightAmb[] = {0.2, 0.2, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 20.0, 0.0, 0.0}; // Position remains the same
    // // Cool global ambient light
    //
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    headLamp.enable();
    brightRed.lightswitch(makeflames);

    brightRed.enable();



    // testflat();

     glEnable(GL_LIGHT_MODEL_AMBIENT);

    finalCrystal();
    glShadeModel(GL_SMOOTH);

    testcampfire.drawBase();

    drawTexEgs();


    if(makeflames) {
        testcampfire.drawFlames();
    }
    else{
        dirtyBlend();
    }
//    floorMat.apply();




    glutSwapBuffers();
}

void drawUnlitShapes() {
    glDisable(GL_LIGHTING);
    if(showInfoViewport){
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

        if (bufferPeeking) {
            drawHiddenBuffer(crystalPos);
        }
    }


    glEnable(GL_LIGHTING);
}

void drawWindow() {
    if (showInfoViewport) {
        setupLeft();
        calculateFPS();
        drawLeft();
    }
    setupRight();

    if (selecting) {
        //
        drawHiddenBuffer(crystalPos);
        if (selecting != selectLock) {
            getID(xClick, yClick);
        }
        selecting = false;
        glutSwapBuffers();
        if (selectLock) {
            selecting = true;
            return;
        }
    }
    drawUnlitShapes();


    drawLitShapes();



    glDisable(GL_LIGHTING);

}

//folding-region draw
#endif

#ifndef FOLDING_REGION_UTILITY_AND_SETUP
// fps function

void showKeybindings() {
    glout << NOPREFIX;
    glout << CLEARALL;
    glout << MAX;
    for (const std::string &i: gameplayVec) {
        glout << i << '\n';
    }
}

//section  setupObjects() {
void setupObjects() {
    cam = Camera(Coord(17.76, 12, 17.70), Coord(17.10, 11.84, 17), Coord(0, 1, 0));
    debugXes.emplace_back(Coord(0, 0, 0), 100, 2);


    //giving them access to the debugging info map
    cam.setDebugStringAdd(&debugMap);
    // windowBlinds.setDebugStringAdd(&debugMap);

    //setup the flame
    testcampfire = Campfire(3, 10);
    testcampfire.animate();
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

    //enabling global ambient light:
    //
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    //setup the sun using a positional light:
    glEnable(GL_LIGHT_MODEL_AMBIENT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}



void setup() {
    float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 7.0, 0.0, 1.0}; // Position remains the same;

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    brightRed.setup();
    //enabling global ambient light:
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.


    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.
    glShadeModel(GL_SMOOTH);
    //check if light0 is enabled with a printStatement:
    //glListallEnabled:
    setupTextures();
    setupObjects();
    setupLights();
    if(showInfoViewport) {
        glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
    }else {
        glClearColor(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f);
    }
    crystalPos = Coord(srnd(-20, 20), 4, srnd(-20, 20));
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


#ifndef FOLDING_REGION_Control

void mouseControl(int button, int state, int x, int y) {
    //check to see if the window is focused
    if (state == GLUT_DOWN && button == GLUT_LEFT) {
        selecting = true;
        selectLock = selectLockingEnabled;
        // if selectLock, set
        glutSetCursor(selectLock ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_FULL_CROSSHAIR);
        if (!selectLock) {
            xClick = x;
            yClick = height - y;
        } //for screen vs mouse coordinates}}
        glutPostRedisplay();
    }
    if (state == GLUT_UP && button == GLUT_LEFT) {
        selectLock = false;
        //reset cursor to default:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        xClick = x;
        yClick = height - y;
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
        glutPassiveMotionFunc(nullptr);
    }
}

void keyboard(unsigned char key, int x, int y) {
    modifiers = glutGetModifiers();
    std::string hallLightState;

    switch (key) {
        case 'w': //CAMERA FORWARD
            cam.moveCamWithColl(Coord(1 * moveSpeed, 0, 0));
            break;
        case 's': //CAMERA BACKWARD
            cam.moveCamWithColl(Coord(-1 * moveSpeed, 0, 0));
            break;
        case 'a': //CAMERA LEFT
            cam.moveCamWithColl(Coord(0, 0, -1 * moveSpeed));
            break;
        case 'd': //CAMERA RIGHT
                cam.moveCamWithColl(Coord(0, 0, 1 * moveSpeed));
            break;
        case 'c': //CAMERA DOWN
            cam.moveCamWithColl(Coord(0, -1 * moveSpeed, 0));
            break;
        case 'f': //CAMERA UP
            cam.moveCamWithColl(Coord(0, 1 * moveSpeed, 0));
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
                cam.restoreState(0);
                glout << "Camera State 1 Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << "Tgt: " << cam.tgt.toString(0) << '\n';

            break;
        case '2':

                cam.restoreState(1);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';

            break;
        case '3':

                cam.restoreState(2);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';

            break;
        case '4':

                cam.restoreState(3);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';

            break;
        case '5':

                cam.restoreState(4);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';

            break;
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
        case '[':
            testcampfire.modFlames(-1);
            glout << "Removed Flame" << '\n';
            break;
        case ']':
            testcampfire.modFlames(1);
            glout << "Added Flame" << '\n';
            break;

        case '?': //print keybinds:
            showKeybindings();
            printinstructionsfromtxt();
            break;

        case 27: //Escape Key: Exit
            exit(0);
            break;
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
            glout << "Headlamp: " << (headLamp.enabled ? "On" : "Off") << '\n';
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
            makeflames = !makeflames;
            glout << "Flame Drawing: " << (makeflames ? "On" : "Off") << '\n';
            break;


        case GLUT_KEY_F7:
            if (shift) {
                makeflames = flamenoanim;
                glout << "Flame Drawing: " << (makeflames ? "On" : "Off") << '\n';
            }
            flamenoanim = !flamenoanim;
            glout << "Flame Animation: " << (!flamenoanim ? "On" : "Off") << '\n';
            break;

        case GLUT_KEY_F8:
            animClip = !animClip;
            glout << "Video " << (animClip ? "Playing" : "Paused") << '\n';
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




void menu(int id) {
    switch (id) {
        case 2: flamenoanim = !flamenoanim;
        glout << "Flame Animation " << (!flamenoanim ? "Enabled" : "Disabled") << '\n';
        break;
        case 3: animClip = !animClip;
        glout << "Video " << (animClip ? "Playing" : "Paused") << '\n';
        break;
        case 4: makeflames = !makeflames;
        glout << "Flames" <<( makeflames? "Enabled" : "Disabled") << '\n';
        break;
        case 5:
            glout << "Loading Next Video" << '\n';
        std::string nextPath = ANIMBASEDIR + std::to_string(animdirnum + 1) + "/";
        std::filesystem::exists(nextPath) && std::filesystem::is_directory(nextPath);
        animdirnum++;
        //manually free the memory
        setupTextures_24();
        break;
    }
}

void quitMenu(int id) {
    switch (id) {
        case 1: exit(0);
        break;
    }
}

void top_menu(int id) {
    if (id == 1) exit(0);
}



void makeMenu(void) {
    int menui = glutCreateMenu(menu);
    glutAddMenuEntry("Toggle Flame Animation", 2);
    glutAddMenuEntry("Toggle Flames", 3);
    glutAddMenuEntry("Video toggled", 4);
    glutAddMenuEntry("Next Video", 5);


    glutCreateMenu(top_menu);
    glutAddSubMenu("Debug Menu", menui);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
#endif




#ifndef FOLDING_REGION_ANIMATION

int peek = 0;
int numPeeks = 200;

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

//idle animation


#define ANIMSPEED 24

void flameAnimFn(int value) {
    int animPeriod =  1000 / 60;
    if(fps!= 0) {
        if (!flamenoanim && makeflames) {
            testcampfire.animate();
        }


        if(fps < animPeriod){
            animPeriod = 1000 / fps;
        }
    }
        glutTimerFunc(animPeriod, flameAnimFn, 1); // 60 times per second

    glutPostRedisplay();
}

void animateClip(int value) {
    int animPeriod = 100;
    if(fps>= 5) {
        if(animClip){
            currFrame = (currFrame + 1) % numAnimFrames;
            debugMap[60 - 25] = "Frame: " + std::to_string(currFrame);
        }else {
            if(!activated) {
                currFrame = textureMap_24["canal"];
            }
        }
    }
    glutTimerFunc(animPeriod, animateClip, 1); // ANIMSPEED times per second
    glutPostRedisplay();
}

void moveCrystal() {
    // Get the camera's position
    Coord camTarget = cam.tgt;





    crystalPos = cam.tgt;
}

void animate(int value) {

    if(hasCrystal) {
        moveCrystal();
    }
    if(bufferPeeking) {
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

    glutDisplayFunc(drawWindow);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(nullptr);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseControl);
    makeMenu();

    glutTimerFunc(1000 / 60, flameAnimFn, 1);
    glutTimerFunc(5, animate, 1);
    glutTimerFunc(1000 / ANIMSPEED, animateClip, 1);


    printinstructionsfromtxt();

    glutMainLoop();
    return 0;
}
