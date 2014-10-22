//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/AZPAMService.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMService_
#define _h_AZPAMService_
#pragma once

#include "IAMService.h"

class IAssignedStory;
class IAssignment;
class IAssetMonitor;
class IDocument;
class IStringData;

class IZPAssetInfo;
class IZPAssetLocalInfo;
class IZPAssetCheckInData;


#define inheritClass_ZPAMService CPMUnknown<IAMService>
class AZPAMService : public inheritClass_ZPAMService
{
public:
	virtual				~AZPAMService();

protected:
						AZPAMService(IPMUnknown * boss);

	virtual const IStringData *	GetDBAsset(
							const IManageableAsset *	inAsset ) const;	//Not used for assignment
	virtual const IStringData *	GetDBAsset_FromDoc(
							const IManageableAsset *	inAsset ) const;
	virtual const IStringData *	GetDBAsset_FromAsgmt(
							const IManageableAsset *	inAsset ) const;
	virtual const IStringData *	GetDBAsset_FromAsgmt(
							const IAssignment *			inAsset ) const;
	virtual const IStringData * GetDBAsset_FromIDStory(
							const IManageableAsset *	inAsset ) const;

	virtual const IZPAssetInfo *	GetDBAssetInfo_FromDoc(
							const IManageableAsset *	inAsset ) const;

	virtual const IDocument *	GetDocumentForFile(
							const IDFile &				inDocAssetFile ) const;
	virtual IAssignment *	GetAssignmentForFile(		//Depends on front document, will not work on ID server.
							const IDFile &				inAssignmentAssetFile) const;
	virtual IManageableAsset *	GetAssignmentAssetForFile(	//Depends on front document, will not work on ID server.
							const IDFile &				inAssignmentAssetFile) const;	//Function not being used.
	virtual IAssignment *	GetAssignment(
							const IManageableAsset *	inManageableAsset ) const;

	virtual IAssignedStory *	GetAssignedStory(
							const IManageableAsset *	inManageableAsset ) const;
#if defined(InDnCS5) || defined(InDnCS5_5)
	virtual UIDRef			GetServiceDataModelObject() const;
#endif

	virtual bool		GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const = 0;

	virtual bool		GetIDStoryFile(					//First check service data then asset.
							const IManageableAsset *	inManageableAsset,
							IDFile &					oIDStoryAssetFile) const;

	virtual bool		IsAssetLockedByCurrentUser(
							const IStringData *			inAssetID ) const;
	virtual bool		IsAssetLocked(					//True if locked by anyone.
							const IStringData *			inAssetID ) const;

	virtual bool		IsLinkedStoryAsset(
							const IManageableAsset *	inManageableAsset ) const;
	virtual bool		IsInCopyDocument(
							const IManageableAsset *	inManageableAsset ) const;
	virtual bool		IsInCopyTemplate(
							const IManageableAsset *	inManageableAsset ) const;

#pragma mark -
	virtual const IZPAssetLocalInfo *	QueryAssetLocalInfo(	//it will create one if not found
							const IStringData *			inAssetID ) const;
	virtual void		UpdateLocalInfo_LocallyModified(
							const IStringData *			inAssetID,
							bool						inIsModified ) const;
	virtual void		UpdateLocalInfo_VersionNumber(
							const IStringData *			inAssetID,
							int32						inVersionNumber ) const;

	virtual ErrorCode	EnsureLatestVersion(
							const IManageableAsset *	inAsset ) const;
	virtual ErrorCode	EnsureLatestVersion(
							const IManageableAsset *	inAsset,
							const IDFile &				inAssetFile ) const;

	virtual void		DoAfterCheckIn_UpdateLocalInfo(					//Update local asset info after check-in.
							const IStringData *			inDBAssetID );
	virtual void		DoAfterCheckIn(					//Update asset info on server if modified.
							const IManageableAsset *	inAsset,
							const IStringData *			inDBAssetID );
	virtual void		DoAfterCheckIn(					//Update asset info on server if modified & Clear check-in info.
							IZPAssetCheckInData *		inAssetCheckInData,
							const IStringData *			inDBAssetID );

	virtual bool		CopyParentCheckInData(			//Copy assignment/doc status to ID story
							IManageableAsset *			inAsset ) const;
	virtual bool		CopyParentCheckInData(			//Copy doc comments to assignment, assignment to ID story
							IAssignment *				inAsset ) const;
	virtual bool		FillCheckInDataFromSelf(		//Copy self status to check-in Data, used from InDesign
							IAssignment *				inAsset ) const;
	virtual bool		FillCheckInDataFromSelf(		//Copy self status to check-in Data, used for story assets
							IManageableAsset *			inAsset ) const;
	virtual bool		FillCheckInDataFromDbAsset(
							IZPAssetCheckInData *		ioCheckInData,
							const IStringData *			inDBAssetID,
							bool						inCopyAssetName = false) const;

	virtual	void		CallEnsureLatestVersion(
							IManageableAsset *			pAsset) const;

	virtual IAssetMonitor *	QueryAssetMonitor(
							ClassID						inServiceProvider ) const;
#pragma mark -
	//Functions used to modify states in DB/WS
	virtual void		DoReleaseLock(
							const IStringData *			inAssetID );
	virtual ErrorCode	DoAcquireLock(
							const IStringData *			inAssetID );

#pragma mark -
	virtual void		LogManageableAssetInfo(
							const IManageableAsset *	inAsset ) const;

	virtual void		LogFilePath(
							const IDFile &				inFile ) const;
private:
};

#endif //_h_AZPAMService_
