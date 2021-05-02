/**----------------------------------------------------------------------------
; @file VertexArray.cpp
;
; @brief
;   The file implements functionality of the 'VertexArray' class.
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include <glad/glad.h>

#include "VertexArray.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func VertexArray
;
; @brief
;   Constructor. Generates vertex array objec and related buffer objects needed
;   to store renderable data (vertex buffer, texture vertices buffer, indices
;   buffer).
;
----------------------------------------------------------------------------**/
VertexArray::VertexArray()
    :id_(0), vbo_vertices_(0), vbo_texture_vertices_(0), ibo_(0), 
    next_free_index_number_(0)
{
    glGenVertexArrays(1, &this->id_);   /* Generate a verex array object     */

    glGenBuffers(1, &this->vbo_vertices_);
                                        /* Generate a buffer object to store */
                                        /* positions of the vertices         */

    glGenBuffers(1, &this->vbo_texture_vertices_);
                                        /* Generate a buffer object to store */
                                        /* texture coordinates               */

    glGenBuffers(1, &this->ibo_);       /* Generate a buffer object to store */
                                        /* the vertex indices                */
}


/**----------------------------------------------------------------------------
; @func ~VertexArray
;
; @brief
;   Destructor. Deletes vertex array object and related buffer objects (vertex
;   buffer object, texture vertices buffer object, indices buffer object).
;
;   Note: According to the OpenGL documentation, deleting a currently bound
;   buffer unbinds it implicitly.
;
----------------------------------------------------------------------------**/
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->id_);
    glDeleteBuffers(1, &vbo_vertices_);
    glDeleteBuffers(1, &this->vbo_texture_vertices_);
    glDeleteBuffers(1, &this->ibo_);
}


/**----------------------------------------------------------------------------
; @func add_textured_rect
;
; @brief
;   Adds vertices of a textured rectangle (or multiple rectangles) to the data
;   from which the vertex array will be constructed. Generates an array of
;   indices corresponding to the received vertices.
;   For each textured rect, there are:
;    - 8 elements of a vertex array (i.e. 4 vertices of the rectangle, 2
;      coordinates (x, y) for each);
;    - 8 elements of a texture vertices array (i.e. 4 vertices of the
;      rectangle, 2 coordinates (x, y) for each);
;    - 6 elements of an index array (i.e. 6 vertex indices, listed in the order
;      they are combined to create a rectangle).
;
; @params
;   vertices            | Local coordinates of the vertices of the rectangle.
;   texture_vertices    | The coordinates of the vertices of the texture.
;
; @return
;   None
;
; // TODO: Add a check of the sizes of incoming vectors for a multiple of
;          eight.
;
----------------------------------------------------------------------------**/
void VertexArray::add_textured_rect(std::vector<float> const& vertices,
    std::vector<float> const& texture_vertices)
{
    int rects_number = vertices.size() / 8;
                                        /* 4 points in 2d space (i.e. 8      */
                                        /* values) are used to describe one  */
                                        /* rectangle.                        */

    this->vertices_.insert(this->vertices_.end(), vertices.begin(),
        vertices.end());                /* Store the received vertices       */

    this->texture_vertices_.insert(this->texture_vertices_.end(),
        texture_vertices.begin(), texture_vertices.end());
                                        /* Store the received texture        */
                                        /* vertices                          */

    for (int i = 0; i < rects_number; i++)
    {                                   /* For each rectangle...             */
                                        /* Build indices                     */
        this->indices_.push_back(this->next_free_index_number_ + 0);
        this->indices_.push_back(this->next_free_index_number_ + 1);
        this->indices_.push_back(this->next_free_index_number_ + 3);
        this->indices_.push_back(this->next_free_index_number_ + 1);
        this->indices_.push_back(this->next_free_index_number_ + 2);
        this->indices_.push_back(this->next_free_index_number_ + 3);
        this->next_free_index_number_ += 4;
    }
}


