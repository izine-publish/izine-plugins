//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLExport_Snapshots.cpp $
//	$Revision: 3907 $
//	$Date: 2012-02-13 07:50:38 +0100 (Mon, 13 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-9-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDocument.h"
#include "IDocumentLayer.h"
#include "IHierarchy.h"
#include "ILayerList.h"
#include "IPageList.h"
#include "IPMStream.h"
#include "IShape.h"
#include "ISpreadLayer.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif


//SDK General includes
#include "FileUtils.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpDocUtils.h"
#include "IZExpFileUtils.h"
#include "IZExpGlobalData.h"

//IZP General includes
#include "StInDnXmlUtils.h"
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Snapshots.h"
#include "ZExpXMLConstants.h"
#include "ZExpStyleAttrConstants.h"
#include "StInDesignUtils.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
#define CZExpXMLExport_Snapshots CZExpXMLExportUtils

#pragma mark -
namespace CZExpXMLExport_Snapshots
{
	ErrorCode			ExportPageLayersSnapshot(
							const IDFile &				inXMLFilePath,
							const UIDRef &				inPageRef,
							const int					inDocLayerIndex,
							const int					inDocLayerCount);

	ErrorCode			ExportPageLayerSnapshot(
							const IDFile &				inXMLFilePath,
							const UIDRef &				inPageRef,
							const UID &					inSpreadLayerID );
}

