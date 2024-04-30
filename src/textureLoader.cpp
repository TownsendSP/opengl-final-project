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

BitMapFile *getBMPData(std::string filename) {
    BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;

    // Read input file name.
    std::ifstream infile(filename.c_str(), std::ios::binary);

    // Get the starting point of the image data.
    infile.seekg(10);
    infile.read((char *) &offset, 4);

    // Get the header size of the bitmap.
    infile.read((char *) &headerSize, 4);

    // Get width and height values in the bitmap header.
    infile.seekg(18);
    infile.read((char *) &bmp->sizeX, 4);
    infile.read((char *) &bmp->sizeY, 4);

    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 24;
    bmp->data = new unsigned char[size];

    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char *) bmp->data, size);

    // Reverse color from bgr to rgb.
    int temp;
    for (int i = 0; i < size; i += 3) {
        temp = bmp->data[i];
        bmp->data[i] = bmp->data[i + 2];
        bmp->data[i + 2] = temp;
    }

    return bmp;
}

void loadTexture(std::string filename, int id) {
    std::string wthfilename = filename;
    BitMapFile *image[1];

    // Load the texture.
    image[0] = getBMPData(std::move(filename));
    // Activate texture index texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[id]);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specify an image as the texture to be bound with the currently active texture index.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
    std::cout << "Loaded texture: " << wthfilename << " with id: " << id << " - Dimensions " << image[0]->sizeX << "x" << image[0]->sizeY << std::endl;
    std::map<std::string, int> whyisthemapbroken = textureMap;
    textureMap[wthfilename]  = id;
}



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
        loadTexture(directoryForFrames + filename, i);
        i++;
    }
    numAnimFrames = i;
    // Return the number of .bmp files
    return filenames.size();
}


int animVideo(int currFrameI) {
    return (currFrameI + 1) % numAnimFrames;
}


void setupTextures() {

    glGenTextures(NUMTEXTURES, texture);

    int numtexts = loadAnim("res/textures/minsize/");
    std::cout << "Loaded " << numtexts << " textures" << std::endl;

    // loadTexture("res/textures/hat.bmp", numAnimFrames);
    loadTexture("res/textures/minsize/hat.bmp", numAnimFrames);
    loadTexture("res/textures/star.bmp", numAnimFrames);




    // Turn on OpenGL texturing.
    // glEnable(GL_TEXTURE_2D);
    // Specify how texture values combine with current surface color values.
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


}