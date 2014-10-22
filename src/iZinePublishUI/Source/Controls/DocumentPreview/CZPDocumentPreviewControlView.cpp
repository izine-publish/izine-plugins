//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/DocumentPreview/CZPDocumentPreviewControlView.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IGraphicsPort.h"
#include "IImageStream.h"
#include "IImportPreview.h"
#include "IImportProvider.h"
#include "IK2ServiceProvider.h"
#include "IK2ServiceRegistry.h"
#include "IPMStream.h"
#include "ISysFileData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "AcquireModalCursor.h"
#include "AGMGraphicsContext.h"
#include "AutoGSave.h"
#include "CControlView.h"
#include "FileUtils.h"
#include "GraphicsExternal.h"	// _t_AGM...
#include "PNGArt.h"
#include "StreamUtil.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Color space family from AGM, subsetted to meet our needs here.
 @ingroup customdatalinkui
 */
enum SubsettedAGMColorSpaceFamily
{
	/** */
	kDontCare,
	/** */
	kAGMCsRGB
	/** */
};

#define inheritClass CControlView
class ZPDocumentPreviewControlView : public inheritClass
{
public:
						ZPDocumentPreviewControlView(IPMUnknown * boss);
	virtual				~ZPDocumentPreviewControlView();

	virtual void		Init(
							const WidgetID&				widgetId,
							const PMRect&				frame,
							RsrcID						rsrcID);
	virtual void		Draw(
							IViewPort*					viewPort,
							SysRgn						updateRgn);
	
protected:
private:
	void				deleteBuffers();
	ErrorCode			createPreview(
							const IDFile&				previewFile,
							uint32						nWidthWidget,
							uint32						nHeightWidget,
							uint8						backGrey);
	
private:
	AGMImageRecord *	fpCurAGMImage;
	int32				fCachedImWidth ;
	int32				fCachedImHeight ;
	uint8 *				fDataBuffer;
	IDFile				fCurImageSysFile;
private:
};


CREATE_PERSIST_PMINTERFACE(ZPDocumentPreviewControlView, kZPDocumentPreviewControlViewImpl)

ZPDocumentPreviewControlView::ZPDocumentPreviewControlView(IPMUnknown * boss)
: inheritClass(boss)
, fpCurAGMImage(nil)
, fCachedImWidth(0)
, fCachedImHeight(0)
, fDataBuffer(nil)
{
	InterfacePtr<ISysFileData> iSysFileData(this, IID_ISYSFILEDATA);
	ASSERT(iSysFileData);

	iSysFileData->Set(IDFile());
}


//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPDocumentPreviewControlView::~ZPDocumentPreviewControlView()
{
	deleteBuffers();
}

#pragma mark -

//----------------------------------------------------------------------------------------
// deleteBuffers
//----------------------------------------------------------------------------------------
void
ZPDocumentPreviewControlView::deleteBuffers()
{
	if(fpCurAGMImage) {
		delete [] fDataBuffer;
		delete fpCurAGMImage;
		fpCurAGMImage = nil;
	}
}

//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
void
ZPDocumentPreviewControlView::Init(
	const WidgetID& widgetId, const PMRect& frame, RsrcID rsrcID)
{
	LogFunctionEnterExit;
	
	CControlView::Init(widgetId, frame, rsrcID);
}

