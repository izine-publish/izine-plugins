//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StInDnXmlUtils.h $
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
#ifndef _h_StInDnXmlUtils_
#define _h_StInDnXmlUtils_
#pragma once

class IXMLOutStream;

#pragma mark -
class StXMLPopper
{
public:
						StXMLPopper(
							IXMLOutStream *				inStream,
							bool						inRemoveIndent = true);
						~StXMLPopper();

	IXMLOutStream *		Forget();
	void				Reset(
							IXMLOutStream *				inStream );
private:
	InterfacePtr<IXMLOutStream>		mStream;
	bool							mRemoveIndent;
};

class StXMLIndentLevel
{
public:
						StXMLIndentLevel(
							IXMLOutStream *				inStream );
						~StXMLIndentLevel();

	IXMLOutStream *		Forget();
	void				Reset(
							IXMLOutStream *				inStream );
private:
	InterfacePtr<IXMLOutStream>		mStream;
};

class StXMLIndentDisabler
{
public:
						StXMLIndentDisabler(
							IXMLOutStream *				inStream );
						~StXMLIndentDisabler();

	IXMLOutStream *		Forget();
private:
	InterfacePtr<IXMLOutStream>	mStream;
	bool						mWasEnabled;
};

#endif //_h_StInDnXmlUtils_
