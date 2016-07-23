//--------------------------------------------------------------------
//	
//	PeopleView.h
//
//	Written by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef PEOPLETREEVIEW_H
#define PEOPLETREEVIEW_H

#include <TabView.h>
#include "PeopleApp.h"
#include "PeopleViewSizes.h"
#include "GPItems.h"

#define VIEW_COLOR			216

class TTextControl;

//====================================================================

int CompareStringItems(const BListItem*, const BListItem*);
int32 ThreadBuildTree(void* data);

//====================================================================

class TPeopleTreeView : public BOutlineListView {

private:
	int32			myCount, myPeopleCount;
	thread_id		myQueryThread;
	
	GroupItem 		*myGroupsRoot;
	GroupItem		*myOrphansRoot;
	BQuery			*myQuery;
		
public:

					TPeopleTreeView(BRect, char*);
					~TPeopleTreeView(void); 
	virtual	void	AttachedToWindow(void);
	virtual void	MessageReceived(BMessage*);

	void			SetMessages(void);
	
	void			PrepareQuery(void);
	void			KillTreeBuilder(void);
	void			UpdateCounts();

	inline int32	GetPeopleCount()		{ return myPeopleCount; };
	inline void		IncrementPeopleCount()	{ myPeopleCount++; myCount++; };
	inline void		DecrementPeopleCount()	{ myPeopleCount--; myCount--; };

	inline int32	GetCount()				{ return myCount; };
	inline void		IncrementCount()		{ myCount++; };
	inline void		DecrementCount()		{ myCount--; };
	
	void			AddPersonInGroup(BFile*, entry_ref *, GroupItem *, bool);
	
	int32 			FindIncompletePerson(const char* string, int32 start = 0, int32 rewind = 0);
	int32			FindPersonFromNode(node_ref*);
		
	void			CollapseAllGroups(void);
	void			ExpandAllGroups(void);
	void			ExpandGroupFromPerson(int32 index);
		
	GroupItem		*FindGroup(const char *);
	PersonItem		*FindPerson(const char *);
	PersonItem 		*FindPersonInGroup(const char *, GroupItem *);

	inline BQuery		*GetQuery(void)				{ return myQuery;};
	
	inline GroupItem	*GetPeopleGroupsRoot(void)	{ return myGroupsRoot;};
	inline GroupItem	*GetPeopleOrphansRoot(void)	{ return myOrphansRoot;};


};

#endif /* PEOPLEVIEW_H */
