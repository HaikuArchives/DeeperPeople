//--------------------------------------------------------------------
//	
//	TextControl.h
//
//	Written by: Robert Polic
//	
//--------------------------------------------------------------------
/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef TEXTCONTROL_H
#define TEXTCONTROL_H

#include "PeopleApp.h"
#include <TextControl.h>

class TTextControl : public BTextControl {

private:

	char			*fLabel;
	char			*fOriginal;
	int32			fLength;

public:

					TTextControl(BRect, char*, int32, char*, int32, int32, uint32 flags = B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
					~TTextControl(void);
	virtual void	AttachedToWindow(void);
	bool			Changed(void);
	void			Revert(void);
	void			Update(void);
	void			MouseDown(BPoint);
};

#endif /* TEXTCONTROL_H */