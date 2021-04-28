/**----------------------------------------------------------------------------
; @file Texture2dArray.cpp
;
; @brief
;   The file implements the functionality of the 'Texture2dArray' class.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include <glad/glad.h>

#include "Texture2dArray.hpp"
#include "Log.hpp"



/** @data_definitions  -----------------------------------------------------**/

unsigned int Texture2dArray::free_texture_images_unit_ = GL_TEXTURE0;



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func Texture2dArray
;
; @brief
;   Constructor. Generates an array of 2D textures.
;
; @params
;   width   | Width of the 2d texture array.
;   height  | Heigh of the 2d texture array.
;   depth   | Depth of the 2d texture array.
;
----------------------------------------------------------------------------**/
Texture2dArray::Texture2dArray(int width, int height, int depth)
    :width_(width), height_(height), depth_(depth)
{
    int dest = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &dest);
                                        /* Get the maximum supported texture */
                                        /* image units that can be used to   */
                                        /* access texture maps from the      */
                                        /* fragment shader                   */
    if (Texture2dArray::free_texture_images_unit_ - GL_TEXTURE0 >=
        static_cast<unsigned int>(dest))
    {                                   /* Compare this with the number of   */
                                        /* the free texture unit             */
        LOG_ERROR("Unable to create texture 2d array. All texture units are \
used.");                                /* Log an error if all texture       */
    }                                   /* units are used                    */

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &dest);
                                        /* Get the maximum supported texture */
                                        /* image size                        */
    if (width > dest || height > dest)
    {
        LOG_ERROR("Unable to create texture 2d array. The maximum supported \
texture image size has been exceeded.")
    }

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &dest);
                                        /* Get the maximum supported texture */
                                        /* 2d array depth                    */
    if (depth > dest)
    {
        LOG_ERROR("Unable to create texture 2d array. The maximum number of \
texture layers has been exceeded.")
    }

    this->texture_unit_ = Texture2dArray::free_texture_images_unit_;

    glActiveTexture(this->texture_unit_);
    glGenTextures(1, &this->id_);       /* Generate a 2d texture array       */
                                        /* object                            */
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->id_);
                                        /* Set 'texture_2d_array' as the     */
                                        /* current vertex array object       */
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,            /* target to which the texture is    */
                                        /* bound                             */
        0,                              /* level                             */
        GL_RGBA8,                       /* Internal format                   */
        (this->width_),                 /* Width of the 2d texture array     */
        this->height_,                  /* Heigh of the 2d texture array     */
        this->depth_,                   /* Depth of the 2d texture array     */
        0,                              /* Border, must be 0                 */
        GL_RGBA,                        /* Format of the pixel data          */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data       */
        nullptr);                       /* A pointer to the image data       */

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
                                        /* Unbind the texture 2d array       */

    // TODO: Restore GL_TEXTURE_2D_ARRAY to the state it was in before calling
    //       the constructor.

    Texture2dArray::free_texture_images_unit_++;
                                        /* Increase the number of used texture
                                        /* units by 1                        */
}


/**----------------------------------------------------------------------------
; @func ~Texture2dArray
;
; @brief
;   Destructor. Removes the texture. If a texture that is currently bound is
;   deleted, the binding reverts to 0 (the default texture).
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
Texture2dArray::~Texture2dArray()
{
    glDeleteTextures(1, &this->id_);
}


/**----------------------------------------------------------------------------
; @func bind
;
; @brief
;   Binds this texture 2d array to GL_TEXTURE_2D_ARRAY.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void Texture2dArray::bind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->id_);
}


/**----------------------------------------------------------------------------
; @func get_id
;
; @brief
;   Returns texture 2d array OpenGL object id.
;
; @params
;   None
;
; @return
;   unsigned int    | Texture 2d array OpenGL object id.
;
----------------------------------------------------------------------------**/
unsigned int Texture2dArray::get_id() const
{
    return this->id_;
}


/**----------------------------------------------------------------------------
; @func get_width
;
; @brief
;   Returns the height of the texture.
;
; @params
;   None
;
; @return
;   int | Height of the texture.
;
----------------------------------------------------------------------------**/
int Texture2dArray::get_width() const
{
    return this->width_;
}


/**----------------------------------------------------------------------------
; @func get_height
;
; @brief
;   Returns the width of the texture.
;
; @params
;   None
;
; @return
;   int | Width of the texture.
;
----------------------------------------------------------------------------**/
int Texture2dArray::get_height() const
{
    return this->height_;
}


/**----------------------------------------------------------------------------
; @func get_texture_unit
;
; @brief
;    Returns a texture unit of this texture 2d array layer.
;
; @params
;   None
;
; @return
;   int | Texture unit.
;
----------------------------------------------------------------------------**/
int Texture2dArray::get_texture_unit() const
{
    return this->texture_unit_;
}