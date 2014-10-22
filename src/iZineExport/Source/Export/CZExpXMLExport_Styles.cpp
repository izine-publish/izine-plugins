//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLExport_Styles.cpp $
//	$Revision: 3710 $
//	$Date: 2011-12-21 08:48:28 +0100 (Wed, 21 Dec 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAttributeStrand.h"
#include "IAttrImportExport.h"
#include "IAttrReport.h"
#include "IDocument.h"
#include "IGraphicAttributeUtils.h"
#include "IStyleGroupHierarchy.h"
#include "IStyleGroupManager.h"
#include "IStyleInfo.h"
#include "ITextAttributes.h"
#include "ITextAttrRealNumber.h"
#include "IWaxRun.h"
#include "IWorkspace.h"
#include "IXMLOutStream.h"

//SDK General includes
#include "AttributeBossList.h"
#include "TextAttributeRunIterator.h"
#include "TextAttrID.h"
#include "TextRange.h"
#include "TTImportExportAttrId.h"

//OS / ThirdParty includes

//#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Colors.h"
#include "CZExpXMLExport_Styles.h"
#include "StInDnXmlUtils.h"
#include "StUtils.h"
#include "ZExpStyleAttrConstants.h"
#include "ZExpXMLConstants.h"

#pragma mark -

#define CZExpXMLExport_Styles CZExpXMLExportUtils

void	HookStyleAttrValue_Color( const IAttrReport * inAttrReport, PMString & ioValue );

//Keep values in sync with enStyleAttr
const StyleAttrPair gStyleAttrs[] = {
	{ L"FontName",			kTextAttrFontUIDBoss, nil},
	{ L"FontStyle",			kTextAttrFontStyleBoss, nil},
	{ L"FontSize",			kTextAttrPointSizeBoss, nil},
	{ L"TextColor",			kTextAttrColorBoss, HookStyleAttrValue_Color},
	{ L"UnderLine",			kTextAttrUnderlineBoss, nil},
	{ L"Strikethrough",		kTextAttrStrikethruBoss, nil},
	{ L"Case",				kTextAttrCapitalModeBoss, nil},
	{ L"HScale",			kTextAttrXGlyphScaleBoss, nil},
	{ L"VScale",			kTextAttrYGlyphScaleBoss, nil},
	{ L"TextAlign",			kTextAttrAlignmentBoss, nil},
	{ L"Position",			kTextAttrPositionModeBoss, nil},
	{ L"Tracking",			kTextAttrTrackKernBoss, nil},
	{ L"KernMethod",		kTextAttrPairKernMethodBoss, nil},
	{ L"Kern",				kTextAttrPairKernBoss, nil},
	{ L"Skew",				kTextAttrSkewAngleBoss, nil}
};

const StyleAttrPair gNonStyleTextRunAttrs[] = {
	{ L"Kerning",				kTextAttrPairKernBoss, nil}
};

/** Sample values for the attributes value
(0) + color: [Black]~ Boss kTextAttrColorBoss 
(1) + Regular~ Boss kTextAttrFontStyleBoss 
(2) + size: 12 pt~ Boss kTextAttrPointSizeBoss 
(3) + char width: 100%~ Boss kTextAttrXGlyphScaleBoss 
(17) + char height: 100%~ Boss kTextAttrYGlyphScaleBoss 
(34) - underline~ Boss kTextAttrUnderlineBoss
(35) + Times New Roman~ Boss kTextAttrFontUIDBoss 
(67) + skew angle: 0°~ Boss kTextAttrSkewAngleBoss
*/

/** Expected bosses to be exported
(57) + stroke tint: 100%~ Boss kTextAttrStrokeTintBoss 
(58) + tint: 100%~ Boss kTextAttrTintBoss 
(207) + Character Rotation:  0°~ Boss kTACharRotateAngleReportBoss
*/


namespace CZExpXMLExport_Styles
{

	void				AddStyles(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc,
							PMIID						inGrpMgrIID = IID_IPARASTYLEGROUPMANAGER,
							bool						inParaStyle = true );

	void				AddStyles(
							IXMLOutStream *				ioStream,
							const IStyleGroupHierarchy* inStyleGrpHierarchy,
							bool						inParaStyle = true);

