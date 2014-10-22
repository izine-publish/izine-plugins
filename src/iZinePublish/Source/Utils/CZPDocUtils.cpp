//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPDocUtils.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IAssignmentMgr.h"
#include "IAssignSetPropsCmdData.h"
#include "IBoolData.h"
#include "ICommand.h"
#include "IDataLinkReference.h"
#include "IDocFileType.h"
#include "IDocSavePrefsCmdData.h"
#include "IDocument.h"
#include "IDocumentCommands.h"
#ifdef InDnCS3
#include "IInCopyBridgeCmdSuite.h"
#endif
#include "ILinkObjectReference.h"
#include "ILinksManager.h"
#include "IManageableAsset.h"
#include "IOutputPages.h"
#include "IPageList.h"
#include "IPDFExportPrefs.h"
#include "IPMStream.h"
#include "IStandAloneDoc.h"
#include "IStoryList.h"
#include "IStringData.h"
#include "IStyleGroupHierarchy.h"
#include "IStyleGroupInfo.h"
#include "IStyleGroupManager.h"
#include "IStyleInfo.h"
#include "ISysFileData.h"
#include "IUIDData.h"
#include "IUIFlagData.h"
#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILiveEditFacade.h"
#include "IStoryUtils.h"
#include "ILinkFacade.h"
#include "FileUtils.h"
#endif

//SDK General includes
#include "AssignmentID.h"
#include "documentID.h"
#include "PreferenceUtils.h"
#include "SDKLayoutHelper.h"
#include "SnapshotUtilsEx.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetUtils.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPDocUtils>
class CZPDocUtils : public inheritClass
{
public:
						CZPDocUtils(IPMUnknown * boss);
	virtual				~CZPDocUtils();

	ErrorCode			DisablePreviewGeneration(
							IDocument *					inDoc ) const;

	ErrorCode			SaveDocToFile(
							const IDocument *			inDoc,
							const IDFile &				inFilePath,
							bool						inAsTemplate,
							bool						inPerformSaveAs ) const;

	void				SaveACopy(
							const IDocument *			inDoc,
							const IDFile &				inFilePath,
							UIFlags						inUIFlags,
							FileTypeInfoID				inFileTypeID) const;

	ErrorCode			OpenDocumentFile(
							const IDFile &				inFilePath,
							IOpenFileCmdData::OpenFlags	inOpenFlags,
							UIDRef &					oDocRef ) const;

	ErrorCode			OpenDocumentWindow(
							const UIDRef &				inDocRef ) const;

	ErrorCode			SetDocumentName(
							IDocument *					inDoc,
							const PMString &			inDocName ) const;

	ErrorCode			SetAssignmentName(
							const UIDRef &				inAssignmentRef,
							const PMString &			inNewName,
							bool16						inDirty) const;

	ErrorCode			CreatePageSnapshot(
							const IDocument *			inDoc,
							UID							inPageUID,
							IPMStream *					oStream ) const;

	ErrorCode			CreatePDF(
							const IDocument *			inDoc,
							const IDFile &				inPdfFile,
							const PMString &			inPdfStyleName ) const;

	ErrorCode			CreatePDF(
							const IDocument *			inDoc,
							const IDFile &				inPdfFile,
							const PMString &			inPdfStyleName,
							const UIDList &				inPageUIDList ) const;

	ErrorCode			SetFilePathWithAssignment(
							const UIDRef &				inAssignmentRef,
							const IDFile &				inFile ) const;

	IAssignment *		QueryAssignmentForFilePath(
							const IDocument *			inDoc,
							const IDFile &				inAssignmentFile ) const;

	IAssignment *		QueryAssignmentForAsset(
							const IDocument *			inDoc,
							const IManageableAsset *	inAssignmentAsset ) const;
	
	IAssignment *		QueryAssignmentFromAssignmentDoc(
							const IDocument *			inAssignmentDoc ) const;

	ErrorCode			RemoveAllAssignmentsOnlyFromDoc(
								const IDocument *			inDoc) const;

	void				AllAssignmentsSetDirty(
								const IDocument *			inDoc,
								bool						inIsDirty) const;

	IManageableAsset *	QueryManageableAssetForAssignment(
							const IDocument *			inDoc,
							const IAssignment *			inAssignment ) const;

	IDocumentList *		QueryOpenDocList() const;

	void				EnsureLatestAllAssignments(
							const IDocument *			inDoc) const;
	void				EnsureLatestAllLinkedStories(
							const IDocument *			inDoc) const;
	ErrorCode			RemoveAllStoriesOnlyFromDoc(
								const IDocument *			inDoc) const;
	void				RemoveAllStyles(
							const UIDRef				inDocRef,
							const PMIID &				inStyleGroupManager) const;

	bool				CanCheckOutAnyStory(
								const IDocument *			inDoc) const;
	void				CheckOutAllStories(
								const IDocument *			inDoc) const;

