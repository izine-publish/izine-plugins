//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/ZMDataHelper.cpp $
//	$Revision: 2774 $
//	$Date: 2011-04-11 12:05:27 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdFileMgr.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"

//IZP General includes
#include "ZMDataHelper.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// IsValidGUID
//----------------------------------------------------------------------------------------
ZMEXPORTS bool
ZMDataHelper::IsValidGUID(
	const PMString &			inGUID)
{
	if( inGUID.empty() )	//Check for space only string
		return false;
	return true;
}

//----------------------------------------------------------------------------------------
// GetTitleByID
//----------------------------------------------------------------------------------------
ZMEXPORTS const IStringData *
ZMDataHelper::GetTitleByID(
	const PMString &			inTitleID)
{
//#ifdef DEBUG
	if( IsValidGUID( inTitleID ) == false )
		return nil;
//#endif
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );
	//ASSERT(theTitle);
	return theTitle;
}

//----------------------------------------------------------------------------------------
// GetEditionByID
//----------------------------------------------------------------------------------------
ZMEXPORTS const IStringData *
ZMDataHelper::GetEditionByID(
	const PMString &			inEditionID)
{
//#ifdef DEBUG
	if( IsValidGUID( inEditionID ) == false )
		return nil;
//#endif
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const IStringData * theEdition = editionMgr->GetEditionForID( inEditionID );
	//ASSERT(theEdition);
	return theEdition;
}

//----------------------------------------------------------------------------------------
// GetAdFileByID
//----------------------------------------------------------------------------------------
ZMEXPORTS const IStringData *
ZMDataHelper::GetAdFileByID(
	const PMString &			inAdFileID)
{
#ifdef DEBUG
	if( IsValidGUID( inAdFileID ) == false )
		return nil;
#endif
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	const IStringData * theAdFileID = adFileMgr->GetAdFileByID( inAdFileID );
	return theAdFileID;
}

