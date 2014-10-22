//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLWriteFacade.cpp $
//	$Revision: 4028 $
//	$Date: 2012-05-29 15:02:07 +0200 (Tue, 29 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAttributeStrand.h"
#include "ICalculatedText.h"
#include "ICalculatedTextResult.h"
#include "ICompositionUtils.h"
#include "IControlledPageItems.h"
#include "IDocument.h"
#include "IFrameList.h"
#include "IGeometry.h"
#include "IGraphicFrameData.h"
#include "IGraphicStyleDescriptor.h"
#include "IHierarchy.h"
#include "IInlineData.h"
#include "IItemStrand.h"
#include "IMasterOverrideList.h"
#include "IMasterPage.h"
#include "IMasterSpread.h"
#include "IMasterSpreadList.h"
//#include "IOwnedItem.h"
#include "IPathGeometry.h"
#include "IPageList.h"
#include "IParcelList.h"
#include "IPasteboardUtils.h"
#include "ISection.h"
#include "ISectionList.h"
#include "ISpread.h"
#include "ISpreadLayer.h"
#include "ISpreadList.h"
#include "IStoryList.h"
#include "ISysFileData.h"
#include "ITableFrame.h"
#include "ITableModel.h"
#include "ITADataPosition.h"
#include "ITextAttrInt16.h"
#include "ITextAttrSpecialGlyph.h"
#include "ITextAttrUtils.h"
#include "ITextFrameColumn.h"
#include "ITextModel.h"
#include "ITextParcelList.h"
#include "ITextStoryThread.h"
#include "ITextStoryThreadDict.h"
#include "ITransform.h"
#include "IWaxGlyphs.h"
#include "IWaxIterator.h"
#include "IWaxLine.h"
#include "IWaxRenderData.h"
#include "IWaxRun.h"
#include "IWaxRunIterator.h"
#include "IWaxStrand.h"
#include "IXMLOutStream.h"
#include "IXMLStreamUtils.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "FileUtils.h"
#include "SplineID.h"		// kSplineItemBoss
#include "TextIterator.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpDocUtils.h"
#include "IZExpFileUtils.h"
#include "IZExpGlobalData.h"
#include "IZExpPageItemUtils.h"
#include "IZExpStoryUtils.h"
#include "IZExpXMLWriteFacade.h"

//IZP General includes
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Articles.h"
#include "CZExpXMLExport_Styles.h"
#include "CZExpXMLExport_Colors.h"
#include "CZExpXMLExport_Paths.h"
#include "CZExpXMLExport_Snapshots.h"
#include "StInDesignUtils.h"
#include "StInDnXmlUtils.h"
#include "StUtils.h"
#include "ZExpXMLConstants.h"
#include "ZPStringUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpXMLWriteFacade>
class CZExpXMLWriteFacade : public inheritClass
{
public:
						CZExpXMLWriteFacade(IPMUnknown * boss);
	virtual				~CZExpXMLWriteFacade();

	ErrorCode			ExportDocument(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc ) const;

	ErrorCode			ExportXMLToFile(
							const IDFile &				inFile,
							const IDocument *			inDoc ) const;

protected:
	ErrorCode			AddAllStories(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc ) const;

	ErrorCode			AddAllSpread(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc ) const;

	void				AddStoryTextContent(
							IXMLOutStream *				ioStream,
							const UIDRef &				inTextModelRef ) const;
#pragma mark -
	ErrorCode			AddSpread(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc,
							const ISpread *				inSpread,
							bool						inIsMaster = false) const;

	void				AddSpreadLayers(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread) const;
	void				AddSpreadLayer(
							IXMLOutStream *				ioStream,
							const ISpreadLayer *		inSpreadLayer,
							const ISpread *				inSpread,
							const int					inIndexInParentList ) const;

	ErrorCode			AddSpreadPages(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IPageList *			inDocPageList ) const;
	ErrorCode			AddPage(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IPageList *			inDocPageList,
							const UIDRef &				inPageRef,
							const int					inPageIndex ) const;	//Index in spread
	ErrorCode			AddMasterPageItems(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IGeometry *			inPageGeometry,
							const UIDRef &				inFinalPageRef,
							const UIDRef &				inCurrentPageRef ) const;	//Index in spread
	ErrorCode			AddPageItems(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IGeometry *			inPageGeometry,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const int					inPageIndex ) const;	//Index in spread
	ErrorCode			AddPageItem(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IGeometry *			inPageGeometry,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const UIDRef &				inPageItemRef ) const;
#pragma mark -
	void				AddTextFrameAttrs(
							IXMLOutStream::AttributeList & ioAttrList,
							const IMultiColumnTextFrame* inMCTextFrame ) const;
	void				AddTextFrameContent(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IGraphicFrameData *	inGraphicFrameData,
							const IMultiColumnTextFrame* inMCTextFrame ) const;
	void				AddTextFrameColumn(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IMultiColumnTextFrame* inMCTextFrame,
							const IWaxStrand *			inWaxStrand,
							const ITextFrameColumn *	inTextFrameColumn ) const;
	void				AddTextWaxLineWithOwnedItem(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IWaxStrand *			inWaxStrand,
							const PMMatrix &			theFrameItemMatrix,
							int32						textStartIndex,
							int32						textEndIndex,
							const ITextFrameColumn *	inTextFrameColumn) const;
	void				AddTextWaxLine(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IWaxLine *			inWaxLine,
							const PMMatrix &			inConversionMatrix,
							const IGeometry *			inFrameGeometry) const;
	void				AddTextRangeOwnedObject(
							IXMLOutStream *				ioStream,
							const IWaxStrand *			inWaxStrand,							
							const UIDRef &				inFinalPageRef,				//cannot be master
							const PMMatrix &			theFrameItemMatrix,
							int32						textStartIndex,
							int32						textEndIndex,
							const ITextFrameColumn *	inTextFrameColumn) const;
	void				AddTextFrameColumnFootnote(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IWaxStrand *			inWaxStrand,
							const PMMatrix &			theFrameItemMatrix,
							const ITextFrameColumn *	inTextFrameColumn) const;
	void				AddInlineChildObjects(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							IHierarchy *				parent) const;
	void				AddWaxRun(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							const IWaxLine *			inWaxLine,
							const IWaxRun *				inWaxRun,
							IAttributeStrand *			inParaAttrStrand,
							IAttributeStrand *			inCharAttrStrand ) const;
	void				AddWaxRunCalculatedText(
							IXMLOutStream *				ioStream,
							ITextModel *				inTextModel,
							const IWaxRun *				inWaxRun) const;
	void				AddWaxRunDropCap(
							ITextModel *				inTextModel,
							IXMLOutStream::AttributeList & ioAttrList,
							const IWaxRun *				inWaxRun) const;
	void				AddWaxRunSpecialCharacters(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							ITextModel *				inTextModel,
							const IWaxRun *				inWaxRun) const;
	void				AddSpecialGlyph(
							IXMLOutStream *				ioStream,
							ITextModel *				inTextModel,
							const TextIndex				inIndex) const;
	void				AddPageNumber(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							ITextModel *				inTextModel,
							const TextIndex				inIndex) const;
	void				AddSectionMarker(
							IXMLOutStream *				ioStream,
							const UIDRef &				inFinalPageRef,				//cannot be master
							ITextModel *				inTextModel,
							const TextIndex				inIndex) const;
	void				AddFootnoteMarker(
							IXMLOutStream *				ioStream,
							ITextModel *				inTextModel,
							const TextIndex				inIndex,
							const UTF32TextChar			inChar) const;
	ErrorCode			AddImageFrameContent(
							IXMLOutStream *				ioStream,
							const ISpread *				inSpread,
							const IGraphicFrameData *	inGraphicFrameData ) const;
	ErrorCode			CreatePagePreview(
							IXMLOutStream *				ioStream,
							const UIDRef &				inPageRef) const;
private:
	void				LogMatrix(
							const PMMatrix &			inMatrix) const;
};


CREATE_PMINTERFACE(CZExpXMLWriteFacade, kZExpXMLWriteFacadeImpl)

CZExpXMLWriteFacade::CZExpXMLWriteFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpXMLWriteFacade::~CZExpXMLWriteFacade()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ExportXMLToFile
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::ExportXMLToFile(
	const IDFile &				inFile,
	const IDocument *			inDoc) const
{
	InterfacePtr<IXMLOutStream> xmlOutStream(Utils<IXMLStreamUtils>()->CreateXMLOutFileStream(inFile, IXMLOutStream::kUTF8));
	if( !xmlOutStream)
		return kIZExpFileWritingError;

	return this->ExportDocument( xmlOutStream, inDoc );
}

