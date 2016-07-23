//--------------------------------------------------------------------
//	
//	PeopleView.cpp
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <fs_attr.h>
#include <Window.h>
#include <Box.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Query.h>
#include <VolumeRoster.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <image.h>
#include <OS.h>
#include <stdlib.h>
#include <OutlineListView.h>

#include "PeoplePrefView.h"
#include "TTextControl.h"

//====================================================================

TPeoplePrefView::TPeoplePrefView(BRect rect, char *title, entry_ref *ref)
		   :BTabView(rect, title, B_WIDTH_AS_USUAL, B_FOLLOW_ALL, B_WILL_DRAW)
{


	if (ref)
		fFile = new BFile(ref, O_RDWR);
	else
		fFile = NULL;
	
}

//--------------------------------------------------------------------

TPeoplePrefView::~TPeoplePrefView(void)
{
	if (fFile)
		delete fFile;
}

//--------------------------------------------------------------------

void TPeoplePrefView::AttachedToWindow(void)
{
	char		*text;
	float		offset;
	BFont		font = *be_plain_font;
	BFont		ufont = *be_plain_font;
	BMenuField	*field;
	BRect		r,rb;
	rgb_color	c;
	attr_info	info;

	ufont.SetFace(B_UNDERSCORE_FACE);
	ufont.SetSpacing(B_FIXED_SPACING);
	
	c.red = c.green = c.blue = VIEW_COLOR;
	SetViewColor(c);
	
	r = Bounds();
	r.InsetBy(-1, -1);
	rb = r;
	offset = font.StringWidth(LARGEST_GENERAL_TEXT) + 18;


	////////////////////////////////////
	// general stuff

	BView *v = new BView(rb, "0", B_FOLLOW_ALL, B_WILL_DRAW);
	v->SetViewColor(c);
	
	
	text = (char *)malloc(1);
	text[0] = 0;

	AddTab(v);
	TabAt(0)->SetLabel("PREFERENCES");

	free(text);

	// final focus ! at last !
	// selecting tab1 then tab0 is awful, but if i don't do that, the first tab
	// never draws ! i must switch manually to make its fielfs appear !
	Select(1);
	Select(0);
	
		

}

//--------------------------------------------------------------------

void TPeoplePrefView::MessageReceived(BMessage* msg)
{
	int32		loop;
	BTextView	*text;
	char		*buff;
	char		**arg_v; 
	int32		arg_c = 2;
	thread_id	t;
	extern char	**environ;

	switch (msg->what) {
		case M_SAVE:
			Save();
			break;

		case M_REVERT:
			for (loop = 0; loop < F_END; loop++)
				fField[loop]->Revert();
			break;

	}
}

//--------------------------------------------------------------------

void TPeoplePrefView::BuildGroupMenu(void)
{
	char			*offset;
	char			str[256];
	char			*text;
	char			*text1;
	int32			count = 0;
	int32			index;
	BEntry			entry;
	BFile			file;
	BMessage		*msg;
	BMenuItem		*item;
	BQuery			query;
	BVolume			vol;
	BVolumeRoster	volume;
	attr_info		info;

	while ((item = fGroups->ItemAt(0))) {
		fGroups->RemoveItem(item);
		delete item;
	}

	volume.GetBootVolume(&vol);
	query.SetVolume(&vol);
	sprintf(str, "%s=*", P_GROUPNAME);
	query.SetPredicate(str);
	query.Fetch();

	while (query.GetNextEntry(&entry) == B_NO_ERROR) {
		file.SetTo(&entry, O_RDONLY);
		if ((file.InitCheck() == B_NO_ERROR) &&
			(file.GetAttrInfo(P_GROUPNAME, &info) == B_NO_ERROR) &&
			(info.size > 1)) {
			text = (char *)malloc(info.size);
			text1 = text;
			file.ReadAttr(P_GROUPNAME, B_STRING_TYPE, 0, text, info.size);
			while (1) {
				if ((offset = strstr(text, ",")))
					*offset = 0;
				if (!fGroups->FindItem(text)) {
					index = 0;
					while ((item = fGroups->ItemAt(index))) {
						if (strcmp(text, item->Label()) < 0)
							break;
						index++;
					}
					msg = new BMessage(M_GROUP_MENU);
					msg->AddString("group", text);
					fGroups->AddItem(new BMenuItem(text, msg), index);
					count++;
				}
				if (offset) {
					text = offset + 1;
					while (*text == ' ')
						text++;
				}
				else
					break;
			}
			free(text1);
		}
	}

	if (!count) {
		fGroups->AddItem(item = new BMenuItem(VIEW_GROUP_NONE, new BMessage(M_GROUP_MENU)));
		item->SetEnabled(FALSE);
	}
}

//--------------------------------------------------------------------

bool TPeoplePrefView::CheckSave(void)
{
	int32	loop;

	for (loop = 0; loop < F_END; loop++)
		if (fField[loop]->Changed())
			return TRUE;

	return FALSE;
}

//--------------------------------------------------------------------

const char* TPeoplePrefView::GetField(int32 index)
{
	if (index < F_END)
		return fField[index]->Text();
	else
		return NULL;
}

//--------------------------------------------------------------------

void TPeoplePrefView::NewFile(entry_ref *ref)
{
	if (fFile)
		delete fFile;
	fFile = new BFile(ref, O_RDWR);
	Save();
}

//--------------------------------------------------------------------

void TPeoplePrefView::Save(void)
{

}

//--------------------------------------------------------------------

void TPeoplePrefView::SetField(int32 index, char *data, bool update)
{
	int32		end;
	int32		start;
	BTextView	*text;

	Window()->Lock();
	if (update) {
		fField[index]->SetText(data);
		fField[index]->Update();
	}
	else {
		text = (BTextView *)fField[index]->ChildAt(0);
		text->GetSelection(&start, &end);
		if (start != end) {
			text->Delete();
			text->Insert(data);
		}
		else if ((end = text->TextLength())) {
			text->Select(end, end);
			text->Insert(",");
			text->Insert(data);
			text->Select(text->TextLength(), text->TextLength());
		}
		else
			fField[index]->SetText(data);
	}
	Window()->Unlock();
}

//--------------------------------------------------------------------

bool TPeoplePrefView::TextSelected(void)
{
	int32		end;
	int32		loop;
	int32		start;
	BTextView	*text;

	for (loop = 0; loop < F_END; loop++) {
		text = (BTextView *)fField[loop]->ChildAt(0);
		text->GetSelection(&start, &end);
		if (start != end)
			return TRUE;
	}
	return FALSE;
}