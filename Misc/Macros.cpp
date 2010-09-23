/******************************************************************************
/ Macros.cpp
/
/ Copyright (c) 2010 Tim Payne (SWS)
/ http://www.standingwaterstudios.com/reaper
/
/ Permission is hereby granted, free of charge, to any person obtaining a copy
/ of this software and associated documentation files (the "Software"), to deal
/ in the Software without restriction, including without limitation the rights to
/ use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/ of the Software, and to permit persons to whom the Software is furnished to
/ do so, subject to the following conditions:
/ 
/ The above copyright notice and this permission notice shall be included in all
/ copies or substantial portions of the Software.
/ 
/ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/ EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
/ OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/ NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
/ HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/ WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/ FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/ OTHER DEALINGS IN THE SOFTWARE.
/
******************************************************************************/

// Actions to help with macro writing

#include "stdafx.h"
#include "Macros.h"

void WaitAction(COMMAND_T* t)
{
	int iPlayState = GetPlayState();
	if (iPlayState && !(iPlayState & 2)) // playing/recording, not paused
	{
		int iMeasure;
		double dBeat = TimeMap2_timeToBeats(NULL, GetPlayPosition(), &iMeasure, NULL, NULL, NULL);
		if (t->user == 0)
		{	// Bar
			iMeasure++;
			dBeat = 0.0;
		}	// Beat
		else
			dBeat = (double)((int)dBeat + 1);

		// GetPlayPosition doesn't work in this loop, unlike GetPlayPosition2
		double dOffset = GetPlayPosition2() - GetPlayPosition();
		double dStop = TimeMap2_beatsToTime(NULL, dBeat, &iMeasure) + dOffset;
		
		while(GetPlayPosition2() < dStop && GetPlayState())
		{
			MSG msg;
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				DispatchMessage(&msg);
			Sleep(1);
		}
	}
}

static COMMAND_T g_commandTable[] = 
{
	{ { DEFACCEL, "SWS: Wait for next bar (if playing)" },			  "SWS_BARWAIT",		WaitAction, NULL, 0 },
	{ { DEFACCEL, "SWS: Wait for next beat (if playing)" },			  "SWS_BEATWAIT",		WaitAction, NULL, 1 },
	
	{ {}, LAST_COMMAND, }, // Denote end of table
};

int MacrosInit()
{
	SWSRegisterCommands(g_commandTable);

	return 1;
}