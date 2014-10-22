//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/IZExpXMLWriteFacade.h $
//	$Revision: 3885 $
//	$Date: 2012-02-09 13:50:26 +0100 (Thu, 09 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpXMLWriteFacade_
#define _h_IZExpXMLWriteFacade_
#pragma once

class IDocument;
class IXMLOutStream;

class IZExpXMLWriteFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPXMLWRITEFACADE };

	virtual ErrorCode	ExportDocument(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc ) const = 0;

	virtual ErrorCode	ExportXMLToFile(
							const IDFile &				inFile,
							const IDocument *			inDoc ) const = 0;

};


#endif //_h_IZExpXMLWriteFacade_
