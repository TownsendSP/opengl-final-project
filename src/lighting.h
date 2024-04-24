//
// Created by tgsp on 4/4/2024.
//

#ifndef LIGHTING_H
#define LIGHTING_H
#include "globals.h"
#include "ColorData.h"


class Light {
public:
    enum lightNum {
        LIGHT0 = GL_LIGHT0,
        LIGHT1 = GL_LIGHT1,
        LIGHT2 = GL_LIGHT2,
        LIGHT3 = GL_LIGHT3,
        LIGHT4 = GL_LIGHT4,
        LIGHT5 = GL_LIGHT5,
        LIGHT6 = GL_LIGHT6,
        LIGHT7 = GL_LIGHT7
    };

    bool enabled = true;

    ColorData lightAmb;
    ColorData lightDiff;
    ColorData lightSpec;
    ColorData lightPos;
    lightNum whichLight;


    Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diff, ColorData spec);;

    Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diffspec);;

    void setup();

    void enable() const;

    void disable() const;

    void lightswitch();;

    void lightswitch(bool onoff);;


    // define a cast so I can use the light in glEnable
    operator lightNum() const;

    //default constructor:
    Light();
};

class Spotlight : public Light {
public:
    Coord spotDir; //angle: X rot, Y rot, Z rot, 1.0
    float spotAttenuation;
    float spotAngle;

    Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, ColorData dir,
              float cutoff, float exponent);;
    Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, Coord dir,
              float cutoff, float exponent);;

    Spotlight();

    Spotlight(Light i, Coord dir, float cutoff, float exponent);

    void setup();

    void drawVis();
};




class Material {
public:
    Material() = default;

private:
    ColorData matSpec;
    ColorData matAmb;
    ColorData matEmission;
    ColorData matDiff;
    float matShine;

public:
    //constructor:

    Material(ColorData spec, ColorData amb, ColorData diff, ColorData emission, float shine);;

    Material(ColorData spec, ColorData amb, ColorData diff, float shine);

    //applyMaterial:
    void apply();

    //default constructor:
};

extern Material wallMat;
extern Material floorMat;
extern Material ceilingMat;
extern Material lampMat;
extern Material cardMat;
extern Material castIronMat;
extern Material tableMat;
extern Material superShinySteel;
extern Material matteConcrete;
extern Material shinyRed;
extern Material shinyGreen;
extern Material shinyBlue;


//light Library:
extern Light roomLight;
extern Light hallLight;
extern Light sunLight;
extern Spotlight headLamp;
// extern Spotlight redLight;
// extern Spotlight blueLight;
// extern Spotlight greenLight;





#endif //LIGHTING_H
