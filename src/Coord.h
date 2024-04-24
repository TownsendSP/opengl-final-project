#ifndef COORD_H
#define COORD_H

//imports
#include "globals.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>




float srnd(float min, float max);


inline float clmp(float val, float min, float max) {
    return std::fmax(min, std::fmin(val, max));
}

//simple function to scale a value from one range to another
inline float clscl(float val, float min, float max) {
    return (val - min) / (max - min);
}

float srndi(float min, float max);


class Coord {

public:
    float X, Y, Z;

    //constructor
    Coord(float x, float y, float z) {
        X = x;
        Y = y;
        Z = z;
    }

    Coord(std::string hexString);

    std::string toHexString(char delim = ',');

    explicit Coord(const float* in) {
    X = in[0];
    Y = in[1];
    Z = in[2];
}

    //default:
    Coord() {
        X = 0;
        Y = 0;
        Z = 0;
    }


    //mathy stuff
#ifndef FOLDING_COORDMATH
    Coord operator+(const Coord &other) const {
        return {X + other.X, Y + other.Y, Z + other.Z};
    }

    Coord operator+(const float other) const {
        return {X + other, Y + other, Z + other};
    }

    [[nodiscard]] float sum() const {
        return X + Y + Z;
    }

    [[nodiscard]] Coord clamp(float min, float max) const {
        return {clmp(X, min, max),
                clmp(Y, min, max),
                clmp(Z, min, max)};
    }

    [[nodiscard]] Coord clamp(Coord min, Coord max) const {
        return {clmp(X, min.X, max.X),
                clmp(Y, min.Y, max.Y),
                clmp(Z, min.Z, max.Z)};
    }

    [[nodiscard]] Coord scale(float min, float max) const {
        return {clscl(X, min, max),
                clscl(Y, min, max),
                clscl(Z, min, max)};
    }

    [[nodiscard]] Coord scale(Coord min, Coord max) const {
        return {clscl(X, min.X, max.X),
                clscl(Y, min.Y, max.Y),
                clscl(Z, min.Z, max.Z)};
    }

    Coord operator-(const Coord &other) const {
        return {X - other.X, Y - other.Y, Z - other.Z};
    }

    // Coord operator*(const int lhs, const Coord & rhs) {
    //     return {rhs.X * lhs, lhs * rhs.Y, lhs * rhs.Z};
    // }

    Coord operator-(const float other) const {
        return {X - other, Y - other, Z - other};
    }

    Coord operator*(const Coord &other) const {
        return {X * other.X, Y * other.Y, Z * other.Z};
    }

    Coord operator*(const float other) const {
        return {X * other, Y * other, Z * other};
    }

    Coord operator/(const Coord &other) const {
        if (other.X != 0 && other.Y != 0 && other.Z != 0) {
            return {X / other.X, Y / other.Y, Z / other.Z};
        } else {
            return {};
        }
    }

    Coord operator/(const float other) const {
        if (other != 0) {
            return {X / other, Y / other, Z / other};
        } else {
            return {};
        }
    }

    Coord operator&(const int i) const {
        return Coord(i & 4 ? X : 0, i & 2 ? Y : 0, i & 1 ? Z : 0); // 4 = 100, 2 = 010, 1 = 001
    }

    Coord operator|(const Coord i) const {
        return {X == 0 ? i.X : X, Y == 0 ? i.Y : Y, Z == 0 ? i.Z : Z};
    }

    Coord operator|(const float i) const {
        return {X == 0 ? i : X, Y == 0 ? i : Y, Z == 0 ? i : Z};
    }

    bool operator==(const Coord &other) const {
        return X == other.X && Y == other.Y && Z == other.Z;
    }


    [[nodiscard]] Coord pow(const float exponent) const {
        return {std::pow(X, exponent), std::pow(Y, exponent), std::pow(Z, exponent)};
    }

    [[nodiscard]] Coord pow(const Coord &other) const {
        return {std::pow(X, other.X), std::pow(Y, other.Y), std::pow(Z, other.Z)};
    }

    [[nodiscard]] float dist(const Coord &other) const {
        return std::sqrt(
                std::abs(X - other.X) * std::abs(X - other.X) +
                std::abs(Y - other.Y) * std::abs(Y - other.Y) +
                std::abs(Z - other.Z) * std::abs(Z - other.Z));
    }
    //swapping will be done by overloading rightshift

    Coord operator>>(const uint8_t m) {
        return(
            (m==XZ) ? Coord(Z, Y, X) :
            (m==YZ) ? Coord(X, Z, Y) :
            (m==XY) ? Coord(X, Y, Z) : *this
            );
    };



#endif

    Coord wrap(Coord maxes, Coord mins, Coord add);

    Coord wrap(float maxe, float min, Coord add);

    operator float* () const {
        return new float[3]{X, Y, Z};
    }



    Coord vecToAngles() const {
        return {atan2(Z, X), asin(Y), 0};

    }

    Coord radiansToDegrees() const;

    Coord degToRad();

    Coord dirVecToRad() const;

    Coord radiansToDirVec() const;

    std::string *toStrings(int precision) const;

    std::string toString(int precision = 2);
    std::string* toStrings(int precision = 2);

    void debug(int idx, float s = 0.5, float w = 0.5);

};

inline Coord randCoord(const Coord &min, const Coord &max) {
    return {srnd(std::min(min.X, max.X), std::max(min.X, max.X)),
            srnd(std::min(min.Y, max.Y), std::max(min.Y, max.Y)),
            srnd(std::min(min.Z, max.Z), std::max(min.Z, max.Z))};
}

float dist(Coord point1, Coord point2);

std::vector<Coord> genRandNonCoLinearCord(int numCoords, float permissibleCloseness, Coord min, Coord max);

Coord genRandNonCoLinearCord(std::vector<Coord> coords, float permissibleCloseness, Coord min, Coord max);
bool isColorCloseEnough(Coord color1, Coord color2, float closeness);
#endif //COORD_H


