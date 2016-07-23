//--------------------------------------------------------------------
//	
//	PeopleApp.h
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef PEOPLEAPP_H
#define PEOPLEAPP_H

#define	B_PERSON_MIMETYPE	"application/x-person"
#define APP_SIG				"application/x-vnd.DeepOcean-PEPL"
#define APP_NAME			"Deeper People"
#define APP_VERSION			"3.0dr8"

#define P_NAME				"META:name"		// compat
#define P_FIRSTNAME			"META:firstname"
#define P_LASTNAME			"META:lastname"
#define P_TITLE				"META:title"
#define P_NICKNAME			"META:nickname" // compat
#define P_GROUPNAME			"META:group"	// compat

#define P_HADDRESS			"META:address"	// compat
#define	P_HADDRESS2			"META:address2" 
#define P_HCITY				"META:city"		// compat
#define P_HSTATE			"META:state"	// compat
#define P_HZIPCODE			"META:zip"		// compat
#define P_HCOUNTRY			"META:country"	// compat
#define P_HPHONE			"META:hphone"	// compat
#define P_HFAX				"META:fax"		// compat
#define	P_HCPHONE			"META:cphone"
// no home pager
#define P_HEMAIL			"META:email"	// compat
#define P_HURL				"META:url"		// compat


#define P_WCOMPANYNAME		"META:company"	// compat
#define P_WPOSITION			"META:wposition"
#define P_WADDRESS			"META:waddress"
#define P_WADDRESS2			"META:waddress2"
#define P_WCITY				"META:wcity"
#define P_WSTATE			"META:wstate"
#define P_WZIPCODE			"META:wzip"
#define P_WCOUNTRY			"META:wcountry"
#define P_WPHONE			"META:wphone"	// compat
#define P_WFAX				"META:wfax"
#define P_WCPHONE			"META:wcphone"	// you're not obliged to put the phone in the wc ;-)
#define P_WPAGER			"META:pager"
#define P_WEMAIL			"META:email2"
#define	P_WURL				"META:url2"

#define P_IEMAIL3			"META:email3"
#define P_IEMAIL4			"META:email4"
#define P_IEMAIL5			"META:email5"
#define P_IURL3				"META:url3"
#define P_IJABBER			"META:jabber"
#define P_IICQ				"META:icq"
#define P_IAIM				"META:aim"
#define P_IYAHOO			"META:yahoo"
#define P_IMSN				"META:msn"

#define P_NOTES				"META:notes"

enum	MESSAGES			{
							// menus messages
							M_NEW = 128, M_SAVE, M_SAVE_AS, M_REVERT,
							M_UNDO, M_SELECT, M_GROUP_MENU, M_DIRTY, M_WINDOW_UPDATE_TITLE,
							// general stuff messages
							M_FIRSTNAME, M_LASTNAME, M_TITLE,
							M_NICKNAME, M_GROUPNAME,
							// home stuff messages
							M_HADDRESS, M_HADDRESS2, M_HCITY,
							M_HSTATE, M_HZIPCODE, M_HCOUNTRY,
							M_HPHONE, M_HFAX, M_HCPHONE,	// no pager
							M_HEMAIL, M_HURL,
							// company stuff messages
							M_WCOMPANYNAME, M_WPOSITION,
							M_WADDRESS, M_WADDRESS2, M_WCITY,
							M_WSTATE, M_WZIPCODE, M_WCOUNTRY,
							M_WPHONE, M_WFAX, M_WCPHONE, M_WPAGER,
							M_WEMAIL, M_WURL,
							// internet stuff
							M_IEMAIL3, M_IEMAIL4, M_IEMAIL5,
							M_IURL3,
							M_IICQ, M_IAIM, M_IJABBER, M_IYAHOO, M_IMSN,
							// notes
							M_NOTES,
							// click everywhere stuff
							M_LAUNCH_HMAIL, M_LAUNCH_WMAIL,
							M_LAUNCH_IMAIL3, M_LAUNCH_IMAIL4,
							M_LAUNCH_IMAIL5,
							M_LAUNCH_HURL, M_LAUNCH_WURL, M_LAUNCH_IURL3,
							// internal messages
							M_TREE_SELECT, M_TREE_INVOKE,
							M_SEARCH, M_SEARCH_ACTIVATE, M_SEARCH_NEXT, M_SEARCH_PREVIOUS,
							M_EXPAND_ALL, M_COLLAPSE_ALL,
							M_SEARCH_ENABLE, M_SEARCH_DISABLE,
							M_PEOPLE_ACTIVATE, M_PREFS_ACTIVATE
							};

enum	FIELDS				{
							// general fields
							F_FIRSTNAME = 0, F_LASTNAME, F_TITLE,
							F_NICKNAME, F_GROUPNAME,
							// home fields
							F_HADDRESS, F_HADDRESS2, F_HCITY,
							F_HSTATE, F_HZIPCODE, F_HCOUNTRY,
							F_HPHONE, F_HFAX, F_HCPHONE, // no pager
							F_HEMAIL, F_HURL,
							// work fields
							F_WCOMPANYNAME, F_WPOSITION,
							F_WADDRESS, F_WADDRESS2, F_WCITY,
							F_WSTATE, F_WZIPCODE, F_WCOUNTRY,
							F_WPHONE, F_WFAX, F_WCPHONE, F_WPAGER,
							F_WEMAIL, F_WURL,
							// internet fields
							F_IEMAIL3, F_IEMAIL4, F_IEMAIL5,
							F_IURL3,
							F_IICQ, F_IAIM, F_IYAHOO, F_IMSN, F_IJABBER,
							// notes
							F_NOTES,
							// last but not least !
							F_END };

//====================================================================

#include <Application.h>

#include "PeopleLang.h"

class	TPeopleWindow;

class TPeopleApp : public BApplication {

private:

	TPeopleWindow	*myWindow;
	BRect			myPosition;

public:

	BFile			*myPrefs;

					TPeopleApp(void);
					~TPeopleApp(void);
	virtual void	AboutRequested(void);
	virtual void	ArgvReceived(int32, char**);
	virtual void	MessageReceived(BMessage*);
	virtual void	RefsReceived(BMessage*);
	virtual void	ReadyToRun(void);
	TPeopleWindow	*FindWindow(entry_ref);
	TPeopleWindow	*NewWindow(entry_ref* = NULL);
};

#endif /* PEOPLEAPP_H */
