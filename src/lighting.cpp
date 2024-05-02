//
// Created by tgsp on 4/4/2024.
//

#include "lighting.h"


Light::Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diff, ColorData spec) {
    whichLight = whichLighta;
    lightAmb = amb;
    lightDiff = diff;
    lightSpec = spec;
    lightPos = pos;
}

Light::Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diffspec) {
    whichLight = whichLighta;
    lightAmb = amb;
    lightDiff = diffspec;
    lightSpec = diffspec;
    lightPos = pos;
}

void Light::setup() {
    glLightfv(whichLight, GL_AMBIENT, lightAmb);
    glLightfv(whichLight, GL_DIFFUSE, lightDiff);
    glLightfv(whichLight, GL_SPECULAR, lightSpec);
    glLightfv(whichLight, GL_POSITION, lightPos);
}

void Light::enable() const {
    if(enabled) {
        glEnable(whichLight);
    }
}

void Light::disable() const {
    glDisable(whichLight);
}

void Light::lightswitch() {
    enabled = !enabled;
}

void Light::lightswitch(bool onoff) {
    enabled = onoff;
}

Light::operator lightNum() const {
    return whichLight; //returns, for example, GL_LIGHT0
}

Light::Light() = default;


Spotlight::Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, ColorData dir,
                     float cutoff, float exponent): Light(whichLight, pos, amb, diff, spec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
}

Spotlight::Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, Coord dir,
                     float cutoff, float exponent): Light(whichLight, pos, amb, diff, spec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
}

Spotlight::Spotlight() = default;

Spotlight::Spotlight(Light i, Coord dir, float cutoff, float exponent): Light(i.whichLight, i.lightPos, i.lightAmb,
                                                                              i.lightDiff, i.lightSpec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
}

void Spotlight::setup() {
    Light::setup();
    glLightfv(whichLight, GL_SPOT_DIRECTION, spotDir);
    glLightf(whichLight, GL_SPOT_CUTOFF, spotAngle);
    glLightf(whichLight, GL_SPOT_EXPONENT, spotAttenuation);
}

void Spotlight::drawVis() {
    // // Rotate to match the direction of the spotlight
    // // Assuming spotDir is a direction vector, we need to convert it to an angle and an axis
    // float angle = acos(spotDir[2]); // Angle with the z-axis
    //
    //
    // float coneHeight = 12.0; // Height of the cone
    // //calc cone radius based on cutoff angle
    // float coneRadius = coneHeight * tan(spotAngle * (PI / 180.0)); // Radius of the cone
    //
    //
    //
    // glPushMatrix(); // Save matrix state
    //
    // // Translate to the position of the spotlight
    // glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    //
    //
    // glRotatef(angle * (180.0 / PI), -spotDir[1] / axisLength, 0, spotDir[0] / axisLength); // Rotate around the cross product of the direction and the z-axis
    //
    // // Set the color and transparency of the material
    // glColor4f(lightDiff[0], lightDiff[1], lightDiff[2], 0.2); // 20% opaque
    //
    // // Draw the cone
    // // Assuming the base radius and height of the cone are 1. Adjust these values as needed.
    // // Also assuming we have a suitable level of detail for the cone. Adjust these values as needed.
    // glutSolidCone(1, 1, 20, 20);
    //
    // glPopMatrix(); // Restore matrix state
}


Material::Material(ColorData spec, ColorData amb, ColorData diff, ColorData emission, float shine) {
    matSpec = spec;
    matAmb = amb;
    matEmission = emission;
    matDiff = diff;
    matShine = shine;
}

Material::Material(ColorData spec, ColorData amb, ColorData diff, float shine) {
    matSpec = spec;
    matAmb = amb;
    matEmission = ColorData(0.0f, 0.0f, 0.0f, 0.0f);
    matDiff = diff;
    matShine = shine;
}

void Material::apply() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec); //GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb); //GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission); //GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff); //GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShine); //GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

//Material Library:
#ifndef FOLDING_REGION_MaterialLibrary

Material wallMat = Material(
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    10.0f);

Material floorMat = Material( //pale brown color
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    33.0f); //just a bit glossy


Material ceilingMat = Material( //matte cream color
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    5.0f);

Material lampMat = Material( //emissive golden color.
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    ColorData(0.2f, 0.2f, 0.1f, 1.0f),
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    5.0f);

Material buttonMat = Material( //emissive bluish color
    ColorData(0.3f, 0.9f, 0.9f, 1.0f),
    ColorData(0.1f, 0.2f, 0.3f, 1.0f),
    ColorData(0.0f, 0.3f, 1.0f, 1.0f),
    ColorData(0.0f, 0.3f, 0.8f, 1.0f),
    5.0f);

