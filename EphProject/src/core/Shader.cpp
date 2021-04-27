/**----------------------------------------------------------------------------
; @file Shader.cpp
;
; @brief
;   The file implements the functionality of the 'Shader' class.
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
Shader::Shader(const char* vertex_shader_source,
    const char* fragment_shader_source)
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


/**----------------------------------------------------------------------------
; @func set_int
;
; @brief
;   Specifies the value of a uniform variable of type int for the current
;   program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_int(std::string const& name, int value) const
{
    glUniform1i(this->get_uniform_location(name), value);
}


/**----------------------------------------------------------------------------
; @func set_float
;
; @brief
;   Specifies the value of a uniform variable of type float for the current
;   program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_float(std::string const& name, float value) const
{
    glUniform1f(this->get_uniform_location(name), value);
}


/**----------------------------------------------------------------------------
; @func set_vec2
;
; @brief
;   Specifies the value of a uniform variable of type glm::vec2 for the current
;   program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_vec2(std::string const& name, const glm::vec2& vec2) const
{
    glUniform2f(this->get_uniform_location(name), vec2.x, vec2.y);
}


/**----------------------------------------------------------------------------
; @func set_ivec2
;
; @brief
;   Specifies the value of a uniform variable of type glm::ivec2 for the
;   current program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_ivec2(std::string const& name, const glm::ivec2& ivec2) const
{
    glUniform2i(this->get_uniform_location(name), ivec2.x, ivec2.y);
}


/**----------------------------------------------------------------------------
; @func set_vec4
;
; @brief
;   Specifies the value of a uniform variable of type glm::vec4 for the current
;   program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_vec4(std::string const& name, const glm::vec4& vec4) const
{
    // TODO: use glUniform4f
    glUniform4f(this->get_uniform_location(name), vec4.x, vec4.y, vec4.z,
        vec4.w);
}


/**----------------------------------------------------------------------------
; @func set_mat4
;
; @brief
;   Specifies the value of a uniform variable of type glm::mat4 for the current
;   program object.
;
; @params
;   name    | Uniform variable name.
;   value   | A new value for a uniform variable.
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Shader::set_mat4(std::string const& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(this->get_uniform_location(name), 1, GL_FALSE,
        glm::value_ptr(matrix));
}


/**----------------------------------------------------------------------------
; @func get_uniform_location
;
; @brief
;   Returns the location of a uniform variable by its name.
;   The first call to this function for each name gets the location using the
;   'glGetUniformLocation' function and stores it in an unordered map, in which
;   the key is the uniform name. The next calls to this function will not
;   access the GPU (i.e. will not call 'glGetUniformLocation'), but will return
;   the value from the map. This reduces the execution time of the function.
;
; @params
;   name    | Uniform variable name.
;
; @return
;   int     | Uniform variable location.
;
----------------------------------------------------------------------------**/
int Shader::get_uniform_location(std::string const& name) const
{
    auto i = this->uniform_locations_.find(name);
    if (i != this->uniform_locations_.end())
        return i->second;
    int uniform_location = glGetUniformLocation(this->id_, name.c_str());
    this->uniform_locations_[name] = uniform_location;
    return uniform_location;
}
