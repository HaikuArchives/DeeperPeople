//--------------------------------------------------------------------
//	
//	PeopleWindow.cpp
//
//	Written by: Robert Polic
//	Extended by: Olivier Cortes
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <MenuBar.h>
#include <MenuItem.h>
#include <FilePanel.h>
#include <NodeInfo.h>
#include <stdio.h>
#include <Alert.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Font.h>
#include <Clipboard.h>
#include <string.h>

#include "PeopleApp.h"
#include "PeopleLang.h"
#include "PeopleSplitPane.h"
#include "PeopleTreeView.h"
#include "PeoplePrefView.h"
#include "PeopleView.h"
#include "PeopleWindow.h"
#include "TTextControl.h"

//====================================================================

TPeopleWindow::TPeopleWindow(BRect rect, char *title = NULL, entry_ref *ref = NULL)
			 :BWindow(rect, title, B_TITLED_WINDOW, 0)
{
	BMenu		*menu;
	BMenuBar	*menu_bar;
	BMenuItem	*item;
	BRect		rback,rtree,rightRect, leftRect,rtabview,rsearch;
	BView		*vback, *rightPane, *leftPane;
	rgb_color	c;
	BPoint 		p;
	char		buff[256];
	
	c.red = c.green = c.blue = VIEW_COLOR;

	myPanel = NULL;
	
	myLastResult = 0;

	rback.Set(0, 0, MENUBAR_W, MENUBAR_H);
	menu_bar = new BMenuBar(rback, "DeeperPeopleMenuBar");
	
	menu = new BMenu(MENU_FILE);
	menu->AddItem(item = new BMenuItem(MENU_FILE_NEW_PERSON, new BMessage(M_NEW), 'N'));
	item->SetTarget(NULL, this);
	menu->AddItem(new BMenuItem(MENU_FILE_CLOSE, new BMessage(B_CLOSE_REQUESTED), 'W'));

	menu->AddSeparatorItem();

	menu->AddItem(mySave = new BMenuItem(MENU_FILE_SAVE, new BMessage(M_SAVE), 'S'));
	mySave->SetEnabled(false);
	menu->AddItem(new BMenuItem(MENU_FILE_SAVE_AS B_UTF8_ELLIPSIS, new BMessage(M_SAVE_AS)));
	menu->AddItem(myRevert = new BMenuItem(MENU_FILE_REVERT, new BMessage(M_REVERT), 'R'));
	myRevert->SetEnabled(false);

	menu->AddSeparatorItem();
	
	menu->AddItem(item = new BMenuItem(MENU_FILE_ABOUT B_UTF8_ELLIPSIS, new BMessage(B_ABOUT_REQUESTED)));
	item->SetTarget(NULL, be_app);

	menu->AddSeparatorItem();
	
	menu->AddItem(new BMenuItem(MENU_FILE_QUIT, new BMessage(B_QUIT_REQUESTED), 'Q'));

	menu_bar->AddItem(menu);


	menu = new BMenu(MENU_EDIT);
	menu->AddItem(myUndo = new BMenuItem(MENU_EDIT_UNDO, new BMessage(M_UNDO), 'Z'));
	
	menu->AddSeparatorItem();
	
	menu->AddItem(myCut = new BMenuItem(MENU_EDIT_CUT, new BMessage(B_CUT), 'X'));
	myCut->SetTarget(NULL, this);
	menu->AddItem(myCopy = new BMenuItem(MENU_EDIT_COPY, new BMessage(B_COPY), 'C'));
	myCopy->SetTarget(NULL, this);
	menu->AddItem(myPaste = new BMenuItem(MENU_EDIT_PASTE, new BMessage(B_PASTE), 'V'));
	myPaste->SetTarget(NULL, this);
	
	menu->AddSeparatorItem();
	
	menu->AddItem(item = new BMenuItem(MENU_EDIT_SELECT_ALL, new BMessage(M_SELECT), 'A'));
//	item->SetTarget(NULL, this);

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(MENU_EDIT_SEARCH, new BMessage(M_SEARCH_ACTIVATE), 'F'));
	menu->AddItem(new BMenuItem(MENU_EDIT_SEARCH_NEXT, new BMessage(M_SEARCH_NEXT), 'G'));
	menu->AddItem(item = new BMenuItem(MENU_EDIT_SEARCH_PREVIOUS, new BMessage(M_SEARCH_PREVIOUS), 'H'));
	item->SetEnabled(false);

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(MENU_EDIT_COLLAPSE_ALL, new BMessage(M_COLLAPSE_ALL), 'L'));
	menu->AddItem(new BMenuItem(MENU_EDIT_EXPAND_ALL, new BMessage(M_EXPAND_ALL), 'D'));

	menu_bar->AddItem(menu);
	AddChild(menu_bar);

	if (ref) {
		myRef = new entry_ref(*ref);
		sprintf(buff, "%s %s - %s",APP_NAME, APP_VERSION, ref->name);
		SetTitle(buff);
	} else {
		myRef = NULL;
		sprintf(buff, "%s", APP_TITLE_TEXT);
		SetTitle(buff);
	}
	
	
	//---------------------------------------------------------------------------------------------------------
	// back of the window
	
	rback = Bounds();
	rback.OffsetTo(0, MENUBAR_H + 1);
	rback.bottom -= MENUBAR_H + 1;
	
	vback = new BView(rback, "WindowBackView", B_FOLLOW_ALL, B_WILL_DRAW);
	vback->SetViewColor(c);


	//---------------------------------------------------------------------------------------------------------
	// left pane (scroll view + tree view + status bar)

	leftRect.Set(0, 0, TREE_W , TREE_H + STATUS_H);
	leftPane = new BView(rightRect, "LeftPane", B_FOLLOW_ALL, B_WILL_DRAW);
	leftPane->SetViewColor(c);

	// l t r b 
	rtree.Set(0,0,TREE_W, TREE_H);
	myTreeView = new TPeopleTreeView(rtree,"PeopleTreeView");	

	myTreeScrollView = new BScrollView("PeopleMainScroll", (BOutlineListView *)myTreeView, B_FOLLOW_ALL, 0, false, true);
	myTreeScrollView->SetViewColor(c);																		// true for horiz

