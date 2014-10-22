//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/DataMgr/CZMAdFileMgr.cpp $
//	$Revision: 2735 $
//	$Date: 2011-04-07 12:08:13 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "Utils.h"
#include "FileUtils.h"

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdFileMgr.h"
#include "IZMEditionMgr.h"
#include "IZMAdFileList.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMAdFileMgr : public CPMUnknown<IZMAdFileMgr>
{
public:
				CZMAdFileMgr(IPMUnknown * boss);
	virtual		~CZMAdFileMgr();

	void				Shutdown();

	void				UpdateAdFileList(
							const IStringData *			inEditionID,
							const ZMPMStringSList &		inAdFileList );
	void				UpdateAdFileList(
							const PMString &			inEditionID,
							const ZMPMStringSList &		inAdFileList );
	
	void				AddAdFile(
							const IStringData *			inEditionID,
							const PMString &			inAdFileID,
							const PMString &			inAdFileName );

	const IStringData *	GetAdFileByID(
							const PMString &			inAdFileID ) const;
	
	void				DecrementReferenceForAdFiles(
							const ZMAdFileIDsList &		inAdFileIDs );

	//void				DeleteAdFile(
	//						const PMString &			inAdFileID ) const;

protected:
	static IStringData * CreateAdFile(
							const PMString &			inAdFileID );

	bool				AddUpdateAdFile(
							const PMString &			inAdFileID,
							const PMString &			inAdFileName,
							const IStringData **		oAdFileID );

	void				RemoveFromCache(
							const IStringData *			inAdFileID );

	void				BroadcastMessage(
							const ClassID &				inTheChange,
							void*						inChangedBy = NULL );

private:
	ZMAdFileIDMap		mAdFileMap;
};


CREATE_PMINTERFACE(CZMAdFileMgr, kZMAdFileMgrImpl)

//----------------------------------------------------------------------------------------
// MakeAdFileID
//----------------------------------------------------------------------------------------
PMString
IZMAdFileMgr::MakeAdFileID(
	const PMString &				inEditionID,
	const PMString &				inAdFileName)
{
	PMString adFileID = inEditionID;
	adFileID.Append( FileUtils::GetDirectorySeperator());
	adFileID.Append( inAdFileName);
	return adFileID;
}



CZMAdFileMgr::CZMAdFileMgr(IPMUnknown * boss) :
CPMUnknown<IZMAdFileMgr>(boss)
{

}