//----------------------------------------------------------------------------------------
// ExportDocument
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::ExportDocument(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc) const
{
	LogFunctionEnterExit; 
	ErrorCode toReturn = kSuccess;
	do
	{
		if( !inDoc || !ioStream )
		{
			toReturn = kIZExpParamError;
			break;
		}

		InterfacePtr<const ISysFileData> streamFileData( ioStream, UseDefaultIID() );
		if( !streamFileData )
		{
			toReturn = kIZExpParamError;
			break;
		}

		//Create streams to export data in.
		const IDFile & theXMLFilePath = streamFileData->GetSysFile();

		//Remove all old image files
		IDFile lowResFile, highResFile;
		Utils<IZExpFileUtils> expFileUtils;
		expFileUtils->GetImageExportFileFolders( theXMLFilePath, lowResFile, highResFile );
		expFileUtils->RemoveAllFiles( FileUtils::SysFileToPMString(lowResFile), "*");
		expFileUtils->RemoveAllFiles( FileUtils::SysFileToPMString(highResFile), "*");

		ioStream->Indent(kTrue);

		//Write header
		ioStream->DocHeader( WideString( L"1.0" ) );

		//Write root element
		WideString elementStrRoot( STRXMLTag_Doc );
		ioStream->Push( elementStrRoot, kTrue );

		//Insert attributes for root element
		//TODO: e.g name

		//Iterate on all stories and add them to xml
		toReturn = this->AddAllStories( ioStream, inDoc );

		if( toReturn != kSuccess )
			break;

		//Add style info
		CZExpXMLExportUtils::AddPragraphStyles( ioStream, inDoc );
		CZExpXMLExportUtils::AddCharaStyles( ioStream, inDoc );
		
		//Color Info
		CZExpXMLExportUtils::AddAllSwatches( ioStream, inDoc );
		
		//Articles
#ifdef InDnCS55
		CZExpXMLExportUtils::AddArticlesInDoc( ioStream, inDoc );
#endif

		//Iterate on all spread and add them to xml
		toReturn = this->AddAllSpread( ioStream, inDoc );
		if( toReturn != kSuccess )
			break;

		ioStream->Pop(kTrue);
	}while(false);
	ioStream->Close();

	InterfacePtr<const ISysFileData> streamFileData( ioStream, UseDefaultIID() );
	if( toReturn == kSuccess && streamFileData )
	{
		//Create streams to export data in.
		const IDFile & theXMLFilePath = streamFileData->GetSysFile();

		//Now create preview for each page layer-wise
		toReturn = CZExpXMLExportUtils::ExportPageLayersSnapshot( theXMLFilePath, inDoc );
	}
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddAllStories
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddAllStories(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inDoc );

	ErrorCode toReturn = kSuccess;

	WideString elementStrStories( STRXMLTag_Stories );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStrStories, kTrue );
	StXMLPopper autoPopStories( ioStream );

	InterfacePtr<const IStoryList> storyList (inDoc, UseDefaultIID());

	int32 storyCount = storyList->GetAllTextModelCount();
	UIDRef storyRef;
	for( int32 i = 0; i < storyCount ; ++i )
	{
		storyRef = storyList->GetNthTextModelUID( i );
		this->AddStoryTextContent( ioStream, storyRef );
		toReturn = ErrorUtils::PMGetGlobalErrorCode();
		if( toReturn != kSuccess )
			break;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AddAllSpread
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddAllSpread(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inDoc );

	ErrorCode toReturn = kSuccess;

	WideString elementStrSpreads( STRXMLTag_Spreads );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStrSpreads, kTrue );
	StXMLPopper autoPopSpreads( ioStream );

	InterfacePtr<ISpreadList> spreadList (inDoc, UseDefaultIID());
	ASSERT( spreadList );
	
	int32 spreadCount = spreadList->GetSpreadCount();

	UIDRef spreadRef( ::GetDataBase(inDoc), kInvalidUID );
	for( int32 i = 0; i < spreadCount ; ++i )
	{
		spreadRef.ResetUID( spreadList->GetNthSpreadUID( i ) );
		InterfacePtr<const ISpread> spread (spreadRef, UseDefaultIID());
		if( spread )
		toReturn = this->AddSpread( ioStream, inDoc, spread );
		if( toReturn != kSuccess )
			break;
	}

	//Master spreads.
	InterfacePtr<IMasterSpreadList> masterSpreadList (inDoc, UseDefaultIID());
	ASSERT( masterSpreadList );
	
	int32 masterSpreadCount = masterSpreadList->GetMasterSpreadCount();

	UIDRef masterSpreadRef( ::GetDataBase(inDoc), kInvalidUID );
	for( int32 i = 0; i < masterSpreadCount ; ++i )
	{
		masterSpreadRef.ResetUID( masterSpreadList->GetNthMasterSpreadUID( i ) );
		InterfacePtr<const ISpread> spread (masterSpreadRef, UseDefaultIID());
		if( spread && 0)//Commented as xhtml converter does not have support for Master spreads.
			toReturn = this->AddSpread( ioStream, inDoc, spread, true );
		if( toReturn != kSuccess )
			break;
	}

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddStory
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddStoryTextContent(
	IXMLOutStream *				ioStream,
	const UIDRef &				inTextModelRef) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream );
	InterfacePtr<const ITextModel> textModel( inTextModelRef, UseDefaultIID() );
	if( !textModel )
		return;
	
	WideString elementStrStory( STRXMLTag_Story );
	
	IXMLOutStream::Attribute attrID;
	IXMLOutStream::AttributeList attrList;

	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, inTextModelRef.GetUID() );
	attrList.push_back( attrID );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrStory, attrList );
	StXMLIndentDisabler disableIndent( ioStream );	//Disable indent as story content is written in data
	StXMLPopper autoPopStory( ioStream );

	WideString storyContent;
	Utils<IZExpStoryUtils>()->GetStoryTextContent( textModel, storyContent );
	//Utils<IZExpStoryUtils>()->FixStoryTextIndexWithInlineItems( textModel, storyContent );

#if 1
	ZPStringUtils::ReplaceControlChars( storyContent );
	ioStream->Write( storyContent, kFalse, kTrue);
#else
	ioStream->Write( storyContent, kFalse, kFalse);
#endif
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddSpread
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddSpread(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc,
	const ISpread *				inSpread,
	bool						inIsMaster) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inDoc );
	if( !inSpread )
		return kIZExpParamError;
	
	ErrorCode toReturn = kSuccess;

	WideString elementStrSpread( STRXMLTag_Spread );
	
	IXMLOutStream::Attribute attrID;
	IXMLOutStream::AttributeList attrList;

	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, ::GetUID(inSpread) );
	attrList.push_back( attrID );

	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_IsMasterSpread) );
	CZExpXMLAttrUtils::SetValue_Bool( attrID, inIsMaster );
	
	attrList.push_back( attrID );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrSpread, attrList, kTrue );
	StXMLPopper autoPopSpread( ioStream );

	{	//Don't remove braces
		//Add Element Layers
		WideString elementStrLayers( STRXMLTag_Layers );
	
		ioStream->IndentLevel(kTrue);
		ioStream->Push( elementStrLayers, kTrue );
		StXMLPopper autoPopLayers( ioStream );

		//Add Layer elements
		this->AddSpreadLayers( ioStream, inSpread );
	}

	toReturn = ioStream->Flush();

	InterfacePtr<const IPageList> docPageList( inDoc, UseDefaultIID() );
	ASSERT( docPageList );

	if( toReturn == kSuccess )
	{	//Don't remove braces
		//Add Element Pages
		WideString elementStrPages( STRXMLTag_Pages );
	
		ioStream->IndentLevel(kTrue);
		ioStream->Push( elementStrPages, kTrue );
		StXMLPopper autoPopPages( ioStream );

		//Add page elements
		toReturn = this->AddSpreadPages( ioStream, inSpread, docPageList );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AddSpreadLayers
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddSpreadLayers(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inSpread );
	InterfacePtr<const IHierarchy> spreadHierarchy( inSpread, UseDefaultIID() );
	ASSERT( spreadHierarchy );

	int32 spreadLayerCount = spreadHierarchy->GetChildCount();
	UIDRef spreadLayerUIDRef( ::GetDataBase(inSpread), kInvalidUID );
	//Starting from 2 as 0 is pages layer & 1 is private layer
	for( int32 i = 2; i < spreadLayerCount ; ++i )
	{
		spreadLayerUIDRef.ResetUID( spreadHierarchy->GetChildUID( i ) );
		InterfacePtr<ISpreadLayer> spreadLayer( spreadLayerUIDRef, UseDefaultIID() );
		ASSERT(spreadLayer);
		if( spreadLayer->GetIsGuideLayer() == kFalse )
			this->AddSpreadLayer( ioStream, spreadLayer, inSpread, i );
	}
}

//----------------------------------------------------------------------------------------
// AddSpreadPages
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddSpreadPages(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IPageList *			inDocPageList ) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inDocPageList );
	if( !inSpread )
		return kIZExpParamError;
	
	ErrorCode toReturn = kSuccess;
	int32 pageCount = const_cast<ISpread*>(inSpread)->GetNumPages();

	UIDRef pageRef( ::GetDataBase(inSpread), kInvalidUID );

	for( int32 i = 0; i < pageCount ; ++i )
	{
		pageRef.ResetUID( const_cast<ISpread*>(inSpread)->GetNthPageUID( i ) );

		toReturn = this->AddPage( ioStream, inSpread, inDocPageList, pageRef, i );
		if( toReturn == kSuccess )
			toReturn = this->CreatePagePreview(ioStream, pageRef);
		if( toReturn != kSuccess )
			break;
	}

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddSpreadLayer
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddSpreadLayer(
	IXMLOutStream *				ioStream,
	const ISpreadLayer *		inSpreadLayer,
	const ISpread *				inSpread,
	const int					inIndexInParentList ) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inSpread );
	if( !inSpreadLayer )
		return;
	
	WideString elementStrSpreadLayer( STRXMLTag_SpreadLayer );
	
	IXMLOutStream::Attribute attrID;
	IXMLOutStream::AttributeList attrList;

	//ID
	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, ::GetUID(inSpreadLayer) );
	attrList.push_back( attrID );

	//Name
	IXMLOutStream::Attribute attrName;
	attrName.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	PMString layerName;
	const_cast<ISpreadLayer*>(inSpreadLayer)->GetName(&layerName);
	attrName.fValue = WideString( layerName );
	attrList.push_back( attrName );

	//IsVisible
	IXMLOutStream::Attribute attrVisible;
	attrVisible.fName.SetX16String( TOWSTRING(STRXMLAttr_IsVisible) );
	CZExpXMLAttrUtils::SetValue_Bool(attrVisible, const_cast<ISpreadLayer*>(inSpreadLayer)->IsVisible() );
	attrList.push_back( attrVisible );

	//Index in parent list
	IXMLOutStream::Attribute attrOrder;
	attrOrder.fName.SetX16String( TOWSTRING(STRXMLAttr_Zorder) );
	CZExpXMLAttrUtils::SetValue_Int( attrOrder, inIndexInParentList );
	attrList.push_back( attrOrder );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrSpreadLayer, attrList, kTrue );
	StXMLPopper autoPopSpreadLayer( ioStream );

}

