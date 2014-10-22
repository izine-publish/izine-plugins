//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/IZPUITVNodeData.h $
//	$Revision: 1221 $
//	$Date: 2010-05-25 11:44:40 +0200 (Tue, 25 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUITVNodeData_
#define _h_IZPUITVNodeData_
#pragma once


class IZPUITVNodeData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUITVNODEDATA };

	virtual const IPMUnknown *
						GetDataObj() const = 0;
							
	virtual void		SetDataObj(
							const IPMUnknown *			inDataObj ) = 0;

	
};


#endif //_h_IZPUITVNodeData_
