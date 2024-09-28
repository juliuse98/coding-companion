#pragma once

#include <GLFW/glfw3.h>
#include <string>

class GLFWmonitor;

namespace cabbage
{

class Window
{
  public:
    Window();
    ~Window();
    bool        init(int width, int height, const std::string& title, GLFWmonitor* monitor = NULL);
    GLFWwindow* GetGLFWwindow();

    void setPosition(int x, int y);
    void setWindowHint(int hint, int value);

    void GetSize(int& width, int& height);
    int  GetWidth();
    int  GetHeight();

  private:
    GLFWwindow* m_window;

    bool m_decorated;
    bool m_transparentFramebuffer;
    bool m_maximized;
    bool m_floating;
    bool m_focused;
    bool m_focusOnShow;
};

} // namespace cabbage
