//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpExportDialogController.cpp $
//	$Revision: 3994 $
//	$Date: 2012-03-23 08:33:55 +0100 (Fri, 23 Mar 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IDocument.h"
#include "IDropDownListController.h"
#include "ILayoutUIUtils.h"
#include "IPanelControlData.h"
#include "IProgressBarManager.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "CDialogController.h"
#include "ErrorUtils.h"
#include "FileUtils.h"
#include "PlatformFileSystemIterator.h"
#include "Utils.h"

//OS / ThirdParty includes
#ifdef MACINTOSH
#include <uuid/uuid.h>
#endif

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpDocUtils.h"
#include "IZExpExportDlgHelper.h"
#include "IZExpFileUtils.h"
#include "IZExpGlobalData.h"
#include "IZExpServerData.h"
#include "IZExpWSFacade.h"
#include "IZExpXMLWriteFacade.h"

//IZP General includes
#include "StUtils.h"
#include "CZExpUploadExportRequest.h"
#include "CZExpUploadExportResponse.h"
#include "CZExpValidateMediaContentRequest.h"
#include "CZExpValidateMediaContentResponse.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZExpExportDialogController : public inheritClass
{
public:
						CZExpExportDialogController(IPMUnknown * boss);
	virtual				~CZExpExportDialogController();

	/** Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
	virtual void		InitializeDialogFields(
							IActiveContext* dlgContext);

	/** Validate the values in the widgets.
		By default, the widget with ID kOKButtonWidgetID causes
		ValidateFields to be called. When all widgets are valid,
		ApplyFields will be called.
		@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

	*/
	virtual WidgetID	ValidateDialogFields(
							IActiveContext* myContext);


	/** Retrieve the values from the widgets and act on them.
		@param widgetId identifies the widget on which to act.
	*/
	virtual void		ApplyDialogFields(
							IActiveContext* myContext, const WidgetID& widgetId);

protected:
		IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;

private:
	bool				UploadImageFilesInDirectory(
							const IDFile &				inFileDirectory,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inArticleID,
							const PMString &			inDocName,
							const bool					inIsHighRes) const;
	bool				UploadPDFFile(
							const IDFile &				inPDFFile,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inArticleID,
							const PMString &			inDocName) const;
	bool				UploadEndOfSession(
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inArticleID,
							const PMString &			inDocName,
							bool						inWithSuccess) const;
	bool16				ValidPath(
							const PMString&				path) const;
};


CREATE_PMINTERFACE(CZExpExportDialogController, kZExpExportDialogControllerImpl)

CZExpExportDialogController::CZExpExportDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpExportDialogController::~CZExpExportDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZExpExportDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;
	
	CDialogController::InitializeDialogFields(dlgContext);
	do
	{
		InterfacePtr<const IZExpExportDlgHelper> exportDlgHelper(this, UseDefaultIID());
		exportDlgHelper->FillTitles();
		exportDlgHelper->FillEditions();

#ifdef ExportWithAdvanceOptions
		SetTriStateControlData(kZExpExportDlgAdvOptionBoth, ITriStateControlData::kSelected);
		SetTriStateControlData(kZExpExportDlgAdvOptionSkipDataCreation, ITriStateControlData::kUnselected);
		SetTriStateControlData(kZExpExportDlgAdvOptionSkipUpload, ITriStateControlData::kUnselected);
#endif

	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZExpExportDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	LogFunctionEnterExit;
	
	WidgetID result = CDialogController::ValidateDialogFields(myContext);
	do
	{
		InterfacePtr<const IZExpExportDlgHelper> exportDlgHelper( this, UseDefaultIID());
		if (exportDlgHelper->GetTitleId().IsEmpty())
		{
			result = kZExpExportTitleDropDownWidgetID;
			break;
		}
		else if (exportDlgHelper->GetEditionId().IsEmpty())
		{
			result = kZExpExportEditionDropDownWidgetID;
			break;
		}
	}while( false );

	return result;
}

