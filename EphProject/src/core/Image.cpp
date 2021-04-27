/**----------------------------------------------------------------------------
; @file Image.cpp
;
; @brief
;   The file implements the functionality of the 'Image' class.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @defines ---------------------------------------------------------------**/

#define STB_IMAGE_IMPLEMENTATION



/** @includes  -------------------------------------------------------------**/

#include <stb_image.h>

#include "Image.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Image
;
; @brief
;   Constructor. Loads image pixels into a byte array, stores the image size
;   and color depth.
;
; @params
;   image_path  | The path to the image.
;
----------------------------------------------------------------------------**/
Image::Image(char const* image_path)
    :width_(0), height_(0), channels_count_(0), data_(nullptr)
{
    // TODO: use std::filesystem
    stbi_set_flip_vertically_on_load(true);
                                        /* To flip loaded image's on the     */
                                        /* y-axis.                           */
    this->data_ = stbi_load(image_path, &this->width_, &this->height_,
        &this->channels_count_, 0);
}


/**----------------------------------------------------------------------------
; @func ~Image
;
; @brief
;   Destructor. For more details see the description of the 'Image::free'
;   method.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
Image::~Image()
{
    this->free();
}


/**----------------------------------------------------------------------------
; @func free
;
; @brief
;   Unloads an image from memory. Zeroes a pointer to image data.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Image::free()
{
    if (this->data_ != nullptr)
    {
        stbi_image_free(this->data_);
        this->data_ = nullptr;  // TODO: delete this line?
    }
}


/**----------------------------------------------------------------------------
; @func get_data
;
; @brief
;   Returns a pointer to an array containing information about the colors of
;   all pixels.
;
; @params
;   None
;
; @return
;   const unsigned char *   | A pointer to an array containing information
;   about the colors of all pixels.
;
----------------------------------------------------------------------------**/
const unsigned char* Image::get_data() const
{
    return this->data_;
}


/**----------------------------------------------------------------------------
; @func get_width
;
; @brief
;   Returns the width of the image (in pixels).
;
; @params
;   None
;
; @return
;   int | Width of the image (in pixels).
;
----------------------------------------------------------------------------**/
int Image::get_width() const
{
    return this->width_;
}


/**----------------------------------------------------------------------------
; @func get_height
;
; @brief
;   Returns the height of the image (in pixels).
;
; @params
;   None
;
; @return
;   int |  Height of the image (in pixels).
;
----------------------------------------------------------------------------**/
int Image::get_height() const
{
    return this->height_;
}
