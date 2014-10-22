//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpDocUtils.cpp $
//	$Revision: 3770 $
//	$Date: 2012-01-19 10:39:59 +0100 (Thu, 19 Jan 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IDocument.h"
#include "IOutputPages.h"
#include "IPageList.h"
#include "IPDFExportPrefs.h"
#include "IPMStream.h"
#include "IShape.h"
#include "ISysFileData.h"
#include "IWorkspace.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "SnapshotUtilsEx.h"
#include "LayerID.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpDocUtils.h"
#include "IZExpInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	//IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpDocUtils>
class CZExpDocUtils : public inheritClass
{
public:
						CZExpDocUtils(IPMUnknown * boss);
	virtual				~CZExpDocUtils();

	ErrorCode			CreatePageItemSnapshot(
							const UIDRef &				inPageItemRef,
							IPMStream *					oStream,
							bool						inLowRes,
							int32						inDrawingFlags,
							int32						inDesiredRes = 72 ) const;
	
	ErrorCode			CreatePageItemGIFSnapshot(
							const UIDRef &				inPageItemRef,
							IPMStream *					oStream,
							bool						inLowRes,
							int32						inDrawingFlags,
							int32						inDesiredRes = 72 ) const;
	
	ErrorCode			CreatePageItemTIFFSnapshot(
							const UIDRef &				inPageItemRef,
							IPMStream *					oStream,
							bool						inLowRes,
							int32						inDrawingFlags,
							int32						inDesiredRes = 72 ) const;

#ifdef InDnCS5
	ErrorCode			CreatePageItemPNGSnapshot(
							const UIDRef &				inPageItemRef,
							IPMStream *					oStream,
							bool						inLowRes,
							int32						inDrawingFlags,
							int32						inDesiredRes = 72 ) const;
#endif

	ErrorCode			CreatePDF(
							const IDocument *			inDoc,
							const IDFile &				inPdfFile,
							const PMString &			inPdfStyleName,
							bool						inUseDefaultStyleIfNotFound) const;

	ErrorCode			CreatePDF(
							const IDocument *			inDoc,
							const IDFile &				inPdfFile,
							const PMString &			inPdfStyleName,
							const UIDList &				inPageUIDList,
							bool						inUseDefaultStyleIfNotFound) const;

	ErrorCode			ShowHideDocLayers (
							const IDocument *			inDoc,
							const UIDList &				inDocLayerIDs,
							bool						inShow ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpDocUtils, kZExpDocUtilsImpl)

CZExpDocUtils::CZExpDocUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpDocUtils::~CZExpDocUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreatePageItemSnapshot
// Page item UID can be of page, text frame, graphic frame
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePageItemSnapshot(
	const UIDRef &				inPageItemRef,
	IPMStream *					oStream,
	bool						inLowRes,
	int32						inDrawingFlags,
	int32						inDesiredRes ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		if( !oStream )
			break;

		int32 flags =  inDrawingFlags
			//| IShape::kDrawingClippedOutImageData 
			//| IShape::kDrawFrameEdge 
			//| IShape::kPrinting 
			;
		if( inLowRes )
			flags |= IShape::kPreviewMode;

		VPAttrMap vpAttrs;

		SnapshotUtilsEx fSnapshotUtils( inPageItemRef, 1.0, 1.0, inDesiredRes, 72, 0, SnapshotUtilsEx::kCsRGB );
		
		bool32 fullResolutionGraphics = kTrue;
		if( inLowRes )
			fullResolutionGraphics = kFalse;

		if (fSnapshotUtils.Draw(flags, fullResolutionGraphics, PMReal(7.0) , kTrue, SnapshotUtils::kXPMaximum, nil, nil, kFalse) != kSuccess)
			break;

#ifdef InDnCS5
#define EmbedColorProfileParam	, true
#else
#define EmbedColorProfileParam
#endif
		if( inLowRes )
			toReturn = fSnapshotUtils.ExportImageToJPEG(oStream, SnapshotUtils::kSSJPEGBaseline, SnapshotUtils::kSSJPEGGoodQuality EmbedColorProfileParam);
		else
			toReturn = fSnapshotUtils.ExportImageToJPEG(oStream, SnapshotUtils::kSSJPEGProgressive, SnapshotUtils::kSSJPEGGreatQuality EmbedColorProfileParam);
		
		oStream->Close();
	}while(false);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreatePageItemGIFSnapshot
// Page item UID can be of page, text frame, graphic frame
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePageItemGIFSnapshot(
	const UIDRef &				inPageItemRef,
	IPMStream *					oStream,
	bool						inLowRes,
	int32						inDrawingFlags,
	int32						inDesiredRes ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		if( !oStream )
			break;

		int32 flags =  inDrawingFlags
			//| IShape::kDrawingClippedOutImageData 
			//| IShape::kDrawFrameEdge 
			//| IShape::kPrinting 
			;
		if( inLowRes )
			flags |= IShape::kPreviewMode;

		VPAttrMap vpAttrs;

		SnapshotUtilsEx fSnapshotUtils( inPageItemRef, 1.0, 1.0, inDesiredRes, 72, 0, SnapshotUtilsEx::kCsRGB, kTrue );
		
		bool32 fullResolutionGraphics = kTrue;
		if( inLowRes )
			fullResolutionGraphics = kFalse;

		if (fSnapshotUtils.Draw(flags, fullResolutionGraphics, PMReal(7.0) , kTrue, SnapshotUtils::kXPMaximum, nil, nil, kFalse) != kSuccess)
			break;
		
		toReturn = fSnapshotUtils.ExportImageToGIF(oStream, SnapshotUtils::kTryExactThenAdaptivePalette, kTrue);
		
		oStream->Close();
	}while(false);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreatePageItemTIFFSnapshot
// Page item UID can be of page, text frame, graphic frame
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePageItemTIFFSnapshot(
	const UIDRef &				inPageItemRef,
	IPMStream *					oStream,
	bool						inLowRes,
	int32						inDrawingFlags,
	int32						inDesiredRes ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		if( !oStream )
			break;

		int32 flags =  inDrawingFlags
			//| IShape::kDrawingClippedOutImageData 
			//| IShape::kDrawFrameEdge 
			//| IShape::kPrinting 
			;
		if( inLowRes )
			flags |= IShape::kPreviewMode;

		VPAttrMap vpAttrs;

		SnapshotUtilsEx fSnapshotUtils( inPageItemRef, 1.0, 1.0, inDesiredRes, 72, 0, SnapshotUtilsEx::kCsRGB, kTrue );
		
		bool32 fullResolutionGraphics = kTrue;
		if( inLowRes )
			fullResolutionGraphics = kFalse;

		if (fSnapshotUtils.Draw(flags, fullResolutionGraphics, PMReal(7.0) , kTrue, SnapshotUtils::kXPMaximum, nil, nil, kFalse) != kSuccess)
			break;

#ifdef InDnCS5
#define EmbedColorProfileParam	, true
#else
#define EmbedColorProfileParam
#endif

		toReturn = fSnapshotUtils.ExportImageToTIFF( oStream, SnapshotUtils::kNoPalette EmbedColorProfileParam);
		
		oStream->Close();
	}while(false);

