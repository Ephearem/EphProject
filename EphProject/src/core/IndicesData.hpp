/**----------------------------------------------------------------------------
; @file IndicesData.hpp
;
; @brief
;   This file describes the 'IndicesData' class. This class is implemented to
;   store data about contents of the OpenGL buffer 'GL_ELEMENT_ARRAY_BUFFER'.
;   This data is needed to draw something using the 'glDrawElements' function.
;   In fact, an object of this class stores arguments for the 'glDrawElements'
;   function.
;   This class only stores data. The presence of an object of this class does
;   not guarantee that the indexes described in it actually exist in the GPU.
;
; @date   May 2021
; @author Eph
;
; // TODO: make the constructor private. Use the friendly class 'VertexArray'
;          to create objects.
;
----------------------------------------------------------------------------**/



/** @classes ---------------------------------------------------------------**/

class IndicesData
{
public:
    IndicesData(unsigned int mode, unsigned int count, void* offset);

    const unsigned int mode_;           /* Vertices connection mode          */
    const unsigned int count_;          /* Number of elements to be rendered */
    const void* offset_;                /* Offset to the first index of      */
                                        /* shape(s)                          */
};
