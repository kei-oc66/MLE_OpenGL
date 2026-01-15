#ifndef MESH_HPP
#define MESH_HPP

#include "Camera.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
#include "VAO.hpp"

class Mesh {
public:
  // mesh data
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  VAO vao;

  Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
       std::vector<Texture> &textures);

  void Draw(Shader &shader, Camera &camera);
};

#endif // !MESH_HPP
