//-------------------------------------------------------
//
// File: Logger.h
// Author: Edward Koch
// Description: Holds the Definitions of Logger class
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/14/23    Initial Creation of Logger Class
//-------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>

#include "Logger.h"

bool Logger::enabled = true;
bool Logger::lastStatus = false;

// @purpose: Enables the Logger
// @parameters: None
// @return: None
void Logger::enable()
{
	lastStatus = enabled;
    enabled = true;
}

// @purpose: Disables the Logger
// @parameters: None
// @return: None
void Logger::disable()
{
	lastStatus = enabled;
    enabled = false;
}

// @purpose:Restores the Logger to the status before the last Enable or Disable
// @parameters: None
// @return: None
void Logger::restore()
{
	enabled = lastStatus;
}

// @purpose: Prints a log messge to the console
// @parameters: printf-style parameters
// @return: None
void Logger::log(const char* msg, ...)
{
    if (!enabled) return;

	va_list argp;
	va_start(argp, msg);
	vfprintf(stderr, msg, argp);
	va_end(argp);
}