	IAssignment *		QueryAssignmentByName(
							const IDocument *			inDoc,
							const PMString &			inAssignmentName ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPDocUtils, kZPDocUtilsImpl)

CZPDocUtils::CZPDocUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDocUtils::~CZPDocUtils()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// DisablePreviewGeneration
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::DisablePreviewGeneration(
	IDocument *					inDoc) const
{
	ErrorCode toReturn = kSuccess;

	InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kSetDocSavePrefsCmdBoss));
	if( !cmd )
		return kFailure;

	cmd->SetUndoability(ICommand::kAutoUndo);

	InterfacePtr<IDocSavePrefsCmdData> cmddata(cmd, UseDefaultIID());
	cmddata->SetDocument( inDoc );
#if defined(InDnCS5) || defined(InDnCS5_5)
	cmddata->SetThumbnailOptions(IDocSavePrefs::kFirstPageJPEGThumbnail);
	cmddata->SetSaveThumbnailWithDocument(kFalse);
#else
	cmddata->SetThumbnailOptions(IDocSavePrefs::kNoThumbnail);
	cmddata->SetPreviewOptions(IDocSavePrefs::kNoPreview);
#endif

	InterfacePtr<IDocSavePrefs> iDocSavePrefs( (IDocSavePrefs*)::QuerySessionPreferences(IID_IDOCSAVEPREFS) );
	cmddata->SetPreviewSizeOptions(iDocSavePrefs->GetPreviewSizeOptions());

	toReturn = CmdUtils::ProcessCommand( cmd );
	//TODO: check the cmd status.
	return toReturn;
}


//----------------------------------------------------------------------------------------
// SaveDocToFile
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::SaveDocToFile(
	const IDocument *			inDoc,
	const IDFile &				inFilePath,
	bool						inAsTemplate,
	bool						inPerformSaveAs ) const
{
	LogFunctionEnterExit;
	ASSERT( inDoc );

	ClassID cmdClass = kSaveDocCmdBoss;	//or kInCopySaveDocCmdBoss?
	if( inPerformSaveAs )
		cmdClass = kSaveAsDocCmdBoss;	//or kInCopySaveAsDocCmdBoss?

	InterfacePtr<ICommand> saveCmd;
	//saveCmd.reset(CmdUtils::CreateCommand(cmdClass));
	if( inPerformSaveAs )
		saveCmd.reset(Utils<IDocumentCommands>()->CreateSaveAsCommand());
	else
		saveCmd.reset(Utils<IDocumentCommands>()->CreateSaveCommand( ::GetUIDRef( inDoc ) ));
	//saveCmd.reset(Utils<IDocumentCommands>()->CreateSaveACopyCommand());

	ASSERT( saveCmd );
	//For Debugging, which class is created for saving the document
	//ClassID classCreatedOfBoss = ::GetClass( saveCmd );
	//ASSERT( cmdClass.Get() == classCreatedOfBoss.Get() );

	UIDList list(::GetDataBase(inDoc), ::GetUID(inDoc));
	saveCmd->SetItemList(list);
	
	InterfacePtr<IUIFlagData> uiFlagData( saveCmd, UseDefaultIID());
	ASSERT( uiFlagData );
	uiFlagData->Set();

	InterfacePtr<ISysFileData> sysFileData( saveCmd, UseDefaultIID());
	ASSERT( sysFileData );
	if( sysFileData )
		sysFileData->Set( inFilePath );
	
	// stationary
	InterfacePtr<IBoolData> boolData( saveCmd, UseDefaultIID());
	
	if( boolData )	//BoolData is not available on SaveACopy command.
		boolData->Set( inAsTemplate );

	InterfacePtr<IStandAloneDoc> standAlone(inDoc, IID_ISTANDALONEDOC);
	if (standAlone && standAlone->IsStandAloneDoc())
	{	//TODO: In InCopy document must be standalone, otherwise story should be saved instead of document.
	}

	FileTypeInfoID saveTypeID = Utils<IZPAssetUtils>()->GetHostAppDocFileTypeInfo( inAsTemplate );
		

	InterfacePtr<IDocFileType> dType(saveCmd, UseDefaultIID());
	if (dType)
		dType->SetDocFileType( saveTypeID );
	return CmdUtils::ProcessCommand( saveCmd );
}

//----------------------------------------------------------------------------------------
// SaveACopy
//----------------------------------------------------------------------------------------
void
CZPDocUtils::SaveACopy(
	const IDocument *			inDoc,
	const IDFile &				inFilePath,
	UIFlags						inUIFlags,
	FileTypeInfoID				inFileTypeID) const
{
	InterfacePtr<ICommand> cmd(Utils<IDocumentCommands>()->CreateSaveACopyCommand() );
	cmd->SetItemList(UIDList(inDoc));
	
	InterfacePtr<ISysFileData> data(cmd, IID_ISYSFILEDATA);
	data->Set(inFilePath);	
	InterfacePtr<IUIFlagData> uiFlagData(cmd, IID_IUIFLAGDATA);
	uiFlagData->Set(inUIFlags);
	InterfacePtr<IDocFileType> dType(cmd, UseDefaultIID());
	if (dType)
		dType->SetDocFileType(inFileTypeID);
	
	CmdUtils::ProcessCommand(cmd);
}

