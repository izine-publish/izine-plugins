//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/AZPAMServiceProvider.h $
//	$Revision: 1976 $
//	$Date: 2010-08-17 12:39:58 +0200 (Tue, 17 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMServiceProvider_
#define _h_AZPAMServiceProvider_
#pragma once

class IStringData;
class IManageableAsset;

class IZPAssetData;

#define inheritClass_ZPAMServiceP CPMUnknown<IAMServiceProvider>
class AZPAMServiceProvider : public inheritClass_ZPAMServiceP
{
public:
	virtual				~AZPAMServiceProvider();

	virtual ErrorCode	Init();
						
	virtual bool16		CanService(
							const IDFile & file, const IAMServiceProvider::AssetHints * hints );
	//virtual bool16		CanService(
	//						const IManageableAsset * asset );
	virtual bool16		CanUpdateScriptedAssetData(
							UIDRef assetRef, const PMString & scriptString, int32 scriptID );
	virtual bool16		GetLockData(
							const IDFile & assetFile, IManagedStatus::StatusStringTable & outLockData, UIFlags uiFlags );
	//virtual IOpenFileCmdData::OpenFlags OverrideOpenFlags(
	//						const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags );
	
	virtual bool16		IsTemporaryVersion(
							const IDFile & file );
	virtual bool16		IsVersioned(
							const IDFile & file );
	virtual bool16		IsWriteable(
							const IDFile & storyFile );
	virtual ErrorCode	UpdateScriptedAssetData(
							UIDRef assetRef, const PMString & scriptString, int32 scriptID );

	virtual bool		IsSubPathOfOurReplicaFolder(
							const IDFile &				inFilePath );

protected:
						AZPAMServiceProvider(IPMUnknown * boss);
	virtual const char *	GetProviderName() const = 0;
	
	//Get cached DB asset ID from file path, will return nil if not already cached with asset mgr.
	virtual const IStringData *	GetDBAsset(
							const IDFile &				inAssetFile );
	//Get cached DB asset ID, will return nil if not already cached with asset mgr.
	//One case of nil can be when asset is deleted by other user.
	virtual const IStringData * GetDBAsset(
							const IManageableAsset *	inManageableAsset );
	virtual bool16		CanServiceDBAsset(
							const IStringData *			inDbAssetID ) const = 0;
	virtual const IStringData * GetDBAsset(
							const IZPAssetData *		inAssetData ) const;

	virtual void		LogManageableAssetInfo(
							const IManageableAsset *	inAsset ) const;
	virtual void		LogFilePath(
							const IDFile &				inFile ) const;
	virtual void		LogFileHints(
							const IAMServiceProvider::AssetHints * inHints ) const;
protected:
	static const SysOSType	kInDesignType;
	static const SysOSType	kInDesignTemplateType;
	static const SysOSType	kICFileType;		//New i.e inx format
	static const SysOSType	kICTemplateFileType;
	static const SysOSType	kAssignmentType;
	static const SysOSType	kInCopyBinType;		//Old i.e binary format

	static const PMString	kInDesignExt;
	static const PMString	kInDesignTemplateExt;
	static const PMString	kICFileExt;
	static const PMString	kICTemplateFileExt;
	static const PMString	kAssignmentExt;
	static const PMString	kInCopyBinExt;

private:
};

#endif //_h_AZPAMServiceProvider_