//----------------------------------------------------------------------------------------
// AddPage
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddPage(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IPageList *			inDocPageList,
	const UIDRef &				inPageRef,
	const int					inPageIndex) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inDocPageList );
	if( !inSpread || inPageRef.GetUID() == kInvalidUID )
		return kIZExpParamError;
	
	ErrorCode toReturn = kSuccess;

	WideString elementStrPage( STRXMLTag_Page );
	
	IXMLOutStream::Attribute attrID;
	IXMLOutStream::AttributeList attrList;

	//ID
	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, inPageRef.GetUID() );
	attrList.push_back( attrID );

	//Name
	IXMLOutStream::Attribute attrName;
	attrName.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	PMString pageName;
	inDocPageList->GetPageString( inPageRef.GetUID(), &pageName, kTrue, kTrue, kActualType );
	attrName.fValue = WideString( pageName );
	attrList.push_back( attrName );

	//Section Name
	IXMLOutStream::Attribute attrName1;
	attrName1.fName.SetX16String( TOWSTRING(STRXMLAttr_SectionName) );
	inDocPageList->GetSectionString( inPageRef.GetUID(), &pageName);
	attrName1.fValue = WideString( pageName );
	attrList.push_back( attrName1 );

	//Page Index
	IXMLOutStream::Attribute attrIndex;
	attrIndex.fName.SetX16String( TOWSTRING(STRXMLAttr_PageIndex) );
	CZExpXMLAttrUtils::SetValue_Int( attrIndex, inDocPageList->GetPageIndex(inPageRef.GetUID()) );
	attrList.push_back( attrIndex );

	//SectionIndex
	attrIndex.fName.SetX16String( TOWSTRING(STRXMLAttr_SectionIndex) );
	CZExpXMLAttrUtils::SetValue_Int( attrIndex, inDocPageList->GetSectionIndex(inPageRef.GetUID()) );
	attrList.push_back( attrIndex );

	//Page Rect in spread co-ordinates
	InterfacePtr<const IGeometry> pageGeometry( inPageRef, UseDefaultIID());
	ASSERT( pageGeometry );
	PMRect pageRect = pageGeometry->GetStrokeBoundingBox();
	CZExpXMLAttrUtils::AddRect( attrList, pageRect );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrPage, attrList, kTrue );
	StXMLPopper autoPopSpreadLayer( ioStream );
	
	//Add page items
	toReturn = this->AddMasterPageItems( ioStream, inSpread, pageGeometry, inPageRef, inPageRef );
	if( toReturn == kSuccess )
		toReturn = this->AddPageItems( ioStream, inSpread, pageGeometry, inPageRef, inPageIndex );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// AddMasterPageItems
//	Index in spread
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddMasterPageItems(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IGeometry *			inPageGeometry,
	const UIDRef &				inFinalPageRef,
	const UIDRef &				inCurrentPageRef) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream );
	if( !inSpread )
		return kIZExpParamError;

	InterfacePtr<IMasterPage> masterPage(inCurrentPageRef, UseDefaultIID());
	if (!masterPage)
	{
		ASSERT (masterPage);
		return kIZExpParamError;
	}

#ifdef InDnCS5
	UID masterSpreadUID = masterPage->GetMasterSpreadUID();	//this is master spread, instead of page.
#else
	UID masterSpreadUID = masterPage->GetMasterPageUID();	//this is master spread, instead of page.
#endif
	if( masterSpreadUID == kInvalidUID )
		return kSuccess;

	InterfacePtr<const IMasterSpread> masterSpread(::GetDataBase(masterPage), masterSpreadUID, UseDefaultIID());
	if (!masterSpread)
		return kIZExpParamError;

	ErrorCode toReturn = kSuccess;

	InterfacePtr<IMasterOverrideList> pageMasterOverrideList(masterPage, UseDefaultIID());
	int32 pageOverrideCount = 0;
	if (pageMasterOverrideList)
		pageOverrideCount = pageMasterOverrideList->OverrideCount();

	InterfacePtr<ISpread> masterPageSpread( masterSpread, UseDefaultIID());
	IDataBase *db = ::GetDataBase(masterPageSpread);
	UIDList masterPageItemList(db);
#ifdef InDnCS5
	UID masterPageUID = masterPageSpread->GetNthPageUID(masterPage->GetMasterSpreadPageIndex());
	masterPageSpread->GetItemsOnPage(masterPage->GetMasterSpreadPageIndex(), &masterPageItemList);
#else
	UID masterPageUID = masterPageSpread->GetNthPageUID(masterPage->GetMasterIndex());
	masterPageSpread->GetItemsOnPage(masterPage->GetMasterIndex(), &masterPageItemList);
#endif

	UIDRef masterPageRef(db, masterPageUID);
	int32 masterPageItemListLength = masterPageItemList.Length();
	if (pageOverrideCount)	//Only when something is overridden
	{
		UID currentPageUID = inCurrentPageRef.GetUID();
		for (int32 i = 0; i < masterPageItemListLength; ++i )
		{
			UID pageUID = kInvalidUID;
			UIDRef masterPageItemRef = masterPageItemList.GetRef(i);
			InterfacePtr<IControlledPageItems> controlledItems(masterPageItemRef, IID_ICONTROLLEDPAGEITEMS);
			if (controlledItems)
			{
				//Find if this is overridden for the current page. If not then only we will export it here.
				int32 controlledItemCount = controlledItems->ControlledCount();
				for (int32 itemIndex = 0; itemIndex < controlledItemCount; ++itemIndex)
				{
					UID overridePageItemUID = controlledItems->GetControlledPageItem(itemIndex, &pageUID);
					//Note: overridePageItemUID page item can be zero, so don't compare it current page's overriden items.
					if (currentPageUID == pageUID)	//Overriden in page
						break;
				}
			}

			if (currentPageUID != pageUID)
				toReturn = this->AddPageItem( ioStream, masterPageSpread, inPageGeometry, inFinalPageRef, masterPageItemRef );
			if( toReturn != kSuccess )
				break;
		}
	}
	else
	{
		for (int32 i = 0; i < masterPageItemListLength; ++i )
		{
			UIDRef masterPageItemRef = masterPageItemList.GetRef(i);
			toReturn = this->AddPageItem( ioStream, masterPageSpread, inPageGeometry, inFinalPageRef, masterPageItemRef );
			if( toReturn != kSuccess )
				break;
		}
	}

	//Call for parent master page for current master
	if( toReturn == kSuccess )
		toReturn = this->AddMasterPageItems( ioStream, masterPageSpread, inPageGeometry, inFinalPageRef, masterPageRef );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AddPageItems
