/**----------------------------------------------------------------------------
; @file Shader.cpp
;
; @brief
;   // TODO: 
; 
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include <glad/glad.h>

#include "Shader.hpp"
#include "Log.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Shader
;
; @brief
;   Constructor. Creates (i.e. compiles and links) a shader program from the
;   source code of the vertex and fragment shaders 
; 
; @params
;   vertex_shader_source    | Zero-terminated vertex shader source string.
;   fragment_shader_source  | Zero-terminated fragment shader source string.
;
----------------------------------------------------------------------------**/
Shader::Shader(const char* vertex_shader_source, const char* fragment_shader_source)
{
    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;
    int result = 0;
    const size_t log_info_len = 512;
    char log_info[log_info_len];

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
                                        /* Create a vertex shader object     */
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
                                        /* Set the source code in the vertex */
                                        /* shader                            */
    glCompileShader(vertex_shader);     /* Compile the vertex shader         */
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
                                        /* Get the compilation status of the */
                                        /* vertex shader                     */
    if (!result)
    {
        glGetShaderInfoLog(vertex_shader, log_info_len, nullptr, log_info);
        //LOG_ERROR("Failed to compile a vertex shader: " + log_info);
        LOG_ERROR(log_info);
    }
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
                                        /* Create a fragment shader object   */
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
                                        /* Set the source code in the        */
                                        /* fragment shader                   */
    glCompileShader(fragment_shader);   /* Compile the fragment shader       */
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
                                        /* Get the compilation status of the */
                                        /* fragment shader                   */
    if (!result)
    {
        glGetShaderInfoLog(fragment_shader, log_info_len, nullptr, log_info);
        //LOG_ERROR("Failed to compile a fragment shader: " + log_info);
        LOG_ERROR(log_info);
    }
    this->id_ = glCreateProgram();      /* Create a shader program object    */
    glAttachShader(this->id_, vertex_shader);
                                        /* Attach the fragment shader to the */
                                        /* shader program                    */
    glAttachShader(this->id_, fragment_shader);
                                        /* Attach the fragmemt shader to the */
                                        /* shader program                    */
    glLinkProgram(this->id_);           /* Link the shader program           */
    glGetProgramiv(this->id_, GL_LINK_STATUS, &result);
                                        /* Get the linking status of the     */
                                        /* shader program.                   */
    if (!result) {
        glGetProgramInfoLog(this->id_, log_info_len, nullptr, log_info);
        //LOG_ERROR("Failed to link shader program: " + log_info);
        LOG_ERROR(log_info);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}


/**----------------------------------------------------------------------------
; @func ~Shader
;
; @brief
;   Frees the memory and invalidates the name associated with the program
;   object specified by program.
; 
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
Shader::~Shader()
{
    glDeleteProgram(this->id_);
}


/**----------------------------------------------------------------------------
; @func use
;
; @brief
;   Installs the program object specified by program as part of current
;   rendering state.
; 
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::use() const
{
    glUseProgram(this->id_);
}


/**----------------------------------------------------------------------------
; @func get_id
;
; @brief
;   Returns the id of the shader program object.
; 
; @params
;
; @return
;   unsigned int    | id of the shader program object.
;
----------------------------------------------------------------------------**/
unsigned int Shader::get_id() const
{
    return this->id_;
}
