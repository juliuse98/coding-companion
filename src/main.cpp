#include "index_buffer.h"
#include "ccobject.h"
#include "shader.h"
#include "sprite.h"
#include "sprite_renderer.h"
#include "texture.h"
#include "texture_loader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

// Easyloggingpp
#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP


#ifdef APIENTRY
#undef APIENTRY  // Fix macro redefinition warning for Windows
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/gtc/type_ptr.hpp>

// stb_image for image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#if defined(__unix__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <X11/X.h>
#include <X11/extensions/shapeconst.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#elif defined(_WIN32)
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#if defined(__unix__)
void makeWindowTransparentAndClickThrough(GLFWwindow* window)
{
    Display* display = glfwGetX11Display();
    Window win = glfwGetX11Window(window);
    Atom opacityAtom = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
    unsigned long opacity = (unsigned long)(1.0 * 0xFFFFFFFF);
    XChangeProperty(display, win, opacityAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&opacity, 1);

    XRectangle rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = 0;
    rect.height =0;

    XserverRegion region = XFixesCreateRegion(display, &rect, 1);
    XFixesSetWindowShapeRegion(display, win, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);
    Atom workarea = XInternAtom(display , "_NET_WORKAREA", True);
    if(workarea == None)
    {
        std::cout << "error retreiving values 1" << std::endl;
    }
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop = NULL;
    if(XGetWindowProperty(display, root, workarea, 0, 4 * sizeof(long), False, XA_CARDINAL, &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success)
    {
        if(prop){
        long *workarea_values = (long*)prop;
        std::cout << "1:" << workarea_values[0] << "2:" << workarea_values[1] << "3:" << workarea_values[2] << "4:" << workarea_values[3] << std::endl;
        }
    }else{
        std::cout << "error retreiving values" << std::endl;
    }
}
#elif defined(_WIN32)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_MINIMIZE) {
            // Ignore minimize requests
            return 0;
        }
        break;
        
    case WM_WINDOWPOSCHANGING: {
        WINDOWPOS* pPos = reinterpret_cast<WINDOWPOS*>(lParam);
        if (pPos) {
            // Prevent the window from being minimized
            pPos->flags &= ~SWP_SHOWWINDOW; // Clear the SWP_SHOWWINDOW flag
            return 0; // Prevent other handlers from processing this message
        }
        break;
    }

    // Handle other cases or pass them to the default window procedure
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
void ReapplyWindowStyles(HWND hwnd) {
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0xFF, LWA_ALPHA);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void SetTimerForStyles(HWND hwnd) {
    SetTimer(hwnd, 1, 1000, [](HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime) {
        ReapplyWindowStyles(hwnd);
    });
}

void makeWindowTransparentAndClickThrough(GLFWwindow* window) {
    HWND hwnd = glfwGetWin32Window(window);

    // Apply initial window styles
    ReapplyWindowStyles(hwnd);

    // Set a timer to reapply styles periodically
    SetTimerForStyles(hwnd);

    // Hook the window procedure to intercept messages
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
}
#endif

void updateUV(cabbage::UVCoordinate vertices[], cabbage::SpriteUVRect rect)
{
    vertices[0].u = rect.u;
    vertices[0].v = rect.v + rect.height;

    vertices[1].u = rect.u + rect.width;
    vertices[1].v = rect.v + rect.height;

    vertices[2].u = rect.u;
    vertices[2].v = rect.v;

    vertices[3].u = rect.u + rect.width;
    vertices[3].v = rect.v;
}

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations conf("./easyloggingpp.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    LOG(INFO) << "Start Init";
    if (!glfwInit()){
	LOG(ERROR) << "GLFW init failed!";
    }
    LOG(INFO) << "Init Complete";


    LOG(INFO) << "Create Window";
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    cabbage::Window cwindow;
    cwindow.setWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2);
    cwindow.setWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0);

    cwindow.setWindowHint( GLFW_DECORATED,                 GLFW_FALSE);
    cwindow.setWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER,   GLFW_TRUE);
    cwindow.setWindowHint( GLFW_MAXIMIZED,                 GLFW_FALSE);
    cwindow.setWindowHint( GLFW_FLOATING,                  GLFW_TRUE);
    cwindow.setWindowHint( GLFW_FOCUSED,                   GLFW_FALSE);
    cwindow.setWindowHint( GLFW_FOCUS_ON_SHOW,             GLFW_FALSE);
    std::string windowTitle = "Coding Companion";
    if(!cwindow.init(mode->width - 1, mode->height - 1, windowTitle)){
        LOG(INFO) << "GLFW cannot open window!";
    }
    cwindow.setPosition(0, 0);

    LOG(INFO) << "Init GLEW";
    glewInit();
   
    makeWindowTransparentAndClickThrough(cwindow.GetGLFWwindow());

    LOG(INFO) << "Init GLEW Complete";
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    coco::CCObject obj;
    coco::CCObject child1;

    cabbage::SpriteRenderer r;
    std::vector<cabbage::Texture*> textures;
    
    stbi_set_flip_vertically_on_load(true);

    cabbage::Texture* texture1 = cabbage::TextureLoader::load("arrowsRed.png");
    textures.push_back(texture1);
    cabbage::Texture* texture2 = cabbage::TextureLoader::load("arrowsGreen.png");
    textures.push_back(texture2);

    cabbage::Sprite catSprite(textures[0]);
    cabbage::Sprite arrowsGreenSprite(textures[1]);
    obj.SetSprite(&catSprite);
    child1.SetSprite(&arrowsGreenSprite);
    obj.addChild(&child1);
    
    //SpriteRenderer r(w);
    //Scene s;
    //s.rootObj.addChild(obj);
    //r.draw(s);


    cabbage::SpriteSheet spriteSheet = cabbage::SpriteSheet(textures.at(0));
    spriteSheet.addSpriteUVRect(0.0f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.66f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.0f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.66f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.0f, 0.0f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.0f, 0.33f, 0.33f);



 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const int target_fps = 60;
    const std::chrono::milliseconds target_frame_duration(1000 / target_fps);
    int spriteId = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(cwindow.GetGLFWwindow())){
        auto frame_start = std::chrono::high_resolution_clock::now();
	glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_LINE);

        obj.GetSprite()->SetUVRect(spriteSheet.getSpriteUVRect(spriteId));
        //updateUV(uvData, spriteSheet.getSpriteUVRect(spriteId));
        //uvBuffer.SetData(0, sizeof(uvData), uvData);
        r.draw(obj);

        std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - lastTime;

        if(elapsedTime.count() >= 0.2f)
        {
            spriteId++;
            if(spriteId > 7)
            {
                spriteId = 0;
            }
            std::cout << spriteId << std::endl;
            lastTime = std::chrono::high_resolution_clock::now();
        }

	glfwSwapBuffers(cwindow.GetGLFWwindow());
        glfwPollEvents();
        auto frame_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frame_time = frame_end - frame_start;
        if(frame_time < target_frame_duration)
        {
            std::this_thread::sleep_for(target_frame_duration - frame_time);
        }
    }
    glfwDestroyWindow(cwindow.GetGLFWwindow());
    glfwTerminate();
    return 0;
}

