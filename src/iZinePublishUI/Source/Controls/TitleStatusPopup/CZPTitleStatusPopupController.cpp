//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitleStatusPopup/CZPTitleStatusPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-6-2010
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
#include "IZPStatusList.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"


//IZP General includes
#include "AZPDBObjPopupController.h"
#include "AZPUIDBObjDropDownObserver.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupController
class CZPTitleStatusPopupController : public inheritClass
{
public:
						CZPTitleStatusPopupController(IPMUnknown * boss);
	virtual				~CZPTitleStatusPopupController();
	void				FillValues();

	PMString			GetNthObjID(
							int							inIndex) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;
protected:
	void				RemoveListeningTitle();
	void				AddListeningToTitle();

	void				FillValues(
							const ZPStatusIDsList	inIDList );
private:
	InterfacePtr<const IStringData> mTitle;	//Title for which status are displayed by this popup.
										//retained here so that it does not get destroyed before its references are removed.

	ZPStatusIDsList		mStatusIDList;
};


CREATE_PMINTERFACE(CZPTitleStatusPopupController, kZPTitleStatusPopupControllerImpl)

CZPTitleStatusPopupController::CZPTitleStatusPopupController(IPMUnknown * boss)
: inheritClass(boss)
//, mStatusIDList( nil )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleStatusPopupController::~CZPTitleStatusPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupController::FillValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IStringData> statusTitleID( this, IID_IZPTITLEIDSTRINGDATA );
	ASSERT(statusTitleID);
	const PMString & theTitleID = statusTitleID->Get();

	ZPStatusIDsList theStatusIDList;
	bool checkAndRemoveForcedStatus = false;

	//check if this ID is empty or NULL ID
	if( theTitleID.empty() || theTitleID.Compare( kFalse, kEmptyGUID ) == 0 )
	{
		this->RemoveListeningTitle();
		mTitle.reset(nil);
		mStatusIDList.clear();
		//TODO: Remove all entries from this popup.
	}
	else
	{
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);

		const IStringData * theTitle = titleMgr->GetTitleForID( theTitleID );
		//ASSERT(theTitle);	//Comes nil when moving from one db to other.

		this->RemoveListeningTitle();
		mTitle.reset(theTitle);
		
		if( theTitle == nil )
		{
			mStatusIDList.clear();
			//TODO: Remove all entries from this popup.
		}
		else
		{
			theTitle->AddRef();
			this->AddListeningToTitle();

			InterfacePtr<IZPStatusList> titleStatusList( mTitle, UseDefaultIID() );
			if ( titleStatusList != nil )
			{
				theStatusIDList = titleStatusList->GetStringList();
				checkAndRemoveForcedStatus = true;
				if( titleStatusList->HasData() == false )
				{
	#pragma message("			Uncomment this when status per title is implemented")
					//toReturn = false;

					InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
					ASSERT( zpApp );

					InterfacePtr<const IZPTitleStatusMgr> statusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
					ASSERT(statusMgr);

					theStatusIDList = statusMgr->GetDefaultStatusIDs();
				}
			//If status id list comes empty, then it's observer will be notified when the list is available.
			}
		}
	}

	if (checkAndRemoveForcedStatus)
	{
		mStatusIDList.clear();
		ZPStatusIDsList::const_iterator iter = theStatusIDList.begin();
		ZPStatusIDsList::const_iterator endIter = theStatusIDList.end();
		for (; iter != endIter; ++iter)
		{
			const PMString &statusID = *iter;
			//TODO: someday, move the following comparison to some filtering function.
			if ((statusID.Compare(false, kTitleStatus_Assigned) == 0)		//Not to display forced status
			|| (statusID.Compare(false, kTitleStatus_Unassigned) == 0)
			|| (statusID.Compare(false, kTitleStatus_Template) == 0))
				continue;

			mStatusIDList.push_back(statusID);
		}

		//Sort status id on sort index
		std::sort( mStatusIDList.begin(), mStatusIDList.end(), CompareStatusIDBySortIndex() );
	}

	//if( mStatusIDList )
		this->FillValues( mStatusIDList );
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupController::FillValues(
	const ZPStatusIDsList	inIDList)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iStatusStringListControlData( this, UseDefaultIID());
	ASSERT(iStatusStringListControlData);
	
	int newListLen = inIDList.size();
	if( newListLen > 0 )
		iStatusStringListControlData->Clear();
	else
		iStatusStringListControlData->Clear(kFalse, kFalse);

	if (inIDList.size())
	{
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZPTitleStatusMgr> titleStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleStatusMgr);


		ZPStatusIDsList::const_iterator endIter = inIDList.end();

		int i=1;
		for( ZPStatusIDsList::const_iterator currIter = inIDList.begin(); currIter != endIter; ++currIter,++i)
		{
			const PMString & statusID = *currIter;
			
			InterfacePtr<const IStringData> statusName( titleStatusMgr->GetStatusForID(statusID), IID_IZPDATANAME );
			ASSERT( statusName );
			const PMString & theStatusName = statusName->Get();

			if ( i == newListLen )
				iStatusStringListControlData->AddString(theStatusName);
			else
				iStatusStringListControlData->AddString(theStatusName, IStringListControlData::kEnd, kFalse, kFalse );
		}
	}

	this->SendMessage_ListLengthChanged();
}

//----------------------------------------------------------------------------------------
// GetNthObjID
//----------------------------------------------------------------------------------------
PMString
CZPTitleStatusPopupController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 || mStatusIDList.size() == 0)
		return toReturn;

	//InterfacePtr<const IZPStatusList> titleStatusList( mTitle, UseDefaultIID() );
	//ASSERT( titleStatusList );

	//const K2Vector<PMString>& theStatusList = titleStatusList->GetStringList();
	const K2Vector<PMString> & theStatusList = mStatusIDList;

	ASSERT( theStatusList.size() > inIndex );

	toReturn = theStatusList[ inIndex ];

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
CZPTitleStatusPopupController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	if(!mTitle || mStatusIDList.size() == 0)
		return toReturn;

	//InterfacePtr<const IZPStatusList> titleStatusList( mTitle, UseDefaultIID() );
	//ASSERT( titleStatusList );

	//const K2Vector<PMString>& theStatusList = titleStatusList->GetStringList();
	const K2Vector<PMString> & theStatusList = mStatusIDList;

	if( theStatusList.size() )
		toReturn = theStatusList.Location( inObjID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupController::RemoveListeningTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->RemoveListeningObject( mTitle.get(), IZPStatusList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupController::AddListeningToTitle()
{
	if( !mTitle )
		return;

	InterfacePtr<AZPUIDBObjDropDownObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	selfObserver->AddListeningToObject( mTitle, IZPStatusList::kDefaultIID );
}
