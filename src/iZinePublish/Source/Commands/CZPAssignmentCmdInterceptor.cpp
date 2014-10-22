//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPAssignmentCmdInterceptor.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAddLinkCmdData.h"
#include "IAMServiceData.h"
#include "IAssignmentMgr.h"
#include "IAssignmentOptionsDialogData.h"
#include "IAssignSetPropsCmdData.h"
#include "IBoolData.h"
#include "ICommandInterceptor.h"
#include "ICopyCmdData.h"
#include "IDocFileType.h"
#include "IDocument.h"
#include "IGraphicFrameData.h"
#include "IMultiColumnTextFrame.h"
#include "IObserver.h"
#include "IStoryLabel.h"
#include "IStringData.h"
#include "ISubject.h"
#include "ISysFileData.h"
#include "ITextModel.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "InCopyBridgeID.h"
#include "IManagedStatus.h"
#include "IAMLockable.h"
#include "IManageableAsset.h"
#include "IWorkgroupFacade.h"
#endif

//SDK General includes
#include "AssignmentID.h"
#include "AssignmentUIID.h"
#include "InCopyBridgeUIID.h"
#include "InCopySharedID.h"
#include "PageItemScrapID.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPGlobalTempData.h"
#include "IZPPrefs.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "ZPDataHelper.h"
#include "ZPPresistDefaults.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	(eIZPMajorLogArea)(enLMA_AM | enLMA_Commands)
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ICommandInterceptor>
class CZPAssignmentCmdInterceptor : public inheritClass
{
public:
						CZPAssignmentCmdInterceptor(IPMUnknown * boss);
	virtual				~CZPAssignmentCmdInterceptor();

	InterceptResult		InterceptProcessCommand(ICommand *cmd);
	InterceptResult		InterceptScheduleCommand(ICommand *cmd) { return kCmdNotHandled; }
	
	InterceptResult		InterceptExecuteDynamic(ICommand * cmd) { return kCmdNotHandled; }

	//InterceptResult		InterceptUndoDynamic(ICommand * cmd) { return kCmdNotHandled; }
	//InterceptResult		InterceptRedoDynamic(ICommand * cmd) { return kCmdNotHandled; }

	// install/deinstall itself as command interceptor
	void		InstallSelf();
	void		DeinstallSelf();

	// these will eventually go away
	InterceptResult		InterceptExecuteImmediate(ICommand * cmd) { return kCmdNotHandled; }
	//InterceptResult		InterceptUndoImmediate(ICommand * cmd) { return kCmdNotHandled; }
	//InterceptResult		InterceptRedoImmediate(ICommand * cmd) { return kCmdNotHandled; }
protected:
	IDocument *			GetTargetDocument(ICommand * cmd) const;
	
	InterceptResult		CreateNewAssignmentAfterCheck(
							ICommand *					inCommand );

	InterceptResult		PatchNewAssignmentFilePath(
							const IDocument *			inTargetDoc,
							ICommand *					inCommand,
							IAssignSetPropsCmdData *	inCmdAssignProps );
	InterceptResult		SetAssetDataIfOurAssignment(
							const IDocument *			inTargetDoc,
							IAssignment *				inTargetAssignment,
							IAssignSetPropsCmdData *	inCmdAssignProps );

	InterceptResult		PatchNewIDStoryPath(
							ICommand *					inCommand );

	InterceptResult		PatchNewIDStoryName(
							ICommand *					inCommand );

	InterceptResult		HandleAddToAssignmentCommand(
							ICommand *					inCommand );

	InterceptResult		RemoveLinkFromPastedFrame();

	void				SetStoryUIDRefFromPastedFrame(
							ICommand *					inCommand );

	void				SetCountForLinkedFrameFromPastedFrame(
							ICommand *					inCommand );

private:
	PMString	mLastAsgnCreateStatusID;	//Used to patch the check-in dialog coming during first check-in
};


CREATE_PMINTERFACE(CZPAssignmentCmdInterceptor, kZPAssignmentCmdInterceptorImpl)

