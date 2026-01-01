#include <glad/glad.h>
#include "VAO.hpp"
#include "VBO.hpp"

VAO::VAO() { glGenVertexArrays(1, &ID); }
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type,
                     GLsizeiptr stride, void *offset) {
  VBO.Bind();
  glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)0);

  glEnableVertexAttribArray(layout);
  VBO.Unbind();
}

void VAO::Bind() { glBindVertexArray(ID); }

void VAO::Unbind() { glBindVertexArray(0); }

void VAO::Delete() { glDeleteVertexArrays(1, &ID); }
