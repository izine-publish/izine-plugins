//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPTitleStatusInfo.h $
//	$Revision: 3213 $
//	$Date: 2011-06-13 12:17:48 +0200 (Mon, 13 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTitleStatusInfo_
#define _h_IZPTitleStatusInfo_
#pragma once

#include "ZPTypes.h"

class IZPTitleStatusInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTITLESTATUSINFO };

	virtual enTitleStatusState	GetState() const = 0;
	virtual void				SetState(
									const enTitleStatusState	inState ) = 0;
	virtual void				SetState(
									const PMString &			inStateID ) = 0;

	virtual const bool			GetIsForLayout() const = 0;
	virtual void				SetIsForLayout(
									const bool					inIsForLayout ) = 0;

	virtual const bool			GetIsForText() const = 0;
	virtual void				SetIsForText(
									const bool					inIsForText ) = 0;

	virtual const int			GetSortIndex() const = 0;
	virtual void				SetSortIndex(
									const int					inSortIndex ) = 0;

	static enTitleStatusState	GetStateFromStateID(
									const PMString &			inStateID );
};


#endif //_h_IZPTitleStatusInfo_
