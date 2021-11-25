#include "Shader.h"


Shader::~Shader()
{
}

void Shader::UseTexture(std::string textureName)
{
	//glActiveTexture(GL_TEXTURE0 + textures[textureName].GetTextureNum());
	glBindTexture(GL_TEXTURE_2D, textures[textureName].GetTextureId());
}

void Shader::LoadTexture(const char* path, const char* textureUniformName, std::string textureName)
{
	Texture tempTexture(path, textureUniformName, program, textureQtd);
	textures[textureName] = tempTexture;
	textureQtd += 1;
}

void Shader::setBool(const std::string name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setMatrix4fv(const std::string name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setVec3(const std::string name, glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), vec.x, vec.y, vec.z);
}
