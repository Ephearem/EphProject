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
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.hpp"
#include "Log.hpp"
#include "Shader.hpp"
#include "Image.hpp"
#include "Texture2dArray.hpp"
#include "Texture2dArrayLayer.hpp"
#include "VertexArray.hpp"
#include "IndicesData.hpp"



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

    this->window_size_ = window_size;
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

    IndicesData* indices_data_1 = nullptr;
                                        /* The 1-st composition's indices    */

    IndicesData* indices_data_2 = nullptr;   
                                        /* The 2-nd composition's indices    */

    VertexArray vertex_array;

    std::vector<float> vertices_1 =     /* The 1-st composition's vertices   */
    {
         1.0f, 0.0f,                    /* Top right                         */
         1.0f, 1.0f,                    /* Bottom right                      */
         0.0f, 1.0f,                    /* Bottom left                       */
         0.0f, 0.0f                     /* Top left                          */
    };
    std::vector<float> txd_vertices_1 = /* The 1st composition's txd vert.   */
    {
         1.0f, 1.0f,                    /* Top right                         */
         1.0f, 0.0f,                    /* Bottom right                      */
         0.0f, 0.0f,                    /* Bottom left                       */
         0.0f, 1.0f                     /* Top left                          */
    };

    std::vector<float> vertices_2 =     /* The 2nd composition's vertices    */
    {
         0.75f, 0.0f,                   /*   _________  Something like that  */
         0.75f, 0.75f,                  /*  |    1    |                      */
         0.0f, 0.75f,                   /*  |       __|______                */
         0.0f, 0.0f,                    /*  |      |  | 2    |               */
         1.0f, 0.25f,                   /*  |      |  |      |               */
         1.0f, 1.0f,                    /*  |______|__|      |               */
         0.25f, 1.0f,                   /*         |         |               */
         0.25f, 0.25f                   /*         |_________|               */
    };
    std::vector<float> txd_vertices_2 = /* The 2nd composition's txd vert.   */
    {
         0.5f, 0.5f,                    /* The 1-st rect's texture vertices  */
         0.5f, 0.0f,
         0.0f, 0.0f,
         0.0f, 0.5f,
         0.5f, 1.0f,                    /* The 2-nd rect's texture vertices  */
         0.5f, 0.75f,
         0.25f, 0.75f,
         0.25f, 1.0f
    };
    indices_data_1 = vertex_array.add_textured_rects(vertices_1,
        txd_vertices_1);                /* Pass the 1-st composition's       */
                                        /* vertices ('vertices_1') and       */
                                        /* texture vertices                  */
                                        /* ('txd_vertices_1') to the         */
                                        /* 'vertex_array' for further        */
                                        /* sending to the GPU                */

    indices_data_2 = vertex_array.add_textured_rects(vertices_2,
        txd_vertices_2);                /* Do the same for the 2-nd          */
                                        /* composition                       */

                                        /* Now 'indices_1' and 'indices_2'   */
                                        /* contain indices data. In the main */
                                        /* loop, these objects will be used  */
                                        /* to draw the first and the second  */
                                        /* composition using the             */
                                        /* 'glDrawElements' function.        */
    
    vertex_array.build();               /* Build a vertex array in the GPU   */
                                        /* using the vertices and texture    */
                                        /* vertices data passed to           */
                                        /* 'add_textured_rects' above        */

    vertex_array.bind();
    
    Image img_1("res/img/512x512_transp.png");
    Image img_2("res/img/256x256.jpg");

    Texture2dArray texture_2d_array(512, 512, 2);
    Texture2dArrayLayer layer_0(&texture_2d_array, 0);
    Texture2dArrayLayer layer_1(&texture_2d_array, 1);
    layer_0.add_subimage(0, 0, 128, 128, 0, 0, img_1.get_data(), 512, 512, 4);
    layer_0.add_subimage(384, 0, 128, 128, 384, 0, img_1.get_data(), 512, 512,
        4);
    layer_0.add_subimage(0, 384, 128, 128, 0, 384, img_1.get_data(), 512, 512,
        4);
    layer_0.add_subimage(384, 384, 128, 128, 384, 384, img_1.get_data(), 512,
        512, 4);
    layer_0.add_subimage(128, 128, 256, 256, 0, 0, img_2.get_data(), 256, 256,
        3);

    layer_1.add_subimage(0, 0, 512, 512, 0, 0, img_1.get_data(), 512, 512, 4);

    texture_2d_array.bind();

    img_1.free();
    img_2.free();

    glm::mat4 projection(1.0);
    projection = glm::ortho(0.0f, static_cast<GLfloat>(this->window_size_.x),
        static_cast<GLfloat>(this->window_size_.y), 0.0f, -0.1f, 0.1f);
                                        /* Create a projection matrix based  */
                                        /* on the size of the window         */
    shader_ptr_->set_mat4("uf_projection", projection);
                                        /* Set the value to the uniform      */

    shader_ptr_->set_int("uf_txd_unit", texture_2d_array.get_texture_unit() -
        GL_TEXTURE0);                   /* Use a texture unit on which       */
                                        /* 'texture_2d_array' is located     */

    shader_ptr_->set_int("uf_txd_unit", texture_2d_array.get_texture_unit() -
        GL_TEXTURE0);                   /* Use a texture unit on which       */
                                        /* 'texture_2d_array' is located     */

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

        this->shader_ptr_->set_vec2("uf_model_pos", (glm::vec2(0, 0)));
        this->shader_ptr_->set_vec2("uf_model_size", (glm::vec2(512, 512)));
        shader_ptr_->set_int("uf_txd_array_z_offset", layer_0.get_z_offset());
        glDrawElements(indices_data_1->mode, indices_data_1->count,
            GL_UNSIGNED_INT, indices_data_1->offset);
                                        /* Draw the 0th layer of the txd     */
                                        /* array using the vertices of the   */
                                        /* 1st composition                   */

        this->shader_ptr_->set_vec2("uf_model_pos", (glm::vec2(512, 0)));
        this->shader_ptr_->set_vec2("uf_model_size", (glm::vec2(256, 128)));
        shader_ptr_->set_int("uf_txd_array_z_offset", layer_1.get_z_offset());
        glDrawElements(indices_data_2->mode, indices_data_2->count,
            GL_UNSIGNED_INT, indices_data_2->offset);
                                        /* Draw the 1st layer of the txd     */
                                        /* array using the vertices of the   */
                                        /* 2nd composition                   */

        // TODO: TEMPORARY CODE END

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