//----------------------------------------------------------------------------------------
// OpenDocumentFile
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::OpenDocumentFile(
	const IDFile &				inFilePath,
	IOpenFileCmdData::OpenFlags	inOpenFlags,
	UIDRef &					oDocRef) const
{
	ErrorCode toReturn = kFailure;
	if( Utils<IZPInDnUtils>()->IsHostAppInCopy() == kFalse )
	{
		//TODO: try without lock file.
		toReturn = Utils<IDocumentCommands>()->Open(&oDocRef, inFilePath,  kSuppressUI,
											 inOpenFlags,
											 IOpenFileCmdData::kUseLockFile, kFalse);
	}
	else
	{
		#define kInCopyOpenDocCmdBoss_	0x1876a
		ClassID kInCopyOpenDocCmdBoss = kInCopyOpenDocCmdBoss_;
		InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand( kOpenFileCmdBoss ));
		ASSERT(cmd);
		
		//InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand( kInCopyOpenDocCmdBoss ));
		
		InterfacePtr<IOpenFileCmdData> iopendata(cmd, IID_IOPENFILECMDDATA);
		ASSERT(iopendata);	
		
		// Open file using inOpenFlags. IC template will be opened as a copy
		iopendata->Set( inFilePath, kFullUI, inOpenFlags, IOpenFileCmdData::kUseLockFile );
		
		toReturn = CmdUtils::ProcessCommand(cmd);
		
		if( toReturn == kSuccess)
			oDocRef = cmd->GetItemList()->GetRef(0);
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// OpenDocumentWindow
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::OpenDocumentWindow(
	const UIDRef &				inDocRef) const
{
	SDKLayoutHelper layoutHelper;
	return layoutHelper.OpenLayoutWindow( inDocRef );
}

//----------------------------------------------------------------------------------------
// SetDocumentName
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::SetDocumentName(
	IDocument *					inDoc,
	const PMString &			inDocName) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		ASSERT( inDoc );

		PMString oldName;
		inDoc->GetName( oldName );

		if( oldName.Compare( kTrue, inDocName ) == 0 )
			break;
		InterfacePtr<ICommand> setDocNameCmd(CmdUtils::CreateCommand(kSetDocNameCmdBoss));
		ASSERT(setDocNameCmd);
		
		setDocNameCmd->SetItemList(UIDList( ::GetUIDRef(inDoc) ));
		
		InterfacePtr<IStringData> setDocNameCmdStringData( setDocNameCmd, UseDefaultIID() );
		ASSERT( setDocNameCmdStringData );
		setDocNameCmdStringData->Set( inDocName );
		toReturn = CmdUtils::ProcessCommand( setDocNameCmd );
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// SetAssignmentName
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::SetAssignmentName(
	const UIDRef &				inAssignmentRef,
	const PMString &			inNewName,
	bool16						inDirty) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	if( inNewName.IsEmpty())
		return toReturn;

	InterfacePtr<IAssignment> assignment(inAssignmentRef, UseDefaultIID());
	ASSERT(assignment);

	if( assignment->GetFile().IsEmpty() )
		return toReturn;

	const PMString & currentAssignmentName = assignment->GetName();
	if(currentAssignmentName.Compare(kTrue, inNewName) == 0)
		return kSuccess;	//No need to change name

	InterfacePtr<ICommand> cmd( CmdUtils::CreateCommand(kAssignSetPropsCmdBoss) );
	ASSERT(cmd);

	UIDList list( inAssignmentRef );
	cmd->SetItemList(list);
	cmd->SetUndoability( ICommand::kAutoUndo );
	InterfacePtr<IAssignSetPropsCmdData> cmddata( cmd, UseDefaultIID() );

	cmddata->Set(&inNewName /* name */, nil/* file */, nil /* user */, nil /* color */, nil /* options */, &inDirty /* dirty */, nil /* includeLinkedWhenPackage */);
	
	return CmdUtils::ProcessCommand(cmd);
}

//----------------------------------------------------------------------------------------
// QueryOpenDocList
//----------------------------------------------------------------------------------------
IDocumentList *
CZPDocUtils::QueryOpenDocList()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> theApp(gSession->QueryApplication());
		
	return theApp->QueryDocumentList();
}



#pragma mark -
//----------------------------------------------------------------------------------------
// CreatePageSnapshot
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::CreatePageSnapshot(
	const IDocument *			inDoc,
	UID							inPageUID,
	IPMStream *					oStream) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		int32 flags =  IShape::kDrawFrameEdge | IShape::kPrinting | IShape::kPreviewMode;

		VPAttrMap vpAttrs;

		SnapshotUtilsEx fSnapshotUtils(UIDRef(::GetDataBase(inDoc), inPageUID));
		if (fSnapshotUtils.Draw(flags, kTrue, PMReal(7.0) ,kTrue,SnapshotUtils::kXPHigh, nil, nil, kFalse) != kSuccess)
			break;
		
		toReturn = fSnapshotUtils.ExportImageToJPEG(oStream, SnapshotUtils::kSSJPEGBaseline, SnapshotUtils::kSSJPEGLowQuality);
		
		oStream->Close();
	}while(false);

	return toReturn;
}


