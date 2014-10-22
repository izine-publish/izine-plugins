//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLExport_Colors.cpp $
//	$Revision: 3612 $
//	$Date: 2011-08-29 13:38:02 +0200 (Mon, 29 Aug 2011) $
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
#include "IColorData.h"
#include "IDocument.h"
#include "IRenderingObject.h"
#include "ISwatchList.h"
#include "IUIColorUtils.h"
#include "IWorkspace.h"
#include "IXMLOutStream.h"

//SDK General includes
#include "AttributeBossList.h"
#include "TextAttrID.h"
#include "TTImportExportAttrId.h"

//OS / ThirdParty includes

//#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "StInDnXmlUtils.h"
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Colors.h"
#include "ZExpXMLConstants.h"
#include "ZExpStyleAttrConstants.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
#define CZExpXMLExport_Colors CZExpXMLExportUtils

#pragma mark -
namespace CZExpXMLExport_Colors
{
	void				AddColorInfo(
							IXMLOutStream *				ioStream,
							const IColorData *			inColorInfo,
							IUIColorUtils *				inColorUtils);

	void				ConvertToRGBHex(
							const ColorArray &			inColorComponents,
							char *						oColorStr );
	void				ConvertToCMYKHex(
							const ColorArray &			inColorComponents,
							char *						oColorStr );

	void				ConvertCMYKToRGBHex(
							const ColorArray &			inColorComponents,
							char *						oColorStr );
}
//----------------------------------------------------------------------------------------
// AddAllSwatches
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::AddAllSwatches( //Color names
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc)
{
	LogFunctionEnterExit;
	if( !inDoc || !ioStream )
		return;

	InterfacePtr<const IWorkspace> docWorkSpace( const_cast<IDocument*>(inDoc)->GetDocWorkSpace(), UseDefaultIID() );
	ASSERT( docWorkSpace );

	InterfacePtr<const ISwatchList> docSwatchList( docWorkSpace, UseDefaultIID() );
	ASSERT( docSwatchList );

	WideString elementStr( STRXMLTag_Colors );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStr, kTrue );
	StXMLPopper autoPopParaStyles( ioStream );

	int32 colorCount = docSwatchList->GetNumSwatches();
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Color Swatch Count: %d", colorCount );

	Utils<IUIColorUtils> colorUtils;
	for( int i = 0; i < colorCount ; ++i )
	{
		UIDRef swatchRef = docSwatchList->GetNthSwatch(i);
		
		InterfacePtr<const IColorData> swatchColor( swatchRef, UseDefaultIID() );
		if( !swatchColor )
			continue;
		ClassID theClassID = ::GetClass( swatchColor );

		AddColorInfo( ioStream, swatchColor, colorUtils);
	}
}

//----------------------------------------------------------------------------------------
// AddColorInfo
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::AddColorInfo(
	IXMLOutStream *				ioStream,
	const IColorData *			inColorInfo,
	IUIColorUtils *				inColorUtils)
{
	LogFunctionEnterExit;

	WideString elementStr( STRXMLTag_ColorInfo );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	InterfacePtr<const IRenderingObject> renderingObject( inColorInfo, UseDefaultIID() );
	ASSERT( renderingObject );

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, ::GetUID(inColorInfo) );
	attrList.push_back( attr );

	const PMString & swatchName = renderingObject->GetSwatchName();
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	attr.fValue = WideString( swatchName );

	attrList.push_back( attr );

	int32 colorSpace = inColorInfo->GetColorSpace();
	
	ColorArray rgbValues = inColorInfo->GetColorData();//inColorUtils->GetRGBColorValue( ::GetDataBase( inColorInfo), ::GetUID( inColorInfo ) );

	int32 colorCmpCount = rgbValues.size();
	char colorStr[12] = {0};
	if( colorSpace == kPMCsCalRGB || colorSpace == kPMCsLab )
		ConvertToRGBHex( rgbValues, colorStr );
	else if( colorSpace == kPMCsCalCMYK )
	{
		colorSpace = kPMCsCalRGB;
		ConvertCMYKToRGBHex( rgbValues, colorStr );//ConvertToCMYKHex( rgbValues, colorStr );
	}

	attr.fName.SetX16String( TOWSTRING("colorSpace") );
	if( colorSpace == kPMCsCalRGB )
		attr.fValue.SetX16String( TOWSTRING("RGB") );
	else if( colorSpace == kPMCsCalCMYK )
		attr.fValue.SetX16String( TOWSTRING("CMYK") );
	else if( colorSpace == kPMCsLab )
		attr.fValue.SetX16String( TOWSTRING("LAB") );
	else if( colorSpace == kPMCsDeviceN )
		attr.fValue.SetX16String( TOWSTRING("DeviceN") );
	else
		attr.fValue.SetX16String( TOWSTRING("Unknown") );

	//CZExpXMLAttrUtils::SetValue_Int( attr, colorSpace );
	attrList.push_back( attr );
	
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Color Swatch Name: %s, color space :%d, colorValue :%s", swatchName.GrabCString(), colorSpace, colorStr );
	if( colorStr[0] )
	{
		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Value) );
		attr.fValue = WideString( colorStr );
		attrList.push_back( attr );
	}
	
	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopParaStyle( ioStream );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ConvertToRGBHex
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::ConvertToRGBHex(
	const float					inR,
	const float					inG,
	const float					inB,
	char *						oColorStr)
{
	unsigned char rInt = ToInt16(inR * 255);
	unsigned char gInt = ToInt16(inG * 255);
	unsigned char bInt = ToInt16(inB * 255);

	sprintf( oColorStr, "#%02x%02x%02x", rInt, gInt, bInt );
}