	void				AddParaStyle(
							IXMLOutStream *				ioStream,
							const IStyleInfo *			inStyleInfo,
							const IStyleGroupHierarchy *	inStyleGrpHierarchy);

	void				AddCharStyle(
							IXMLOutStream *				ioStream,
							const IStyleInfo *			inStyleInfo,
							const IStyleGroupHierarchy *	inStyleGrpHierarchy);

	void				AddStyleInfo(
							IXMLOutStream::AttributeList& ioAttrList,
							const IStyleInfo *			inStyleInfo,
							const IStyleGroupHierarchy *	inStyleGrpHierarchy);

	void				AddTextAttributes(
							IXMLOutStream *				ioStream,
							const ITextAttributes *		inTextAttributes,
							const StyleAttrPair *		inAttrsToExport,
							int							inAttrsCount );

	void				AddGraphicStyle_FillColor(
							IXMLOutStream *				ioStream,
							const IGraphicStyleDescriptor *	inGStyleDesc );
}


//----------------------------------------------------------------------------------------
// AddPragraphStyles
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddPragraphStyles(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc)
{
	ASSERT( ioStream && inDoc );

	WideString elementStr( STRXMLTag_ParaStyles );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStr, kTrue );
	StXMLPopper autoPopParaStyles( ioStream );

	AddStyles( ioStream, inDoc, IID_IPARASTYLEGROUPMANAGER, true );
}

//----------------------------------------------------------------------------------------
// AddCharaStyles
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddCharaStyles(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc)
{
	ASSERT( ioStream && inDoc );

	WideString elementStr( STRXMLTag_CharStyles );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStr, kTrue );
	StXMLPopper autoPopParaStyles( ioStream );

	AddStyles( ioStream, inDoc, IID_ICHARSTYLEGROUPMANAGER, false );
}

