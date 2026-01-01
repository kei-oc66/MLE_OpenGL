#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "EBO.hpp"
#include "Shader_Class.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include <stb_image.h>

// Vertices coordinates
GLfloat vertices[] = {
    //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Lower left corner
    -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Upper right corner
    0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f  // Lower right corner
};

GLuint indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int main() {
  // Initialize GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using
  // In this case we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
  GLFWwindow *window = glfwCreateWindow(800, 800, "WML", NULL, NULL);
  // Error check if the window fails to create
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);

  // Load GLAD so it configures OpenGL
  gladLoadGL();
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  glViewport(0, 0, 800, 800);

  // Generates Shader object using shaders defualt.vert and default.frag
  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

  // Generates Vertex Array Object and binds it
  VAO VAO1;
  VAO1.Bind();

  // Generates Vertex Buffer Object and links it to vertices
  VBO VBO1(vertices, sizeof(vertices));

  EBO EBO1(indices, sizeof(indices));
  // Links VBO to VAO
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float),
                  (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float),
                  (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  int widthImg, heightImg, numColCh;
  unsigned char *bytes = stbi_load("../resources/textures/grass_text.jpg",
                                   &widthImg, &heightImg, &numColCh, 0);

  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, bytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint text0Uni = glGetUniformLocation(shaderProgram.ID, "text0");
  shaderProgram.use();
  glUniform1i(text0Uni, 0);

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // Specify the color of the background
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.use();

    glUniform1f(uniID, 0.5f);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();
  }

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  glDeleteTextures(1, &texture);

  // Delete window before ending the program
  glfwDestroyWindow(window);
  // Terminate GLFW before ending the program
  glfwTerminate();
  return 0;
}
