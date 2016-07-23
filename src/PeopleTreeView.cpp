//--------------------------------------------------------------------
//	
//	PeopleTreeView.cpp
//
//	Written by: Olivier Cortes
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
#include <NodeMonitor.h>
#include <String.h>
#include <VolumeRoster.h>
#include <malloc.h>
#include <OS.h>
#include <OutlineListView.h>
#include <string.h>
#include <stdio.h>

#include "PeopleTreeView.h"
#include "TTextControl.h"

//--------------------------------------------------------------------------------------

int CompareStringItems(const BListItem *i1, const BListItem *i2){
	
	return(strcmp(((BStringItem*) i1)->Text(),((BStringItem*) i2)->Text()));
	
}

//--------------------------------------------------------------------------------------

int32 TreeBuilder( void * data )
{
	TPeopleTreeView		*myTree	 = (TPeopleTreeView*) data;
	BQuery				*myQuery = myTree->GetQuery();
	
	BFile				myFile;
	BListItem			*item;
	GroupItem			*group;
	GroupItem			*groups_root = myTree->GetPeopleGroupsRoot();
	GroupItem			*orphans_root = myTree->GetPeopleOrphansRoot();
	
	entry_ref			ref;
	attr_info			info_group;
	
	char				*text_group, *offset;
	
	while (myQuery->GetNextRef(&ref) == B_NO_ERROR) {

		myFile.SetTo(&ref, O_RDONLY);

		if ((myFile.InitCheck() == B_NO_ERROR) && (myFile.GetAttrInfo(P_GROUPNAME, &info_group) == B_NO_ERROR)) {
		
			if(info_group.size > 1) {
			
				text_group = (char *)malloc(info_group.size);
				myFile.ReadAttr(P_GROUPNAME, B_STRING_TYPE, 0, text_group, info_group.size);

				while (1) {
				
					//----------------------------------------------------------------
					// fetch group and verify we don't already have it.
				
					if ((offset = strstr(text_group, ",")))
						*offset = 0;
					
					group = myTree->FindGroup(text_group);
					
					if ( ! group ) {
						
						groups_root->IncrementCount();
						
						myTree->LockLooper();
							myTree->InvalidateItem(0);
							myTree->AddUnder( group = new GroupItem(text_group), groups_root);
						myTree->UnlockLooper();
						
						myTree->IncrementCount();
					}	


					if (offset) {
						text_group = offset + 1;
						while (*text_group == ' ')
							text_group++;
					} else {
				
						// just before break, attach the person after the group !
											
						myTree->AddPersonInGroup(&myFile, &ref, group, false);
						myTree->IncrementPeopleCount();
						
						break; // exit from the while
					}
				}
			} else {

				// no group, the person is orphan !			
			
				myTree->AddPersonInGroup(&myFile, &ref, orphans_root, false);
				myTree->IncrementPeopleCount();

			}	
		}
	}

	//-------------------------------------------
	// sort if needed
	
	if( groups_root->GetCount() > 1){
	
		myTree->LockLooper();
			myTree->SortItemsUnder( groups_root, false, &CompareStringItems);		
		myTree->UnlockLooper();
	
	}

	//----------------------------------------------------------------
	// Redraw the tree and set up invocation messages...

	groups_root->SetEnabled(true);
	orphans_root->SetEnabled(true);

	myTree->LockLooper();
		myTree->Expand((BListItem*)myTree->GetPeopleGroupsRoot());
	myTree->UnlockLooper();

	myTree->SetMessages();
	
	return B_OK;
}
	
	
//====================================================================

TPeopleTreeView::TPeopleTreeView(BRect rect, char *title)
		   :BOutlineListView(rect, title, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE)
{
	
	//SetPulseRate(100000);
	
	myCount = 0;
	myPeopleCount = 0;

}

//--------------------------------------------------------------------

TPeopleTreeView::~TPeopleTreeView(void)
{

}

