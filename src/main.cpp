#include "companion_factory.h"
#include "config.h"

#include "ccobject.h"
#include "companion.h"
#include "graphics_manager.h"
#include "sprite.h"
#include "sprite_renderer.h"
#include "time_controller.h"
#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

// Easyloggingpp
#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#ifdef APIENTRY
#undef APIENTRY // Fix macro redefinition warning for Windows
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stb_image for image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if defined(__unix__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/shapeconst.h>

#elif defined(_WIN32)
#include <Windows.h>
#include <winuser.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#if defined(__unix__)
void makeWindowTransparentAndClickThrough(GLFWwindow* window)
{
    Display* display = glfwGetX11Display();
    Window   win = glfwGetX11Window(window);
    Atom     opacityAtom = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);

    unsigned long opacity = (unsigned long)(1.0 * 0xFFFFFFFF);
    XChangeProperty(display, win, opacityAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&opacity, 1);

    XRectangle rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = 0;
    rect.height = 0;

    XserverRegion region = XFixesCreateRegion(display, &rect, 1);
    XFixesSetWindowShapeRegion(display, win, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);

    int    screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);
    Atom   workarea = XInternAtom(display, "_NET_WORKAREA", True);
    if (workarea == None)
    {
        LOG(ERROR) << "error retreiving values 1";
    }
    Atom           actual_type;
    int            actual_format;
    unsigned long  nitems;
    unsigned long  bytes_after;
    unsigned char* prop = NULL;
    if (XGetWindowProperty(
            display, root, workarea, 0, 4 * sizeof(long), False, XA_CARDINAL, &actual_type, &actual_format, &nitems,
            &bytes_after, &prop
        ) == Success)
    {
        if (prop)
        {
            long* workarea_values = (long*)prop;
        }
    }
    else
    {
        LOG(ERROR) << "error retreiving values";
    }
}
#elif defined(_WIN32)

WNDPROC originalProc = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_MINIMIZE)
        {
            // Ignore minimize requests
            LOG(ERROR) << "MINIMIZE";
            return 0; // Block minimize
        }
        if ((wParam & 0xfff0) == SC_RESTORE)
        {
            // Ignore restore requests
            LOG(ERROR) << "RESTORE";
            return 0; // Block restore
        }
    }
    return CallWindowProc(originalProc, hwnd, uMsg, wParam, lParam);
    // Handle other cases or pass them to the default window procedure
}

void makeWindowTransparentAndClickThrough(GLFWwindow* window)
{
    HWND hwnd = glfwGetWin32Window(window);

    originalProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    // Hook the window procedure to intercept messages
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
}
#endif

int main(int argc, char* argv[])
{
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations conf("./easyloggingpp.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    coco::Config::getInstance().load("cc-config.json");
    LOG(INFO) << "Start Init";
    if (!glfwInit())
    {
        LOG(ERROR) << "GLFW init failed!";
    }
    LOG(INFO) << "Init Complete";

    LOG(INFO) << "Create Window";
    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    cabbage::Window    cwindow;
    cwindow.setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    cwindow.setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    cwindow.setWindowHint(GLFW_DECORATED, GLFW_FALSE);
    cwindow.setWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    cwindow.setWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    cwindow.setWindowHint(GLFW_FLOATING, GLFW_TRUE);
    cwindow.setWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    cwindow.setWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    cwindow.setWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    std::string windowTitle = "Coding Companion";
    LOG(INFO) << "Monitor Size: " << mode->width << "x" << mode->height;
    if (!cwindow.init(mode->width - 1, mode->height - 1, windowTitle))
    {
        LOG(INFO) << "GLFW cannot open window!";
    }
    cwindow.setPosition(0, 0);

    GLFWimage icon;
    icon.pixels = stbi_load("resources/icons/icon_128x128.png", &icon.width, &icon.height, 0, 4);
    if (icon.pixels)
    {
        glfwSetWindowIcon(cwindow.GetGLFWwindow(), 1, &icon);
        stbi_image_free(icon.pixels);
    }
    else
    {
        LOG(ERROR) << "Could not load icon";
    }

    LOG(INFO) << "Init GLEW";
    glewInit();

    makeWindowTransparentAndClickThrough(cwindow.GetGLFWwindow());

    LOG(INFO) << "Init GLEW Complete";
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    cabbage::GraphicsManager graphicsManager;
    coco::TimeController     timeController;

    coco::Companion rootObj;
    coco::Companion companion;
    timeController.addObject(companion);
    stbi_set_flip_vertically_on_load(true);
    int width, height;
    cwindow.GetSize(width, height);
    LOG(INFO) << "CWINDOW Size:" << width << "x" << height;
    coco::CompanionFactory::loadCompanion(companion, "resources/companions/cat_companion.json", graphicsManager);
    companion.PlayAnimation("sleeping");
    const int companionSize = coco::Config::getInstance().getValue<float>("companionSize", 100.0f);
    const int TICKS_PER_SECOND = coco::Config::getInstance().getValue<int>("ticksPerSecond", 30);
    companion.GetTransform().SetScale({companionSize, companionSize, 0});
    companion.GetTransform().SetPosition({width - companionSize, height - companionSize, 0});

    rootObj.addChild(&companion);
    {

        cabbage::SpriteRenderer r(&cwindow);

        LOG(INFO) << "Init GLEW Complete";

        r.prepareDraw(&rootObj);

        LOG(INFO) << "Init GLEW Complete";

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const int target_fps = coco::Config::getInstance().getValue<int>("fpsLimit", 60);

        const std::chrono::milliseconds target_frame_duration(1000 / target_fps);

        auto lastTime = std::chrono::high_resolution_clock::now();

        int spriteId = 0;
        while (!glfwWindowShouldClose(cwindow.GetGLFWwindow()))
        {
            auto frame_start = std::chrono::high_resolution_clock::now();
            glClear(GL_COLOR_BUFFER_BIT);
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_LINE);

            r.draw();

            std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - lastTime;

            if (elapsedTime.count() >= float(1.0f / TICKS_PER_SECOND))
            {
                timeController.Update(elapsedTime.count());
                lastTime = std::chrono::high_resolution_clock::now();
            }

            glfwSwapBuffers(cwindow.GetGLFWwindow());
            glfwPollEvents();
            auto frame_end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> frame_time = frame_end - frame_start;
            if (frame_time < target_frame_duration)
            {
                std::this_thread::sleep_for(target_frame_duration - frame_time);
            }
        }
    }
    glfwDestroyWindow(cwindow.GetGLFWwindow());
    glfwTerminate();
    return 0;
}
