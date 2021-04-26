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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.hpp"
#include "Log.hpp"



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
    :window_ptr_(nullptr), window_size_(0), main_loop_iteration_func_(nullptr)
{
}