CZPAssignmentCmdInterceptor::CZPAssignmentCmdInterceptor(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssignmentCmdInterceptor::~CZPAssignmentCmdInterceptor()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InterceptProcessCommand
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::InterceptProcessCommand(
	ICommand *cmd)
{
	//LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ClassID cmdClass = GetClass( cmd );
	do
	{
		if( cmdClass == kAssignmentOptionsDialogCmdBoss )
		{
			result = this->CreateNewAssignmentAfterCheck( cmd );
		}
		else if(( cmdClass == kAssignDocCmdBoss )
		||( cmdClass == kAssignSetPropsCmdBoss ) )
		{
			//Check if this document is from our replica folder or not.

			const IDocument * theTargetDoc = this->GetTargetDocument( cmd );
			if( !theTargetDoc )
				return result;

			InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
			ASSERT( selfDoc );
			if( selfDoc != theTargetDoc )
				return result;	//Will be handled by some other interceptor

			LogFunctionEnterExit;

			InterfacePtr<IAssignSetPropsCmdData> assignProps( cmd, UseDefaultIID() );
			ASSERT( assignProps );

			//Create a new asset if this is new assignment
			if( cmdClass == kAssignDocCmdBoss )
			{
				//TODO: move this code to IZPAMHelper
				result = this->PatchNewAssignmentFilePath( theTargetDoc, cmd, assignProps);
				
			}
			else if ( kAssignSetPropsCmdBoss == cmdClass )
			{	//We can set the asset data at this place
				// For a new assignment, it comes before saving it to file.

				const UIDList * cmdItemList = cmd->GetItemList();
				ASSERT( cmdItemList );
				ASSERT( cmdItemList->size() == 1 );
				const UIDRef targetItem = cmdItemList->GetRef( 0 );
				
				InterfacePtr<IAssignment> targetAssignment( targetItem, UseDefaultIID() );
				this->SetAssetDataIfOurAssignment( theTargetDoc, targetAssignment, assignProps );
				//TODO: check if assignment name is changed. Update the asset on server.

				InterfacePtr<IBoolData> interceptorFlag(cmd , IID_IZPRUNBYPUBLISH );
				
				if( assignProps->GetFile() && (!interceptorFlag || interceptorFlag->Get() == kFalse) )
				{	//Don't Allow to change the file location.
					PMString * cmdFilePath = const_cast<PMString*>(assignProps->GetFile());
					cmdFilePath->operator =( kNullString );
					//result = kCmdHandled;
				}
			}
		}
		else if ( kICBridgeExportDialogCmdBoss == cmdClass )
		{	//We can create a new ID story asset here. And handle this command by not showing the dialog.
			result = this->PatchNewIDStoryPath( cmd );
		}
		else if ( kSetStoryLabelCmdBoss == cmdClass )
		{
			InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
			ASSERT( gTempData );
			if(gTempData->GetSkipAddLinkCount() > 0)
				result = this->RemoveLinkFromPastedFrame();
			
			if(result == kCmdNotHandled)
				result = this->PatchNewIDStoryName( cmd );
		}
		else if( kAddToAssignmentCmdBoss == cmdClass )
		{
			result = this->HandleAddToAssignmentCommand( cmd );
		}
		else if( kPasteCmdBoss == cmdClass)
		{
			InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
			ASSERT( gTempData );
			gTempData->SetSkipAddLink( true );
			gTempData->SetSkipAddLinkCount( 0 );

			//Start listening to cmd mgr for paste after do message
			InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
			InterfacePtr<IObserver> selfCmdObserver( this, IID_IZPDOCCMDOBSERVER );
			ASSERT( selfCmdObserver );
			if( selfSubject->IsAttached( selfCmdObserver, IID_ICOMMANDMGR, IID_IZPDOCCMDOBSERVER ) == false )	//Don't Detach it.
				selfSubject->AttachObserver( selfCmdObserver, IID_ICOMMANDMGR, IID_IZPDOCCMDOBSERVER );
		}
		else if( kAddLinkCmdBoss == cmdClass)
		{
			this->SetStoryUIDRefFromPastedFrame(cmd);
		}
		else if( kCopyPageItemCmdBoss == cmdClass)
		{
			this->SetCountForLinkedFrameFromPastedFrame(cmd);
		}		
#if defined(InDnCS5) || defined(InDnCS5_5)		
		else if(kLiveEditUpdateStoryLinksCmdBoss == cmdClass)
		{			
			const UIDList * cmdItemList = cmd->GetItemList();
			const UIDRef targetItem = cmdItemList->GetRef( 0 );
			UIDRef assetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef(targetItem);
			InterfacePtr<IManageableAsset> asset(assetRef, UseDefaultIID());
			if(asset)
			{
				InterfacePtr<IManagedStatus> storyStatus( asset->GetStatus(), UseDefaultIID() );
				if(storyStatus)
				{
					InterfacePtr<IAMLockable> storyLock( storyStatus, UseDefaultIID() );
					if(storyLock)
					{
						IManagedStatus::VersionState state = Utils<Facade::IWorkgroupFacade>()->GetVersionState(::GetUIDRef( asset ));
						if(state == IManagedStatus::enLocalIsOutOfDate)
							Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(::GetUIDRef( asset ));

					}
				}
			}			
		}
#endif
	}while(kFalse);

	return result;
}


//----------------------------------------------------------------------------------------
// InstallSelf
//----------------------------------------------------------------------------------------
void
CZPAssignmentCmdInterceptor::InstallSelf()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommandProcessor> proc ( gSession->QueryCommandProcessor() );
	proc->InstallInterceptor( this );
}