//-------------------------------------------------------------------------------------

void TPeopleTreeView::CollapseAllGroups(void)
{

	int32 icg, i;
	BListItem	*item;

	LockLooper();
		
		for(i=0; i < myGroupsRoot->GetCount(); i++) {
			Collapse(ItemUnderAt(myGroupsRoot, true, i));
		}

		Collapse(myOrphansRoot);

	UnlockLooper();

}

//--------------------------------------------------------------------------

void TPeopleTreeView::ExpandAllGroups(void)
{

	int32 icg, i;

	LockLooper();
		
		for(i=0; i < myGroupsRoot->GetCount(); i++) {
			Expand( ItemUnderAt(myGroupsRoot, true, i) );
		}
		
		Expand( myOrphansRoot );
		

	UnlockLooper();

}
//--------------------------------------------------------------------------------------

void TPeopleTreeView::ExpandGroupFromPerson(int32 index)
{
	int32 i;
	BListItem *item = Superitem(FullListItemAt(index));
	BListItem *item2;	

	for(i=0; i < myGroupsRoot->GetCount(); i++) {

		item2 = ItemUnderAt(myGroupsRoot, true, i);

		if(item == item2) {
			Expand(item2);
			LockLooper();
				Invalidate();
			UnlockLooper();
			return;
		}
	}
	
	Expand(myOrphansRoot);
	LockLooper();
		Invalidate();
	UnlockLooper();
}

//--------------------------------------------------------------------------------------

void TPeopleTreeView::PrepareQuery() {

	attr_info		info_group, info_name;
	int32			result;
	BListItem		*item, *groot, *oroot, *group;
	BVolume			vol;
	BVolumeRoster	volume;

	LockLooper();
		AddItem( myGroupsRoot	= new GroupItem(VIEW_PEOPLE_TREE_GROUPS)	);
		AddItem( myOrphansRoot	= new GroupItem(VIEW_PEOPLE_TREE_ORPHANS)	);
	UnlockLooper();

	myGroupsRoot->SetEnabled(false);
	myOrphansRoot->SetEnabled(false);

	// don't cache global count ++ ++, it is done in the constructor.

	volume.GetBootVolume(&vol);

	myQuery = new BQuery();
	myQuery->SetVolume(&vol);
	if(	myQuery->SetTarget(Window()) != B_OK )
		result = (new BAlert("Message", "Couldn't assign Query messenger ! the query will not be Live.", "Too bad !"))->Go();


	myQuery->PushAttr(P_GROUPNAME);
	myQuery->PushString("**");
	myQuery->PushOp(B_EQ);

	myQuery->PushAttr(P_GROUPNAME);
	myQuery->PushString("**");
	myQuery->PushOp(B_NE);
	
	myQuery->PushOp(B_OR);
	
	myQuery->Fetch();
	
	myQueryThread = spawn_thread(TreeBuilder, "Tree Builder", B_NORMAL_PRIORITY, (void*) this);
	resume_thread(myQueryThread);
}

//----------------------------------------------------------------------------------------------

