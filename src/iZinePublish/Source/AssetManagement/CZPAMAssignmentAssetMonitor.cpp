//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMAssignmentAssetMonitor.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssignmentMgr.h"
#include "IManageableAsset.h"
#include "IManagedStatus.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAMHelper.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "AZPAMAssetMonitor.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetMonitor
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMAssetMonitor
class CZPAMAssignmentAssetMonitor : public inheritClass
{
public:
						CZPAMAssignmentAssetMonitor(IPMUnknown * boss);
	virtual				~CZPAMAssignmentAssetMonitor();

	bool				ProcessAsset(
							IManageableAsset *			inAsset);

	bool16				CanMonitor( IManageableAsset *	asset );

protected:
	const char *		GetAssetMonitorName( ) const;
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentAssetMonitor, kZPAMAssignmentAssetMonitorImpl)

CZPAMAssignmentAssetMonitor::CZPAMAssignmentAssetMonitor(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentAssetMonitor::~CZPAMAssignmentAssetMonitor()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetMonitorName
//----------------------------------------------------------------------------------------
const char *
CZPAMAssignmentAssetMonitor::GetAssetMonitorName() const
{
	//LogFunctionEnterExit;
	return "Assignment";
}

//----------------------------------------------------------------------------------------
// CanMonitor
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentAssetMonitor::CanMonitor(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	bool16 toReturn = inheritClass::CanMonitor( pAsset );
	if( toReturn )
	{	//Check if this assignment is ours.
		InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		if( !assetData || assetData->GetAssetID().IsEmpty() )		//Handle only if our Asset Data is set.
			toReturn = kFalse;
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ProcessAsset
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentAssetMonitor::ProcessAsset(
	IManageableAsset *			inAsset)
{
	//Check Current editing state and update the Managed status.
	//Also check the local version & remote version status.

	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		if( !inAsset )
			break;
		
		InterfacePtr<IManagedStatus> assignmentStatus( inAsset->GetStatus(), UseDefaultIID() );
		InterfacePtr<IAMLockable> assignmentLock( assignmentStatus, UseDefaultIID() );
		ASSERT(assignmentLock);
		if ( !assignmentLock )
			break; //Should we remove such assets from monitoring list?

#if defined(InDnCS5) || defined(InDnCS5_5)
		assignmentLock->VerifyState(kSuppressUI, assignmentLock->GetLockObjectUID());
#else
		assignmentLock->VerifyState();
#endif

		bool16 isOnLine = assignmentStatus->IsOnline();
		IManagedStatus::VersionState versionState = assignmentStatus->GetVersionState();
		
		InterfacePtr<const IStringData> assignmentAssetID( assignmentStatus, IID_IZPDATAID );
		ASSERT( assignmentAssetID );
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assignmentAssetID->Get() );

		IManagedStatus::VersionState newVersionState = Utils<IZPAMHelper>()->GetAssetVersionState( dbAssetID );
		
		IManagedStatus::EditingState editingState = assignmentStatus->GetEditingState();
		IManagedStatus::StatusStringTable statusStrings;
		IManagedStatus::StatusIconTable statusIcons;
		assignmentStatus->GetDisplayStatusData( statusStrings, statusIcons );
		assignmentStatus->Update( isOnLine, editingState, newVersionState, statusStrings, statusIcons );

		bool needsUpdate = versionState != newVersionState;

		IDataBase * db = ::GetDataBase( inAsset );

		InterfacePtr<IDocument> storyDoc( db, db->GetRootUID(), UseDefaultIID() );
		if( !storyDoc )
			break;
		
		if( needsUpdate )
		{
			//Patch: change local file modification date, so that assignment panel shows out-of-date status.
			if( newVersionState == IManagedStatus::enLocalIsOutOfDate && versionState != IManagedStatus::enLocalIsOutOfDate )
			{
				IDFile assetFile;
				if( inAsset->GetWriteFile(assetFile) )
					Utils<IZPFileUtils>()->TouchFile( assetFile );
			}

			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
			ASSERT(assMgr);
			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
		}
	}while(kFalse);

	return true;
}