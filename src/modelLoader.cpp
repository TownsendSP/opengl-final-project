//
// Created by tgsp on 4/28/2024.
//


#include "modelLoader.h"
#include "globals.h"

#include "../res/models/hatuv.h"
#include "../res/models/crystal.h"

void drawHatUV(){
    //enable drawing with the loaded arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //give it the pointers
    glVertexPointer(3, GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].position[0]);
    glNormalPointer(GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].normal[0]);
    glTexCoordPointer(2, GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].uv[0]);

    //draw it using those pointers
    glDrawElements(GL_TRIANGLES, sizeof(hatUVIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, hatUVIndices);

    //exit the vertex, normal, and texture_24 coordinate modes
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void drawCrystal(){
    //enabling drawing with the drawing the loadedArrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //give it the pointers
    glVertexPointer(3, GL_FLOAT, sizeof(CrystalVertex), &crystalVertices[0].position[0]);
    glNormalPointer(GL_FLOAT, sizeof(CrystalVertex), &crystalVertices[0].normal[0]);

    //draw it using those pointers
    glDrawElements(GL_TRIANGLES, 4428, GL_UNSIGNED_INT, crystalIndices);

    //exit the vertex and normal mode
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