PMString
NewGUID()
{
	LogFunctionEnterExit;
	
#ifdef MACINTOSH
	uuid_t uu;
	char uuid_s[37];
	uuid_generate(uu);
	uuid_unparse(uu, uuid_s);
#ifdef InDnCS5
	PMString id(uuid_s, PMString::kDontTranslateDuringCall );
#else	
	PMString id(uuid_s, -1, PMString::kNoTranslate);
#endif

	id.ToLower();
	return (id);
#else
	// see http://trac.xapian.org/ticket/303 for similar issue
	// http://trac.xapian.org/attachment/ticket/303/win32-uuid-cleanup.patch
	const size_t UUID_SIZE = 16;

	do {

		UUID uuid;
		if (UuidCreate(&uuid) != RPC_S_OK)
			break;
		
		RPC_WSTR uuidstr;
		if (UuidToString(&uuid, (RPC_WSTR*)(&uuidstr)) != RPC_S_OK)
			break;
		
		WideString id(uuidstr);
		id.ToLower();

		RpcStringFree((RPC_WSTR*)(&uuidstr));

		return (id);
	} while (kFalse);

	return (kNullString);
#endif
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZExpExportDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	if (widgetId != kOKButtonWidgetID)
		return;

	InterfacePtr<const IZExpExportDlgHelper> exportDlgHelper( this, UseDefaultIID());
	const PMString & titleId = exportDlgHelper->GetTitleId();
	const PMString & editionId = exportDlgHelper->GetEditionId();

	IDocument* theFrontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
	ASSERT(theFrontDoc);
	if (theFrontDoc == nil)
		return;

#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	InterfacePtr<IProgressBarManager> progressBarManager(application, UseDefaultIID());
	int32 taskBarId = -1;
	if (progressBarManager)
	{
		taskBarId = progressBarManager->RegisterTaskSet(IProgressBarManager::kTaskProgress, kZExpProgRedfiveExportTitleKey, 0, 6, kTrue, kTrue);
		progressBarManager->DoTask(kZExpProgCreatingXMLKey);
	}

	PMString dialogMessage("");
	bool isError = false;
	do
	{
		PMString docName;
		theFrontDoc->GetName(docName);
		int nameLength = docName.CharCount();
		if ((nameLength - 5) > 0)		//Remove extension from document name
		{
			const PMString docExtn(".indd");
			int pos = docName.IndexOfString(docExtn, nameLength - 5);
			if (pos > 0)
			{
				PMString *newName = docName.Substring(0, pos);
				if (newName)
				{
					docName = *newName;
					delete newName;
				}
			}
		}

		if(progressBarManager )
			progressBarManager->SetTaskStatus(kZExpProgFetchingConfigKey);

		exportDlgHelper->GetConfig();
		
		ErrorCode errorResult = ErrorUtils::PMGetGlobalErrorCode();
		if( errorResult != kSuccess )
		{
			int clickedButton = CAlert::ModalAlert
			(
				kZExpMsgConfigDataFetchFailKey,	// Alert string
				kYesString, 					// Yes button
				kNoString, 						// No second button
				kNullString, 					// No third button
				1,								// Set Yes button to default
				CAlert::eWarningIcon			// Information icon.
			);

			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			if(clickedButton != 1)
			{
				if(progressBarManager )
				{
					progressBarManager->Abort();
					progressBarManager->UnregisterTaskSet(taskBarId);
				}

				return;
			}
		}

		if(progressBarManager )
			progressBarManager->SetTaskStatus("");

		if(progressBarManager && progressBarManager->WasCancelled())
			break;
		
		InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "ConfigSettings : HighDpi - %d, LowDpi - %d, ExportTiff - %hd, ExportLowRes - %hd, ExportPDF - %hd, PdfStyle - %s",
			gGlobalData->GetDPIHighRes(),  gGlobalData->GetDPILowRes(), (short int)gGlobalData->IsExportTiff(), (short int)gGlobalData->IsExportLowRes(), (short int)gGlobalData->IsExportPDF(), gGlobalData->GetStyleName().GrabCString());

		const PMString xmlExt("xml");
		Utils<IZExpFileUtils> fileUtils;
		IDFile path;
		fileUtils->GetTemporaryFolder(path);
		fileUtils->AppendPath(path, docName, &xmlExt);

		if(progressBarManager && progressBarManager->WasCancelled())
		{
			dialogMessage = kZExpMsgAbortedByUserKey;
			break;
		}

#ifdef ExportWithAdvanceOptions
		if( GetTriStateControlData(kZExpExportDlgAdvOptionBoth) == ITriStateControlData::kSelected
			|| GetTriStateControlData(kZExpExportDlgAdvOptionSkipUpload) == ITriStateControlData::kSelected )
#endif
			errorResult = Utils<IZExpXMLWriteFacade>()->ExportXMLToFile(path, theFrontDoc);		//Export xml to file
		if( errorResult == kSuccess &&	progressBarManager->WasCancelled())
			errorResult = ErrorUtils::PMGetGlobalErrorCode();

		if(errorResult == kCancel)
			break;

		if(errorResult != kSuccess )
		{
			isError = true;
			ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
			if(globalErrCode == kSuccess)
				dialogMessage = kZExpMsgXMLFileCreationFailKey;

			break;
		}

		IDFile pdfFilePath;
		if(gGlobalData->IsExportPDF())
		{
			if (progressBarManager)
				progressBarManager->DoTask(kZExpProgCreatingPDFKey);

			PMString docFileName = fileUtils->GetFileName( path, false );
			const PMString pdf("pdf");
			
			fileUtils->GetTemporaryFolder(pdfFilePath);
			fileUtils->AppendPath(pdfFilePath, docFileName, &pdf);

			PMString pdfStyleName = gGlobalData->GetStyleName();
			errorResult = Utils<IZExpDocUtils>()->CreatePDF( theFrontDoc, pdfFilePath, pdfStyleName, true );
			if( errorResult == kSuccess &&	progressBarManager->WasCancelled())
				errorResult = ErrorUtils::PMGetGlobalErrorCode();

			if(errorResult == kCancel)
				break;

			if(errorResult != kSuccess)
			{
				isError = true;
				ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
				if(globalErrCode == kSuccess)
					dialogMessage = kZExpMsgPDFFileCreationFailKey;

				break;
			}
		}
		else if (progressBarManager)
		{
			progressBarManager->DoTask("");		//Reduce 1 task
		}

#ifdef ExportWithAdvanceOptions
		if( GetTriStateControlData(kZExpExportDlgAdvOptionSkipUpload) == ITriStateControlData::kSelected )
		{
			dialogMessage = kZExpMsgExportCreateSuccessKey;
			break;
		}
#endif

		//TODO: make zip file here and upload it, instead of xml file content.

		uchar * dataBuff = nil;
		int dataLen = 0;
		bool fileRead = fileUtils->ReadFile(path, dataBuff, dataLen);
		if( !fileRead )
		{
			isError = true;
			ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
			if(globalErrCode == kSuccess)
				dialogMessage = kZExpMsgXMLReadFailKey;

			break;
		}

		if(progressBarManager && progressBarManager->WasCancelled())
		{
			errorResult = ErrorUtils::PMGetGlobalErrorCode();
			break;
		}

		if (progressBarManager)
			progressBarManager->DoTask(kZExpProgUploadXMLKey);

		StPtrDeleter<uchar>			autoDelBuff( dataBuff );

		PMString articleID = NewGUID();
		CZExpUploadExportRequest theReq;
		CZExpUploadExportResponse * theResp = (CZExpUploadExportResponse*)((AZExpWSRequest*)(&theReq))->CreateResponseObj();
		StPtrDeleter<CZExpUploadExportResponse> autoDelResp( theResp, true );
		theReq.SetTitleId(titleId);
		theReq.SetEditionId(editionId);
		theReq.SetArticleId(articleID);
		theReq.SetDocName(docName);
		//theReq.SetImageFileName(kNullString);
		theReq.SetContent(dataBuff);
		theReq.SetBuffSize(dataLen);

		CZExpValidateMediaContentRequest theReqValidateMediaContent;
		CZExpValidateMediaContentResponse * theRespValidateMediaContent = (CZExpValidateMediaContentResponse*)((AZExpWSRequest*)(&theReqValidateMediaContent))->CreateResponseObj();
		StPtrDeleter<CZExpValidateMediaContentResponse> autoDelRespValidateMediaContent( theRespValidateMediaContent, true );
		theReqValidateMediaContent.SetTitleId(titleId);
		theReqValidateMediaContent.SetEditionId(editionId);
		theReqValidateMediaContent.SetArticleId(articleID);
		theReqValidateMediaContent.SetDocName(docName);
		//theReqValidateMediaContent.SetImageFileName(kNullString);
		theReqValidateMediaContent.SetContent(dataBuff);
		theReqValidateMediaContent.SetBuffSize(dataLen);

		ZExpWSError wsError = Utils<IZExpWSFacade>()->ValidateMediaContent( &theReqValidateMediaContent, theRespValidateMediaContent);
		bool uploadSuccess = theRespValidateMediaContent->GetIsItemPresent() && theRespValidateMediaContent->GetIsItemAddedToArticle();
		PMString errorDescription( theRespValidateMediaContent->GetErrorDescription() );
		PMString errorDetails( theRespValidateMediaContent->GetErrorDetails() );
		if(wsError == eWSErr_Success && !uploadSuccess)
		{
			wsError = Utils<IZExpWSFacade>()->UploadExport( &theReq, theResp);
			uploadSuccess = theResp->GetUploadSuccess();
			errorDescription.SetCString( theResp->GetErrorDescription() );
			errorDetails.SetCString( theResp->GetErrorDetails() );
		}

		if(progressBarManager && progressBarManager->WasCancelled())
		{
			errorResult = ErrorUtils::PMGetGlobalErrorCode();
			break;
		}
		if(wsError != eWSErr_Success)
		{
			isError = true;
			ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
			if(globalErrCode == kSuccess)
			{
				dialogMessage = kZExpMsgXMLUploadingFailKey;
				dialogMessage.Translate();
				dialogMessage.Append("\n");
				dialogMessage.Append( errorDescription);
				dialogMessage.Append("\n");
				dialogMessage.Append( errorDetails );
			}

			break;
		}

		if (uploadSuccess ==  false)
		{
			dialogMessage = kZExpMsgXMLUploadingFailKey;
			break;
		}
		
		IDFile lowResFile, highResFile;
		Utils<IZExpFileUtils> expFileUtils;
		expFileUtils->GetImageExportFileFolders( path, lowResFile, highResFile );

		uploadSuccess = true;
		if ( gGlobalData->IsExportLowRes() )
		{
			if(progressBarManager)
				progressBarManager->DoTask(kZExpProgUploadLowResImageKey);
		
			uploadSuccess = this->UploadImageFilesInDirectory(lowResFile, titleId, editionId, articleID, docName, false);
			if(uploadSuccess == false)
			{
				isError = true;
				ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
				if(globalErrCode == kSuccess)
					dialogMessage = kZExpMsgLowResImageUploadFailKey;

				break;
			}
		}
		else if( progressBarManager )
			progressBarManager->DoTask("");

		if(progressBarManager && progressBarManager->WasCancelled())
		{
			errorResult = ErrorUtils::PMGetGlobalErrorCode();
			break;
		}
		if (progressBarManager)
			progressBarManager->DoTask(kZExpProgUploadHighResImageKey);

		uploadSuccess = this->UploadImageFilesInDirectory(highResFile, titleId, editionId, articleID, docName, true);
		if(uploadSuccess == false)
		{
			isError = true;
			ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
			if(globalErrCode == kSuccess)
				dialogMessage = kZExpMsgHighResImageUploadFailKey;

			break;
		}

		if(progressBarManager && progressBarManager->WasCancelled())
		{
			errorResult = ErrorUtils::PMGetGlobalErrorCode();
			break;
		}
		if(progressBarManager )
			progressBarManager->SetTaskStatus("");

		if (errorResult == kSuccess && gGlobalData->IsExportPDF())
		{
			if (progressBarManager)
				progressBarManager->DoTask(kZExpProgUploadPDFKey);

			uploadSuccess = this->UploadPDFFile(pdfFilePath, titleId, editionId, articleID, docName);
			if(uploadSuccess == false)
			{
				isError = true;
				ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
				if(globalErrCode == kSuccess)
					dialogMessage = kZExpMsgPDFUploadingFailKey;

				break;
			}
		}

		if (progressBarManager)
			progressBarManager->DoTask(kZExpProgFinishingUploadKey);

		//No cancelling for this WS call
		uploadSuccess = this->UploadEndOfSession(titleId, editionId, articleID, docName, uploadSuccess);
		if(uploadSuccess == false)
		{
			isError = true;
			ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
			if(globalErrCode == kSuccess)
				dialogMessage = kZExpMsgUploadFinishFailKey;

			break;
		}
		dialogMessage = kZExpMsgUploadSuccessKey;
	
	}while(kFalse);

	if(isError && progressBarManager)
		progressBarManager->Abort();

	if (progressBarManager)
		progressBarManager->UnregisterTaskSet(taskBarId);

	if(dialogMessage.empty() == false)
	{
		ErrorCode globalErrCode = ErrorUtils::PMGetGlobalErrorCode();
		if(isError)
		{
			if(globalErrCode == kSuccess)	//Show error message if not shown and set globally
			{
				CAlert::ModalAlert
				(
					dialogMessage,					// Alert string
					kOKString, 						// OK button
					kNullString, 					// No second button
					kNullString, 					// No third button
					1,								// Set OK button to default
					CAlert::eErrorIcon				// Error icon.
				);
			}
		}
		else if ( globalErrCode != kCancel )
		{
			CAlert::ModalAlert
			(
				dialogMessage,					// Alert string
				kOKString, 						// OK button
				kNullString, 					// No second button
				kNullString, 					// No third button
				1,								// Set OK button to default
				CAlert::eInformationIcon		// Information icon.
			);
		}
	}
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZExpExportDialogController::GetWidgetView(
	const WidgetID &			inWidgetID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( inWidgetID );
		ASSERT(widgetView);
		return widgetView;
	}
	return NULL;
}

