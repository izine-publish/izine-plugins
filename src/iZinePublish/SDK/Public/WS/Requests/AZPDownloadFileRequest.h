//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/AZPDownloadFileRequest.h $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPDownloadFileRequest_
#define _h_AZPDownloadFileRequest_
#pragma once

#include "AZPSoapRequest.h"

class AZPDownloadFileRequest : public AZPSoapRequest
{
public:
	virtual				~AZPDownloadFileRequest();

	const IDFile &		GetOutputFile() const;
	void				SetOutputFile(
							const IDFile &				inOutputFile );

	const bool			GetNeedDecompression() const;
	void				SetNeedDecompression(
							const bool					inNeedDecompression );
protected:
	AZPDownloadFileRequest(
							enRequestType				inReqType);
private:

protected:
	IDFile			mOutputFile;
	bool			mNeedDecompression;
private:
};

#endif //_h_AZPDownloadFileRequest_
