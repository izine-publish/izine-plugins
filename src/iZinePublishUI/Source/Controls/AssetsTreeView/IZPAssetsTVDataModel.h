//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/IZPAssetsTVDataModel.h $
//	$Revision: 2144 $
//	$Date: 2010-09-22 13:27:38 +0200 (Wed, 22 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetsTVDataModel_
#define _h_IZPAssetsTVDataModel_
#pragma once

#include "ZPSmartPtrVector.h"
#include "ZPTypes.h"

class IStringData;
class ISubject;

typedef bool	(*FilterAssetFnPtr)(const IStringData * inAssetID );

class IZPAssetsTVDataModel : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETSTVDATAMODEL };

	virtual const PMString &		GetTitleID() const = 0;
	virtual const PMString &		GetGetEditionID() const = 0;
	//No setter for TitleID and EditionID. Use UpdateData instead.

	virtual const ZPStatusIDsList *	GetStatusIDList() const = 0;
	virtual void					SetStatusIDList(
										const ZPStatusIDsList *		inStatusIDList ) = 0;
	virtual const PMString *		GetNthStatusID(
										int							inIndex ) const = 0;
	virtual int32					GetStatusIDIndex(
										const PMString &			inStatusID ) const = 0;

	virtual const ZPAssetIDsList *	GetAssetIDList(
										const PMString &			inStatusID ) const = 0;
	virtual const PMString *		GetNthAssetID(
										const PMString &			inStatusID,
										int							inIndex ) const = 0;
	virtual int32					GetAssetIDIndex(
										const PMString &			inStatusID,
										const PMString &			inAssetID ) const = 0;
	virtual const PMString *		GetStatusIDOfAsset(
										const PMString &			inAssetID ) const = 0;
	//virtual void					SetAssetIDList(
	//									const PMString &			inStatusID,
	//									const ZPAssetIDsList &		inAssetIDList ) = 0;
	
	virtual bool					GetSkipAssetListUpdates() const = 0; //AssetList will not change if true.
	virtual void					SetSkipAssetListUpdates(
										bool						inSkipModelChanges ) = 0;
	virtual void					UpdateData(
										const PMString &			inTitleID,
										const PMString &			inEditionID ) = 0;

	virtual bool					UpdateStatusList() = 0;	//Returns false if list update is pending.
	virtual void					UpdateAssetList() = 0;

	virtual void					HandleAssetInfoChange(
										ISubject*					inAssetSubject ) = 0;

	virtual const FilterAssetFnPtr	GetAssetFilteringFunction() const = 0;
	virtual void					SetAssetFilteringFunction(
										FilterAssetFnPtr			inFunc ) = 0;

	static bool						CanDisplayAssetInInDesign(
										const IStringData *			inAssetID );
	static bool						CanDisplayAssetInInCopy(
										const IStringData *			inAssetID );
	static bool						CanDisplayAssetTypeIn_InDesign(
										const enAssetType 			inAssetID );
	static bool						CanDisplayAssetTypeIn_InCopy(
										const enAssetType 			inAssetID );
};

#endif //_h_IZPAssetsTVDataModel_
