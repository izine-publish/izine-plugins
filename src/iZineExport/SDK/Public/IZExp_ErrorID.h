//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/IZExp_ErrorID.h $
//	$Revision: 3900 $
//	$Date: 2012-02-10 13:12:32 +0100 (Fri, 10 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExp_ErrorID_
#define _h_IZExp_ErrorID_
#pragma once

DECLARE_PMID(kErrorIDSpace, kIZExpFileWritingError,															kIZExpPrefix + 0 )
DECLARE_PMID(kErrorIDSpace, kIZExpParamError,																kIZExpPrefix + 1 )
DECLARE_PMID(kErrorIDSpace, kIZExpWSFailError,																kIZExpPrefix + 2 )
DECLARE_PMID(kErrorIDSpace, kIZExpXMLFileCreationFailError,													kIZExpPrefix + 3 )
DECLARE_PMID(kErrorIDSpace, kIZExpPDFFileCreationFailError,													kIZExpPrefix + 4 )
DECLARE_PMID(kErrorIDSpace, kIZExpXMLFileReadFailError,														kIZExpPrefix + 5 )
DECLARE_PMID(kErrorIDSpace, kIZExpXMLFileUploadFailError,													kIZExpPrefix + 6 )
DECLARE_PMID(kErrorIDSpace, kIZExpPDFFileUploadFailError,													kIZExpPrefix + 7 )
DECLARE_PMID(kErrorIDSpace, kIZExpHighResImageUploadFailError,												kIZExpPrefix + 8 )
DECLARE_PMID(kErrorIDSpace, kIZExpLowResImageUploadFailError,												kIZExpPrefix + 9 )
DECLARE_PMID(kErrorIDSpace, kIZExpUploadFinishFailError,													kIZExpPrefix + 10 )
DECLARE_PMID(kErrorIDSpace, kIZExpPluginIdEmptyError,														kIZExpPrefix + 11 )
DECLARE_PMID(kErrorIDSpace, kIZExpConfigDataFetchFailError,													kIZExpPrefix + 12 )


#endif //_h_IZExp_ErrorID_
