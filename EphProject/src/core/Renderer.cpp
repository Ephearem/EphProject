/**----------------------------------------------------------------------------
; @file Renderer.cpp
;
; @brief
;   The file implements the functionality of the 'Renderer' class.
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @includes  -------------------------------------------------------------**/

#include <glad/glad.h>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Texture2dArrayLayer.hpp"
#include "IndicesData.hpp"



/** @functions -------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Renderer
;
; @brief
;   Constructor. Initializes the fields of the class. Generates a projection
;   matrix based on the size of the scene. Sends it to the shader program as a
;   uniform variable.
;
; @params
;   shader_ptr  | Pointer to the shader used for rendering.
;   scene_size  | Scene (window) size.
;
----------------------------------------------------------------------------**/
Renderer::Renderer(Shader* shader_ptr, glm::ivec2 const& scene_size)
    :shader_ptr_(shader_ptr), scene_size_(scene_size)
{
    glm::mat4 projection(1.0);
    projection = glm::ortho(0.0f, static_cast<GLfloat>(scene_size.x),
        static_cast<GLfloat>(scene_size.y), 0.0f, -0.1f, 0.1f);
                                        /* Create a projection matrix based  */
                                        /* on the scene size                 */

    this->shader_ptr_->set_mat4("uf_projection", projection);
                                        /* Send it to the shader program     */
}


/**----------------------------------------------------------------------------
; @func draw_sprite
;
; @brief
;   The function sets some presets for rendering a sprite (binds the texture 2d
;   array, sets the texture unit and layer number), and then renders the sprite
;   using the indexes.
;   In fact, setting presets is a CPU->GPU transfer that is time consuming.
;   For optimization, the last used values(which are already in the GPU) are
;   stored in static variables. Data is sent to the GPU only if the new values
;   are not equal to the previous ones.
;
; @params
;   sprite_ptr  | Sprite object pointer.
;   pos         | Sprite position (in pixels).
;   size        | Sprite size (in pixels).
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Renderer::draw_sprite(Sprite* sprite_ptr, glm::vec2 const& pos,
    glm::vec2 const& size) const
{
    // static Texture2dArray* last_texture_2d_array_ = nullptr;
    // static Texture2dArrayLayer* last_texture_2d_array_layer = nullptr;


                                        /* Sending data to the shader and    */
                                        /* binding OpenGL objects can be     */
                                        /* time consuming. The following     */
                                        /* logic prevents unnecessary data   */
                                        /* transfers to shaders and          */
                                        /* unnecessary 2d texture array      */
                                        /* bindings.                         */
    static unsigned int prev_texture_2d_array_id = 0;
    static int prev_texture_unit = 0;
    static int prev_texture_2d_array_z_offset = 0;

                                        /* Get presets for rendering the     */
                                        /* current sprite:                   */
    unsigned int cur_texture_2d_array_id = sprite_ptr->
        texture_2d_array_layer_ptr_->get_texture_2d_array()->get_id();
                                        /* Get current sprite's texture 2d   */
                                        /* array id                          */

    int cur_texture_unit = sprite_ptr->texture_2d_array_layer_ptr_->
        get_texture_2d_array()->get_texture_unit();
                                        /* Get current sprite's texture unit */

    int cur_texture_2d_array_z_offset = sprite_ptr->
        texture_2d_array_layer_ptr_->get_z_offset();
                                        /* Get current sprite's texture 2d   */
                                        /* array layer number                */


    // TODO: Check if the if-statement below makes the function faster.
    if (prev_texture_2d_array_id != cur_texture_2d_array_id)
    {                                   /* Compare the current sprite's      */
                                        /* texture 2d array with the         */
                                        /* currently bound 2d texture array  */

        sprite_ptr->texture_2d_array_layer_ptr_->get_texture_2d_array()->
            bind();                      /* Bind the new 2d texture array    */
                                         /* only if it has changed           */

        prev_texture_2d_array_id = cur_texture_2d_array_id;
                                        /* Update the current state of the   */
                                        /* static variable for the next      */
                                        /* function call                     */
    }

    // TODO: Check if the if-statement below makes the function faster.
    if (prev_texture_unit != cur_texture_unit)
    {                                   /* Compare the current sprite's      */
                                        /* texture unit with the value       */
                                        /* currently set in the shader       */
        shader_ptr_->set_int("uf_txd_unit", cur_texture_unit - GL_TEXTURE0);
                                        /* Send the new texture unit number  */
                                        /* to the GPU only if it has changed */ 

        prev_texture_unit = cur_texture_unit;
                                        /* Update the current state of the   */
                                        /* static variable for the next      */
                                        /* function call                     */
    }

    // TODO: Check if the if-statement below makes the function faster.
    if (prev_texture_2d_array_z_offset != cur_texture_2d_array_z_offset)
    {                                   /* Compare the current sprite's      */
                                        /* texture 2d array layer number     */
                                        /* with the value currently set in   */
                                        /* the shader                        */

        shader_ptr_->set_int("uf_txd_array_z_offset",
            cur_texture_2d_array_z_offset);
                                        /* Send the new z offset to the GPU  */
                                        /* only if it has changed            */

        prev_texture_2d_array_z_offset = cur_texture_2d_array_z_offset;
                                        /* Update the current state of the   */
                                        /* static variable for the next      */
                                        /* function call                     */
    }

    // TODO: 'uf_model_pos' and 'uf_model_size' must be in the same vec4.
    this->shader_ptr_->set_vec2("uf_model_pos", pos);
    this->shader_ptr_->set_vec2("uf_model_size", size);

    glDrawElements(sprite_ptr->indices_data_ptr_->mode,
        sprite_ptr->indices_data_ptr_->count,
        GL_UNSIGNED_INT, sprite_ptr->indices_data_ptr_->offset);
}