//	leftPane->AddChild(myTreeScrollView);
	
//	rtree.Set(0, myTreeScrollView->Bounds().Height() + 1 , myTreeScrollView->Bounds().Width(), STATUS_H);
//	myProgressBar = new BStatusBar(rtree, "Builder Progress");
//	myProgressBar->SetResizingMode(B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
//	leftPane->AddChild(myProgressBar);
	

	//---------------------------------------------------------------------------------------------------------
	// right pane ( search + peopleview )

	rightRect.Set(0, 0, TABVIEW_W , TABVIEW_H + SEARCH_H);
	myBackTab = new BView(rightRect, "PeopleBackView", B_FOLLOW_ALL, B_WILL_DRAW);
	myBackTab->SetViewColor(c);
	
	rsearch.Set(4 * MARGIN_W, 			// 3 from the right + 1 from the back view
				MARGIN_H,
				SEARCH_W - 3 * MARGIN_W,
				SEARCH_H - MARGIN_H);
	mySearchField = new BTextControl(rsearch,
										"searchField",
										VIEW_SEARCH_LABEL,
										VIEW_SEARCH_DEFAULT_TEXT,
										new BMessage(M_SEARCH),
										B_FOLLOW_LEFT_RIGHT);
	mySearchField->SetViewColor(c);
	mySearchField->SetDivider(mySearchField->StringWidth(VIEW_SEARCH_LABEL) + 7);
	mySearchField->SetEnabled(false);
	myBackTab->AddChild(mySearchField);

	rtabview.Set(MARGIN_W, 
				SEARCH_H + 1,
				TABVIEW_W,				// no margin because there's already the margin of the splitpane...
				TABVIEW_H + SEARCH_H);

	myPeopleView = new TPeopleView(rtabview, "PeopleView", myRef);
	myBackTab->AddChild(myPeopleView);
	

	//---------------------------------------------------------------------------------------------------------
	// split pane


	rback.Set(MARGIN_W, MARGIN_H, vback->Bounds().Width() - MARGIN_W, vback->Bounds().Height() - MARGIN_H);
