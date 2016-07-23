#include "GPItems.h"

//-------------------------------------------------------------------


AbstractItem::AbstractItem(const char * name, uint32 level, bool expanded)
	:BStringItem(name, level, expanded) { }

AbstractItem::~AbstractItem() { }

//--------------------------------------------------------------------

PersonItem::PersonItem(const char * name, entry_ref *ref, BString &descr, uint32 level = 0,bool expanded = true)
	: AbstractItem(name, level, expanded)
{

	BNode	myTempNode;

	myRef   = *ref;
	myTempNode.SetTo(&myRef);
	myTempNode.GetNodeRef(&myNRef);
	myDescr = new BString(descr); 
	
}

PersonItem::~PersonItem()
{
	delete myDescr;
}

//==================================================================

GroupItem::GroupItem(const char * name, uint32 level = 0, bool expanded = false, uint32 count = 0)
	: AbstractItem(name, level, expanded)
{
	char buff[256];
	
	myLabel = name;
	myCount = count;

}

GroupItem::~GroupItem()
{ }
