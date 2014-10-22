//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpWSFacadeProxy.cpp $
//	$Revision: 3260 $
//	$Date: 2011-06-15 09:27:21 +0200 (Wed, 15 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "FileUtils.h"
#include "K2Vector.tpp"

//OS / ThirdParty includes
extern "C"
{
#include "curl\curl.h"
}
#include "stdio.h"
#include "tinyxml.h"

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpWSFacade.h"

//IZP General includes
#include "StUtils.h"
#include "ZExpWSRequests.h"
#include "ZExpWSResponses.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

typedef KeyValuePair<PMString, PMString> KeyValue;

#define inheritClass CPMUnknown<IZExpWSFacade>
class CZExpWSFacade : public inheritClass
{
public:
						CZExpWSFacade(IPMUnknown * boss);
	virtual				~CZExpWSFacade();

	ZExpWSError				GetTitles(
								const AZExpWSRequest *	inReq,
								AZExpWSResponse *		oResp);
	ZExpWSError				GetEditions(
								const AZExpWSRequest *	inReq,
								AZExpWSResponse *		oResp);

	ZExpWSError				UploadExport(
								const AZExpWSRequest *	inReq,
								AZExpWSResponse *		oResp);

protected:
private:
};

#pragma mark -

CREATE_PMINTERFACE(CZExpWSFacade, kZExpWSFacadeImpl)

CZExpWSFacade::CZExpWSFacade(IPMUnknown * boss)
: inheritClass(boss)
{
	
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpWSFacade::~CZExpWSFacade()
{
	
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitles
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::GetTitles(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpIDPairList & titles = ((CZExpFetchTitleListResponse*)oResp)->GetTitleList();

	titles.push_back(ZExpIDPair("1111", "aman"));
	titles.push_back(ZExpIDPair("2222", "ssfsdg"));
	titles.push_back(ZExpIDPair("3333", "dsgadgagasg"));

	return 0;
}

//----------------------------------------------------------------------------------------
// GetEditions
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::GetEditions(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpIDPairList & editions = ((CZExpFetchTitleEditionsResponse*)oResp)->GetEditionList();
	
	const CZExpFetchTitleEditionsRequest * theReq = (CZExpFetchTitleEditionsRequest*)inReq;
	const PMString & titleID = theReq->GetTitleId();
	if (titleID == "1111")
	{
		editions.push_back(ZExpIDPair("1111", "aman"));
		editions.push_back(ZExpIDPair("2222", "ssfsdg"));
		editions.push_back(ZExpIDPair("3333", "dsgadgagasg"));
	}
	else if (titleID == "2222")
	{
		editions.push_back(ZExpIDPair("1111", "ds"));
		editions.push_back(ZExpIDPair("2222", "dsgsg"));
		editions.push_back(ZExpIDPair("3333", "wew"));
	}
	else if (titleID == "3333")
	{
		editions.push_back(ZExpIDPair("1111", "oooo"));
		editions.push_back(ZExpIDPair("2222", "bn"));
		editions.push_back(ZExpIDPair("3333", "vbdfh"));
	}

	return 0;
}

//----------------------------------------------------------------------------------------
// UploadExport
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::UploadExport(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	const CZExpUploadExportRequest * theReq = (CZExpUploadExportRequest*)inReq;
	const PMString & titleID = theReq->GetTitleId();
	const PMString & editionID = theReq->GetEditionId();
	PMString docName = theReq->GetDocName();	//TODO: make name URL safe
	const uchar * content = theReq->GetContent();
	int contentLength = theReq->GetBuffSize();
	const PMString & filePath = theReq->GetFilePath();

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "title : %s", titleID.GrabCString() );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "edition : %s", editionID.GrabCString() );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "XML : %s", content);

	return 0;
}