//	mySplitPane = new SplitPane(rback, leftPane, (BView*) myBackTab, B_FOLLOW_ALL);
	mySplitPane = new SplitPane(rback, myTreeScrollView, (BView*) myBackTab, B_FOLLOW_ALL);
	mySplitPane->SetViewColor(c);

	p.Set(SPLIT_W,SPLIT_W);
	mySplitPane->SetBarThickness(p);
	p.Set(TREE_W,TREE_W);
	mySplitPane->SetBarPosition(p);
	mySplitPane->SetMinSizeOne(p);
	p.Set(TABVIEW_W,TABVIEW_W);
    mySplitPane->SetMinSizeTwo(p);
	

	vback->AddChild((BView*)mySplitPane);

	Lock();
	SetSizeLimits(WINDOW_W, 32768, WINDOW_H, 32768);
	AddChild(vback);
	Unlock();
	
	//preparequery here ?
}

//---------------------------------------------------------------------

void TPeopleWindow::SetRef(entry_ref* ref)
{

	BRect	rtabview;
	char	buff[256];

	if (ref) {
		if(myRef){
		
			myPeopleView->Save();
			
			delete myRef;
			myRef = new entry_ref(*ref);
			
			LockLooper();
			rtabview.Set(MARGIN_W, 
				SEARCH_H + 1,
				myBackTab->Bounds().Width(),				// no margin because there's already the margin of the splitpane...
				myBackTab->Bounds().Height());
	
			myBackTab->RemoveChild(myPeopleView);
			myBackTab->AddChild( myPeopleView = new TPeopleView(rtabview, "PeopleView", myRef));
			UnlockLooper();
			
		} else if(myPeopleView->CheckSave()) {
		
			SaveAs();
		
		} else {

			myRef = new entry_ref(*ref);

			LockLooper();
			rtabview.Set(MARGIN_W, 
				SEARCH_H + 1,
				myBackTab->Bounds().Width(),				// no margin because there's already the margin of the splitpane...
				myBackTab->Bounds().Height());			

			myBackTab->RemoveChild(myPeopleView);
			myBackTab->AddChild( myPeopleView = new TPeopleView(rtabview, "PeopleView", myRef));
			UnlockLooper();
		}

		sprintf(buff, "%s %s - %s",APP_NAME, APP_VERSION, myRef->name);
		SetTitle(buff);

	}
}

//--------------------------------------------------------------------

void TPeopleWindow::NewPerson()
{
	char  buff[256];
	BRect rtabview;
		
	if(myRef){
		
		myPeopleView->Save();
			
		delete myRef;
		myRef = NULL;
			
		LockLooper();
		rtabview.Set(MARGIN_W, 
			SEARCH_H + 1,
			myBackTab->Bounds().Width(),				// no margin because there's already the margin of the splitpane...
			myBackTab->Bounds().Height());

		myBackTab->RemoveChild(myPeopleView);
		myBackTab->AddChild( myPeopleView = new TPeopleView(rtabview, "PeopleView", myRef));
		UnlockLooper();
			
		} else if(myPeopleView->CheckSave()) {
			SaveAs();
		}

		sprintf(buff, "%s",APP_TITLE_TEXT);
		SetTitle(buff);
}

//--------------------------------------------------------------------

TPeopleWindow::~TPeopleWindow(void)
{
	if (myRef)
		delete myRef;
	if (myPanel)
		delete myPanel;
}

//--------------------------------------------------------------------

void TPeopleWindow::MenusBeginning(void)
{
	bool	enabled;

	enabled = myPeopleView->CheckSave();
	mySave->SetEnabled(enabled);
	myRevert->SetEnabled(enabled);

	myUndo->SetEnabled(FALSE);
	enabled = myPeopleView->TextSelected();
	myCut->SetEnabled(enabled);
	myCopy->SetEnabled(enabled);

	be_clipboard->Lock();
	myPaste->SetEnabled(be_clipboard->Data()->HasData("text/plain", B_MIME_TYPE));
	be_clipboard->Unlock();

	myPeopleView->BuildGroupMenu();
}

//--------------------------------------------------------------------

