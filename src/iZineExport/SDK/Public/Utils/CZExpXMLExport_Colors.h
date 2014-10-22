//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLExport_Colors.h $
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
#ifndef _h_CZExpXMLExport_Colors_
#define _h_CZExpXMLExport_Colors_
#pragma once

class IXMLOutStream;
class IDocument;
class IAttributeStrand;

namespace CZExpXMLExportUtils
{
	void				AddAllSwatches(	//Color names
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc );

	void				ConvertToRGBHex(
							const float					inR,
							const float					inG,
							const float					inB,
							char *						oColorStr );
	
	void				ConvertCMYKToRGBHex(
							const float					inC,
							const float					inM,
							const float					inY,
							const float					inK,
							char *						oColorStr );
}
#endif //_h_CZExpXMLExport_Colors_
