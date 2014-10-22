//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMAdFileList.cpp $
//	$Revision: 2711 $
//	$Date: 2011-04-06 15:20:51 +0200 (Wed, 06 Apr 2011) $
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
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMModelController.h"
#include "IZMAdFileList.h"
#include "IZMAdFileMgr.h"

//IZP General includes
#include "CZMStringListData.h"
//#include "ZMConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMStringListData_<IZMAdFileList>
class CZMAdFileList : public inheritClass
{
public:
						CZMAdFileList(IPMUnknown * boss);
	virtual				~CZMAdFileList();

	void				SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	void				RequestUpdateFromServer();

	bool				ContainsAdFileID(
							const PMString &			inAdFileID ) const;

protected:
	bool	mHasData;
private:
};


CREATE_PMINTERFACE(CZMAdFileList, kZMAdFileListImpl)

CZMAdFileList::CZMAdFileList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdFileList::~CZMAdFileList()
{
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement AdFile references.
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );
	if (!zpApp)
		return;

	InterfacePtr<IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);
	
	adFileMgr->DecrementReferenceForAdFiles( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZMAdFileList::SetStringList(
	const K2Vector<PMString>& newList,
	const bool					inHasCompleteList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZMAdFileListWillChangeMsg, IZMAdFileList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	if( inHasCompleteList )
		this->SetHasData( true );

	selfSubject->Change( kZMAdFileListChangedMsg, IZMAdFileList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZMAdFileList::GetStringList(
	const bool					inFetchIfNotHave)
{
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
		this->RequestUpdateFromServer();
	}
	return inheritClass::GetStringList();
}

//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZMAdFileList::RequestUpdateFromServer()
{
	InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
	ASSERT( modelController );
	
	InterfacePtr<const IStringData> selfID( this, IID_IZMDATAID );
	ASSERT(selfID);

	const PMString & theEditionID = selfID->Get();
	ASSERT( theEditionID.empty() == false );
	//ASSERT( theEditionID.Compare( kFalse, kEmptyGUID ) != 0 );

	modelController->FetchAdFilesForEdition( theEditionID );
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZMAdFileList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZMAdFileList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsAdFileID
//----------------------------------------------------------------------------------------
bool
CZMAdFileList::ContainsAdFileID(
	const PMString &			inAdFileID) const
{
	if( inAdFileID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inAdFileID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}