//	Index in spread
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddPageItems(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IGeometry *			inPageGeometry,
	const UIDRef &				inFinalPageRef,
	const int					inPageIndex) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream );
	if( !inSpread )
		return kIZExpParamError;

	ErrorCode toReturn = kSuccess;
	// Iterate over all the pages on the spread.
	UIDList pageItemList(::GetDataBase(inSpread));

	// don't include the page object itself
	// don't include items that lie on the pasteboard because we only want what's on the page?  
	const_cast<ISpread*>(inSpread)->GetItemsOnPage( inPageIndex, &pageItemList, kFalse, kFalse );
	
	// Iterate the page items and save off the UIDs of frames.
	int32 pageItemListLength = pageItemList.Length();
	UIDRef pageItemRef;
	for (int32 i = 0; i < pageItemListLength; ++i )
	{
		pageItemRef = pageItemList.GetRef(i);
		toReturn = this->AddPageItem( ioStream, inSpread, inPageGeometry, inFinalPageRef, pageItemRef );
		if( toReturn != kSuccess )
			break;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AddPageItem
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddPageItem(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IGeometry *			inPageGeometry,
	const UIDRef &				inFinalPageRef,
	const UIDRef &				inPageItemRef) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inSpread );

	ErrorCode toReturn = ErrorUtils::PMGetGlobalErrorCode();
	if( toReturn != kSuccess )
		return toReturn;

	InterfacePtr<const IGraphicFrameData> graphicFrameData( inPageItemRef, UseDefaultIID());
	if (!graphicFrameData )
		return kSuccess;

	WideString elementStrPageItem( STRXMLTag_TextFrame );

	InterfacePtr<const IMultiColumnTextFrame> mcTextFrame( graphicFrameData->QueryMCTextFrame() );
	bool isGraphic = (mcTextFrame == nil);
	if( isGraphic )
		elementStrPageItem = WideString( STRXMLTag_ImageFrame );
	
	IXMLOutStream::Attribute attrID;
	IXMLOutStream::AttributeList attrList;

	//ID
	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, inPageItemRef.GetUID() );
	attrList.push_back( attrID );

	//Frame Rect
	InterfacePtr<const IGeometry> frameGeometry( graphicFrameData, UseDefaultIID());
	ASSERT( frameGeometry );
	PMRect frameRect = frameGeometry->GetStrokeBoundingBox();
	frameRect = Utils<IZExpPageItemUtils>()->ConvertCoordinatesPageItemToPage( inSpread, inPageGeometry, frameGeometry, frameRect );
	CZExpXMLAttrUtils::AddRect( attrList, frameRect );

	this->AddTextFrameAttrs( attrList, mcTextFrame );

	//Other frame properties
	InterfacePtr<const ITransform> pageItemTransform( graphicFrameData, UseDefaultIID() );
	if( pageItemTransform )
	{
		IXMLOutStream::Attribute attrReal;
		attrReal.fName.SetX16String( TOWSTRING(STRXMLAttr_RotationAngle) );
		CZExpXMLAttrUtils::SetValue_Real( attrReal, pageItemTransform->GetItemRotationAngle() );
		attrList.push_back( attrReal );
	}

	//LayerID
	InterfacePtr<IHierarchy> frameHierarchy( graphicFrameData, UseDefaultIID() );
	ASSERT( frameHierarchy );

	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_LayerID) );
	CZExpXMLAttrUtils::SetValue_UID( attrID, frameHierarchy->GetLayerUID() );
	attrList.push_back( attrID );

	//Index in parent list
	InterfacePtr<const IHierarchy> frameParentHierarchy( frameHierarchy->QueryParent() );
	ASSERT( frameParentHierarchy );
	
	attrID.fName.SetX16String( TOWSTRING(STRXMLAttr_Zorder) );
	CZExpXMLAttrUtils::SetValue_Int( attrID, frameParentHierarchy->GetChildIndex(frameHierarchy) );
	attrList.push_back( attrID );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrPageItem, attrList, kTrue );
	StXMLPopper autoPopSpreadLayer( ioStream );

	//Path/Shape Info
	InterfacePtr<const IPathGeometry> framePathGeometry( graphicFrameData, UseDefaultIID() );
	if( framePathGeometry )
		CZExpXMLExportUtils::AddPathInfo( ioStream, framePathGeometry );

	//Add Frame style info
	InterfacePtr<const IGraphicStyleDescriptor> frameGraphicStyleDesc( graphicFrameData, UseDefaultIID() );
	if( frameGraphicStyleDesc )
		CZExpXMLExportUtils::AddGraphicStyles( ioStream, frameGraphicStyleDesc );
	
	//Insert page item content info
	if( isGraphic )
		toReturn = this->AddImageFrameContent( ioStream, inSpread, graphicFrameData );
	else
		this->AddTextFrameContent( ioStream, inSpread, inFinalPageRef, graphicFrameData, mcTextFrame );

	return toReturn;
}
#pragma mark -
//----------------------------------------------------------------------------------------
// AddTextFrameAttrs
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextFrameAttrs(
	IXMLOutStream::AttributeList & ioAttrList,
	const IMultiColumnTextFrame* inMCTextFrame) const
{
	if( !inMCTextFrame )
		return;

	IXMLOutStream::Attribute attr;
	//storyID
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_StoryID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, inMCTextFrame->GetTextModelUID() );
	ioAttrList.push_back( attr );
	
	//text span
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexStart) );
	CZExpXMLAttrUtils::SetValue_Int( attr, inMCTextFrame->TextStart() );
	ioAttrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexEnd) );
	CZExpXMLAttrUtils::SetValue_Int( attr, inMCTextFrame->TextStart()+ inMCTextFrame->TextSpan() );
	ioAttrList.push_back( attr );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddTextFrameContent
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextFrameContent(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const UIDRef &				inFinalPageRef,
	const IGraphicFrameData *	inGraphicFrameData,
	const IMultiColumnTextFrame* inMCTextFrame ) const
{
	ASSERT( ioStream && inSpread && inGraphicFrameData && inMCTextFrame );

	if( inMCTextFrame->GetIsEmpty() )
		return;

	InterfacePtr<const IFrameList> frameList( inMCTextFrame->QueryFrameList() );
	ASSERT( frameList );

	InterfacePtr<const IWaxStrand> waxStrand( frameList, UseDefaultIID() );	//Wax strand is common for whole story, it is on kFrameListBoss
	ASSERT( waxStrand );

	InterfacePtr<const IHierarchy> mcFrameHierarchy( inMCTextFrame, UseDefaultIID() );
	ASSERT( mcFrameHierarchy );

	IDataBase * db = ::GetDataBase( inMCTextFrame );
	//Add Columns
	int32 colCount = mcFrameHierarchy->GetChildCount();
	for( int i = 0; i < colCount ; ++i )
	{
		InterfacePtr<const ITextFrameColumn> tfColumn( db, mcFrameHierarchy->GetChildUID(i), UseDefaultIID() );
		ASSERT( tfColumn );
		this->AddTextFrameColumn( ioStream, inSpread, inFinalPageRef, inMCTextFrame, waxStrand, tfColumn );
	}
}

//----------------------------------------------------------------------------------------
// AddTextFrameColumn
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextFrameColumn(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const UIDRef &				inFinalPageRef,
	const IMultiColumnTextFrame* inMCTextFrame,
	const IWaxStrand *			inWaxStrand,
	const ITextFrameColumn *	inTextFrameColumn) const
{
	LogFunctionEnterExit; 
	ASSERT( ioStream && inSpread && inMCTextFrame && inWaxStrand && inTextFrameColumn );

	WideString elementStrTextCol( STRXMLTag_TextFrameColumn );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	//ID
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, ::GetUID(inTextFrameColumn) );
	attrList.push_back( attr );

	//text span
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexStart) );
	CZExpXMLAttrUtils::SetValue_Int( attr, inTextFrameColumn->TextStart() );
	attrList.push_back( attr );

	int32 textIndexEnd = inTextFrameColumn->TextStart()+ inTextFrameColumn->TextSpan();
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexEnd) );
	CZExpXMLAttrUtils::SetValue_Int( attr, textIndexEnd );
	attrList.push_back( attr );

	//Frame Rect
	InterfacePtr<const IGeometry> frameGeometry( inMCTextFrame, UseDefaultIID());
	ASSERT( frameGeometry );
	PMRect frameRect = frameGeometry->GetStrokeBoundingBox();
	//frameRect = Utils<IZExpPageItemUtils>()->ConvertCoordinatesPageItemToPage( inSpread, inPageGeometry, frameGeometry, frameRect );
	CZExpXMLAttrUtils::AddRect( attrList, frameRect );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrTextCol, attrList, kTrue );
	StXMLPopper autoPopTextColumn( ioStream );

	PMMatrix theFrameItemMatrix;	//To be used to convert the line xy to frame rect's left,top being origin.
	InterfacePtr<const ITransform> frameTransform( frameGeometry, UseDefaultIID() );
	ASSERT( frameTransform );
	theFrameItemMatrix = frameTransform->GetInnerToParentMatrix();	//Here text frame and MC frame have same corrdinates
	theFrameItemMatrix.TranslateTo( -frameRect.Left(), -frameRect.Top() );

	this->AddTextWaxLineWithOwnedItem(ioStream, inFinalPageRef, inWaxStrand, theFrameItemMatrix, inTextFrameColumn->TextStart(), inTextFrameColumn->TextStart() + inTextFrameColumn->TextSpan(), inTextFrameColumn);
	
	//Footnotes are not iterated along with owned items 
	// as the footnotes streatching to mulitple text columns are not coming again when looking for second column owned items
	// The table does come again in owned items again for each text frame column in which its cells reside.
	this->AddTextFrameColumnFootnote(ioStream, inFinalPageRef, inWaxStrand, theFrameItemMatrix, inTextFrameColumn);
}

//----------------------------------------------------------------------------------------
// AddTextWaxLine
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextWaxLineWithOwnedItem(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	const IWaxStrand *			inWaxStrand,
	const PMMatrix &			theFrameItemMatrix,
	int32						textStartIndex,
	int32						textEndIndex,
	const ITextFrameColumn *	inTextFrameColumn) const
{
	ASSERT( ioStream && inWaxStrand );

	const IWaxIterator * waxIter = inWaxStrand->NewReadOnlyWaxIterator();
	if( !waxIter )
		return;

	const IWaxLine * waxLine = waxIter->GetFirstWaxLine( textStartIndex );
	if( !waxLine )
	{
		delete waxIter;
		return;
	}

	InterfacePtr<const IMultiColumnTextFrame> textFrame(inTextFrameColumn->QueryMultiColumnTextFrame());
	InterfacePtr<const IGeometry> frameGeometry(textFrame, IGeometry::kDefaultIID);

	int32 lineTextStart = waxLine->TextOrigin();
	int32 lineTextEnd = lineTextStart + waxLine->GetTextSpan();
	while( waxLine && lineTextEnd <= textEndIndex )
	{
		this->AddTextWaxLine( ioStream, inFinalPageRef, waxLine, theFrameItemMatrix, frameGeometry);

		waxLine = waxIter->GetNextWaxLine();
		if( waxLine )
		{
			lineTextStart = waxLine->TextOrigin();
			lineTextEnd = lineTextStart + waxLine->GetTextSpan();
		}
	}

	//Code for owned objects
	this->AddTextRangeOwnedObject( ioStream, inWaxStrand, inFinalPageRef, theFrameItemMatrix, textStartIndex, textEndIndex, inTextFrameColumn );

	delete waxIter;
}

