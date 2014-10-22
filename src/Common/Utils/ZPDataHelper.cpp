//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPDataHelper.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetMgr.h"
#include "IZPDefaultSession.h"
#include "IZPEditionMgr.h"
#include "IZPSessionData.h"
#include "IZPTaskMgr.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"
#include "IZPUserCredentials.h"
#include "IZPUserRole.h"

//IZP General includes
#include "ZPConstants.h"
#include "ZPDataHelper.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// IsValidGUID
//----------------------------------------------------------------------------------------
ZPEXPORTS bool
ZPDataHelper::IsValidGUID(
	const PMString &			inGUID)
{
	if( inGUID.empty() || inGUID.Compare( kFalse, kEmptyGUID ) == 0 )
		return false;
	return true;
}

//----------------------------------------------------------------------------------------
// GetTitleByID
//----------------------------------------------------------------------------------------
ZPEXPORTS const IStringData *
ZPDataHelper::GetTitleByID(
	const PMString &			inTitleID)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
//#ifdef DEBUG
	if( IsValidGUID( inTitleID ) == false )
		return nil;
//#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );
	//ASSERT(theTitle);
	return theTitle;
}

//----------------------------------------------------------------------------------------
// GetEditionByID
//----------------------------------------------------------------------------------------
ZPEXPORTS const IStringData *
ZPDataHelper::GetEditionByID(
	const PMString &			inEditionID)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
//#ifdef DEBUG
	if( IsValidGUID( inEditionID ) == false )
		return nil;
//#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const IStringData * theEdition = editionMgr->GetEditionForID( inEditionID );
	//ASSERT(theEdition);
	return theEdition;
}

ZPEXPORTS
//----------------------------------------------------------------------------------------
// GetTitleStatusByID
//----------------------------------------------------------------------------------------
const IStringData *
ZPDataHelper::GetTitleStatusByID(
	const PMString &			inStatusID)
{
	if( IsValidGUID( inStatusID ) == false )
		return nil;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleStatusMgr> titleStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleStatusMgr);

	const IStringData * theTitleStatus = titleStatusMgr->GetStatusForID( inStatusID );
	return theTitleStatus;
}

//----------------------------------------------------------------------------------------
// GetAssetByID
//----------------------------------------------------------------------------------------
ZPEXPORTS const IStringData *
ZPDataHelper::GetAssetByID(
	const PMString &			inAssetID)
{
#ifdef DEBUG
	if( IsValidGUID( inAssetID ) == false )
		return nil;
#endif
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	const IStringData * theAsset = assetMgr->GetAssetByID( inAssetID );
	return theAsset;
}

ZPEXPORTS
//----------------------------------------------------------------------------------------
// GetTaskByID
//----------------------------------------------------------------------------------------
const IStringData *
ZPDataHelper::GetTaskByID(
	const PMString &			inTaskID)
{
#ifdef DEBUG
	if( IsValidGUID( inTaskID ) == false )
		return nil;
#endif
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);

	const IStringData * dbTaskID = taskMgr->GetTaskByID( inTaskID );
	return dbTaskID;
}

//----------------------------------------------------------------------------------------
// GetCurrUserRole
//----------------------------------------------------------------------------------------
enZPUserRole
ZPDataHelper::GetCurrUserRole()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	InterfacePtr<const IZPUserRole> userRole( defaultSession->GetUserCredentials(), UseDefaultIID() );
	ASSERT( userRole );
	if( userRole )
		return userRole->GetRole();
	return eUserRole_None;
}

ZPEXPORTS
//----------------------------------------------------------------------------------------
// GetCurrUserID
//----------------------------------------------------------------------------------------
const PMString &
ZPDataHelper::GetCurrUserID()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	InterfacePtr<const IZPSessionData> userSession( defaultSession->GetUserCredentials(), UseDefaultIID() );
	ASSERT( userSession );
	if( userSession )
		return userSession->GetUserID();

	return kNullString;
}

ZPEXPORTS
//----------------------------------------------------------------------------------------
// IsLoggedIn
//----------------------------------------------------------------------------------------
const bool
ZPDataHelper::IsLoggedIn()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );
	const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();
	InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
	
	bool isLoggedIn = ( defaultCredentials && isValidData && isValidData->Get());
	return isLoggedIn;
}
