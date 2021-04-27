/**----------------------------------------------------------------------------
; @file Image.hpp
;
; @brief
;   The file describes the 'Image' class that loads the image into a byte
;   array, stores the image size and color depth.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @classes ---------------------------------------------------------------**/

class Image
{
public:
    Image(char const* image_path);
    ~Image();

    void free();
    const unsigned char* get_data() const;
    int get_width() const;
    int get_height() const;

private:
    int width_;
    int height_;
    int channels_count_;
    unsigned char* data_;
};