//----------------------------------------------------------------------------------------
// DeinstallSelf
//----------------------------------------------------------------------------------------
void
CZPAssignmentCmdInterceptor::DeinstallSelf()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommandProcessor> proc ( gSession->QueryCommandProcessor() );
	proc->DeinstallInterceptor( this );
}

//----------------------------------------------------------------------------------------
// GetTargetDocument
//----------------------------------------------------------------------------------------
IDocument *
CZPAssignmentCmdInterceptor::GetTargetDocument(
	ICommand * cmd )const
{
	//LogFunctionEnterExit;
	IDocument * toReturn = nil;

	const UIDRef & cmdTarget = cmd->GetTarget();

	InterfacePtr<IDocument> targetDoc( cmdTarget, UseDefaultIID() );
	toReturn = targetDoc;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreateNewAssignmentAfterCheck
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::CreateNewAssignmentAfterCheck(
	ICommand *					inCommand)
{
	//LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	//ClassID cmdClass = GetClass( inCommand );
	do
	{
		InterfacePtr<IAssignmentOptionsDialogData> cmdOptionDlgData( inCommand, UseDefaultIID() );
		ASSERT( cmdOptionDlgData );

		//Check if this document is from our replica folder or not.
		InterfacePtr<const IDocument> theTargetDoc( cmdOptionDlgData->GetDocRef(), UseDefaultIID() );
		if( !theTargetDoc )
			return result;

		InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
		ASSERT( selfDoc );
		if( selfDoc != theTargetDoc )
			return result;	//Will be handled by some other interceptor

		LogFunctionEnterExit;

		mLastAsgnCreateStatusID = kNullString;

		if( cmdOptionDlgData->IsNewAssignment() == kFalse )	//Handle this command only for new assignment.
			break;

		if( !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( theTargetDoc ) )
		{	//New Assignment is not allowed in this document, generally ID Template.
			ErrorUtils::PMSetGlobalErrorCode(kCancel);//kZPNewAssignmentNotAllowedError);	//Setting error here results in crash. Will set it again in kAssignDocCmdBoss.
			cmdOptionDlgData->SetDocRef( UIDRef::gNull );
			cmdOptionDlgData->SetNewFile( kNullString );
			result = kCmdHandled;
			break;
		}

		result = kCmdHandled;	//What ever the result is Cancel or OK, don't allow InDesign to handle it.
		
		//Show UI for new assignment creation.
		InterfacePtr<IManageableAsset> docManagableAsset( theTargetDoc, UseDefaultIID() );
		InterfacePtr<IAMUIService> amUIService( Utils<IZPAMHelper>()->QueryAMUIService(docManagableAsset, (IAMUIServiceProvider::UIService)enAMUINewAssignmentService ) );
		ASSERT( amUIService );
		ErrorCode serviceReturn = amUIService->Do( docManagableAsset );

		//Save values with command.
		InterfacePtr<IAMServiceData> amServiceData(amUIService, UseDefaultIID());
		ASSERT(amServiceData);
		IAMUIService::FollowupAction userAction = IAMUIService::enCancel;
		amServiceData->Get(IAMUIService::enFollowupAction, userAction);
		if( userAction != IAMUIService::enSave )
		{
			ErrorUtils::PMSetGlobalErrorCode(kCancel);	//Setting cancel here does is not adhered. Still comes in kAssignDocCmdBoss.
			cmdOptionDlgData->SetDocRef( UIDRef::gNull );
			cmdOptionDlgData->SetNewFile( kNullString );
			result = kCmdHandled;
			break;
		}

		IDFile selectedFile;
		amServiceData->Get( IAMUIService::enSelectedFile, selectedFile );
		PMString assetFilePath;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( selectedFile, assetFilePath );
		cmdOptionDlgData->SetNewFile( assetFilePath );

		PMString assetName;
		amServiceData->Get(IAMUIService::enDocumentName, assetName);
		cmdOptionDlgData->SetName( assetName );

		//ioServiceData->Set( kZPAMServiceDataAssetID, dialogAssetData->GetAssetID() );
		//ioServiceData->Set( kZPAMServiceDataTitleID, dialogAssetData->GetTitleID() );
		//ioServiceData->Set( kZPAMServiceDataEditionID, dialogAssetData->GetEditionID() );
	
		amServiceData->Get( kZPAMServiceDataStatusID, mLastAsgnCreateStatusID );
		//ioServiceData->Set( enVersionComment, dlgAssetCheckInData->GetDescription() );

		//Set hardcode values
		cmdOptionDlgData->SetExportOption( (IAssignment::AssignmentExportOptions)kZPPrefDefault_NewAsgnExpOption );
		cmdOptionDlgData->SetIncludeLinkedFilesWhenPackage( kZPPrefDefault_NewAsgnIncludeLinked );
		cmdOptionDlgData->SetNewAssignmentLocationNeedsValidation( kFalse );
	}while(false);
	return result;
}




//----------------------------------------------------------------------------------------
// PatchNewAssignmentFilePath
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::PatchNewAssignmentFilePath(
	const IDocument *			inTargetDoc,
	ICommand *					inCommand,
	IAssignSetPropsCmdData *	inCmdAssignProps  )
{
	LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	do
	{
		InterfacePtr<const IZPAssetData> targetDocAssetData( inTargetDoc, UseDefaultIID() );
		ASSERT( targetDocAssetData );

		if( !targetDocAssetData )
			break;

		if( !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( inTargetDoc ) )
		{	//New Assignment is not allowed in this document, generally ID Template.
			ErrorUtils::PMSetGlobalErrorCode(kZPNewAssignmentNotAllowedError);
			result = kCmdHandled;
			break;
		}

		const PMString * newAssetName = inCmdAssignProps->GetName();
		ASSERT( newAssetName != nil );
		const PMString & docEditionID = targetDocAssetData->GetEditionID();
		const PMString & assetType = Utils<IZPAssetUtils>()->GetAssetTypeID( eAssetType_ICAssignment );
		PMString newAssetID;

		if( docEditionID.IsEmpty() )
			break;

		//TODO: Check if the path is already patched by kAssignmentOptionsDialogCmdBoss
		//returning for now.
		//ErrorCode currError = ErrorUtils::PMGetGlobalErrorCode();
		//if( currError != kSuccess )
			//result = kCmdHandled;
		break;

		IZPLog_Str_( thisFileLA, enLT_Details, "Creating new asset of type - assignment" );
		if (kSuccess != Utils<IZPAMHelper>()->CreateNewAsset( newAssetID, *newAssetName, assetType, docEditionID, targetDocAssetData->GetAssetID() ))
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSAssetCreatingError);
			result = kCmdHandled;	//Supress further processing of the command
			break;
		}

		PMString * cmdFilePath = const_cast<PMString*>(inCmdAssignProps->GetFile());
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Old path for new assignment : %s", cmdFilePath->GrabCString() );
		
		const PMString & assignmentFileExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICAssignment );
		IDFile newPathForAssignmentCreation = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, assignmentFileExt, *newAssetName );
		PMString newPathStr;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( newPathForAssignmentCreation, newPathStr );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "New path for new assignment : %s", newPathStr.GrabCString() );
		cmdFilePath->operator =( newPathStr );
	}while(kFalse);

	return result;
}

