/**----------------------------------------------------------------------------
; @file Texture2dArray.hpp
;
; @brief
;   The file describes the 'Texture2dArray' class that implements the logic
;   for creating arrays of 2d textures.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @classes ---------------------------------------------------------------**/

class Texture2dArray
{
public:
    Texture2dArray(int width, int height, int depth);
    ~Texture2dArray();

    void bind() const;

    unsigned int get_id() const;
    int get_width() const;
    int get_height() const;
    int get_texture_unit() const;
private:
    unsigned int id_;
    int width_;
    int height_;
    int depth_;
    unsigned int texture_unit_;
    static unsigned int free_texture_images_unit_;
};