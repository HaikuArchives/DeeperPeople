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

#ifndef PEOPLEPREFVIEW_H
#define PEOPLEPREFVIEW_H

#include <TabView.h>
#include "PeopleApp.h"
#include "PeoplePrefViewSizes.h"

#define VIEW_COLOR			216

class TTextControl;


//====================================================================

class TPeoplePrefView : public BTabView {

private:

	BFile			*fFile;
	BPopUpMenu		*fGroups;
	TTextControl	*fField[F_END];

public:

					TPeoplePrefView(BRect, char*, entry_ref*);
					~TPeoplePrefView(void); 
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