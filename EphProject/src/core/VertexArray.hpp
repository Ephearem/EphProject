/**----------------------------------------------------------------------------
; @file VertexArray.hpp
;
; @brief
;   This file describes the 'VertexArray' class that implements logic for
;   creating, filling with data, building and freeing vertex arrays.
;
;   Creation of a vertex array is implemented in the constructor of the
;   class and consists in generating a vertex array object and related buffer
;   objects needed to store renderable data (vertex buffer, texture vertices
;   buffer, indices buffer).
;
;   Filling a vertex array with data is implemented in the 'add_textured_rects'
;   method and consists in adding vertices to the data required to build the
;   vertex array, generating an array of indices corresponding to this
;   vertices and creating an object of 'IndicesData' class that contains the
;   data needed to draw the added rectangle.
;
;   Building of a vertex array is:
;       - filling in OpenGL buffer objects (vertices, texture vertices and)
;         indices with data (i.e., sending this data to the GPU)
;       - binding these buffer objects to a vertex array.
;
;   Freeing a vertex array consists in unbinding and deleting the vertex array
;   object and related buffers.
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @includes --------------------------------------------------------------**/

#include <vector>



/** @type_declarations -----------------------------------------------------**/

class IndicesData;



/** @classes ---------------------------------------------------------------**/

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    IndicesData* add_textured_rects(std::vector<float> const& vertices,
                           std::vector<float> const& texture_vertices);

    void build();
    void bind() const;

private:
    unsigned int id_;
    unsigned int vbo_vertices_;
    unsigned int vbo_texture_vertices_;
    unsigned int ibo_;

    std::vector<float> vertices_;
    std::vector<float> texture_vertices_;
    std::vector<unsigned int> indices_;

    unsigned int next_free_index_number_;
};