//----------------------------------------------------------------------------------------
// SetAssetDataIfOurAssignment
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::SetAssetDataIfOurAssignment(
	const IDocument *			inTargetDoc,
	IAssignment *				inTargetAssignment,
	IAssignSetPropsCmdData *	inCmdAssignProps)
{
	//LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	do
	{
		if( inTargetDoc == nil || inTargetAssignment == nil )
			break;

		//TODO: this document must be from our replica folder.
		InterfacePtr<const IZPAssetData> targetAssetData( inTargetAssignment, UseDefaultIID() );
		ASSERT( targetAssetData );
		if( targetAssetData->GetAssetID().IsEmpty() == kFalse )
			break;		//Already set, not a new assignment case.

		const PMString & assignmentFilePath = inTargetAssignment->GetFile();
		if( assignmentFilePath.IsEmpty() )
			break;
		PMString assetIDStr = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( assignmentFilePath );
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetIDStr );
		
		if( dbAssetID )
		{
			Utils<IZPCommandFacade>()->SaveAssetData( dbAssetID, inTargetAssignment );

			//Patch: Status ID was asked from user before creating the assignment asset. Use that value for first check-in
			if( mLastAsgnCreateStatusID.IsEmpty() == kFalse )
			{
				InterfacePtr<IZPAssetCheckInData> assignmentCheckInData( inTargetAssignment, UseDefaultIID() );
				ASSERT( assignmentCheckInData );

				assignmentCheckInData->SetStatusID( mLastAsgnCreateStatusID );

				const PMString & asgnName = inTargetAssignment->GetName();
				assignmentCheckInData->SetAssetName( asgnName );
			}
		}
	}while( false );
	return result;
}

