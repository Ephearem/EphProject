/**----------------------------------------------------------------------------
; @file Log.hpp
;
; @brief
;   The file contains the declaration of functions, macros and data types
;   required to implement the logic that is executed when errors, warnings or
;   logs occur.
; 
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @enums -----------------------------------------------------------------**/

enum enLogFlags
{
    MSG = 0x1,
    WARNING = 0x2,
    ERROR = 0x4,
    PAUSE = 0x8,
    TERMINATE = 0x10,
};



/** @defines ---------------------------------------------------------------**/

#define LOG_ERROR(message) \
log(enLogFlags::ERROR | enLogFlags::PAUSE | enLogFlags::TERMINATE, message,\
__FILE__, __LINE__);

#define LOG_WARNING(message) \
log(enLogFlags::WARNING, message, __FILE__, __LINE__);



/** @function_prototypes ---------------------------------------------------**/

void log(int flags, char const* message, char const* file_path,
    int line_number);