//----------------------------------------------------------------------------------------
// UploadImageFilesInDirectory
//----------------------------------------------------------------------------------------
bool
CZExpExportDialogController::UploadImageFilesInDirectory(
	const IDFile &				inFileDirectory,
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inArticleID,
	const PMString &			inDocName,
	const bool					inIsHighRes) const
{
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(inFileDirectory))
		return false;

	iter.SetStartingPath(inFileDirectory);

	IDFile tempCountIdFile;
	int fileCount = 0, currFileNum = 0;
#ifndef MACINTOSH
	char * exts[] = {"*.jpg", "*.png", "*.tiff" };
	int totalExts = sizeof( exts )/sizeof(char*);
	for( int currExtIndex = 0; currExtIndex < totalExts ; ++currExtIndex )
	{
		PMString filter(exts[currExtIndex]);
#else
	PMString filter("*.jpg;*.png;*.tiff");
#endif
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	bool16 hasNext1 = iter.FindFirstFile(tempCountIdFile, filter);
	while(hasNext1) {
		PMString tempFileName = tempCountIdFile.GetFileName();
		if(ValidPath(tempFileName))
		{
#ifdef MACINTOSH
			PMString tempExtension;
			FileUtils::GetExtension(tempCountIdFile, tempExtension);
			if (tempExtension != "jpg" && tempExtension != "png" && tempExtension != "tiff")
			{
				hasNext1 = iter.FindNextFile(tempCountIdFile);
				continue;
			}
#endif

			++fileCount;
		}

		hasNext1 = iter.FindNextFile(tempCountIdFile);
	}
#ifndef MACINTOSH
	}
