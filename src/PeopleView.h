//--------------------------------------------------------------------
//	
//	PeopleView.h
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef PEOPLEVIEW_H
#define PEOPLEVIEW_H

#include <TabView.h>
#include "PeopleApp.h"
#include "PeopleViewSizes.h"

#define VIEW_COLOR			216

class TTextControl;


//====================================================================

class TPeopleView : public BTabView {

private:

	BFile			*fFile;
	BEntry			*fEntry;
	BPopUpMenu		*fGroups;


public:

	TTextControl	*fField[F_END];

					TPeopleView(BRect, char*, entry_ref*);
					~TPeopleView(void); 
	virtual	void	AttachedToWindow(void);
	virtual void	MessageReceived(BMessage*);
	void			BuildGroupMenu(void);
	bool			CheckSave(void);
	const char*		GetField(int32);
	void			NewFile(entry_ref*);
	void			Save(void);
	void			SetField(int32, char*, bool);
	bool			TextSelected(void);
};

#endif /* PEOPLEVIEW_H */
