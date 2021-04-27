/**----------------------------------------------------------------------------
; @file main.cpp
;
; @brief
;   Contains the entry point to the program.
; 
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes  -------------------------------------------------------------**/

#include "core/Core.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func main_loop_iteration
;
; @brief
;   Main loop iteration logic.
;
; @params
;   None
;
; @return
;   None
;
----------------------------------------------------------------------------**/
void main_loop_iteration()
{

}


/**----------------------------------------------------------------------------
; @func main
;
; @brief
;   Entry point.
;
----------------------------------------------------------------------------**/
int main(int argc, char** argv)
{
    Core::instance().init_window("Eph Project", { 800,600 }, false, 0);
    Core::instance().start_main_loop(main_loop_iteration);
    return 0;
}
