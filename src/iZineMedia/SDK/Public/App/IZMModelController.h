//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/App/IZMModelController.h $
//	$Revision: 2882 $
//	$Date: 2011-04-28 13:57:35 +0200 (Thu, 28 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMModelController_
#define _h_IZMModelController_
#pragma once


class IZMModelController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMMODELCONTROLLER };

	virtual	void		FetchTitleList() = 0;
	virtual	void		FetchEditions(						//Empty title id will fetch all editions for user.
							const PMString &				inTitleID) = 0;

	virtual void		FetchAdFilesForEdition(
							const PMString &				inEditionID ) = 0;

	virtual void		FetchAdsIfUpdated(
							const PMString &				inAdFileID ) = 0;

	virtual PMString	GetTitleParentFolder() const = 0;
};

#endif //_h_IZMModelController_