CZMAdFileMgr::~CZMAdFileMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateAdFile
//----------------------------------------------------------------------------------------
IStringData *
CZMAdFileMgr::CreateAdFile(
	const PMString &			inAdFileID)
{
	IStringData * theAdFileID = ::CreateObject2<IStringData>( kZMAdFileContentBoss , IID_IZMDATAID );
	ASSERT(theAdFileID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theAdFileID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "AdFile boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inAdFileID.GrabCString() );
#endif
	theAdFileID->Set( inAdFileID );

	return theAdFileID;
}

//----------------------------------------------------------------------------------------
// DeleteAdFile
//----------------------------------------------------------------------------------------
//void
//CZMAdFileMgr::DeleteAdFile(
//	const PMString &			inAdFileID) const
//{
//	LogFunctionEnterExit;
//	//Not required.
//}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateAdFileList
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::UpdateAdFileList(
	const PMString &			inEditionID,
	const ZMPMStringSList &		inAdFileList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const IStringData * theEditionID = editionMgr->GetEditionForID( inEditionID );

	this->UpdateAdFileList( theEditionID, inAdFileList );
}

//----------------------------------------------------------------------------------------
// UpdateAdFileList
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::UpdateAdFileList(
	const IStringData *			inEditionID,
	const ZMPMStringSList &		inAdFileList)
{
	ZMAdFileIDsList adFileIDsToRemove;
	ZMAdFileIDsList adFileIDsNewList;

	InterfacePtr<IZMAdFileList> editionAdFileList( inEditionID, UseDefaultIID() );

	ASSERT( editionAdFileList != nil );	//Updating an edition that is not cached.
	if ( editionAdFileList == nil )
		return;

	adFileIDsToRemove = editionAdFileList->GetStringList( false );
	
	InterfacePtr<ISubject> editionSubject( editionAdFileList, UseDefaultIID() );
	ASSERT(editionSubject);
	//Send list will change message so that UI does not update with intermediate data.
	editionSubject->Change( kZMAdFileListWillChangeMsg, IZMAdFileList::kDefaultIID, nil );

	//Add/Update the AdFile for edition
	ZMPMStringSListCIter	currIter = inAdFileList.begin();
	ZMPMStringSListCIter	endIter = inAdFileList.end();
	
	const PMString & theEditionIDStr = inEditionID->Get();

	while( currIter != endIter )
	{
		const PMString adFileID = IZMAdFileMgr::MakeAdFileID( theEditionIDStr, **currIter );
		
		//Update the AdFile
		const IStringData * theAdFileID = NULL;
		//(*currIter)->mName.SetTranslatable( kFalse );
		bool newAdFileCreated = this->AddUpdateAdFile( adFileID, (**currIter), &theAdFileID );
		
		//Add to new list.
		adFileIDsNewList.push_back(adFileID);
		
		++currIter;
		
		if( newAdFileCreated == false )
			theAdFileID->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theAdFileID, IID_IZMCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the AdFile id list
	editionAdFileList->SetStringList( adFileIDsNewList );

	//Remove from cached list. Should we? what if same Edition is shared by multiple titles.
	this->DecrementReferenceForAdFiles( adFileIDsToRemove );
}

//----------------------------------------------------------------------------------------
// AddAdFile
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::AddAdFile(
	const IStringData *			inEditionID,
	const PMString &			inAdFileID,
	const PMString &			inAdFileName )
{
	InterfacePtr<IZMAdFileList> editionAdFileList( inEditionID, UseDefaultIID() );

	ASSERT( editionAdFileList != nil );	//Updating an edition that is not cached.
	if ( editionAdFileList == nil )
		return;

	ZMAdFileIDsList adFileIDsNewList = editionAdFileList->GetStringList( false );
	//Update the AdFile
	const IStringData * theAdFileID = NULL;
	//(*currIter)->mName.SetTranslatable( kFalse );
	
	bool newAdFileCreated = this->AddUpdateAdFile( inAdFileID, inAdFileName, &theAdFileID );
	
	if( !theAdFileID )
		return;

	//Add to new list.
	adFileIDsNewList.push_back( inAdFileID );

	//Increament ref count maintained by this manager.
	InterfacePtr<IIntData> cachedCount( theAdFileID, IID_IZMCACHEMGRCOUNT );
	ASSERT( cachedCount );
	cachedCount->Set( cachedCount->Get() + 1 );

	//Update the AdFile id list
	editionAdFileList->SetStringList( adFileIDsNewList, false );
}

//----------------------------------------------------------------------------------------
// AddUpdateAdFile
//----------------------------------------------------------------------------------------
bool
CZMAdFileMgr::AddUpdateAdFile(
	const PMString &			inAdFileID,
	const PMString &			inAdFileName,
	const IStringData **		oAdFileID)
{
	const IStringData * adFileID = this->GetAdFileByID( inAdFileID );
	bool toReturn = adFileID == nil;
	if( toReturn )
	{
		adFileID = mAdFileMap[inAdFileID] = CreateAdFile( inAdFileID );

		//AdFile name does not changes, so setting this only once when it is created.
		InterfacePtr<IStringData> adFileName( adFileID, IID_IZMDATANAME );
		ASSERT( adFileName );
		adFileName->Set( inAdFileName );
	}

	//Nothing to update;

	if( oAdFileID )
		*oAdFileID = adFileID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::RemoveFromCache(
	const IStringData *				inAdFileID)
{
	if( !inAdFileID )
		return;

	const PMString & theAdFileIDStr = inAdFileID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inAdFileID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "AdFile boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theAdFileIDStr.GrabCString() );
#endif

	mAdFileMap.erase( inAdFileID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForAdFiles
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::DecrementReferenceForAdFiles(
	const ZMAdFileIDsList &		inAdFileIDs)
{
	ZMAdFileIDsListCIter idListCurrIter = inAdFileIDs.begin();
	ZMAdFileIDsListCIter idListEndIter = inAdFileIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theAdFileID = this->GetAdFileByID( *idListCurrIter );

		//Update reference count maintained by this manager
		InterfacePtr<IIntData> cachedCount( theAdFileID, IID_IZMCACHEMGRCOUNT );
		ASSERT( cachedCount );
		if( cachedCount->Get() == 1 )
			this->RemoveFromCache( theAdFileID );
		else
			cachedCount->Set( cachedCount->Get() -1 );

		if( theAdFileID )
			theAdFileID->Release();
		++idListCurrIter;
	}
}

//----------------------------------------------------------------------------------------
// GetAdFileByID
//----------------------------------------------------------------------------------------
const IStringData *
CZMAdFileMgr::GetAdFileByID(
	const PMString &			inAdFileID) const

{
	ZMAdFileIDMapCIter foundIter = mAdFileMap.find( inAdFileID );
	ZMAdFileIDMapCIter endIter = mAdFileMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::BroadcastMessage(
	const ClassID &				inTheChange,
	void*						inChangedBy)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZMAdFileMgr::kDefaultIID, inChangedBy );
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZMAdFileMgr::Shutdown()
{
	//TODO: 
}

#pragma mark -