//----------------------------------------------------------------------------------------
// ExportPageLayersSnapshot
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLExport_Snapshots::ExportPageLayersSnapshot(
	const IDFile &				inXMLFilePath,
	const IDocument *			inDoc)
{
	LogFunctionEnterExit;
	ASSERT( inDoc );
	
	InterfacePtr<const ILayerList> docLayerList( inDoc, UseDefaultIID() );
	ASSERT( docLayerList );

	ErrorCode toReturn = kSuccess;

	UIDList visibleDocLayers( ::GetDataBase(inDoc) );
	int docLayerCount = docLayerList->GetCount();
	//Store visible doc layer IDs
	for( int i = 0; i < docLayerCount ; ++i )
	{
		InterfacePtr<IDocumentLayer> currDocLayer( docLayerList->QueryLayer(i) );
		ASSERT( currDocLayer );
		
		if( currDocLayer->IsUILayer() && currDocLayer->IsVisible() )
			visibleDocLayers.Append(::GetUID( currDocLayer ) );
	}

	Utils<IZExpDocUtils> zexpDocUtils;

	//Hide all doc layers.
	for( int idIndex = 0; idIndex < visibleDocLayers.Length() ; ++idIndex )
	{
		UIDList currDocLayerList( visibleDocLayers.GetRef(idIndex) );
		zexpDocUtils->ShowHideDocLayers( inDoc, currDocLayerList, false );
	}

	InterfacePtr<const IPageList> docPageList( inDoc, UseDefaultIID() );
	ASSERT( docPageList );
	int pageCount = docPageList->GetPageCount();

	for( int i = 0; i < docLayerCount ; ++i )
	{
		InterfacePtr<IDocumentLayer> currDocLayer( docLayerList->QueryLayer(i) );
		ASSERT( currDocLayer );
	
		if( currDocLayer->IsUILayer() == kFalse )
			continue;

		UIDList currDocLayerList( ::GetUIDRef(currDocLayer) );

		//Show current doc layer
		zexpDocUtils->ShowHideDocLayers( inDoc, currDocLayerList, true );

		UIDRef currPageRef( GetDataBase(inDoc), kInvalidUID );
		
		//Take snapshot of all pages
		for( int j = 0; j < pageCount ; ++j )
		{
			currPageRef.ResetUID( docPageList->GetNthPageUID( j ) );

			toReturn = CZExpXMLExport_Snapshots::ExportPageLayersSnapshot( inXMLFilePath, currPageRef, i, docLayerCount);
			if( toReturn != kSuccess )
				break;
		}

		//Hide current doc layer
		zexpDocUtils->ShowHideDocLayers( inDoc, currDocLayerList, false );

		if( toReturn != kSuccess )
			break;
	}

	//Restore visible doc layers
	for( int idIndex = 0; idIndex < visibleDocLayers.Length() ; ++idIndex )
	{
		UIDList currDocLayerList( visibleDocLayers.GetRef(idIndex) );
		zexpDocUtils->ShowHideDocLayers( inDoc, currDocLayerList, true );
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ExportPageLayersSnapshot
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLExport_Snapshots::ExportPageLayersSnapshot(
	const IDFile &				inXMLFilePath,
	const UIDRef &				inPageRef,
	const int					inDocLayerIndex,
	const int					inDocLayerCount)
{
	ErrorCode toReturn = kSuccess;

	InterfacePtr<IHierarchy> pageHier( inPageRef, UseDefaultIID() );
	ASSERT( pageHier );

	InterfacePtr<IHierarchy> spreadHierarchy( pageHier->QueryRoot() );
	ASSERT( spreadHierarchy );

	int32 spreadLayerCount = spreadHierarchy->GetChildCount();
	UIDRef spreadLayerUIDRef( inPageRef.GetDataBase(), kInvalidUID );
	
//From SDK help
//Variable definitions:
//i = index of document layer (kDocumentLayerBoss) in ILayerList whose associated spread layer (kSpreadlayerBoss) is wanted.
//c = number of document layers in ILayerList.
//
//If guides are displayed in front of content, the following calculations are used:
//Content-spread layer IHierarchy child index = i + 1.
//Guide-spread layer IHierarchy child index = i + c.
//
//If guides are displayed behind content, the following calculations are used:
//Content-spread layer IHierarchy child index = i +c.
//Guide-spread layer IHierarchy child index = i + 1

	int spreadLayerIndex = inDocLayerIndex + 1;
	
	spreadLayerUIDRef.ResetUID( spreadHierarchy->GetChildUID( spreadLayerIndex ) );
	InterfacePtr<ISpreadLayer> spreadLayer( spreadLayerUIDRef, UseDefaultIID() );
	ASSERT(spreadLayer);
	if( spreadLayer->GetIsGuideLayer() == kTrue )
	{
		spreadLayerIndex = inDocLayerIndex + inDocLayerCount;
		spreadLayerUIDRef.ResetUID( spreadHierarchy->GetChildUID( spreadLayerIndex ) );
		InterfacePtr<ISpreadLayer> spreadLayer2( spreadLayerUIDRef, UseDefaultIID() );
		ASSERT(spreadLayer2);
		ASSERT( spreadLayer2->GetIsGuideLayer() == kFalse );
	}
	toReturn = CZExpXMLExport_Snapshots::ExportPageLayerSnapshot( inXMLFilePath, inPageRef, spreadLayerUIDRef.GetUID() );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ExportPageLayerSnapshot
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLExport_Snapshots::ExportPageLayerSnapshot(
	const IDFile &				inXMLFilePath,
	const UIDRef &				inPageRef,
	const UID &					inSpreadLayerID)
{
	ErrorCode toReturn = kSuccess;
#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif

	InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );

	if( gGlobalData->IsExportPagePreview() == false )
		return toReturn;

	//Create streams to export data in.
	IDFile lowResFile, highResFile;
	Utils<IZExpFileUtils> expFileUtils;
	expFileUtils->GetImageExportFileFolders( inXMLFilePath, lowResFile, highResFile );

	PMString imageFileName;
	imageFileName.AppendNumber( inPageRef.GetUID().Get() );
	imageFileName.Append( "_" );
	imageFileName.AppendNumber( inSpreadLayerID.Get() );
#ifdef InDnCS5
	imageFileName.Append( ".png" );
#else
	imageFileName.Append( ".jpg" );
#endif
	FileUtils::AppendPath( &lowResFile, imageFileName );
	FileUtils::AppendPath( &highResFile, imageFileName );

	InterfacePtr<IPMStream> lowResStream;//( expFileUtils->CreateWriteStreamForImage( lowResFile ) );
	InterfacePtr<IPMStream> highResStream( expFileUtils->CreateWriteStreamForImage( highResFile ) );

	if( gGlobalData->IsExportLowRes() )
		lowResStream.reset( expFileUtils->CreateWriteStreamForImage( lowResFile ) );

	StStreamCloser autoCloseLowRes( lowResStream );
	StStreamCloser autoCloseHighRes( highResStream );

	Utils<IZExpDocUtils> docUtils;
	//docUtils->CreatePageItemSnapshot( inPageRef, lowResStream, true, IShape::kPrinting );
#ifdef InDnCS5
	if( gGlobalData->IsExportLowRes() )
		toReturn = docUtils->CreatePageItemPNGSnapshot( inPageRef, lowResStream, false, IShape::kPrinting, gGlobalData->GetDPILowRes() );
	if( toReturn == kSuccess )
		toReturn = docUtils->CreatePageItemPNGSnapshot( inPageRef, highResStream, false, IShape::kPrinting, gGlobalData->GetDPIHighRes() );
#else
	if( gGlobalData->IsExportLowRes() )
		toReturn = docUtils->CreatePageItemSnapshot( inPageRef, lowResStream, false, IShape::kPrinting, gGlobalData->GetDPILowRes() );
	if( toReturn == kSuccess )
		toReturn = docUtils->CreatePageItemSnapshot( inPageRef, highResStream, false, IShape::kPrinting, gGlobalData->GetDPIHighRes() );
#endif
	if(toReturn == kSuccess && gGlobalData->IsExportTiff())
	{
		imageFileName = "";
		imageFileName.AppendNumber( inPageRef.GetUID().Get() );
		imageFileName.Append( "_" );
		imageFileName.AppendNumber( inSpreadLayerID.Get() );
		imageFileName.Append( ".tiff");//gif" );

		expFileUtils->GetImageExportFileFolders( inXMLFilePath, lowResFile, highResFile );
		FileUtils::AppendPath( &lowResFile, imageFileName );
		FileUtils::AppendPath( &highResFile, imageFileName );

		InterfacePtr<IPMStream> lowResGIFStream;//( expFileUtils->CreateWriteStreamForImage( lowResFile ) );
		InterfacePtr<IPMStream> highResGIFStream( expFileUtils->CreateWriteStreamForImage( highResFile ) );

		if( gGlobalData->IsExportLowRes() )
			lowResGIFStream.reset( expFileUtils->CreateWriteStreamForImage( lowResFile ) );

		StStreamCloser autoCloseGIFLowRes( lowResGIFStream );
		StStreamCloser autoCloseGIFHighRes( highResGIFStream );

		//docUtils->CreatePageItemGIFSnapshot( inPageRef, lowResGIFStream, true, IShape::kPrinting, gGlobalData->GetDPILowRes() );
		//docUtils->CreatePageItemGIFSnapshot( inPageRef, highResGIFStream, false, IShape::kPrinting, gGlobalData->GetDPIHighRes() );
		//docUtils->CreatePageItemTIFFSnapshot( inPageRef, lowResGIFStream, true, IShape::kPrinting, gGlobalData->GetDPILowRes() );
		if( gGlobalData->IsExportLowRes() )
			toReturn = docUtils->CreatePageItemTIFFSnapshot( inPageRef, lowResGIFStream, false, IShape::kPrinting, gGlobalData->GetDPILowRes() );
		if( toReturn == kSuccess )
			docUtils->CreatePageItemTIFFSnapshot( inPageRef, highResGIFStream, false, IShape::kPrinting, gGlobalData->GetDPIHighRes() );
	}
	return toReturn;
}
