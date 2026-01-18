#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// std
#include <string>
class Window {
public:
  Window(int w, int h, std::string name);
  ~Window();

  bool shouldClose() { return glfwWindowShouldClose(window); }

private:
  void initWindow();

  const int width;
  const int height;

  std::string windowName;
  GLFWwindow *window;
};

#endif // !WINDOW_HPP