void TPeopleWindow::MessageReceived(BMessage* msg)
{
	char			str[256];
	entry_ref		dir;
	BEntry			entry;
	BFile			file;
	int32			personIndex, result;
	BDirectory		directory;
	BNodeInfo		*node;
	BListItem		*item;

	switch(msg->what) {
		case B_QUERY_UPDATE:
			myTreeView->MessageReceived(msg);
			break;
		case M_NEW:
			NewPerson();
			break;

		case M_SAVE:
			if (!myRef) {
				SaveAs();
				break;
			}

		case M_REVERT:
		case M_SELECT:
			myPeopleView->MessageReceived(msg);
			break;

		case M_SAVE_AS:
			SaveAs();
			break;

		case M_COLLAPSE_ALL:
			myTreeView->CollapseAllGroups();
			break;

		case M_EXPAND_ALL:
			myTreeView->ExpandAllGroups();
			break;

		//---------------------------------------------------------------------------
		// tree selection and invocation

		case M_TREE_SELECT:
			/*
			msg->FindInt32("index", &personIndex);
			if(personIndex > 0){
				if( ! ((AbstractItem*)myTreeView->ItemAt(personIndex))->IsPerson() ) {

				}
			}
			*/
			break;
		case M_TREE_INVOKE:
			msg->FindInt32("index", &personIndex);
			if(personIndex > 0){
				if( ((AbstractItem*)myTreeView->ItemAt(personIndex))->IsPerson() ) {
					SetRef(((PersonItem*)myTreeView->ItemAt(personIndex))->GetRef());
				} else {
				
				}
			}
			break;


		//--------------------------------------------------------------------------
		// search
		
		case M_SEARCH_ENABLE:
			LockLooper();
				mySearchField->SetEnabled(true);
			UnlockLooper();
			break;

		case M_SEARCH_DISABLE:
			LockLooper();
				mySearchField->SetEnabled(false);
			UnlockLooper();
			break;

		case M_SEARCH_ACTIVATE:
			mySearchField->MakeFocus();
			myLastResult = 0;
			break;
			
		case M_SEARCH:
			myLastResult = 0;
			
		case M_SEARCH_NEXT:

			result = myTreeView->FindIncompletePerson(mySearchField->Text(), myLastResult);

			if( result > 0 && result < myTreeView->GetCount() ){

				item = myTreeView->FullListItemAt(result);
				myTreeView->CollapseAllGroups();
				myTreeView->DeselectAll();
				myTreeView->ExpandGroupFromPerson(result);
				item->Select();
				myTreeView->ScrollToSelection();
				
				SetRef(((PersonItem*)item)->GetRef());
				myLastResult = result+1;

			} else if (myLastResult != 0) {
				if( strncmp(mySearchField->Text(), VIEW_SEARCH_NO_MORE_RESULT, strlen(VIEW_SEARCH_NO_MORE_RESULT)) ) {
					sprintf(str,"%s \"%s\"", VIEW_SEARCH_NO_MORE_RESULT, mySearchField->Text());
					mySearchField->SetText(str);
				}
				mySearchField->MakeFocus();
			} else {
				if( strncmp(mySearchField->Text(), VIEW_SEARCH_NO_RESULT, strlen(VIEW_SEARCH_NO_RESULT)) ) {
					sprintf(str,"%s \"%s\"", VIEW_SEARCH_NO_RESULT, mySearchField->Text());
					mySearchField->SetText(str);
				}
				mySearchField->MakeFocus();
			}
			break;

		case M_SEARCH_PREVIOUS:
			
			break;

		case M_GROUP_MENU:
		{
			char *name = NULL;
			msg->FindString("group", (const char **)&name);
			myPeopleView->SetField(F_GROUPNAME, name, false);
			break;
		}
		case B_SAVE_REQUESTED:
			if (msg->FindRef("directory", &dir) == B_NO_ERROR) {
				const char *name = NULL;
				msg->FindString("name", &name);
				directory.SetTo(&dir);
				if (directory.InitCheck() == B_NO_ERROR) {
					directory.CreateFile(name, &file);
					if (file.InitCheck() == B_NO_ERROR) {
						node = new BNodeInfo(&file);
						node->SetType("application/x-person");
						delete node;

						directory.FindEntry(name, &entry);
						entry.GetRef(&dir);
						if (myRef)
							delete myRef;
						myRef = new entry_ref(dir);
						
						sprintf(str, "%s %s - %s", APP_NAME, APP_VERSION, myRef->name);
												
						SetTitle(str);
						myPeopleView->NewFile(myRef);
					}
					else {
						sprintf(str, ERROR_CREATION_TEXT, name);
						(new BAlert("", str, ERROR_CREATION_BUTTON_TEXT))->Go();
					}
				}
			}
			break;
		case M_LAUNCH_HMAIL:
		case M_LAUNCH_WMAIL:
		case M_LAUNCH_IMAIL3:
		case M_LAUNCH_IMAIL4:
		case M_LAUNCH_IMAIL5:
		/*
		case M_LAUNCH_HURL:
		case M_LAUNCH_WURL:
		case M_LAUNCH_IURL3:
		*/
			myPeopleView->MessageReceived(msg);
			break;

		default:
			BWindow::MessageReceived(msg);
	}
}

