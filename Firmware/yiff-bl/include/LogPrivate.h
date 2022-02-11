/*
 * LogPrivate.h
 *
 *  Created on: Feb 11, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_LOGPRIVATE_H_
#define INCLUDE_LOGPRIVATE_H_

/**
 * How many log lines to display
 */
#define LOG_LINES_COUNT 9U

/**
 * Maximal log line length (null-terminator not included)
 */
#define LOG_LINE_LENGTH 32U

/**
 * Log line memory size
 */
#define LOG_LINE_MEMORY_SIZE (LOG_LINE_LENGTH + 1U)


/**
 * Next line will be lower than the previous line to this value
 */
#define LOG_LINE_Y_SHIFT 14U

/**
 * Log lines
 */
char Log_LogLines[LOG_LINES_COUNT][LOG_LINE_MEMORY_SIZE];

/**
 * New line will be stored at this index
 */
uint8_t Log_CurrentLineIndex;


#endif /* INCLUDE_LOGPRIVATE_H_ */
