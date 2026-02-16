#pragma once
#include <glm/glm.hpp>

class GLFWwindow;

namespace coco
{
class InputHandler
{
  public:
    InputHandler();
    void init();
    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

  public:
    glm::vec2 mousePosition;
};
} // namespace coco
