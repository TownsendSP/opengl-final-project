

#include "Camera.h"
#include "globals.h"

//#include "bee.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>

#include "testingFunctions.h"


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Camera::Camera(float position[3], float target[3], float orientation[3], float transSpeed, float rot_sens) {

    pos = Coord(position[0], position[1], position[2]);
    tgt = Coord(target[0], target[1], target[2]);
    up = Coord(orientation[0], orientation[1], orientation[2]);
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll

    transSpeed = transSpeed;
}

Camera::Camera(Coord Pos, Coord Target, Coord orientation, float transSpeed, float rot_sens) {
    pos = Pos;
    tgt = Target;
    up = orientation;
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll
}

Camera::Camera(Coord Pos, Coord Target, Coord orientation) {
    pos = Pos;
    tgt = Target;
    up = orientation;
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll
}

Coord Camera::normDirVec() {
    return (tgt - pos) / sqrt((tgt - pos).pow(2).sum());
}

int Camera::saveToFile(const std::string& filename) {
    // Open the file for output using the filename
    std::ofstream file(filename);

    if (!file.is_open()) {
        glout << "Error: File is not open" << std::endl;
        std::cerr << "Error: File is not open" << std::endl;
        return -1;
    }
    for (std::tuple<Coord, Coord> state: storedStates) {
        file << std::get<0>(state).toHexString() << "," << std::get<1>(state).toHexString() << std::endl;
    }

    if (file.fail()) {
        glout << "Error: Failed to save camera" << std::endl;
        std::cerr << "Error: Failed to save camera" << std::endl;
        return -2;
    }
    file.close();
    return 0;
}

std::vector<std::string> readFileLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return lines;
    }
    std::string lineForWriting;
    while (std::getline(file, lineForWriting)) {
        lines.push_back(lineForWriting);
    }

    file.close();
    return lines;
}

std::vector<float> splitAndConvertToFloats(const std::string &input, char delimiter) {
    std::vector<float> floats;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        // Convert hex string to float
        double d = std::strtod(token.c_str(), nullptr);
        floats.push_back(static_cast<float>(d));
    }

    return floats;
}

int Camera::loadFromFile(const std::string& filename) {
    std::vector<std::string> lines = readFileLines(filename);
    std::vector<float> floats;
    int i = 0;
    for (const auto& lineForIteration : lines) {
        floats = splitAndConvertToFloats(lineForIteration, ',');
        storedStates[i] = std::make_tuple(Coord(floats[0], floats[1], floats[2]), Coord(floats[3], floats[4], floats[5]));
        // glout << "Loaded state " << i << " as " << std::get<0>(storedStates[i]).toString() << ", " << std::get<1>(storedStates[i]).toString() << std::endl;
        i++;
    }
    return 0;
}


void Camera::setPitchYaw() {
    Coord dirVec = (tgt - pos);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    ang = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
}

Coord calcPitchYaw(Coord position, Coord target) {
    Coord dirVec = (target - position);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    Coord rv = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
    return rv;
}

Coord calcTarget(Coord position, Coord pitchYaw) {
    float dirX = cos(pitchYaw.X) * cos(pitchYaw.Y);
    float dirY = sin(pitchYaw.Y);
    float dirZ = sin(pitchYaw.X) * cos(pitchYaw.Y);
    return position + Coord(dirX, dirY, dirZ);
}



Coord roomBottomNearLeft = Coord(roomBnlF);
Coord roomTopFarRight = Coord(roomtfrF);
Coord hallBottomNearLeft = Coord(hallBnlF);
Coord hallTopFarRight = Coord(halltfrF);
//collision:

//XZ Collision:





int Camera::whichSpace(Coord posi) {
    if (posi.X > roomBottomNearLeft.X+moveSpeed/2 && posi.X < roomTopFarRight.X &&
        posi.Y > roomBottomNearLeft.Y && posi.Y < roomTopFarRight.Y &&
        posi.Z > roomBottomNearLeft.Z && posi.Z < roomTopFarRight.Z) {

        whichPlace = IN_ROOM;
    }
    if (posi.X > hallBottomNearLeft.X && posi.X < hallTopFarRight.X-moveSpeed/2 &&
        posi.Y > hallBottomNearLeft.Y && posi.Y < hallTopFarRight.Y &&
        posi.Z > hallBottomNearLeft.Z && posi.Z < hallTopFarRight.Z) {
        whichPlace = IN_HALL;
    }
    std::string place = whichPlace == IN_ROOM ? "Room" : "Hall";
    debugMap[60-21] = "Location: " + place;
    return whichPlace;
}

int Camera::collisionYZ(Coord pos) {

    float roomTfrfH[3] = {roomTopFarRight.X, roomTopFarRight.Y, roomTopFarRight.Z};
    float roomBnlfH[3] = {roomBottomNearLeft.X, roomBottomNearLeft.Y, roomBottomNearLeft.Z};
    float hallTfrfH[3] = {hallTopFarRight.X, hallTopFarRight.Y, hallTopFarRight.Z};
    float hallBnlfH[3] = {hallBottomNearLeft.X, hallBottomNearLeft.Y, hallBottomNearLeft.Z};
    //min X check for hall:
    int coll = whichSpace(pos); //1 means in hall
    if (pos.X < hallBottomNearLeft.X) {
        debugMap[60-22] = "Near YZ collision";
        // glout << "Near YZ collision" << std::endl;
        return COLL_NEAR_YZ;
    }
    float  rttfr= roomTopFarRight.X;
    double rtfrExtend = rttfr;
    bool doorWalkthrough = pos.X >= hallTopFarRight.X-moveSpeed/2;
    bool doorOpen = doorOpenPercent < 90;
    bool inHall = coll == IN_HALL;
    bool inRm = coll == IN_ROOM;
    bool leftXRoom = pos.X < roomBottomNearLeft.X;
    bool inHallByZ = hallTopFarRight.Z > pos.Z > hallBottomNearLeft.Z;

    if ((inHall && doorWalkthrough && doorOpen) ||
        (inRm && doorOpen && (leftXRoom && inHallByZ) )) {
        debugMap[60-22] = "Door collision";

            return COLL_DOOR;
    }
    bool throughFarWall = pos.X > rttfr;

    if (throughFarWall) {

        debugMap[60-22] = "Far YZ collision";
        return COLL_FAR_YZ;
    }

    debugMap[60-22] = "No YZ collision";
    return COLL_NONE_YZ;
}

