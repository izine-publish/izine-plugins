//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPTestWSCalling.h $
//	$Revision: 2942 $
//	$Date: 2011-05-05 14:04:10 +0200 (Thu, 05 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPTestWSCalling_
#define _h_CZPTestWSCalling_
#pragma once

class PLUGIN_DECL CZPTestWSCalling
{
public:
						CZPTestWSCalling();
	virtual				~CZPTestWSCalling();

	static void			TestDownloadVersion();
	static void			TestDownloadVersion(
							const PMString &			inVersionID,
							const IDFile &				inFileToSaveIn );

	static void			TestUploadVersion();
	static void			TestUploadVersion(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile );

	static void			TestUpdateStatus();
	static void			TestUpdateStatus(
							const PMString &			inAssetID,
							const PMString &			inOldStatusID,
							const PMString &			inNewStatusID);

	static IZPUserCredentials * QueryTestingUserCredentials();
protected:
private:
};

#endif //_h_CZPTestWSCalling_
