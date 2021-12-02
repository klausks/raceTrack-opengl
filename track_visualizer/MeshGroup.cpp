#include "MeshGroup.h"

void MeshGroup::bindVAO()
{
    if (this->vao == NULL) {
        glGenVertexArrays(1, &vao);
    }
    glBindVertexArray(vao);
}

void MeshGroup::bindTexture()
{
    if (this->texture != NULL)
    {
        GLuint textureId = this->texture->GetTextureId();
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

void MeshGroup::setTexture(string filename)
{

}