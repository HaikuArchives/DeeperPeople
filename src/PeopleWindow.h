//--------------------------------------------------------------------
//	
//	PeopleWindow.h
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef PEOPLEWINDOW_H
#define PEOPLEWINDOW_H

// no titlebar_w
#define MARGIN_H			5
#define MARGIN_W			5

#define MENUBAR_W			32768
#define SEARCH_W			440
#define TREE_W				180
#define TABVIEW_W			SEARCH_W
#define	WINDOW_W			(TREE_W + SPLIT_W + TABVIEW_W)
#define SPLIT_W				7

#define TABVIEW_WM			(TABVIEW_W - 9 * MARGIN_W)

#define	TITLEBAR_H			25
#define	MENUBAR_H			15
#define SEARCH_H			40
#define TREE_H				385
#define STATUS_H			5
#define TABVIEW_H			(TREE_H + STATUS_H - SEARCH_H)
#define WINDOW_H			(TREE_H + STATUS_H + MENUBAR_H + MARGIN_H*2)
//no split_h

class TPeopleView;
class TPeopleTreeView;
class TPeoplePrefView;
class BFilePanel;
class BMenuItem;
class SplitPane;


#include <StatusBar.h>
#include <TextControl.h>
#include <Window.h>
//====================================================================

class TPeopleWindow : public BWindow {

private:

	BFilePanel		*myPanel;
	BMenuItem		*myCopy;
	BMenuItem		*myCut;
	BMenuItem		*myPaste;
	BMenuItem		*myRevert;
	BMenuItem		*mySave;
	BMenuItem		*myUndo;

	SplitPane			*mySplitPane;
	TPeopleView			*myPeopleView;
	TPeopleTreeView		*myTreeView;
	BScrollView			*myTreeScrollView;
	TPeoplePrefView		*myPrefView;
	BView				*myBackTab;
	BStatusBar			*myProgressBar;

	BTextControl		*mySearchField;

	entry_ref		*myRef;
	
	int32			myLastResult;

public:


					TPeopleWindow(BRect, char*, entry_ref*);
					~TPeopleWindow(void);
	virtual void	MenusBeginning(void);
	virtual void	MessageReceived(BMessage*);
	virtual bool	QuitRequested(void);
	void			DefaultName(char*);
	void			NewPerson();
	void			SetField(int32, char*);
	void			SaveAs(void);
	
	inline entry_ref *GetRef() {return myRef;};
	void			SetRef(entry_ref*);
	
	inline BView	*GetPeopleView(void) {return (BView*)myPeopleView;};
	
	
};

#endif /* PEOPLEWINDOW_H */
