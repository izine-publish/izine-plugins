//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitleUsersPopup/CZPTitleUsersPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IStringListControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPUserMgr.h"
#include "IZPUserList.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPDBObjPopupWithFixedValController.h"
#include "AZPUIDBObjDropDownObserver.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupWithFixedValController
class CZPTitleUsersPopupController : public inheritClass
{
public:
						CZPTitleUsersPopupController(IPMUnknown * boss);
	virtual				~CZPTitleUsersPopupController();

	void				AppendDBObjectValues();

	PMString			GetNthDBObjID(
							int							inIndex) const;
	int32				GetIndexOfDBObjID(
							const PMString &			inObjID ) const;
protected:
	void				RemoveListeningTitle();
	void				AddListeningToTitle();

	void				FillValues(
							const ZPUserIDsList &	inIDList );
private:
	InterfacePtr<const IStringData> mTitle;	//Title for which users are displayed by this popup.
										//retained here so that it does not get destroyed before its references are removed.
};


CREATE_PMINTERFACE(CZPTitleUsersPopupController, kZPTitleUsersPopupControllerImpl)

CZPTitleUsersPopupController::CZPTitleUsersPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleUsersPopupController::~CZPTitleUsersPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AppendDBObjectValues
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupController::AppendDBObjectValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IStringData> usersTitleID( this, IID_IZPTITLEIDSTRINGDATA );
	ASSERT(usersTitleID);
	const PMString & theTitleID = usersTitleID->Get();

	ZPUserIDsList theUserIDList;

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

		InterfacePtr<IZPUserList> titleUserList( theTitle, UseDefaultIID() );
	
		if ( titleUserList != nil )
		{
			theUserIDList = titleUserList->GetStringList();
			//If user id list comes empty, then it's observer will be notified when the list is available.
		}
	}
	this->FillValues( theUserIDList );
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupController::FillValues(
	const ZPUserIDsList &	inIDList)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iUsersStringListControlData( this, UseDefaultIID());
	ASSERT(iUsersStringListControlData);
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPUserMgr> userMgr( zpApp->GetUserMgr(), UseDefaultIID() );
	ASSERT(userMgr);

	int newListLen = inIDList.size();
	if( newListLen <= 0 )
		iUsersStringListControlData->Clear();
	else
		iUsersStringListControlData->Clear(kFalse, kFalse);

	ZPUserIDsList::const_iterator endIter = inIDList.end();

	int i=1;
	for( ZPUserIDsList::const_iterator currIter = inIDList.begin(); currIter != endIter; ++currIter,++i)
	{
		const PMString & userID = *currIter;
		
		InterfacePtr<const IStringData> userName( userMgr->GetUserForID(userID), IID_IZPDATANAME );
		ASSERT( userName );
		const PMString & theUserName = userName->Get();

		if ( i == newListLen )
			iUsersStringListControlData->AddString(theUserName);
		else
			iUsersStringListControlData->AddString(theUserName, IStringListControlData::kEnd, kFalse, kFalse );
	}

	//this->SendMessage_ListLengthChanged();	//Commented as it is now send by its base class.
}

//----------------------------------------------------------------------------------------
// GetNthDBObjID
//----------------------------------------------------------------------------------------
PMString
CZPTitleUsersPopupController::GetNthDBObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	InterfacePtr<IZPUserList> titleUserList( mTitle, UseDefaultIID() );
	ASSERT( titleUserList );

	const K2Vector<PMString>& theUsersList = titleUserList->GetStringList( false );

	ASSERT( theUsersList.size() > inIndex );

	toReturn = theUsersList[ inIndex ];

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfDBObjID
//----------------------------------------------------------------------------------------
int32
CZPTitleUsersPopupController::GetIndexOfDBObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	if(!mTitle )
		return toReturn;

	InterfacePtr<IZPUserList> titleUserList( mTitle, UseDefaultIID() );
	ASSERT( titleUserList );

	const K2Vector<PMString>& theUsersList = titleUserList->GetStringList( false );

	if( theUsersList.size() )
		toReturn = theUsersList.Location( inObjID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupController::RemoveListeningTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->RemoveListeningObject( mTitle.get(), IZPUserList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupController::AddListeningToTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->AddListeningToObject( mTitle, IZPUserList::kDefaultIID );
}
