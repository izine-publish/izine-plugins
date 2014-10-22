//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPAssetUtils.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetUtils_
#define _h_IZPAssetUtils_
#pragma once

#include "ZPTypes.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "URI.h"
#endif
class IZPAssetData;

class IZPAssetUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETUTILS };

	virtual bool		IsTemplateTypeID(
							const PMString &			inTypeID ) const = 0;
	virtual bool		IsTemplateType(
							const enAssetType			inType ) const = 0;

	virtual enAssetType GetAssetTypeFromID(
							const PMString &			inTypeID ) const = 0;
	virtual const PMString &	
						GetAssetTypeID(
							const enAssetType			inType ) const = 0;

	virtual const PMString &
						GetHostAppDocType(				//Not being used.
							bool						inForTemplate = false ) const = 0;
	virtual enAssetType	GetHostAppDocAssetType(
							bool						inForTemplate = false ) const = 0;
	virtual const FileTypeInfoID
						GetHostAppDocFileTypeInfo(
							bool						inForTemplate ) const = 0;

	virtual const PMString &
						GetAssetFileExtenstion(
							const enAssetType			inType ) const = 0;
	virtual const PMString &
						GetAssetFileExtenstion(
							const PMString &			inTypeID ) const = 0;

	virtual IDFile		CreateLocalFilePathFromAssetID(
							const PMString &			inAssetID,
							const PMString &			inFileExtension,
							const PMString &			inName) const = 0;

	virtual IDFile		CreateAssetCacheFolderPath(
							const PMString &			inAssetID,
							bool						createIfNotExists = true) const = 0;

	//Note: Avoid using this function, usage should be minium. Get assetID from manageable asset.
	virtual PMString	GetAssetIDFromLocalFile(
							const IDFile&				inFile ) const = 0;
	virtual PMString	GetAssetIDFromLocalFile(
							const PMString &			inFilePath ) const = 0;

	virtual	PMString	NewGUID() const = 0;

	virtual bool		CanOpenAsset(					//Check if we can open inAssetID file or it's master (where this asset is linked) file.
							const PMString &			inAssetID ) const = 0;
	virtual bool		CanOpenAssetType(				//Check if we can open inAssetType asset type.
							enAssetType					inAssetType ) const = 0;

	virtual bool		CopyAssetDataFromAsset(			//Copy IZPAssetData values from cached asset object.
							const IStringData *			inAssetID,
							IZPAssetData *				ioCopyDataTo ) const = 0;

	virtual PMString	GetStatusIDOfAsset(
							const PMString &			inAssetID ) const = 0;
	virtual PMString	GetStatusIDOfAsset(
							const IStringData *			inDBAssetID ) const = 0;
#if defined(InDnCS5) || defined(InDnCS5_5)	
	virtual bool		FileToURI(
							const IDFile&				inFilePath,
							URI&						oURI ) const = 0;
	virtual bool		URIToIDFile(
							const URI&					inURI,
							IDFile&						oFilePath  ) const = 0;
#endif
};


#endif //_h_IZPAssetUtils_
