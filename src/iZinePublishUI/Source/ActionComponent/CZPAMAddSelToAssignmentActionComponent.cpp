//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPAMAddSelToAssignmentActionComponent.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionIDToUIDData.h"
#include "IActiveContext.h"
#include "IAssignment.h"
#include "IAssignmentMgr.h"
#include "IAssignmentUIUtils.h"
#include "IBoolData.h"
#include "IDocument.h"
#include "IDynamicMenu.h"
#include "IExportProvider.h"
#include "ISelectionManager.h"
#include "ISelectionUtils.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILayoutUIUtils.h"
#include "IActionStateList.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"
#include "CActionComponent.h"
#include "ErrorUtils.h"
#include "InCopyImportExportDefs.h"
#include "InCopyWorkflowID.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPDefaultSession.h"
#include "IZPUserCredentials.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAMHelper.h"

//IZP General includes
#include "CZPInCopyStoryExportSuite.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CActionComponent
class CZPAMAddSelToAssignmentActionComponent : public inheritClass
{
public:
						CZPAMAddSelToAssignmentActionComponent(IPMUnknown * boss);
	virtual				~CZPAMAddSelToAssignmentActionComponent();

	void				DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
protected:
	void				DoAction_Default(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
#if defined(InDnCS5) || defined(InDnCS5_5)
	void				UpdateAction_Default(IActiveContext* ac, IActionStateList* listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
#endif
private:
	PMIID		mDelegateIID;
};


CREATE_PMINTERFACE(CZPAMAddSelToAssignmentActionComponent, kZPAMAddSelToAssignmentActionComponentImpl)

CZPAMAddSelToAssignmentActionComponent::CZPAMAddSelToAssignmentActionComponent(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZPACTIONCOMPONENTDELEGATE )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAddSelToAssignmentActionComponent::~CZPAMAddSelToAssignmentActionComponent()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPAMAddSelToAssignmentActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
	do
	{
		InterfacePtr<const IZPDefaultSession> defaultSession( GetExecutionContextSession(), UseDefaultIID() );
		ASSERT( defaultSession );
		
		const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();
		InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
	
		bool isLoggedIn = ( defaultCredentials && isValidData && isValidData->Get());

		InterfacePtr<const IDocument> frontDoc( Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID() );
		if(frontDoc == nil)
		{
			this->UpdateAction_Default( ac, listToUpdate, mousePoint, widget );
			break;
		}
	
		InterfacePtr<const IZPAssetData> assetData( frontDoc, UseDefaultIID() );
		ASSERT(assetData);

		const PMString & assetID = assetData->GetAssetID();
	
		if(isLoggedIn && assetData && assetID.IsEmpty())
		{
			int actionIDListLen = listToUpdate->Length();

			for(int i = 0; i < actionIDListLen; ++i)
			{
				ActionID actionID = listToUpdate->GetNthAction(i);
				int16 state = listToUpdate->GetNthActionState(i);
			
				switch (actionID.Get())
				{
					case kSLAddSelToNewAssignmentLayoutContextActionID:
					{
						state = kDisabled_Unselected;
						listToUpdate->SetNthActionState(i, state);
					}
				}
			}
		}
		else
		{
			this->UpdateAction_Default( ac, listToUpdate, mousePoint, widget );
		}
	}while(kFalse);	
#else
	this->UpdateAction_Default( ac, listToUpdate, mousePoint, widget );
#endif
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZPAMAddSelToAssignmentActionComponent::DoAction(
	IActiveContext* ac, ActionID inActionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );
	const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();
	InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
	
	bool isLoggedIn = ( defaultCredentials && isValidData && isValidData->Get());

	bool doDefaultAction = true;
	bool newAssignmentCreationCalled = false;
	if( inActionID == kSLAddSelToNewAssignmentLayoutContextActionID 
	|| inActionID == kSLAddSelToNewAssignmentActionID
	|| ( inActionID == kSLAddToNewAssignmentActionID )			//Comes from Assignment panel fly menu
	|| ( inActionID > kSLAddSelToAssignmentDynamicActionID && inActionID <= kSLAddSelToAssignmentDynamicEndActionID )
	|| ( inActionID > kSLAddSelToAssignmentLayoutContextDynamicActionID && inActionID <= kSLAddSelToAssignmentLayoutContextDynamicEndActionID )
	|| ( inActionID > kSLAddToAssignmentDynamicActionID && inActionID <= kSLAddToAssignmentDynamicEndActionID )
	)
	{
		do
		{
			if( !isLoggedIn )
				break;
			InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
			if(!ac)
				break;

			InterfacePtr<ISelectionManager> selMgr(Utils<ISelectionUtils>()->GetActiveSelection(), UseDefaultIID());
			if(!selMgr)
				break;
			
			InterfacePtr<IDocument> targetDoc(ac->GetContextDocument(), UseDefaultIID());
			if(!targetDoc)
				break;
			
			//check if this is our document else do default action.
			InterfacePtr<const IZPAssetData> targetDocAssetData( targetDoc, UseDefaultIID() );
			ASSERT( targetDocAssetData );
			if( targetDocAssetData->GetAssetID().IsEmpty() )
				break;

#if defined(InDnCS5) || defined(InDnCS5_5)
			const PMString formatName = InCopy::kInCopyMarkupFormatName;
#else
			const PMString formatName = InCopy::kInCopyInterchangeFormatName;
#endif
			InterfacePtr<IExportProvider> selectionMgrExporter(selMgr, IID_IINCOPYEXPORTSUITE );
			if( !selectionMgrExporter || !selectionMgrExporter->CanExportThisFormat( targetDoc, selMgr, formatName ) )
				break;
			InterfacePtr<CZPInCopyStoryExportSuite> exporter(this, IID_IEXPORTPROVIDER );
			ASSERT( exporter );
			if( !exporter )
				break;

			bool isNewAssignmentActionID = (inActionID == kSLAddSelToNewAssignmentActionID
											||	inActionID == kSLAddToNewAssignmentActionID
											|| 	inActionID == kSLAddSelToNewAssignmentLayoutContextActionID );

			if( isNewAssignmentActionID && !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( targetDoc ) )
			{
				newAssignmentCreationCalled = true;	//Supress calling default.
				ErrorUtils::PMSetGlobalErrorCode(kZPNewAssignmentNotAllowedError);
				break;
			}

			UIDList uidList = exporter->GetExportableItemList(selMgr);

			UID assignmentUID = kInvalidUID;
			if( isNewAssignmentActionID )
			{
				newAssignmentCreationCalled = true;
				UIDRef newAssignmentRef = Utils<IAssignmentUIUtils>()->NewAssignment( ac, kFullUI, kFalse );
				assignmentUID = newAssignmentRef.GetUID();
				//InterfacePtr<IAssignment> iAssignment;//(Utils<IIZPUtils>()->CreateNewAssignment());
				//if(!iAssignment)
				//	break;
				//assignmentUID = ::GetUID( iAssignment );
				if( assignmentUID == kInvalidUID )
					break;	//Cancel the operation

				InterfacePtr<IActionIDToUIDData> actionID2UID(this, UseDefaultIID());
				ASSERT(actionID2UID);
				if (!actionID2UID)
					break;
			
				ActionID startActionID = kSLAddSelToAssignmentDynamicActionID;
				ActionID endActionID = kSLAddSelToAssignmentDynamicEndActionID;
				if( inActionID == kSLAddSelToNewAssignmentActionID )
				{
					startActionID = kSLAddSelToAssignmentDynamicActionID;
					endActionID = kSLAddSelToAssignmentDynamicEndActionID;
				}
				else if ( inActionID == kSLAddToNewAssignmentActionID )
				{
					startActionID = kSLAddToAssignmentDynamicActionID;
					endActionID = kSLAddToAssignmentDynamicEndActionID;
				}
				else if ( inActionID == kSLAddSelToNewAssignmentLayoutContextActionID )
				{
					startActionID = kSLAddSelToAssignmentLayoutContextDynamicActionID;
					endActionID = kSLAddSelToAssignmentLayoutContextDynamicEndActionID;
				}
				InterfacePtr<IDynamicMenu> dynamicMenu(this , UseDefaultIID() );
				dynamicMenu->RebuildMenu(startActionID, nil);

				//TODO: make util function
				//Change the inActionID from kSLAddSelToAssignmentDynamicActionID to dynamic ID so that default action does not create new assignment.
				UID tempAssignmentUID = kInvalidUID;
				for (ActionID tempActionId = startActionID +1; tempActionId < endActionID; ++tempActionId)
				{
					tempAssignmentUID = actionID2UID->GetUID( tempActionId );
					if( tempAssignmentUID == assignmentUID)
					{
						inActionID = tempActionId;
						break;
					}
				}
			}
			else
			{
				InterfacePtr<IActionIDToUIDData> actionID2UID(this, UseDefaultIID());
				ASSERT(actionID2UID);
				if (!actionID2UID)
					break;
			
				assignmentUID = actionID2UID->GetUID( inActionID );
			}

			if( assignmentUID == kInvalidUID )
				break;

			doDefaultAction = false;

			IDataBase* targetDB = ::GetDataBase(targetDoc);
			ASSERT(targetDB);
			//Note: Single selection gets handled correctly, issue is with muliple selection.
			if(uidList.Length() > 1)
			{
				IDFile sysFile = Utils<IZPAMHelper>()->CreateNewStoryAsset( targetDoc );

				exporter->ExportToFile(sysFile, targetDoc, selMgr, formatName, kSuppressUI);
				
				Utils<IAssignmentUIUtils>()->AddListToAssignment(UIDRef(targetDB, assignmentUID), uidList);
			}

			//Add all layer items i.e that are already exported
			this->DoAction_Default( ac, inActionID, mousePoint, widget );

			if( newAssignmentCreationCalled ) //save newly created assignment
			{
				InterfacePtr<IAssignment> assignment(UIDRef(targetDB, assignmentUID), UseDefaultIID()); 
				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID()); 
				assMgr->SaveAssignment(assignment);
			}
		} while (false);
	}
	
	//Not handled by us, so do default action
	if( !newAssignmentCreationCalled && doDefaultAction )
	{
		this->DoAction_Default( ac, inActionID, mousePoint, widget );
	}
}

//----------------------------------------------------------------------------------------
// DoAction_Default
//----------------------------------------------------------------------------------------
void
CZPAMAddSelToAssignmentActionComponent::DoAction_Default(
IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	InterfacePtr<IActionComponent> selfDelegate( this, mDelegateIID );
	if( selfDelegate )
		selfDelegate->DoAction( ac, actionID, mousePoint, widget );
}

//----------------------------------------------------------------------------------------
// UpdateAction default
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
void
CZPAMAddSelToAssignmentActionComponent::UpdateAction_Default(
IActiveContext* ac, IActionStateList* listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	InterfacePtr<IActionComponent> selfDelegate( this, mDelegateIID );
	if( selfDelegate )
		selfDelegate->UpdateActionStates( ac, listToUpdate, mousePoint, widget );
}
#endif