//----------------------------------------------------------------------------------------
// AddTextWaxLine
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextWaxLine(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	const IWaxLine *			inWaxLine,
	const PMMatrix &			inConversionMatrix,
	const IGeometry *			inFrameGeometry) const
{
	ASSERT( ioStream && inWaxLine );

	WideString elementStrTextLine( STRXMLTag_TextLine );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	//text span
	int32 lineTextStart = inWaxLine->TextOrigin();
	int32 lineTextEnd = lineTextStart + inWaxLine->GetTextSpan();

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexStart) );
	CZExpXMLAttrUtils::SetValue_Int( attr, lineTextStart );
	attrList.push_back( attr );

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexEnd) );
	CZExpXMLAttrUtils::SetValue_Int( attr, lineTextEnd );
	attrList.push_back( attr );

	//Line X, Y, Height, Width
	PMReal tempVal;
	PMPoint pt;

	PMRect frameRect = inFrameGeometry->GetStrokeBoundingBox();
	PMPoint ptFrameTop = frameRect.LeftTop();
	PMReal lineAdvance = inWaxLine->GetYAdvance();
	PMReal lineYAdjustment = inWaxLine->GetYPositionAdj();
	PMReal lineLeadingOffset = inWaxLine->GetLeadingModelOffset();
	PMReal lineLeadingHeightDiff = inWaxLine->GetLineHeightLeadingDiff();

	pt.X() = inWaxLine->GetXPosition();
	pt.Y() = inWaxLine->GetYPosition();
	
	//PMReal contentBottom = inWaxLine->GetContentBottom();
	//PMReal leadingModelOffset = inWaxLine->GetLeadingModelOffset();

	pt.Y() = pt.Y() - lineAdvance;	//Try to move line y to top of line.

	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Line Y : %f, Advance : %f, YAdjustment : %f, leadingOffset : %f, heightDiff = %f ",
						ToFloat(pt.Y()), ToFloat(lineAdvance), ToFloat(lineYAdjustment), ToFloat(lineLeadingOffset), ToFloat(lineLeadingHeightDiff) );
	//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Item X : %f, Y : %f", pt.X(), pt.Y());

	PMMatrix matFrame = inWaxLine->GetToFrameMatrix();
	PMMatrix matParcel = inWaxLine->GetToParcelMatrix();
	//PMMatrix matTrans = inWaxLine->GetTransform();

	//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Frame matrix ");
	//LogMatrix(matFrame);
	//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Parcel matrix ");
	//LogMatrix(matParcel);
	//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Transform matrix ");
	//LogMatrix(matTrans);

	if ( matFrame != matParcel ) //For inline objects
		matFrame.Transform( &pt );
	
	//Make it relative to frame top left as 0,0
	//TODO: use SetOrigin of matrix and then transform, instead of subtracting top left.
	pt = pt - ptFrameTop;

	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Line Y after transform: %f", ToFloat(pt.Y()) );


	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_X) );
	//tempVal = inWaxLine->GetXPosition();
	CZExpXMLAttrUtils::SetValue_Real( attr, pt.X() );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Y) );
	//tempVal = inWaxLine->GetYPosition();// - inWaxLine->GetLineHeightLeadingDiff();
	CZExpXMLAttrUtils::SetValue_Real( attr, pt.Y() );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Height) );
	tempVal = inWaxLine->GetLineHeight();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Width) );
	tempVal = inWaxLine->GetWidth();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );


	//Ends with hyphen
	if( inWaxLine->LineEndsWithHyphen() )
	{
		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_EndWithHyphen) );
		CZExpXMLAttrUtils::SetValue_Bool( attr, inWaxLine->LineEndsWithHyphen() );
		attrList.push_back( attr );
	}

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrTextLine, attrList, kTrue );
	StXMLPopper autoPopTextLine( ioStream );

	InterfacePtr<const ITextModel> textModel( inWaxLine->QueryTextModel() );
	ASSERT( textModel );

	InterfacePtr<IAttributeStrand> paraAttrStrand( (IAttributeStrand*)textModel->QueryStrand( kParaAttrStrandBoss , IAttributeStrand::kDefaultIID ) );
	ASSERT( paraAttrStrand );

	InterfacePtr<IAttributeStrand> charAttrStrand( (IAttributeStrand*)textModel->QueryStrand( kCharAttrStrandBoss , IAttributeStrand::kDefaultIID ) );
	ASSERT( charAttrStrand );

	IWaxRunIterator * waxRunIter = inWaxLine->QueryWaxRunIterator();
	if( !waxRunIter )
		return;
	const IWaxRun * waxRun = waxRunIter->GetFirstRun();
	while( waxRun )
	{
		this->AddWaxRun( ioStream, inFinalPageRef, inWaxLine, waxRun, paraAttrStrand, charAttrStrand );
		waxRun = waxRunIter->GetNextRun();
	}
	delete waxRunIter;
}

//----------------------------------------------------------------------------------------
// AddTextRangeOwnedObject
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextRangeOwnedObject(
	IXMLOutStream *				ioStream,
	const IWaxStrand *			inWaxStrand,
	const UIDRef &				inFinalPageRef,
	const PMMatrix &			theFrameItemMatrix,
	int32						textStartIndex,
	int32						textEndIndex,
	const ITextFrameColumn *	inTextFrameColumn) const
{
	ASSERT( ioStream );

	const IWaxIterator * waxIter = inWaxStrand->NewReadOnlyWaxIterator();
	if( !waxIter )
		return;
	StPtrDeleter<IWaxIterator> autoDelWaxIterator( waxIter, true );

	const IWaxLine *waxLine = waxIter->GetFirstWaxLine(textStartIndex);	

	InterfacePtr<const ITextModel> textModel( waxLine->QueryTextModel() );
	ASSERT( textModel );
	if (!textModel)
		return;

	InterfacePtr<const IItemStrand> itemStrand((IItemStrand*)textModel->QueryStrand(kOwnedItemStrandBoss, IItemStrand::kDefaultIID));	
	OwnedItemDataList ownedList;
	itemStrand->CollectOwnedItems(textStartIndex, textEndIndex - textStartIndex, &ownedList);
	int32 ownedItemCount = ownedList.size();
	if (!ownedItemCount)
		return;

	UID textFrameUID = ::GetUID(inTextFrameColumn);
	IDataBase* database = ::GetDataBase(textModel);
	for (int32 ownedItemIndex = 0; ownedItemIndex < ownedItemCount ; ++ownedItemIndex) 
	{
		UID inlineUID = ownedList[ownedItemIndex].fUID;
		ClassID classId = ownedList[ownedItemIndex].fClassID;
		
		//Note: Footnotes not handled here, see comments for AddTextFrameColumnFootnote call above

		if (classId == kTableFrameBoss)	//Table
		{
			InterfacePtr<ITableFrame> tableFrame(database, inlineUID, UseDefaultIID());
			ASSERT (tableFrame);
			if (!tableFrame)
				continue;

			InterfacePtr<ITableModel> tableModel(tableFrame->QueryModel());
			InterfacePtr<ITextStoryThreadDict> textStoryThreadDict(tableModel, UseDefaultIID());
			ASSERT(textStoryThreadDict);
			if (!textStoryThreadDict) 
				continue;

			ITableModel::const_iterator iterTable(tableModel->begin());
			ITableModel::const_iterator endTable(tableModel->end());
			
			for (; iterTable != endTable; ++iterTable)
			{
				GridAddress gridAddress = *iterTable;
				const GridID gridID = tableModel->GetGridID(gridAddress);				
				if (gridID == kInvalidGridID)
					continue;

				InterfacePtr<ITextStoryThread> textStoryThread(textStoryThreadDict->QueryThread(gridID));
				ASSERT(textStoryThread);
				if (textStoryThread == nil)
					continue;

				InterfacePtr<const IParcelList> parcelList(textStoryThread,UseDefaultIID());
				ASSERT(parcelList);
				if( !parcelList )
					continue;
				ASSERT((parcelList->GetParcelCount()) == 1);
				const ParcelKey parcelKey = parcelList->GetFirstParcelKey();
				InterfacePtr<const IParcel> parcel(parcelList->QueryParcel(parcelKey));
				UID frameUID = parcel->GetFrameUID();
				if (textFrameUID != frameUID)
					continue;

				int32 start = textStoryThread->GetTextStart();
				int32 end = start + textStoryThread->GetTextSpan();

				this->AddTextWaxLineWithOwnedItem(ioStream, inFinalPageRef, inWaxStrand, theFrameItemMatrix, start, end, inTextFrameColumn);
			}
		}
		else if (classId == kInlineBoss)	//Inline object
		{
			//image and textbox
			InterfacePtr<const IInlineData> inlineData(database, inlineUID, UseDefaultIID());
			if (!inlineData) 
				continue;           // not an inline item, so skip

			InterfacePtr<IHierarchy> parentHierarchy(inlineData, UseDefaultIID());
			if (!parentHierarchy) 
				continue; // can't get IHierarchy, so skip 

			InterfacePtr<IHierarchy> splineHierarchy(parentHierarchy->QueryChild(0));
			if (splineHierarchy) 
				this->AddInlineChildObjects(ioStream, inFinalPageRef, splineHierarchy);
		}
	}
}

