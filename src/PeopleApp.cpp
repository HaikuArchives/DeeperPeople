//--------------------------------------------------------------------
//	
//	PeopleApp.cpp
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <Bitmap.h>
#include <Directory.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Screen.h>
#include <fs_index.h>
#include <string.h>
#include <Alert.h>

#include "PeopleApp.h"
#include "PeopleWindow.h"
#include "PersonIcons.h"

//====================================================================

TPeopleApp::TPeopleApp(void)
		  :BApplication(APP_SIG)
{
	bool			valid = FALSE;
	const char		*str;
	int32			index = 0;
	BBitmap			large_icon(BRect(0, 0, B_LARGE_ICON - 1, B_LARGE_ICON - 1), B_COLOR_8_BIT);
	BBitmap			mini_icon(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON - 1), B_COLOR_8_BIT);
	BDirectory		dir;
	BEntry			entry;
	BMessage		msg;
	BMessage		info;
	BMimeType		mime;
	BPath			path;
	BPoint			pos;
	BVolume			vol;
	BVolumeRoster	roster;

	

	myPosition.Set(6, TITLEBAR_H, 6 + WINDOW_W, TITLEBAR_H + WINDOW_H);
	pos = myPosition.LeftTop();

	myWindow = new TPeopleWindow(myPosition, NULL, NULL);
	myWindow->Show();

	find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);
	dir.SetTo(path.Path());
	if (dir.FindEntry("DeeperPeople_data", &entry) == B_NO_ERROR) {
		myPrefs = new BFile(&entry, O_RDWR);
		if (myPrefs->InitCheck() == B_NO_ERROR) {
			myPrefs->Read(&pos, sizeof(BPoint));
			if (BScreen(B_MAIN_SCREEN_ID).Frame().Contains(pos))
				myPosition.OffsetTo(pos);
		}
	}
	else {
		myPrefs = new BFile();
		if (dir.CreateFile("DeeperPeople_data", myPrefs) != B_NO_ERROR) {
			delete myPrefs;
			myPrefs = NULL;
		}
	}

	roster.GetBootVolume(&vol);
	fs_create_index(vol.Device(), P_NAME, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_FIRSTNAME, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_LASTNAME, B_STRING_TYPE, 0);


	fs_create_index(vol.Device(), P_HADDRESS, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HCITY, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HSTATE, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HCOUNTRY, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HPHONE, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HCPHONE, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_HEMAIL, B_STRING_TYPE, 0);

	fs_create_index(vol.Device(), P_WCOMPANYNAME, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_WPHONE, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_WCPHONE, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_WEMAIL, B_STRING_TYPE, 0);

	fs_create_index(vol.Device(), P_GROUPNAME, B_STRING_TYPE, 0);

	fs_create_index(vol.Device(), P_IICQ, B_STRING_TYPE, 0);
	fs_create_index(vol.Device(), P_IJABBER, B_STRING_TYPE, 0);


	// install person mime type
	mime.SetType(B_PERSON_MIMETYPE);
	if (mime.IsInstalled()) {
		if (mime.GetAttrInfo(&info) == B_NO_ERROR) {
			while (info.FindString("attr:name", index++, &str) == B_NO_ERROR) {
				if (!strcmp(str, P_NOTES)) {
					valid = TRUE;
					break;
				}
			}
			if (!valid)
				mime.Delete();
		}
	}
	if (!valid) {
		mime.Install();
		large_icon.SetBits(kLargePersonIcon, large_icon.BitsLength(), 0, B_COLOR_8_BIT);
		mini_icon.SetBits(kSmallPersonIcon, mini_icon.BitsLength(), 0, B_COLOR_8_BIT);
		mime.SetShortDescription(MIME_SHORT_DESCR);
		mime.SetLongDescription(MIME_LONG_DESCR);
		mime.SetIcon(&large_icon, B_LARGE_ICON);
		mime.SetIcon(&mini_icon, B_MINI_ICON);
		mime.SetPreferredApp(APP_SIG);

		// add relevant person fields to meta-mime type

		msg.AddString("attr:public_name", VIEW_FIRST_NAME); 
		msg.AddString("attr:name", P_FIRSTNAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 
		
		msg.AddString("attr:public_name", VIEW_LAST_NAME); 
		msg.AddString("attr:name", P_LASTNAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_NICKNAME); 
		msg.AddString("attr:name", P_NICKNAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_TITLE); 
		msg.AddString("attr:name", P_TITLE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_GROUPNAME); 
		msg.AddString("attr:name", P_GROUPNAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 


		/////////////////////////////////
		// home stuff
		
		msg.AddString("attr:public_name", VIEW_HOME_ADDRESS); 
		msg.AddString("attr:name", P_HADDRESS); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_ADDRESS2); 
		msg.AddString("attr:name", P_HADDRESS2); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_CITY); 
		msg.AddString("attr:name", P_HCITY); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_STATE); 
		msg.AddString("attr:name", P_HSTATE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 50); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_ZIP_CODE); 
		msg.AddString("attr:name", P_HZIPCODE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 50); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_COUNTRY); 
		msg.AddString("attr:name", P_HCOUNTRY); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_PHONE); 
		msg.AddString("attr:name", P_HPHONE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		
		msg.AddString("attr:public_name", VIEW_HOME_FAX); 
		msg.AddString("attr:name", P_HFAX); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false);
		 
		msg.AddString("attr:public_name", VIEW_HOME_CELL_PHONE); 
		msg.AddString("attr:name", P_HCPHONE);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		// no home pager

		msg.AddString("attr:public_name", VIEW_HOME_EMAIL); 
		msg.AddString("attr:name", P_HEMAIL);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_HOME_URL); 
		msg.AddString("attr:name", P_HURL); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		/////////////////////////////////
		// work stuff

		msg.AddString("attr:public_name", VIEW_WORK_COMPANY_NAME); 
		msg.AddString("attr:name", P_WCOMPANYNAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_ADDRESS); 
		msg.AddString("attr:name", P_WADDRESS); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_ADDRESS2); 
		msg.AddString("attr:name", P_WADDRESS2); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_CITY); 
		msg.AddString("attr:name", P_WCITY); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_STATE); 
		msg.AddString("attr:name", P_WSTATE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 50); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_ZIP_CODE); 
		msg.AddString("attr:name", P_WZIPCODE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 50); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_COUNTRY); 
		msg.AddString("attr:name", P_WCOUNTRY); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_PHONE); 
		msg.AddString("attr:name", P_WPHONE); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		
		msg.AddString("attr:public_name", VIEW_WORK_FAX); 
		msg.AddString("attr:name", P_WFAX); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false);
		 
		msg.AddString("attr:public_name", VIEW_WORK_CELL_PHONE); 
		msg.AddString("attr:name", P_WCPHONE);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_EMAIL); 
		msg.AddString("attr:name", P_WEMAIL);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_URL); 
		msg.AddString("attr:name", P_WURL); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_WORK_PAGER); 
		msg.AddString("attr:name", P_WPAGER);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 90); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 


		/////////////////////////////////
		// internet stuff

		
		msg.AddString("attr:public_name", VIEW_INET_EMAIL3); 
		msg.AddString("attr:name", P_IEMAIL3);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_EMAIL4); 
		msg.AddString("attr:name", P_IEMAIL4);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 
		
		msg.AddString("attr:public_name", VIEW_INET_EMAIL5); 
		msg.AddString("attr:name", P_IEMAIL4);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 
	
		msg.AddString("attr:public_name", VIEW_INET_ICQ); 
		msg.AddString("attr:name", P_IICQ);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_AIM); 
		msg.AddString("attr:name", P_IAIM);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_MSN); 
		msg.AddString("attr:name", P_IMSN);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_JABBER); 
		msg.AddString("attr:name", P_IJABBER);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_YAHOO); 
		msg.AddString("attr:name", P_IYAHOO);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_INET_URL3); 
		msg.AddString("attr:name", P_IURL3);
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 

		msg.AddString("attr:public_name", VIEW_NOTES); 
		msg.AddString("attr:name", P_NOTES); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", true); 
		msg.AddBool("attr:editable", true); 
		msg.AddInt32("attr:width", 400);
		msg.AddInt32("attr:height", 300); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 
		
		msg.AddString("attr:public_name", VIEW_COMPAT_NAME); 
		msg.AddString("attr:name", P_NAME); 
		msg.AddInt32("attr:type", B_STRING_TYPE); 
		msg.AddBool("attr:viewable", false); 
		msg.AddBool("attr:editable", false); 
		msg.AddInt32("attr:width", 120); 
		msg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
		msg.AddBool("attr:extra", false); 
		
		mime.SetAttrInfo(&msg);
	}
}

