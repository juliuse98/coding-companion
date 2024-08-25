#include "sprite.h"
#include "texture.h"
#include "texture_loader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#ifdef APIENTRY
#undef APIENTRY  // Fix macro redefinition warning for Windows
#endif

#include <iostream>

#include <cstddef>
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

void updateUV(float array[], SpriteUVRect rect)
{
    array[3] = rect.u;
    array[4] = rect.v + rect.height;

    array[8] = rect.u + rect.width;
    array[9] = rect.v + rect.height;

    array[13] = rect.u;
    array[14] = rect.v;

    array[18] = rect.u + rect.width;
    array[19] = rect.v;
}

int main(int argc, char *argv[]) {
    std::cout << "Start Init" << std::endl;
    if (!glfwInit()){
	std::cout << "GLFW init failed!" << std::endl;
    }
    std::cout << "Init Complete" << std::endl;
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    std::cout << "Create Window" << std::endl;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width - 1, mode->height - 1, "Coding Companion", NULL, NULL);
    if(!window){
        std::cout << "GLFW cannot open window!" << std::endl;
    }
    glfwSetWindowPos(window, 0, 0);
    glfwMakeContextCurrent(window);
    std::cout << "Init GLEW" << std::endl;
    glewInit();
   
    makeWindowTransparentAndClickThrough(window);

    std::cout << "Init GLEW Complete" << std::endl;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    float vertices[] = {
	(float)(mode->width) - 100.0f, (float)(mode->height) - 100.0f, 0.0f, 0.0f, 1.0f, // top left
	(float)(mode->width)         , (float)(mode->height) - 100.0f, 0.0f, 1.0f, 1.0f, // top right
	(float)(mode->width) - 100.0f, (float)(mode->height)         , 0.0f, 0.0f, 0.0f, // bottom left
	(float)(mode->width)         , (float)(mode->height)         , 0.0f, 1.0f, 0.0f,  // bottom right
    };

    unsigned int indices[] = {
        0,2,1,
        1,2,3
    };

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    unsigned int VAO;
    std::cout << "TEST1" << std::endl;
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 texCoords;\n"
    "uniform mat4 projection;\n"
    "out vec2 TexCoords;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoords = texCoords;\n"
    "}\0";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "  FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
        "  FragColor = texture(ourTexture, TexCoords);\n"
        "}\n";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    std::cout << success << "hello" << std::endl;
    if(!success) {
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

    
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);

    std::cout << success << "hello" << std::endl;
    unsigned int EBO;
    glGenBuffers(1,&EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  
    // 2. use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    // 3. now draw the object 

    stbi_set_flip_vertically_on_load(true);
    Texture* texture = TextureLoader::load("test.png");
    SpriteSheet spriteSheet = SpriteSheet(texture);
    spriteSheet.addSpriteUVRect(0.0f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.66f, 0.66f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.0f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.66f, 0.33f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.0f, 0.0f, 0.33f, 0.33f);
    spriteSheet.addSpriteUVRect(0.33f, 0.0f, 0.33f, 0.33f);

    glm::mat4 projection = glm::ortho(0.0f,(float)(mode->width), (float)(mode->height), 0.0f, -1.0f,1.0f);

    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    texture->bind();
    std::cout << glGetError() << std::endl;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    const int target_fps = 60;
    const std::chrono::milliseconds target_frame_duration(1000 / target_fps);
    int spriteId = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(window)){
        auto frame_start = std::chrono::high_resolution_clock::now();
	glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_LINE);

        glBindVertexArray(VAO);
        updateUV(vertices, spriteSheet.getSpriteUVRect(spriteId));
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

	glfwSwapBuffers(window);
        glfwPollEvents();
        auto frame_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frame_time = frame_end - frame_start;
        if(frame_time < target_frame_duration)
        {
            std::this_thread::sleep_for(target_frame_duration - frame_time);
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

