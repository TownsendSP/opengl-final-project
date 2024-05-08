//
// Created by tgsp on 4/9/2024.
//

# include "globals.h"

#include <chrono>
#include <iomanip>
#include <map>
#include <random>


// #define NOPREFIX 0x03
// #define PREFIX 0x05
// #define CLEARALL 0x04
// #define LINEFEED 0x06
// #define NEWLINE 0x07
// #define DEL 0x08
// #define GROW
// float conHeightPercent = 0.3;


bool GLStreamOut::parseControlChars(char in_char) {
    //cast to hex for comparison
    // unsigned char c = in_char;
    // uint_fast16_t c = xx8(in_char);

    int_fast8_t c = xx8(in_char);
    if (c == xx8(0xFF) || control == true) {
        // std::cout << "Control char: " << c << std::endl;
        switch (c) {
            case 0x00:
                control = 0;
            // std::cout << "Control off" << std::endl;
                return false;
            case 0xFF:
                control = 1;
            // std::cout << "Control on" << std::endl;
                return false;
            case 0x03:
                pref = 0;
            // std::cout << "No prefix" << std::endl;
                return false;
                break;
            case 0x04: //clear buffer and console
                // std::cout << "Clear all" << std::endl;
                buffer.clear();
                glConsoleVec.clear();
                pref = true;
                glConsoleVec.push_back("$>");
                buffer.clear();

                return false;
            case 0x05:
                // std::cout << "Prefix On" << std::endl;
                pref = 1;
                return false;
            case 0x06:
                glConsoleVec.push_back(buffer);
            // std::cout << "Linefeed" << std::endl;
                buffer.clear();
                return false;
            case 0x09:
                glConsoleVec.push_back(buffer);
                buffer.clear();
                buffer += "  ";
            // std::cout << "Carriage return" << std::endl;
                return false;
            case 0x0A:
                if (buffer.size() > 2)
                    buffer.pop_back();
                else if (glConsoleVec.size() > 1)
                    glConsoleVec.pop_back();
            // std::cout << "Delete" << std::endl;
                return false;
            case 0x7f:
                conHeightPercent += 0.1;
                if (conHeightPercent > 0.52)
                    conHeightPercent = 0.52;
            // std::cout << "Grow" << std::endl;
                return false;
            case 0x80:
                conHeightPercent -= 0.1;
                if (conHeightPercent < 0.0)
                    conHeightPercent = 0.0;
                return false;
            case 0x81:
                conHeightPercent = 0.52;
            // std::cout << "MAX" << std::endl;
                return false;
            case 0x82:
                conHeightPercent = 0.3;
            // std::cout << "Default" << std::endl;
                return false;
            default:
                return true;
        }
    } else {
        return true;
    }
}

std::streambuf::int_type GLStreamOut::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // the buffer should always start with $> to look like a cli
        if (buffer.empty() && pref) {
            buffer += "$>";
        }
        // Append the character to the buffer
        buffer += c;
        if (c == '\n') {
            glConsoleVec.push_back(buffer);
            buffer.clear();
        }
    }
    return c;
}


std::streambuf::int_type GLStreamOut::sync() {
    std::string parsedBuffer;
    for (char c: buffer) {
        // std::streambuf::int_type parsedChar = parseControlChars(c);
        if (parseControlChars(reinterpret_cast<std::streambuf::int_type &>(c)))
            parsedBuffer += c;
        // if (parsedChar != EOF) {
        //     parsedBuffer += parsedChar;
        // }
    } //std::cout << "after parsing: " << parsedBuffer << "before: " << buffer << std::endl;
    if (!parsedBuffer.empty()) {
        //add the shell prompt to parsedBuffer
        glConsoleVec.push_back(">$" + parsedBuffer);
        parsedBuffer.clear();
    }
    buffer.clear();
    return 0;
}

void GLStreamOut::rm(int n) {
    glConsoleVec.erase(glConsoleVec.begin() + n);
}

GLStreamOut glConsole;
std::ostream glout(&glConsole);

std::vector<thingHolder> staticPoints;

void addDbgPt(int idx, float xyz[3], float size, float weight) {
    return;
    // I think this may be causing the memory leak, happening @ ~2MB/s.
    //This is the only thing i can think of that's actually creating data every frame,
    //I was careful to destroy or overwrite all other data logging
    staticPoints.insert(staticPoints.begin() + idx, thingHolder(xyz[0], xyz[1], xyz[2], size, weight));

}