#include "FileUtils.h"
//----------------------------------------------------------------------------------------
// PatchNewIDStoryPath
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::PatchNewIDStoryPath(
	ICommand *					inCommand)
{
	LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	do
	{
		//Check if this document is from our replica folder or not.
		const IDocument * theTargetDoc = this->GetTargetDocument( inCommand );
		if( !theTargetDoc )
			return result;

		InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
		ASSERT( selfDoc );
		if( selfDoc != theTargetDoc )
			return result;	//Will be handled by some other interceptor
		
		LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		FileTypeInfoID saveTypeID = kInCopyXMLMarkupFileTypeInfoID;
#else
		FileTypeInfoID saveTypeID = kInCopyXMLInterchangeFileTypeInfoID;
#endif
		InterfacePtr<IDocFileType> cmdDocFileType( inCommand, UseDefaultIID());
		ASSERT( cmdDocFileType );
		if( cmdDocFileType )	//TODO: should we check the type here? For Graphic stories?
		{
#ifdef DEBUG
			FileTypeInfoID oldFileID = cmdDocFileType->GetDocFileType();
			IZPLog_Str_( thisFileLA, enLT_Details, "Old file type ID : %d", oldFileID.Get());
#endif
			cmdDocFileType->SetDocFileType( saveTypeID );
		}

		if( !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( theTargetDoc ) )
		{	//New Assignment is not allowed in this document, generally ID Template.
			//If assignment is not allowed then don't allow story to export
			ErrorUtils::PMSetGlobalErrorCode(kZPNewAssignmentNotAllowedError);
			result = kCmdHandled;
			break;
		}

		InterfacePtr<const IZPAssetData> targetDocAssetData( theTargetDoc, UseDefaultIID() );
		ASSERT( targetDocAssetData );

		//TODO: try using name from oldPath.
		const PMString newAssetName = kNullString;	//Change the name afterwards, as we don't know the name at this point.
		const PMString & docEditionID = targetDocAssetData->GetEditionID();
		const PMString & assetType = Utils<IZPAssetUtils>()->GetAssetTypeID( eAssetType_IDStory );
		PMString newAssetID;

		if( docEditionID.IsEmpty() )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSAssetCreatingError);
			result = kCmdHandled;	//Supress further processing of the command
			break;
		}

		IZPLog_Str_( thisFileLA, enLT_Details, "Creating new asset of type - ID Story" );
		if (kSuccess != Utils<IZPAMHelper>()->CreateNewAsset( newAssetID, newAssetName, assetType, docEditionID))
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSAssetCreatingError);
			result = kCmdHandled;	//Supress further processing of the command
			break;
		}

		InterfacePtr<ISysFileData> cmdSysFileData( inCommand, UseDefaultIID() );
		ASSERT( cmdSysFileData );
#ifdef DEBUG
		const IDFile & oldPath = cmdSysFileData->GetSysFile();
		PMString oldPathStr;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( oldPath, oldPathStr );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Old path for ID Story : %s", oldPathStr.GrabCString() );
