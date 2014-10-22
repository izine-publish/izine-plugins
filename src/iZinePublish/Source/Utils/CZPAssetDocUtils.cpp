//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPAssetDocUtils.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssignedDocument.h"
#include "IAssignment.h"
#include "IAssignmentMgr.h"
#include "IAssignSetPropsCmdData.h"
#include "IBoolData.h"
#include "IDataLink.h"
#include "IDataLinkReference.h"
#include "IDocument.h"
#include "IDocumentList.h"
#include "ILinksManager.h"
#include "IStoryLabel.h"
#include "IStoryList.h"
#include "IStringData.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkgroupUIFacade.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"
#include "UIDList.h"
#include "ErrorUtils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "StUtils.h"
#include "ZPConstants.h"
#include "ZPDataHelper.h"
#include "ZPPresistDefaults.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetDocUtils>
class CZPAssetDocUtils : public inheritClass
{
public:
						CZPAssetDocUtils(IPMUnknown * boss);
	virtual				~CZPAssetDocUtils();

	IAssignment *		CreateAssignment(
							const IDocument *		inDocument,
							const PMString &		inAssignmentName,
							bool					inShowCheckinUI ) const;

	IDocument *			GetDocumentForDocAssetID(
							const PMString &		inDocAssetID ) const;

	ErrorCode			GetDocumentAssignments(
							const IDocument *		inDocument,
							ZPAssetIDsList *		oAssignmentAssetIDs,
							UIDList *				oAssignments ) const;

	void				CheckAndUpdateAssignmentStoriesStatus(	//Set all stories status to be "Assigned"
							IAssignment*			inAssignment ) const;
	void				CheckAndUpdateUnAssignmentStoriesStatus(	//Set all unassigned stories status to be "UnAssigned"
							IDocument*				inDocument) const;
	void				CheckAndUpdateAssignmentStoriesStatus(
							IAssignment*			inAssignment,
							const PMString &		inNewStatusID ) const;

	int					CountLinkExistForAssetID(
								const IDocument *		inDocument,
								const PMString &		inAssetID) const;

	bool				IsNewAssignmentAllowed(
							const IDocument *		inDocument ) const;

	void				GetAssetIdsUsedInDoc(
							const IDocument *		inDocument,
							ZPAssetIDsList &		oAssetIdList) const;

	void				GetAssignmentAssetIdsUsedInDoc(
							const IDocument *		inDocument,
							ZPAssetIDsList &		oAssetIdList) const;

	void				GetStoryAssetIdsUsedInDoc(
							const IDocument *		inDocument,
							ZPAssetIDsList &		oAssetIdList) const;

	bool				IsOurAssetDocument(
								const IDocument *		inDocument) const;

	void				ValidateAssignmentsName(
								const IDocument *		inDocument) const;

	void				ValidateStoriesName(
								const IDocument *		inDocument) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPAssetDocUtils, kZPAssetDocUtilsImpl)

