#include <Windows.h>
#include <Types.h>
#include <Processes.h>
#include <QuickDraw.h>
#include <Sound.h>
#include <stdlib.h>
#include <ToolUtils.h>

#define windowID 0x3E8 		/* Resource ID for main window */
#define appleId 1000 	 		/* Apple Menu ID */
#define fileId 1001 	 		/* File Menu ID */
#define editId 1002 	 		/* Edit Menu ID */
#define appleM 0			 		/* Index of apple menu */
#define fileM 1				 		/* Index of file menu */
#define editM 2				 		/* Index of edit menu */
#define quitItem 1		 		/* Quit in the menu of course. */
#define aboutMeCommand 1	/* Menu item in apple menu for About TubeTest item */
#define menuCount 3				/* Number of menus this application uses */

WindowPtr   globalWindow,
            whichWindow;
Rect        dragRect;
Boolean     doneFlag;
EventRecord	myEvent;
OSErr       error;
SysEnvRec	  theWorld;
QDGlobals   qdg;
char        theChar;
MenuHandle	myMenus[menuCount];

void SetUpMenus(void);
void ShowAboutMeDialog(void);
void DoCommand(long int mResult);

void main(void) {
  /*
	**	Test the computer to be sure we can do color.  
	**	If not we would crash, which would be bad.  
	**	If we can't run, just beep and exit.
	*/
	error = SysEnvirons(1, &theWorld);
	if (theWorld.hasColorQD == false) {
		SysBeep (50);
		ExitToShell();							/* If no color QD, we must leave. */
	};

	InitGraf(&qdg.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	InitCursor();

	SetRect(&dragRect, 4, 24, qdg.screenBits.bounds.right - 4, qdg.screenBits.bounds.bottom - 4);
	doneFlag = false;							/* flag to detect when Quit command is chosen */

	/*
	**	Open the color window.
	*/
	globalWindow = GetNewCWindow(windowID, nil, (WindowPtr) -1);
	SetPort(globalWindow);

	/*
	**	Set up menus last, since the menu drawing can then use 
	**	the palette we have for our window. 
	**	Makes the Apple look better, in particular.
	*/
	SetUpMenus();
	
	/*
	**	Main Event Loop
	*/
	do {
		SystemTask();

		if (WaitNextEvent(everyEvent, &myEvent, 5L, NULL)) {
			switch (myEvent.what) {				/* case on event type */

				case mouseDown:
					switch (FindWindow(myEvent.where, &whichWindow)) {

						case inSysWindow:		/* desk accessory window: call Desk Manager to handle it */
							SystemClick(&myEvent, whichWindow);
							break;

						case inMenuBar:			/* Menu bar: learn which command, then execute it. */
							DoCommand(MenuSelect(myEvent.where));
							break;

						case inDrag:			/* title bar: call Window Manager to drag */
							DragWindow(whichWindow, myEvent.where, &dragRect);
							break;

						case inContent:			/* body of application window: */
							if (whichWindow != FrontWindow())
								SelectWindow(whichWindow); /* and make it active if not */
							break;
					}
					break;

				case updateEvt:					/* Update the eyes in window. */
					if ((WindowPtr) myEvent.message == globalWindow) {
						// BeginUpdate((WindowPtr) myEvent.message);
						// DrawEyes();
						// EndUpdate((WindowPtr) myEvent.message);
					}
					break;
							
				case keyDown:
				case autoKey:					/* key pressed once or held down to repeat */
					if (globalWindow == FrontWindow()) {
						theChar = (myEvent.message & charCodeMask); /* get the char */
						/* 
						**	If Command key down, do it as a Menu Command.
						*/
						if (myEvent.modifiers & cmdKey) {
							DoCommand(MenuKey(theChar));
            }
					}
					break;

			}
		}

		/*	
		**	If we have menu item checked, go ahead and animate colors.
		*/
		// if (tubeCheck) ShiftyColors();
		
	} while (!doneFlag);

	/*
	**	clean up after palette manager, 
	**	so he can chuck the palette in use.
	*/
	DisposeWindow (globalWindow);
}

/*
**	Read menu descriptions from resource file into memory 
**	and store handles in myMenus array.
**	Insert into MenuBar and draw.
*/
void SetUpMenus(void)
{
	int i;

	myMenus[appleM] = GetMenu(appleId); 		/* read Apple menu from resource file */
	AppendResMenu(myMenus[appleM], 'DRVR');		/* add desk accessory names to Apple menu */
	myMenus[fileM] = GetMenu(fileId);			/* read file menu from resource file */
	myMenus[editM] = GetMenu(editId);			/* read edit menu from resource file */

	for (i = 0; i < menuCount; i++) 
		InsertMenu(myMenus[i], 0); 				/* install menus in menu bar */
	
	DrawMenuBar();								/* and draw menu bar */
}

/*	
**	Display the dialog box in response to the 'About TubeTest' menu item
*/
void ShowAboutMeDialog(void)
{
	DialogPtr	theDialog;
	short		itemHit;

	theDialog = GetNewDialog(1000, nil, (WindowPtr) -1);
	ModalDialog(nil, &itemHit);
	DisposeDialog(theDialog);
}

/*
**	Execute menu command specified by mResult,
**	the result of MenuSelect
*/
void DoCommand(long int mResult)
{
	short	theItem,							/* menu item number from mResult low-order word */
			theMenu;							/* menu number from mResult high-order word */
	Str255	name;								/* desk accessory name */
	int		temp;
	Boolean	dummy;

	theItem = LoWord(mResult);					/* call Toolbox Utility routines to */
	theMenu = HiWord(mResult);					/* set menu item number and menu */

	switch (theMenu) {							/* switch on menu ID */

		case appleId:
			if (theItem == aboutMeCommand)
				ShowAboutMeDialog();
			break;

		case fileId:
			if (theItem == quitItem)
				doneFlag = true;
			break;

		case editId:
			dummy = SystemEdit(theItem - 1);	/* Pass the command on to the Desk Manager. */
			break;
	}

	HiliteMenu(0);								/* Unhighlight menu title */
												/* (highlighted by MenuSelect) */
}
