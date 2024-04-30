//
// Created by tgsp on 4/28/2024.
//

#ifndef TGSOUTHAFINALPROJECT_TEXTURELOADER_H
#define TGSOUTHAFINALPROJECT_TEXTURELOADER_H


#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


struct BitMapFile {
    int sizeX;
    int sizeY;
    unsigned char *data;
};

BitMapFile *getBMPData(std::string filename);

void loadTexture(std::string filename, int id);

//returns: number of frames
int loadAnim(const std::string &directoryForFrames);

void setupTextures();

#endif //TGSOUTHAFINALPROJECT_TEXTURELOADER_H