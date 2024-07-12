//-------------------------------------------------------
//
// File: Logger.h
// Author: Edward Koch
// Description: Holds the Declarations of Logger class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/14/23    Initial Creation of Logger Class
//-------------------------------------------------------

#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
public:
    // @purpose: Enables the Logger
    // @parameters: None
    // @return: None
    static void enable();

    // @purpose: Disables the Logger
    // @parameters: None
    // @return: None
    static void disable();

    // @purpose:Restores the Logger to the status before the last Enable or Disable
    // @parameters: None
    // @return: None
    static void restore();

    // @purpose: Prints a log messge to the console
    // @parameters: printf-style parameters
    // @return: None
    static void log(const char* msg, ...);

private:

    // Only prints if the Logger is enabled
    static bool enabled;

    // Holds the last logger value for restoration
    static bool lastStatus;

    // Default Constructor
    Logger();

    // Defautl Destructor
    ~Logger();
};

#endif // LOGGER_H