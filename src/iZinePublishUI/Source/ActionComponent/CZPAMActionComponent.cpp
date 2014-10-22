//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPAMActionComponent.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"
#include "IActiveContext.h"
#include "IAMServiceData.h"
#include "IAMUIService.h"
#include "IApplication.h"
#include "IAssignment.h"
#include "IAssignmentUIUtils.h"
#include "IBoolData.h"
#include "IControlView.h"
#include "IDocument.h"
#include "ILayoutUIUtils.h"
#include "IManageableAsset.h"
#include "InCopyBridgeUIID.h"
#include "IPanelMgr.h"
#include "IStringData.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILink.h"
#include "ILinkFacade.h"
#include "IGalleyUtils.h"
#include "ILinkManager.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"
#include "CActionComponent.h"
#include "CAlert.h"
#include "ErrorUtils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetsPanelHelper.h"
#include "IZPAssetUtils.h"
#include "IZPDefaultSession.h"
#include "IZPDocUtils.h"
#include "IZPGlobalTempData.h"
#include "IZPInDnUtils.h"
#include "IZPUIActionFilterData.h"
#include "IZPUserCredentials.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "CActionStateList.h"
#include "ZPDataHelper.h"

#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"

#include "IZPLog.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IAssignmentMgr.h"
#include "IExportProvider.h"
#include "ISelectionManager.h"
#include "ISelectionUtils.h"
#include "ILayoutSelectionSuite.h"
#include "InCopyWorkflowID.h"
#include "InCopyImportExportDefs.h"
#include "ITextFrameColumn.h"
#include "IHierarchy.h"
#include "ITextModel.h"
#include "IZPFileUtils.h"
#include "ProgressBar.h"
#endif

#pragma mark -

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CActionComponent
class CZPAMActionComponent : public inheritClass
{
public:
						CZPAMActionComponent(IPMUnknown * boss);
	virtual				~CZPAMActionComponent();