#endif

#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	InterfacePtr<IProgressBarManager> progressBarManager(application, UseDefaultIID());

	iter.SetStartingPath(inFileDirectory);

	const PMString & folderName = inFileDirectory.GetFileName();

	IDFile tempIdFile;
#ifndef MACINTOSH
	for( int currExtIndex = 0; currExtIndex < totalExts ; ++currExtIndex )
	{
		PMString filter(exts[currExtIndex]);
#else
	//PMString filter("*.jpg;*.png;*.tiff");
#endif
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);
	while(hasNext) {
		PMString tempFileName = tempIdFile.GetFileName();
		if(ValidPath(tempFileName))
		{
#ifdef MACINTOSH
			PMString tempExtension;
			FileUtils::GetExtension(tempIdFile, tempExtension);
			if (tempExtension != "jpg" && tempExtension != "png" && tempExtension != "tiff")
			{
				hasNext= iter.FindNextFile(tempIdFile);
				continue;
			}
#endif

			if( progressBarManager )
			{
				if( progressBarManager->WasCancelled() )
					return false;
				char str[100] = {0};
				sprintf( str, "Uploading %d of %d image files.", ++currFileNum, fileCount );
				PMString uploadStatusStr(str);
				uploadStatusStr.SetTranslatable(kFalse);
				progressBarManager->SetTaskStatus( uploadStatusStr );
			}
			tempFileName.InsertW(FileUtils::GetDirectorySeperator(), 0);	//Note: this will be different on Mac & Win.
			tempFileName.Insert(folderName);

			do
			{
				//Upload file
				CZExpUploadExportRequest theReq;
				CZExpUploadExportResponse * theResp = (CZExpUploadExportResponse*)((AZExpWSRequest*)(&theReq))->CreateResponseObj();
				StPtrDeleter<CZExpUploadExportResponse> autoDelResp( theResp, true );
				theReq.SetTitleId(inTitleID);
				theReq.SetEditionId(inEditionID);
				theReq.SetArticleId( inArticleID );
				theReq.SetDocName(inDocName);
				theReq.SetImageFileName(tempFileName);
				PMString currFilePath;
				FileUtils::IDFileToPMString(tempIdFile, currFilePath);
				theReq.SetFilePath(currFilePath);

				if( inIsHighRes )
					theReq.SetOtherParamStr( "&imgres=\"high\"" );
				else
					theReq.SetOtherParamStr( "&imgres=\"low\"" );


				CZExpValidateMediaContentRequest theReqValidateMediaContent;
				CZExpValidateMediaContentResponse * theRespValidateMediaContent = (CZExpValidateMediaContentResponse*)((AZExpWSRequest*)(&theReqValidateMediaContent))->CreateResponseObj();
				StPtrDeleter<CZExpValidateMediaContentResponse> autoDelRespValidateMediaContent( theRespValidateMediaContent, true );
				theReqValidateMediaContent.SetTitleId(inTitleID);
				theReqValidateMediaContent.SetEditionId(inEditionID);
				theReqValidateMediaContent.SetArticleId( inArticleID );
				theReqValidateMediaContent.SetDocName(inDocName);
				theReqValidateMediaContent.SetImageFileName(tempFileName);
				theReqValidateMediaContent.SetFilePath(currFilePath);
				theReqValidateMediaContent.SetOtherParamStr( theReq.GetOtherParamStr() );
				
				ZExpWSError wsError = Utils<IZExpWSFacade>()->ValidateMediaContent( &theReqValidateMediaContent, theRespValidateMediaContent);

				bool uploadSuccess = theRespValidateMediaContent->GetIsItemPresent() && theRespValidateMediaContent->GetIsItemAddedToArticle();
				int errorCode = theRespValidateMediaContent->GetErrorCode();
				PMString errorDescription( theRespValidateMediaContent->GetErrorDescription() );
				PMString errorDetails( theRespValidateMediaContent->GetErrorDetails() );
				if (wsError == eWSErr_Success && !uploadSuccess)
				{
					wsError = Utils<IZExpWSFacade>()->UploadExport( &theReq, theResp);
					uploadSuccess = theResp->GetUploadSuccess();
					errorCode = theResp->GetErrorCode();
					errorDescription.SetCString( theResp->GetErrorDescription() );
					errorDetails.SetCString( theResp->GetErrorDetails() );
				}
				
				if (wsError != eWSErr_Success || !uploadSuccess)
				{
					if( errorCode != kSuccess )
					{
						ErrorCode err = kIZExpWSFailError;
						PMString errStr("Server Error: ");
						errStr.AppendNumber( errorCode );
						errStr.Append("\n");
						errStr.Append( errorDescription );
						errStr.Append("\n");
						errStr.Append( errorDetails );
						errStr.SetTranslatable( kFalse );

						PMString errMsg (kZExpMsgRetryImageUploadStringKey);
						errMsg.Translate();
						errMsg.Append( errStr );
						errMsg.SetTranslatable( kFalse );
						int clickedButton = CAlert::ModalAlert(
														errMsg,							// Alert string
														kYesString,//kIZExpRetryBtnStringKey,		// Retry button
														kNoString, //kIZExpAbortBtnStringKey,		// Abort button
														kNullString, 					// No third button
														1,								// Set Yes button to default
														CAlert::eQuestionIcon			// Information icon.
													);

						if(clickedButton == 1)
						{
							ErrorUtils::PMSetGlobalErrorCode(kSuccess);
							continue;
						}
						else
						{
							//ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
							ErrorUtils::PMSetGlobalErrorCode( kCancel );
							return false;
						}
					}
				}
				else
				{
					break;	//Continue with next file.
				}
			} while( kTrue );
		}

		hasNext= iter.FindNextFile(tempIdFile);
	}
