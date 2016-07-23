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
#include <Alert.h>
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

#include "PeopleView.h"
#include "TTextControl.h"

//====================================================================

TPeopleView::TPeopleView(BRect rect, char *title, entry_ref* ref)
		   :BTabView(rect, title, B_WIDTH_FROM_LABEL, B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE)
{
	if (ref){
		fFile = new BFile(ref, O_RDWR);
		fEntry = new BEntry(ref);
	} else {
		fFile = NULL;
		fEntry = NULL;
	}

}

//--------------------------------------------------------------------

TPeopleView::~TPeopleView(void)
{
	if (fFile)
		delete fFile;
	if (fEntry)
		delete fEntry;
}

//--------------------------------------------------------------------

void TPeopleView::AttachedToWindow(void)
{

	char		*text, *old_name;
	float		offset;
	BFont		font = *be_plain_font;
	BFont		ufont = *be_plain_font;
	BMenuField	*field;
	BRect		r,rb;
	rgb_color	c;
	attr_info	info, old_info;


	ufont.SetFace(B_UNDERSCORE_FACE);
	ufont.SetSpacing(B_FIXED_SPACING);
	
	c.red = c.green = c.blue = VIEW_COLOR;
	SetViewColor(c);
	
	r = Bounds();
	//r.InsetBy(-1, -1);
	rb = r;


	////////////////////////////////////
	// general stuff

	if(font.StringWidth(LARGEST_GENERAL_TEXT) >=
			(font.StringWidth(VIEW_GROUPNAME)+30)) {
		offset = font.StringWidth(LARGEST_GENERAL_TEXT) + 18;
	} else {
		offset = font.StringWidth(VIEW_GROUPNAME) + 30 + 18;
	}


	BView *v = new BView(rb, "0", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);
	
	
	text = (char *)malloc(1);
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_FIRST_NAME) - 11, FIRSTNAME_V,
		  FIRSTNAME_H + FIRSTNAME_W, FIRSTNAME_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_NAME, &old_info) == B_NO_ERROR) {
			old_name = (char *)malloc(old_info.size);
			fFile->ReadAttr(P_NAME, B_STRING_TYPE, 0, old_name, old_info.size);
		}
		if (fFile->GetAttrInfo(P_FIRSTNAME, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_FIRSTNAME, B_STRING_TYPE, 0, text, info.size);
		}
	}
	
	fField[F_FIRSTNAME] = new TTextControl(r, VIEW_FIRST_NAME, 256,
								text, M_WINDOW_UPDATE_TITLE, M_FIRSTNAME);
	fField[F_FIRSTNAME]->SetTarget(this);
	v->AddChild(fField[F_FIRSTNAME]);

	// last name
	text[0] = 0;
	r.Set(LASTNAME_H + 11, LASTNAME_V,
		  LASTNAME_H + LASTNAME_W, LASTNAME_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_LASTNAME, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_LASTNAME, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_LASTNAME] = new TTextControl(r, VIEW_LAST_NAME, 256,
								text, M_WINDOW_UPDATE_TITLE, M_LASTNAME, B_FOLLOW_RIGHT);
	fField[F_LASTNAME]->SetTarget(this);
	v->AddChild(fField[F_LASTNAME]);

	if( strlen(fField[F_FIRSTNAME]->Text()) == 0 && strlen(fField[F_LASTNAME]->Text()) == 0 && fEntry) {
		fField[F_FIRSTNAME]->SetText(old_name);
		fField[F_LASTNAME]->SetText(VIEW_UPDATE_THIS_FIELD);
	}


	//TITLE
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_TITLE) - 11, TITLE_V,
		  TITLE_H + TITLE_W, TITLE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_TITLE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_TITLE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_TITLE] = new TTextControl(r, VIEW_TITLE, 256,
								text, M_DIRTY, M_TITLE, B_FOLLOW_LEFT);
	fField[F_TITLE]->SetTarget(this);
	v->AddChild(fField[F_TITLE]);

	// nick
	text[0] = 0;
	r.Set(NICKNAME_H + 11, NICKNAME_V,
		  NICKNAME_H + NICKNAME_W, NICKNAME_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_NICKNAME, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_NICKNAME, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_NICKNAME] = new TTextControl(r, VIEW_NICKNAME, 256,
								text, M_DIRTY, M_NICKNAME, B_FOLLOW_LEFT_RIGHT);
	fField[F_NICKNAME]->SetTarget(this);
	v->AddChild(fField[F_NICKNAME]);

	// group (don't forget popup !)
	text[0] = 0;
	r.Set(offset - 11, GROUP_V,
		  GROUP_H + GROUP_W, GROUP_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_GROUPNAME, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_GROUPNAME, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_GROUPNAME] = new TTextControl(r, "", 256,
								text, M_DIRTY, M_GROUPNAME);
	fField[F_GROUPNAME]->SetTarget(this);
	v->AddChild(fField[F_GROUPNAME]);


	r.right = r.left + 3;
	r.left = r.right - font.StringWidth(VIEW_GROUPNAME) - 30;
	r.top -= 1;
	fGroups = new BPopUpMenu(VIEW_GROUPNAME);
	fGroups->SetRadioMode(FALSE);
	field = new BMenuField(r, "", "", fGroups);
	field->SetDivider(0.0);
	field->SetFont(&font);
	field->SetEnabled(TRUE);
	v->AddChild(field);

	
	AddTab(v);
	TabAt(0)->SetLabel(VIEW_GENERAL_TAB);

	//=========================================================================
	// home stuff
	
	offset = font.StringWidth(LARGEST_HOME_TEXT) + 18;
	
	v = new BView(rb,  "1", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);

	// home address 1
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_ADDRESS) - 11, HOME_ADDRESS_V,
		  HOME_ADDRESS_H + HOME_ADDRESS_W, HOME_ADDRESS_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HADDRESS, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HADDRESS, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HADDRESS] = new TTextControl(r, VIEW_HOME_ADDRESS, 256,
								text, M_DIRTY, M_HADDRESS);
	fField[F_HADDRESS]->SetTarget(this);
	v->AddChild(fField[F_HADDRESS]);

	// home address 2
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_ADDRESS2) - 11, HOME_ADDRESS2_V,
		  HOME_ADDRESS2_H + HOME_ADDRESS2_W, HOME_ADDRESS2_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HADDRESS2, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HADDRESS2, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HADDRESS2] = new TTextControl(r, VIEW_HOME_ADDRESS2, 256,
								text, M_DIRTY, M_HADDRESS2);
	fField[F_HADDRESS2]->SetTarget(this);
	v->AddChild(fField[F_HADDRESS2]);

	// city
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_CITY) - 11, HOME_CITY_V,
		  HOME_CITY_H + HOME_CITY_W, HOME_CITY_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HCITY, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HCITY, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HCITY] = new TTextControl(r, VIEW_HOME_CITY, 256,
								text, M_DIRTY, M_HCITY);
	fField[F_HCITY]->SetTarget(this);
	v->AddChild(fField[F_HCITY]);

	// state
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_STATE) - 11, HOME_STATE_V,
		  HOME_STATE_H + HOME_STATE_W, HOME_STATE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HSTATE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HSTATE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HSTATE] = new TTextControl(r, VIEW_HOME_STATE, 256,
								text, M_DIRTY, M_HSTATE);
	fField[F_HSTATE]->SetTarget(this);
	v->AddChild(fField[F_HSTATE]);

	// code postal
	text[0] = 0;
	r.Set(HOME_ZIP_CODE_H + 11, HOME_ZIP_CODE_V,
		  HOME_ZIP_CODE_H + HOME_ZIP_CODE_W, HOME_ZIP_CODE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HZIPCODE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HZIPCODE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HZIPCODE] = new TTextControl(r, VIEW_HOME_ZIP_CODE, 256,
								text, M_DIRTY, M_HZIPCODE, B_FOLLOW_RIGHT);
	fField[F_HZIPCODE]->SetTarget(this);
	v->AddChild(fField[F_HZIPCODE]);

	// pays
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_COUNTRY) - 11, HOME_COUNTRY_V,
		  HOME_COUNTRY_H + HOME_COUNTRY_W, HOME_COUNTRY_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HCOUNTRY, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HCOUNTRY, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HCOUNTRY] = new TTextControl(r, VIEW_HOME_COUNTRY, 256,
								text, M_DIRTY, M_HCOUNTRY);
	fField[F_HCOUNTRY]->SetTarget(this);
	v->AddChild(fField[F_HCOUNTRY]);

	
	// home phone
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_PHONE) - 11, HOME_PHONE_V,
		  HOME_PHONE_H + HOME_PHONE_W, HOME_PHONE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HPHONE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HPHONE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HPHONE] = new TTextControl(r, VIEW_HOME_PHONE, 256,
								text, M_DIRTY, M_HPHONE);
	fField[F_HPHONE]->SetTarget(this);
	v->AddChild(fField[F_HPHONE]);

	// home fax
	text[0] = 0;
	r.Set(HOME_FAX_H + 11, HOME_FAX_V,
		  HOME_FAX_H + HOME_FAX_W, HOME_FAX_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HFAX, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HFAX, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HFAX] = new TTextControl(r, VIEW_HOME_FAX, 256,
								text, M_DIRTY, M_HFAX, B_FOLLOW_RIGHT);
	fField[F_HFAX]->SetTarget(this);
	v->AddChild(fField[F_HFAX]);

	// mobile phone
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_CELL_PHONE) - 11, HOME_CELL_PHONE_V,
		  HOME_CELL_PHONE_H + HOME_CELL_PHONE_W, HOME_CELL_PHONE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HCPHONE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HCPHONE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HCPHONE] = new TTextControl(r, VIEW_HOME_CELL_PHONE, 256,
								text, M_DIRTY, M_HCPHONE);
	fField[F_HCPHONE]->SetTarget(this);
	v->AddChild(fField[F_HCPHONE]);

	// home email
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_EMAIL) - 11, HOME_EMAIL_V,
		  HOME_EMAIL_H + HOME_EMAIL_W, HOME_EMAIL_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HEMAIL, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HEMAIL, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HEMAIL] = new TTextControl(r, VIEW_HOME_EMAIL, 256,
								text, M_DIRTY, M_HEMAIL);
	fField[F_HEMAIL]->SetTarget(this);
	v->AddChild(fField[F_HEMAIL]);

	// home URI
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_HOME_URL) - 11, HOME_URL_V,
		  HOME_URL_H + HOME_URL_W, HOME_URL_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_HURL, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_HURL, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_HURL] = new TTextControl(r, VIEW_HOME_URL, 256,
								text, M_DIRTY, M_HURL);
	fField[F_HURL]->SetTarget(this);
	v->AddChild(fField[F_HURL]);
	
	AddTab(v);
	TabAt(1)->SetLabel(VIEW_HOME_TAB);
	

	//=========================================================================
	// work stuff


	offset = font.StringWidth(LARGEST_WORK_TEXT) + 18;

	v = new BView(rb,  "2", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);

	// company name
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_COMPANY_NAME) - 11, WORK_COMPANY_NAME_V,
		  WORK_COMPANY_NAME_H + WORK_COMPANY_NAME_W, WORK_COMPANY_NAME_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WCOMPANYNAME, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WCOMPANYNAME, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WCOMPANYNAME] = new TTextControl(r, VIEW_WORK_COMPANY_NAME, 256,
								text, M_DIRTY, M_WCOMPANYNAME);
	fField[F_WCOMPANYNAME]->SetTarget(this);
	v->AddChild(fField[F_WCOMPANYNAME]);
	
	// work position
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_POSITION) - 11, WORK_POSITION_V,
		  WORK_POSITION_H + WORK_POSITION_W, WORK_POSITION_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WPOSITION, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WPOSITION, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WPOSITION] = new TTextControl(r, VIEW_WORK_POSITION, 256,
								text, M_DIRTY, M_WPOSITION);
	fField[F_WPOSITION]->SetTarget(this);
	v->AddChild(fField[F_WPOSITION]);
	

	// work address 1
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_ADDRESS) - 11, WORK_ADDRESS_V,
		  WORK_ADDRESS_H + WORK_ADDRESS_W, WORK_ADDRESS_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WADDRESS, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WADDRESS, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WADDRESS] = new TTextControl(r, VIEW_WORK_ADDRESS, 256,
								text, M_DIRTY, M_WADDRESS);
	fField[F_WADDRESS]->SetTarget(this);
	v->AddChild(fField[F_WADDRESS]);

	// work address 2
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_ADDRESS2) - 11, WORK_ADDRESS2_V,
		  WORK_ADDRESS2_H + WORK_ADDRESS2_W, WORK_ADDRESS2_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WADDRESS2, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WADDRESS2, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WADDRESS2] = new TTextControl(r, VIEW_WORK_ADDRESS2, 256,
								text, M_DIRTY, M_WADDRESS2);
	fField[F_WADDRESS2]->SetTarget(this);
	v->AddChild(fField[F_WADDRESS2]);
	
	// work city
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_CITY) - 11, WORK_CITY_V,
		  WORK_CITY_H + WORK_CITY_W, WORK_CITY_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WCITY, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WCITY, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WCITY] = new TTextControl(r, VIEW_WORK_CITY, 256,
								text, M_DIRTY, M_WCITY);
	fField[F_WCITY]->SetTarget(this);
	v->AddChild(fField[F_WCITY]);

	// work state
	text[0] = 0;
	r.Set(offset - font.StringWidth(VIEW_WORK_STATE) - 11, WORK_STATE_V,
		  WORK_STATE_H + WORK_STATE_W, WORK_STATE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WSTATE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WSTATE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WSTATE] = new TTextControl(r, VIEW_WORK_STATE, 256,
								text, M_DIRTY, M_WSTATE);
	fField[F_WSTATE]->SetTarget(this);
	v->AddChild(fField[F_WSTATE]);

	// work code postal
	r.Set(WORK_ZIP_CODE_H + 11, WORK_ZIP_CODE_V,
		  WORK_ZIP_CODE_H + WORK_ZIP_CODE_W, WORK_ZIP_CODE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WZIPCODE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WZIPCODE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WZIPCODE] = new TTextControl(r, VIEW_WORK_ZIP_CODE, 256,
								text, M_DIRTY, M_WZIPCODE, B_FOLLOW_RIGHT);
	fField[F_WZIPCODE]->SetTarget(this);
	v->AddChild(fField[F_WZIPCODE]);

	// work pays
	r.Set(offset - font.StringWidth(VIEW_WORK_COUNTRY) - 11, WORK_COUNTRY_V,
		  WORK_COUNTRY_H + WORK_COUNTRY_W, WORK_COUNTRY_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WCOUNTRY, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WCOUNTRY, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WCOUNTRY] = new TTextControl(r, VIEW_WORK_COUNTRY, 256,
								text, M_DIRTY, M_WCOUNTRY);
	fField[F_WCOUNTRY]->SetTarget(this);
	v->AddChild(fField[F_WCOUNTRY]);

	
	// work phone
	r.Set(offset - font.StringWidth(VIEW_WORK_PHONE) - 11, WORK_PHONE_V,
		  WORK_PHONE_H + WORK_PHONE_W, WORK_PHONE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WPHONE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WPHONE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WPHONE] = new TTextControl(r, VIEW_WORK_PHONE, 256,
								text, M_DIRTY, M_WPHONE);
	fField[F_WPHONE]->SetTarget(this);
	v->AddChild(fField[F_WPHONE]);

	// work fax
	r.Set(WORK_FAX_H + 11, WORK_FAX_V,
		  WORK_FAX_H + WORK_FAX_W, WORK_FAX_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WFAX, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WFAX, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WFAX] = new TTextControl(r, VIEW_WORK_FAX, 256,
								text, M_DIRTY, M_WFAX, B_FOLLOW_RIGHT);
	fField[F_WFAX]->SetTarget(this);
	v->AddChild(fField[F_WFAX]);

	// mobile phone
	r.Set(offset - font.StringWidth(VIEW_WORK_CELL_PHONE) - 11, WORK_CELL_PHONE_V,
		  WORK_CELL_PHONE_H + WORK_CELL_PHONE_W, WORK_CELL_PHONE_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WCPHONE, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WCPHONE, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WCPHONE] = new TTextControl(r, VIEW_WORK_CELL_PHONE, 256,
								text, M_DIRTY, M_WCPHONE);
	fField[F_WCPHONE]->SetTarget(this);
	v->AddChild(fField[F_WCPHONE]);

	// work pager
	r.Set(WORK_PAGER_H + 11, WORK_PAGER_V,
		  WORK_PAGER_H + WORK_PAGER_W, WORK_PAGER_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WPAGER, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WPAGER, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WPAGER] = new TTextControl(r, VIEW_WORK_PAGER, 256,
								text, M_DIRTY, M_WPAGER, B_FOLLOW_RIGHT);
	fField[F_WPAGER]->SetTarget(this);
	v->AddChild(fField[F_WPAGER]);
	
	// work email
	r.Set(offset - font.StringWidth(VIEW_WORK_EMAIL) - 11, WORK_EMAIL_V,
		  WORK_EMAIL_H + WORK_EMAIL_W, WORK_EMAIL_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WEMAIL, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WEMAIL, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WEMAIL] = new TTextControl(r, VIEW_WORK_EMAIL, 256,
								text, M_DIRTY, M_WEMAIL);
	fField[F_WEMAIL]->SetTarget(this);
	v->AddChild(fField[F_WEMAIL]);

	// work URI
	r.Set(offset - font.StringWidth(VIEW_WORK_URL) - 11, WORK_URL_V,
		  WORK_URL_H + WORK_URL_W, WORK_URL_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_WURL, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_WURL, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_WURL] = new TTextControl(r, VIEW_WORK_URL, 256,
								text, M_DIRTY, M_WURL);
	fField[F_WURL]->SetTarget(this);
	v->AddChild(fField[F_WURL]);
	
	
	AddTab(v);
	TabAt(2)->SetLabel(VIEW_WORK_TAB);

	
	
	//=========================================================================
	// internet stuff


	offset = font.StringWidth(LARGEST_INET_TEXT) + 18;

	v = new BView(rb,  "3", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);

	// email 3
	r.Set(offset - font.StringWidth(VIEW_INET_EMAIL3) - 11, INET_EMAIL3_V,
		  INET_EMAIL3_H + INET_EMAIL3_W, INET_EMAIL3_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IEMAIL3, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IEMAIL3, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IEMAIL3] = new TTextControl(r, VIEW_INET_EMAIL3, 256,
								text, M_DIRTY, M_IEMAIL3);
	fField[F_IEMAIL3]->SetTarget(this);
	v->AddChild(fField[F_IEMAIL3]);

	//email 4
	r.Set(offset - font.StringWidth(VIEW_INET_EMAIL4) - 11, INET_EMAIL4_V,
		  INET_EMAIL4_H + INET_EMAIL4_W, INET_EMAIL4_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IEMAIL4, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IEMAIL4, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IEMAIL4] = new TTextControl(r, VIEW_INET_EMAIL4, 256,
								text, M_DIRTY, M_IEMAIL4);
	fField[F_IEMAIL4]->SetTarget(this);
	v->AddChild(fField[F_IEMAIL4]);

	//email5
	r.Set(offset - font.StringWidth(VIEW_INET_EMAIL5) - 11, INET_EMAIL5_V,
		  INET_EMAIL5_H + INET_EMAIL5_W, INET_EMAIL5_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IEMAIL5, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IEMAIL5, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IEMAIL5] = new TTextControl(r, VIEW_INET_EMAIL5, 256,
								text, M_DIRTY, M_IEMAIL5);
	fField[F_IEMAIL5]->SetTarget(this);
	v->AddChild(fField[F_IEMAIL5]);

	// uri
	r.Set(offset - font.StringWidth(VIEW_INET_URL3) - 11, INET_URL3_V,
		  INET_URL3_H + INET_URL3_W, INET_URL3_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IURL3, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IURL3, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IURL3] = new TTextControl(r, VIEW_INET_URL3, 256,
								text, M_DIRTY, M_IURL3);
	fField[F_IURL3]->SetTarget(this);
	v->AddChild(fField[F_IURL3]);

	// inet ICQ
	r.Set(offset - font.StringWidth(VIEW_INET_ICQ) - 11, INET_ICQ_V,
		  INET_ICQ_H + INET_ICQ_W, INET_ICQ_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IICQ, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IICQ, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IICQ] = new TTextControl(r, VIEW_INET_ICQ, 256,
								text, M_DIRTY, M_IICQ);
	fField[F_IICQ]->SetTarget(this);
	v->AddChild(fField[F_IICQ]);

	// inet AIM
	r.Set(INET_AIM_H + 11, INET_AIM_V,
		  INET_AIM_H + INET_AIM_W, INET_AIM_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IAIM, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IAIM, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IAIM] = new TTextControl(r, VIEW_INET_AIM, 256,
								text, M_DIRTY, M_IAIM);
	fField[F_IAIM]->SetTarget(this);
	v->AddChild(fField[F_IAIM]);
	

	// inet Yahoo messenger
	r.Set(offset - font.StringWidth(VIEW_INET_YAHOO) - 11, INET_YAHOO_V,
		  INET_YAHOO_H + INET_YAHOO_W, INET_YAHOO_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IYAHOO, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IYAHOO, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IYAHOO] = new TTextControl(r, VIEW_INET_YAHOO, 256,
								text, M_DIRTY, M_IYAHOO);
	fField[F_IYAHOO]->SetTarget(this);
	v->AddChild(fField[F_IYAHOO]);
	
	// inet MSN
	r.Set(INET_MSN_H + 11, INET_MSN_V,
		  INET_MSN_H + INET_MSN_W, INET_MSN_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IMSN, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IMSN, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IMSN] = new TTextControl(r, VIEW_INET_MSN, 256,
								text, M_DIRTY, M_IMSN);
	fField[F_IMSN]->SetTarget(this);
	v->AddChild(fField[F_IMSN]);

	// inet Jabber
	r.Set(offset - font.StringWidth(VIEW_INET_JABBER) - 11, INET_JABBER_V,
		  INET_JABBER_H + INET_JABBER_W, INET_JABBER_V + TEXT_HEIGHT);
	if (fFile) {
		if (fFile->GetAttrInfo(P_IJABBER, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_IJABBER, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_IJABBER] = new TTextControl(r, VIEW_INET_JABBER, 256,
								text, M_DIRTY, M_IJABBER);
	fField[F_IJABBER]->SetTarget(this);
	v->AddChild(fField[F_IJABBER]);


	AddTab(v);
	TabAt(3)->SetLabel(VIEW_INET_TAB);


	//=========================================================================
	// notes
	
	v = new BView(rb,  "3", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);

	r = rb;
	r.OffsetBy(5,5);
	r.InsetBy(15,15);
	if (fFile) {
		if (fFile->GetAttrInfo(P_NOTES, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_NOTES, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_NOTES] = new TTextControl(r, "", 256,
								text, M_DIRTY, M_NOTES);
	fField[F_NOTES]->SetTarget(this);
	v->AddChild(fField[F_NOTES]);

	AddTab(v);
	TabAt(4)->SetLabel(VIEW_NOTES_TAB);


	//===========================================================================
	// preferences
	
	v = new BView(rb,  "4", B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE);
	v->SetViewColor(c);

	r = rb;
	r.OffsetBy(5,5);
	r.InsetBy(15,15);
	/*if (fFile) {
		if (fFile->GetAttrInfo(P_NOTES, &info) == B_NO_ERROR) {
			text = (char *)realloc(text, info.size+1);
			fFile->ReadAttr(P_NOTES, B_STRING_TYPE, 0, text, info.size);
		}
	}
	fField[F_NOTES] = new TTextControl(r, "", 256,
								text, M_DIRTY, M_NOTES);
	fField[F_NOTES]->SetTarget(this);
	v->AddChild(fField[F_NOTES]);
*/
	AddTab(v);
	TabAt(5)->SetLabel(VIEW_PREFS_TAB);
	
	
	
	
	
	
	
	
	
	
	
	
	
	//-----------------------------------------------------------------------------
	// final focus ! at last !
	// selecting tab1 then tab0 is awful, but if i don't do that, the first tab
	// never draws ! i must switch manually to make its fielfs appear !

	free(text);

	Select(1);
	Select(0);
	fField[F_FIRSTNAME]->MakeFocus();
		

}

//--------------------------------------------------------------------------------------------

void TPeopleView::MessageReceived(BMessage* msg)
{
	int32		loop;
	char		*buff;
	char		str[256];
	char		**arg_v; 
	int32		arg_c = 2;
	thread_id	t;
	extern char	**environ;

	BTextView	*text;



	switch (msg->what) {
		
		case M_PEOPLE_ACTIVATE:
		
			break;
		case M_PREFS_ACTIVATE:
		
		
			break;

		case M_SAVE:
			Save();
			break;

		case M_REVERT:
			for (loop = 0; loop < F_END; loop++)
				fField[loop]->Revert();
			break;

		case M_SELECT:
			for (loop = 0; loop < F_END; loop++) {
				text = (BTextView *)fField[loop]->ChildAt(0);
				if (text->IsFocus()) {
					text->Select(0, text->TextLength());
					break;
				}
			}
			break;
			
		case M_LAUNCH_HMAIL:
			buff = (char *)malloc(sizeof(char) * ( 7 + strlen(fField[F_HEMAIL]->Text())) + 1);
			sprintf(buff,"mailto:%s",fField[F_HEMAIL]->Text());
			goto launch_thread;
		case M_LAUNCH_WMAIL:
			buff = (char *)malloc(sizeof(char) * ( 7 + strlen(fField[F_WEMAIL]->Text())) + 1);
			sprintf(buff,"mailto:%s",fField[F_WEMAIL]->Text());
			goto launch_thread;
		case M_LAUNCH_IMAIL3:
			buff = (char *)malloc(sizeof(char) * ( 7 + strlen(fField[F_IEMAIL3]->Text())) + 1);
			sprintf(buff,"mailto:%s",fField[F_IEMAIL3]->Text());
			goto launch_thread;
		case M_LAUNCH_IMAIL4:
			buff = (char *)malloc(sizeof(char) * ( 7 + strlen(fField[F_IEMAIL4]->Text())) + 1);
			sprintf(buff,"mailto:%s",fField[F_IEMAIL4]->Text());
			goto launch_thread;
		case M_LAUNCH_IMAIL5:
			buff = (char *)malloc(sizeof(char) * ( 7 + strlen(fField[F_IEMAIL5]->Text())) + 1);
			sprintf(buff,"mailto:%s",fField[F_IEMAIL5]->Text());
							
			launch_thread:
			arg_v = (char **)malloc(sizeof(char *) * (arg_c + 1));

			arg_v[0] = strdup("/boot/apps/BeMail");
			arg_v[1] = strdup(buff);
			arg_v[2] = NULL;

			t = load_image(arg_c, (const char**) arg_v, (const char**) environ);
			resume_thread(t);

			while (--arg_c >= 0)
				free(arg_v[arg_c]);
			free(arg_v);
			free(buff);

			

	////////////

			break;
			
		case M_LAUNCH_HURL:
		case M_LAUNCH_WURL:
		case M_LAUNCH_IURL3:
			
			break;

		case M_DIRTY:
		
			break;
		case M_WINDOW_UPDATE_TITLE:
			if( strlen(fField[F_FIRSTNAME]->Text()) != 0 || strlen(fField[F_LASTNAME]->Text()) != 0 ){
				
				sprintf(str,APP_NAME" "APP_VERSION" - %s %s", fField[F_FIRSTNAME]->Text(), fField[F_LASTNAME]->Text());
				
			} else {
				
				sprintf(str, APP_NAME" "APP_VERSION" - "UNKNOWN_PERSON);
			}

			Window()->SetTitle(str);				

			break;

			default:
			
				if(msg->WasDropped()){
				
				int32 result = (new BAlert("Message dropped", "soon i will do something with what you dropped :)", "OK"))->Go();

				}
			// end default
	}
}

//--------------------------------------------------------------------

void TPeopleView::BuildGroupMenu(void)
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
		item->SetEnabled(false);
	}
}

//--------------------------------------------------------------------

bool TPeopleView::CheckSave(void)
{
	int32	loop;

	for (loop = 0; loop < F_END; loop++)
		if (fField[loop]->Changed())
			return true;

	return false;
}

//--------------------------------------------------------------------

const char* TPeopleView::GetField(int32 index)
{
	if (index < F_END)
		return fField[index]->Text();
	else
		return NULL;
}

//--------------------------------------------------------------------

void TPeopleView::NewFile(entry_ref *ref)
{
	if (fFile){
		delete fFile;
		delete fEntry;	
	}
	
	fFile = new BFile(ref, O_RDWR);
	fEntry = new BEntry(ref);
	
	Save();
}

//--------------------------------------------------------------------

void TPeopleView::Save(void)
{
	const char	*text;
	char 		compatName[256], probableEntryName[256], currentEntryName[256];

	text = fField[F_FIRSTNAME]->Text();
	fFile->WriteAttr(P_FIRSTNAME, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_FIRSTNAME]->Update();
	
	text = fField[F_LASTNAME]->Text();
	fFile->WriteAttr(P_LASTNAME, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_LASTNAME]->Update();
	
	// sauvegarde du champ "Prénom Nom" (compat BeMail et autres) META:name
	// warning, snprintf doesn't exist on PPC !!!
 	sprintf(compatName, "%s %s", fField[F_FIRSTNAME]->Text(),fField[F_LASTNAME]->Text());
 	fFile->WriteAttr(P_NAME, B_STRING_TYPE, 0, compatName, strlen(compatName) + 1);
 	//pas d'update, car il n'est pas visible (pas de field associé)
	
	text = fField[F_GROUPNAME]->Text();
	fFile->WriteAttr(P_GROUPNAME, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_GROUPNAME]->Update();

	text = fField[F_TITLE]->Text();
	fFile->WriteAttr(P_TITLE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_TITLE]->Update();

	text = fField[F_NICKNAME]->Text();
	fFile->WriteAttr(P_NICKNAME, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_NICKNAME]->Update();
	
	
	// home stuff
	

	text = fField[F_HADDRESS]->Text();
	fFile->WriteAttr(P_HADDRESS, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HADDRESS]->Update();

	text = fField[F_HADDRESS2]->Text();
	fFile->WriteAttr(P_HADDRESS2, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HADDRESS2]->Update();
	
	text = fField[F_HCITY]->Text();
	fFile->WriteAttr(P_HCITY, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HCITY]->Update();

	text = fField[F_HSTATE]->Text();
	fFile->WriteAttr(P_HSTATE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HSTATE]->Update();

	text = fField[F_HZIPCODE]->Text();
	fFile->WriteAttr(P_HZIPCODE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HZIPCODE]->Update();

	text = fField[F_HCOUNTRY]->Text();
	fFile->WriteAttr(P_HCOUNTRY, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HCOUNTRY]->Update();

	text = fField[F_HPHONE]->Text();
	fFile->WriteAttr(P_HPHONE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HPHONE]->Update();
	
	text = fField[F_HFAX]->Text();
	fFile->WriteAttr(P_HFAX, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HFAX]->Update();
	
	// no pager
	
	text = fField[F_HCPHONE]->Text();
	fFile->WriteAttr(P_HCPHONE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HCPHONE]->Update();	

	text = fField[F_HEMAIL]->Text();
	fFile->WriteAttr(P_HEMAIL, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HEMAIL]->Update();

	text = fField[F_HURL]->Text();
	fFile->WriteAttr(P_HURL, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_HURL]->Update();

	// work stuff

	text = fField[F_WCOMPANYNAME]->Text();
	fFile->WriteAttr(P_WCOMPANYNAME, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WCOMPANYNAME]->Update();

	text = fField[F_WPOSITION]->Text();
	fFile->WriteAttr(P_WPOSITION, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WPOSITION]->Update();

	text = fField[F_WADDRESS]->Text();
	fFile->WriteAttr(P_WADDRESS, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WADDRESS]->Update();

	text = fField[F_WADDRESS2]->Text();
	fFile->WriteAttr(P_WADDRESS2, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WADDRESS2]->Update();
	
	text = fField[F_WCITY]->Text();
	fFile->WriteAttr(P_WCITY, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WCITY]->Update();

	text = fField[F_WSTATE]->Text();
	fFile->WriteAttr(P_WSTATE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WSTATE]->Update();

	text = fField[F_WZIPCODE]->Text();
	fFile->WriteAttr(P_WZIPCODE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WZIPCODE]->Update();

	text = fField[F_WCOUNTRY]->Text();
	fFile->WriteAttr(P_WCOUNTRY, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WCOUNTRY]->Update();

	text = fField[F_WPHONE]->Text();
	fFile->WriteAttr(P_WPHONE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WPHONE]->Update();

	text = fField[F_WFAX]->Text();
	fFile->WriteAttr(P_WFAX, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WFAX]->Update();

	text = fField[F_WCPHONE]->Text();
	fFile->WriteAttr(P_WCPHONE, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WCPHONE]->Update();

	text = fField[F_WPAGER]->Text();
	fFile->WriteAttr(P_WPAGER, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WPAGER]->Update();

	text = fField[F_WEMAIL]->Text();
	fFile->WriteAttr(P_WEMAIL, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WEMAIL]->Update();

	text = fField[F_WURL]->Text();
	fFile->WriteAttr(P_WURL, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_WURL]->Update();


	// internet stuff

	text = fField[F_IEMAIL3]->Text();
	fFile->WriteAttr(P_IEMAIL3, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IEMAIL3]->Update();

	text = fField[F_IEMAIL4]->Text();
	fFile->WriteAttr(P_IEMAIL4, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IEMAIL4]->Update();

	text = fField[F_IEMAIL5]->Text();
	fFile->WriteAttr(P_IEMAIL5, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IEMAIL5]->Update();

	text = fField[F_IICQ]->Text();
	fFile->WriteAttr(P_IICQ, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IICQ]->Update();

	text = fField[F_IAIM]->Text();
	fFile->WriteAttr(P_IAIM, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IAIM]->Update();

	text = fField[F_IJABBER]->Text();
	fFile->WriteAttr(P_IJABBER, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IJABBER]->Update();
	
	text = fField[F_IMSN]->Text();
	fFile->WriteAttr(P_IMSN, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IMSN]->Update();

	text = fField[F_IYAHOO]->Text();
	fFile->WriteAttr(P_IYAHOO, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IYAHOO]->Update();

	text = fField[F_IURL3]->Text();
	fFile->WriteAttr(P_IURL3, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_IURL3]->Update();

	text = fField[F_NOTES]->Text();
	fFile->WriteAttr(P_NOTES, B_STRING_TYPE, 0, text, strlen(text) + 1);
	fField[F_NOTES]->Update();


	if(1){

		// auto rename feature

		if (fFile)
			delete fFile;
	
		sprintf(probableEntryName, "%s", compatName);
	
		int32 i = 2;
		
		fEntry->GetName(currentEntryName);
		
		if( strcmp(probableEntryName, currentEntryName) ){
		
			while ( fEntry->Rename(probableEntryName) != B_OK ){
				sprintf(probableEntryName, "%s %d", compatName, i);
				i++;
			}
		}
		
		fFile = new BFile(fEntry, O_RDWR);
	}
}

//--------------------------------------------------------------------

void TPeopleView::SetField(int32 index, char *data, bool update)
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

bool TPeopleView::TextSelected(void)
{
	int32		end;
	int32		loop;
	int32		start;
	BTextView	*text;

	for (loop = 0; loop < F_END; loop++) {
		text = (BTextView *)fField[loop]->ChildAt(0);
		text->GetSelection(&start, &end);
		if (start != end)
			return true;
	}
	return false;
}