CZPAssetDocUtils::CZPAssetDocUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetDocUtils::~CZPAssetDocUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateAssignment
//----------------------------------------------------------------------------------------
IAssignment *
CZPAssetDocUtils::CreateAssignment(
	const IDocument *		inDocument,
	const PMString &		inAssignmentName,
	bool					inShowCheckinUI) const
{
	IAssignment * toReturn = nil;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<const IZPAssetData> targetDocAssetData( inDocument, UseDefaultIID() );
		ASSERT( targetDocAssetData );

		if( !targetDocAssetData )
			break;

		if( !Utils<IZPAssetDocUtils>()->IsNewAssignmentAllowed( inDocument ) )
		{	//New Assignment is not allowed in this document, generally ID Template.
			ErrorUtils::PMSetGlobalErrorCode(kZPNewAssignmentNotAllowedError);
			break;
		}

		const PMString & docEditionID = targetDocAssetData->GetEditionID();
		const PMString & assetType = Utils<IZPAssetUtils>()->GetAssetTypeID( eAssetType_ICAssignment );
		PMString newAssetID;

		if( docEditionID.IsEmpty() )
			break;

		IZPLog_Str_( thisFileLA, enLT_Details, "Creating new asset of type - assignment" );
		if (kSuccess != Utils<IZPAMHelper>()->CreateNewAsset( newAssetID, inAssignmentName, assetType, docEditionID, targetDocAssetData->GetAssetID() ))
		{
			//CAlert::ErrorAlert("Error occured creating asset in repository.");	//TODO: set this as error
		}

		const PMString & assignmentFileExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICAssignment );
		IDFile newPathForAssignmentCreation = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, assignmentFileExt, inAssignmentName );
		PMString newPathStr;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( newPathForAssignmentCreation, newPathStr );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "New path for new assignment : %s", newPathStr.GrabCString() );

		//make new assignment 
		InterfacePtr<ICommand> assignDocCmd(CmdUtils::CreateCommand(kAssignDocCmdBoss));
		UIDRef docRef = ::GetUIDRef(inDocument);
		UIDList uidl(docRef);
		assignDocCmd->SetItemList(uidl);

		if (inShowCheckinUI == false)
		{
			InterfacePtr<IBoolData> assignBoolData(assignDocCmd , UseDefaultIID() );
			assignBoolData->Set(inShowCheckinUI);
		}

		IAssignSetPropsCmdDataPtr data(assignDocCmd, UseDefaultIID());
		bool16 dirty = kTrue;
		bool16 includeLinked = kZPPrefDefault_NewAsgnIncludeLinked;
		IAssignment::AssignmentExportOptions asgnExpOption = (IAssignment::AssignmentExportOptions)kZPPrefDefault_NewAsgnExpOption;
		data->Set(&inAssignmentName, &newPathStr, nil, nil, &asgnExpOption, &dirty, &includeLinked);
		ErrorCode err = CmdUtils::ProcessCommand(assignDocCmd);
		if (err != kSuccess)
			break;

		InterfacePtr<IAssignment> assignment(assignDocCmd->GetItemList()->GetRef(0), UseDefaultIID());
		InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID()); 
		
		if (inShowCheckinUI == false)
		{
			//TODO: Code duplicacy. Already done in AZPAMService::CopyParentCheckInData()

			InterfacePtr<IZPAssetCheckInData> assetCheckInData( assignment, UseDefaultIID());
			ASSERT(assetCheckInData);

			IDataBase * assetDB = ::GetDataBase( assignment );
			
			const IStringData * docDbAssetID = ZPDataHelper::GetAssetByID( targetDocAssetData->GetAssetID() );
			ASSERT( docDbAssetID );
			if( docDbAssetID )
			{
				InterfacePtr<const IZPAssetInfo> docDbAssetInfo( docDbAssetID, UseDefaultIID());
				ASSERT( docDbAssetInfo );
				if( docDbAssetInfo->GetVersionNumber() > 0 )
				{
					ASSERT( docDbAssetInfo->GetStatusID().IsEmpty() == kFalse );
					assetCheckInData->SetStatusID( docDbAssetInfo->GetStatusID() );
				}
			}

			//save assignment
			assMgr->SaveAssignment(assignment);
		}

		assMgr->NotifyPanel();

		toReturn = assignment;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDocumentForDocAssetID
