//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLExport_Paths.h $
//	$Revision: 3616 $
//	$Date: 2011-08-31 13:10:22 +0200 (Wed, 31 Aug 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-08-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpXMLExport_Paths_
#define _h_CZExpXMLExport_Paths_
#pragma once

class IXMLOutStream;
class IDocument;
class IPathGeometry;

namespace CZExpXMLExportUtils
{
	void				AddPathInfo(
							IXMLOutStream *				ioStream,
							const IPathGeometry *		inPathGeometry );
}
#endif //_h_CZExpXMLExport_Paths_
