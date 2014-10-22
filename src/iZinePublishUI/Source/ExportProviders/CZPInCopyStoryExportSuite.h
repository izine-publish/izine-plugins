//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ExportProviders/CZPInCopyStoryExportSuite.h $
//	$Revision: 2028 $
//	$Date: 2010-09-06 12:21:59 +0200 (Mon, 06 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPInCopyStoryExportSuite_
#define _h_CZPInCopyStoryExportSuite_
#pragma once

#include "CInCopyStoryExportSuite.h"

#define inheritClass_ZPInCopyStoryExport CInCopyStoryExportSuite
class CZPInCopyStoryExportSuite : public inheritClass_ZPInCopyStoryExport
{
public:
						CZPInCopyStoryExportSuite( IPMUnknown* boss );
	virtual				~CZPInCopyStoryExportSuite();

	virtual void		ExportToFile(
							const IDFile& sysFile, 
							IDocument* doc,
							IPMUnknown* targetboss, 
							const PMString& formatName, 
							UIFlags uiFlags = kFullUI
							);
	virtual UIDList		GetExportableItemList (IPMUnknown* targetboss) const;

	virtual PMIID		GetDelegateInterfaceID() const = 0;

	virtual CInCopyStoryExportSuite *	QueryDelegateExportProvider(
							IPMUnknown *				inTargetBoss ) const;
protected:
	bool			mReturnSingleSelection;
private:
};

#endif //_h_CZPInCopyStoryExportSuite_