#ifndef MACINTOSH
	}
#endif
	return true;
}

//----------------------------------------------------------------------------------------
// UploadPDFFile
//----------------------------------------------------------------------------------------
bool
CZExpExportDialogController::UploadPDFFile(
	const IDFile &				inPDFFile,
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inArticleID,
	const PMString &			inDocName) const
{
	const PMString pdfFileName = inPDFFile.GetFileName();
	CZExpUploadExportRequest theRequest;
	CZExpUploadExportResponse * theResponse = (CZExpUploadExportResponse*)((AZExpWSRequest*)(&theRequest))->CreateResponseObj();
	StPtrDeleter<CZExpUploadExportResponse> autoDelResp( theResponse, true );
	theRequest.SetTitleId(inTitleID);
	theRequest.SetEditionId(inEditionID);
	theRequest.SetArticleId( inArticleID );
	theRequest.SetDocName(inDocName);
	theRequest.SetImageFileName(pdfFileName);
	PMString currFilePath;
	FileUtils::IDFileToPMString(inPDFFile, currFilePath);
	theRequest.SetFilePath(currFilePath);
	
	CZExpValidateMediaContentRequest theRequestValidateMediaContent;
	CZExpValidateMediaContentResponse * theRespValidateMediaContent = (CZExpValidateMediaContentResponse*)((AZExpWSRequest*)(&theRequestValidateMediaContent))->CreateResponseObj();
	StPtrDeleter<CZExpValidateMediaContentResponse> autoDelRespValidateMediaContent( theRespValidateMediaContent, true );
	theRequestValidateMediaContent.SetTitleId(inTitleID);
	theRequestValidateMediaContent.SetEditionId(inEditionID);
	theRequestValidateMediaContent.SetArticleId( inArticleID );
	theRequestValidateMediaContent.SetDocName(inDocName);
	theRequestValidateMediaContent.SetImageFileName(pdfFileName);
	theRequestValidateMediaContent.SetFilePath(currFilePath);
	
	ZExpWSError wsError = Utils<IZExpWSFacade>()->ValidateMediaContent( &theRequestValidateMediaContent, theRespValidateMediaContent);

	bool uploadSuccess = theRespValidateMediaContent->GetIsItemPresent() && theRespValidateMediaContent->GetIsItemAddedToArticle();
	int errorCode = theRespValidateMediaContent->GetErrorCode();
	PMString errorDescription( theRespValidateMediaContent->GetErrorDescription() );
	PMString errorDetails( theRespValidateMediaContent->GetErrorDetails() );

	if (wsError == eWSErr_Success && !uploadSuccess)
	{
		wsError = Utils<IZExpWSFacade>()->UploadExport( &theRequest, theResponse);

		uploadSuccess = theResponse->GetUploadSuccess();
		errorCode = theResponse->GetErrorCode();
		errorDescription.SetCString( theResponse->GetErrorDescription() );
		errorDetails.SetCString( theResponse->GetErrorDetails() );
	}
	
	if (wsError != eWSErr_Success || !uploadSuccess)
	{
		if( errorCode != kSuccess )
		{
			ErrorCode err = kIZExpWSFailError;
			PMString errStr("Server Error: ");
			errStr.AppendNumber( errorCode );
			errStr.Append("\n");
			errStr.Append( errorDescription );
			errStr.Append("\n");
			errStr.Append( errorDetails );
			errStr.SetTranslatable( kFalse );
			ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
		}
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// UploadEndOfSession
//----------------------------------------------------------------------------------------
bool
CZExpExportDialogController::UploadEndOfSession(
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inArticleID,
	const PMString &			inDocName,
	bool						inWithSuccess) const
{
	CZExpUploadExportRequest theRequest;
	CZExpUploadExportResponse * theResponse = (CZExpUploadExportResponse*)((AZExpWSRequest*)(&theRequest))->CreateResponseObj();
	StPtrDeleter<CZExpUploadExportResponse> autoDelResp( theResponse, true );
	theRequest.SetTitleId(inTitleID);
	theRequest.SetEditionId(inEditionID);
	theRequest.SetArticleId( inArticleID );
	theRequest.SetDocName(inDocName);
	const char * data = "END";
	if( inWithSuccess == false )
		data = "ENDFAIL";
	theRequest.SetContent( (const uchar *)data );
	theRequest.SetBuffSize( strlen( data ) );
	ZExpWSError wsError = Utils<IZExpWSFacade>()->UploadExport( &theRequest, theResponse);
	if (wsError != eWSErr_Success || !theResponse->GetUploadSuccess())
	{
		if( theResponse->GetErrorCode() != kSuccess )
		{
			ErrorCode err = kIZExpWSFailError;
			PMString errStr("Server Error: ");
			errStr.AppendNumber( theResponse->GetErrorCode() );
			errStr.Append("\n");
			errStr.Append( theResponse->GetErrorDescription() );
			errStr.Append("\n");
			errStr.Append( theResponse->GetErrorDetails() );
			errStr.SetTranslatable( kFalse );
			ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
		}
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// ValidPath
//----------------------------------------------------------------------------------------
bool16
CZExpExportDialogController::ValidPath(
	const PMString&				path) const
{
	const PMString thisDir(".");
	const PMString parentDir("..");
	return path != thisDir && path != parentDir;
}
