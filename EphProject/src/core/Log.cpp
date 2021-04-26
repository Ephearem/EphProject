/**----------------------------------------------------------------------------
; @file_path Log.cpp
;
; @brief
;   The file_path implements the logic that is executed when errors or warnings
;   occur.
;
; @date   April 2021
; @author Eph
;
----------------------------------------------------------------------------**/



/** @includes --------------------------------------------------------------**/

#include <iostream>
#include <string>
#include <sstream>

#include "Log.hpp"



/** @functions  ------------------------------------------------------------**/

/**----------------------------------------------------------------------------
; @func log
;
; @brief
;   Prints a message and some debugging information. Pauses/terminates program
;   execution as needed.
; 
; @params
;   flags       | The sum of the values of the 'enLogFlags' enumeration. Used
;               | to define the type of log message and to determine if
;               | additional actions are required (such as pause or terminate
;               | the process after the message is displayed).
;   message     | The text of the message to be displayed.
;   file_path   | Path of the file from which this function was called.
;   line_number | Line number from which this function was called.
;
; @return
;   none
;
; // TODO: Add the ability to log to a file.
; // TODO: Add timecode.
;
----------------------------------------------------------------------------**/
void log(int flags, char const* message, char const* file_path,
         int line_number)
{
    std::stringstream full_message;
    for (int i = 0; i < 79; i++)        /* Put the header in the message     */
    { 
        full_message << "-";
    }
    full_message << std::endl;

    if ((flags & enLogFlags::ERROR) == enLogFlags::ERROR)
    {                                   /* Add the 'ERROR' word if it is an  */
        full_message << "ERROR";        /* error                             */
    }
    else if ((flags & enLogFlags::WARNING) == enLogFlags::WARNING)
    {                                   /* Add the 'WARNING' word if it is a */
        full_message << "WARNING";      /* warning                           */
    }
    full_message << " 0x" << std::hex << flags << ". " << message << std::endl;
                                        /* Add the flags (i.e. error/warning */
                                        /* code)                             */
    full_message << "File: " << file_path << std::endl;
                                        /* Put the path of the file from     */
                                        /* which this function was called    */
                                        /* into the message                  */
    full_message << "Line: " << std::dec << line_number << std::endl;
                                        /* Put the line number from which    */
                                        /* this function was called into the
                                        /* message                           */
    for (int i = 0; i < 79; i++)        /* Put the footer in the message     */
    { 
        full_message << "-";
    }
    full_message << std::endl;

    if ((flags & enLogFlags::ERROR) == enLogFlags::ERROR)
    {                                   /* cerr if an error                  */
        std::cerr << full_message.str();
    }
    else
    {                                   /* Otherwise - cout                  */
        std::cout << full_message.str();
    }

    if ((flags & enLogFlags::PAUSE) == enLogFlags::PAUSE)
    {                                    
        system("pause");                /* Pause the program, if necessary   */
    }
    if ((flags & enLogFlags::TERMINATE) == enLogFlags::TERMINATE)
    {
        exit(flags);                    /* Terminate the program, if         */
    }                                   /* necessary                         */
}