#endif
		
		const PMString & idStoryFileExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_IDStory );
		IDFile newPathForIDStoryCreation = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, idStoryFileExt , newAssetName);

		PMString newPathStr;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( newPathForIDStoryCreation, newPathStr );
#ifdef DEBUG
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "New path for ID story : %s", newPathStr.GrabCString() );
#endif
		cmdSysFileData->Set( newPathForIDStoryCreation );

#ifdef DEBUG
		InterfacePtr<const IBoolData> cmdBoolData( inCommand, UseDefaultIID() );
		ASSERT( cmdBoolData );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Bool flag on command : %hd", cmdBoolData->Get() );
#endif

		InterfacePtr<IStringData> cmdStringData( inCommand, IID_IDLGRESULTDATA );
		ASSERT( cmdStringData );
#ifdef DEBUG
		const PMString& cmdStringDataStr = cmdStringData->Get();
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "String data on command : %s", cmdStringDataStr.GrabCString() );
#endif
		cmdStringData->Set( newPathStr );

		result = kCmdHandled;
	}while( false );

	return result;
}

//----------------------------------------------------------------------------------------
// PatchNewIDStoryName
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::PatchNewIDStoryName(
	ICommand *					inCommand)
{
	//LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	do
	{
		//Check if this document is from our replica folder or not.
		const IDocument * theTargetDoc = this->GetTargetDocument( inCommand );
		if( !theTargetDoc )
			return result;

		InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
		ASSERT( selfDoc );
		if( selfDoc != theTargetDoc )
			return result;	//Will be handled by some other interceptor
		
		LogFunctionEnterExit;
		InterfacePtr<IStringData> cmdStoryLabel( inCommand, UseDefaultIID() );
		ASSERT( cmdStoryLabel );
		PMString storyLabelStr = cmdStoryLabel->Get();
		IZPLog_Str_( thisFileLA, enLT_Details, "Story label in command : %s", storyLabelStr.GrabCString() );
		
		//If setting the story name for the first time,
		//Get the name from story content. If empty then some fixed name.
		const UIDList * cmdItemList = inCommand->GetItemList();
		if( !cmdItemList || cmdItemList->size() != 1 )
			break;
		const UIDRef & storyRef = cmdItemList->GetRef(0);
		InterfacePtr<IStoryLabel> storyLabel( storyRef, IID_ISTORYLABEL);
		if( !storyLabel )
			break;
		
		PMString storyNewLabel = storyLabel->GetLabel();
		if( storyNewLabel.IsEmpty() )
		{
			InterfacePtr<const ITextModel> storyTextModel( storyRef, UseDefaultIID());
			storyNewLabel = Utils<IZPStoryUtils>()->GetStoryFirstNChars( storyTextModel, 26, true, StoryNameFilter );

			if( storyNewLabel.IsEmpty() )
			{
				//Patch: commented setting label to "Story"
				//This also gets called when updating the content, and the new label comes empty each time.
				//So don't set modify the label when new label is coming empty.
			
				//storyNewLabel = "Story";	//TODO: append the numeric value, unique in document
			}

			IZPLog_Str_( thisFileLA, enLT_Details, "Story new label : %s", storyNewLabel.GrabCString() );
			if( !storyNewLabel.IsEmpty() )
				cmdStoryLabel->Set( storyNewLabel );
		}
	}while( false );

	return result;
}

