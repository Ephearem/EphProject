/**----------------------------------------------------------------------------
; @file Renderer.hpp
;
; @brief
;   This file describes the 'Renderer' class. This class implements the
;   rendering logic.
;   
; @date   May 2021
; @author Eph
;
; // TODO: Implement a text rendering logic.
;
----------------------------------------------------------------------------**/

#pragma once



/** @includes  -------------------------------------------------------------**/

#include <glm/vec2.hpp>



/** @type_declarations -----------------------------------------------------**/

class Shader;
class Sprite;



/** @classes ---------------------------------------------------------------**/

class Renderer
{
public:
    Renderer(Shader* shader_ptr, glm::ivec2 const& scene_size);
    void draw_sprite(Sprite* sprite_ptr, glm::vec2 const& pos,
        glm::vec2 const& size) const;

private:
    Shader* shader_ptr_;
    glm::ivec2 scene_size_;
};
