/**----------------------------------------------------------------------------
; @file Sprite.hpp
;
; @brief
;   This class is implemented to store a minimum set of data required to render
;   a sprite on the screen.
;
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @type_declarations -----------------------------------------------------**/

class IndicesData;
class Texture2dArrayLayer;



/** @classes ---------------------------------------------------------------**/

class Sprite
{
public:
    Sprite(IndicesData* indices_data_ptr_,
           Texture2dArrayLayer* texture_2d_array_layer_ptr);

    IndicesData const* const indices_data_ptr_;
    Texture2dArrayLayer const* const texture_2d_array_layer_ptr_;
};