//----------------------------------------------------------------------------------------
// CreatePDF
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::CreatePDF(
	const IDocument *			inDoc,
	const IDFile &				inPdfFile,
	const PMString &			inPdfStyleName) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		// Get the pages to be exported and set them on the command's output pages interface
		InterfacePtr<IPageList> thePageList( inDoc, UseDefaultIID());
		ASSERT( thePageList != nil );
		if (!thePageList)
			break;
		
		UIDList thePageUIDList( ::GetDataBase( inDoc ) );
		for(int32 iPage = 0; iPage < thePageList->GetPageCount(); ++iPage)
			thePageUIDList.Append(thePageList->GetNthPageUID(iPage));

		toReturn = this->CreatePDF( inDoc, inPdfFile, inPdfStyleName, thePageUIDList );
	}while(false);
	return toReturn;

}
//----------------------------------------------------------------------------------------
// CreatePDF
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::CreatePDF(
	const IDocument *			inDoc,
	const IDFile &				inPdfFile,
	const PMString &			inPdfStyleName,
	const UIDList &				inPageUIDList) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode toReturn = kFailure;
	do
	{
		// create pdf
		InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
		ASSERT( theWorkspace != nil );
		
		InterfacePtr<IPDFExportPrefs> thePDFExportWorkspacePrefs(theWorkspace, UseDefaultIID());
		ASSERT_MSG(thePDFExportWorkspacePrefs != nil, "Could not get IPDFExportPrefs from workspace");
		if (!thePDFExportWorkspacePrefs)
			break;
		
		// Create a PDF Export Command
		InterfacePtr<ICommand> thePDFExportCmd(CmdUtils::CreateCommand(kPDFExportCmdBoss));
		ASSERT_MSG(thePDFExportCmd != nil, "Could not create PDF Export ICommand");
		if (!thePDFExportCmd)
			break;
		
		// Get the PDF Export Preferences interface
		InterfacePtr<IPDFExportPrefs> thePDFExportPrefs(thePDFExportCmd, UseDefaultIID());
		ASSERT_MSG(thePDFExportPrefs != nil, "Could not get IPDFExportPrefs");
		if (!thePDFExportPrefs)
			break;
		
		// check if profile exists, probably not necessary, use the last style used for creating PDF.
		int pdfStyleIndex = Utils<IZPInDnUtils>()->GetPDFStyleIndex( inPdfStyleName );	//Should we use inDoc here?
		if( pdfStyleIndex == -1 )
			pdfStyleIndex = Utils<IZPInDnUtils>()->GetCurrentPDFExportStyleIndex();

		if( pdfStyleIndex == -1 )
			pdfStyleIndex = 0;	//Use the first one.

#if 0
		PMString pdfStyleName = Utils<IZPInDnUtils>()->GetPDFExportStyleName( pdfStyleIndex );

		thePDFExportPrefs->CopyPrefs(thePDFExportWorkspacePrefs);

		//if ( pdfStleIndex != -1 )
		thePDFExportPrefs->SetPDFXProfileSelectorAndName(IPDFExportPrefs::kExportPDFDestProfileUseName, pdfStyleName);
#else
		UIDRef styleRef = Utils<IZPInDnUtils>()->GetPDFExportStyle( pdfStyleIndex );

		if( styleRef.GetUID() != kInvalidUID )
		{
			InterfacePtr<IPDFExportPrefs> thePDFStyleExportPrefs(styleRef, UseDefaultIID());
			ASSERT_MSG(thePDFStyleExportPrefs != nil, "Could not get IPDFExportPrefs of the style");
			if (thePDFStyleExportPrefs)
			{
				thePDFExportPrefs->CopyPrefs(thePDFStyleExportPrefs);
			}
			else
			{
				thePDFExportPrefs->CopyPrefs(thePDFExportWorkspacePrefs);
			}
		}
		else
		{
			thePDFExportPrefs->CopyPrefs(thePDFExportWorkspacePrefs);
		}
#endif
		
		// Specify the output file
		InterfacePtr<ISysFileData> theFileData(thePDFExportCmd, UseDefaultIID());
		ASSERT( theFileData != nil );
		theFileData->Set( inPdfFile );
		
		InterfacePtr<IOutputPages> theOutputPages(thePDFExportCmd, UseDefaultIID());
		ASSERT( theOutputPages != nil );
		if (!theOutputPages)
			break;
		
		theOutputPages->InitializeFrom( inPageUIDList, kFalse );
		
		// Process the command
		toReturn = CmdUtils::ProcessCommand(thePDFExportCmd);
	}while(false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// SetFilePathWithAssignment
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::SetFilePathWithAssignment(
	const UIDRef &				inAssignmentRef,
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	InterfacePtr<ICommand> cmd( CmdUtils::CreateCommand(kAssignSetPropsCmdBoss) );
	ASSERT(cmd);

	UIDList list( inAssignmentRef );
	cmd->SetItemList(list);
	cmd->SetUndoability( ICommand::kAutoUndo );
	InterfacePtr<IAssignSetPropsCmdData> cmddata( cmd, UseDefaultIID() );
	
	PMString pathStr;
	Utils<IZPFileUtils>()->ConvertIDFileToStr( inFile, pathStr );
	
	bool16 dirty = kFalse;
	cmddata->Set(nil /* name */, &pathStr/* file */, nil /* user */, nil /* color */, nil /* options */, &dirty /* dirty */, nil /* includeLinkedWhenPackage */);
	
	InterfacePtr<IBoolData> interceptorFlag(cmd , IID_IZPRUNBYPUBLISH );
	if (interceptorFlag)
		interceptorFlag->Set(kTrue);

	return CmdUtils::ProcessCommand(cmd);
}

//----------------------------------------------------------------------------------------
// QueryAssignmentForFilePath
//----------------------------------------------------------------------------------------
IAssignment *
CZPDocUtils::QueryAssignmentForFilePath(
	const IDocument *			inDoc,
	const IDFile &				inAssignmentFile) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IAssignedDocument> iAssignedDoc(inDoc, UseDefaultIID());
		if(!iAssignedDoc)
			break;

		InterfacePtr<IAssignmentMgr> iAssMgr(gSession, UseDefaultIID());
		if(!iAssMgr)
			break;

		PMString strFilePath;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( inAssignmentFile, strFilePath );
		
		return iAssMgr->QueryAssignment(iAssignedDoc, strFilePath);
	}while( false );
	return nil;
}

