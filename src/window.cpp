#include "window.h"
#include "GLFW/glfw3.h"

namespace cabbage
{

Window::Window()
    : m_window(NULL)
    , m_decorated(true)
    , m_transparentFramebuffer(false)
    , m_maximized(false)
    , m_floating(false)
    , m_focused(true)
    , m_focusOnShow(true)
{
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}
bool Window::init(int width, int height, const std::string& title, GLFWmonitor* monitor)
{
    m_window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
    if (!m_window)
    {
        return false;
    }

    glfwMakeContextCurrent(m_window);
    return true;
}
void Window::setPosition(int x, int y)
{
    glfwSetWindowPos(m_window, x, y);
}
void Window::setWindowHint(int hint, int value)
{
    glfwWindowHint(hint, value);
}
GLFWwindow* Window::GetGLFWwindow()
{
    return m_window;
}
void Window::GetSize(int& width, int& height)
{
    glfwGetWindowSize(m_window, &width, &height);
}
int Window::GetWidth()
{
    int width, height;
    GetSize(width, height);
    return width;
}

int Window::GetHeight()
{
    int width, height;
    GetSize(width, height);
    return height;
}

} // namespace cabbage
