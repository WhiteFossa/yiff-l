/*
 * Log.h
 *
 *  Created on: Feb 11, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fmgl.h>

extern FMGL_API_DriverContext fmglContext;
extern FMGL_API_FontSettings commonFont;


/**
 * Initialize logger
 */
void Log_Init(void);

/**
 * Displays current log lines
 */
void Log_DrawCurrentLog(void);

/**
 * Add line to the log. If line is too long - leftmost part will be displayed
 */
void Log_AddLine(char* line);

#endif /* INCLUDE_LOG_H_ */
