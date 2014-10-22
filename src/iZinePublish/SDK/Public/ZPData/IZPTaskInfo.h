//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPTaskInfo.h $
//	$Revision: 2400 $
//	$Date: 2010-10-26 14:35:10 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTaskInfo_
#define _h_IZPTaskInfo_
#pragma once

#include "ZPWSTypeDefs.h"

class IZPTaskInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKINFO };
	
	virtual const ZPTaskInfo &	GetTaskInfo() const = 0;
	virtual void				SetTaskInfo(
									const ZPTaskInfo &			inTaskInfo ) = 0;

	virtual const PMString &	GetDescription() const = 0;
	virtual void				SetDescription(
									const PMString &			inDescription ) = 0;

	virtual const PMString &	GetDocumentID() const = 0;
	virtual void				SetDocumentID(
									const PMString &			inDocumentID ) = 0;

	virtual const PMString &	GetAssignedToUserID() const = 0;
	virtual void				SetAssignedToUserID(
									const PMString &			inAssignedToUserID ) = 0;

	virtual const PMString &	GetStatusID() const = 0;
	virtual void				SetStatusID(
									const PMString &			inStatusID ) = 0;

	virtual const PMString &	GetCategoryID() const = 0;
	virtual void				SetCategoryID(
									const PMString &			inCategoryID ) = 0;

	virtual const PMString &	GetSpread() const = 0;
	virtual void				SetSpread(
									const PMString &			inSpread ) = 0;

	virtual const PMString &	GetPage() const = 0;
	virtual void				SetPage(
									const PMString &			inPage ) = 0;

	virtual	int					GetCurrentVersion() const = 0;

	virtual void				Copy(
									const IZPTaskInfo *			inSrc ) = 0;
};


#endif //_h_IZPTaskInfo_
