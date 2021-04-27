/**----------------------------------------------------------------------------
; @file Shader.hpp
;
; @brief
;   The file describes the 'Shader' class, which provides creation and
;   interaction with shaders.
; 
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @includes  -------------------------------------------------------------**/

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>



/** @classes ---------------------------------------------------------------**/

class Shader
{
public:
    Shader(const char* vertex_shader_source,
           const char* fragment_shader_source);
    ~Shader();
    void use() const;
    unsigned int get_id() const;

    // TODO: use GL types ?
    void set_int(std::string const& name, int value) const; 
    void set_float(std::string const& name, float value) const; 
    void set_vec2(std::string const& name, const glm::vec2& vec2) const;
    void set_ivec2(std::string const& name, const glm::ivec2& ivec2) const;
    void set_vec4(std::string const& name, const glm::vec4& vec4) const;
    void set_mat4(std::string const& name, const glm::mat4& matrix) const;

private:
    unsigned int id_;
    mutable std::unordered_map<std::string, int> uniform_locations_;
    int get_uniform_location(std::string const& name) const;
};