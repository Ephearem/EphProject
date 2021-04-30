/**----------------------------------------------------------------------------
; @file Texture2dArrayLayer.hpp
;
; @brief
;   This file describes the 'Texture2dArrayLayer' class that implements logic
;   for loading images (or subimages) onto layers of a 2D texture array.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @includes  -------------------------------------------------------------**/

#include "Texture2dArray.hpp"



/** @type_declarations -----------------------------------------------------**/

class Texture2dArray;



/** @classes ---------------------------------------------------------------**/

class Texture2dArrayLayer
{
public:
    Texture2dArrayLayer(Texture2dArray* texture_2d_array_ptr, int z_offset);

    void add_texture(int subtexture_x_offset, int subtexture_y_offset,
        int subtexture_width, int subtexture_hight,
        int img_x_offset, int img_y_offset,
        unsigned char const* img_bytes, int img_width, int img_height,
        int img_channels_count_);

    int get_z_offset() const;
    Texture2dArray* get_texture_2d_array() const;
private:
    int z_offset_;
    Texture2dArray* texture_2d_array_ptr_;
};