//
// Created by tgsp on 4/9/2024.
//

# include "globals.h"

#include <chrono>
#include <iomanip>
#include <iostream>
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

const std::vector<std::string> &GLStreamOut::getConsole() const {
    return glConsoleVec;
}

void GLStreamOut::rm(int n) {
    glConsoleVec.erase(glConsoleVec.begin() + n);
}

GLStreamOut glConsole;
std::ostream glout(&glConsole);


std::streambuf::int_type GLInfo::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // Append the character to the buffer
        buffer += c;

        // If the character is a newline, interpret buffer [0] and [1] as a 2-digit number. store that number to int[idx]
        if (c == '\n') {
            int idx = buffer[0] - '0'; // convert char to int
            idx *= 10;
            idx += buffer[1] - '0'; // convert char to int
            glInfoMap[idx] = buffer.substr(2);
            buffer.clear();
        }
    }
    return c;
}

std::streambuf::int_type GLInfo::sync() {
    if (!buffer.empty()) {
        int idx = buffer[0] - '0'; // convert char to int
        idx *= 10;
        idx += buffer[1] - '0'; // convert char to int
        glInfoMap[idx] = buffer.substr(2);
        buffer.clear();
    }
    return 0;
}

const std::map<int, std::string> &GLInfo::getInfo() const {
    return glInfoMap;
}

GLInfo glStatus;
std::ostream glInfoOut(&glStatus);


std::vector<thingHolder> staticPoints;

void addDbgPt(int idx, float xyz[3], float size, float weight) {
    return;
    // I think this may be causing the memory leak, happening @ ~2MB/s.
    //This is the only thing i can think of that's actually creating data every frame,
    //I was careful to destroy or overwrite all other data logging
    staticPoints.insert(staticPoints.begin() + idx, thingHolder(xyz[0], xyz[1], xyz[2], size, weight));

}

std::vector<thingHolder> getDbgPts() {
    return staticPoints;
}

float *getDbgPts(int which) {
    // if which is in the range of the vector, return it, otherwise return the first element
    if (which >= 0 && which < staticPoints.size()) {
        return staticPoints[which];
    } else {
        return staticPoints[0];
    }
}

int getNextPoint(int current) {
    // simply returns the index of the next point, unless the next point would be outside the bounds of the vector, in which case it returns 0
    return current + 1 < staticPoints.size() ? current + 1 : 0;
}

//I did write these, c++ was annoying me my not letting me have the hex
#ifndef FOLDING_REGION_WHY_CPLUSPLUS_WHY
int_fast8_t xx8(char input) {
    return static_cast<int_fast8_t>(input);
}

char xd8(int_fast8_t input) {
    return static_cast<char>(input);
}

int_fast16_t xx16(int input) {
    return static_cast<int_fast16_t>(input);
}

short xd16(int_fast16_t input) {
    return static_cast<short>(input);
}

int_fast32_t xx32(long input) {
    return static_cast<int_fast32_t>(input);
}

long xd32(int_fast32_t input) {
    return static_cast<long>(input);
}

int_fast64_t xx64(long input) {
    return static_cast<int_fast64_t>(input);
}

long xd64(int_fast64_t input) {
    return static_cast<long>(input);
}

std::string xs8(int_fast8_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(value);
    return ss.str();
}

std::string xs16(int_fast16_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(value);
    return ss.str();
}

std::string xs32(int_fast32_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << static_cast<int>(value);
    return ss.str();
}

std::string xs64(int_fast64_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << static_cast<int>(value);
    return ss.str();
}


#endif


int8_t doorOpenPercent = 0;


int8_t animateDoor = 3;


int winner;

std::string getUName() {
    const char *username;

#ifdef _WIN32
    username = std::getenv("USERNAME");
#else
    username = std::getenv("USER");
#endif

    if (username != nullptr) {
        return username;
    } else {
        return "Unable to get username.";
    }
}

std::string getDayOfWeek() {
    std::vector<std::string> daysOfWeek = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&timePoint);

    return daysOfWeek[tm.tm_wday];
}

int useTimeToSeedRandomToSetWinner() {
    std::random_device rd;
    srand(rd());
    int winnera = rand() % 2;
    return winnera;
}

std::string retWinner() {
    if (winner) {
        return (getUName());
    } else {
        return (getDayOfWeek());
    }
}



//mapping the state to strings for printing

std::map<int, std::string> dbgNormMap = {
    {DBG_NORM_OFF, "None"},
    {ABS_NORM, "Abs(Norm)"},
    {RAW_NORM, "Scaled"},
    {POS_NORM, "Positive Only"},
    {NEG_NORM, "Negative Only"}
};


int nextDbgState() {
    if (dbgNormals == NEG_NORM) {
        return DBG_NORM_OFF;
    } else {
        return dbgNormals + 1;
    }
}

int cardRotState = CARD_ROT_NONE;
int cardRotPercent = 0; //out of 100, but will be scaled
int cardRotSpeed = 1;
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
bool windowFocused = false;
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