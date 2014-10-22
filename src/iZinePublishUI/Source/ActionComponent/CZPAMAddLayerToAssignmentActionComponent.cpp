//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPAMAddLayerToAssignmentActionComponent.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-8-2010
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
#endif

//SDK General includes
#include "AssignmentUIID.h"
#include "CActionComponent.h"
#include "InCopyImportExportDefs.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPDefaultSession.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAMHelper.h"
#include "IZPUserCredentials.h"

//IZP General includes
#include "CZPInCopyStoryExportSuite.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CActionComponent
class CZPAMAddLayerToAssignmentActionComponent : public inheritClass
{
public:
						CZPAMAddLayerToAssignmentActionComponent(IPMUnknown * boss);
	virtual				~CZPAMAddLayerToAssignmentActionComponent();

	void				DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
protected:
	void				DoAction_Default(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
private:
	PMIID		mDelegateIID;
};


CREATE_PMINTERFACE(CZPAMAddLayerToAssignmentActionComponent, kZPAMAddLayerToAssignmentActionComponentImpl)

CZPAMAddLayerToAssignmentActionComponent::CZPAMAddLayerToAssignmentActionComponent(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZPACTIONCOMPONENTDELEGATE )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAddLayerToAssignmentActionComponent::~CZPAMAddLayerToAssignmentActionComponent()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPAMAddLayerToAssignmentActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	InterfacePtr<IActionComponent> selfDelegate( this, mDelegateIID );
	if( selfDelegate )
		selfDelegate->UpdateActionStates( ac, listToUpdate, mousePoint, widget );
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZPAMAddLayerToAssignmentActionComponent::DoAction(
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
	if( inActionID == kSLAddLayerToNewAssignmentActionID 
	|| ( inActionID > kSLAddLayerToAssignmentDynamicActionID && inActionID <= kSLAddLayerToAssignmentDynamicEndActionID ) )
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

			//InterfacePtr<IExportProvider> exporter(::CreateObject2<IExportProvider>( kInCopyExportMenuComponentBoss, IID_IINCOPYEXPORTLAYERSUITE));
			//InterfacePtr<IExportProvider> exporter(this, IID_IINCOPYEXPORTLAYERSUITE );
			InterfacePtr<CZPInCopyStoryExportSuite> exporter(this, IID_IEXPORTPROVIDER );
			ASSERT( exporter );

			bool isNewAssignmentActionID = (inActionID == kSLAddLayerToNewAssignmentActionID);
			if( isNewAssignmentActionID && !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( targetDoc ) )
			{
				newAssignmentCreationCalled = true;	//Supress calling default.
				break;
			}
			UIDList uidList = exporter->GetExportableItemList(selMgr);

			UID assignmentUID = kInvalidUID;
			if( isNewAssignmentActionID )
			{
				newAssignmentCreationCalled = true;
				UIDRef newAssignmentRef = Utils<IAssignmentUIUtils>()->NewAssignment( ac, kFullUI, kFalse );	//TODO: make sure this get save before return, Create St class for this
				assignmentUID = newAssignmentRef.GetUID();
				//InterfacePtr<IAssignment> iAssignment;//(Utils<IIZPUtils>()->CreateNewAssignment());
				//if(!iAssignment)
				//	break;
				//assignmentUID = ::GetUID( iAssignment );
				if( assignmentUID == kInvalidUID )
					break;	//Cancel the operation

				InterfacePtr<IDynamicMenu> dynamicMenu(this , UseDefaultIID() );
				dynamicMenu->RebuildMenu(kSLAddLayerToAssignmentDynamicActionID, nil);

				InterfacePtr<IActionIDToUIDData> actionID2UID(this, UseDefaultIID());
				ASSERT(actionID2UID);
				if (!actionID2UID)
					break;
			
				//TODO: make util function
				const ActionID startActionID = kSLAddLayerToAssignmentDynamicActionID + 1;
				const ActionID endActionID = kSLAddLayerToAssignmentDynamicEndActionID;

				//Change the inActionID from kSLAddLayerToNewAssignmentActionID to dynamic ID so that default action does not create new assignment.
				UID tempAssignmentUID = kInvalidUID;
				for (ActionID tempActionId = startActionID; tempActionId < endActionID; ++tempActionId)
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
			if(uidList.Length() != 0)
			{
				//Add new story to assginment
				IDFile sysFile = Utils<IZPAMHelper>()->CreateNewStoryAsset( targetDoc );

#if defined(InDnCS5) || defined(InDnCS5_5)
				const PMString formatName = InCopy::kInCopyMarkupFormatName;
#else
				const PMString formatName = InCopy::kInCopyInterchangeFormatName;
#endif
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
CZPAMAddLayerToAssignmentActionComponent::DoAction_Default(
IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	InterfacePtr<IActionComponent> selfDelegate( this, mDelegateIID );
	if( selfDelegate )
		selfDelegate->DoAction( ac, actionID, mousePoint, widget );
}
