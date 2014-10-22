//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StInDnXmlUtils.cpp $
//	$Revision: 2523 $
//	$Date: 2010-11-16 10:01:30 +0100 (Tue, 16 Nov 2010) $
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
#include "IXMLOutStream.h"

//SDK General includes

//OS / ThirdParty includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "StInDnXmlUtils.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StXMLPopper::StXMLPopper(
	IXMLOutStream *				inStream,
	bool						inRemoveIndent)
{
	if( inStream )
		inStream->AddRef();
	mStream.reset( inStream );
	mRemoveIndent = inRemoveIndent ;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StXMLPopper::~StXMLPopper()
{
	if( mStream )
	{
		mStream->Pop( kTrue );
		if( mRemoveIndent )
			mStream->IndentLevel(kFalse);
	}
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
IXMLOutStream *
StXMLPopper::Forget()
{
	return mStream.forget();
}

//----------------------------------------------------------------------------------------
// Reset
//----------------------------------------------------------------------------------------
void
StXMLPopper::Reset(
	IXMLOutStream *				inStream)
{
	if( inStream )
		inStream->AddRef();
	mStream.reset( inStream );
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StXMLIndentLevel::StXMLIndentLevel(
	IXMLOutStream *				inStream)
{
	if( inStream )
	{
		inStream->AddRef();
		inStream->IndentLevel( kTrue );
	}
	mStream.reset( inStream );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StXMLIndentLevel::~StXMLIndentLevel()
{
	if( mStream )
	{
		mStream->IndentLevel(kFalse);
	}
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
IXMLOutStream *
StXMLIndentLevel::Forget()
{
	return mStream.forget();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StXMLIndentDisabler::StXMLIndentDisabler(
	IXMLOutStream *				inStream)
{
	if( inStream )
	{
		inStream->AddRef();
		mWasEnabled = inStream->DoIndent() == kTrue;
		inStream->Indent(kFalse);
	}
	mStream.reset( inStream );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StXMLIndentDisabler::~StXMLIndentDisabler()
{
	if( mStream && mWasEnabled)
	{
		mStream->Indent(kTrue);
	}
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
IXMLOutStream *
StXMLIndentDisabler::Forget()
{
	return mStream.forget();
}
#pragma mark -