#ifndef FOLDING_REGION_WHY_CPLUSPLUS_WHY
int_fast8_t xx8(char input) {
    return static_cast<int_fast8_t>(input);
}

#endif


int8_t doorOpenPercent = 0;

int useTimeToSeedRandomToSetWinner() {
    std::random_device rd;
    srand(rd());
    int winnera = rand() % 2;
    return winnera;
}



//mapping the state to strings for printing

std::map<int, std::string> dbgNormMap = {
    {DBG_NORM_OFF, "None"},
    {ABS_NORM, "Abs(Norm)"},
    {RAW_NORM, "Scaled"},
    {POS_NORM, "Positive Only"},
    {NEG_NORM, "Negative Only"}
};

int dbgNormals = 0;
uint8_t enabledFaces = 0b00111111;

// this is what enables the normals visualization
void glNormal3fvd(float whyAreMyNormalsBroken[3]) {
    GLfloat normToColors[3];
    float debugWhyAreMyNormalsBroken[3]  = {whyAreMyNormalsBroken[0], whyAreMyNormalsBroken[1], whyAreMyNormalsBroken[2]};
    switch (dbgNormals) {
        case 0:
            glNormal3fv(whyAreMyNormalsBroken);
            break;
        case 1:
            normToColors[0] = abs(whyAreMyNormalsBroken[0]);
            normToColors[1] = abs(whyAreMyNormalsBroken[1]);
            normToColors[2] = abs(whyAreMyNormalsBroken[2]);
            glDisable(GL_LIGHTING);
            break;
        case 2:
            normToColors[0] = (whyAreMyNormalsBroken[0] + 1) / 2;
            normToColors[1] = (whyAreMyNormalsBroken[1] + 1) / 2;
            normToColors[2] = (whyAreMyNormalsBroken[2] + 1) / 2;
            glDisable(GL_LIGHTING);
            break;
        case 3: //just get rid of any values below 0
            normToColors[0] = whyAreMyNormalsBroken[0] > 0 ? whyAreMyNormalsBroken[0] : 0;
            normToColors[1] = whyAreMyNormalsBroken[1] > 0 ? whyAreMyNormalsBroken[1] : 0;
            normToColors[2] = whyAreMyNormalsBroken[2] > 0 ? whyAreMyNormalsBroken[2] : 0;
            glDisable(GL_LIGHTING);
            break;
        case 4: //just get rid of any values above 0
            normToColors[0] = whyAreMyNormalsBroken[0] < 0 ? -1*whyAreMyNormalsBroken[0] : 0;
            normToColors[1] = whyAreMyNormalsBroken[1] < 0 ? -1*whyAreMyNormalsBroken[1] : 0;
            normToColors[2] = whyAreMyNormalsBroken[2] < 0 ? -1*whyAreMyNormalsBroken[2] : 0;
            glDisable(GL_LIGHTING);
            break;
        default:
            glNormal3fv(whyAreMyNormalsBroken);
            break;
    }
    if (dbgNormals != 0) {
        glColor3fv(normToColors);
    }
}

bool selecting = false;
int xClick;
int yClick;

float globAmb[4] = {0.2, 0.2, 0.2, 1.0};

float hallBnlF[3] = {0, 0, -2};
float halltfrF[3] = {10, 4, 2};

//Coord roomBnl = Coord(halltfrF[0], 0, 3*hallBnlF[2]);
float roomBnlF[3] = {10, 0, -6};
float roomtfrF[3] = {20, 4, 6};
bool useCollision = true;
float moveSpeed = 0.5f;
float cardDist;

bool bufferPeeking = false;
bool selectLock = false;
bool selectLockingEnabled = false;
bool hasCrystal = false;

unsigned int texture_24[NUMTEXTURES_24];
unsigned int texture_32[NUMTEXTURES_32];



unsigned int numAnimFrames;
unsigned int currFrame;

std::map<std::string, int> textureMap_24;
std::map<std::string, int> textureMap_32;

bool showInfoViewport = true;

bool makeflames = false;
bool flamenoanim = true;

int animdirnum = 0;