void TPeopleTreeView::AddPersonInGroup(BFile * file, entry_ref *ref, GroupItem * group, bool withsearch) {

	char		*text_name;
	char		*buff;
	attr_info	info_name;
	node_ref	nref;
	BListItem	*person = NULL;
	BString		searchText;

	if ( (file->GetAttrInfo(P_NAME, &info_name) == B_NO_ERROR) && (info_name.size > 1)) {
	
		text_name = (char *)malloc(info_name.size);
		file->ReadAttr(P_NAME, B_STRING_TYPE, 0, text_name, info_name.size);

		// search in the entire list because the person can be already in another group.
		// later we could add a link to say person belongs to this group TOO.

		if(withsearch)
			person = FindPerson(text_name);
				
		if ( ! person ) {

			searchText.SetTo(text_name);
		
			file->GetAttrInfo(P_NICKNAME, &info_name);	
			buff = (char *)malloc(info_name.size);		
			file->ReadAttr(P_NICKNAME, B_STRING_TYPE, 0, buff, info_name.size);
			searchText += buff;

			file->GetAttrInfo(P_WCOMPANYNAME, &info_name);		
			buff = (char *)realloc(buff, info_name.size);	
			file->ReadAttr(P_WCOMPANYNAME, B_STRING_TYPE, 0, buff, info_name.size);
			searchText += buff;


			file->GetAttrInfo(P_NOTES, &info_name);			
			buff = (char *)realloc(buff, info_name.size);
			file->ReadAttr(P_NOTES, B_STRING_TYPE, 0, buff, info_name.size);
			searchText += buff;

			LockLooper();
				AddUnder((BStringItem *) new PersonItem(text_name, ref, searchText), group);
			UnlockLooper();
			
			group->IncrementCount();
		}
	}
}



//-----------------------------------------------------------------------------

GroupItem *TPeopleTreeView::FindGroup(const char* group) {

	int32 i;
	
	for(i=0; i < myGroupsRoot->GetCount(); i++) {
		if( ! strncmp(group, ((BStringItem*)ItemUnderAt(myGroupsRoot, true, i))->Text(), strlen(group)) )
			return ((GroupItem*)ItemUnderAt(myGroupsRoot, true, i));
	}

	return NULL;
}

//-------------------------------------------------------------------------------------

PersonItem *TPeopleTreeView::FindPersonInGroup(const char* people, GroupItem *group) {

	int32 i;

	for(i=0; i < group->GetCount(); i++) {
		if( ! strcmp(people, ((BStringItem*)ItemUnderAt(group, true, i))->Text()) )
			return (PersonItem*)ItemUnderAt(group, true, i);
	}

	return NULL;
}


//-------------------------------------------------------------------------------------

PersonItem *TPeopleTreeView::FindPerson(const char* name) {

	int32 i;
	BListItem *item;
	
	for(i=0; i < myCount; i++) {
		item = FullListItemAt(i);
		if( item && ((AbstractItem*)item)->IsPerson()
			&& ! strcmp(name, ((BStringItem*) item)->Text()) )
				return (PersonItem*)item;
	}

	return NULL;
}

//-------------------------------------------------------------

int32 TPeopleTreeView::FindIncompletePerson(const char* string, int32 start = 0, int32 rewind = 0) {

	int32 i;
	BListItem *item;
	
	if(rewind) {
	
		// search backward.
		
	} else {
	
		for(i=start; i < myCount; i++) {
			item = FullListItemAt(i);
			if( item && ((AbstractItem*)item)->IsPerson() 
				&& ((PersonItem*)item)->GetDescr()->IFindFirst(string) != B_ERROR) {
				return i;
			}
		}
	}
	
	return 0;
}

//-------------------------------------------------------------

int32 TPeopleTreeView::FindPersonFromNode(node_ref * node) {

	int32 i;
	BListItem *item;
	
	for(i=0; i < myCount; i++) {
		item = FullListItemAt(i);
		if( item && ((AbstractItem*)item)->IsPerson() 
			&& ((PersonItem*)item)->GetNodeRef() == node ) {
			return i;
		}
	}

	return 0;
}

//--------------------------------------------------------------------

void TPeopleTreeView::AttachedToWindow(void)
{
	PrepareQuery();
}

//------------------------------------------------------------------

void TPeopleTreeView::SetMessages(void)
{

	//BuildList();
 
 	if(0){
		// one-click mode
		SetSelectionMessage(new BMessage(M_TREE_INVOKE));
	} else {
		SetSelectionMessage(new BMessage(M_TREE_SELECT));
	}

	SetInvocationMessage(new BMessage(M_TREE_INVOKE));
	
	SetTarget((BMessenger) Window());
	
	Window()->MessageReceived(new BMessage(M_SEARCH_ENABLE));
}
//--------------------------------------------------------------------