//----------------------------------------------------------------------------------------
// AddTextFrameColumnFootnote
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddTextFrameColumnFootnote(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	const IWaxStrand *			inWaxStrand,
	const PMMatrix &			theFrameItemMatrix,
	const ITextFrameColumn *	inTextFrameColumn) const
{	
	if (!inTextFrameColumn)
	{
		ASSERT (inTextFrameColumn);	
		return;
	}
	
	InterfacePtr<const ITextModel> textModel(inTextFrameColumn->QueryTextModel());
	if (!textModel)
	{
		ASSERT (textModel);
		return;
	}
	
	int32 primarySpan = textModel->GetPrimaryStoryThreadSpan()-1;
	if (primarySpan == 0)
		return;
	
	InterfacePtr<const IItemStrand> itemStrand((IItemStrand*)textModel->QueryStrand(kOwnedItemStrandBoss, IItemStrand::kDefaultIID));	
	OwnedItemDataList ownedList;
	itemStrand->CollectOwnedItems(0, primarySpan, &ownedList);
	int32 ownedItemCount = ownedList.size();
	if (!ownedItemCount)
		return;

	UID textFrameUID = ::GetUID(inTextFrameColumn);
	IDataBase* database = ::GetDataBase(textModel);
	for (int32 ownedItemIndex = 0; ownedItemIndex < ownedItemCount ; ++ownedItemIndex) 
	{
		UID inlineUID = ownedList[ownedItemIndex].fUID;
		ClassID classId = ownedList[ownedItemIndex].fClassID;

		if (classId != kFootnoteReferenceBoss)	//Ignore others
			continue;

		InterfacePtr<ITextStoryThread> textStoryThread(database, inlineUID, UseDefaultIID());
		if (!textStoryThread)
		{
			ASSERT (textStoryThread);
			continue;
		}

		InterfacePtr<const IParcelList> parcelList(textStoryThread, UseDefaultIID());
		if (!parcelList)
			continue;

		InterfacePtr<ITextParcelList> textParcelList(parcelList, UseDefaultIID());
		if (!textParcelList)
		{
			ASSERT (textParcelList);
			continue;
		}

		int32 parcelCount = parcelList->GetParcelCount();
		for (int32 index = 0; index < parcelCount; ++index)
		{
			ParcelKey key = parcelList->GetNthParcelKey(index);
			InterfacePtr<const IParcel> parcel(parcelList->QueryParcel(key));
			if (!parcel)
				continue;

			UID frameId = parcel->GetFrameUID();
			if (textFrameUID != frameId)
				continue;

			int32 start = textParcelList->GetTextStart(key);
			int32 end = start + textParcelList->GetTextSpan(key);

			//Patch: Footnote marker & seprator are not being seprated by a waxrun,
			//So we will skip the start index here and add the text run before it manually here.
			//TODO:

			this->AddTextWaxLineWithOwnedItem(ioStream, inFinalPageRef, inWaxStrand, theFrameItemMatrix, start, end, inTextFrameColumn);
		}
	}
}

//----------------------------------------------------------------------------------------
// AddInlineChildObjects
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddInlineChildObjects(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	IHierarchy *				inSplineHierarchy) const
{
	if (!inSplineHierarchy)
		return;

	ClassID classId = ::GetClass(inSplineHierarchy);
	if (classId == kSplineItemBoss)	//Consider only spline item
	{
		UIDRef uidRef = ::GetUIDRef(inSplineHierarchy);
		InterfacePtr<const IGeometry> geometry(inFinalPageRef, IGeometry::kDefaultIID );
		InterfacePtr<const ISpread> spread(Utils<IPasteboardUtils>()->QuerySpread(inSplineHierarchy)); 
		if (!(geometry && spread))
			return;

		this->AddPageItem(ioStream, spread, geometry, inFinalPageRef, uidRef);
	}
}

//----------------------------------------------------------------------------------------
// AddWaxRun
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddWaxRun(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	const IWaxLine *			inWaxLine,
	const IWaxRun *				inWaxRun,
	IAttributeStrand *			inParaAttrStrand,
	IAttributeStrand *			inCharAttrStrand) const
{
	ASSERT( ioStream && inWaxRun );

	//TODO: this element must be different for
	// Glyphs, InlineGraphic, Table, Inserted char, Footnote char, Text variable
	// This will be checked with first char of the run in story.
	WideString elementStrTextRun( STRXMLTag_TextRun );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	//text span
	int32 runTextStart = inWaxRun->TextOrigin();
	int32 runTextEnd = runTextStart + inWaxRun->GetWaxSpan();

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexStart) );
	CZExpXMLAttrUtils::SetValue_Int( attr, runTextStart );
	attrList.push_back( attr );

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexEnd) );
	CZExpXMLAttrUtils::SetValue_Int( attr, runTextEnd );
	attrList.push_back( attr );

	//Line X, Y, Height, Width
	PMReal tempVal;

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_X) );
	tempVal = inWaxRun->GetXPosition();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Y) );
	tempVal = inWaxRun->GetYPosition();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Height) );
	tempVal = inWaxRun->GetHeight();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Width) );
	tempVal = inWaxRun->GetWidth();
	CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
	attrList.push_back( attr );


	//Style ID
	UID styleID = inParaAttrStrand->GetStyleUID( runTextStart, nil );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_StyleIDPara) );
	CZExpXMLAttrUtils::SetValue_UID( attr, styleID );
	attrList.push_back( attr );

	styleID = inCharAttrStrand->GetStyleUID( runTextStart, nil );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_StyleIDChar) );
	CZExpXMLAttrUtils::SetValue_UID( attr, styleID );
	attrList.push_back( attr );

	//Wax run boss Class id
	ClassID classId = ::GetClass(inWaxRun);
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ClassID) );
	CZExpXMLAttrUtils::SetValue_Int( attr, classId.Get() );
	attrList.push_back( attr );

	//Scaling
	InterfacePtr<const IWaxRenderData> waxRenderData( inWaxRun, UseDefaultIID() );
	if( waxRenderData )
	{
		const PMMatrix & fontMatrix = waxRenderData->GetFontMatrix();
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Font matrix ");
		LogMatrix(fontMatrix);

		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ScaleFontX) );
		CZExpXMLAttrUtils::SetValue_Real( attr, fontMatrix.GetXScale() );
		attrList.push_back( attr );

		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ScaleFontY) );
		CZExpXMLAttrUtils::SetValue_Real( attr, fontMatrix.GetYScale() );
		attrList.push_back( attr );

		tempVal = waxRenderData->GetLeading();
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Run leading : %f ", ToFloat(tempVal));
	}
#if 0
	PMMatrix runMatrix = inWaxRun->GetTransform();

	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Run matrix ");
	LogMatrix(runMatrix);

	InterfacePtr<const IWaxGlyphs> waxGlyphs( inWaxRun, UseDefaultIID() );
	ASSERT(waxGlyphs);

	//PMRect runRect = waxGlyphs->GetStrokeBoundingBox();

	if( waxGlyphs->HasPerGlyphMatrix() || waxGlyphs->HasGlyphMatrix() )
	{
		PMPoint pt;
		PMMatrix runMatrix = waxGlyphs->GetAllGlyphsMatrix( &pt );//inWaxRun->GetTransform();
		tempVal = runMatrix.GetXScale();
		if( tempVal != 1.0 )
		{
			attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ScaleX) );
			CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
			attrList.push_back( attr );
		}
		tempVal = runMatrix.GetYScale();
		if( tempVal != 1.0 )
		{
			attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ScaleY) );
			CZExpXMLAttrUtils::SetValue_Real( attr, tempVal );
			attrList.push_back( attr );
		}
	}
#endif

	InterfacePtr<ITextModel> textModel(inWaxLine->QueryTextModel());
	this->AddWaxRunDropCap(textModel, attrList, inWaxRun);	//TODO: only if it is text run

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrTextRun, attrList, kTrue );
	StXMLPopper autoPopTextWaxRun( ioStream );

	//--- Special Glyph
#if 0
	for (int32 index = runTextStart; index < runTextEnd; ++index)
	{
		AttributeBossList bossList;
		Utils<ITextAttrUtils>()->GetTotalAttributesAtIndex(textModel, index, false, false, bossList);
		InterfacePtr<const ITextAttrSpecialGlyph> textAttrSpecialGlyph((ITextAttrSpecialGlyph*)bossList.QueryByClassID(kTextAttrSpecialGlyphBoss, ITextAttrSpecialGlyph::kDefaultIID));
		Text::GlyphID glyphId = textAttrSpecialGlyph->GetGlyphID();
		if (!glyphId)
			break;

		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Glyph id %d at index %d in waxRun", glyphId, index);

		WideString elementStrGlyphID( STRXMLTag_Glyph );

		IXMLOutStream::Attribute glyphAttr;
		IXMLOutStream::AttributeList glyphAttrList;
		
		glyphAttr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
		CZExpXMLAttrUtils::SetValue_UID( glyphAttr, glyphId );
		glyphAttrList.push_back( glyphAttr);

		glyphAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
		CZExpXMLAttrUtils::SetValue_Int( glyphAttr, index );
		glyphAttrList.push_back( glyphAttr );

		ioStream->IndentLevel(kTrue);
		ioStream->PushWithAttributes( elementStrGlyphID, glyphAttrList );
		StXMLPopper autoPopGlyph( ioStream );
	}
	//-- Special Glyph end
#else

#endif 

	this->AddWaxRunCalculatedText(ioStream, textModel, inWaxRun);
	this->AddWaxRunSpecialCharacters(ioStream, inFinalPageRef, textModel, inWaxRun);

	//Add Local overrides
	CZExpXMLExportUtils::AddStyleOverrides( ioStream, inParaAttrStrand, runTextStart, true );
	CZExpXMLExportUtils::AddStyleOverrides( ioStream, inCharAttrStrand, runTextStart, false );
	CZExpXMLExportUtils::AddAttributesNotComingInStyle( ioStream, textModel, inWaxRun );
}

