//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/IZMUITVNodeData.h $
//	$Revision: 2702 $
//	$Date: 2011-04-06 10:59:01 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMUITVNodeData_
#define _h_IZMUITVNodeData_
#pragma once


class IZMUITVNodeData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMUITVNODEDATA };

	virtual const IPMUnknown *
						GetDataObj() const = 0;
							
	virtual void		SetDataObj(
							const IPMUnknown *			inDataObj ) = 0;

	
};

#endif //_h_IZMUITVNodeData_