	return toReturn;
}

#ifdef InDnCS5
//----------------------------------------------------------------------------------------
// CreatePageItemPNGSnapshot
// Page item UID can be of page, text frame, graphic frame
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePageItemPNGSnapshot(
	const UIDRef &				inPageItemRef,
	IPMStream *					oStream,
	bool						inLowRes,
	int32						inDrawingFlags,
	int32						inDesiredRes ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		if( !oStream )
			break;

		int32 flags =  inDrawingFlags
			//| IShape::kDrawingClippedOutImageData 
			//| IShape::kDrawFrameEdge 
			//| IShape::kPrinting 
			;
		if( inLowRes )
			flags |= IShape::kPreviewMode;

		VPAttrMap vpAttrs;

		SnapshotUtilsEx fSnapshotUtils( inPageItemRef, 1.0, 1.0, inDesiredRes, 72, 0, SnapshotUtilsEx::kCsRGB, kTrue );
		
		bool32 fullResolutionGraphics = kTrue;
		if( inLowRes )
			fullResolutionGraphics = kFalse;

		if (fSnapshotUtils.Draw(flags, fullResolutionGraphics, PMReal(7.0) , kTrue, SnapshotUtils::kXPMaximum, nil, nil, kFalse) != kSuccess)
			break;
		
		toReturn = fSnapshotUtils.ExportImageToPNG( oStream );
		
		oStream->Close();
	}while(false);

	return toReturn;
}
#endif

//----------------------------------------------------------------------------------------
// CreatePDF
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePDF(
	const IDocument *			inDoc,
	const IDFile &				inPdfFile,
	const PMString &			inPdfStyleName,
	bool						inUseDefaultStyleIfNotFound) const
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

		toReturn = this->CreatePDF( inDoc, inPdfFile, inPdfStyleName, thePageUIDList, inUseDefaultStyleIfNotFound);
	}while(false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreatePDF
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::CreatePDF(
	const IDocument *			inDoc,
	const IDFile &				inPdfFile,
	const PMString &			inPdfStyleName,
	const UIDList &				inPageUIDList,
	bool						inUseDefaultStyleIfNotFound) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		// create pdf
#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
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
		
		thePDFExportPrefs->CopyPrefs(thePDFExportWorkspacePrefs);
		
		// check if profile exists, probably not necessary, use the last style used for creating PDF.
		int pdfStyleIndex = Utils<IZExpInDnUtils>()->GetPDFStyleIndex( inPdfStyleName );	//Should we use inDoc here?
		if( pdfStyleIndex == -1 )
		{
			if( !inUseDefaultStyleIfNotFound )
				break;
			
			pdfStyleIndex = Utils<IZExpInDnUtils>()->GetCurrentPDFExportStyleIndex();
			if( pdfStyleIndex == -1 )
				pdfStyleIndex = 0;	//Use the first one.
		}

		PMString pdfStyleName = Utils<IZExpInDnUtils>()->GetPDFExportStyleName( pdfStyleIndex );
		//if ( pdfStleIndex != -1 )
		thePDFExportPrefs->SetPDFXProfileSelectorAndName(IPDFExportPrefs::kExportPDFDestProfileUseName, pdfStyleName);
		
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

#pragma mark -
//----------------------------------------------------------------------------------------
// ShowHideDocLayers
//----------------------------------------------------------------------------------------
ErrorCode
CZExpDocUtils::ShowHideDocLayers (
	const IDocument *			inDoc,
	const UIDList &				inDocLayerIDs,
	bool						inShow) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		InterfacePtr<ICommand> showLayerCmd(CmdUtils::CreateCommand(kShowLayerCmdBoss));
		if(!showLayerCmd)
			return toReturn;

		showLayerCmd->SetItemList( inDocLayerIDs );

		InterfacePtr<IBoolData> cmdBoolData( showLayerCmd, UseDefaultIID() );
		ASSERT(cmdBoolData);

		if( cmdBoolData )
			cmdBoolData->Set( inShow );

		toReturn = CmdUtils::ProcessCommand( showLayerCmd );

	}while(false);
	return toReturn;
}