//----------------------------------------------------------------------------------------
// ConvertToRGBHex
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::ConvertToRGBHex(
	const ColorArray &			inColorComponents,
	char *						oColorStr)
{
	ASSERT( inColorComponents.size() == 3 );

	const PMReal & rValue = inColorComponents[0];
	const PMReal & gValue = inColorComponents[1];
	const PMReal & bValue = inColorComponents[2];

	unsigned char rInt = ToInt16(rValue * 255);
	unsigned char gInt = ToInt16(gValue * 255);
	unsigned char bInt = ToInt16(bValue * 255);

	sprintf( oColorStr, "#%02x%02x%02x", rInt, gInt, bInt );
}

//----------------------------------------------------------------------------------------
// ConvertToRGBHex
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::ConvertToCMYKHex(
	const ColorArray &			inColorComponents,
	char *						oColorStr)
{
	ASSERT( inColorComponents.size() == 4 );

	if( inColorComponents.size() == 4 )
	{
		const PMReal & cValue = inColorComponents[0];
		const PMReal & mValue = inColorComponents[1];
		const PMReal & yValue = inColorComponents[2];
		const PMReal & kValue = inColorComponents[3];

		unsigned char cInt = ToInt16(cValue * 100);
		unsigned char mInt = ToInt16(mValue * 100);
		unsigned char yInt = ToInt16(yValue * 100);
		unsigned char kInt = ToInt16(kValue * 100);

		sprintf( oColorStr, "#%02x%02x%02x%02x", cInt, mInt, yInt, kInt );
	}
}

//----------------------------------------------------------------------------------------
// ConvertCMYKToRGBHex
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::ConvertCMYKToRGBHex(
	const float					inC,
	const float					inM,
	const float					inY,
	const float					inK,
	char *						oColorStr)
{
	float colorR = ToFloat((1 - (inC * (1 - inK) + inK)) * 255);
	float colorG = ToFloat((1 - (inM * (1 - inK) + inK)) * 255);
	float colorB = ToFloat((1 - (inY * (1 - inK) + inK)) * 255);

	unsigned char rInt = colorR;
	unsigned char gInt = colorG;
	unsigned char bInt = colorB;

	sprintf( oColorStr, "#%02x%02x%02x", rInt, gInt, bInt );
}

//----------------------------------------------------------------------------------------
// ConvertCMYKToRGBHex
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Colors::ConvertCMYKToRGBHex(
	const ColorArray &			inColorComponents,
	char *						oColorStr)
{
	ASSERT( inColorComponents.size() == 4 );

	if( inColorComponents.size() == 4 )
	{
		const PMReal & colorC = inColorComponents[0];
		const PMReal & colorM = inColorComponents[1];
		const PMReal & colorY = inColorComponents[2];
		const PMReal & colorK = inColorComponents[3];

		float colorR = ToFloat((1 - (colorC * (1 - colorK) + colorK)) * 255);
		float colorG = ToFloat((1 - (colorM * (1 - colorK) + colorK)) * 255);
		float colorB = ToFloat((1 - (colorY * (1 - colorK) + colorK)) * 255);

		unsigned char rInt = colorR;
		unsigned char gInt = colorG;
		unsigned char bInt = colorB;

		sprintf( oColorStr, "#%02x%02x%02x", rInt, gInt, bInt );
	}
}