Material cardMat = Material(
       ColorData(0.3f,0.3f,0.3f, 1.0f),
        ColorData(0.05f, 0.05f, 0.1f, 1.0f), // Lower ambient for better shine
        ColorData(0.15f, 0.25f, 0.8f, 1.0f), // Slightly lighter blue (optional)
        10.0f); // Very high shininess for sharp highlight

Material castIronMat = Material( // nearly black, metallic
    ColorData(1.0f, 1.0f, 1.0f, 1.0f),
    ColorData(0.0f, 0.0f, 0.0f, 1.0f),
    ColorData(0.1f, 0.1f, 0.1f, 1.0f),
    10.0f);

//shiny dark brown texture_24 for table
Material tableMat = Material(
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.1f,0.1f,0.1f, 1.0f),
    ColorData(Coord(149, 69, 53)/255, 1.0f),
    10.0f);

Material superShinySteel = Material(
    ColorData(0.8f, 1.0f, 1.0f, 1.0f),
    // ColorData(scrollVar, scrollVar, scrollvar, 1.0f),
    ColorData(0.1f, 0.1f, 0.1f, 1.0f),
    ColorData(0.2f,0.2f,0.2f, 1.0f),
    10.0f);

Material matteConcrete = Material(
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    5.0f);

Material shinyRed = Material(
    ColorData(0.9f, 0.5f, 0.5f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(1.0f, 0.0f, 0.0f, 1.0f), //diffuse
    10.0f); //shininess

Material shinyGreen = Material(
    ColorData(1.0f, 1.0f, 1.0f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
    100.0f); //shininess

Material shinyBlue = Material(
    ColorData(0.4f, 0.4f, 1.0f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(0.0f, 0.0f, 1.0f, 1.0f), //diffuse
    10.0f); //shininess


#endif


//globally accessible object roomLight;
Light roomLight = Light(
    Light::LIGHT1,
    ColorData(Coord(15, 3, 0), 1.0), //pos
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //amb
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //diff
    ColorData(1.0f, 1.0f, 1.0f, 1.0f) //spec
);

Light hallLight = Light(
    Light::LIGHT5,
    ColorData(Coord(15, 3, 0), 1.0), //pos
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //amb
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //diff
    ColorData(1.0f, 1.0f, 1.0f, 1.0f) //spec
);
    // ColorData(0.2f, 0.2f, 0.2f, 1),
    // ColorData(1.0f, 1.0f, 1.0f, 1.0f),
    // ColorData(1.0f, 1.0f, 1.0f, 1.0f));

Light brightWhite = Light(
    Light::LIGHT3,
    ColorData(Coord(0, 0, 0), 1.0),
    ColorData(0.2f, 0.2f, 0.2f, 1),
    ColorData(1.0f, 1.0f, 1.0f, 1.0f),
    ColorData(1.0f, 1.0f, 1.0f, 1.0f));

Light brightRed = Light(
    Light::LIGHT4,
    ColorData(Coord(0, 0, 0), 1.0),
    ColorData(0.2f, 0.05f, 0.05f, 1),
    ColorData(1.0f, 0.2f, 0.2f, 1.0f),
    ColorData(1.0f, 0.2f, 0.2f, 1.0f));


//
// Light brightGreen = Light(
//     Light::LIGHT5,
//     ColorData(Coord(0, 10, 0), 1.0),
// ColorData(0.05f, 0.2f, 0.05f, 1),
// ColorData(0.2f, 1.0f, 0.2f, 1.0f),
// ColorData(0.2f, 1.0f, 0.2f, 1.0f));
//
// Light brightBlue = Light(
//     Light::LIGHT6,
//     ColorData(Coord(0, 10, 0), 1.0),
//     ColorData(0.05f, 0.05f, 0.2f, 1),
//     ColorData(0.2f, 0.2f, 1.0f, 1.0f),
//     ColorData(0.2f, 0.2f, 1.0f, 1.0f));



Spotlight headLamp = Spotlight(
    brightWhite,
    Coord(1, 0, 0),
    15.0f,
    10.0f);

// Spotlight redLight = Spotlight(
//     brightRed,
//     Coord(1, 0, 0),
//     30.0f,
//     10.0f);
//
// Spotlight greenLight = Spotlight(
//     brightGreen,
//     Coord(1, 0, 0),
//     30.0f,
//     10.0f);
//
// Spotlight blueLight = Spotlight(
//     brightBlue,
//     Coord(1, 0, 0),
//     30.0f,
//     10.0f);



Light sunLight = Light( //directional sunlight, high in the sky
    Light::LIGHT2,
    ColorData(1.0f, 1.0f, 1.0f, 0.0f), //pos
    ColorData(0.2f, 0.2f, 0.2f, 1), //amb
    ColorData(1.0f, 1.0f, 1.0f, 1.0f), //diff
    ColorData(1.0f, 1.0f, 1.0f, 1.0f)); //spec
