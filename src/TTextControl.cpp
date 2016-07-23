//--------------------------------------------------------------------
//	
//	TTextControl.cpp
//
//	Written by: Robert Polic
// Extended by : Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <string.h>
#include <malloc.h>
#include <Font.h>
#include <Application.h>
#include <Window.h>

#include "TTextControl.h"

//====================================================================

TTextControl::TTextControl(BRect r, char *label, int32 length,
						   char *text, int32 mod_msg, int32 msg, uint32 flags = B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP)
			 :BTextControl(r, "HOP", label, text, new BMessage(msg), flags)
{
	SetModificationMessage(new BMessage(mod_msg));

	fLabel = (char *)malloc(strlen(label) + 1);
	strcpy(fLabel, label);
	fOriginal = (char *)malloc(strlen(text) + 1);
	strcpy(fOriginal, text);
	fLength = length;
}

//--------------------------------------------------------------------

TTextControl::~TTextControl(void)
{
	free(fLabel);
	free(fOriginal);
}

//--------------------------------------------------------------------

void TTextControl::AttachedToWindow(void)
{
	BTextView	*text;

	BTextControl::AttachedToWindow();

	SetDivider(StringWidth(fLabel) + 7);
	text = (BTextView *)ChildAt(0);
	text->SetMaxBytes(fLength - 1);
}

//--------------------------------------------------------------------

bool TTextControl::Changed(void)
{
	return strcmp(fOriginal, Text());
}

//--------------------------------------------------------------------

void TTextControl::Revert(void)
{
	if (Changed())
		SetText(fOriginal);
}

//--------------------------------------------------------------------

void TTextControl::MouseDown(BPoint p)
{

if(strlen(Text()) > 0){

	if (!strcmp(VIEW_HOME_EMAIL,fLabel)){
			Window()->MessageReceived(new BMessage(M_LAUNCH_HMAIL));
	} else if (!strcmp(VIEW_WORK_EMAIL,fLabel)){
			Window()->MessageReceived(new BMessage(M_LAUNCH_WMAIL));
	} else if (!strcmp(VIEW_INET_EMAIL3,fLabel)){
			Window()->MessageReceived(new BMessage(M_LAUNCH_IMAIL3));
	} else if (!strcmp(VIEW_INET_EMAIL4,fLabel)){
			Window()->MessageReceived(new BMessage(M_LAUNCH_IMAIL4));
	} else if (!strcmp(VIEW_INET_EMAIL5,fLabel)){
			Window()->MessageReceived(new BMessage(M_LAUNCH_IMAIL5));
	} else if (!strcmp(VIEW_HOME_URL,fLabel)) {
			Window()->MessageReceived(new BMessage(M_LAUNCH_HURL));
	} else if (!strcmp(VIEW_WORK_URL,fLabel)) {
			Window()->MessageReceived(new BMessage(M_LAUNCH_WURL));
	} else if (!strcmp(VIEW_INET_URL3,fLabel)) {
			Window()->MessageReceived(new BMessage(M_LAUNCH_IURL3));
	}
}
	//BTextControl::MouseDown(p);
}

//--------------------------------------------------------------------

void TTextControl::Update(void)
{
	fOriginal = (char *)realloc(fOriginal, strlen(Text()) + 1);
	strcpy(fOriginal, Text());
}