//--------------------------------------------------------------------

bool TPeopleWindow::QuitRequested(void)
{
	int32			count = 0;
	int32			index = 0;
	BPoint			pos;
	BRect			r;
	status_t		result;
	TPeopleWindow	*window;

	if (myPeopleView->CheckSave()) {
		result = (new BAlert("", SAVE_CHANGES_TEXT,
							SAVE_CHANGES_CANCEL_BUTTON_TEXT,
							SAVE_CHANGES_QUIT_BUTTON_TEXT,
							SAVE_CHANGES_SAVE_BUTTON_TEXT))->Go();
		if (result == 2) {
			if (myRef)
				myPeopleView->Save();
			else {
				SaveAs();
				return FALSE;
			}
		}
		else if (result == 0)
			return FALSE;
	}

	while ((window = (TPeopleWindow *)be_app->WindowAt(index++))) {
		if (window->FindView("PeopleView"))
			count++;
	}

	if (count == 1) {
		r = Frame();
		pos = r.LeftTop();
		if (((TPeopleApp*)be_app)->myPrefs) {
			((TPeopleApp*)be_app)->myPrefs->Seek(0, 0);
			((TPeopleApp*)be_app)->myPrefs->Write(&pos, sizeof(BPoint));
		}
		
		myTreeView->KillTreeBuilder();

		be_app->PostMessage(B_QUIT_REQUESTED);
	}
	
	
	return TRUE;
}

//--------------------------------------------------------------------

void TPeopleWindow::DefaultName(char *name)
{

	sprintf(name, "%s %s", myPeopleView->GetField(F_FIRSTNAME), myPeopleView->GetField(F_LASTNAME));

	while (*name) {
		if (*name == '/')
			*name = '-';
		name++;
	}
}

//--------------------------------------------------------------------

void TPeopleWindow::SetField(int32 index, char *text)
{
	myPeopleView->SetField(index, text, TRUE);
}

//--------------------------------------------------------------------

void TPeopleWindow::SaveAs(void)
{
	char		name[B_FILE_NAME_LENGTH];
	BDirectory	dir;
	BEntry		entry;
	BMessenger	window(this);
	BPath		path;

	DefaultName(name);
	if (!myPanel) {
		myPanel = new BFilePanel(B_SAVE_PANEL, &window);
		myPanel->SetSaveText(name);
		find_directory(B_USER_DIRECTORY, &path, true);
		dir.SetTo(path.Path());
		if (dir.FindEntry("people", &entry) == B_NO_ERROR)
			myPanel->SetPanelDirectory(&entry);
		else if (dir.CreateDirectory("people", &dir) == B_NO_ERROR) {
			dir.GetEntry(&entry);
			myPanel->SetPanelDirectory(&entry);
		}
	}
	else if (myPanel->Window()->Lock()) {
		if (!myPanel->Window()->IsHidden())
			myPanel->Window()->Activate();
		else
			myPanel->SetSaveText(name);
		myPanel->Window()->Unlock();
	}

	if (myPanel->Window()->Lock()) {
		if (myPanel->Window()->IsHidden())
			myPanel->Window()->Show();
		myPanel->Window()->Unlock();
	}	
}