//----------------------------------------------------------------------------------------
// AddStyles
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddStyles(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc,
	PMIID						inGrpMgrIID,
	bool						inParaStyle)
{
	InterfacePtr<const IWorkspace> docWorkSpace( const_cast<IDocument*>(inDoc)->GetDocWorkSpace(), UseDefaultIID() );
	ASSERT( docWorkSpace );

	InterfacePtr<const IStyleGroupManager> styleGrpMgr( docWorkSpace, inGrpMgrIID );
	ASSERT( styleGrpMgr );
	if( !styleGrpMgr )
		return;

	InterfacePtr<const IStyleGroupHierarchy> styleGrpHierarchy( styleGrpMgr->GetRootHierarchy(), UseDefaultIID() );
	ASSERT(styleGrpHierarchy);

	AddStyles( ioStream, styleGrpHierarchy, inParaStyle );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddStyles
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddStyles(
	IXMLOutStream *					ioStream,
	const IStyleGroupHierarchy *	inStyleGrpHierarchy,
	bool							inParaStyle )
{
	if( inStyleGrpHierarchy->GetHierarchyType() == IStyleGroupHierarchy::kHierarchyTypeStyle )
	{
		InterfacePtr<const IStyleInfo> styleInfo( inStyleGrpHierarchy, UseDefaultIID() );
		ASSERT( styleInfo );
		
		if( inParaStyle )
			AddParaStyle( ioStream, styleInfo, inStyleGrpHierarchy );
		else
			AddCharStyle( ioStream, styleInfo, inStyleGrpHierarchy );
	}
	else
	{
		int32 childCount = inStyleGrpHierarchy->GetChildCount();
		for( int i = 0; i < childCount ; ++i )
		{
			InterfacePtr<const IStyleGroupHierarchy> childHier( inStyleGrpHierarchy->QueryChild(i) );
			ASSERT( childHier );
			AddStyles( ioStream, childHier, inParaStyle );
		}
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddParaStyle
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddParaStyle(
	IXMLOutStream *					ioStream,
	const IStyleInfo *				inStyleInfo,
	const IStyleGroupHierarchy *	inStyleGrpHierarchy)
{
	ASSERT( ioStream && inStyleInfo );

	WideString elementStr( STRXMLTag_Style );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;
	AddStyleInfo( attrList, inStyleInfo, inStyleGrpHierarchy );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopParaStyle( ioStream );
	
	InterfacePtr<const ITextAttributes> styleAttributes( inStyleInfo, UseDefaultIID() );
	ASSERT( styleAttributes );

	AddTextAttributes( ioStream, styleAttributes, gStyleAttrs, sizeof(gStyleAttrs)/sizeof(StyleAttrPair) );
}

//----------------------------------------------------------------------------------------
// AddCharStyle
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddCharStyle(
	IXMLOutStream *					ioStream,
	const IStyleInfo *				inStyleInfo,
	const IStyleGroupHierarchy *	inStyleGrpHierarchy)
{
	ASSERT( ioStream && inStyleInfo );

	WideString elementStr( STRXMLTag_Style );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;
	AddStyleInfo( attrList, inStyleInfo, inStyleGrpHierarchy );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopCharStyle( ioStream );
	
	InterfacePtr<const ITextAttributes> styleAttributes( inStyleInfo, UseDefaultIID() );
	ASSERT( styleAttributes );

	AddTextAttributes( ioStream, styleAttributes, gStyleAttrs, sizeof(gStyleAttrs)/sizeof(StyleAttrPair) );
}


//----------------------------------------------------------------------------------------
// AddStyleInfo
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddStyleInfo(
	IXMLOutStream::AttributeList&	ioAttrList,
	const IStyleInfo *				inStyleInfo,
	const IStyleGroupHierarchy *	inStyleGrpHierarchy)
{
	IXMLOutStream::Attribute attr;

	//ID
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, ::GetUID(inStyleInfo) );
	ioAttrList.push_back( attr );

	//Name
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	const PMString & styleName = inStyleInfo->GetName();
	attr.fValue = WideString( styleName );
	ioAttrList.push_back( attr );

	//ID parent
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ParentStyleID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, inStyleInfo->GetBasedOn() );
	ioAttrList.push_back( attr );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddTextAttributes
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddTextAttributes(
	IXMLOutStream *				ioStream,
	const ITextAttributes *		inTextAttributes,
	const StyleAttrPair *		inAttrsToExport,
	int							inAttrsCount)
{
	PMString tagValue;
	IXMLOutStream::Attribute attr;
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	WideString elementStr( STRXMLTag_StyleAttribute );

	IDataBase * db = ::GetDataBase( inTextAttributes );

	for( int i = 0; i < inAttrsCount ; ++i )
	{
		const StyleAttrPair & currAttrExportInfo = inAttrsToExport[i];
		const ClassID & theAttrClass = currAttrExportInfo.mClassID;
		
		InterfacePtr<const IAttrReport> textAttr( (const IAttrReport*)inTextAttributes->QueryByClassID( theAttrClass, IAttrReport::kDefaultIID) );
		if( !textAttr )
			continue;
		InterfacePtr<const IAttrImportExport> textAttrExport( textAttr, IID_IATTRIMPORTEXPORT );
		if( textAttrExport )
		{
			textAttrExport->GetTagValue( &tagValue, db );
			if( currAttrExportInfo.mValueHook )
				currAttrExportInfo.mValueHook( textAttr, tagValue );
	
			IXMLOutStream::AttributeList attrList;
			
			attr.fValue = WideString( currAttrExportInfo.mAttrName );
			attrList.push_back( attr );

			ioStream->IndentLevel(kTrue);
			ioStream->PushWithAttributes( elementStr, attrList );
			StXMLIndentDisabler disableIndent( ioStream );	//Disable indent as story content is written in data
			StXMLPopper autoPopStyleAttr( ioStream );
			ioStream->Write( WideString(tagValue) );
		}
	}
}

//----------------------------------------------------------------------------------------
// AddStyleOverrides
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddStyleOverrides(
	IXMLOutStream *				ioStream,
	IAttributeStrand *			inAttrStrand,
	TextIndex					inTextIndex,
	bool						inIsParaStyle)
{

	const DataWrapper<AttributeBossList> charListOverrides =  inAttrStrand->GetLocalOverrides(inTextIndex, nil);
	const AttributeBossList * attrBossList = charListOverrides.get();
	if( !attrBossList || attrBossList->CountBosses() <= 0 )
		return;


	WideString elementStrStyles( STRXMLTag_StyleOverrideChar );
	if( inIsParaStyle )
		elementStrStyles = WideString(STRXMLTag_StyleOverridePara);
	
	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStrStyles, kTrue );
	StXMLPopper autoPopStyles( ioStream );


	const StyleAttrPair *		inAttrsToExport = gStyleAttrs;
	int							inAttrsCount = sizeof(gStyleAttrs)/sizeof(StyleAttrPair);
	
	PMString tagValue;
	IXMLOutStream::Attribute attr;
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	WideString elementStr( STRXMLTag_StyleAttribute );

	IDataBase * db = ::GetDataBase( inAttrStrand );

	for( int i = 0; i < inAttrsCount ; ++i )
	{
		const StyleAttrPair & currAttrExportInfo = inAttrsToExport[i];
		const ClassID & theAttrClass = currAttrExportInfo.mClassID;
		
		InterfacePtr<const IAttrReport> textAttr( (const IAttrReport*)attrBossList->QueryByClassID( theAttrClass, IAttrReport::kDefaultIID) );
		if( !textAttr )
			continue;
		InterfacePtr<const IAttrImportExport> textAttrExport( textAttr, IID_IATTRIMPORTEXPORT );
		if( textAttrExport )
		{
			textAttrExport->GetTagValue( &tagValue, db );
			if( currAttrExportInfo.mValueHook )
				currAttrExportInfo.mValueHook( textAttr, tagValue );
	
			IXMLOutStream::AttributeList attrList;
			
			attr.fValue = WideString( currAttrExportInfo.mAttrName );
			attrList.push_back( attr );

			ioStream->IndentLevel(kTrue);
			ioStream->PushWithAttributes( elementStr, attrList );
			StXMLIndentDisabler disableIndent( ioStream );	//Disable indent as story content is written in data
			StXMLPopper autoPopStyleAttr( ioStream );
			ioStream->Write( WideString(tagValue) );
		}
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HookStyleAttrValue_Color
//----------------------------------------------------------------------------------------
void
HookStyleAttrValue_Color(
	const IAttrReport * inAttrReport, PMString & ioValue)
{
	int inStrLen = ioValue.CharCount();
	do
	{
		const char * toCmp = "COLOR:RGB:Process:";
		float f1 = 0, f2 = 0, f3 = 0, f4 = 0;
		StPtrDeleter<PMString> autoDel;
		char colorStr[12];
		if( inStrLen > strlen(toCmp) && ioValue.IndexOfString( toCmp ) == 0 )
		{
			autoDel.Set( ioValue.Substring(strlen(toCmp) ), true );

			if( autoDel.Get() )
				sscanf(autoDel.Get()->GrabCString(), "%f,%f,%f", &f1, &f2, &f3 );

			CZExpXMLExportUtils::ConvertToRGBHex( f1, f2, f3, colorStr );
			ioValue = PMString( colorStr );
			break;
		}

		toCmp = "COLOR:CMYK:Process:";
		if( inStrLen > strlen(toCmp) && ioValue.IndexOfString( toCmp ) == 0 )
		{
			autoDel.Set( ioValue.Substring(strlen(toCmp) ), true );
			if( autoDel.Get() )
				sscanf(autoDel.Get()->GrabCString(), "%f,%f,%f,%f", &f1, &f2, &f3, &f4 );

			CZExpXMLExportUtils::ConvertCMYKToRGBHex( f1, f2, f3, f4, colorStr );
			ioValue = PMString( colorStr );
			break;
		}

		toCmp = "COLOR:LAB:Process:";
		if( inStrLen > strlen(toCmp) && ioValue.IndexOfString( toCmp ) == 0 )
		{
			autoDel.Set( ioValue.Substring(strlen(toCmp) ), true );
			//TODO: convert LAB to RGB
			if( autoDel.Get() )
				sscanf(autoDel.Get()->GrabCString(), "%f,%f,%f", &f1, &f2, &f3 );
			break;
		}
		
	}while( false );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddGraphicStyles
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddGraphicStyles(
	IXMLOutStream *					ioStream,
	const IGraphicStyleDescriptor *	inGStyleDesc)
{
	ASSERT( ioStream && inGStyleDesc );

	WideString elementStr( STRXMLTag_GraphicStyle );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStr, kTrue );
	StXMLPopper autoPopGraphicStyle( ioStream );

	// fill colour.
	CZExpXMLExport_Styles::AddGraphicStyle_FillColor( ioStream, inGStyleDesc );
}

//----------------------------------------------------------------------------------------
// AddGraphicStyle_FillColor
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddGraphicStyle_FillColor(
	IXMLOutStream *				ioStream,
	const IGraphicStyleDescriptor *	inGStyleDesc)
{
	Utils<IGraphicAttributeUtils>	grpAttUtils;

	PMString tagValue;
	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	WideString elementAttrStr( STRXMLTag_StyleAttribute );

	// Inspect fill colour.
	UID fillRenderingUID = kInvalidUID;
	if (grpAttUtils->GetFillRenderingUID(fillRenderingUID, (IGraphicStyleDescriptor *)inGStyleDesc) == kSuccess)
	{
		attr.fValue = WideString( "FillColorID" );
		attrList.push_back( attr );

		tagValue = "";//"ColorID_";
		//tagValue.AppendNumber( fillRenderingUID.Get() );
		
		ioStream->IndentLevel(kTrue);
		ioStream->PushWithAttributes( elementAttrStr, attrList );
		StXMLIndentDisabler disableIndent( ioStream );	//Disable indent as story content is written in data
		StXMLPopper autoPopStyleAttr( ioStream );

		tagValue.AppendNumber( fillRenderingUID.Get() );
		ioStream->Write( WideString(tagValue) );

		//PMString swatchName = Utils<ISwatchUtils>()->GetSwatchName (db, fillRenderingUID);
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddAttributesNotComingInStyle
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Styles::AddAttributesNotComingInStyle(
	IXMLOutStream *				ioStream,
	const ITextModel *			inTextModel,
	const IWaxRun *				inWaxRun)
{
	do
	{
		InterfacePtr<ITextModel> textModel(inTextModel , UseDefaultIID() );
		InDesign::TextRange range(textModel, inWaxRun->TextOrigin() , inWaxRun->GetWaxSpan());

		if (range.IsValid() == kFalse)
			break;

		K2Vector<InDesign::TextRange> textRanges;
		textRanges.push_back(range);

		const StyleAttrPair *		inAttrsToExport = gNonStyleTextRunAttrs;
		int							inAttrsCount = sizeof(gNonStyleTextRunAttrs)/sizeof(StyleAttrPair);

		PMString tagValue;
		IDataBase * db = ::GetDataBase( inTextModel );
		for( int i = 0; i < inAttrsCount ; ++i )
		{
			const StyleAttrPair & currAttrExportInfo = inAttrsToExport[i];
			const ClassID & theAttrClass = currAttrExportInfo.mClassID;

			K2Vector<ClassID> attributeClasses;
			attributeClasses.push_back(theAttrClass);

			TextAttributeRunIterator runIter(textRanges.begin(), textRanges.end(), attributeClasses.begin(), attributeClasses.end());

			while (runIter)
			{
				InterfacePtr<const IAttrReport> textAttrReport( (const IAttrReport*)runIter->QueryBossN( 0, IAttrReport::kDefaultIID) );
				if( !textAttrReport )
				{
					++runIter;
					continue;
				}

				InterfacePtr<const IAttrImportExport> textAttrExport( textAttrReport, IID_IATTRIMPORTEXPORT );
				if( textAttrExport )
				{
					textAttrExport->GetTagValue( &tagValue, db );
					if( currAttrExportInfo.mValueHook )
						currAttrExportInfo.mValueHook( textAttrReport, tagValue );

					WideString elementStrTextCol( currAttrExportInfo.mAttrName );

					IXMLOutStream::Attribute attr;
					IXMLOutStream::AttributeList attrList;

					const RangeData runRange = runIter.GetRunRange();

					//text attribute range
					attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexStart) );
					CZExpXMLAttrUtils::SetValue_Int( attr, runRange.Start(nil) );
					attrList.push_back( attr );

					int32 textIndexEnd = runRange.End();
					attr.fName.SetX16String( TOWSTRING(STRXMLAttr_TextIndexEnd) );
					CZExpXMLAttrUtils::SetValue_Int( attr, textIndexEnd );
					attrList.push_back( attr );

					attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Value) );
					attr.fValue = WideString( tagValue );
					attrList.push_back( attr );

					ioStream->IndentLevel(kTrue);
					ioStream->PushWithAttributes( elementStrTextCol, attrList, kTrue );
					StXMLPopper autoPopTextColumn( ioStream );
				}
				++runIter;
			}
		}
	}while(kFalse);
}
