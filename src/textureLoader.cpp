//
// Created by tgsp on 4/28/2024.
//

#include "textureLoader.h"
#include "globals.h"

#include <fstream>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <filesystem>
bool shouldbreak = false;
//not bothering to create 24/32 bit depth animations, if I need any transparent animations i'll just load an 8-bit mask
int loadAnim(const std::string &directoryForFrames) {
    std::list<std::string> filenames;

    for (const auto &entry : std::filesystem::directory_iterator(directoryForFrames)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bmp") {
            filenames.push_back(entry.path().filename().string());
        }
    }

    // Load each .bmp file
    int i = 0;
    for (const auto &filename : filenames) {
        loadTexture_24(directoryForFrames + filename, i);
        i++;
    }
    numAnimFrames = i;
    // Return the number of .bmp files
    return filenames.size();
}

int animVideo(int currFrameI) {
    return (currFrameI + 1) % numAnimFrames;
}

std::string extractFileName(std::string filename) {
    std::string::size_type slash = filename.find_last_of("/");
    std::string::size_type period = filename.find_last_of(".");
    return filename.substr(slash + 1, period - slash - 1);
}

#ifndef FOLDING_REGION_24_BIT_TEXTURES
BitMapFile *getBMPData_24(std::string filename) {
    std::ifstream infile(filename.c_str(), std::ios::binary);

    // Check if the file was opened successfully
    if (!infile) {
        std::cout << "Texture " << filename << " not found, exiting." << std::endl;
        exit(1);
    }

    BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;

    // Read input file name.

    // Get the starting point of the image data.
    infile.seekg(10);
    infile.read((char *) &offset, 4);

    // Get the header size of the bitmap.
    infile.read((char *) &headerSize, 4);

    int off = offset;
    int headr = headerSize;
    // Get width and height values in the bitmap header.
    int xOff = 18;
    infile.seekg(xOff);
    infile.read((char *) &bmp->sizeX, 4);
    int sizex = bmp->sizeX;
    infile.read((char *) &bmp->sizeY, 4);
    int sizey = bmp->sizeY;

    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 24;
    bmp->data = new unsigned char[size];

    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char *) bmp->data, size);
    // for debugging, print the entire file as bytes

    // Reverse color from bgr to rgb.
    int temp;

    for (int i = 0; i < size; i += 3) {
        temp = bmp->data[i];
        bmp->data[i] = bmp->data[i + 2];
        bmp->data[i + 2] = temp;
    }
    int a = 3;
    return bmp;
}


void loadTexture_24(std::string filename, int id) {

    std::string pathname = filename;
    std::string name = extractFileName(filename);

    if(id == 357 || id == 358)
        shouldbreak = true;
    else
        shouldbreak = false;


    BitMapFile *image[1];

    // Load the texture_24.
    image[0] = getBMPData_24(std::move(filename));
    // Activate texture_24 index texture_24[0].
    glBindTexture(GL_TEXTURE_2D, texture_24[id]);

    // Set texture_24 parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture_24 parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specify an image as the texture_24 to be bound with the currently active texture_24 index.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);


    std::cout << "Loaded texture_24: " << pathname << " as " << name << " with index: " << id << " - Dimensions " <<
            image[0]->sizeX << "x" << image[0]->sizeY << std::endl;
    // std::map<std::string, int> whyisthemapbroken = textureMap_24;


    textureMap_24[name] = id;
}

void setupTextures_24() {
    glGenTextures(NUMTEXTURES_24, texture_24);

    int numtexts = loadAnim("res/textures/animated/");
    std::cout << "Loaded " << numtexts << " textures" << std::endl;

    // loadTexture_24("res/textures/hat.bmp", numAnimFrames);
    loadTexture_24("res/textures/hat.bmp", numAnimFrames);
    loadTexture_24("res/textures/star.bmp", numAnimFrames + 1);
    // loadTexture_24("res/textures/ba.bmp", numAnimFrames + 2);
    loadTexture_24("res/textures/flame24.bmp", numAnimFrames + 3);
    loadTexture_24("res/textures/flame24.bmp", 0);
}
#endif

#ifndef FOLDING_REGION_32_BIT_TEXTURES
BitMapFile *getBMPData_32(std::string filename) {
    std::ifstream infile(filename.c_str(), std::ios::binary);

    if (!infile) {
        std::cout << "Texture " << filename << " not found, exiting." << std::endl;
        exit(1);
    }

    BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;

    infile.seekg(10);
    infile.read((char *) &offset, 4);

    // Get the header size of the bitmap.
    infile.read((char *) &headerSize, 4);

    // Get width and height values in the bitmap header.
    infile.seekg(18);
    infile.read((char *) &bmp->sizeX, 4);
    infile.read((char *) &bmp->sizeY, 4);

    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 32; //32 bits here
    bmp->data = new unsigned char[size];

    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char *) bmp->data, size);

    return bmp;
}

void loadTexture_32(std::string filename, int id) {
    std::string pathname = filename;
    std::string name = extractFileName(filename);

    BitMapFile *image[1];

    // Load the texture_32.
    image[0] = getBMPData_32(std::move(filename));
    // Activate texture_32 index texture_32[0].
    glBindTexture(GL_TEXTURE_2D, texture_32[id]);

    // Set texture_32 parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture_32 parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specify an image as the texture_32 to be bound with the currently active texture_32 index.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, image[0]->data); // Change here for 32-bit

    std::cout << "Loaded texture_32: " << pathname << " as " << name << " with index: " << id << " - Dimensions " <<
            image[0]->sizeX << "x" << image[0]->sizeY << std::endl;

    textureMap_32[name] = id;
}

void setupTextures_32() {
    glGenTextures(NUMTEXTURES_32, texture_32);
    loadTexture_32("res/textures32/flame.bmp", 0);

}
#endif

void setupTextures() {
    setupTextures_24();
    setupTextures_32();
}