/**----------------------------------------------------------------------------
; @func build
;
; @brief
;   Fills OpenGL buffer objects (vertices, texture vertices and indices) with
;   data (i.e., sends this data to the GPU). Binds these buffer objects to a
;   vertex array. Removes data that was sent to the GPU from RAM.
;
; @params
;   None
;
; @return
;   None
;
; // TODO: Create and use constants as attribute indices (0 for vertices, 1 for
;          texture coordinates).
;
----------------------------------------------------------------------------**/
void VertexArray::build()
{
    int bound_vertex_array_object;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_vertex_array_object);
                                        /* Save the current vertex array     */
                                        /* object name to restore it after   */
                                        /* the function has been processed   */

    this->bind();                       /* Bind a vertex array object        */
                                        /* related to this class object      */

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices_);
                                        /* Bind 'vbo_vertices_' to           */
                                        /* GL_ARRAY_BUFFER. All following    */
                                        /* calls to GL_ARRAY_BUFFER will     */
                                        /* refer to this 'vbo_vertices_'     */
                                        /* object.                           */

    glBufferData(GL_ARRAY_BUFFER, this->vertices_.size() * sizeof(float),
        this->vertices_.data(), GL_STATIC_DRAW);
                                        /* Put data from 'vertices_' into    */
                                        /* GL_ARRAY_BUFFER (i.e. into        */
                                        /* 'vbo_vertices_')                  */

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_texture_vertices_);
                                        /* Bind 'vbo_texture_vertices_' to   */
                                        /* GL_ARRAY_BUFFER. All following    */
                                        /* calls to GL_ARRAY_BUFFER will     */
                                        /* refer to this                     */
                                        /* 'vbo_texture_vertices_' object.   */

    glBufferData(GL_ARRAY_BUFFER, this->texture_vertices_.size() *
        sizeof(float), this->texture_vertices_.data(), GL_STATIC_DRAW);
                                        /* Put data from 'txd_vertices' into */
                                        /* GL_ARRAY_BUFFER (i.e. into        */
                                        /* 'vbo_texture_vertices_')          */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
                                        /* Bind 'ibo_' to                    */
                                        /* GL_ELEMENT_ARRAY_BUFFER. All      */
                                        /* following calls to                */
                                        /* GL_ELEMENT_ARRAY_BUFFER will      */
                                        /* refer to this 'ibo_' object       */

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices_.size() *
        sizeof(unsigned int), this->indices_.data(), GL_STATIC_DRAW);
                                        /* Put data from 'indices_' into     */
                                        /* GL_ELEMENT_ARRAY_BUFFER (i.e.     */
                                        /* into 'ibo_')                      */

    glBindVertexBuffer(0, this->vbo_vertices_, 0, sizeof(GLfloat) * 2);
                                        /* Bind 'vbo_vertices_' to vertex    */
                                        /* array 'id_' at index 0.           */

    glBindVertexBuffer(1, this->vbo_texture_vertices_, 0, sizeof(GLfloat) * 2);
                                        /* Bind 'vbo_texture_vertices_' to   */
                                        /* vertex array 'id_' at index 1.    */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   /* Buffers can be unbound from       */
                                        /* 'GL_ARRAY_BUFFER', since their    */
                                        /* contents are already sent to the  */
                                        /* GPU (as vertex array elements)    */
                                        /* and can be accessed by accessing  */
                                        /* the vertex array at specific      */
                                        /* indices                           */

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                                        /* 'ibo_' cannot be unbound from     */
                                        /* GL_ELEMENT_ARRAY_BUFFER while     */
                                        /* vertex array 'id_' is bound       */


    glBindVertexArray(bound_vertex_array_object);  
                                        /* Restore the vertex array that was */
                                        /* bound before the function call    */
    
    this->vertices_.clear();            /* All this data is already in the   */
    this->texture_vertices_.clear();    /* GPU. Remove it from RAM           */
    this->indices_.clear();
}


/**----------------------------------------------------------------------------
; @func bind
;
; @brief
;   Binds this vertex array object.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void VertexArray::bind() const
{
    glBindVertexArray(this->id_);
}