//--------------------------------------------------------------------

TPeopleApp::~TPeopleApp(void)
{
	if (myPrefs)
		delete myPrefs;
}

//--------------------------------------------------------------------

void TPeopleApp::AboutRequested(void)
{
	(new BAlert("", ABOUT_BY_TEXT, ABOUT_BUTTON_TEXT))->Go();
}

//--------------------------------------------------------------------

void TPeopleApp::ArgvReceived(int32 argc, char **argv)
{
	char			*arg;
	int32			index;
	int32			loop;
	TPeopleWindow	*window = NULL;

	for (loop = 1; loop < argc; loop++) {
		arg = argv[loop];

		// general stuff
		if (!strncmp(P_FIRSTNAME, arg, strlen(P_FIRSTNAME)))
			index = F_FIRSTNAME;
		else if (!strncmp(P_LASTNAME, arg, strlen(P_LASTNAME)))
			index = F_LASTNAME;
		else if (!strncmp(P_NICKNAME, arg, strlen(P_NICKNAME)))
			index = F_NICKNAME;
		else if (!strncmp(P_TITLE, arg, strlen(P_TITLE)))
			index = F_TITLE;
		else if (!strncmp(P_GROUPNAME, arg, strlen(P_GROUPNAME)))
			index = F_GROUPNAME;	
			
		// home stuff

		else if (!strncmp(P_HADDRESS, arg, strlen(P_HADDRESS)))
			index = F_HADDRESS;
		else if (!strncmp(P_HADDRESS2, arg, strlen(P_HADDRESS2)))
			index = F_HADDRESS2;
		else if (!strncmp(P_HCITY, arg, strlen(P_HCITY)))
			index = F_HCITY;
		else if (!strncmp(P_HSTATE, arg, strlen(P_HSTATE)))
			index = F_HSTATE;
		else if (!strncmp(P_HZIPCODE, arg, strlen(P_HZIPCODE)))
			index = F_HZIPCODE;
		else if (!strncmp(P_HCOUNTRY, arg, strlen(P_HCOUNTRY)))
			index = F_HCOUNTRY;
		else if (!strncmp(P_HPHONE, arg, strlen(P_HPHONE)))
			index = F_HPHONE;
		else if (!strncmp(P_HFAX, arg, strlen(P_HFAX)))
			index = F_HFAX;
		else if (!strncmp(P_HCPHONE, arg, strlen(P_HCPHONE)))
			index = F_HCPHONE;
		// no pager
		else if (!strncmp(P_HEMAIL, arg, strlen(P_HEMAIL)))
			index = F_HEMAIL;
		else if (!strncmp(P_HURL, arg, strlen(P_HURL)))
			index = F_HURL;
		
		
		// work stuff
		else if (!strncmp(P_WCOMPANYNAME, arg, strlen(P_WCOMPANYNAME)))
			index = F_WCOMPANYNAME;
		else if (!strncmp(P_WPOSITION, arg, strlen(P_WPOSITION)))
			index = F_WPOSITION;
		
		else if (!strncmp(P_WADDRESS, arg, strlen(P_WADDRESS)))
			index = F_WADDRESS;
		else if (!strncmp(P_WADDRESS2, arg, strlen(P_WADDRESS2)))
			index = F_WADDRESS2;
		else if (!strncmp(P_WCITY, arg, strlen(P_WCITY)))
			index = F_WCITY;
		else if (!strncmp(P_WSTATE, arg, strlen(P_WSTATE)))
			index = F_WSTATE;
		else if (!strncmp(P_WZIPCODE, arg, strlen(P_WZIPCODE)))
			index = F_WZIPCODE;
		else if (!strncmp(P_WCOUNTRY, arg, strlen(P_WCOUNTRY)))
			index = F_WCOUNTRY;
		else if (!strncmp(P_WPHONE, arg, strlen(P_WPHONE)))
			index = F_WPHONE;
		else if (!strncmp(P_WFAX, arg, strlen(P_WFAX)))
			index = F_WFAX;
		else if (!strncmp(P_WCPHONE, arg, strlen(P_WCPHONE)))
			index = F_WCPHONE;
		else if (!strncmp(P_WPAGER, arg, strlen(P_WPAGER)))
			index = F_WPAGER;
		else if (!strncmp(P_WEMAIL, arg, strlen(P_WEMAIL)))
			index = F_WEMAIL;
		else if (!strncmp(P_WURL, arg, strlen(P_WURL)))
			index = F_WURL;
		

		// internet stuff
		
		else if (!strncmp(P_IEMAIL3, arg, strlen(P_IEMAIL3)))
			index = F_IEMAIL3;
		else if (!strncmp(P_IEMAIL4, arg, strlen(P_IEMAIL4)))
			index = F_IEMAIL4;
		else if (!strncmp(P_IEMAIL5, arg, strlen(P_IEMAIL5)))
			index = F_IEMAIL5;
		
		else if (!strncmp(P_IURL3, arg, strlen(P_IURL3)))
			index = F_IURL3;
			
		else if (!strncmp(P_IICQ, arg, strlen(P_IICQ)))
			index = F_IICQ;
		else if (!strncmp(P_IAIM, arg, strlen(P_IAIM)))
			index = F_IAIM;
		else if (!strncmp(P_IJABBER, arg, strlen(P_IJABBER)))
			index = F_IJABBER;
		else if (!strncmp(P_IYAHOO, arg, strlen(P_IYAHOO)))
			index = F_IYAHOO;
		else if (!strncmp(P_NOTES, arg, strlen(P_NOTES)))
			index = F_NOTES;
		else		
			index = F_END;

		if (index != F_END) {
			while(*arg != ' ')
				arg++;
			arg++;
			myWindow->SetField(index, arg);
			
			// activate the good tab view !

		}
	}
}

//--------------------------------------------------------------------

void TPeopleApp::MessageReceived(BMessage *msg)
{
	BApplication::MessageReceived(msg);
}

//--------------------------------------------------------------------

void TPeopleApp::RefsReceived(BMessage *msg)
{
	int32			item = 0;
	BFile			file;
	entry_ref		ref;
	TPeopleWindow	*window;

	if (msg->HasRef("refs", item)) {
		msg->FindRef("refs", item++, &ref);
		myWindow->SetRef(&ref);
	}
}

//--------------------------------------------------------------------

void TPeopleApp::ReadyToRun(void)
{
/*
	if (!myWindow)
		NewWindow();
*/
}
