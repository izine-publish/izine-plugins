//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/IZMAdsTVDataModel.h $
//	$Revision: 2913 $
//	$Date: 2011-04-29 16:48:36 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdsTVDataModel_
#define _h_IZMAdsTVDataModel_
#pragma once

#include "ZPSmartPtrVector.h"
#include "ZMTypes.h"

class IStringData;
class ISubject;

//typedef bool	(*FilterAdFnPtr)(const IStringData * inAdID );

class IZMAdsTVDataModel : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADSTVDATAMODEL };

	//virtual const PMString &		GetTitleID() const = 0;
	virtual const PMString &		GetEditionID() const = 0;
	//No setter for TitleID and EditionID. Use UpdateData instead.

	virtual const ZMAdFileIDsList *	GetAdFileIDList() const = 0;
//	virtual void					SetAdFileIDList(
//										const ZMAdFileIDsList *		inAdFileIDList ) = 0;
	virtual const PMString *		GetNthAdFileID(
										int							inIndex ) const = 0;
	virtual int32					GetAdFileIDIndex(
										const PMString &			inAdFileID ) const = 0;

	virtual const ZMAdIDsList *		GetAdIDList(
										const PMString &			inAdFileID ) const = 0;
	virtual const PMString *		GetNthAdID(
										const PMString &			inAdFileID,
										int							inIndex ) const = 0;
	virtual int32					GetAdIDIndex(
										const PMString &			inAdFileID,
										const PMString &			inAdID ) const = 0;
	virtual const PMString *		GetAdFileIDOfAd(
										const PMString &			inAdID ) const = 0;
	//virtual void					SetAdIDList(
	//									const PMString &			inAdFileID,
	//									const ZMAdIDsList &			inAdIDList ) = 0;
	
	virtual bool					GetSkipAdFileListUpdates() const = 0; //AdList will not change if true.
	virtual void					SetSkipAdFileListUpdates(
										bool						inSkipModelChanges ) = 0;
	virtual void					UpdateData(
										const PMString &			inTitleID,
										const PMString &			inEditionID ) = 0;

	virtual bool					UpdateAdFileList() = 0;	//Returns false if list update is pending.
	//virtual void					UpdateAdList() = 0;

	virtual void					HandleAdInfoChange(
										ISubject*					inAdSubject ) = 0;

	virtual void					MarkAllAdFileDirty() = 0;
//	virtual const FilterAdFnPtr		GetAdFilteringFunction() const = 0;
//	virtual void					SetAdFilteringFunction(
//										FilterAdFnPtr				inFunc ) = 0;
};

#endif //_h_IZMAdsTVDataModel_