//----------------------------------------------------------------------------------------
// HandleAddToAssignmentCommand
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::HandleAddToAssignmentCommand(
	ICommand *					inCommand)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterceptResult result = kCmdNotHandled;
	do
	{
		//Check if this document is from our replica folder or not.
		const IDocument * theTargetDoc = this->GetTargetDocument( inCommand );
		if( !theTargetDoc )
			break;

		InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
		ASSERT( selfDoc );
		if( selfDoc != theTargetDoc )
			break;	//Will be handled by some other interceptor
		
		LogFunctionEnterExit;

		InterfacePtr<IZPGlobalTempData> zpGlobalData( gSession, UseDefaultIID() );
		ASSERT( zpGlobalData );
		const PMString draggedAssetID = zpGlobalData->GetLastDraggedAssetID();	//Don't make it reference
		if( draggedAssetID.IsEmpty() )
			break;

		zpGlobalData->SetLastDraggedAssetID( kNullString );

		//Check if the asset dropped needs to have its own assignment. Redmine#1871
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
		ASSERT(zpPrefs);
		if( zpPrefs->GetCreateAssignmentOnStoryImport() == kFalse )
			break;
		
		//TODO: Check if assignment to which story is being added is Unassigned or not.
		//Currently we are getting cmd item list as null
		const UIDList * cmdItemList = inCommand->GetItemList();
		if( !cmdItemList || cmdItemList->size() <= 0)
			break;
		//InterfacePtr<IAssignment> addingToAssignment(cmdItemList->GetRef(1), UseDefaultIID());
		//ASSERT( addingToAssignment );
		//if( addingToAssignment->GetFile().IsEmpty() == kFalse )	//Not adding the assignment to "Unassigned content"
		//	break;

		//TODO: Check if this is same asset story that was dragged.
		
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( draggedAssetID );
		ASSERT( dbAssetID );
		if( !dbAssetID )
			break;

		InterfacePtr<const IStringData> dbAssetName( dbAssetID, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		const PMString & dbAssetNameStr = dbAssetName->Get();

		//Check if a assignment of same name already exists
		InterfacePtr<IAssignment> newAssignmentToMoveIn( Utils<IZPDocUtils>()->QueryAssignmentByName( theTargetDoc, dbAssetNameStr ) );
		
		if( !newAssignmentToMoveIn )
		{	//Create a new assignment & assignment asset.
			bool suppressCheckinDialog = true;

			IAssignment * newAssignment = Utils<IZPAssetDocUtils>()->CreateAssignment(selfDoc, dbAssetNameStr, !suppressCheckinDialog);
			newAssignment->AddRef();
			newAssignmentToMoveIn.reset(newAssignment);
			
		}

		if( !newAssignmentToMoveIn )
			break;

		//Move the assigned story to assignment of self name.
		InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
		UIDList newCmdItemList( *cmdItemList );
		ErrorCode movedError = assMgr->AddToAssignmentCmd( newAssignmentToMoveIn, newCmdItemList );


		//If moved then set the command result to be handled.
		if( movedError == kSuccess )
			result = kCmdHandled;
	}while( false );
	return result;

}

//----------------------------------------------------------------------------------------
// RemoveLinkFromPastedFrame
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPAssignmentCmdInterceptor::RemoveLinkFromPastedFrame()
{
	InterceptResult result = kCmdNotHandled;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
		ASSERT( gTempData );
		if(gTempData->GetSkipAddLinkCount() > 0)
		{
			const UIDRef storyUIDRef = gTempData->GetStoryUIDRefToSkipAddLink();
			if( storyUIDRef == UIDRef::gNull )
				break;
			gTempData->SetStoryUIDRefToSkipAddLink( UIDRef::gNull );

			//asset id ref
			UIDRef storyAssetUIDRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyUIDRef );
			if (storyAssetUIDRef == UIDRef::gNull)
				break;

			IDataBase* storyDB = storyAssetUIDRef.GetDataBase();

			InterfacePtr<IDataLink> iDataLink(storyDB, storyAssetUIDRef.GetUID(), IID_IDATALINK);
			if (iDataLink)
			{
				InterfacePtr<const IZPAssetData> dataLinkAssetData( iDataLink, UseDefaultIID() );
				ASSERT( dataLinkAssetData );
				
				if( !dataLinkAssetData )
					break;
				const PMString & assetID = dataLinkAssetData->GetAssetID();
				if( assetID.IsEmpty() == kFalse )
				{
					//Check if this is already present in the document. Remove only if present.
					InterfacePtr<IDocument> document(this , UseDefaultIID() );
					if(Utils<IZPAssetDocUtils>()->CountLinkExistForAssetID(document, assetID) > 1)
					{
						InterfacePtr<ICommand> removeDataLink (CmdUtils::CreateCommand(kRemoveLinksCmdBoss));
						removeDataLink->SetItemList(UIDList(iDataLink));
						ErrorCode err = CmdUtils::ScheduleCommand(removeDataLink, ICommand::kMediumPriority );

						result = kCmdHandled;
					}
				}

				//Decrease count and unset flag
				gTempData->SetSkipAddLinkCount( gTempData->GetSkipAddLinkCount() - 1 );
				if(gTempData->GetSkipAddLinkCount() <= 0)
					gTempData->SetSkipAddLink( false );
			}
		}
	}while(false);

	return result;
}

