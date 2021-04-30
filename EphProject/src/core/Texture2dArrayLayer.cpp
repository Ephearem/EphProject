/**----------------------------------------------------------------------------
; @file Texture2dArrayLayer.cpp
;
; @brief
;    The file implements the functionality of the 'Texture2dArrayLayer' class.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include <glad/glad.h>

#include "Texture2dArrayLayer.hpp"
#include "Texture2dArray.hpp"
#include "Log.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Texture2dArrayLayer
;
; @brief
;   Constructor.
;
; @params
;   texture_2d_array_ptr    | Pointer to the 2d texture array this layer
;                           | belongs to.
;   z_offset                | Z-offset in an array of 2d textures.
;
----------------------------------------------------------------------------**/
Texture2dArrayLayer::Texture2dArrayLayer(Texture2dArray* texture_2d_array_ptr,
    int z_offset)
    :texture_2d_array_ptr_(texture_2d_array_ptr), z_offset_(z_offset)
{
}


/**----------------------------------------------------------------------------
; @func add_texture
;
; @brief
;   Loads an image (or subimage) onto a 2d texture array layer
;
; @params
;   subtexture_x_offset | X-offset from the beginning of the layer.
;   subtexture_y_offset | Y-offset from the beginning of the layer.
;   subtexture_width    | Width of the texture to be added to the layer.
;   subtexture_hight    | Height of the texture to be added to the layer.
;   img_x_offset        | X-offset from the beginning of the image.
;   img_y_offset        | Y-offset from the beginning of the image.
;   img_bytes           | A pointer to the image pixels data.
;   img_width           | Width of the full image.
;   img_height          | Height of the full image.
;   img_channels_count_ | Bytes per pixel in the image data.
;
; @return
;   None
;
; // TODO: Check if the sub-texture fits on the layer.
;
----------------------------------------------------------------------------**/
void Texture2dArrayLayer::add_texture(int subtexture_x_offset,
    int subtexture_y_offset,
    int subtexture_width,
    int subtexture_hight,
    int img_x_offset, int img_y_offset,
    unsigned char const* img_bytes,
    int img_width, int img_height,
    int img_channels_count_)
{

    GLenum format = 0;
    switch (img_channels_count_)
    {
    case 4:                             /* If 4 bytes per pixel              */
        format = GL_RGBA;               /* It's RGBA                         */
        break;
    case 3:                             /* if 3 bytes per pixel              */
        format = GL_RGB;                /* it's RGB                          */
        break;
    default:                            /* Otherwise, log an error           */
        LOG_ERROR("Undefined image format.");
        break;
        // TODO: Provide functionality for other formats.
    }
    this->texture_2d_array_ptr_->bind();
    glActiveTexture(this->texture_2d_array_ptr_->get_texture_unit());
    glPixelStorei(GL_UNPACK_ROW_LENGTH, img_width);
                                        /* The full width of the image from  */
                                        /* which the texture is created      */
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, img_x_offset);
                                        /* Subimage x-offset (from the       */
                                        /* beginning of the image).          */
    glPixelStorei(GL_UNPACK_SKIP_ROWS, img_height - img_y_offset
        - subtexture_hight);            /* Subimage y-offset (from the       */
                                        /* beginning of the image).          */
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,            /* Target to which the texture is    */
                                        /* bound                             */
        0,                              /* Level-of-detail. 0 - base image   */
        subtexture_x_offset,            /* X-offset within the texture array */
        subtexture_y_offset,            /* Y offset within the texture array */

                                        /* The y-offset is calculated from   */
                                        /* the bottom of the texture. For it */
                                        /* to be considered from the top,    */
                                        /* need to swap the vertices related */
                                        /* to the texture in the 'vertices'  */
                                        /* array.                            */
        // TODO: Check out the comment posted above because I don't remember
        //       exactly :)

        this->z_offset_,                /* Z offset (layer)                  */
        subtexture_width,               /* Width of the texture subimage     */
        subtexture_hight,               /* Height of the texture subimage    */
        1,                              /* Depth of the texture subimage     */
        format,                         /* Format of the pixel data          */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data       */
        static_cast<const void*>(img_bytes));
                                        /* Image pixels data pointer         */
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
                                        /* Unbind the texture 2d array       */

    // TODO: Restore GL_TEXTURE_2D_ARRAY to the state it was in before calling
    //       the constructor.
}


/**----------------------------------------------------------------------------
; @func get_z_offset
;
; @brief
;   Returns the z-offset of the current layer.
;
; @params
;   None
;
; @return
;   int | Z-offset of the current layer.
;
----------------------------------------------------------------------------**/
int Texture2dArrayLayer::get_z_offset() const
{
    return this->z_offset_;
}


/**----------------------------------------------------------------------------
; @func get_texture_2d_array
;
; @brief
;   Returns a pointer to the 'Texture2dArray' object that this layer belongs
;   to.
;
; @params
;   None
;
; @return
;   Texture2dArray *    | Texture 2d array object that this layer belongs to.
;
----------------------------------------------------------------------------**/
Texture2dArray* Texture2dArrayLayer::get_texture_2d_array() const
{
    return this->texture_2d_array_ptr_;
}
