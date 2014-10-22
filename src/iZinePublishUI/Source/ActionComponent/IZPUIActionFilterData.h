//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/IZPUIActionFilterData.h $
//	$Revision: 1262 $
//	$Date: 2010-05-31 13:29:23 +0200 (Mon, 31 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUIActionFilterData_
#define _h_IZPUIActionFilterData_
#pragma once


class IZPUIActionFilterData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUIACTIONFILTERDATA };

	virtual void		ClearList() = 0;
	virtual ClassID		GetOldHandlerForAction(
							ActionID					inActionID ) const = 0;
	virtual void		SetOldHandlerForAction(
							ActionID					inActionID,
							ClassID						inHandler ) = 0;
};


#endif //_h_IZPUIActionFilterData_
