#ifndef VAO_HPP
#define VAO_HPP

#include "VBO.hpp"
#include <glad/glad.h>

class VAO {
public:
  GLuint ID;
  VAO();

  void LinkVBO(VBO VBO, GLuint layout);
  void Bind();
  void Unbind();
  void Delete();
};
#endif // !VAO_HPP