//----------------------------------------------------------------------------------------
// AddWaxRunCalculatedText
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddWaxRunCalculatedText(
	IXMLOutStream *				ioStream,
	ITextModel *				inTextModel,
	const IWaxRun *				inWaxRun) const
{
	if (!(ioStream && inTextModel && inWaxRun))
	{
		ASSERT (ioStream && inTextModel && inWaxRun);
		return;
	}
	
	InterfacePtr<const IItemStrand> itemStrand((IItemStrand*)inTextModel->QueryStrand(kOwnedItemStrandBoss, IItemStrand::kDefaultIID));	
	OwnedItemDataList ownedList;
	itemStrand->CollectOwnedItems(inWaxRun->TextOrigin() , inWaxRun->GetWaxSpan(), &ownedList);
	int32 ownedItemCount = ownedList.size();
	if (!ownedItemCount)
		return;

	IDataBase* database = ::GetDataBase(inTextModel);
	//InterfacePtr<IComposeScanner> scanner(inTextModel, UseDefaultIID());
	for (int32 ownedItemIndex = 0; ownedItemIndex < ownedItemCount ; ++ownedItemIndex) 
	{
		UID inlineUID = ownedList[ownedItemIndex].fUID;
		ClassID classId = ownedList[ownedItemIndex].fClassID;
		TextIndex textIndex = ownedList[ownedItemIndex].fAt;
		
		//Note: commenting following condition will also export footnote marker,
		// which is also being exported by AddFootnoteMarker
		if (classId != kCalculatedTextVariableBoss)	//Ignore others
			continue;

		InterfacePtr<ICalculatedText> calText(database, inlineUID, ICalculatedText::kDefaultIID);
		if (!calText)
		{
			//ASSERT (calText);
			continue;
		}
		
		//IDrawingStyle *style = scanner->GetCompleteStyleAt(textIndex);	//Someday: find why we need style in this function
#ifdef InDnCS5
		ICalculatedTextResult *calTextResult = calText->NewCalculatedText(NULL);//style);
#else
		ICalculatedTextResult *calTextResult = calText->QueryCalculatedText(NULL);//style);
#endif
		if (!calTextResult)
			continue;		
		StPtrDeleter<ICalculatedTextResult> autoDelCalTextResult( calTextResult, true );

		WideString elementStrText( STRXMLTag_Text );

		IXMLOutStream::Attribute variableAttr;
		IXMLOutStream::AttributeList variableAttrList;
		
		variableAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Value ) );
		WideString variableValue(calTextResult->GetString());
		variableAttr.fValue = variableValue;
		variableAttrList.push_back( variableAttr );

		variableAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
		CZExpXMLAttrUtils::SetValue_Int( variableAttr, textIndex );
		variableAttrList.push_back( variableAttr );

		ioStream->IndentLevel(kTrue);
		ioStream->PushWithAttributes( elementStrText, variableAttrList );
		StXMLPopper autoPopVariable( ioStream );
	}
}

//----------------------------------------------------------------------------------------
// AddWaxRunDropCap
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddWaxRunDropCap(
	ITextModel *				inTextModel,
	IXMLOutStream::AttributeList & ioAttrList,
	const IWaxRun *				inWaxRun) const
{
	ASSERT( inTextModel &&  inWaxRun );

	// Drop cap functionality. Only flag is exported to XML for drop cap.
	do
	{
		InterfacePtr<IAttributeStrand> paraAttrstrand((IAttributeStrand *)(inTextModel->QueryStrand(kParaAttrStrandBoss, IID_IATTRIBUTESTRAND)));
		if (!paraAttrstrand)
			break;

		//text span
		int32 runTextStart = inWaxRun->TextOrigin();
		AttributeBossList bossList;
		Utils<ITextAttrUtils>()->GetTotalAttributesAtIndex(inTextModel, runTextStart, false, false, bossList);
		InterfacePtr<const ITextAttrInt16> textAttrChar((ITextAttrInt16*)bossList.QueryByClassID(kTextAttrDropCapCharsBoss, ITextAttrInt16::kDefaultIID));
		if (!textAttrChar)
			break;

		int16 charInDropCap = textAttrChar->Get();
		if (charInDropCap == 0)
			break;

		InterfacePtr<const ITextAttrInt16> textAttrLine((ITextAttrInt16*)bossList.QueryByClassID(kTextAttrDropCapLinesBoss, ITextAttrInt16::kDefaultIID));
		if (!textAttrLine)
			break;

		int16 lineInDropCap = textAttrLine->Get();
		if (lineInDropCap == 0)	//This should be 1?
			break;

		int32 count = 0;
		int32 paraRunBegin = 0;
		paraAttrstrand->GetStyleUID(runTextStart, &count, &paraRunBegin);
		if ((runTextStart - charInDropCap) >= paraRunBegin)
			break;

		IXMLOutStream::Attribute attr;
		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_IsDropCap) );
		CZExpXMLAttrUtils::SetValue_Bool(attr, kTrue );
		ioAttrList.push_back( attr );

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// AddWaxRunSpecialCharacters
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddWaxRunSpecialCharacters(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	ITextModel *				inTextModel,
	const IWaxRun *				inWaxRun) const
{
	if (!(ioStream && inTextModel && inWaxRun))
	{
		ASSERT (ioStream && inTextModel && inWaxRun);
		return;
	}

	TextIterator iter(inTextModel, inWaxRun->TextOrigin());
	TextIterator end(inTextModel, inWaxRun->TextOrigin() + inWaxRun->GetWaxSpan());
	while(iter != end)
	{
		const UTF32TextChar characterCode = *iter;
		int32 index = iter.Position();

		switch (characterCode.GetValue())
		{
		case kTextChar_SpecialGlyph:
			this->AddSpecialGlyph(ioStream, inTextModel, index);
			break;

		case kTextChar_PageNumber:
			this->AddPageNumber(ioStream, inFinalPageRef, inTextModel, index);
			break;

		case kTextChar_SectionName:
			this->AddSectionMarker(ioStream, inFinalPageRef, inTextModel, index);
			break;
		case kTextChar_FootnoteMarker:
			this->AddFootnoteMarker(ioStream, inTextModel, index, characterCode);
			break;
		}

		//TODO: make a list of chars which need to have a empty text value. like
		//Indent to here, break in style, inline text frame, image, table etc
		//EM space,
		++iter;
	}
}

//----------------------------------------------------------------------------------------
// AddSpecialGlyph
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddSpecialGlyph(
	IXMLOutStream *				ioStream,
	ITextModel *				inTextModel,
	const TextIndex				inIndex) const
{
	if (!(ioStream && inTextModel))
	{
		ASSERT (ioStream && inTextModel);
		return;
	}

	AttributeBossList bossList;
	Utils<ITextAttrUtils>()->GetTotalAttributesAtIndex(inTextModel, inIndex, false, false, bossList);
	
	InterfacePtr<const ITextAttrSpecialGlyph> textAttrSpecialGlyph((ITextAttrSpecialGlyph*)bossList.QueryByClassID(kTextAttrSpecialGlyphBoss, ITextAttrSpecialGlyph::kDefaultIID));
	Text::GlyphID glyphId = textAttrSpecialGlyph->GetGlyphID();
	if (!glyphId)
		return;

	WideString elementStrGlyphID( STRXMLTag_Glyph );

	IXMLOutStream::Attribute glyphAttr;
	IXMLOutStream::AttributeList glyphAttrList;
	
	glyphAttr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( glyphAttr, glyphId );
	glyphAttrList.push_back( glyphAttr);

	glyphAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
	CZExpXMLAttrUtils::SetValue_Int( glyphAttr, inIndex );
	glyphAttrList.push_back( glyphAttr );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStrGlyphID, glyphAttrList );
	StXMLPopper autoPopGlyph( ioStream );
}

//----------------------------------------------------------------------------------------
// AddPageNumber
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddPageNumber(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	ITextModel *				inTextModel,
	const TextIndex				inIndex) const
{	
	if (!(ioStream && inTextModel))
	{
		ASSERT (ioStream && inTextModel);
		return;
	}

	InterfacePtr<const ITextParcelList> textParcelList(inTextModel->QueryTextParcelList(inIndex));
	ParcelKey parcelKey = textParcelList->GetParcelContaining(inIndex);
	if (parcelKey.IsValid() == kFalse)
		return;

	InterfacePtr<IParcelList> parcelList(textParcelList, UseDefaultIID());
	if (!parcelList)
		return;

	InterfacePtr<const IParcel> parcel(parcelList->QueryParcel(parcelKey));
	if (!parcel)
		return;

	InterfacePtr<const ITextFrameColumn> textFrame(parcel->QueryFrame());
	UIDRef frameRef = ::GetUIDRef(textFrame);

	AttributeBossList bossList;
	Utils<ITextAttrUtils>()->GetTotalAttributesAtIndex(inTextModel, inIndex, false, false, bossList);
	
	InterfacePtr<const ITextAttrDataPosition> textAttr((ITextAttrDataPosition*)bossList.QueryByClassID(kTextAttrPageNumberTypeBoss, IID_ITEXTATTRDATAPOSITION));
	IDrawingStyle::PositionIndicator pos = textAttr->Get();
	if (pos == IDrawingStyle::kDataPosition_TextVariable)
		return;

	PMString pageNumber;
	UIDRef textModelPageRef = Utils<ITextUtils>()->GetPageUIDRef(frameRef);
	if (textModelPageRef == inFinalPageRef)
	{
		//Not a master page, it will give correct values for page number.
		Utils<ITextUtils>()->GetFormattedPageNumberString(frameRef, &pageNumber, pos);
	}
	else
	{
		//Master page
		//Assumption: user will not use prev & next page number in master page where story flows in more than 1 frame.
		//Where story has only one frame, prev & next page are equal to the current page number.
		IDataBase* db = ::GetDataBase(inTextModel);
		InterfacePtr<ISectionList> sectionList(db, db->GetRootUID(), IID_ISECTIONLIST);
		if (!sectionList)
			return;

		InterfacePtr<const IPageList> pageList(db, db->GetRootUID(), IID_IPAGELIST);
		if (!pageList)
			return;

		UID pageUID = inFinalPageRef.GetUID();
		int32 sectionIndex = pageList->GetSectionIndex(pageUID);
		InterfacePtr<ISection> section(sectionList->QueryNthSection(sectionIndex));
		if (!section)
			return;

		ClassID style;
		section->GetStyleInfo(&style);
		bool numberStyle = true;
		if (style == kRomanLowerPageNumberingStyleServiceBoss || style == kRomanUpperPageNumberingStyleServiceBoss 
		|| style == kLettersUpperPageNumberingStyleServiceBoss || style == kLettersLowerPageNumberingStyleServiceBoss)
			numberStyle = false;

		bool16 includeSection = section->GetPageNumberIncludeSectionPrefix();
		sectionList->GetPageString(pageUID , &pageNumber, includeSection, numberStyle, kDefaultPageType);
	}

	//if (pageNumber.IsEmpty())
	//	return;

	WideString elementStr( STRXMLTag_Text );

	IXMLOutStream::Attribute elementAttr;
	IXMLOutStream::AttributeList elementAttrList;
	
	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Value ) );
	WideString variableValue(pageNumber);
	elementAttr.fValue = variableValue;
	elementAttrList.push_back( elementAttr );

	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
	CZExpXMLAttrUtils::SetValue_Int( elementAttr, inIndex );
	elementAttrList.push_back( elementAttr );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, elementAttrList );
	StXMLPopper autoPopVariable( ioStream );
}