//----------------------------------------------------------------------------------------
// SetStoryUIDRefFromPastedFrame
//----------------------------------------------------------------------------------------
void
CZPAssignmentCmdInterceptor::SetStoryUIDRefFromPastedFrame(
	ICommand *					inCommand)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
		ASSERT( gTempData );
		if(gTempData->GetSkipAddLinkCount() > 0)
		{
			InterfacePtr<IAddLinkCmdData > addLinkCmdData( inCommand, IID_IADDLINKCMDDATA );

			IDataBase * db = addLinkCmdData->GetDataBase();
			UID pageItemUID = addLinkCmdData->GetPageItemUID();

			InterfacePtr<const ITextModel> textModel( db, pageItemUID, UseDefaultIID());
			if (!textModel)
				break;

			UIDRef storyUIDRef = ::GetUIDRef(textModel);

			gTempData->SetStoryUIDRefToSkipAddLink(storyUIDRef);		//Set Story UIDRef to remove link

			//Remove link of scrap doc boss, if it is ours
			InterfacePtr<IDataLink> dataLink( addLinkCmdData->GetDataLink(), UseDefaultIID() );
			if (dataLink)
			{
				InterfacePtr<const IZPAssetData> dataLinkAssetData( dataLink, UseDefaultIID() );
				ASSERT( dataLinkAssetData );
				
				if( !dataLinkAssetData )
					break;
				const PMString & assetID = dataLinkAssetData->GetAssetID();
				if( assetID.IsEmpty() == kFalse )
				{
					InterfacePtr<ICommand> removeDataLink (CmdUtils::CreateCommand(kRemoveLinksCmdBoss));
					removeDataLink->SetItemList(UIDList(dataLink));
					ErrorCode err = CmdUtils::ScheduleCommand(removeDataLink, ICommand::kLowestPriority );
				}
			}
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// SetCountForLinkedFrameFromPastedFrame
//----------------------------------------------------------------------------------------
void
CZPAssignmentCmdInterceptor::SetCountForLinkedFrameFromPastedFrame(
	ICommand *					inCommand)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
		ASSERT( gTempData );
		if(gTempData->GetSkipAddLink())
		{
			InterfacePtr<ICopyCmdData> copyCmdData( inCommand, UseDefaultIID() );
			const UIDList * cmdItemList = copyCmdData->GetInputItems();
			ASSERT( cmdItemList );
			ASSERT( cmdItemList->size() == 1 );
			int totalSkipAddLinkCount = gTempData->GetSkipAddLinkCount();
			int count = cmdItemList->size();
			for(int x = 0; x < count; ++x)
			{
				const UIDRef targetItem = cmdItemList->GetRef( x );
				InterfacePtr<IGraphicFrameData> graphicFrameData(targetItem, UseDefaultIID());
				if(!graphicFrameData)
					continue;

				InterfacePtr<IMultiColumnTextFrame> textFrameMC(graphicFrameData->QueryMCTextFrame());
				if(!textFrameMC)
					continue;

				InterfacePtr<const ITextModel> textModel( textFrameMC->QueryTextModel());
				if (!textModel)
					continue;

				UIDRef storyUIDRef = ::GetUIDRef(textModel);

				//asset id ref
				UIDRef storyAssetUIDRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyUIDRef );
				if (storyAssetUIDRef == UIDRef::gNull)
					continue;

#if defined(InDnCS5) || defined(InDnCS5_5)
				InterfacePtr<ILinkResource> assetDataLink( storyAssetUIDRef, UseDefaultIID() );
				if( assetDataLink )
#else
				InterfacePtr<IDataLink> assetDataLink( storyAssetUIDRef , UseDefaultIID() );
				if( assetDataLink )
#endif
				{
					InterfacePtr<const IZPAssetData> dataLinkAssetData( assetDataLink, UseDefaultIID() );
					ASSERT( dataLinkAssetData );
				
					if( !dataLinkAssetData )
						break;
					totalSkipAddLinkCount = totalSkipAddLinkCount + 1;	//Increase count
				}
			}

			gTempData->SetSkipAddLinkCount( totalSkipAddLinkCount );
		}
	}while(false);
}