int Camera::collisionYX(Coord pos) {
    if (whichPlace == IN_HALL){
        if(hallTopFarRight.Z < pos.Z) {

            debugMap[60-23] = "Hall Right YX collision";
            return COLL_HALL_RYX;
        }
        if(hallBottomNearLeft.Z > pos.Z) {

            debugMap[60-23] = "Hall Left YX collision";
            return COLL_HALL_LYX;
        }
    }if(whichPlace == IN_ROOM){

        if(roomTopFarRight.Z < pos.Z) {

            debugMap[60-23] = "Room Right YX collision";
            return COLL_ROOM_RYX;
        }
        if(roomBottomNearLeft.Z > pos.Z) {

            debugMap[60-23] = "Room Left YX collision";
            return COLL_ROOM_LYX;
        }
    }
    debugMap[60-23] = "No YX collision";
    return COLL_NONE_YX;
}

int Camera::collisionXZ(Coord pos) {
    if (pos.Y > roomTopFarRight.Y) {
        debugMap[60-24] = "Up XZ collision";
        return COLL_UP_XZ;
    }
    if (pos.Y < roomBottomNearLeft.Y) {
        debugMap[60-24] = "Down XZ collision";
        return COLL_DOWN_XZ;
    }
    debugMap[60-24] = "No XZ collision";
    return COLL_NONE_XZ;
}


void Camera::getRelTrans(Coord deltaTranslation) {
    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);

    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;

    pos = pos + transVec;
    tgt = tgt + transVec;
}


void Camera::relTrans(Coord deltaTranslation) {

    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);

    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;

    pos = pos + transVec;
    tgt = tgt + transVec;
}

void Camera::relRot(Coord deltaAngle) {
    Coord tAng = ang + deltaAngle;
    //begin Pitch clamping to ensure no backflips or frontflips
    float a = PI / 2.0f;
    float b = tAng.X;
    float c = std::min(a, b);
    float d = -PI / 2.0f;
    tAng.X = std::max(d, c);
    //end Pitch clamping

    tAng.Y = fmod(tAng.Y, 2 * PI);
    if (tAng.Y < 0) {
        tAng.Y += 2 * PI;
    }

    ang = tAng;
    Coord dirVec = Coord(cos(tAng.Y) * cos(tAng.X), sin(tAng.X), sin(tAng.Y) * cos(tAng.X));
    tgt = pos + dirVec;
}

void Camera::updateCardInfo() {
    cardDist =pos.dist(Coord(18.6000004, 1.84333336, 0.25));

}

void Camera::moveCamWithColl(Coord deltaTranslation) {
    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);

    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;
    Coord a = pos + transVec;
    if(useCollision) {
        float distance = sqrt(a.X*a.X + a.Z*a.Z);

        // If the new position is within 10 units of (0,0), prevent the translation towards (0,0)
        if(distance < 10.0f) {
            if(a.X < pos.X) {
                transVec.X = 0;
            }
            if(a.Z < pos.Z) {
                transVec.Z = 0;
            }
        }
    }
    pos = pos + transVec;
    tgt = tgt + transVec;


    // check distance to card:

    updateCardInfo();
}



void Camera::lookAt(DebugLevel dbg) {
    gluLookAt(pos.X, pos.Y, pos.Z, tgt.X, tgt.Y, tgt.Z, up.X, up.Y, up.Z);
        std::vector<std::string> debugToAdd = toString(2);
        for (unsigned int i = 0; i < debugToAdd.size(); i++) {
            (*debug_string_add_map_)[i + 45] = debugToAdd[i];
        }
}

void Camera::useDebugStringAdd(int lineInt, std::string message) {
    (*debug_string_add_map_)[lineInt] = message;
}

std::vector<std::string> Camera::toString(int prec) const {
    std::vector<std::string> retVal;
    std::string* p = pos.toStrings(prec);
    std::string* t = tgt.toStrings(prec);
    std::string* u = up.toStrings(prec);
    std::string* a = ang.toStrings(prec);
    std::string* d = Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X)).toStrings(prec);

    retVal.emplace_back(std::string("DirVec: X " + d[0] + ", Y " + d[1] + ", Z " + d[2]));
    retVal.emplace_back(std::string("Agnle: Pitch " + a[0] + ", Yaw " + a[1] + ", Roll " + a[2]));
    retVal.emplace_back(std::string("Orientation X: " + u[0] + ", Y: " + u[1] + ", Z: " + u[2]));
    retVal.emplace_back(std::string("Target X: " + t[0] + ", Y: " + t[1] + ", Z: " + t[2]));
    retVal.emplace_back(std::string("Pos X: " + p[0] + ", Y: " + p[1] + ", Z: " + p[2]));
    retVal.emplace_back("          Camera Info");
    return retVal;
}

