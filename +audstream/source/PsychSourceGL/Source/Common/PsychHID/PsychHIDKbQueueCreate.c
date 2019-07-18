/*
	PsychtoolboxGL/Source/Common/PsychHID/PsychHIDKbQueueCreate.c		
  
	PROJECTS: 
	
		PsychHID only.
  
	PLATFORMS:  
	
		All.  
  
	AUTHORS:
	
		rwoods@ucla.edu		rpw 
        mario.kleiner@tuebingen.mpg.de      mk
      
	HISTORY:
		8/19/07  rpw		Created.
		8/23/07  rpw        Added PsychHIDQueueFlush to documentation
		12/17/09 rpw		Added support for keypads
  
	NOTES:
	
		The routines PsychHIDKbQueueCreate, PsychHIDKbQueueStart, PsychHIDKbQueueCheck, PsychHIDKbQueueStop
		and PsychHIDKbQueueRelease comprise a replacement for PsychHIDKbCheck, providing the following
		advantages:
		
			1) Brief key presses that would be missed by PsychHIDKbCheck are reliably detected
			2) The times of key presses are recorded more accurately
			3) Key releases are also recorded
		
		It is acceptable to call PsychHIDKbQueueCreate at any time (e.g., to switch to a new device) without
		calling PsychKbQueueRelease.
		
		PsychHIDKbQueueCreate:
			Creates the queue for the specified (or default) device number
			No events are delivered to the queue until PsychHIDKbQueueStart is called
			Can be called again at any time
			
		PsychHIDKbQueueStart:
			Starts delivering keyboard or keypad events from the specified device to the queue
			
		PsychHIDKbQueueStop:
			Stops delivery of new keyboard or keypad events from the specified device to the queue.
			Data regarding events already queued is not cleared and can be recovered by PsychHIDKbQueueCheck
			
		PsychHIDKbQueueCheck:
			Obtains data about keypresses on the specified device since the most recent call to
			this routine or to PsychHIDKbQueueStart
			
			Clears all currently scored events (unscored events may still be in the queue)
			
		PsychHIDKbQueueFlush:
			Flushes unscored events from the queue and zeros all previously scored events
			
		PsychHIDKbQueueRelease:
			Releases queue-associated resources; once called, PsychHIDKbQueueCreate must be invoked
			before using any of the other routines
			
			This routine is called automatically at clean-up and can be omitted at the potential expense of
			keeping memory allocated unnecesarily
				

		---

*/

#include "PsychHID.h"

static char useString[]= "PsychHID('KbQueueCreate', [deviceNumber], [keyFlags])";
static char synopsisString[] = 
        "Creates a queue for events generated by an input device (keyboard, keypad, mouse, ...).\n"
        "By default the first keyboard device (the one with the lowest device number) is "
        "used. If no keyboard is found, the first keypad device is used, followed by other "
        "devices, e.g., mice.  Optionally, the deviceNumber of any keyboard or HID device may be specified.\n"
        "On MS-Windows it is not possible to enumerate different keyboards and mice separately. "
        "Therefore the 'deviceNumber' argument is mostly useless for keyboards and mice. Usually you can "
        "only check the system keyboard or mouse.\n";

static char seeAlsoString[] = "KbQueueStart, KbQueueStop, KbQueueCheck, KbQueueFlush, KbQueueRelease";

PsychError PSYCHHIDKbQueueCreate(void) 
{
	int deviceIndex = -1;
	int numScankeys = 0;
	int* scanKeys = NULL;
	int rc;
	
	PsychPushHelp(useString, synopsisString, seeAlsoString);
	if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

	PsychErrorExit(PsychCapNumInputArgs(2)); // Optionally specifies the deviceNumber of the keyboard or keypad to scan and an array of keyCode indicators.

	// Get optional deviceIndex:
	PsychCopyInIntegerArg(1, FALSE, &deviceIndex);

	// Get optional scanKeys vector:
	PsychAllocInIntegerListArg(2, FALSE, &numScankeys, &scanKeys);

	// Perform actual, OS-dependent init and return its status code:
	rc = PsychHIDOSKbQueueCreate(deviceIndex, numScankeys, scanKeys);

	return(rc);
}