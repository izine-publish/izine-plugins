//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpXMLAttrUtils.cpp $
//	$Revision: 3224 $
//	$Date: 2011-06-14 09:02:17 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "PMRect.h"

//SDK General includes

//OS / ThirdParty includes

//#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpXMLAttrUtils.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// SetValue_UID
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::SetValue_UID(
	IXMLOutStream::Attribute &	ioAttr,
	const UID &					inUID)
{
	PMString tempStr;
	tempStr.AppendNumber( inUID.Get() );
	ioAttr.fValue = WideString(tempStr);
}

//----------------------------------------------------------------------------------------
// SetValue_Bool
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::SetValue_Bool(
	IXMLOutStream::Attribute &	ioAttr,
	const bool					inValue)
{
	if( inValue )
		ioAttr.fValue.SetX16String( L"true" );
	else
		ioAttr.fValue.SetX16String( L"false" );
}

//----------------------------------------------------------------------------------------
// SetValue_Bool
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::SetValue_Bool(
	IXMLOutStream::Attribute &	ioAttr,
	const bool16				inValue)
{
	if( inValue == kTrue )
		ioAttr.fValue.SetX16String( L"true" );
	else
		ioAttr.fValue.SetX16String( L"false" );
}

//----------------------------------------------------------------------------------------
// SetValue_Int
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::SetValue_Int(
	IXMLOutStream::Attribute &	ioAttr,
	int							inValue)
{
	PMString tempStr;
	tempStr.AppendNumber( inValue );
	ioAttr.fValue = WideString(tempStr);
}

//----------------------------------------------------------------------------------------
// SetValue_Real
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::SetValue_Real(
	IXMLOutStream::Attribute &	ioAttr,
	const PMReal &				inValue)
{
	PMString tempStr;
	tempStr.AppendNumber( inValue );
	ioAttr.fValue = WideString(tempStr);
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddRect
//----------------------------------------------------------------------------------------
void
CZExpXMLAttrUtils::AddRect(
	IXMLOutStream::AttributeList & ioAttrList,
	const PMRect &				inValue)
{
	IXMLOutStream::Attribute attr;
	attr.fName.SetX16String( L"left" );
	CZExpXMLAttrUtils::SetValue_Real( attr, inValue.Left() );
	ioAttrList.push_back( attr );

	attr.fName.SetX16String( L"top" );
	CZExpXMLAttrUtils::SetValue_Real( attr, inValue.Top() );
	ioAttrList.push_back( attr );

	attr.fName.SetX16String( L"right" );
	CZExpXMLAttrUtils::SetValue_Real( attr, inValue.Right() );
	ioAttrList.push_back( attr );

	attr.fName.SetX16String( L"bottom" );
	CZExpXMLAttrUtils::SetValue_Real( attr, inValue.Bottom() );
	ioAttrList.push_back( attr );
}


