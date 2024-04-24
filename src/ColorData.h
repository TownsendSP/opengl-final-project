//
// Created by tgsp on 4/4/2024.
//

#ifndef COLORDATA_H
#define COLORDATA_H

#include "globals.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Coord.h"




class ColorData {
public:
    float R, G, B, A;
    float *X = &R;
    float *Y = &G;
    float *Z = &B;
    float *W = &A;


    ColorData(float r, float g, float b, float a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    // ColorData(int r, int g, int b, float a, uint8_t conv = NOCONVERT) {
    //         R = conv?(r/ 255.0):r;
    //         G = conv?(g/ 255.0):g;
    //         B = conv?(b/ 255.0):b;
    //         A = a;
    // }
    ColorData(uint32_t hexColor, float a) {
        R = ((hexColor >> 16) & 0xFF) / 255.0;
        G = ((hexColor >> 8) & 0xFF) / 255.0;
        B = (hexColor & 0xFF) / 255.0;
        A = a;
    }

    ColorData(const float in[4]) {
        R = in[0];
        G = in[1];
        B = in[2];
        A = in[3];
    }

    //default:
    ColorData() {
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 0.0f;
    }

    ColorData(bool a) {
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 0.0f;
    }

    ColorData(GLdouble r, GLdouble g, GLdouble b, GLdouble a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    ColorData(Coord qq, float a) {
        R = qq.X;
        G = qq.Y;
        B = qq.Z;
        A = a;
    }

#ifndef ARITHMETIC_OPERATORS

    ColorData operator+(const ColorData &other) const {
        return {R + other.R, G + other.G, B + other.B, A + other.A};
    }

    ColorData operator+(const float other) const {
        return {R + other, G + other, B + other, A + other};
    }

    ColorData operator-(const ColorData &other) const {
        return {R - other.R, G - other.G, B - other.B, A - other.A};
    }

    ColorData operator-(const float other) const {
        return {R - other, G - other, B - other, A - other};
    }

    ColorData operator*(const ColorData &other) const {
        return {R * other.R, G * other.G, B * other.B, A * other.A};
    }

    ColorData operator*(const float other) const {
        return {R * other, G * other, B * other, A * other};
    }

    ColorData operator/(const ColorData &other) const {
        if (other.R != 0 && other.G != 0 && other.B != 0 && other.A != 0) {
            return {R / other.R, G / other.G, B / other.B, A / other.A};
        } else {
            return {};
        }
    }

    ColorData operator/(const float other) const {
        if (other != 0) {
            return {R / other, G / other, B / other, A / other};
        } else {
            return {};
        }
    }
#endif


    //<editor-fold desc="Bitwise">
    ColorData operator&(const int i) const {
        return ColorData(i & 8 ? R : 0, i & 4 ? G : 0, i & 2 ? B : 0,
                         i & 1 ? A : 0); // 8 = 1000, 4 = 0100, 2 = 0010, 1 = 0001
    }

    ColorData operator|(const ColorData i) const {
        return {R == 0 ? i.R : R, G == 0 ? i.G : G, B == 0 ? i.B : B, A == 0 ? i.A : A};
    }

    ColorData operator|(const float i) const {
        return {R == 0 ? i : R, G == 0 ? i : G, B == 0 ? i : B, A == 0 ? i : A};
    }

    //</editor-fold>

    //otherMath:
    ColorData clamp(float min, float max) const {
        return {
            clmp(R, min, max),
            clmp(G, min, max),
            clmp(B, min, max),
            clmp(A, min, max)
        };
    }

    ColorData clamp(ColorData min, ColorData max) const {
        return {
            clmp(R, min.R, max.R),
            clmp(G, min.G, max.G),
            clmp(B, min.B, max.B),
            clmp(A, min.A, max.A)
        };
    }

    ColorData scale(float min, float max) const {
        return {
            clscl(R, min, max),
            clscl(G, min, max),
            clscl(B, min, max),
            clscl(A, min, max)
        };
    }

    ColorData scale(ColorData min, ColorData max) const {
        return {
            clscl(R, min.R, max.R),
            clscl(G, min.G, max.G),
            clscl(B, min.B, max.B),
            clscl(A, min.A, max.A)
        };
    }

    ColorData pow(const float exponent) const {
        return {std::pow(R, exponent), std::pow(G, exponent), std::pow(B, exponent), A};
    }

    ColorData pow(const ColorData &other) const {
        return {std::pow(R, other.R), std::pow(G, other.G), std::pow(B, other.B), A};
    }

    float dist(const ColorData &other) const {
        return std::sqrt(
            std::abs(R - other.R) * std::abs(R - other.R) +
            std::abs(G - other.G) * std::abs(G - other.G) +
            std::abs(B - other.B) * std::abs(B - other.B));
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << std::showpoint;
        oss << "(" << R << ", " << G << ", " << B << ", " << A << ")";
        std::string str = oss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    }


    bool operator==(const ColorData &other) const {
        return R == other.R && G == other.G && B == other.B && A == other.A;
    }

    ColorData scale255() const {
        return {R * 255, G * 255, B * 255, A};
    }

    ColorData scale1() const {
        return {R / 255, G / 255, B / 255, A};
    }

    Coord toCoord() const {
        return {R, G, B};
    }

    //toArray
    float *toArray() {
        return new float[4]{R, G, B, A};
    }

    GLfloat *toGLfloatArray() {
        return new GLfloat[4]{R, G, B, A};
    }

    // operator GLfloat*() const {
    //     return new GLfloat[4]{R, G, B, A};
    // }

    operator float *() const {
        return new float[4]{R, G, B, A};
    }


    operator Coord() const {
        return {R, G, B};
    }
};


bool isColorCloseEnough(ColorData color1, ColorData color2, float closeness);

class QuadPos : public ColorData {
};


#endif //COLORDATA_H
