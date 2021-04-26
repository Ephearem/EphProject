/**----------------------------------------------------------------------------
; @file Core.hpp
;
; @brief
;   The file describes the singleton class 'Core'. This class implements access
;   to the developed functionality through its object.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/

#pragma once



/** @classes ---------------------------------------------------------------**/

class Core
{
public:
    static Core& instance();

private:
    Core();
    Core(const Core& root) = delete;
    Core& operator=(const Core&) = delete;
};