//----------------------------------------------------------------------------------------
// QueryAssignmentForAsset
//----------------------------------------------------------------------------------------
IAssignment *
CZPDocUtils::QueryAssignmentForAsset(
	const IDocument *			inDoc,
	const IManageableAsset *	inAssignmentAsset) const
{
	IAssignment * toReturn = nil;
	InterfacePtr<const ILinkObjectReference> linkObjectRef( inAssignmentAsset, UseDefaultIID() );
	if( linkObjectRef && linkObjectRef->GetUID() != kInvalidUID )
	{
		UID assignmentUID = linkObjectRef->GetUID();
		InterfacePtr<IAssignment> assignment( reinterpret_cast<IAssignment*>( linkObjectRef->QueryLinkedObject( IAssignment::kDefaultIID)) );
		ASSERT( assignment );
		toReturn = assignment.forget();
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// QueryAssignmentFromAssignmentDoc
//----------------------------------------------------------------------------------------
IAssignment *
CZPDocUtils::QueryAssignmentFromAssignmentDoc(
	const IDocument *			inAssignmentDoc) const
{
	do
	{
		InterfacePtr<IAssignedDocument> iAssignedDoc( inAssignmentDoc, UseDefaultIID());
		if(!iAssignedDoc)
			break;
		UIDList assignments = iAssignedDoc->GetAssignments();
		if( assignments.size() == 1 )	//For InCopy document/template it is zero.
		{
			UID assignmentUID = assignments[0];
			InterfacePtr<IAssignment>	iAssignment (GetDataBase(inAssignmentDoc), assignmentUID, UseDefaultIID ());
			return iAssignment.forget();
		}
	}while(false);
	return nil;
}

//----------------------------------------------------------------------------------------
// QueryManageableAssetForAssignment
//----------------------------------------------------------------------------------------
IManageableAsset *
CZPDocUtils::QueryManageableAssetForAssignment(
	const IDocument *			inDoc,
	const IAssignment *			inAssignment) const
{
	IManageableAsset * toReturn = nil;
	do
	{
		InterfacePtr<ILinksManager> docLinksMgr( inDoc, UseDefaultIID() );
		UID assignmentUID = ::GetUID( inAssignment );

		int linksCount = docLinksMgr->GetLinkCount();
		for( int i = 0 ; i < linksCount; ++i )
		{
			InterfacePtr<const ILinkObjectReference> linkObjectRef( docLinksMgr->GetNthLinkUID(i), UseDefaultIID() );
			if( linkObjectRef && linkObjectRef->GetUID() == assignmentUID )
			{
				InterfacePtr<IManageableAsset> manageableAsset( linkObjectRef, UseDefaultIID() );
				ASSERT( manageableAsset );
				toReturn = manageableAsset.forget();
			}
		}

	}while( false );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveAllAssignmentsOnlyFromDoc
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::RemoveAllAssignmentsOnlyFromDoc(
	const IDocument *			inDoc) const
{
	ErrorCode result = kFailure;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IAssignedDocument> assignedDocument(inDoc, UseDefaultIID());
	ASSERT(assignedDocument);
	
	IDataBase *db = ::GetDataBase(inDoc);
	
	UIDList assignedListInDoc(::GetUIDRef(assignedDocument));
	UIDList assignmentList = assignedDocument->GetAssignments();
	for (int32 i = 0 ; i < assignmentList.Length(); ++i)
	{
		UIDRef assignmentRef(db, assignmentList[i]);
		
		InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
		ASSERT(assignment);
		
		const PMString & filePath = assignment->GetFile();
		if (filePath.IsEmpty())
			continue;

		// remove assignment from document
		InterfacePtr<ICommand> unassignDocCmd(CmdUtils::CreateCommand(kUnassignDocCmdBoss));
		unassignDocCmd->SetItemList(assignedListInDoc);
		InterfacePtr<IStringData> sd(unassignDocCmd, UseDefaultIID());
		sd->Set(assignment->GetFile());
		result = CmdUtils::ProcessCommand(unassignDocCmd);
	}

	InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
	assMgr->NotifyPanel();

	return result;
}

//----------------------------------------------------------------------------------------
// AllAssignmentsSetDirty
//----------------------------------------------------------------------------------------
void
CZPDocUtils::AllAssignmentsSetDirty(
	const IDocument *			inDoc,
	bool						inIsDirty) const
{
	InterfacePtr<IAssignedDocument> assignedDocument(inDoc, UseDefaultIID());
	ASSERT(assignedDocument);
	
	IDataBase *db = ::GetDataBase(inDoc);
	
	UIDList assignedListInDoc(::GetUIDRef(assignedDocument));
	UIDList assignmentList = assignedDocument->GetAssignments();
	for (int32 i = 0 ; i < assignmentList.Length(); ++i)
	{
		UIDRef assignmentRef(db, assignmentList[i]);
		
		InterfacePtr<IAssignment> assignment(assignmentRef, UseDefaultIID());
		ASSERT(assignment);
		
		const PMString & filePath = assignment->GetFile();
		if (filePath.IsEmpty())
			continue;

		assignment->SetDirty(inIsDirty);
	}
}

//----------------------------------------------------------------------------------------
// EnsureLatestAllAssignments
//----------------------------------------------------------------------------------------
void
CZPDocUtils::EnsureLatestAllAssignments(
	const IDocument *			inDoc) const
{
	if(!inDoc)
		return;

	InterfacePtr<IAssignedDocument> assignmentDoc(inDoc, UseDefaultIID());
	ASSERT(assignmentDoc);

	IDataBase* db = ::GetDataBase(inDoc);
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

#if defined(InDnCS5) || defined(InDnCS5_5)				
		PMString assgnFile = assignment->GetFile();
		IDFile file;
		FileUtils::PMStringToIDFile(assgnFile, file);
		Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(file);
#else
		Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(dataLink);
#endif		
	}
}

//----------------------------------------------------------------------------------------
// EnsureLatestAllLinkedStories
//----------------------------------------------------------------------------------------
void
CZPDocUtils::EnsureLatestAllLinkedStories(
	const IDocument *			inDoc) const
{
	if(!inDoc)
		return;

	InterfacePtr<IStoryList> iStoryList(inDoc, UseDefaultIID());
	if(!iStoryList)
		return;

	IDataBase* db = ::GetDataBase(inDoc);
	if(!db)
		return;

	UIDList storyList(db);
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
			Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(storyAssetRef);
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

			Utils<Facade::IWorkgroupFacade>()->EnsureLatestVersion(dataLink);
#endif
		} while (kFalse);
	}
}

