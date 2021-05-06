/**----------------------------------------------------------------------------
; @file Sprite.cpp
;
; @brief
;   The file implements the functionality of the 'Sprite' class.
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/


/** @includes  -------------------------------------------------------------**/

#include "Sprite.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Sprite
;
; @brief
;   Constructor. Initializes the fields of the class.
;
; @params
;   indices_data_ptr_           | A texture layer data for this sprite.
;   texture_2d_array_layer_ptr  | Data about the vertices and texture vertices
;                               | that make up the srpite.
;
----------------------------------------------------------------------------**/
Sprite::Sprite(IndicesData* indices_data_ptr_,
               Texture2dArrayLayer* texture_2d_array_layer_ptr)
    :indices_data_ptr_(indices_data_ptr_),
    texture_2d_array_layer_ptr_(texture_2d_array_layer_ptr)
{}
