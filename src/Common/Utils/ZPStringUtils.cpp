//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPStringUtils.cpp $
//	$Revision: 3593 $
//	$Date: 2011-08-17 08:49:23 +0200 (Wed, 17 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam 
//	Created: 09-03-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces


//SDK General includes
#include "stringutils.h"
#include "WideString.h"

//OS / ThirdParty includes

//IZP Interfaces

//IZP General includes
#include "ZPStringUtils.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// ReplaceControlChars
//----------------------------------------------------------------------------------------
void
ZPStringUtils::ReplaceControlChars(
	WideString &								ioText)
{
	WideString::iterator_raw iter = ioText.begin_raw();
	WideString::iterator_raw iterEnd = ioText.end_raw();
	while (iter != iterEnd)
	{
		const UTF16TextChar & currChar = *iter;
		if( currChar <= 31 && currChar > 1 
			&& currChar != 0x9 && currChar != 0xA && currChar != 0xD )
		{
			*iter = 168;	//'¿'
		}
		++iter;
	}
}

//----------------------------------------------------------------------------------------
// EscapeXMLChars
//----------------------------------------------------------------------------------------
void
ZPStringUtils::EscapeXMLChars(
	WideString *								inText,
	WideString *								oText)
{
#ifndef InDnCS5
	*oText = WideString();
	if (!inText)
		return;

	if (inText->Length() == 0)
		return;

	EscapePair <char, char>escapePairs[] = {
											{'&', "&amp;"},
											{'<', "&lt;"},
											{'>', "&gt;"},
											{'`', "&apos;"},
											{'\"', "&quot;"},
											{kTextChar_FootnoteMarker, "&#x04;"}};

	const EscapeTable<char, char> escapeTable (escapePairs, sizeof(escapePairs)/ sizeof(escapePairs[0]));

	EscapeTable<char, char>::EscapeMap const& searchMap = escapeTable.GetEscapeMap();

	WideString::const_iterator_raw iter = inText->begin_raw();
	WideString::const_iterator_raw iterEnd = inText->end_raw();
	while (iter != iterEnd)
	{
		EscapeTable<char, char>::EscapeMapKey key(*iter);
		EscapeTable<char, char>::EscapeMap::const_iterator i = searchMap.find( key );
		if ( i != searchMap.end() )
		{
			PMString tempStr("");
			tempStr.Append(i->second.c_str());
			const char* t= i->second.data();
			PMString tempStr1(i->second.data());
		}
		else
		{
			oText->Append(*iter);
		}
		++iter;
	}
#else
	//TODO: CS5 porting
#endif
}
