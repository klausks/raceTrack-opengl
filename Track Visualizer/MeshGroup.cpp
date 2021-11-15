#include "MeshGroup.h"

void MeshGroup::bindVAO() {
    if (this->vao == NULL) {
        glGenVertexArrays(1, &vao);
    }
    glBindVertexArray(vao);
}

void MeshGroup::bindTexture() {
    if (this->textureId == NULL) {
        return;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}