//----------------------------------------------------------------------------------------
// RemoveAllStoriesOnlyFromDoc
//----------------------------------------------------------------------------------------
ErrorCode
CZPDocUtils::RemoveAllStoriesOnlyFromDoc(
	const IDocument *			inDoc) const
{
	ErrorCode result = kFailure;

	IDataBase* db = ::GetDataBase(inDoc);
	UIDList links(db);

	// remove story links
#if defined(InDnCS5) || defined(InDnCS5_5)
	links = Utils<IStoryUtils>()->GetStoryLinks(inDoc);
	if( links.size() )
		result = Utils<Facade::ILinkFacade>()->DeleteLinks(links, false, false);
#else	
	InterfacePtr<IStoryList> storyList (inDoc, UseDefaultIID());
	
	Utils<Facade::IWorkgroupStoryFacade> workGroupFacade;
	int storyCount = storyList->GetUserAccessibleStoryCount();
	for(int i = 0; i < storyCount; i++)
	{
		UIDRef storyRef = storyList->GetNthUserAccessibleStoryUID(i);
		
		UIDRef assetUIDRef = workGroupFacade->GetAssetUIDRef(storyRef);
		if (assetUIDRef.GetUID() != 0)
			links.push_back(assetUIDRef.GetUID());
	}
	
	InterfacePtr<ICommand> removeDataLink (CmdUtils::CreateCommand(kRemoveLinksCmdBoss));
	if(links.size() && removeDataLink)
	{
		removeDataLink->SetItemList(links);
		removeDataLink->SetUndoability(ICommand::kAutoUndo);
		result = CmdUtils::ProcessCommand(removeDataLink);
	}	
#endif	
	return result;
}

