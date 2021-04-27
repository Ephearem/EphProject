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
#include "Image.hpp"



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
;   vertex_shader_file_path   | The path to the vertex shader source file.
;   fragment_shader_file_path | The path to the fragment shader source file.
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

    // TODO: The code between this and the next 'TODO' is for testing the
    //       developed functionality. During development, the logic described
    //       below will be moved into separate methods of the 'Core' class and
    //       called outside of the methods of this class.
    // TEMPORARY CODE START


    float vertices[] =                  /* Set up vertices and txd vertices  */
    {
         0.5f,  0.5f, 1.0f, 1.0f,       /* Top right                         */
         0.5f, -0.5f, 1.0f, 0.0f,       /* Bottom right                      */
        -0.5f, -0.5f, 0.0f, 0.0f,       /* Bottom left                       */
        -0.5f,  0.5f, 0.0f, 1.0f        /* Top left                          */
    };

    unsigned int indices[] =            /* Set up indices                    */
    {
        0, 1, 3,                        /* The 1-st triangle                 */
        1, 2, 3                         /* The 2-nd triangle                 */
    };
    unsigned int vertex_buffer = 0;
    unsigned int indices_buffer = 0;
    unsigned int vertex_array = 0;

    glGenVertexArrays(1, &vertex_array);/* Generate a verex array object     */
    glGenBuffers(1, &vertex_buffer);    /* Generate a buffer object to store */
                                        /* the positions of the vertices     */
    glGenBuffers(1, &indices_buffer);   /* Generate a buffer object to store */
                                        /* the vertex indices                */
    glBindVertexArray(vertex_array);    /* Set 'vertex_array' as the current */
                                        /* vertex array object.              */

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
                                        /* Bind 'vertex_buffer' to           */
                                        /* GL_ARRAY_BUFFER. All following    */
                                        /* calls to GL_ARRAY_BUFFER will     */
                                        /* refer to this 'vertex_buffer'     */
                                        /* object.                           */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                                        /* Put data from 'vertices' into     */
                                        /* GL_ARRAY_BUFFER (i.e. into        */
                                        /* 'vertex_buffer')                  */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
                                        /* Bind 'vertex_buffer' to           */
                                        /* GL_ELEMENT_ARRAY_BUFFER. All      */
                                        /* following calls to                */
                                        /* GL_ELEMENT_ARRAY_BUFFER will refer*/
                                        /* to this 'indices_buffer' object.  */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);                /* Put data from 'indices' into      */
                                        /* GL_ELEMENT_ARRAY_BUFFER (i.e.     */
                                        /* into 'indices_buffer')            */
    glBindVertexBuffer(0, vertex_buffer, 0, sizeof(GLfloat) * 4);
                                        /* Bind 'vertex_buffer' to           */
                                        /* 'vertex_array' at index 0.        */
    glBindVertexBuffer(1, vertex_buffer, sizeof(GLfloat) * 2,
        sizeof(GLfloat) * 4);           /* Bind 'vertex_buffer' to           */
                                        /* 'vertex_array' at index 0.        */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   /* 'vertex_buffer' can be safely     */
                                        /* unbind since it is bound to       */
                                        /* 'vertex_array' as the vertex      */
                                        /* attribute at index 0.             */

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                                        /* 'indices_buffer' cannot be        */
                                        /* unbound from 'vertex_array' while */
                                        /* 'vertex_array' is active          */

    glBindVertexArray(0);               /* Unbind 'vertex_array' so that     */
                                        /* other calls to the vertex array   */
                                        /* will not change the current       */
                                        /* ('vertex_array') object.          */
    glBindVertexArray(vertex_array);


    
    unsigned int texture_2d_array;
    glGenTextures(1, &texture_2d_array);
                                        /* Generate a 2d texture array       */
                                        /* object                            */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_2d_array);
                                        /* Set 'texture_2d_array' as the     */
                                        /* current vertex array object.      */
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,            /* target to which the texture is    */
                                        /* bound                             */
        0,                              /* level                             */
        GL_RGBA8,                       /* Internal format                   */
        static_cast<GLsizei>(512),      /* Width of the 2d texture array     */
        static_cast<GLsizei>(512),      /* Heigh of the 2d texture array     */
        static_cast<GLsizei>(2),        /* Depth of the 2d texture array     */
        0,                              /* Border, must be 0.                */
        GL_RGBA,                        /* Format of the pixel data          */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data       */
        nullptr);                       /* A pointer to the image data       */

    Image img_1("res/img/512x512_transp.png");
    Image img_2("res/img/256x256.jpg");


                                        /* Add texture to the 0th layer of   */
                                        /* the 2d texture array              */

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 512);  
                                        /* The full width of the image from  */
                                        /* which the texture is created      */
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
                                        /* Subtexture x-offset (from the     */
                                        /* beginning of the image).          */
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
                                        /* Subtexture y-offset (from the     */
                                        /* beginning of the image).          */
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,            /* target to which the texture is    */
                                        /* bound                             */
        0,                              /* Level-of-detail. 0 - base image   */
        0,                              /* x-offset within the texture array */
        0,                              /* y offset within the texture array */
        0,                              /* z offset (layer)                  */
        static_cast<GLsizei>(256),      /* width of the texture subimage     */
        static_cast<GLsizei>(512),      /* height of the texture subimage    */
        1,                              /* depth of the texture subimage     */
        GL_RGBA,                        /* Format of the pixel data          */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data       */
        static_cast<const void*>(img_1.get_data()));
                                        /* A pointer to the image data       */


                                        /* Add texture to the 1st layer of   */
                                        /* the 2d texture array              */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_2d_array);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 256);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        256,
        256,                            /* The offset is calculated from the */
                                        /* bottom of the texture. For it to  */
                                        /* be considered from the top, need  */
                                        /* to swap the vertices related to   */
                                        /* the texture in the 'vertices'     */
                                        /* array.                            */
        // TODO: Check out the comment posted above because I don't remember
        // exactly :)
        0,
        static_cast<GLsizei>(256),
        static_cast<GLsizei>(256),
        1,
        GL_RGB,                         /* There are only 3 channels in this */
                                        /* .jpg image                        */
        GL_UNSIGNED_BYTE,
        static_cast<const void*>(img_2.get_data()));

    img_1.free();
    img_2.free();

    shader_ptr_->set_int("uf_txd_array_z_offset", 0);
    shader_ptr_->set_int("uf_txd_unit", 0);

    // TODO: TEMPORARY CODE END


    while (!glfwWindowShouldClose(this->window_ptr_))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                                        /* Specify clear values for the      */
                                        /* color buffer                      */
        glClear(GL_COLOR_BUFFER_BIT);   /* Clear the 'GL_COLOR_BUFFER_BIT'   */
                                        /* buffer using the selected color   */


        // TODO: The code between this and the next 'TODO' is for testing the
        //       developed functionality. During development, the logic
        //       described below will be moved into separate methods of the
        //       'Core' class and called inside the user-defined function
        //       'main_loop_iteration_func'.
        // TEMPORARY CODE START

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // TODO: TEMPORARY CODE END


        main_loop_iteration_func();     /* Call a custom callback            */

        glfwSwapBuffers(this->window_ptr_);
                                        /* Swap the front and back buffers   */
        glfwPollEvents();               /* Process all pending events        */
    }
    // TODO: The code between this and the next 'TODO' is for testing the
    //       developed functionality.
    // TEMPORARY CODE START
                                        /* De-allocate all resources         */
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &indices_buffer);
    glDeleteTextures(1, &texture_2d_array);

    // TODO: TEMPORARY CODE END

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
