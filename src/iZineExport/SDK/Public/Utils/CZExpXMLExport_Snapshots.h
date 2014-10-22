//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLExport_Snapshots.h $
//	$Revision: 3885 $
//	$Date: 2012-02-09 13:50:26 +0100 (Thu, 09 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-9-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpXMLExport_Snapshots_
#define _h_CZExpXMLExport_Snapshots_
#pragma once

class IXMLOutStream;
class IDocument;
class IAttributeStrand;

namespace CZExpXMLExportUtils
{
	ErrorCode			ExportPageLayersSnapshot(
							const IDFile &				inXMLFilePath,
							const IDocument *			inDoc );
}
#endif //_h_CZExpXMLExport_Snapshots_