//----------------------------------------------------------------------------------------
// Draw
//----------------------------------------------------------------------------------------
void
ZPDocumentPreviewControlView::Draw(
	IViewPort* viewPort, SysRgn updateRgn)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	AGMGraphicsContext gc(viewPort, this, updateRgn);

	InterfacePtr<IGraphicsPort>	gPort(gc.GetViewPort(), UseDefaultIID());
	ASSERT(gPort);
	if(!gPort) {
		return;
	}
	
	AutoGSave autoGSave(gPort);
	
	do {
		ErrorCode resultOfCreatingPreview = kFailure;
		
		InterfacePtr<const IInterfaceColors>
		iInterfaceColors(gSession, IID_IINTERFACECOLORS);
		ASSERT(iInterfaceColors);
		if(!iInterfaceColors){
			break;
		}
		
		RealAGMColor whiteFill;
		iInterfaceColors->GetRealAGMColor(kInterfaceWhite, whiteFill);
		
		RealAGMColor defaultGreyFill;
		iInterfaceColors->GetRealAGMColor(kInterfacePaletteFill, defaultGreyFill);
		
		PMRect frame = GetFrame();
		frame.MoveTo(0, 0);				// get into local coordinates
		
		uint32 nWidthWidget = 1;
		uint32 nHeightWidget = 1;
				
		// Dropped the 3d border effect
		nWidthWidget = ToInt32(frame.Width());
		nHeightWidget = ToInt32(frame.Height());
		
		// get filespec currently associated with widget
		InterfacePtr<ISysFileData> iImageSysFile(this, IID_ISYSFILEDATA);
		ASSERT(iImageSysFile);
		if(!iImageSysFile)
		{
			PNGArt png;
			png.SetRsrc(kIZPUIPluginID, kIZPLogoIconRsrcID, kFalse);
			
			PMReal imageWidth = png.GetWidth();
			PMReal imageHeight = png.GetHeight();
			
			PMReal xOffset = frame.GetHCenter() - imageWidth/2;
			PMReal yOffset = frame.GetVCenter() - imageHeight/2;
			
			// Centered
			gPort->translate(xOffset, yOffset);
			
			PMMatrix theMatrix;	// No transform
			png.Draw(gPort, theMatrix,0);
			break;
		}
		
		IDFile previewFile = iImageSysFile->GetSysFile();
		if(FileUtils::DoesFileExist(previewFile) == kFalse)
		{
			PNGArt png;
			png.SetRsrc(kIZPUIPluginID, kIZPLogoIconRsrcID, kFalse);

			PMReal imageWidth = png.GetWidth();
			PMReal imageHeight = png.GetHeight();

			PMReal xOffset = frame.GetHCenter() - imageWidth/2;
			PMReal yOffset = frame.GetVCenter() - imageHeight/2;
			
			gPort->setrgbcolor(0,0,0);
			gPort->rectpath(frame);

			// Centered
			gPort->translate(xOffset, yOffset);
			
			PMMatrix theMatrix;	// No transform
			png.Draw(gPort, theMatrix,0);
			break;
		}
		
		if(fCurImageSysFile != previewFile) {
			// Take average greyvalue of what we're filling background to
			uint8 backgroundGrey = ToInt32(Round(
												 255.0* (defaultGreyFill.red +  defaultGreyFill.green + defaultGreyFill.blue)/3.0));
			// Create another preview, if we need to
			resultOfCreatingPreview = this->createPreview(previewFile, nWidthWidget, nHeightWidget, backgroundGrey);
			
		} else {
			// That's OK- assume safe to use the old one
			resultOfCreatingPreview = kSuccess;
		}
		
		
		// if we got an image, then display it
		// otherwise, draw a red diagonal line through the widget
		if (resultOfCreatingPreview == kSuccess) {
			// Fill the background with white
//			gPort->setrgbcolor(whiteFill.red, whiteFill.green, whiteFill.blue);
//			gPort->rectpath(frame);
			// gPort->fill();
			
			PMReal imageWidth = (fpCurAGMImage->bounds.xMax - fpCurAGMImage->bounds.xMin);
			PMReal imageHeight = (fpCurAGMImage->bounds.yMax - fpCurAGMImage->bounds.yMin);
			PMReal xOffset = frame.GetHCenter() - imageWidth/2;
			PMReal yOffset = frame.GetVCenter() - imageHeight/2;
			
			gPort->setrgbcolor(0,0,0);
			gPort->rectpath(frame);

			// Centered
			gPort->translate(xOffset, yOffset);
			
			PMMatrix theMatrix;	// No transform
			ASSERT(fpCurAGMImage);
			gPort->image(fpCurAGMImage, theMatrix, 0);
		}
		else {
			// Fill with the palette default background fill
			gPort->setrgbcolor(defaultGreyFill.red, defaultGreyFill.green, defaultGreyFill.blue);
			gPort->rectpath(frame);
			gPort->fill();
		}
	} while(kFalse);
}
//----------------------------------------------------------------------------------------
// createPreview
//----------------------------------------------------------------------------------------
ErrorCode
ZPDocumentPreviewControlView::createPreview(
	const IDFile& previewFile,
	uint32 width,
	uint32 height,
	uint8 backGrey )
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		// get source stream (image file to preview)
		InterfacePtr<IPMStream> fileStream(StreamUtil::CreateFileStreamReadLazy(previewFile));

		if(fileStream == nil) {
			break;
		}
		
		InterfacePtr<IK2ServiceRegistry>	serviceRegistry(gSession, UseDefaultIID());
		ASSERT(serviceRegistry);
		if(!serviceRegistry) {
			break;
		}
		
		int	numHandlers = serviceRegistry->GetServiceProviderCount(kImportProviderService);
		for (int i = 0; i < numHandlers; i++) {
			
			InterfacePtr<IK2ServiceProvider> provider(serviceRegistry->QueryNthServiceProvider(kImportProviderService, i));
			
			InterfacePtr<IImportProvider> importProvider(provider, IID_IIMPORTPROVIDER);
			if (importProvider && importProvider->CanImportThisStream(fileStream) == IImportProvider::kFullImport) {
				
				InterfacePtr<IImportPreview> preview(importProvider, IID_IIMPORTPREVIEW);
				if(preview) {
					bool16 reallocateNeeded = kTrue;
					if( (this->fCachedImHeight == height) && (this->fCachedImWidth == width)) {
						reallocateNeeded = kFalse;
					}
					
					if(reallocateNeeded) {
						
						// Trash any existing storage
						this->deleteBuffers();
						
						fpCurAGMImage = new AGMImageRecord;
						memset (fpCurAGMImage, 0, sizeof(AGMImageRecord));
						
						//create the preview
						
						fpCurAGMImage->bounds.xMin 			= 0;
						fpCurAGMImage->bounds.yMin 			= 0;
						fpCurAGMImage->bounds.xMax 			= width;
						fpCurAGMImage->bounds.yMax 			= height;
						fpCurAGMImage->byteWidth 			= 3*width;
						// For optimal performance, the pixel dimensions should be multiples of
						// 4, then you can use:
						//fpCurAGMImage->byteWidth = BYTES2ROWBYTES(3*width);
						fpCurAGMImage->colorSpace 			= kAGMCsRGB;
						fpCurAGMImage->bitsPerPixel 		= 24;
						fpCurAGMImage->decodeArray 			= 0;
						fpCurAGMImage->colorTab.numColors 	= 0;
						fpCurAGMImage->colorTab.theColors 	= nil;
						
						
						this->fDataBuffer = new uint8[((fpCurAGMImage->byteWidth) * height)];
						ASSERT(this->fDataBuffer);
						fpCurAGMImage->baseAddr = static_cast<void *>(this->fDataBuffer);
						
					}
					ASSERT(fpCurAGMImage);
					//set the background to be grey
					::memset(fpCurAGMImage->baseAddr, backGrey,(fpCurAGMImage->byteWidth) * height);
					
					if (fpCurAGMImage->baseAddr) {
						AcquireWaitCursor busyCursor;
						
						preview->Create24bitRGBPreview( (uint8*)fpCurAGMImage->baseAddr,
														width, height, fileStream, kTrue );
						fCurImageSysFile = previewFile;
						return kSuccess;
					}
				}
			}
			// Be sure to reset the stream
			fileStream->Seek(0,kSeekFromStart);
		} // i loop ove handlers
		
		
		
	} while(false);
	
	return kFailure;
}

