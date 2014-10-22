//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPUIActionFilter.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionFilter.h"
#include "InCopyBridgeUIID.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUIActionFilterData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IActionFilter>
class CZPUIActionFilter : public inheritClass
{
public:
						CZPUIActionFilter(IPMUnknown * boss);
	virtual				~CZPUIActionFilter();

	void				FilterAction(
							ClassID*					ioComponentClass,
							ActionID*					ioActionID,
							PMString*					ioActionName,
							PMString*					ioActionArea,
							int16*						ioActionType,
							uint32*						ioEnablingType,
							bool16*						ioUserEditable);
	
protected:
	void				ClearOldList();
private:
};


CREATE_PMINTERFACE(CZPUIActionFilter, kZPUIActionFilterImpl)

CZPUIActionFilter::CZPUIActionFilter(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIActionFilter::~CZPUIActionFilter()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// FilterAction
//----------------------------------------------------------------------------------------
void
CZPUIActionFilter::FilterAction(
	ClassID*					ioComponentClass,
	ActionID*					ioActionID,
	PMString*					ioActionName,
	PMString*					ioActionArea,
	int16*						ioActionType,
	uint32*						ioEnablingType,
	bool16*						ioUserEditable)
{
	static bool firstCall = true;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	ClassID newComponentClass = kInvalidClass;
	if( !ioActionID )
		return;
	const ActionID & theActionID = *ioActionID;
	if ( false
	//|| (theActionID	==	kICBridgeExportSelContextActionID )
	//|| (theActionID	==	kICBridgeExportSelMenuActionID )
	//|| (theActionID	==	kICBridgeRevertSelActionID )
	//|| (theActionID	==	kICBridgeSaveVersionSelActionID )
	//|| (theActionID	==	kICBridgeSLRevertActionID )
	|| (theActionID	==	kICBridgeSLSubmitActionID )
	|| (theActionID	==	kICBridgeSLSubmitAllActionID )
	|| (theActionID	==	kICBridgeSubmitAllActionID )
	|| (theActionID	==	kICBridgeSubmitSelActionID )
	|| (theActionID	==	kSaveActionID )
	//|| (theActionID	==	kSaveAllStoriesActionID )
	|| (theActionID	==	kSaveAsActionID )
	//|| (theActionID	==	kSLAddSelToNewAssignmentTextContextActionID )
	//|| (theActionID	==	kSLAddToNewAssignmentRtMenuActionID )	//Comes from Assignment panel right click
	|| (theActionID	==	kSLAssignmentOptionsActionID )
	//|| (theActionID	==	kSLNewAssignmentActionID )
	|| (theActionID	==	kSLRelinkAssignmentActionID )		//Change assignment location., not to be shown
	//|| (theActionID	==	kSLRemoveActionID )
	//|| (theActionID	==	kSLUpdateAllAssignmentsActionID )
	//|| (theActionID	==	kSLUpdateSelectedAssignmentsActionID )
	//|| (theActionID	>=	kSLAddAllGraphicsToAssignmentDynamicActionID &&		theActionID	<=	kSLAddAllGraphicsToAssignmentDynamicEndActionID )
	//|| (theActionID	>=	kSLAddSelToAssignmentTextContextDynamicActionID &&	theActionID	<=	kSLAddSelToAssignmentTextContextDynamicEndActionID )
	//|| (theActionID	>=	kSLAddToAssignmentRtMenuDynamicActionID &&	theActionID	<=	kSLAddToAssignmentRtMenuDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMActionComponentBoss;
	}
	else if (false
	||(theActionID	==	kSLAddLayerToNewAssignmentActionID )
	|| (theActionID	>=	kSLAddLayerToAssignmentDynamicActionID &&	theActionID	<=	kSLAddLayerToAssignmentDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMAddLayerToAssignmentActionComponentBoss;
	}
	else if ( false
	|| (theActionID	==	kSLAddAllStoriesToNewAssignmentActionID )
	|| (theActionID	>=	kSLAddAllStoriesToAssignmentDynamicActionID &&	theActionID	<=	kSLAddAllStoriesToAssignmentDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMAddAllStoriesToAssignmentActionComponentBoss;
	}
	else if ( false
	|| (theActionID	==	kSLAddSelToNewAssignmentActionID )
	|| (theActionID	>=	kSLAddSelToAssignmentDynamicActionID &&		theActionID	<=	kSLAddSelToAssignmentDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMAddSelToAssignmentActionComponentBoss;
	}
	else if ( false
	|| (theActionID	==	kSLAddSelToNewAssignmentLayoutContextActionID )
	|| (theActionID	>=	kSLAddSelToAssignmentLayoutContextDynamicActionID &&	theActionID	<=	kSLAddSelToAssignmentLayoutContextDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMAddSelToAssignmentLayoutContextActionComponentBoss;
	}
	else if ( false
	|| (theActionID	==	kSLAddToNewAssignmentActionID )			//Comes from Assignment panel fly menu
	|| (theActionID	>=	kSLAddToAssignmentDynamicActionID &&	theActionID	<=	kSLAddToAssignmentDynamicEndActionID )
	)
	{
		newComponentClass = kZPUIAMAddToAssignmentActionComponentBoss;
	}
	else if ( false
	|| theActionID == kPackageAssignmentActionID
	|| theActionID == kPackageAndEmailAssignmentActionID
	|| theActionID == kForwardPackagedAssignmentActionID
	|| theActionID == kForwardAndEmailPackagedAssignmentActionID
	|| theActionID == kReturnPackagedAssignmentActionID
	|| theActionID == kReturnAndEmailPackagedAssignmentActionID
	|| theActionID == kOpenPackagedAssignmentActionID
	|| theActionID == kCancelPackagedAssignmentActionID
#if defined(InDnCS5) || defined(InDnCS5_5)
	|| theActionID == kICBridgeRelinkSelActionID
	|| theActionID == kSLAddSelToNewAssignmentTextContextActionID
#endif
	)
	{
		newComponentClass = kZPUIAMActionComponentBoss;
	}

	if( newComponentClass != kInvalidClass )
	{
		if( firstCall )
		{	//Clear old persisted list only if we have filtered it.
			//In InCopy it works fine, but in InDesign FilterAction gets called even on the second run.
			//this->ClearOldList();
			firstCall = false;
		}
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		InterfacePtr<IZPUIActionFilterData> zpActionFilterData( iWorkspace, UseDefaultIID() );
		ASSERT(zpActionFilterData);

		if( ioComponentClass )
		{
			IDataBase * workspaceDb = ::GetDataBase( iWorkspace );
			//if( workspaceDb->GetTransactionLevel() == 0 )
			workspaceDb->BeginTransaction();
			
			zpActionFilterData->SetOldHandlerForAction( theActionID , *ioComponentClass );

			//if( workspaceDb->GetTransactionLevel() == 1 )
			workspaceDb->EndTransaction();

			IZPLog_Str_( thisFileLA, enLT_Details, "Setting action component : action = 0x%08X, handler = 0x%08X, OldHandler = 0x%08X", theActionID.Get(), newComponentClass.Get(), ioComponentClass->Get() );
			*ioComponentClass = newComponentClass;// kZPUIAMActionComponentBoss;
		}
	}
}

//----------------------------------------------------------------------------------------
// ClearOldList
//----------------------------------------------------------------------------------------
void
CZPUIActionFilter::ClearOldList()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
	InterfacePtr<IZPUIActionFilterData> zpActionFilterData( iWorkspace, UseDefaultIID() );
	ASSERT(zpActionFilterData);

	IDataBase * workspaceDb = ::GetDataBase( iWorkspace );
	//if( workspaceDb->GetTransactionLevel() == 0 )
	workspaceDb->BeginTransaction();
	
	zpActionFilterData->ClearList();
	
	//if( workspaceDb->GetTransactionLevel() == 1 )
	workspaceDb->EndTransaction();
}


