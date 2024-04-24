#include "globals.h"
#include <cmath>

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif

#include "Coord.h"

#include <cmath>

#define DEBUG

#include <algorithm>
#include <iostream>
#include <random>
#include <iomanip>
#include <map>
#include <sstream>

void mapInsert(std::map<int, std::string>* mapthing, std::vector<std::string> vec, int indexInsert) {
    for (int i = 0; i < vec.size(); i++) {
        mapthing->insert(std::pair<int, std::string>(indexInsert + i, vec[i]));
    }

}

std::string trunc(float fl, int prec) {
    char buffer[50];
    std::snprintf(buffer, sizeof(buffer), "%.*f", prec, fl);
    return buffer;
}



float dist(Coord point1, Coord point2) {
    float distX = std::abs(point1.X - point2.X);
    float distY = std::abs(point1.Y - point2.Y);
    float distZ = std::abs(point1.Z - point2.Z);

    return std::sqrt(distX * distX + distY * distY + distZ * distZ);
}

float srnd(float min, float max) {
    if (min > max) {
        std::swap(min, max);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

float srndi(float min, float max) {
    return static_cast<int>(srnd(min, max + 1));
}

Coord::Coord(std::string hexString) {
    std::stringstream Xstr (hexString.substr(0, 20));
    std::stringstream Ystr (hexString.substr(20, 20));
    std::stringstream Zstr (hexString.substr(40, 20));
    // float outX, outY, outZ;
    Xstr >> std::hexfloat >> X;
    Ystr >> std::hexfloat >> Y;
    Zstr >> std::hexfloat >> Z;
}

std::string Coord::toHexString(char delim) { //remember, comma-delimited
    std::stringstream stream;
    stream << std::hexfloat << X << delim << std::hexfloat << Y << delim << std::hexfloat << Z;
    return stream.str();
}

float randomNaiveNotSeeded(float min, float max) {
    float randthing = min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
// #ifdef DEBUG
//   std::cout << "randthing: " << randthing << std::endl;
// #endif

    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}

std::vector<Coord> genRandNonCoLinearCord(int numCoords, float permissibleCloseness, Coord min, Coord max) {
    std::vector<Coord> coords;
    for (int i = 0; i < numCoords; i++) {
        Coord tempCoord;
        tempCoord = randCoord(min, max);
        bool isCoLinear = false;
        for (auto &coord: coords) {
            if (tempCoord.dist(coord) < permissibleCloseness) {
                isCoLinear = true;
                break;
            }
        }
        if (!isCoLinear) {
            coords.push_back(tempCoord);
        }
    }
    return coords;
}

Coord Coord::wrap(Coord maxes, Coord mins, Coord add) {
    Coord result = *this + add;

    if (result.X > maxes.X) {
        result.X = mins.X + (result.X - maxes.X);
    } else if (result.X < mins.X) {
        result.X = maxes.X - (mins.X - result.X);
    }

    if (result.Y > maxes.Y) {
        result.Y = mins.Y + (result.Y - maxes.Y);
    } else if (result.Y < mins.Y) {
        result.Y = maxes.Y - (mins.Y - result.Y);
    }

    if (result.Z > maxes.Z) {
        result.Z = mins.Z + (result.Z - maxes.Z);
    } else if (result.Z < mins.Z) {
        result.Z = maxes.Z - (mins.Z - result.Z);
    }

    return result;
}

Coord Coord::wrap(float maxe, float min, Coord add) {
    Coord result = *this + add;

    if (result.X > maxe) {
        result.X = min + (result.X - maxe);
    } else if (result.X < min) {
        result.X = maxe - (min - result.X);
    }

    if (result.Y > maxe) {
        result.Y = min + (result.Y - maxe);
    } else if (result.Y < min) {
        result.Y = maxe - (min - result.Y);
    }

    if (result.Z > maxe) {
        result.Z = min + (result.Z - maxe);
    } else if (result.Z < min) {
        result.Z = maxe - (min - result.Z);
    }

    return result;
}

Coord Coord::radiansToDegrees() const {
    float herePi = 3.14159265358979323846;
        return {(X * 180 / herePi),(Y * 180 / herePi),(Z * 180 / herePi)};

}

Coord Coord::degToRad() {
    float herePi = 3.14159265358979323846;
    return {(X * herePi / 180),(Y * herePi / 180),(Z * herePi / 180)};



}

Coord Coord::dirVecToRad() const {
        float pitch = atan2(Y, Z);
        float yaw = atan2(X, Z);
        return Coord(pitch, yaw, 0);
}

Coord Coord::radiansToDirVec() const {
    float pitch = X;
    float yaw = Y;
    return Coord(sin(pitch) * cos(yaw), sin(pitch) * sin(yaw), cos(pitch));
}


std::string* Coord::toStrings(int precision) const{
      std::string* ret = new std::string[3];
    ret[0] = trunc(X, precision);
    ret[1] = trunc(Y, precision);
    ret[2] = trunc(Z, precision);
    return ret;
}

std::string* Coord::toStrings(int precision) {
    std::string* ret = new std::string[3];
    ret[0] = trunc(X, precision);
    ret[1] = trunc(Y, precision);
    ret[2] = trunc(Z, precision);
    return ret;
}

void Coord::debug(int idx, float s, float w) {
    addDbgPt(idx, new float[3]{X, Y, Z}, s, w);
}



std::string Coord::toString(int precision) {
    std::string* things = toStrings(precision);
    std::string str = "(" + things[0] + ", " + things[1] + ", " + things[2] + ")";
    return str;
}


Coord genRandNonCoLinearCord(std::vector<Coord> coordsIn, float permissibleCloseness, Coord min, Coord max) {
    std::vector<Coord> coords = coordsIn;;
    for (int i = 0; i < 1; i++) {
        Coord tempCoord;
        tempCoord = randCoord(min, max);
        bool isCoLinear = false;
        for (auto &coord: coords) {
            if (tempCoord.dist(coord) < permissibleCloseness) {
                isCoLinear = true;
                break;
            }
        }
        if (!isCoLinear) {
            return tempCoord;
        }
    }
    return randCoord(min, max);

}



bool isColorCloseEnough(Coord color1, Coord color2, float closeness) {
    if (abs(color1.X - color2.X) < closeness &&
        abs(color1.Y - color2.Y) < closeness &&
        abs(color1.Z - color2.Z) < closeness) {
        return true;
        }
    return false;
}