//----------------------------------------------------------------------------------------
IDocument *
CZPAssetDocUtils::GetDocumentForDocAssetID(
	const PMString &		inDocAssetID) const
{
	InterfacePtr<IDocumentList> docList( Utils<IZPDocUtils>()->QueryOpenDocList() );
		
	IDocument* toReturn = nil;
	IDocument* currDocInList = nil;
	
	int32 count = docList->GetDocCount();
	
	while(count--)
	{
		currDocInList = docList->GetNthDoc(count);

		InterfacePtr<const IZPAssetData> assetData(currDocInList, UseDefaultIID());
		
		if ( !assetData )	//Must not be the case
			continue;
	
		const PMString & currDocAssetID = assetData->GetAssetID();
		if( currDocAssetID.Compare( kFalse, inDocAssetID ) == 0 )
		{
			toReturn = currDocInList;
			break;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDocumentAssignments
//----------------------------------------------------------------------------------------
//Not being used. Instead of this use GetAssignmentAssetIdsUsedInDoc
ErrorCode
CZPAssetDocUtils::GetDocumentAssignments(
	const IDocument *		inDocument,
	ZPAssetIDsList *		oAssignmentAssetIDs,
	UIDList *				oAssignments) const
{
	ASSERT( inDocument );

	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;

	if( !oAssignmentAssetIDs && !oAssignments )
		return toReturn;

	do
	{
		InterfacePtr<IAssignedDocument> assignedDoc( inDocument, UseDefaultIID());
		ASSERT( assignedDoc );
		
		IDataBase* docDB = ::GetDataBase( inDocument );
		ASSERT( docDB );

		if( oAssignments )
			*oAssignments = UIDList( docDB );
		// relink assignment files
		UIDList assignmentList = assignedDoc->GetAssignments();
		int listLen = assignmentList.Length();
		for (int32 i = 0 ; i < listLen; i++)
		{
			UIDRef assignmentref( docDB, assignmentList[i]);
			
			// why is iassetdata empty?
			// because it is probably not in the scriptprovider. And IAssignment don't have a assetdata
			
			InterfacePtr<IAssignment> assignment( assignmentref, UseDefaultIID() );
			ASSERT( assignment );
			
			// unassigned is also an assignment with
			PMString filePath;
			Utils<IZPFileUtils>()->GetFileName( assignment->GetFile(), filePath );
			
			if( filePath.IsEmpty() )
				continue;
			
			// remove trailing numbers
			PMString* assetID = filePath.Substring(0, 36);
			StPtrDeleter<PMString>		autoDel( assetID, true);
			ASSERT( assetID );
			
			if( !assetID )
				continue;
			
			if( oAssignments )
				oAssignments->push_back( assignmentList[i] );
			
			if( oAssignmentAssetIDs )
				oAssignmentAssetIDs->push_back( *assetID );
		}
		toReturn = kSuccess;
	}while( false );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckAndUpdateAssignmentStoriesStatus
//----------------------------------------------------------------------------------------
//Set all stories status to be "Assigned"
void
CZPAssetDocUtils::CheckAndUpdateAssignmentStoriesStatus(
	IAssignment*				inAssignment) const
{
	LogFunctionEnterExit;
	if( !inAssignment )
		return;

	this->CheckAndUpdateAssignmentStoriesStatus( inAssignment, kTitleStatus_Assigned);
}

//----------------------------------------------------------------------------------------
// CheckAndUpdateUnAssignmentStoriesStatus
//----------------------------------------------------------------------------------------
//Set all unassigned stories status to be "UnAssigned"
void
CZPAssetDocUtils::CheckAndUpdateUnAssignmentStoriesStatus(
	IDocument*				inDocument) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IAssignmentMgr> iAsgnMgr(gSession, UseDefaultIID());
	ASSERT(iAsgnMgr);

	IAssignedDocument * assignedDoc = iAsgnMgr->GetAssignedDocument( ::GetDataBase(inDocument) );
	ASSERT(assignedDoc);

	InterfacePtr<IAssignment> unassginedAssignment( iAsgnMgr->QueryAssignment(assignedDoc, kNullString ) );
	ASSERT( unassginedAssignment );

	this->CheckAndUpdateAssignmentStoriesStatus( unassginedAssignment, kTitleStatus_Unassigned );
}

//----------------------------------------------------------------------------------------
// CheckAndUpdateAssignmentStoriesStatus
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::CheckAndUpdateAssignmentStoriesStatus(
	IAssignment*			inAssignment,
	const PMString &		inNewStatusID) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IAssignmentMgr> iAsgnMgr(gSession, UseDefaultIID());
	ASSERT(iAsgnMgr);

	IDataBase* asgnDB = ::GetDataBase( inAssignment );
	UIDList storyList(inAssignment);
	iAsgnMgr->GetStoryListOnAssignment(inAssignment, storyList);
	
	Utils<IZPAMHelper> amHelperUtils;

	int storyCount = storyList.size();
	UIDRef asgnStoryRef(storyList.GetDataBase(), 0 );
	for(int i = 1; i < storyCount; ++i)		//first is assignment itself.
	{
		asgnStoryRef.ResetUID( storyList[i] );

		do {
			//InterfacePtr<IAssignedStory> assignedStory(asgnStoryRef, UseDefaultIID());
			//if(!assignedStory)
			//	break;

			if( asgnStoryRef.GetUID() == kInvalidUID )
				break;

			UIDRef storyRef = asgnStoryRef;
			if( storyRef.GetUID() == kInvalidUID )
				break;
#if defined(InDnCS5) || defined(InDnCS5_5)
			UIDRef storyAssetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
			if( storyAssetRef.GetUID() == kInvalidUID )
				break;

			InterfacePtr<IZPAssetData> theStoryAssetData(storyAssetRef, UseDefaultIID());
			if(!theStoryAssetData)
				break;
#else
			//InterfacePtr<IDataLinkReference> dataReference(assignedStory->GetUIDRef(), UseDefaultIID());
			InterfacePtr<IDataLinkReference> dataReference(storyRef, UseDefaultIID());
			if(!dataReference)
				break;

			UID dataLinkUID = dataReference->GetUID();
			if( dataLinkUID == 0 )
				break;

			UIDRef storyAssetRef(asgnDB, dataLinkUID);
			InterfacePtr<IDataLink> dataLink(storyAssetRef, UseDefaultIID());
			if(!dataLink)
				break;

			InterfacePtr<IZPAssetData> theStoryAssetData(dataLink, UseDefaultIID());
#endif
			if(!theStoryAssetData)
				break;

			const PMString & storyAssetID = theStoryAssetData->GetAssetID(); 
			if ( storyAssetID.IsEmpty())
				break;
			
			const IStringData * theDBAssetID = ZPDataHelper::GetAssetByID(storyAssetID);
			if( !theDBAssetID )
				break;
			
			if( amHelperUtils->StoryNeedsStatusUpdateOnServer( theDBAssetID, inNewStatusID ) )
			{
				if( inNewStatusID.Compare( kFalse, kTitleStatus_Unassigned ) == 0 )
				{	//Show the Check-in dialog for story asset. where user can set the asset name

					//InterfacePtr<IZPAssetData> theStoryAssetData( dataLink, UseDefaultIID() );
					ASSERT(theStoryAssetData);

					InterfacePtr<IZPAssetCheckInData> theAssetCheckInData( theStoryAssetData, UseDefaultIID() );
					ASSERT(theAssetCheckInData);

#if 0				//Commented as this is giving name that is displayed in links panel
					InterfacePtr<IAssignment> theAssignment;
					InterfacePtr<IAssignedStory>  iAssignedStory;
					iAsgnMgr->QueryAssignmentAndAssignedStory  ( asgnStoryRef, theAssignment, iAssignedStory);
					ASSERT(inAssignment == theAssignment.get() && iAssignedStory);

					const PMString & storyName = iAssignedStory->GetName();
#else
					InterfacePtr<IStoryLabel> storyLabel( storyRef, UseDefaultIID() );
					ASSERT(storyLabel);
					const PMString storyName = storyLabel->GetLabel();
#endif
					theAssetCheckInData->SetAssetName( storyName );

					PMString checkInComments;
					IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( storyAssetRef, checkInComments );
					if(followupAction == IAMUIService::enCancel || followupAction == IAMUIService::enNone )
						break;

					//Update the story asset name & status
					theAssetCheckInData->SetStatusID( inNewStatusID);

					Utils<IZPAMHelper>()->UpdateAssetInfoOnServer( theStoryAssetData, theAssetCheckInData, theDBAssetID );
					break;	//Already updated with new status.
				}
				else
				{
					amHelperUtils->MakeSureStoryStatus( theDBAssetID, inNewStatusID );
				}
			}
		} while (kFalse);
	}
}

//----------------------------------------------------------------------------------------
// CountLinkExistForAssetID
//----------------------------------------------------------------------------------------
int
CZPAssetDocUtils::CountLinkExistForAssetID(
	const IDocument *		inDocument,
	const PMString &		inAssetID) const
{
	int result = 0;
	do
	{
		if(!inDocument)
			break;

		if(inAssetID.IsEmpty())
			break;

		InterfacePtr<ILinksManager> linksMgr(inDocument , UseDefaultIID() );
		if(!linksMgr)
			break;

		int32 linkNum = linksMgr->GetLinkCount() ;
		for (int32 i = 0 ; i < linkNum ; i++)
		{
			UIDRef linkUIDRef  = linksMgr->GetNthLinkUID(i);
			InterfacePtr<const IZPAssetData> dataLinkAssetData( linkUIDRef, UseDefaultIID() );
			ASSERT( dataLinkAssetData );

			if( !dataLinkAssetData )
				continue;

			const PMString & assetID = dataLinkAssetData->GetAssetID();
			if( assetID.IsEmpty() == kFalse && assetID.Compare(kFalse, inAssetID) == 0)
			{
				++result;
			}
		}
	}while(false);

	return result;
}

//----------------------------------------------------------------------------------------
// IsNewAssignmentAllowed
//----------------------------------------------------------------------------------------
bool
CZPAssetDocUtils::IsNewAssignmentAllowed(
	const IDocument *		inDocument) const
{
	bool toReturn = true;
	do
	{
		//If this is a InDesign Template then don't allow new assignments to be created.

		InterfacePtr<const IZPAssetData> theDocAssetData( inDocument, UseDefaultIID() );
		if( !theDocAssetData )
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theDocAssetData->GetAssetID() );
		if( !dbAssetID )
			break;

		InterfacePtr<const IZPAssetInfo> assetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( assetInfo );
		toReturn = ( assetInfo->GetType() != eAssetType_IDTemplate );

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetIdsUsedInDoc
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::GetAssetIdsUsedInDoc(
	const IDocument *		inDocument,
	ZPAssetIDsList &		oAssetIdList) const
{
	this->GetAssignmentAssetIdsUsedInDoc(inDocument, oAssetIdList);
	this->GetStoryAssetIdsUsedInDoc(inDocument, oAssetIdList);
}

//----------------------------------------------------------------------------------------
// GetAssignmentAssetIdsUsedInDoc
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::GetAssignmentAssetIdsUsedInDoc(
	const IDocument *		inDocument,
	ZPAssetIDsList &		oAssetIdList) const
{
	if(!inDocument)
		return;

	InterfacePtr<IAssignedDocument> assignmentDoc(inDocument, UseDefaultIID());
	ASSERT(assignmentDoc);

	IDataBase* db = ::GetDataBase(inDocument);
	if(!db)
		return;
	
	// relink assignment files
	UIDList assignmentList = assignmentDoc->GetAssignments();
	for (int32 i = 0 ; i < assignmentList.Length(); ++i) 
	{
		UIDRef assignmentref(db, assignmentList[i]);
		
		InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
		ASSERT(assignment);

		InterfacePtr<IDataLink> dataLink(assignment, UseDefaultIID());
		if(!dataLink)
			continue;

		InterfacePtr<IZPAssetData> asset(dataLink, UseDefaultIID());
		if(!asset)
			continue;

		if (! asset->GetAssetID().IsEmpty())
			oAssetIdList.push_back(asset->GetAssetID());
	}
}

//----------------------------------------------------------------------------------------
// GetStoryAssetIdsUsedInDoc
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::GetStoryAssetIdsUsedInDoc(
	const IDocument *		inDocument,
	ZPAssetIDsList &		oAssetIdList) const
{
	if(!inDocument)
		return;

	InterfacePtr<IStoryList> iStoryList(inDocument, UseDefaultIID());
	if(!iStoryList)
		return;

	IDataBase* db = ::GetDataBase(inDocument);
	if(!db)
		return;

	int storyCount = iStoryList->GetUserAccessibleStoryCount();
	for(int i = 0; i < storyCount; i++)
	{
		UIDRef storyRef = iStoryList->GetNthUserAccessibleStoryUID(i);

		do {
			if( storyRef.GetUID() == kInvalidUID )
				break;
#if defined(InDnCS5) || defined(InDnCS5_5)
			UIDRef storyAssetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
			if( storyAssetRef.GetUID() == kInvalidUID )
				break;

			InterfacePtr<IZPAssetData> asset(storyAssetRef, UseDefaultIID());
			if(!asset)
				break;
#else
			InterfacePtr<IDataLinkReference> dataReference(storyRef, UseDefaultIID());
			if(!dataReference)
				break;

			UID dataLinkUID = dataReference->GetUID();
			if( dataLinkUID == 0 )
				break;
			InterfacePtr<IDataLink> dataLink(db, dataLinkUID, UseDefaultIID());
			if(!dataLink)
				break;

			InterfacePtr<IZPAssetData> asset(dataLink, UseDefaultIID());
			if(!asset)
				break;
#endif

			if (! asset->GetAssetID().IsEmpty())
				oAssetIdList.push_back(asset->GetAssetID());
		} while (kFalse);
	}
}

//----------------------------------------------------------------------------------------
// IsOurAssetDocument
//----------------------------------------------------------------------------------------
bool
CZPAssetDocUtils::IsOurAssetDocument(
	const IDocument *		inDocument) const
{
	bool toReturn = false;
	do
	{
		if (!inDocument)
			break;

		InterfacePtr<IZPAssetData> asset(inDocument, UseDefaultIID());
		if(!asset)
			break;

		if (! asset->GetAssetID().IsEmpty())
			toReturn = true;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ValidateAssignmentsName
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::ValidateAssignmentsName(
	const IDocument *		inDocument) const
{
	if(!inDocument)
		return;

	InterfacePtr<IAssignedDocument> assignmentDoc(inDocument, UseDefaultIID());
	ASSERT(assignmentDoc);

	IDataBase* db = ::GetDataBase(inDocument);
	if(!db)
		return;

	Utils<IZPDocUtils> docUtils;
	UIDList assignmentList = assignmentDoc->GetAssignments();
	for (int32 i = 0 ; i < assignmentList.Length(); ++i) 
	{
		UIDRef assignmentRef(db, assignmentList[i]);
		
		InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
		ASSERT(assignment);

		if( assignment->GetFile().IsEmpty() )
			continue;

		InterfacePtr<IZPAssetData> asset(assignment, UseDefaultIID());
		if(!asset)
			continue;

		const IStringData * dbAssetId = ZPDataHelper::GetAssetByID( asset->GetAssetID() );
		if( !dbAssetId )
			continue;

		InterfacePtr<const IStringData> assignmentName( dbAssetId, IID_IZPDATANAME );
		ASSERT( assignmentName );
		const PMString & assignmentNameStr = assignmentName->Get();
		docUtils->SetAssignmentName(assignmentRef, assignmentNameStr, kFalse);
	}
}

//----------------------------------------------------------------------------------------
// ValidateStoriesName
//----------------------------------------------------------------------------------------
void
CZPAssetDocUtils::ValidateStoriesName(
	const IDocument *		inDocument) const
{
	if(!inDocument)
		return;

	InterfacePtr<IStoryList> iStoryList(inDocument, UseDefaultIID());
	if(!iStoryList)
		return;

	IDataBase* db = ::GetDataBase(inDocument);
	if(!db)
		return;

	Utils<IZPStoryUtils> storyUtils;
	int storyCount = iStoryList->GetUserAccessibleStoryCount();
	for(int i = 0; i < storyCount; i++)
	{
		UIDRef storyRef = iStoryList->GetNthUserAccessibleStoryUID(i);
		UIDRef assetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
		InterfacePtr<IZPAssetData> asset(assetRef, UseDefaultIID());
		if(!asset)
			continue;

		const IStringData * dbAssetId = ZPDataHelper::GetAssetByID( asset->GetAssetID() );
		if( !dbAssetId )
			continue;

		InterfacePtr<const IStringData> storyName( dbAssetId, IID_IZPDATANAME );
		ASSERT( storyName );
		const PMString & storyNameStr = storyName->Get();
		storyUtils->SetStoryName(storyRef, storyNameStr);
	}
}