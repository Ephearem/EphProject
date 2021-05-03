/**----------------------------------------------------------------------------
; @file IndicesData.cpp
;
; @brief
;   The file implements the functionality of the 'IndicesData' class.
;
; @date   May 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes --------------------------------------------------------------**/

#include "IndicesData.hpp"



/** @functions -------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func IndicesData
;
; @brief
;   Constructor.
;
; @params
;   mode    | Specifies what kind of primitives to render. Symbolic constants
;           | GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
;           | GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP,
;           | GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY,
;           | GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted. 
;   count   | Number of elements to be rendered.
;   offset  | Offset to the first index of this(these) primitive(s).
;
----------------------------------------------------------------------------**/
IndicesData::IndicesData(unsigned int mode, unsigned int count, void* offset)
    :mode_(mode), count_(count), offset_(offset)
{
}
