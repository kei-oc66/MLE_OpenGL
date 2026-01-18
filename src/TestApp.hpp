#ifndef TEST_APP_HPP
#define TEST_APP_HPP

#include "Window.hpp"

class TestApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  Window mWindow{WIDTH, HEIGHT, "NULL"};
};
#endif // !TEST_APP_HPP