void TPeopleTreeView::MessageReceived(BMessage* msg)
{
	int32 			opcode;
	entry_ref		ref; 
	const char		*name;
	node_ref		nref; 

	BListItem		*item;	

	GroupItem		*group;


	switch (msg->what) {
	
		case B_QUERY_UPDATE:
	
	//int32 result = (new BAlert("update", "query update !", "OK"))->Go();
	
			msg->FindInt32("opcode", &opcode);
	
			if( opcode == B_ENTRY_CREATED) {

				char			*offset, *text_group, *name_group;
				attr_info		info_group;

				BFile			myFile;
				
				msg->FindInt32("device", &nref.device); 
				msg->FindInt64("node", &nref.node);
				
				msg->FindInt64("directory", &ref.directory); 
				msg->FindString("name", &name); 
				ref.set_name(name);
				ref.device = nref.device;

				myFile.SetTo(&ref, O_RDONLY);

				if ((myFile.InitCheck() == B_NO_ERROR) && (myFile.GetAttrInfo(P_GROUPNAME, &info_group) == B_NO_ERROR)) {
		
					if(info_group.size > 1) {
			
						text_group = (char *)malloc(info_group.size);
						myFile.ReadAttr(P_GROUPNAME, B_STRING_TYPE, 0, text_group, info_group.size);

						while (1) {
				
							//----------------------------------------------------------------
							// fetch group and verify we don't already have it.
				
							if ((offset = strstr(text_group, ",")))
								*offset = 0;
					
							group = FindGroup(text_group);
					
							if ( ! group ) {
						
								myGroupsRoot->IncrementCount();
						
								LockLooper();
									InvalidateItem(0);
									AddUnder( group = new GroupItem(text_group), myGroupsRoot);
								UnlockLooper();
						
								IncrementCount();
							}	


							if (offset) {
								text_group = offset + 1;
								while (*text_group == ' ')
									text_group++;
							} else {
				
								// just before break, attach the person after the group !
											
								AddPersonInGroup(&myFile, &ref, group, true);
								IncrementPeopleCount();
								LockLooper();
									InvalidateItem(IndexOf(group));
								UnlockLooper();
						
								break; // exit from the while
							}
						}
					} else {

						// no group, the person is orphan !			
			
						AddPersonInGroup(&myFile, &ref, myOrphansRoot, true);
						IncrementPeopleCount();
						LockLooper();
							InvalidateItem(IndexOf(myOrphansRoot));
						UnlockLooper();

					}
				}
			
			} else {
				// B_ENTRY_REMOVED
				int32			person;
				
				msg->FindInt32("device", &nref.device); 
				msg->FindInt64("node", &nref.node);
				
				person = FindPersonFromNode(&nref);
				
				if(person) {
					
					item = RemoveItem(person);		
					group = (GroupItem*) Superitem(item);
					group->DecrementCount();

					LockLooper();
						InvalidateItem(IndexOf(group));
					UnlockLooper();

					if( ! group->GetCount()) {
						RemoveItem(group);
						delete group;
						myGroupsRoot->DecrementCount();

						if( ! myGroupsRoot->GetCount()) {
							myGroupsRoot->SetEnabled(false);
							LockLooper();
								InvalidateItem(0);
							UnlockLooper();
						}
					}
					
					delete item;					
				}	
			
			}
			break;
	
		default:
			BOutlineListView::MessageReceived(msg);
	}	
}

//--------------------------------------------------------------------

void TPeopleTreeView::KillTreeBuilder(void)
{
	kill_thread(myQueryThread);
}

//---------------------------------------------------------------------

void TPeopleTreeView::UpdateCounts(void)
{
	myCount			= FullListCountItems();
	myPeopleCount	= myCount - 2 - myGroupsRoot->GetCount();
	// 2 is for groupRoot & OrphanRoot.	
}
