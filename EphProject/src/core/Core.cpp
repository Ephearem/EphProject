/**----------------------------------------------------------------------------
; @file Core.cpp
;
; @brief
;   The file implements the functionality of the singleton class 'Core'.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.hpp"
#include "Log.hpp"
#include "Shader.hpp"


/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func instance
;
; @brief
;   Creates (if not already created) and returns a Core instance pointer.
;
; @params
;   None
;
; @return
;   Core instance.
;
----------------------------------------------------------------------------**/
Core& Core::instance()
{
    static Core instance_;
    return instance_;
}


/**----------------------------------------------------------------------------
; @func init_window
;
; @brief
;   Initialize GLFW, GLAD. Create a window.
;
; @params
;   window_title    | Title of the window to be created.
;   window_size     | The size of the window (in pixels) to be created.
;   is_full_screen  | Whether to enable full screen mode after window creation.
;   swap_interval   | The minimum number of screen updates to wait for until
;                   | the buffers are swapped by glfwSwapBuffers.
;
; @return
;   None
;
; // TODO: return GLFWwindow*     | GLFW window object pointer.
;
----------------------------------------------------------------------------**/
void Core::init_window(char const* window_title, glm::ivec2 const& window_size,
                       bool is_full_screen, int swap_interval)
{
    GLFWmonitor* monitor_ptr = nullptr;

    glfwInit();                         /* Initialize and configure GLFW     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (is_full_screen)
    {
        monitor_ptr = glfwGetPrimaryMonitor();
    }
    this->window_ptr_ = glfwCreateWindow(window_size.x, window_size.y,
        window_title, monitor_ptr, nullptr);
                                        /* Create the window                 */
    if (this->window_ptr_ == nullptr)
    {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(this->window_ptr_);
    glfwSwapInterval(swap_interval);    /* Set the number of screen updates  */
                                        /* to wait from the time
                                        /* glfwSwapBuffers was called before */
                                        /* swapping the buffers and          */
                                        /* returning (i.e. vsync)            */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {                                   /* Initialize GLAD                   */
        LOG_ERROR("Failed to initialize GLAD");
        exit(-1);
    }
    glViewport(0, 0, window_size.x, window_size.y);
    glEnable(GL_BLEND);                 /* Enable GL_BLEND to support        */
                                        /* transparent textures              */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


/**----------------------------------------------------------------------------
; @func init_shaders
;
; @brief
;   Reads the contents of the vertex and fragment shader files and passes it to
;   the constructor of the 'Shader' class. Binds the created shader.
;   For more details, see the description of the 'Shader' class constructor.
; 
; @params
;   vertex_shader_file_path   | Zero-terminated vertex shader source string.
;   fragment_shader_file_path | Zero-terminated fragment shader source string.
;
; @return
;   None
;
; // TODO: create a separate class for working with files.
;
----------------------------------------------------------------------------**/
void Core::init_shaders(const char* vertex_shader_file_path,
                        const char* fragment_shader_file_path)
{
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    std::stringstream vertex_shader_lines;
    std::stringstream fragment_shader_lines;

    vertex_shader_file.exceptions(std::ifstream::failbit |
        std::ifstream::badbit);         /* Set exception mask to detect      */
                                        /* failbit and badbit errors while   */
                                        /* opening a vertex shader file      */
    fragment_shader_file.exceptions(std::ifstream::failbit |
        std::ifstream::badbit);         /* Set exception mask to detect      */
                                        /* failbit and badbit errors while   */
                                        /* opening a fragment shader file    */
    try
    {
        vertex_shader_file.open(vertex_shader_file_path);
                                        /* Open vertex shader file           */
        vertex_shader_lines << vertex_shader_file.rdbuf();
                                        /* Read vertex shader file's buffer  */
                                        /* Content into stream               */
        vertex_shader_file.close();     /* Close vertex shader file handler  */
    }
    catch (std::ifstream::failure& e)
    {
        std::string error_msg = "Failed to read a file: " +
            std::string(vertex_shader_file_path) + ". Error code: " +
            std::to_string(e.code().value()) +  ". " + std::string(e.what());
        LOG_ERROR(error_msg.c_str());
    }
    try
    {
        fragment_shader_file.open(fragment_shader_file_path);
                                        /* Open fragment shader file         */
        fragment_shader_lines << fragment_shader_file.rdbuf();
                                        /* Read fragment shader file's       */
                                        /* buffer                            */
        fragment_shader_file.close();   /* Close fragment shader file        */
                                        /* handler                           */
    }
    catch (std::ifstream::failure& e)
    {
        std::string error_msg = "Failed to read a file: " +
            std::string(vertex_shader_file_path) + ". Error code: " +
            std::to_string(e.code().value()) + ". " + std::string(e.what());
        LOG_ERROR(error_msg.c_str());
    }
    this->shader_ptr_ = new Shader(vertex_shader_lines.str().c_str(),
        fragment_shader_lines.str().c_str());
                                        /* Convert stream into c-string and  */
                                        /* pass them to the constructor of   */
                                        /* 'Shader' class                    */
    this->shader_ptr_->use();           /* Bind created shader               */
}


/**----------------------------------------------------------------------------
; @func start_main_loop
;
; @brief
;   Runs the render loop. Each iteration of the render loop clears the screen,
;   calls a custom function (to draw/compute logic, etc.), and swaps the front
;   and back buffers. After exiting the render loop, the GLFW windows will be
;   destroyed, the allocated resources will be freed.
;
; @params
;   main_loop_iteration_func
;
; @return
;   None
;
; // TODO: Sync, calc FPS.
; // TODO: This function should only start the loop. So, move initialization
;          'main_loop_iteration_func_' to another function and add
;          is-main-loop-iteration-func-nullptr check before starting the loop.
;
----------------------------------------------------------------------------**/
void Core::start_main_loop(void(*main_loop_iteration_func)())
{
    this->main_loop_iteration_func_ = main_loop_iteration_func;

    while (!glfwWindowShouldClose(this->window_ptr_))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        /* Specify clear values for the      */
        /* color buffer                      */
        glClear(GL_COLOR_BUFFER_BIT);   /* Clear the 'GL_COLOR_BUFFER_BIT'   */
                                        /* buffer using the selected color   */

        main_loop_iteration_func();     /* Call a custom callback            */

        glfwSwapBuffers(this->window_ptr_);
        /* Swap the front and back buffers   */
        glfwPollEvents();               /* Process all pending events        */
    }
    glfwTerminate();                    /* Destroy all windows, free         */
                                        /* allocated resources               */
}


/**----------------------------------------------------------------------------
; @func get_window_ptr
;
; @brief
;   Returns a pointer to the created GLFW window object.
; 
; @params
;   None
;
; @return
;   GLFWwindow * 
;
----------------------------------------------------------------------------**/
GLFWwindow* Core::get_window_ptr() const
{
    return this->window_ptr_;
}


/**----------------------------------------------------------------------------
; @func Core
;
; @brief
;   Constructor.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
Core::Core()
    :window_ptr_(nullptr), window_size_(0), shader_ptr_(nullptr),
    main_loop_iteration_func_(nullptr)
{
}