	void				DoAction(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(
							IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
protected:
	void				DoDefaultAction(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateDefaultActionStates(
							int							inActionIndex,
							IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);

	void				DoDocSave(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget );
	void				DoDocSaveAs(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget );

	void				HandleSubmitAction(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	//void				DoSaveStory();
	//void				DoUpdateStory();
	//void				DoCheckInStory();
	//void				DoCheckOutStory();

	//void				DoApplyTags();
	//void				UpdateAssetsTreeView();
	
	//ErrorCode			DeleteAssignment(
	//						IAssignment * assignment);

	//UIDRef				GetSelectedAssignmentRef();

	bool				CanUpdateAssetStatus();
	void				DoUpdateAssetStatus();
	bool				CanCheckoutAllStories(
							IActiveContext*					ac,
							GSysPoint						mousePoint,
							IPMUnknown*						widget);

	const IStringData *	GetSelectedAsset() const;

private:
#if defined(InDnCS5) || defined(InDnCS5_5)
	void				GetExportableAssets(IActiveContext* ac, ZPAssgStoriesMap& list);
	ErrorCode				ReExportAssets(IActiveContext* ac, ZPAssgStoriesMap& list);

#endif
};

CREATE_PMINTERFACE(CZPAMActionComponent, kZPAMActionComponentImpl)

CZPAMActionComponent::CZPAMActionComponent(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMActionComponent::~CZPAMActionComponent()
{

}

void SNIPLOG(const char* str, ...)
{
	static char gBuffer[2048];

	va_list args;
	va_start(args, str);
	vsnprintf(gBuffer, sizeof(gBuffer) - 1, str, args);
	va_end(args);
	
	TRACE(gBuffer);
	//	SnipRunLog::Instance()->Log(PMString(gBuffer));
}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
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


	InterfacePtr<const IDocument> frontDoc( Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID() );
	
	//TODO: create bool values out of for loop for 
	// doc saved state, logged in state.
	int actionIDListLen = listToUpdate->Length();
	do
	{
		bool bStateSet = false;
		for(int i=0; i < actionIDListLen; ++i)
		{
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			bStateSet = false;
			switch (actionID.Get())
			{
				case kIZPUpdatedSelectedAssetStatusActionID:
				{
					if( isLoggedIn && this->CanUpdateAssetStatus() )
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				//////case kICBridgeExportSelContextActionID:
				//////case kICBridgeExportSelMenuActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	ISelectionManager* selMgr = Utils<ISelectionUtils>()->GetActiveSelection();
				//////	if (!selMgr)
				//////		break;
				//////	
				//////	InterfacePtr<IExportProvider> inCopyExportProvider (selMgr, IID_IINCOPYEXPORTSUITE);
				//////	if (!inCopyExportProvider)
				//////		break;
				//////	
				//////	if (!inCopyExportProvider->CanExportThisFormat(frontDoc, selMgr, InCopy::kInCopyStoryFormatName))
				//////		break;
				//////	
				//////	state = kEnabledAction;

				//////	break;
				//////}
				//////case kIZPApplyTagsActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////	break;
				//////}
				case kICBridgeSLSubmitActionID :
				case kICBridgeSLSubmitAllActionID :
				case kICBridgeSubmitAllActionID :
				case kICBridgeSubmitSelActionID :
					this->UpdateDefaultActionStates( i, ac, listToUpdate, mousePoint, widget );
					bStateSet = true;
					break;
				//////case kICBridgeSLSubmitAllActionID:
				//////{
				//////	if (!Utils<IAssignmentUIUtils>()->CanSubmitAll(ac))
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
				//////	break;
				//////	
				//////case kICBridgeSubmitAllActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
				//////	ASSERT(selection);
				//////	if (!selection)
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////	break;
				//////}
				case kSLAssignmentOptionsActionID:
					// disable this actions
					break;
				//////	do {
				//////		if (!frontDoc || !frontDoc->IsSaved())
				//////			break;
				//////		
				//////		if (!isLoggedIn)
				//////			break;
				//////		
				//////		if (!Utils<IIZPUtils>()->IsInDesign())
				//////			break;
				//////		
				//////		if (!Utils<IAssignmentUIUtils>()->CanInvokeAssignmentOptions(ac))
				//////			break;
				//////		
				//////		state = kEnabledAction;
				//////		
				//////	} while (kFalse);
				//////	
				//////	break;
				//////case kSaveAllStoriesActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	InterfacePtr<IDocument> iDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
				//////	if(!iDoc)
				//////		break;
				//////	
				//////	UIDRef docRef = ::GetUIDRef(iDoc);
				//////	
				//////	// Get the storylist
				//////	InterfacePtr<IStoryList> storyList (iDoc, UseDefaultIID());
				//////	
				//////	// Figure out how many we have
				//////	int32 storyCount = storyList->GetUserAccessibleStoryCount();
				//////	
				//////	bool16 exportParentsOnly = kTrue; //preference?
				//////	
				//////	// Loop through the storyList
				//////	for (int32 i = 0; i < storyCount; i++)
				//////		
				//////	{
				//////		UIDRef storyRef = storyList->GetNthUserAccessibleStoryUID(i);
				//////		
				//////		UIDList singleStoryList(storyRef);
				//////		
				//////		InterfacePtr<IDataLink> iDataLink(Utils<IInCopyBridgeUtils>()->QueryDataLink(storyRef));
				//////		if(!iDataLink)
				//////			continue;
				//////		
				//////		InterfacePtr<IManageableAsset> iAsset(iDataLink, UseDefaultIID());
				//////		if(!iAsset)
				//////			continue;
				//////		
				//////		InterfacePtr<IAssetData> data(iAsset, UseDefaultIID());
				//////		if (!data)
				//////			continue;
				//////		
				//////		InterfacePtr<IManageableAsset> asset(iDataLink, UseDefaultIID());
				//////		if(!asset)
				//////			continue;
				//////		
				//////		InterfacePtr<IAMLockable> assetLock( iAsset->GetLockable(), UseDefaultIID() );
				//////		if(!assetLock)
				//////			continue;
				//////		
				//////		// currently CheckedOut
				//////		if(!assetLock->HasLock())
				//////			continue;
				//////		
				//////		state = kEnabledAction;
				//////	}
				//////	
				//////}
				//////	break;
				//////case kICBridgeSaveVersionSelActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
				//////	ASSERT(selection);
				//////	if (!selection)
				//////		break;

				//////	if (!selection->CanSaveVersion())
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
				//////	break;
				//////case kSLRelinkAssignmentActionID:
				//////	// disable change assignment location
				//////	break;
				//////case kSLUpdateAllAssignmentsActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	if (!Utils<IIZPUtils>()->IsInDesign())
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////	break;
				//////}
				//////case kSLUpdateSelectedAssignmentsActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	if (!Utils<IIZPUtils>()->IsInDesign())
				//////		break;
				//////	
				//////	UIDRef assignmentRef = GetSelectedAssignmentRef();
				//////	
				//////	if (assignmentRef==kInvalidUIDRef)
				//////		break;
				//////	
				//////	InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
				//////	ASSERT (assignment);
				//////	
				//////	if (!assignment)
				//////		break;

				//////	if (assignment->GetFile().IsEmpty())
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
				//////	break;
				//////case kSLRemoveActionID:
				//////{
				//////	do {
				//////		// only if linked story selected
				//////		if (!frontDoc || !frontDoc->IsSaved())
				//////			break;
				//////		
				//////		if (!isLoggedIn)
				//////			break;
				//////		
				//////		if (!Utils<IIZPUtils>()->IsInDesign())
				//////			break;
				//////		
				//////		UIDRef assignmentRef = GetSelectedAssignmentRef();
				//////		
				//////		if (assignmentRef==kInvalidUIDRef)
				//////			break;
				//////		
				//////		InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
				//////		ASSERT (assignment);
				//////		
				//////		if (!assignment)
				//////			break;
				//////		
				//////		if (assignment->GetFile().IsEmpty())
				//////			break;
				//////		
				//////		state = kEnabledAction;
				//////	} while (kFalse);
				//////}
				//////break;
				//////case kICBridgeSLRevertActionID:
				//////{
				//////	if (!Utils<IAssignmentUIUtils>()->CanRevertOnSelection(ac))
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////	
				//////	break;
				//////}
				//////	/*
				//////	 case kRevertActionID:
				//////	 */
				//////case kICBridgeRevertSelActionID:
				//////{
				//////	InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
				//////	ASSERT(ac);
				//////	
				//////	InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
				//////	ASSERT(selection);
				//////	
				//////	if (!selection)
				//////		break;
				//////	
				//////	if (!selection->CanRevertChanges())
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
				//////break;
				//////case kRevertStoryActionID:
				//////{
				//////	InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
				//////	ASSERT(selection);
				//////	if (!selection)
				//////		break;
				//////	
				//////	if (!selection->CanRevertChanges())
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
			//////		break;
				//////
				//////case kICBridgeSLSubmitActionID:
				//////{
				//////	if (!Utils<IAssignmentUIUtils>()->CanSubmitOnSelection(ac))
				//////		break;

				//////	state = kEnabledAction;
				//////	
				//////	break;
				//////}
				//////case kICBridgeSubmitSelActionID:
				//////{
				//////	InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
				//////	ASSERT(selection);
				//////	if (!selection)
				//////		break;
				//////	
				//////	if (!selection->CanSubmit())
				//////		break;

				//////	state = kEnabledAction;
	
				//////	break;
				//////}
				//////case kSaveACopyActionID:
				//////	if (!frontDoc)
				//////		break;
				//////	
				//////	if (!Utils<IIZPUtils>()->IsInDesign())
				//////		break;

				//////	state = kEnabledAction;
				//////	break;
				//////case kIZPCheckOutAllActionID:
				//////{
				//////	if (!frontDoc || !frontDoc->IsSaved())
				//////		break;
				//////	
				//////	if (!isLoggedIn)
				//////		break;
				//////	
				//////	
				//////	state = kEnabledAction;
				//////}
				//////	break;
#if defined(InDnCS5) || defined(InDnCS5_5)
				case kICBridgeRelinkSelActionID:
#endif
				case kSaveActionID:
				case kSaveAsActionID:
					this->UpdateDefaultActionStates( i, ac, listToUpdate, mousePoint, widget );
					bStateSet = true;
					break;
				//////case kUpdateDesignActionID:
				//////{
				//////	// update design is not working, should create our own version of sdk sample incopyfileactions
				//////	if (!frontDoc)
				//////		break;

				//////	if (!Utils<IIZPUtils>()->IsInCopy())
				//////		break;
				//////
				//////	if(!frontDoc)
				//////		break;
				//////	
				//////	state = kEnabledAction;
				//////}
				//////	break;

				//menu items not be shown for these actions
				case kSLRelinkAssignmentActionID:					//Change assignment location.
				case kPackageAssignmentActionID:					//All package relation actions
				case kPackageAndEmailAssignmentActionID:
				case kForwardPackagedAssignmentActionID:
				case kForwardAndEmailPackagedAssignmentActionID:
				case kReturnPackagedAssignmentActionID:
				case kReturnAndEmailPackagedAssignmentActionID:
				case kOpenPackagedAssignmentActionID:
				case kCancelPackagedAssignmentActionID:
					// disable these actions
					break;

				case kIZPCheckoutAllAssignmentStoriesActionID:
				{
					if (this->CanCheckoutAllStories(ac, mousePoint, widget))
						state = kEnabledAction;
					break;
				}
#if defined(InDnCS5) || defined(InDnCS5_5)
				case kSLAddSelToNewAssignmentTextContextActionID:				
				{
					IDocument* document = GetExecutionContextSession()->GetActiveContext()->GetContextDocument();
					ASSERT(document);

					InterfacePtr<const IZPAssetData> assetData( document, UseDefaultIID() );
					ASSERT(assetData);

					const PMString & assetID = assetData->GetAssetID();					
					
					if( isLoggedIn && assetID.IsEmpty())					
						state = kDisabled_Unselected;
					else
					{
						this->UpdateDefaultActionStates( i, ac, listToUpdate, mousePoint, widget );
						bStateSet = true;
					}
					break;
				}
#endif
				//////default:
				//////{
				//////	state = kEnabledAction;
				//////	break;
				//////}
			}
			if( !bStateSet )
				listToUpdate->SetNthActionState(i, state);
		}
	} while (kFalse);
}

/* DoAction
*/
//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
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

	switch (actionID.Get())
	{
		case kIZPUpdatedSelectedAssetStatusActionID:
		{
			if( isLoggedIn )
				this->DoUpdateAssetStatus();
			break;
		}
//////		case kICBridgeExportSelContextActionID:
//////		case kICBridgeExportSelMenuActionID:
//////		{
//////			IDocument* frontDoc = ac->GetContextDocument();
//////
//////			ISelectionManager* selMgr = Utils<ISelectionUtils>()->GetActiveSelection();
//////			if (!selMgr)
//////				break;
//////			
//////			InterfacePtr<IExportProvider> inCopyExportProvider (selMgr, IID_IINCOPYEXPORTSUITE);
//////			if (!inCopyExportProvider)
//////				break;
//////			
//////			if (!inCopyExportProvider->CanExportThisFormat(frontDoc, selMgr, InCopy::kInCopyStoryFormatName))
//////				break;
//////
//////			IDFile sysFile;
//////			
//////			// sysFile should be our destination at this point.
//////			const PMString formatName = InCopy::kInCopyStoryFormatName;
//////			inCopyExportProvider->ExportToFile(sysFile, frontDoc, selMgr, formatName, kFullUI);
//////			
//////			break;
//////		}
			case kICBridgeSLSubmitActionID :
			case kICBridgeSLSubmitAllActionID :
			case kICBridgeSubmitAllActionID :
			case kICBridgeSubmitSelActionID :
				this->HandleSubmitAction( ac, actionID, mousePoint, widget );
				break;
//////		case kICBridgeSLSubmitAllActionID:
//////		{
//////			do {
//////				if (!Utils<IAssignmentUIUtils>()->CanSubmitAll(ac))
//////					break;
//////
//////				// Get the application interface and the DialogMgr.
//////				InterfacePtr<IApplication> application(gSession->QueryApplication());
//////				ASSERT(application);
//////				if (application == nil) {
//////					break;
//////				}
//////				
//////				InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
//////				ASSERT(dialogMgr);
//////				if (dialogMgr == nil) {
//////					break;
//////				}
//////				
//////				// Load the plug-in's resource.
//////				PMLocaleId nLocale = LocaleSetting::GetLocale();
//////				RsrcSpec dialogSpec
//////				(
//////				 nLocale,						// Locale index from PMLocaleIDs.h.
//////				 kIZPUIPluginID,				// Our Plug-in ID
//////				 kViewRsrcType,					// This is the kViewRsrcType.
//////				 kIZPCommentDlgRsrcID,			// Resource ID for our dialog.
//////				 kTrue							// Initially visible.
//////				 );
//////				
//////				// CreateNewDialog takes the dialogSpec created above, and also
//////				// the type of dialog being created (kMovableModal).
//////				IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
//////				ASSERT(dialog);
//////				
//////				IControlView* dialogPanel = dialog->GetDialogPanel();
//////				ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
//////				
//////				InterfacePtr<IIZPCommentsDialogData> dialogData (dialogPanel, UseDefaultIID());
//////				ASSERT(dialogData);
//////				
//////				// Open the dialog.
//////				dialog->Open();
//////				dialog->WaitForDialog();
//////				
//////				if (dialogData->GetDialogResult()!=IIZPCommentsDialogData::Ok)
//////					break;
//////				
//////				PMString comment = dialogData->GetComment();
//////				PMString statusid = dialogData->GetStatusId();
//////
//////				/*
//////				InterfacePtr<IInCopyBridgeCmdSuite> myCmdSuite(::CreateObject2<IInCopyBridgeCmdSuite>(kICBridgeBaseCmdBoss));
////////				InterfacePtr<IInCopyBridgeCmdSuite> myCmdSuite(::CreateObject2<IInCopyBridgeCmdSuite>(kICBridgeCmdSuiteBoss));
//////				if(!myCmdSuite)
//////					break;
//////
//////				if (!Utils<IAssignmentUIUtils>()->SubmitAll(myCmdSuite))
//////				{
//////					CAlert::InformationAlert("Submit All failed.");
//////					break;
//////				}
//////				*/
//////				
//////				// somehow Utils<IAssignmentUIUtils>()->SubmitAll(myCmdSuite) is not working, temporarily using selection->SubmitAll
//////				InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////				ASSERT(selection);
//////				if (!selection)
//////					break;
//////				
//////				selection->SubmitAll();
//////				
//////				
//////				// get assignment document
//////				InterfacePtr<IDocument> document(ac->GetContextDocument(), UseDefaultIID());
//////				ASSERT(document);
//////				
//////				IDataBase* db = ::GetDataBase(document);
//////				ASSERT(db);
//////
//////				IAssignedDocumentPtr assignDoc(document, UseDefaultIID());
//////				ASSERT(assignDoc);
//////				
//////				UIDList assignmentList = assignDoc->GetAssignments();
//////				
//////				for (int32 i = 0 ; i < assignmentList.Length(); i++) {
//////					UIDRef assignmentref(db, assignmentList[i]);
//////					
//////					InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
//////					
//////					if (assignment->GetFile().IsEmpty())
//////						continue;
//////					
//////					IDFile file;
//////					FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////					
//////					PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////					if (!IZPServer::CheckInAsset(assetid, statusid, comment)) {
//////						continue;
//////					}
//////					
//////					InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
//////					ASSERT (data);
//////					
//////					{
//////						InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
//////						ASSERT(cmd);
//////						UIDList assignments(::GetUIDRef(assignment));
//////						cmd->SetItemList(assignments);
//////						cmd->SetUndoability(ICommand::kAutoUndo);
//////						
//////						InterfacePtr<IIZPAssignmentData> cmddata(cmd, UseDefaultIID());
//////						cmddata->Copy(data);
//////						cmddata->SetStatusId(statusid);
//////						CmdUtils::ProcessCommand(cmd);
//////					}
//////				}
//////				
//////				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////				ASSERT(assMgr);
//////				assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
//////				
//////				Utils<IIZPUtils>()->UpdateAssetsTreeView();
//////			} while (kFalse);
//////	}
//////			break;
//////		case kICBridgeSubmitAllActionID:
//////		{
//////			do {
//////				InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////				ASSERT(selection);
//////				if (!selection)
//////					break;
//////				
//////				// Get the application interface and the DialogMgr.
//////				InterfacePtr<IApplication> application(gSession->QueryApplication());
//////				ASSERT(application);
//////				if (application == nil) {
//////					break;
//////				}
//////				
//////				InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
//////				ASSERT(dialogMgr);
//////				if (dialogMgr == nil) {
//////					break;
//////				}
//////				
//////				// Load the plug-in's resource.
//////				PMLocaleId nLocale = LocaleSetting::GetLocale();
//////				RsrcSpec dialogSpec
//////				(
//////				 nLocale,						// Locale index from PMLocaleIDs.h.
//////				 kIZPUIPluginID,					// Our Plug-in ID
//////				 kViewRsrcType,					// This is the kViewRsrcType.
//////				 kIZPCommentDlgRsrcID,	// Resource ID for our dialog.
//////				 kTrue							// Initially visible.
//////				 );
//////				
//////				// CreateNewDialog takes the dialogSpec created above, and also
//////				// the type of dialog being created (kMovableModal).
//////				IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
//////				ASSERT(dialog);
//////				
//////				IControlView* dialogPanel = dialog->GetDialogPanel();
//////				ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
//////				
//////				InterfacePtr<IIZPCommentsDialogData> dialogData (dialogPanel, UseDefaultIID());
//////				ASSERT(dialogData);
//////				
//////				// Open the dialog.
//////				dialog->Open();
//////				dialog->WaitForDialog();
//////				
//////				if (dialogData->GetDialogResult()!=IIZPCommentsDialogData::Ok)
//////					break;
//////				
//////				PMString comment = dialogData->GetComment();
//////				PMString statusid = dialogData->GetStatusId();
//////				
//////				selection->SubmitAll();
//////				
//////				// get assignment document
//////				InterfacePtr<IDocument> document(ac->GetContextDocument(), UseDefaultIID());
//////				ASSERT(document);
//////				
//////				IDataBase* db = ::GetDataBase(document);
//////				ASSERT(db);
//////
//////				IAssignedDocumentPtr assignDoc(document, UseDefaultIID());
//////				ASSERT(assignDoc);
//////				
//////				UIDList assignmentList = assignDoc->GetAssignments();
//////		
//////				for (int32 i = 0 ; i < assignmentList.Length(); i++) {
//////					UIDRef assignmentref(db, assignmentList[i]);
//////					
//////					InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
//////					
//////					if (assignment->GetFile().IsEmpty())
//////						continue;
//////					
//////					IDFile file;
//////					FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////					
//////					PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////					if (!IZPServer::CheckInAsset(assetid, statusid, comment)) {
//////						continue;
//////					}
//////					
//////					InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
//////					ASSERT (data);
//////					
//////					{
//////						InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
//////						ASSERT(cmd);
//////						UIDList assignments(::GetUIDRef(assignment));
//////						cmd->SetItemList(assignments);
//////						cmd->SetUndoability(ICommand::kAutoUndo);
//////						
//////						InterfacePtr<IIZPAssignmentData> cmddata(cmd, UseDefaultIID());
//////						cmddata->Copy(data);
//////						cmddata->SetStatusId(statusid);
//////						CmdUtils::ProcessCommand(cmd);
//////					}
//////				}
//////					
//////				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////				ASSERT(assMgr);
//////				assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
//////				
//////				Utils<IIZPUtils>()->UpdateAssetsTreeView();
//////			} while (false);
//////			
//////			break;
//////		}
//////		case kSaveACopyActionID:
//////		{
//////			InterfacePtr<IDocument> document(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
//////			if(!document)
//////				break;
//////			
//////			if (Utils<IIZPUtils>()->IsInCopy()) {
//////				Utils<IDocumentUtils>()->DoSaveACopy(document);
//////			} else if ( Utils<IIZPUtils>()->IsInDesign())
//////			{
//////				Utils<IDocumentUtils>()->DoSaveACopy(document);
//////			}
//////			break;
//////		}
//////			break;
//////		case kICBridgeSLRevertActionID:
//////		{
//////			if (!Utils<IAssignmentUIUtils>()->CanRevertOnSelection(ac))
//////				break;
//////			
//////			InterfacePtr<const IInCopyBridgeCmdSuite> myCmdSuite(::CreateObject2<const IInCopyBridgeCmdSuite>(kICBridgeBaseCmdBoss));
//////			Utils<IAssignmentUIUtils>()->RevertChanges(myCmdSuite);
//////			
//////			break;
//////		}
//////			/*
//////			 case kRevertActionID:
//////			 */
//////		case kICBridgeRevertSelActionID:
//////		{
//////			InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
//////			ASSERT(ac);
//////			
//////			InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////			ASSERT(selection);
//////			
//////			if (!selection)
//////				break;
//////			
//////			if (!selection->CanRevertChanges())
//////				break;
//////			
//////			selection->RevertChanges();
//////		}
//////		break;
//////		case kRevertStoryActionID:
//////		{
//////			InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////			ASSERT(selection);
//////			if (!selection)
//////				break;
//////			
//////			if (!selection->CanRevertChanges())
//////				break;
//////			
//////			selection->RevertChanges();
//////			break;
//////		}
//////		case kSaveAllStoriesActionID:
//////		{
//////			InterfacePtr<IDocument> iDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
//////			if(!iDoc)
//////				break;
//////			
//////			UIDRef docRef = ::GetUIDRef(iDoc);
//////			
//////			// Get the storylist
//////			InterfacePtr<IStoryList> storyList (iDoc, UseDefaultIID());
//////			
//////			// Figure out how many we have
//////			int32 storyCount = storyList->GetUserAccessibleStoryCount();
//////			
//////			bool16 exportParentsOnly = kTrue; //preference?
//////			
//////			// Loop through the storyList
//////			for (int32 i = 0; i < storyCount; i++)
//////
//////			{
//////				UIDRef storyRef = storyList->GetNthUserAccessibleStoryUID(i);
//////
//////				UIDList singleStoryList(storyRef);
//////				
//////				InterfacePtr<IDataLink> iDataLink(Utils<IInCopyBridgeUtils>()->QueryDataLink(storyRef));
//////				if(!iDataLink)
//////					continue;
//////				
//////				InterfacePtr<IManageableAsset> iAsset(iDataLink, UseDefaultIID());
//////				if(!iAsset)
//////					continue;
//////				
//////				InterfacePtr<IAssetData> data(iAsset, UseDefaultIID());
//////				if (!data)
//////					continue;
//////				
//////				InterfacePtr<IManageableAsset> asset(iDataLink, UseDefaultIID());
//////				if(!asset)
//////					continue;
//////				
//////				InterfacePtr<IAMLockable> assetLock( iAsset->GetLockable(), UseDefaultIID() );
//////				if(!assetLock)
//////					continue;
//////				
//////				// currently CheckedOut
//////				if(!assetLock->HasLock())
//////					continue;
//////				
//////				UIDRef assetRef = ::GetUIDRef( asset );
//////
//////				// checkin
//////				Utils<Facade::IWorkgroupFacade>()->CheckIn(assetRef, kTrue);
//////
//////				// checkout
//////				Utils<Facade::IWorkgroupFacade>()->CheckOut(assetRef, kFullUI);
//////
//////			}
//////			
//////			break;
//////		}
//////		case kICBridgeSaveVersionSelActionID:
//////		{
//////			InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////			ASSERT(selection);
//////			if (!selection)
//////				break;
//////			
//////			if (!selection->CanSaveVersion())
//////				break;
//////
//////			selection->Submit();
//////			
//////			selection->EditInPlace();
//////			break;
//////		}
//////		case kICBridgeSLSubmitActionID:
//////		{
//////			if (!Utils<IAssignmentUIUtils>()->CanSubmitOnSelection(ac))
//////				break;
//////			
//////			// Get the application interface and the DialogMgr.
//////			InterfacePtr<IApplication> application(gSession->QueryApplication());
//////			ASSERT(application);
//////			if (application == nil) {
//////				break;
//////			}
//////			
//////			InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
//////			ASSERT(dialogMgr);
//////			if (dialogMgr == nil) {
//////				break;
//////			}
//////			
//////			// Load the plug-in's resource.
//////			PMLocaleId nLocale = LocaleSetting::GetLocale();
//////			RsrcSpec dialogSpec
//////			(
//////			 nLocale,						// Locale index from PMLocaleIDs.h.
//////			 kIZPUIPluginID,					// Our Plug-in ID
//////			 kViewRsrcType,					// This is the kViewRsrcType.
//////			 kIZPCommentDlgRsrcID,	// Resource ID for our dialog.
//////			 kTrue							// Initially visible.
//////			 );
//////			
//////			// CreateNewDialog takes the dialogSpec created above, and also
//////			// the type of dialog being created (kMovableModal).
//////			IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
//////			ASSERT(dialog);
//////			
//////			IControlView* dialogPanel = dialog->GetDialogPanel();
//////			ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
//////			
//////			InterfacePtr<IIZPCommentsDialogData> dialogData (dialogPanel, UseDefaultIID());
//////			ASSERT(dialogData);
//////			
//////			// Open the dialog.
//////			dialog->Open();
//////			dialog->WaitForDialog();
//////			
//////			if (dialogData->GetDialogResult()!=IIZPCommentsDialogData::Ok)
//////				break;
//////			
//////			PMString comment = dialogData->GetComment();
//////			PMString statusid = dialogData->GetStatusId();
//////						
//////			InterfacePtr<IInCopyBridgeCmdSuite> myCmdSuite(::CreateObject2<IInCopyBridgeCmdSuite>(kICBridgeBaseCmdBoss));
//////			Utils<IAssignmentUIUtils>()->Submit(myCmdSuite);
//////						
//////			// update status of selected assignment
//////			UIDRef assignmentRef = GetSelectedAssignmentRef();
//////			
//////			if (assignmentRef==kInvalidUIDRef)
//////				break;
//////			
//////			InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
//////			ASSERT (assignment);
//////			
//////			if (!assignment)
//////				break;
//////			
//////			if (assignment->GetFile().IsEmpty())
//////				break;
//////			
//////			IDFile file;
//////			FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////			
//////			PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////			if (!IZPServer::CheckInAsset(assetid, statusid, comment)) {
//////				CAlert::InformationAlert("Error during CheckInAsset of Assignment");
//////				break;
//////			}
//////			
//////			InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
//////			ASSERT (data);
//////			
//////			{
//////				InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
//////				ASSERT(cmd);
//////				UIDList assignments(::GetUIDRef(assignment));
//////				cmd->SetItemList(assignments);
//////				cmd->SetUndoability(ICommand::kAutoUndo);
//////				
//////				InterfacePtr<IIZPAssignmentData> cmddata(cmd, UseDefaultIID());
//////				cmddata->Copy(data);
//////				cmddata->SetStatusId(statusid);
//////				CmdUtils::ProcessCommand(cmd);
//////			}
//////			
//////			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////			ASSERT(assMgr);
//////			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
//////			
//////			Utils<IIZPUtils>()->UpdateAssetsTreeView();
//////		}
//////			break;
//////		case kICBridgeSubmitSelActionID:
//////		{
//////			InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
//////			ASSERT(selection);
//////			if (!selection)
//////				break;
//////
//////			
//////			// Get the application interface and the DialogMgr.
//////			InterfacePtr<IApplication> application(gSession->QueryApplication());
//////			ASSERT(application);
//////			if (application == nil) {
//////				break;
//////			}
//////			
//////			InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
//////			ASSERT(dialogMgr);
//////			if (dialogMgr == nil) {
//////				break;
//////			}
//////			
//////			// Load the plug-in's resource.
//////			PMLocaleId nLocale = LocaleSetting::GetLocale();
//////			RsrcSpec dialogSpec
//////			(
//////			 nLocale,						// Locale index from PMLocaleIDs.h.
//////			 kIZPUIPluginID,					// Our Plug-in ID
//////			 kViewRsrcType,					// This is the kViewRsrcType.
//////			 kIZPCommentDlgRsrcID,	// Resource ID for our dialog.
//////			 kTrue							// Initially visible.
//////			 );
//////			
//////			// CreateNewDialog takes the dialogSpec created above, and also
//////			// the type of dialog being created (kMovableModal).
//////			IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
//////			ASSERT(dialog);
//////			
//////			IControlView* dialogPanel = dialog->GetDialogPanel();
//////			ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
//////			
//////			InterfacePtr<IIZPCommentsDialogData> dialogData (dialogPanel, UseDefaultIID());
//////			ASSERT(dialogData);
//////			
//////			// Open the dialog.
//////			dialog->Open();
//////			dialog->WaitForDialog();
//////			
//////			if (dialogData->GetDialogResult()!=IIZPCommentsDialogData::Ok)
//////				break;
//////			
//////			PMString comment = dialogData->GetComment();
//////			PMString statusid = dialogData->GetStatusId();
//////			
//////			selection->Submit();
//////			
//////			// update status of selected assignment
//////			UIDRef assignmentRef = GetSelectedAssignmentRef();
//////			
//////			if (assignmentRef==kInvalidUIDRef)
//////				break;
//////			
//////			InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
//////			ASSERT (assignment);
//////			
//////			if (!assignment)
//////				break;
//////			
//////			if (assignment->GetFile().IsEmpty())
//////				break;
//////			
//////			IDFile file;
//////			FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////			
//////			PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////			if (!IZPServer::CheckInAsset(assetid, statusid, comment)) {
//////				CAlert::InformationAlert("Error during CheckInAsset of Assignment");
//////				break;
//////			}
//////			
//////			InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
//////			ASSERT (data);
//////			
//////			{
//////				InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
//////				ASSERT(cmd);
//////				UIDList assignments(::GetUIDRef(assignment));
//////				cmd->SetItemList(assignments);
//////				cmd->SetUndoability(ICommand::kAutoUndo);
//////				
//////				InterfacePtr<IIZPAssignmentData> cmddata(cmd, UseDefaultIID());
//////				cmddata->Copy(data);
//////				cmddata->SetStatusId(statusid);
//////				CmdUtils::ProcessCommand(cmd);
//////			}
//////			
//////			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////			ASSERT(assMgr);
//////			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
//////			
//////			Utils<IIZPUtils>()->UpdateAssetsTreeView();
//////		}
//////			
//////			break;
//////		
//////		case kSLAssignmentOptionsActionID:
//////		{
//////			UIDRef assignmentRef = GetSelectedAssignmentRef();
//////			
//////			if (assignmentRef==kInvalidUIDRef)
//////				break;
//////			
//////			InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
//////			ASSERT (assignment);
//////
//////			InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
//////			ASSERT (data);
//////
//////			IDocument * doc = ac->GetContextDocument();
//////			IDataBase *db = ::GetDataBase(doc);
//////			
//////			InterfacePtr<IApplication> application(gSession->QueryApplication());
//////			ASSERT(application);
//////			if (application == nil) {
//////				break;
//////			}
//////			
//////			// Get the application interface and the DialogMgr.
//////			InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
//////			ASSERT(dialogMgr);
//////			
//////			// Load the plug-in's resource.
//////			PMLocaleId nLocale = LocaleSetting::GetLocale();
//////			
//////			RsrcSpec dialogSpec
//////			(
//////			 nLocale,						// Locale index from PMLocaleIDs.h.
//////			 kIZPUIPluginID,					// Our Plug-in ID
//////			 kViewRsrcType,					// This is the kViewRsrcType.
//////			 kIZPAssignmentDialogResourceID,	// Resource ID for our dialog.
//////			 kTrue							// Initially visible.
//////			 );
//////			
//////			PMString assetid = data->GetId();
//////			
//////			// CreateNewDialog takes the dialogSpec created above, and also
//////			// the type of dialog being created (kMovableModal).
//////			IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
//////			ASSERT(dialog);
//////			
//////			IControlView* dialogPanel = dialog->GetDialogPanel();
//////			ASSERT(dialogPanel);
//////			
//////			InterfacePtr<IIZPAssignmentDialogData> dialogdata(dialogPanel, UseDefaultIID());
//////			dialogdata->SetDialogResult(IIZPAssignmentDialogData::Cancel);
//////			
//////			dialogdata->SetName(data->GetName());
//////			dialogdata->SetTitleId(data->GetTitleId());
//////			dialogdata->SetTitle(data->GetTitle());
//////			dialogdata->SetEditionId(data->GetShelveId());
//////			dialogdata->SetEdition(data->GetShelve());
//////			dialogdata->SetStatusId(data->GetStatusId());
//////			dialogdata->SetStatus(data->GetStatus());
//////			
//////			InterfacePtr<IUIDData> colorData(dialogPanel, UseDefaultIID());
//////			ASSERT(colorData);
//////			
//////			colorData->Set(db, assignment->GetColor());
//////			
//////			// Open the dialog.
//////			dialog->Open();
//////			dialog->WaitForDialog();
//////			
//////			if (dialogdata->GetDialogResult()!=IIZPAssignmentDialogData::Ok)
//////				break;
//////			
//////			PMString name = dialogdata->GetName();
//////			PMString description = dialogdata->GetDescription();
//////			PMString shelveid = dialogdata->GetEditionId();
//////			PMString shelve = dialogdata->GetEdition();
//////			PMString titleid = dialogdata->GetTitleId();
//////			PMString title = dialogdata->GetTitle();
//////			PMString statusid = dialogdata->GetStatusId();
//////			PMString status = dialogdata->GetStatus();
//////			
//////			UID color = colorData->Get();
//////			
//////			InterfacePtr<IIZPSessionData> sessionData(gSession, UseDefaultIID());
//////			ASSERT(sessionData);
//////			
//////			IZPService& service = sessionData->GetService();
//////			
//////			{
//////				InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kAssignSetPropsCmdBoss));
//////				ASSERT(cmd);
//////				UIDList list(::GetUIDRef(assignment));
//////				cmd->SetItemList(list);
//////				InterfacePtr<IAssignSetPropsCmdData> cmddata(cmd, UseDefaultIID());
//////				
//////				bool16 dirty = kTrue;
//////				cmddata->Set(&name /* name */, nil /* file */, nil /* user */, &color /* color */, nil /* options */, &dirty /* dirty */, nil /* includeLinkedWhenPackage */);
//////				CmdUtils::ProcessCommand(cmd);
//////			}
//////			
//////			{
//////				InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
//////				ASSERT(cmd);
//////				UIDList assignments(assignmentRef);
//////				cmd->SetItemList(assignments);
//////				cmd->SetUndoability(ICommand::kAutoUndo);
//////
//////				InterfacePtr<IIZPAssignmentData> cmddata(cmd, UseDefaultIID());
//////				cmddata->SetId(assetid);
//////				cmddata->SetName(name);
//////				cmddata->SetDescription(description);
//////				cmddata->SetShelve(shelve);
//////				cmddata->SetShelveId(shelveid);
//////				cmddata->SetTitle(title);
//////				cmddata->SetTitleId(titleid);
//////				cmddata->SetStatus(status);
//////				cmddata->SetStatusId(statusid);
//////				CmdUtils::ProcessCommand(cmd);
//////			}
//////			break;
//////		}
//////		case kIZPApplyTagsActionID:
//////		{
//////			DoApplyTags();
//////			break;
//////		}
//////		case kSLRemoveActionID:
//////		{
//////			do {
//////				AcquireWaitCursor busyCursor;
//////
//////				UIDRef assignmentRef = GetSelectedAssignmentRef();
//////				
//////				if (assignmentRef == kInvalidUIDRef)
//////					break;
//////
//////				InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
//////				ASSERT (assignment);
//////
//////				IDFile file;
//////				FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////				PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////				
//////				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////				ASSERT(assMgr);
//////				
//////				InterfacePtr<IDocument> doc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
//////				ASSERT(doc);
//////				
//////				InterfacePtr<IAssignedDocument> assignmentDoc(doc, UseDefaultIID());
//////				ASSERT(assignmentDoc);
//////				
//////				InterfacePtr<ICommand> unassignDocCmd(CmdUtils::CreateCommand(kUnassignDocCmdBoss));
//////				UIDList uidl(::GetUIDRef(assignmentDoc));
//////				unassignDocCmd->SetItemList(uidl);
//////				InterfacePtr<IStringData> sd(unassignDocCmd, UseDefaultIID());
//////				sd->Set(assignment->GetFile());
//////				ErrorCode result = CmdUtils::ProcessCommand(unassignDocCmd);
//////								
//////				assMgr->NotifyPanel();
//////				
//////				this->UpdateAssetsTreeView();
//////
//////			} while (kFalse);
//////		}
//////			break;
//////		case kSLRelinkAssignmentActionID:
//////		{
//////			CAlert::InformationAlert("kSLRelinkAssignmentActionID");
//////		}
//////			break;
//////		case kSLUpdateSelectedAssignmentsActionID:
//////		{
//////			do {
//////				AcquireWaitCursor busyCursor;
//////
//////				UIDRef assignmentRef = GetSelectedAssignmentRef();
//////				
//////				if (assignmentRef==kInvalidUIDRef)
//////					break;
//////				
//////				InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
//////				ASSERT (assignment);
//////				
//////				IDFile file;
//////				FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////				PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////				if (!assetid.IsEmpty()) {
//////					Utils<IIZPUtils>()->UpdateAssignment(assignment);
//////				}
//////				
//////				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////				assMgr->NotifyPanel();
//////				
//////				this->UpdateAssetsTreeView();
//////			} while (kFalse);
//////		}
//////			break;
//////		case kSLUpdateAllAssignmentsActionID:
//////		{
//////				do {
//////				AcquireWaitCursor busyCursor;
//////
//////				InterfacePtr<IDocument> document(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
//////				if(!document)
//////					break;
//////				
//////				IDataBase* db = ::GetDataBase(document);
//////				ASSERT(db);
//////				
//////				InterfacePtr<IAssignedDocument> iad(document, UseDefaultIID());
//////				ASSERT(iad);
//////				
//////				InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////				ASSERT(assMgr);
//////				
//////				UIDList assignmentList = iad->GetAssignments();
//////				for (int32 i = 0 ; i < assignmentList.Length(); i++) {
//////					UIDRef assignmentref(db, assignmentList[i]);
//////					
//////					InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
//////					ASSERT(assignment);
//////					
//////					IDFile file;
//////					FileUtils::PMStringToIDFile(assignment->GetFile(), file);
//////					PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
//////					if (!assetid.IsEmpty()) {
//////						Utils<IIZPUtils>()->UpdateAssignment(assignment);
//////					}
//////				}
//////				
//////				assMgr->NotifyPanel();
//////				
//////				this->UpdateAssetsTreeView();
//////				} while (false);
//////		}
//////		break;
//////		case kIZPCheckOutAllActionID:
//////		{
//////		
//////			InterfacePtr<IDocument> iDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
//////			if(!iDoc)
//////				break;
//////			
//////			UIDRef docRef = ::GetUIDRef(iDoc);
//////			
//////			// Get the storylist
//////			InterfacePtr<IStoryList> storyList (iDoc, UseDefaultIID());
//////			
//////			// Figure out how many we have
//////			int32 storyCount = storyList->GetUserAccessibleStoryCount();
//////			
//////			TaskProgressBar bar("Check Out", storyCount, kTrue, kFalse, nil, nil);
//////
//////
//////			// Loop through the storyList
//////			for (int32 i = 0; i < storyCount; i++)
//////				
//////			{
//////				UIDRef storyRef = storyList->GetNthUserAccessibleStoryUID(i);
//////				
//////				UIDList singleStoryList(storyRef);
//////				
//////				InterfacePtr<IDataLink> iDataLink(Utils<IInCopyBridgeUtils>()->QueryDataLink(storyRef));
//////				if(!iDataLink)
//////					continue;
//////				
//////				InterfacePtr<IAssetData> data(iDataLink, UseDefaultIID());
//////				if(!data)
//////					continue;
//////				
//////				InterfacePtr<IManageableAsset> asset(iDataLink, UseDefaultIID());
//////				if(!asset)
//////					continue;
//////				
//////				bar.DoTask(data->GetId());
//////
//////				Utils<Facade::IWorkgroupFacade>()->CheckOut(::GetUIDRef(asset), kFullUI);
//////
//////			}
//////			
//////			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
//////			ASSERT(assMgr);
//////			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
//////			
//////		}
//////			break;
		case kSaveActionID:
		{
			if( isLoggedIn && Utils<IZPInDnUtils>()->IsHostAppInCopy())
				this->DoDocSave( ac, actionID, mousePoint, widget );
			else
				this->DoDefaultAction( ac, actionID, mousePoint, widget ); //do default action handler.
			break;
		}
		case kSaveAsActionID:
		{
			ErrorCode err = kFailure;
			IDocument * doc = ac->GetContextDocument();
			if( isLoggedIn && Utils<IZPInDnUtils>()->IsHostAppInCopy())
				this->DoDocSaveAs( ac, actionID, mousePoint, widget );
			else if (isLoggedIn && Utils<IZPInDnUtils>()->IsHostAppInDesign() && Utils<IZPAssetDocUtils>()->IsOurAssetDocument(doc) == false)
			{				
#if defined(InDnCS5) || defined(InDnCS5_5)			
				// map to store all assignments and corresponding stories
				ZPAssgStoriesMap assgStoriesMap;
				bool16 bIsConverted = kFalse;				

				bIsConverted = doc->IsConverted();

				// get exportable old assets in assgStoriesMap
				if( Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( doc ) && bIsConverted ) // if not template
					this->GetExportableAssets( ac, assgStoriesMap );
#endif							
				// this will remove all existing stories and assignments
				err = Utils<IZPAMHelper>()->ProcessDocSaveAs_FromLocalDocument( doc );				

#if defined(InDnCS5) || defined(InDnCS5_5)
				// re-export all assets
				if( Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( doc ) && bIsConverted && err == kSuccess ) // if not template				
					this->ReExportAssets( ac, assgStoriesMap );				
#endif
			}
			else
				this->DoDefaultAction( ac, actionID, mousePoint, widget ); //do default action handler.
		}
			break;
		//////case kUpdateDesignActionID:
		//////	Utils<IInCopyDocUtils>()->DoUpdateDesign(::GetUIDRef(Utils<ILayoutUIUtils>()->GetFrontDocument()));
		//////	break;
		case kIZPCheckoutAllAssignmentStoriesActionID:
		{
			Utils<IZPDocUtils>()->CheckOutAllStories(ac->GetContextDocument());
			break;
		}
#if defined(InDnCS5) || defined(InDnCS5_5)
		case kICBridgeRelinkSelActionID:
		{
			//Patch for Update Content in IC: Linked story asset in IC does not get updated
			if(isLoggedIn && Utils<IZPInDnUtils>()->IsHostAppInCopy())
			{	
				UIDRef modelRef = kInvalidUIDRef;
				TextIndex activeStart, activeEnd;				
				Utils<IGalleyUtils>()->GetCurrentTextSelection(activeStart, activeEnd, &modelRef);
				
				if(modelRef)
				{
					// Get latest version of linked story from repository 
					UIDRef assetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef(modelRef);				
					Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(assetRef);

					IDataBase* db = modelRef.GetDataBase();
					UIDList storyList(db), links(db);
					storyList.push_back(modelRef.GetUID());

					InterfacePtr<ILinkManager> linkMgr(db, db->GetRootUID(), UseDefaultIID());
					linkMgr->QueryLinksByObjectUID(modelRef.GetUID(), links);

					for(int32 i = 0; i < links.Length(); i++)
					{
						UID newLink;
						Utils<Facade::ILinkFacade>()->UpdateLink(links.GetRef(i), true, kSuppressUI, newLink);
					}
				}								
			}
			this->DoDefaultAction( ac, actionID, mousePoint, widget ); //do default action handler.
			break;
		}
		case kSLAddSelToNewAssignmentTextContextActionID:
		{			
			this->DoDefaultAction( ac, actionID, mousePoint, widget ); //do default action handler.
			break;
		}
#endif
		default:
		{
			break;
		}
	}
}

//----------------------------------------------------------------------------------------
// DoDefaultAction
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoDefaultAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
	InterfacePtr<const IZPUIActionFilterData> zpActionFilterData( iWorkspace, UseDefaultIID() );
	ASSERT(zpActionFilterData);

	ClassID oldHandler = zpActionFilterData->GetOldHandlerForAction( actionID );
	if( oldHandler.IsValid() )
	{
		InterfacePtr<IActionComponent> oldActionComponent( ::CreateObject2<IActionComponent>(oldHandler, IActionComponent::kDefaultIID ));
		if( oldActionComponent )
			oldActionComponent->DoAction( ac, actionID, mousePoint, widget );
	}
}

//----------------------------------------------------------------------------------------
// UpdateDefaultActionStates
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::UpdateDefaultActionStates(
	int					inActionIndex,
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
	InterfacePtr<const IZPUIActionFilterData> zpActionFilterData( iWorkspace, UseDefaultIID() );
	ASSERT(zpActionFilterData);

	ActionID actionID = listToUpdate->GetNthAction( inActionIndex );
	ClassID oldHandler = zpActionFilterData->GetOldHandlerForAction( actionID );
	if( oldHandler.IsValid() )
	{
		InterfacePtr<IActionComponent> oldActionComponent( ::CreateObject2<IActionComponent>(oldHandler, IActionComponent::kDefaultIID ));
		if( oldActionComponent )
		{
			CActionStateList newList;
			newList.AddAction( listToUpdate->GetNthAction(inActionIndex), listToUpdate->GetNthActionName( inActionIndex ), listToUpdate->GetNthActionState( inActionIndex ) );
			//newList.SetNthActionName( 0, listToUpdate->GetNthActionName( inActionIndex ) );
			//newList.SetNthActionState( 0, listToUpdate->GetNthActionState( inActionIndex ) );
			oldActionComponent->UpdateActionStates( ac, &newList, mousePoint, widget );
			listToUpdate->SetNthActionName( inActionIndex, newList.GetNthActionName( 0 ) );
			listToUpdate->SetNthActionState( inActionIndex, newList.GetNthActionState( 0 ) );
		}
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanUpdateAssetStatus
//----------------------------------------------------------------------------------------
bool
CZPAMActionComponent::CanUpdateAssetStatus()
{
	bool toReturn = false;
	do
	{
		const IStringData * dbAssetIDToUpdate = this->GetSelectedAsset();
		if( !dbAssetIDToUpdate )
			break;

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetIDToUpdate, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		
		const PMString & theDocAssetIDStr = dbAssetInfo->GetLinkedWithDocumentID();
		
		if( theDocAssetIDStr.IsEmpty() )
			break;

		IDocument* currentDoc = Utils<IZPAssetDocUtils>()->GetDocumentForDocAssetID( theDocAssetIDStr );
		if( !currentDoc )
			break;
		toReturn = true;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUpdateAssetStatus
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoUpdateAssetStatus()
{
	LogFunctionEnterExit;

	do
	{
		const IStringData * dbAssetIDToUpdate = this->GetSelectedAsset();
		if( !dbAssetIDToUpdate )
			break;

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetIDToUpdate, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		
		const PMString & theDocAssetIDStr = dbAssetInfo->GetLinkedWithDocumentID();
		
		if( theDocAssetIDStr.IsEmpty() )
			break;

		//Ask for check-in comments
		IDFile assignmentFile;
		if( Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetIDToUpdate, assignmentFile ) )
		{
			InterfacePtr<IDocument> iFrontDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
			if(!iFrontDoc)
				break;

			InterfacePtr<IAssignment> theAssignment( Utils<IZPDocUtils>()->QueryAssignmentForFilePath( iFrontDoc, assignmentFile) );
			if( !theAssignment )
				break;

			PMString checkInComments;
			UIDRef assignmentUIDRef = ::GetUIDRef( theAssignment );

			IAMUIService::FollowupAction followupAction = Utils<IZPAMHelper>()->AskVersionComments_Assignment( assignmentFile, assignmentUIDRef, checkInComments );
			if(followupAction != IAMUIService::enCancel && followupAction != IAMUIService::enNone )
			{
				InterfacePtr<const IZPAssetCheckInData> assetCheckInData( theAssignment, UseDefaultIID());
				ASSERT(assetCheckInData);
				InterfacePtr<const IZPAssetData> assetData( theAssignment, UseDefaultIID());
				ASSERT(assetData);
				do
				{
					const PMString & statusID = assetCheckInData->GetStatusID();
					if( statusID.IsEmpty() )
						return;
					
					const PMString & assetName = assetCheckInData->GetAssetName();

					const PMString & theVersionID = dbAssetInfo->GetVersionID();

					#pragma message("//TODO: pass old version number, so that server can compare it with latest/head.")
					//TODO: pass checkin comments
					ErrorCode toReturn = Utils<IZPAMWSUtils>()->UpdateAsset_sync( assetData->GetAssetID(), assetName, assetData->GetEditionID(), theVersionID, statusID );
					if( toReturn != kSuccess )
					{
						ErrorUtils::PMSetGlobalErrorCode(kZPAssignmentUpdatingError);
						break;
					}
				}while(kFalse);
			}
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetSelectedAsset
//----------------------------------------------------------------------------------------
const IStringData *
CZPAMActionComponent::GetSelectedAsset()const
{
	const IStringData * toReturn = nil;
	do
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<IApplication> application(gSession->QueryApplication());
		ASSERT(application);
		
		InterfacePtr<IPanelMgr> panelMgr(application->QueryPanelManager());
		if( !panelMgr )
			break;
		
		if( !panelMgr->IsPanelWithWidgetIDShown( kZPUIAssetsPanelWidgetID ) )
			break;

		IControlView* assetsPanelView = panelMgr->GetPanelFromWidgetID( kZPUIAssetsPanelWidgetID );
		if( !assetsPanelView )
			break;

		InterfacePtr<const IZPAssetsPanelHelper> assetPanelHelper( assetsPanelView, UseDefaultIID() );
		ASSERT( assetPanelHelper );

		toReturn = assetPanelHelper->GetSelectedAsset();

	}while(kFalse);
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleSubmitAction
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::HandleSubmitAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IDocument * theTargetDoc = ac->GetContextDocument();

	bool doDefaultAction = true;
	IDataBase *docDB = ::GetDataBase(theTargetDoc);
	const IDFile *docFile = docDB->GetSysFile();
	if (docFile)
	{
		PMString assetIDStr = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( *docFile );
		if( assetIDStr.IsEmpty() == kFalse )
		{
			const IStringData * docDBAssetId = ZPDataHelper::GetAssetByID( assetIDStr );
			if (docDBAssetId)
			{
				doDefaultAction = false;
				InterfacePtr<const IZPAssetInfo> dbAssetInfo( docDBAssetId, UseDefaultIID() );
				ASSERT( dbAssetInfo );
				enAssetType assetType = dbAssetInfo->GetType();
				//Patch: After first check-in (save) of InCopy document/template this action is coming, which does not come if the same
				//Document/Tempalte is opened again.
				if (assetType == eAssetType_IDStory || assetType == eAssetType_ICDocument || assetType == eAssetType_ICTemplate)
					doDefaultAction = true;
			}
		}
	}

	if( doDefaultAction )
	{
		this->DoDefaultAction( ac, actionID, mousePoint, widget );
		return;
	}
	
	//Show UI for assignment status change.
	InterfacePtr<IManageableAsset> docManagableAsset( theTargetDoc, UseDefaultIID() );
	InterfacePtr<IAMUIService> amUIService( Utils<IZPAMHelper>()->QueryAMUIService(docManagableAsset, (IAMUIServiceProvider::UIService)enAMUIUpdateVersionStatusService ) );
	if( !amUIService )
	{
		this->DoDefaultAction( ac, actionID, mousePoint, widget );
		return;
	}
	ErrorCode serviceReturn = amUIService->Do( docManagableAsset );

	//Save values with gSession.
	InterfacePtr<IAMServiceData> amServiceData(amUIService, UseDefaultIID());
	ASSERT(amServiceData);
	IAMUIService::FollowupAction userAction = IAMUIService::enCancel;
	amServiceData->Get(IAMUIService::enFollowupAction, userAction);
	if( userAction != IAMUIService::enSave )
		return;	//User cancelled the action, do nothing.

	PMString newStatusID;
	amServiceData->Get( kZPAMServiceDataStatusID, newStatusID );
	//amServiceData->Set( enVersionComment, dlgAssetCheckInData->GetDescription() );

	//Set hardcode values
	InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
	ASSERT( gTempData );
	gTempData->SetAssignmentNewStatusID( newStatusID );	//This will result in assignment status change after story check-in
	this->DoDefaultAction( ac, actionID, mousePoint, widget );
	gTempData->SetAssignmentNewStatusID( kNullString );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// DoDocSave
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoDocSave(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	do
	{
		IDocument * theDocument = ac->GetContextDocument();
		ASSERT(theDocument);

		if( theDocument )
		{
			bool isNewDocument = theDocument->IsSaved() == kFalse;
			{
				InterfacePtr<const IBoolData> isTemplateCopy( theDocument, IID_IZPISTEMPLATECOPY );
				ASSERT(isTemplateCopy);
				if( !isNewDocument )
					isNewDocument = isTemplateCopy->Get();
			}

			if( isNewDocument )
				Utils<IZPAMHelper>()->ProcessAction_DocSave( theDocument );
			else
				this->DoDefaultAction( ac, actionID, mousePoint, widget );
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// DoDocSaveAs
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoDocSaveAs(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	do
	{
		IDocument * theDocument = ac->GetContextDocument();
		ASSERT(theDocument);

		if( theDocument )
		{
			bool isNewDocument = theDocument->IsSaved() == kFalse;
			{
				InterfacePtr<const IBoolData> isTemplateCopy( theDocument, IID_IZPISTEMPLATECOPY );
				ASSERT(isTemplateCopy);
				if( !isNewDocument )
					isNewDocument = isTemplateCopy->Get();
			}

			if( isNewDocument )
				Utils<IZPAMHelper>()->ProcessAction_DocSave( theDocument );
			else
				this->DoDefaultAction( ac, actionID, mousePoint, widget );
		}
	} while (kFalse);
}


#pragma mark -
#if 0 //TODO: RJK
//----------------------------------------------------------------------------------------
// GetSelectedAssignmentRef
//----------------------------------------------------------------------------------------
UIDRef
CZPAMActionComponent::GetSelectedAssignmentRef()
{
	LogFunctionEnterExit;
	
	UIDRef assignmentRef = kInvalidUIDRef;

	do {
		InterfacePtr<const IApplication> iApp(gSession->QueryApplication());
		ASSERT(iApp);

		InterfacePtr<const IPanelMgr> panelMgr(iApp->QueryPanelManager());
		ASSERT(panelMgr);
		
		IControlView* ptrPanelView = panelMgr->GetPanelFromWidgetID(kStoryListPanelWidgetID);
		ASSERT(ptrPanelView);
		
		if (!ptrPanelView)
			break;
		
		InterfacePtr<const IPanelControlData> panelControlData(ptrPanelView, UseDefaultIID());
		ASSERT(panelControlData);

		if (!panelControlData)
			break;
		
		InterfacePtr<const ITreeViewController> tvc(panelControlData->FindWidget(kStoryListTreeViewWidgetID ), UseDefaultIID());
		ASSERT (tvc);

		if (!tvc)
			break;

		NodeIDList selectedItems;
		tvc->GetSelectedItems(selectedItems);
		ASSERT(selectedItems.size());
		
		if (selectedItems.size()==0)
			break;
		
		TreeNodePtr<StoryListNodeID> nodeID(selectedItems[0]);
		ASSERT(nodeID);
		if (!nodeID)
			break;
		
		if (nodeID->GetStoryListType()!=StoryListNodeID::kAssignmentNodeType)
			break;

		assignmentRef = nodeID->GetUIDRef();

	} while (kFalse);
	
	return (assignmentRef);
}

//----------------------------------------------------------------------------------------
// DeleteAssignment
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMActionComponent::DeleteAssignment(
	IAssignment * assignment)
{
	LogFunctionEnterExit;
	
	ErrorCode result = kFailure;
	do {
		
 		if (!assignment)
		{
			SNIPLOG("No assignment available for delete");
			break;
		}
		
		IDataBase *db = ::GetDataBase(assignment);
		PMString fromAssignPath	= assignment->GetFile();
		IAssignedDocumentPtr assignDoc(db, db->GetRootUID(), UseDefaultIID());
		
		// getfile verplicht?
		
		// remove assignment from document
		InterfacePtr<ICommand> unassignDocCmd(CmdUtils::CreateCommand(kUnassignDocCmdBoss));
		UIDList uidl(::GetUIDRef(assignDoc));
		unassignDocCmd->SetItemList(uidl);
		InterfacePtr<IStringData> sd(unassignDocCmd, UseDefaultIID());
		sd->Set(fromAssignPath);
		result = CmdUtils::ProcessCommand(unassignDocCmd);
		if (result == kSuccess)
			SNIPLOG("Assignment deleted.");
		
		InterfacePtr<const IAssignmentMgr> assMgr(gSession, UseDefaultIID());
		assMgr->NotifyPanel();
		
	} while(false);
	return result;
}

//----------------------------------------------------------------------------------------
// DoApplyTags
//----------------------------------------------------------------------------------------
void
CZPAMActionComponent::DoApplyTags()
{
	LogFunctionEnterExit;
	
	do	{
		InterfacePtr<IDocument> iDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
		ASSERT(iDoc);
		
		UIDRef documentUIDRef(::GetUIDRef(iDoc));
		
		InterfacePtr<IDocument> document(documentUIDRef, UseDefaultIID());
		if(!document) {
			break;
		}
		InterfacePtr<IXMLTagList> tagList(Utils<IXMLUtils>()->QueryXMLTagList(documentUIDRef.GetDataBase()));
		ASSERT(tagList);
		if(!tagList) {
			break;
		}
		
		
		UIDRef retval;
		
		InterfacePtr<IIZPSessionData> sessionData(gSession, UseDefaultIID());
		ASSERT(sessionData);
		
		IZPService& service = sessionData->GetService();
		_ns3__GetTags getTagsRequest;
		_ns3__GetTagsResponse getTagsResponse;
		service.GetTags(&getTagsRequest, &getTagsResponse);
		
		if (getTagsResponse.GetTagsResult)
		{
			for (int i=0; i<getTagsResponse.GetTagsResult->__sizestring; i++)
			{
				PMString tagName = WideString(getTagsResponse.GetTagsResult->string[i]);
				
				
				UID existingTagUID  = tagList->GetTag(tagName);
				if(existingTagUID == kInvalidUID) {
					
					UID createdTagUID = kInvalidUID;
					ErrorCode err = Utils<IXMLTagCommands>()->CreateTag (::GetUIDRef(tagList),
																		 tagName,
																		 kInvalidUID,
																		 &createdTagUID);
					
					ASSERT(err == kSuccess);
					ASSERT(createdTagUID != kInvalidUID);
					retval = UIDRef(::GetDataBase(tagList), createdTagUID);
					
				} else {
					retval = UIDRef(::GetDataBase(tagList), existingTagUID);
				}
				
			}
		}
		
	} while (kFalse);
}
#endif //RJK:TODO

//----------------------------------------------------------------------------------------
// CanCheckoutAllStories
//----------------------------------------------------------------------------------------
bool
CZPAMActionComponent::CanCheckoutAllStories(
	IActiveContext*					ac,
	GSysPoint						mousePoint,
	IPMUnknown*						widget)
{
	bool result = false;
	do
	{
		//InDesign Only
		if (Utils<IZPInDnUtils>()->IsHostAppInCopy())
			break;

		//Commented for Redmine #1983
		//if (Utils<IAssignmentUIUtils>()->CanEditOnSelection (ac))
		//	break;

		InterfacePtr<const IDocument> doc(ac->GetContextDocument(), UseDefaultIID());
		ASSERT(doc);
		if (!doc)
			break;

		if (Utils<IZPDocUtils>()->CanCheckOutAnyStory(doc) == false)
			break;

		result = true;
	}while(false);

	return result;
}

#if defined(InDnCS5) || defined(InDnCS5_5)
// get all exportable assets for re-exporting
void
CZPAMActionComponent::GetExportableAssets(
	IActiveContext*			ac,
	ZPAssgStoriesMap&		list)
{
	IDocument* doc = ac->GetContextDocument();	
	UIDList storyList( doc );	
	UIDList pageItemUIDs( doc );	

	InterfacePtr<IAssignedDocument> assgDoc( doc, UseDefaultIID() );
	ASSERT( assgDoc );

	UIDList assgList( doc );	
	assgList = assgDoc->GetAssignments();	

	for( int32 k = 0; k < assgList.Length(); k++ )
	{	
		storyList.Clear();
		pageItemUIDs.Clear();

		InterfacePtr<IAssignment> assgn( assgList.GetRef( k ), UseDefaultIID() );
		ASSERT( assgn );

		if( assgn->GetCompatibility() != IAssignment::kUnassignedCompatibility )
		{
			IDFile idFile;
			const PMString& filePath = assgn->GetFile();
			Utils<IZPFileUtils>()->ConvertStrToIDFile(idFile, filePath);

			if( !Utils<IZPAMHelper>()->IsPathOfAnyReplicaFolder( idFile ) )
				continue;
		}	

		storyList = assgn->GetStories();

		//get UID of all page items corresponding to stories in assignment
		for( int32 i = 0; i < storyList.Length(); i++ )
		{
			InterfacePtr<IAssignedStory> iAssignedStory( storyList.GetDataBase(), storyList[i], UseDefaultIID ());
			ASSERT( iAssignedStory );

			/*IDFile file;
			const PMString& storyPath = iAssignedStory->GetFile();
			Utils<IZPFileUtils>()->ConvertStrToIDFile(file, storyPath);

			if( !Utils<IZPAMHelper>()->IsPathOfAnyReplicaFolder( file ) )
				continue;*/

			ClassID classID = GetClass( iAssignedStory );

			if( classID == kAssignedStoryBoss) // if text story
			{					
				InterfacePtr<ITextModel> model( iAssignedStory->GetUIDRef(), UseDefaultIID() );			
				ASSERT( model ); 	

				InterfacePtr<IFrameList> frameList( model->QueryFrameList() );
				ASSERT( frameList );

				InterfacePtr<ITextFrameColumn> col( frameList->QueryNthFrame( 0 ) );
				ASSERT( col );

				InterfacePtr<IHierarchy> hier( col, UseDefaultIID() );
				ASSERT( hier );						

				InterfacePtr<IHierarchy> parentHier( hier->QueryParent() );
				ASSERT( parentHier );

				pageItemUIDs.push_back( parentHier->GetParentUID() );	 
			}
			else if (classID == kAssignedImageBoss) // if image item
			{
				pageItemUIDs.push_back( iAssignedStory->GetUIDRef().GetUID() );	
			}
		}// end for loop

		if( assgn->GetCompatibility() == IAssignment::kUnassignedCompatibility )
			list[PMString(kIZPUnAssignedICContent)] = pageItemUIDs;		
		else
			list[assgn->GetName()] = pageItemUIDs;		
	}// end for loop
}

// re-export all assets obtained in map
ErrorCode
CZPAMActionComponent::ReExportAssets(
	IActiveContext*				ac,
	ZPAssgStoriesMap&			list )
{
	ErrorCode result = kSuccess;
	ZPAssgStoriesMapIter assgStoriesMapIter;

	InterfacePtr<IApplication> app( GetExecutionContextSession()->QueryApplication() );
	ASSERT( app );

	IDocument* doc = ac->GetContextDocument();
	ASSERT( doc );

	if( !doc )
		return kFailure;

	// Display status of re-export of assets
	TaskProgressBar progress( PMString( kIZPAssetConversionKey ), std::distance( list.begin(), list.end() ), kFalse, kFalse );

	for( assgStoriesMapIter = list.begin(); assgStoriesMapIter != list.end(); assgStoriesMapIter++ )
	{	
		bool bUnAssigned = false;
		PMString assgName = assgStoriesMapIter->first;
		PMString unassignedIC(kIZPUnAssignedICContent);
		IAssignment* assignment = nil;

		PMString display(kIZPExportAssignmentKey);
		display.Translate();
		display.Append( " " );
		display.AppendNumber( std::distance( list.begin(), assgStoriesMapIter ) + 1 );
		display.Append(" of ");
		display.AppendNumber( std::distance( list.begin(), list.end() ) );
		display.SetTranslatable(kFalse);
		progress.DoTask( display );

		if( assgName.IsEqual( unassignedIC ) )
			bUnAssigned = true;		

		if( !bUnAssigned ) // skip creation of assignment in case of Unassigned Incopy content
		{
			assgName.Append( app->GetUIVersionString() ); // append UI version string CS5, CS5.5, etc., to assg name
			assignment = Utils<IZPAssetDocUtils>()->CreateAssignment( doc, assgName, false );	
		}	
		
		int32 numStories = (assgStoriesMapIter->second).Length(); 
		
		// auto select and export each story
		for( int32 i = 0; i < numStories; i++ )
		{  
			PMString taskStatus( kIZPExportStoriesKey );
			taskStatus.Translate();
			taskStatus.Append( " " );
			taskStatus.AppendNumber( i + 1 );
			taskStatus.Append( " of " );
			taskStatus.AppendNumber( numStories );
			taskStatus.SetTranslatable( kFalse );
			progress.SetTaskStatus( taskStatus );

			UIDList pageItem( doc );
			pageItem.Clear();
			pageItem.push_back( (assgStoriesMapIter->second).GetRef( i ).GetUID() );

			InterfacePtr<ISelectionManager> selMgr( Utils<ISelectionUtils>()->QueryActiveSelection() );
			ASSERT( selMgr );		

			InterfacePtr<ILayoutSelectionSuite> selSuite(selMgr, UseDefaultIID() );
			ASSERT( selSuite );
			
			selSuite->DeselectAllPageItems(); // if any items are already selected
			selSuite->SelectPageItems( pageItem, Selection::kReplace, Selection::kScrollIntoViewIfNecessary );

			const PMString formatName = InCopy::kInCopyMarkupFormatName;			

			InterfacePtr<IExportProvider> selectionMgrExporter( selMgr, IID_IINCOPYEXPORTSUITE );
			ASSERT( selectionMgrExporter );			

			if( selectionMgrExporter->CanExportThisFormat( doc, selMgr, formatName ) )
			{
				IDFile sysFile = Utils<IZPAMHelper>()->CreateNewStoryAsset( doc );
				selectionMgrExporter->ExportToFile( sysFile, doc, selMgr, formatName, kSuppressUI );
			}
			
			selSuite->DeselectPageItems( pageItem );
			taskStatus.clear();
			progress.SetTaskStatus( taskStatus );

		} // end for loop
		
		// Add stories to the respective assignment
		if( numStories && !bUnAssigned )
		{
			InterfacePtr<IAssignmentMgr> assMgr( GetExecutionContextSession(), UseDefaultIID() ); 
			ASSERT( assMgr );

			assMgr->AddToAssignmentCmd( assignment, assgStoriesMapIter->second );		
			assMgr->SaveAssignment( assignment );
		}
	} // end for loop
	
	return result;
}

#endif

