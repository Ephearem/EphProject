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



/** @includes  -------------------------------------------------------------**/

#include <glm/vec2.hpp>



/** @type_declarations -----------------------------------------------------**/

struct GLFWwindow;


/** @classes ---------------------------------------------------------------**/

class Core
{
public:
    static Core& instance();

    void init_window(char const* window_title, glm::ivec2 const& window_size,
                     bool is_full_screen, int swap_interval);

    GLFWwindow* get_window_ptr() const;

private:
    GLFWwindow* window_ptr_;
    glm::ivec2 window_size_;

    Core();
    Core(const Core& root) = delete;
    Core& operator=(const Core&) = delete;
};