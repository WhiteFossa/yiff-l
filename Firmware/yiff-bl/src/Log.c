/*
 * Log.c
 *
 *  Created on: Feb 11, 2022
 *      Author: fossa
 */

#include <Log.h>
#include <LogPrivate.h>

void Log_Init(void)
{
	Log_CurrentLineIndex = 0;

	for (uint8_t line = 0; line < LOG_LINES_COUNT; line ++)
	{
		strncpy(Log_LogLines[line], "", LOG_LINE_LENGTH);
	}
}

void Log_DrawCurrentLog(void)
{
	FMGL_API_ClearScreen(&fmglContext);

	uint16_t y = 0;

	uint16_t outWidth;
	uint16_t outHeight;
	for (uint8_t line = 0; line < LOG_LINES_COUNT; line ++)
	{
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, y, &outWidth, &outHeight, false, Log_LogLines[line]);

		y += LOG_LINE_Y_SHIFT;
	}

	FMGL_API_PushFramebuffer(&fmglContext);
}

void Log_AddLine(char* line)
{
	/* Do we need to shift? */
	if (Log_CurrentLineIndex == LOG_LINES_COUNT)
	{
		for (uint8_t lineIndex = 1; lineIndex < LOG_LINES_COUNT; lineIndex ++)
		{
			strncpy(Log_LogLines[lineIndex - 1], Log_LogLines[lineIndex], LOG_LINE_LENGTH);
		}

		strncpy(Log_LogLines[LOG_LINES_COUNT - 1], line, LOG_LINE_LENGTH);
	}
	else
	{
		strncpy(Log_LogLines[Log_CurrentLineIndex], line, LOG_LINE_LENGTH);
	}

	if (Log_CurrentLineIndex < LOG_LINES_COUNT)
	{
		Log_CurrentLineIndex ++;
	}

	Log_DrawCurrentLog();
}