//----------------------------------------------------------------------------------------
// RemoveAllStyles
//----------------------------------------------------------------------------------------
void
CZPDocUtils::RemoveAllStyles(
	const UIDRef				inDocRef,
	const PMIID &				inStyleGroupManager) const
{
	do
	{
		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if( !theDocument )
			break;

		InterfacePtr<IWorkspace> workspace(theDocument->GetDocWorkSpace(), UseDefaultIID());
		if( !workspace )
			break;

		K2Vector<UID> stylesToDelete;
		IDataBase * db = ::GetDataBase(theDocument);
		InterfacePtr<IStyleGroupManager> styleGroupMgr(workspace, inStyleGroupManager);

		//Styles
		UIDList uids(::GetDataBase(workspace));
		styleGroupMgr->GetRootHierarchy()->GetDescendents(&uids, IID_ISTYLEINFO);
		int32 count = uids.Length();
		for (int32 i = 0; i < count; ++i)
		{
			UID styleUID = uids[i];
			InterfacePtr<IStyleInfo> styleInfo(db, styleUID, UseDefaultIID());
			UID basedOnUID = styleInfo->GetBasedOn();
			if( basedOnUID == kInvalidUID || basedOnUID == styleUID || styleInfo->GetName() == "NormalParagraphStyle")	//Skip defaults
				continue;

			stylesToDelete.push_back(styleUID);
		}

		K2Vector<UID>::iterator iter = stylesToDelete.begin();
		K2Vector<UID>::iterator endIter = stylesToDelete.end();
		while( iter != endIter )
		{
			UID styleUIDToDelete = *iter;
			InterfacePtr<IStyleInfo> styleInfo(db, styleUIDToDelete, UseDefaultIID());

#ifdef DEBUG
			PMString styleName = styleInfo->GetName();
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Deleting style : %s", styleName.GrabCString() );
#endif

			do
			{
				ClassID styleDeleteCmdID;
				if (styleInfo->GetStyleType() == IStyleInfo::kParagraphStyle)
					styleDeleteCmdID = kDeleteParaStyleCmdBoss;
				else if (styleInfo->GetStyleType() == IStyleInfo::kCharacterStyle)
					styleDeleteCmdID = kDeleteCharStyleCmdBoss;
				else
					break;

				InterfacePtr<ICommand> deleteStyleCmd(CmdUtils::CreateCommand(styleDeleteCmdID));
				if (!deleteStyleCmd)
					break;

				// Set the ItemList to point at this workspace
				deleteStyleCmd->SetItemList(UIDList(workspace));

				// Get an IUIDData Interface for the command:
				InterfacePtr<IUIDData> uidData(deleteStyleCmd, UseDefaultIID());
				if (!uidData)
					break;

				// Set the IUIDData Interface’s data to indicate the style we want to delete
				uidData->Set(::GetDataBase(workspace), styleUIDToDelete);

				// Get an IUIDData Interface for the command:
				//InterfacePtr<IUIDData> uidReplaceData(deleteStyleCmd, IID_IREPLACEUIDDATA);
				//if (!uidReplaceData)
				//	break;

				// Set the IUIDData Interface’s data to indicate the style we want to delete
				//const UID  replaceStyle = kInvalidUID;
				//uidReplaceData->Set(::GetDataBase(workspace), replaceStyle);

				// Process the DeleteParaStyleCmd:
				CmdUtils::ProcessCommand(deleteStyleCmd);
			}while(kFalse);

			++iter;
		}

		//Style groups
		UIDList groupUIDs(::GetDataBase(workspace));
		UIDList groupUIDsToDelete(db);
		styleGroupMgr->GetRootHierarchy()->GetDescendents(&groupUIDs, IID_ISTYLEGROUPINFO);
		int32 groupCount = groupUIDs.Length();
		for (int32 i = 0; i < groupCount; ++i)
		{
			UID groupStyleUID = groupUIDs[i];
			InterfacePtr<IStyleGroupInfo> groupStyleInfo(db, groupStyleUID, UseDefaultIID());
			if (!groupStyleInfo)
				continue;

			groupUIDsToDelete.Append(groupStyleUID);
#ifdef DEBUG
			PMString groupStyleName = groupStyleInfo->GetName();
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Deleting group style : %s", groupStyleName.GrabCString() );
#endif
		}

		if( groupUIDsToDelete.size() == 0 )
			break;

		ClassID groupStyleDeleteCmdID;
		if (inStyleGroupManager == IID_IPARASTYLEGROUPMANAGER)
			groupStyleDeleteCmdID = kDeleteParaStyleGroupCmdBoss;
		else if (inStyleGroupManager == IID_ICHARSTYLEGROUPMANAGER)
			groupStyleDeleteCmdID = kDeleteCharStyleGroupCmdBoss;
		else
			break;

		InterfacePtr<ICommand> deleteGroupStyleCmd(CmdUtils::CreateCommand(groupStyleDeleteCmdID));
		if (!deleteGroupStyleCmd)
			break;

		// Set the ItemList to point at this workspace
		deleteGroupStyleCmd->SetItemList(groupUIDsToDelete);

		// Process the Delete Group Style Cmd:
		CmdUtils::ProcessCommand(deleteGroupStyleCmd);
	} while (false);
}