//----------------------------------------------------------------------------------------
// AddSectionMarker
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddSectionMarker(
	IXMLOutStream *				ioStream,
	const UIDRef &				inFinalPageRef,
	ITextModel *				inTextModel,
	const TextIndex					index) const
{
	if (!(ioStream && inTextModel))
	{
		ASSERT (ioStream && inTextModel);
		return;
	}

	IDataBase* db = ::GetDataBase(inTextModel);
	InterfacePtr<ISectionList> sectionList(db, db->GetRootUID(), IID_ISECTIONLIST);
	if (!sectionList)
		return;

	InterfacePtr<const IPageList> pageList(db, db->GetRootUID(), IID_IPAGELIST);
	if (!pageList)
		return;

	UID pageUID = inFinalPageRef.GetUID();
	int32 sectionIndex = pageList->GetSectionIndex(pageUID);
	InterfacePtr<ISection> section(sectionList->QueryNthSection(sectionIndex));
	if (!section)
		return;

	PMString sectionMarker;
	section->GetMarker(&sectionMarker);
	//if (sectionMarker.IsEmpty())
	//	return;

	WideString elementStr( STRXMLTag_Text );

	IXMLOutStream::Attribute elementAttr;
	IXMLOutStream::AttributeList elementAttrList;
	
	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Value ) );
	WideString variableValue(sectionMarker);
	elementAttr.fValue = variableValue;
	elementAttrList.push_back( elementAttr );

	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
	CZExpXMLAttrUtils::SetValue_Int( elementAttr, index );
	elementAttrList.push_back( elementAttr );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, elementAttrList );
	StXMLPopper autoPopVariable( ioStream );
}

//----------------------------------------------------------------------------------------
// AddFootnoteMarker
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::AddFootnoteMarker(
	IXMLOutStream *				ioStream,
	ITextModel *				inTextModel,
	const TextIndex				inIndex,
	const UTF32TextChar			inChar) const
{
	if (!(ioStream && inTextModel))
	{
		ASSERT (ioStream && inTextModel);
		return;
	}

#ifdef InDnCS5
	ICalculatedTextResult *calTextResult = Utils<ICompositionUtils>()->NewCalculatedText(inTextModel, inIndex, inChar);
#else
	ICalculatedTextResult *calTextResult = Utils<ICompositionUtils>()->QueryCalculatedText(inTextModel, inIndex, inChar);
#endif
	if (!calTextResult)
		return;
	StPtrDeleter<ICalculatedTextResult> autoDelResult( calTextResult, true );

	const WideString footnoteNumber = calTextResult->GetString();
	//if (footnoteNumber.empty())
	//	return;

	WideString elementStr( STRXMLTag_Text );

	IXMLOutStream::Attribute elementAttr;
	IXMLOutStream::AttributeList elementAttrList;
	
	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Value ) );
	WideString value(footnoteNumber);
	elementAttr.fValue = value;
	elementAttrList.push_back( elementAttr );

	elementAttr.fName.SetX16String( TOWSTRING( STRXMLAttr_Index ) );		
	CZExpXMLAttrUtils::SetValue_Int( elementAttr, inIndex );
	elementAttrList.push_back( elementAttr );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, elementAttrList );
	StXMLPopper autoPopVariable( ioStream );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddImageFrameContent
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::AddImageFrameContent(
	IXMLOutStream *				ioStream,
	const ISpread *				inSpread,
	const IGraphicFrameData *	inGraphicFrameData) const
{
	LogFunctionEnterExit; 

	ErrorCode toReturn = ErrorUtils::PMGetGlobalErrorCode();
	if( toReturn != kSuccess )
		return toReturn;

	if( inGraphicFrameData->HasContent() == kFalse )
		return kSuccess;

	InterfacePtr<const ISysFileData> streamFileData( ioStream, UseDefaultIID() );
	if( !streamFileData )
		return kIZExpParamError;

#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif

	InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );

	//Create streams to export data in.
	const IDFile & inFile = streamFileData->GetSysFile();
	IDFile lowResFile, highResFile;
	Utils<IZExpFileUtils> expFileUtils;
	expFileUtils->GetImageExportFileFolders( inFile, lowResFile, highResFile );

	PMString imageFileName;
	imageFileName.AppendNumber( ::GetUID( inGraphicFrameData ).Get() );
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
	//docUtils->CreatePageItemSnapshot( ::GetUIDRef( inGraphicFrameData), lowResStream, true, IShape::kDrawingClippedOutImageData, gGlobalData->GetDPILowRes() );
#ifdef InDnCS5
	if( gGlobalData->IsExportLowRes() )
		toReturn = docUtils->CreatePageItemPNGSnapshot( ::GetUIDRef( inGraphicFrameData), lowResStream, false, IShape::kDrawingClippedOutImageData, gGlobalData->GetDPILowRes() );
	if( toReturn == kSuccess )
		toReturn = docUtils->CreatePageItemPNGSnapshot( ::GetUIDRef( inGraphicFrameData), highResStream, false, IShape::kDrawingClippedOutImageData, gGlobalData->GetDPIHighRes() );
#else
	if( gGlobalData->IsExportLowRes() )
		toReturn = docUtils->CreatePageItemSnapshot( ::GetUIDRef( inGraphicFrameData), lowResStream, false, IShape::kDrawingClippedOutImageData, gGlobalData->GetDPILowRes() );
	if( toReturn == kSuccess )
		toReturn = docUtils->CreatePageItemSnapshot( ::GetUIDRef( inGraphicFrameData), highResStream, false, IShape::kDrawingClippedOutImageData, gGlobalData->GetDPIHighRes() );
#endif
	//imageFileName = "";
	//imageFileName.AppendNumber( ::GetUID( inGraphicFrameData ).Get() );
	//imageFileName.Append( ".tiff");//gif" );

	//expFileUtils->GetImageExportFileFolders( inFile, lowResFile, highResFile );
	//FileUtils::AppendPath( &lowResFile, imageFileName );
	//FileUtils::AppendPath( &highResFile, imageFileName );

	//InterfacePtr<IPMStream> lowResGIFStream;//( expFileUtils->CreateWriteStreamForImage( lowResFile ) );
	//InterfacePtr<IPMStream> highResGIFStream( expFileUtils->CreateWriteStreamForImage( highResFile ) );

	//if( gGlobalData->IsExportLowRes() )
	//	lowResGIFStream.reset( expFileUtils->CreateWriteStreamForImage( lowResFile ) );

	//StStreamCloser autoCloseGIFLowRes( lowResGIFStream );
	//StStreamCloser autoCloseGIFHighRes( highResGIFStream );

	////docUtils->CreatePageItemGIFSnapshot( ::GetUIDRef( inGraphicFrameData), lowResStream, true, IShape::kDrawingClippedOutImageData );
	////docUtils->CreatePageItemGIFSnapshot( ::GetUIDRef( inGraphicFrameData), highResStream, false, IShape::kDrawingClippedOutImageData, 300 );
	//docUtils->CreatePageItemTIFFSnapshot( ::GetUIDRef( inGraphicFrameData), lowResStream, true, IShape::kDrawingClippedOutImageData );
	//docUtils->CreatePageItemTIFFSnapshot( ::GetUIDRef( inGraphicFrameData), highResStream, false, IShape::kDrawingClippedOutImageData, 300 );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreatePagePreview
//----------------------------------------------------------------------------------------
ErrorCode
CZExpXMLWriteFacade::CreatePagePreview(
	IXMLOutStream *				ioStream,
	const UIDRef &				inPageRef) const
{
	LogFunctionEnterExit; 
	if( inPageRef == UIDRef::gNull )
	{
		ASSERT (inPageRef);
		return kIZExpParamError;
	}

	InterfacePtr<const ISysFileData> streamFileData( ioStream, UseDefaultIID() );
	if( !streamFileData )
		return kIZExpParamError;

	ErrorCode toReturn = kSuccess;

#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif

	InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );

	if( gGlobalData->IsExportPagePreview() == false )
		return toReturn;

	//Create streams to export data in.
	const IDFile & inFile = streamFileData->GetSysFile();
	IDFile lowResFile, highResFile;
	Utils<IZExpFileUtils> expFileUtils;
	expFileUtils->GetImageExportFileFolders( inFile, lowResFile, highResFile );

	PMString imageFileName;
	imageFileName.AppendNumber( inPageRef.GetUID().Get() );
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
	//docUtils->CreatePageItemSnapshot( inPageRef, lowResStream, true, IShape::kPrinting, gGlobalData->GetDPILowRes() );
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

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// LogMatrix
//----------------------------------------------------------------------------------------
void
CZExpXMLWriteFacade::LogMatrix(
	const PMMatrix &			inMatrix) const
{
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "XScale %f", ToFloat(inMatrix.GetXScale()));
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "YScale %f", ToFloat(inMatrix.GetYScale()));
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "XTrans %f", ToFloat(inMatrix.GetXTranslation()));
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "YTrans %f", ToFloat(inMatrix.GetYTranslation()));
}



