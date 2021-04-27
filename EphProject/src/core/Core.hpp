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
class Shader;



/** @classes ---------------------------------------------------------------**/

class Core
{
public:
    static Core& instance();

    void init_window(char const* window_title, glm::ivec2 const& window_size,
                     bool is_full_screen, int swap_interval);

    void init_shaders(const char* vertex_shader_file_path,
                      const char* fragment_shader_file_path);

    void start_main_loop(void(*main_loop_iteration_func)());

    GLFWwindow* get_window_ptr() const;

private:
    GLFWwindow* window_ptr_;
    glm::ivec2 window_size_;
    Shader* shader_ptr_;
    void(*main_loop_iteration_func_)();

    Core();
    Core(const Core& root) = delete;
    Core& operator=(const Core&) = delete;
};