//--------------------------------------------------------------------
//	
//	GPItems.h
//	Group & Person Items !
//	Written by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef GPITEMS_H
#define GPITEMS_H

#include <ListItem.h>
#include <Entry.h>
#include <Node.h>
#include <String.h>
#include <stdio.h>


class AbstractItem : public BStringItem {

public:
	AbstractItem(const char *, uint32, bool);

	virtual inline bool	IsPerson() { return false; };	
	~AbstractItem();	
};

//========================================================================================================

class PersonItem : public AbstractItem {

private:
	entry_ref	myRef;
	node_ref	myNRef;
	BString		*myDescr;
	
public:
	PersonItem(const char * name, entry_ref *ref, BString &descr, uint32 level = 0, bool expanded = true);
	~PersonItem();
			
	inline bool			IsPerson()		{ return true; };
	inline entry_ref	*GetRef()		{ return &myRef; };
	inline node_ref		*GetNodeRef()	{ return &myNRef; };
	inline BString		*GetDescr()		{ return myDescr; };

};

//=========================================================================================================

class GroupItem : public AbstractItem {

private:
	uint32		myCount;
	const char	*myLabel;

public:
			GroupItem( const char*, uint32 level = 0, bool expanded = false, uint32 count = 0);
			~GroupItem();
			
			inline bool		IsPerson()			{ return false; };
			inline void		SetCount(int32 c)	{ myCount = c; Update(); };
			inline void		IncrementCount()	{ myCount++; Update(); };
			inline void		DecrementCount()	{ myCount--; Update(); };
			inline int32	GetCount()			{ return myCount; };

			inline void		Update()
			{
				char buff[256];
				if(myCount){
					sprintf(buff, "%s (%i)", myLabel, myCount );	
					SetText(buff);
				} else {
					SetText(myLabel);
				}
			};
};

#endif
