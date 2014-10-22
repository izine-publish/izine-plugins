//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/EditionsPopup/CZMEditionPopupController.cpp $
//	$Revision: 2723 $
//	$Date: 2011-04-07 08:55:51 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMEditionList.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"


//IZP General includes
#include "AZMDataObjPopupController.h"
#include "AZMUIDataObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZMDataObjPopupController
class CZMEditionPopupController : public inheritClass
{
public:
						CZMEditionPopupController(IPMUnknown * boss);
	virtual				~CZMEditionPopupController();
	void				FillValues();

	PMString			GetNthObjID(
							int							inIndex) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;
protected:
	void				RemoveListeningTitle();
	void				AddListeningToTitle();

	void				FillValues(
							const ZMEditionIDsList &	inIDList );
private:
	InterfacePtr<const IStringData> mTitle;	//Title for which editions are displayed by this popup.
										//retained here so that it does not get destroyed before its references are removed.
};


CREATE_PMINTERFACE(CZMEditionPopupController, kZMEditionPopupControllerImpl)

CZMEditionPopupController::CZMEditionPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMEditionPopupController::~CZMEditionPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZMEditionPopupController::FillValues()
{
	LogFunctionEnterExit;

	InterfacePtr<const IStringData> editionTitleID( this, IID_IZMTITLEIDSTRINGDATA );
	ASSERT(editionTitleID);
	const PMString & theTitleID = editionTitleID->Get();

	ZMEditionIDsList theEditionIDList;

	//check if this ID is empty or NULL ID
	if( theTitleID.empty() )
	{
		this->RemoveListeningTitle();
		mTitle.reset(nil);
		//TODO: Remove all entries from this popup.
	}
	else
	{
		InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);

		const IStringData * theTitle = titleMgr->GetTitleForID( theTitleID );
		
		this->RemoveListeningTitle();
		if( theTitle )		//Can be nil, if previous selected title is not in current user title list
			theTitle->AddRef();
		mTitle.reset(theTitle);
		this->AddListeningToTitle();

		InterfacePtr<IZMEditionList> titleEditionList( theTitle, UseDefaultIID() );
	
		if ( titleEditionList != nil )
		{
			theEditionIDList = titleEditionList->GetStringList();
			//If edition id list comes empty, then it's observer will be notified when the list is available.
		}
	}
	this->FillValues( theEditionIDList );
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZMEditionPopupController::FillValues(
	const ZMEditionIDsList &	inIDList)
{
	InterfacePtr<IStringListControlData> iEditionStringListControlData( this, UseDefaultIID());
	ASSERT(iEditionStringListControlData);
	
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	int newListLen = inIDList.size();
	if( newListLen <= 0 )
		iEditionStringListControlData->Clear();
	else
		iEditionStringListControlData->Clear(kFalse, kFalse);

	ZMEditionIDsListCIter endIter = inIDList.end();

	int i=1;
	for( ZMEditionIDsListCIter currIter = inIDList.begin(); currIter != endIter; ++currIter,++i)
	{
		const PMString & editionID = *currIter;
		
		InterfacePtr<const IStringData> editionName( editionMgr->GetEditionForID(editionID), IID_IZMDATANAME );
		ASSERT( editionName );
		const PMString & theEditionName = editionName->Get();

		if ( i == newListLen )
			iEditionStringListControlData->AddString(theEditionName);
		else
			iEditionStringListControlData->AddString(theEditionName, IStringListControlData::kEnd, kFalse, kFalse );
	}

	this->SendMessage_ListLengthChanged();
}

//----------------------------------------------------------------------------------------
// GetNthObjID
//----------------------------------------------------------------------------------------
PMString
CZMEditionPopupController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	InterfacePtr<IZMEditionList> titleEditionList( mTitle, UseDefaultIID() );
	ASSERT( titleEditionList );

	const K2Vector<PMString>& theEditionsList = titleEditionList->GetStringList( false );

	ASSERT( theEditionsList.size() > inIndex );

	toReturn = theEditionsList[ inIndex ];

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
CZMEditionPopupController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	if(!mTitle )
		return toReturn;

	InterfacePtr<IZMEditionList> titleEditionList( mTitle, UseDefaultIID() );
	ASSERT( titleEditionList );

	const K2Vector<PMString>& theEditionsList = titleEditionList->GetStringList( false );

	if( theEditionsList.size() )
		toReturn = theEditionsList.Location( inObjID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
void
CZMEditionPopupController::RemoveListeningTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZMUIDataObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->RemoveListeningObject( mTitle.get(), IZMEditionList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
void
CZMEditionPopupController::AddListeningToTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZMUIDataObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->AddListeningToObject( mTitle, IZMEditionList::kDefaultIID );
}