//----------------------------------------------------------------------------------------
// CanCheckOutAnyStory
//----------------------------------------------------------------------------------------
bool
CZPDocUtils::CanCheckOutAnyStory(
	const IDocument *			inDoc) const
{
	bool toReturn = false;
	do
	{
		if(!inDoc)
			break;

		InterfacePtr<IStoryList> iStoryList(inDoc, UseDefaultIID());
		if(!iStoryList)
			break;

		IDataBase* db = ::GetDataBase(inDoc);
		if(!db)
			break;

		UIDList storyList(db);
		int storyCount = iStoryList->GetUserAccessibleStoryCount();
		for(int i = 0; i < storyCount && toReturn == false; i++)
		{
			UIDRef storyRef = iStoryList->GetNthUserAccessibleStoryUID(i);

			do {
				if( storyRef.GetUID() == kInvalidUID )
					break;

#if defined(InDnCS5) || defined(InDnCS5_5)
				UIDRef storyAssetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
				if( storyAssetRef.GetUID() == kInvalidUID )
					break;
#else
				InterfacePtr<IDataLinkReference> dataReference(storyRef, UseDefaultIID());
				if(!dataReference)
					break;

				UID dataLinkUID = dataReference->GetUID();
				if( dataLinkUID == 0 )
					break;
				UIDRef storyAssetRef( db, dataLinkUID );

				InterfacePtr<IDataLink> dataLink(storyAssetRef, UseDefaultIID());
				if(!dataLink)
					break;
#endif
				if( Utils<Facade::IWorkgroupFacade>()->CanCheckOut(storyAssetRef) )
				{
					toReturn = true;
					break;
				}

			} while (kFalse);
		}

	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckOutAllStories
//----------------------------------------------------------------------------------------
void
CZPDocUtils::CheckOutAllStories(
	const IDocument *			inDoc) const
{
	if(!inDoc)
		return;

	InterfacePtr<IStoryList> iStoryList(inDoc, UseDefaultIID());
	if(!iStoryList)
		return;

	IDataBase* db = ::GetDataBase(inDoc);
	if(!db)
		return;

#ifdef InDnCS3	//TODO: CS5 Porting
	InterfacePtr<IInCopyBridgeCmdSuite> incopyBridgeCmdSuite( ((IDocument *)inDoc)->GetDocWorkSpace(), UseDefaultIID());
#endif

	UIDList storyList(db);
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
#else
			InterfacePtr<IDataLinkReference> dataReference(storyRef, UseDefaultIID());
			if(!dataReference)
				break;

			UID dataLinkUID = dataReference->GetUID();
			if( dataLinkUID == 0 )
				break;
			UIDRef storyAssetRef( db, dataLinkUID );

			InterfacePtr<IDataLink> dataLink(storyAssetRef, UseDefaultIID());
			if(!dataLink)
				break;
#endif
			if( Utils<Facade::IWorkgroupFacade>()->CanCheckOut(storyAssetRef) )
			{
				//Utils<Facade::IWorkgroupFacade>()->CheckOut(storyAssetRef);	//It only unlocks not update stories
				storyList.Append(storyRef.GetUID());
			}
		} while (kFalse);
	}

#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
	if(storyList.size())
		Utils<Facade::ILiveEditFacade>()->CheckOut(storyList, ICommand::kRegularUndo, nil, kFalse, kSuppressUI);
#else
	if (incopyBridgeCmdSuite && storyList.size())
		incopyBridgeCmdSuite->EditInPlace( storyList, kSuppressUI );
#endif
}

//----------------------------------------------------------------------------------------
// QueryAssignmentByName
//----------------------------------------------------------------------------------------
IAssignment *
CZPDocUtils::QueryAssignmentByName(
	const IDocument *			inDoc,
	const PMString &			inAssignmentName) const
{
	IAssignment *toReturn = nil;
	do
	{
		if (!inDoc || inAssignmentName.IsEmpty())
			break;

		InterfacePtr<const IAssignedDocument> assignedDoc(inDoc, UseDefaultIID());
		if (!assignedDoc)
			break;

		UIDList assignmentUIDList = assignedDoc->GetAssignments();
		for (int32 i = 0; i < assignmentUIDList.size(); ++i)
		{
			InterfacePtr<IAssignment> assignment (assignmentUIDList.GetDataBase (), assignmentUIDList[i], UseDefaultIID ());
			if (!(assignment->GetFile().empty()))
			{
				if (inAssignmentName == assignment->GetName())
				{
					toReturn = assignment.forget();
					break;
				}
			}
		}

	}while(kFalse);

	return toReturn;
}
