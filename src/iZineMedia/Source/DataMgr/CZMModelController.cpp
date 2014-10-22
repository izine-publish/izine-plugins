//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/DataMgr/CZMModelController.cpp $
//	$Revision: 3035 $
//	$Date: 2011-05-18 13:11:42 +0200 (Wed, 18 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "IWorkspace.h"

//SDK General includes
#include "FileUtils.h"
#include "Utils.h"

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMModelController.h"
#include "IZMAdFileMgr.h"
#include "IZMAdMgr.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"
#include "IZMFileUtils.h"
#include "IZMPrefs.h"
#include "IZMAdFileFacade.h"

//IZP General includes
#include "ZMTypes.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMModelController : public CPMUnknown<IZMModelController>
{
public:
						CZMModelController(IPMUnknown * boss);
	virtual				~CZMModelController();

	void				FetchAllConfigData();

	void				FetchTitleList();
	void				FetchEditions(
							const PMString &				inTitleID);

	void				FetchAdFilesForEdition(
							const PMString &				inEditionID );

	void				FetchAdsIfUpdated(
							const PMString &				inAdFileID );

	PMString			GetTitleParentFolder() const;
protected:
	void				HandleResponse_FetchTitleList(
							const ZMstTitleSList &			inTitleList);

	void				HandleResponse_FetchEditionList(
							const PMString &				inTitleID,
							const ZMstEditionSList &		inEditionList );

	void				HandleResponse_FetchAdFileList(
							const PMString &				inEditionID,
							const ZMPMStringSList &			inAdFileList );

	void				HandleResponse_FetchAdList(
							const PMString &				inAdFileID,
							const ZMstAdSList &				inAdList );
	

private:
};


CREATE_PMINTERFACE(CZMModelController, kZMModelControllerImpl)

CZMModelController::CZMModelController(IPMUnknown * boss) :
CPMUnknown<IZMModelController>(boss)
{
	LogFunctionEnterExit;
}

CZMModelController::~CZMModelController()
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// FetchAllConfigData
//----------------------------------------------------------------------------------------
// Sequence of request, Title
void
CZMModelController::FetchAllConfigData()
{
	LogFunctionEnterExit;

	this->FetchTitleList();
}

//----------------------------------------------------------------------------------------
// FetchTitleList
//----------------------------------------------------------------------------------------
void
CZMModelController::FetchTitleList()
{
	LogFunctionEnterExit;

	Utils<IZMFileUtils> fileUtils;
	ASSERT(fileUtils);

	ZMstTitleSList titleList;

	fileUtils->FetchListFromPath( this->GetTitleParentFolder(), titleList );

#ifdef DEBUG
	//fileUtils->PrintToLog(titleList);
#endif

	this->HandleResponse_FetchTitleList( titleList );

	//TODO: do we need this in media
	//Send the fetching message.
	///InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	///ASSERT(selfSubject);
	///selfSubject->Change( kZPLogin_ConfigDataFetchingMsg, IZPConfigController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// FetchEditions
//----------------------------------------------------------------------------------------
void
CZMModelController::FetchEditions(
	const PMString &				inTitleID)
{
	LogFunctionEnterExit;

	Utils<IZMFileUtils> fileUtils;
	ASSERT(fileUtils);

	ZMstEditionSList editionList;

	PMString titlePath( this->GetTitleParentFolder() );
	titlePath.Append( FileUtils::GetDirectorySeperator() );
	titlePath.Append( inTitleID );
	fileUtils->FetchEditionsFromPath( titlePath, editionList );

	this->HandleResponse_FetchEditionList( inTitleID, editionList );
}

//----------------------------------------------------------------------------------------
// FetchAdFilesForEdition
//----------------------------------------------------------------------------------------
void
CZMModelController::FetchAdFilesForEdition(
	const PMString &			inEditionID)
{
	LogFunctionEnterExit;

	Utils<IZMFileUtils> fileUtils;
	ASSERT(fileUtils);

	ZMPMStringSList adFileList;

	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	InterfacePtr<const IZMPrefs> workspacePrefs( iWorkspace, UseDefaultIID() );
	ASSERT( workspacePrefs );
	
	PMString editionPath( this->GetTitleParentFolder() );
	editionPath.Append( FileUtils::GetDirectorySeperator() );
	editionPath.Append( inEditionID );
	fileUtils->FetchAdsFileFromPath( editionPath, adFileList, workspacePrefs->GetAdFileNamePrefix() );

	this->HandleResponse_FetchAdFileList( inEditionID, adFileList );
}

//----------------------------------------------------------------------------------------
// FetchAdsIfUpdated
//----------------------------------------------------------------------------------------
void
CZMModelController::FetchAdsIfUpdated(
	const PMString &				inAdFileID)
{
	LogFunctionEnterExit;

	Utils<IZMAdFileFacade> adFileFacade;
	ASSERT(adFileFacade);

	ZMstAdSList adList;

	PMString adFilePath( this->GetTitleParentFolder() );
	adFilePath.Append( FileUtils::GetDirectorySeperator() );
	adFilePath.Append( inAdFileID );

	uint32 lastUpdateDate;	//TODO: use last updated date to optimize parsing.
	adFileFacade->ParseAdXMLFile( adFilePath, adList, &lastUpdateDate );

	this->HandleResponse_FetchAdList( inAdFileID, adList );
}

//----------------------------------------------------------------------------------------
// GetTitleParentFolder
//----------------------------------------------------------------------------------------
PMString
CZMModelController::GetTitleParentFolder()const
{
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	InterfacePtr<const IZMPrefs> workspacePrefs( iWorkspace, UseDefaultIID() );
	ASSERT( workspacePrefs );

	const PMString & repositoryLocation = workspacePrefs->GetRepositoryPath();

	//Add a hardcode string after repository path, where ad file will be present.
	PMString toReturn( repositoryLocation);
	toReturn.Append(FileUtils::GetDirectorySeperator());
	toReturn.Append("_Publish");
	
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleResponse_FetchTitleList
//----------------------------------------------------------------------------------------
void
CZMModelController::HandleResponse_FetchTitleList(
	const ZMstTitleSList &			inTitleList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	titleMgr->UpdateTitleList( inTitleList );

	//TODO: do we need this in media
	//Send the fetched message. One message per request
	///InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	///ASSERT(selfSubject);
	///selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}


//----------------------------------------------------------------------------------------
// HandleResponse_FetchEditionList
//----------------------------------------------------------------------------------------
void
CZMModelController::HandleResponse_FetchEditionList(
	const PMString &				inTitleID,
	const ZMstEditionSList &		inEditionList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	editionMgr->UpdateEditionList( inTitleID, inEditionList );

	//TODO: do we need this in media
	//Send the fetched message. One message per request
	///InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	///ASSERT(selfSubject);
	///selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchAdFileList
//----------------------------------------------------------------------------------------
void
CZMModelController::HandleResponse_FetchAdFileList(
	const PMString &				inEditionID,
	const ZMPMStringSList &			inAdFileList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	adFileMgr->UpdateAdFileList( inEditionID, inAdFileList );
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchAdList
//----------------------------------------------------------------------------------------
void
CZMModelController::HandleResponse_FetchAdList(
	const PMString &				inAdFileID,
	const ZMstAdSList &				inAdList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMAdMgr> adMgr( zpApp->GetAdMgr(), UseDefaultIID() );
	ASSERT(adMgr);

	adMgr->UpdateAdList( inAdFileID, inAdList );

}
