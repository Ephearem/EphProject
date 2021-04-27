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



/** @classes ---------------------------------------------------------------**/

class Shader
{
public:
    Shader(const char* vertex_shader_source,
           const char* fragment_shader_source);
    ~Shader();
    void use() const;
    unsigned int get_id() const;

private:
    unsigned int id_;
};