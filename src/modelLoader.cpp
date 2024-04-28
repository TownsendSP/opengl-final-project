//
// Created by tgsp on 4/28/2024.
//

#include "modelLoader.h"
#include "globals.h"
#include "../res/models/hatuv.h"

void drawHatUV(){
    // Enable drawing with the loaded arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Give it the pointers
    glVertexPointer(3, GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].position[0]);
    glNormalPointer(GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].normal[0]);
    glTexCoordPointer(2, GL_FLOAT, sizeof(HatUVVertex), &hatUVverts[0].uv[0]);

    // Draw it using those pointers
    glDrawElements(GL_TRIANGLES, sizeof(hatUVIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, hatUVIndices);

    // Exit the vertex, normal, and texture coordinate modes
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}




/*
 * void drawModel() {
    //enabling drawing with the drawing the loadedArrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //give it the pointers
    glVertexPointer(3, GL_FLOAT, sizeof(struct ModelVertex), &modelVertices[0].position[0]);
    glNormalPointer(GL_FLOAT, sizeof(struct ModelVertex), &modelVertices[0].normal[0]);

    //draw it using those pointers
    glDrawElements(GL_TRIANGLES, sizeof(modelIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, modelIndices);

    //exit the vertex and normal mode
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
 * */