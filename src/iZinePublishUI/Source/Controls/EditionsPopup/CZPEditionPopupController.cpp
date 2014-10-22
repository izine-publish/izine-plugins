//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/EditionsPopup/CZPEditionPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPEditionList.h"
#include "IZPEditionMgr.h"
#include "IZPTitleMgr.h"


//IZP General includes
#include "AZPDBObjPopupController.h"
#include "AZPUIDBObjDropDownObserver.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupController
class CZPEditionPopupController : public inheritClass
{
public:
						CZPEditionPopupController(IPMUnknown * boss);
	virtual				~CZPEditionPopupController();
	void				FillValues();

	PMString			GetNthObjID(
							int							inIndex) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;
protected:
	void				RemoveListeningTitle();
	void				AddListeningToTitle();

	void				FillValues(
							const ZPEditionIDsList &	inIDList );
private:
	InterfacePtr<const IStringData> mTitle;	//Title for which editions are displayed by this popup.
										//retained here so that it does not get destroyed before its references are removed.
	ZPEditionIDsList mEditionIDList;
};


CREATE_PMINTERFACE(CZPEditionPopupController, kZPEditionPopupControllerImpl)

CZPEditionPopupController::CZPEditionPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionPopupController::~CZPEditionPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPEditionPopupController::FillValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IStringData> editionTitleID( this, IID_IZPTITLEIDSTRINGDATA );
	ASSERT(editionTitleID);
	const PMString & theTitleID = editionTitleID->Get();

	mEditionIDList.clear();
	//ZPEditionIDsList theEditionIDList;
	ZPEditionIDsList & theEditionIDList = mEditionIDList;

	//check if this ID is empty or NULL ID
	if( theTitleID.empty() || theTitleID.Compare( kFalse, kEmptyGUID ) == 0 )
	{
		this->RemoveListeningTitle();
		mTitle.reset(nil);
		//TODO: Remove all entries from this popup.
	}
	else
	{
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);

		const IStringData * theTitle = titleMgr->GetTitleForID( theTitleID );
		
		this->RemoveListeningTitle();
		if( theTitle )		//Can be nil, if previous selected title is not in current user title list
			theTitle->AddRef();
		mTitle.reset(theTitle);
		this->AddListeningToTitle();

		InterfacePtr<IZPEditionList> titleEditionList( theTitle, UseDefaultIID() );
	
		if ( titleEditionList != nil )
		{
			theEditionIDList = titleEditionList->GetStringList();
			//If edition id list comes empty, then it's observer will be notified when the list is available.
		}
	}

	//Sort status id on sort index
	std::sort( theEditionIDList.begin(), theEditionIDList.end(), CompareEditionIDByName() );

	this->FillValues( theEditionIDList );
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPEditionPopupController::FillValues(
	const ZPEditionIDsList &	inIDList)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iEditionStringListControlData( this, UseDefaultIID());
	ASSERT(iEditionStringListControlData);
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	int newListLen = inIDList.size();
	if( newListLen <= 0 )
		iEditionStringListControlData->Clear();
	else
		iEditionStringListControlData->Clear(kFalse, kFalse);

	ZPEditionIDsList::const_iterator endIter = inIDList.end();

	int i=1;
	for( ZPEditionIDsList::const_iterator currIter = inIDList.begin(); currIter != endIter; ++currIter,++i)
	{
		const PMString & editionID = *currIter;
		
		InterfacePtr<const IStringData> editionName( editionMgr->GetEditionForID(editionID), IID_IZPDATANAME );
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
CZPEditionPopupController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	InterfacePtr<IZPEditionList> titleEditionList( mTitle, UseDefaultIID() );
	ASSERT( titleEditionList );

#if 0
	const K2Vector<PMString>& theEditionsList = titleEditionList->GetStringList( false );
#else
	const K2Vector<PMString>& theEditionsList = mEditionIDList;
#endif

	ASSERT( theEditionsList.size() > inIndex );

	toReturn = theEditionsList[ inIndex ];

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
CZPEditionPopupController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	if(!mTitle )
		return toReturn;

	InterfacePtr<IZPEditionList> titleEditionList( mTitle, UseDefaultIID() );
	ASSERT( titleEditionList );

#if 0
	const K2Vector<PMString>& theEditionsList = titleEditionList->GetStringList( false );
#else
	const K2Vector<PMString>& theEditionsList = mEditionIDList;
#endif

	if( theEditionsList.size() )
		toReturn = theEditionsList.Location( inObjID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
void
CZPEditionPopupController::RemoveListeningTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->RemoveListeningObject( mTitle.get(), IZPEditionList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
void
CZPEditionPopupController::AddListeningToTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->AddListeningToObject( mTitle, IZPEditionList::kDefaultIID );
}
