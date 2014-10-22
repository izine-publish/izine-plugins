//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/IZP_ErrorID.h $
//	$Revision: 3544 $
//	$Date: 2011-07-26 14:39:52 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZP_ErrorID_
#define _h_IZP_ErrorID_
#pragma once

DECLARE_PMID(kErrorIDSpace, kZPOpenFileWithoutLoginError,													kIZPPrefix + 0 )
DECLARE_PMID(kErrorIDSpace, kZPNewAssignmentNotAllowedError,												kIZPPrefix + 1 )
DECLARE_PMID(kErrorIDSpace, kZPAssetDeletedOnServerError,													kIZPPrefix + 2 )
DECLARE_PMID(kErrorIDSpace, kZPAssetDownloadingError,														kIZPPrefix + 3 )
DECLARE_PMID(kErrorIDSpace, kZPAssignmentCheckinError,														kIZPPrefix + 4 )
DECLARE_PMID(kErrorIDSpace, kZPAssignmentPropertiesUpdateError,												kIZPPrefix + 5 )
DECLARE_PMID(kErrorIDSpace, kZPAssignmentUpdatingError,														kIZPPrefix + 6 )
DECLARE_PMID(kErrorIDSpace, kZPDocumentAlreadyOpenError,													kIZPPrefix + 7 )
DECLARE_PMID(kErrorIDSpace, kZPDocumentLockError,															kIZPPrefix + 8 )
DECLARE_PMID(kErrorIDSpace, kZPDocumentWritingError,														kIZPPrefix + 9 )
DECLARE_PMID(kErrorIDSpace, kZPExportImageToJpegError,														kIZPPrefix + 10 )
DECLARE_PMID(kErrorIDSpace, kZPFileOpeningError,															kIZPPrefix + 11 )
DECLARE_PMID(kErrorIDSpace, kZPLinkedStoryWritingError,														kIZPPrefix + 12 )
DECLARE_PMID(kErrorIDSpace, kZPLockAcquireFailError,														kIZPPrefix + 13 )
DECLARE_PMID(kErrorIDSpace, kZPLoginFailedError,															kIZPPrefix + 14 )
DECLARE_PMID(kErrorIDSpace, kZPServerErrorError,															kIZPPrefix + 15 )

DECLARE_PMID(kErrorIDSpace, kZPWSAssetCreatingError,														kIZPPrefix + 50 )
DECLARE_PMID(kErrorIDSpace, kZPWSAssetRetrieveError,														kIZPPrefix + 51 )
DECLARE_PMID(kErrorIDSpace, kZPWSAssetUploadError,															kIZPPrefix + 52 )
DECLARE_PMID(kErrorIDSpace, kZPWSAssignmentUploadingError,													kIZPPrefix + 53 )
DECLARE_PMID(kErrorIDSpace, kZPWSDocumentCheckinError,														kIZPPrefix + 54 )
DECLARE_PMID(kErrorIDSpace, kZPWSDocumentUploadingError,													kIZPPrefix + 55 )
DECLARE_PMID(kErrorIDSpace, kZPWSPDFUploadError,															kIZPPrefix + 56 )

DECLARE_PMID(kErrorIDSpace, kZPUIEditionCannotChangeError,													kIZPPrefix + 100 )
DECLARE_PMID(kErrorIDSpace, kZPUIEditionEmptyError,															kIZPPrefix + 101 )
DECLARE_PMID(kErrorIDSpace, kZPUINameEmptyError,															kIZPPrefix + 102 )
DECLARE_PMID(kErrorIDSpace, kZPUITitleCannotChangeError,													kIZPPrefix + 103 )
DECLARE_PMID(kErrorIDSpace, kZPUITitleEmptyError,															kIZPPrefix + 104 )
DECLARE_PMID(kErrorIDSpace, kZPUIStatusEmptyError,															kIZPPrefix + 105 )
DECLARE_PMID(kErrorIDSpace, kZPUIDuplicateAssetNameError,													kIZPPrefix + 106 )

#endif //_h_IZP_ErrorID_
