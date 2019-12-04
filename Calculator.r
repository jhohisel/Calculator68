#include "Types.r"

/* These define's are used in the MENU resources to disable specific
   menu items. */
#define AllItems	0b1111111111111111111111111111111	/* 31 flags */
#define MenuItem1	0b00001
#define MenuItem2	0b00010
#define MenuItem3	0b00100
#define MenuItem4	0b01000
#define MenuItem5	0b10000


resource 'MENU' (1000, "Apple", preload) {
	1000, textMenuProc,
	AllItems & ~MenuItem2,	/* Disable item #2 */
	enabled, apple,
	{
		"About Calculator",
			noicon, nokey, nomark, plain;
		"-",
			noicon, nokey, nomark, plain
	}
};

resource 'MENU' (1001, "File", preload) {
	1001, textMenuProc,
	AllItems & ~MenuItem2,	/* Disable item #2 */
	enabled, "File",
	{
		"Quit",
			noicon, "Q", nomark, plain
	}
};

resource 'MENU' (1002, "Edit", preload) {
	1002, textMenuProc,
	AllItems & ~MenuItem2,	/* Disable items #2 */
	enabled, "Edit",
	 {
		"Undo",
			noicon, "Z", nomark, plain;
		"-",
			noicon, nokey, nomark, plain;
		"Cut",
			noicon, "X", nomark, plain;
		"Copy",
			noicon, "C", nomark, plain;
		"Paste",
			noicon, "V", nomark, plain;
		"Clear",
			noicon, nokey, nomark, plain
	}
};


resource 'WIND' (1000, "Calculator") {
	{50, 10, 350, 610},
	documentProc, visible, noGoAway, 0x0, "Calculator", noAutoCenter
};


resource 'DLOG' (1000, "About Calculator") {
	{90, 50, 180, 460},
	rDocProc, visible, noGoAway, 0x0, 1000, "CalculatorAbout", centerMainScreen
};


resource 'DITL' (1000) {
	 {
/* 1 */ {60, 167, 81, 244},
		button {
			enabled,
			"OK"
		};
/* 2 */ {5, 10, 60, 400},				/* SourceLanguage Item */
		staticText {
			disabled,
			"First attempt at writing a Classic Mac OS application.  "
			"A simple calculator, written by Jacob Hohisel.  "
			"Version 0.1"
		}
	}
};


/* here is the quintessential MultiFinder friendliness device, the SIZE resource */

resource 'SIZE' (-1) {
	saveScreen,
	acceptSuspendResumeEvents,
	disableOptionSwitch,
	canBackground,
	needsActivateOnFGSwitch,	/* this says we do our own activate/deactivate; don't fake us out */
	backgroundAndForeground,	/* this is definitely not a background-only application! */
	dontGetFrontClicks,			  /* change this is if you want "do first click" behavior like the Finder */
	ignoreAppDiedEvents,		  /* essentially, I'm not a debugger (sub-launching) */
	is32BitCompatible,			  /* this app should not be run in 32-bit address space -- Why? */
	notHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	dontUseTextEditServices,
	reserved,
	reserved,
	reserved,
	100 * 1024,					/* we made this (preferred) size bigger than the other (minimum) size */
								      /* so you can have more text & scraps */
	100 * 1024					/* we looked at a heap dump while the program was running */
								      /* it was using about 27K; we added 13K for stack, text & scraps */
};
