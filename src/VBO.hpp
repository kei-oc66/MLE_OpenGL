#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Colors;
  glm::vec2 TexUV;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

class VBO {
public:
  GLuint ID;
  VBO(std::vector<Vertex> &vertices);

  void Bind();
  void Unbind();
  void Delete();
};
#endif // !VBO_HPP
