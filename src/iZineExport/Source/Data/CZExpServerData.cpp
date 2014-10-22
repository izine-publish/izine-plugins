//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/CZExpServerData.cpp $
//	$Revision: 3905 $
//	$Date: 2012-02-10 15:35:39 +0100 (Fri, 10 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "ErrorUtils.h"
#include "KeyValuePair.h"
#include "Utils.h"
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpGlobalData.h"
#include "IZExpServerData.h"
#include "IZExpWSFacade.h"

//IZP General includes
#include "CZExpFetchTitleListRequest.h"
#include "CZExpFetchTitleListResponse.h"
#include "CZExpFetchTitleEditionsRequest.h"
#include "CZExpFetchTitleEditionsResponse.h"
#include "CZExpGetConfigRequest.h"
#include "CZExpGetConfigResponse.h"

#include "StUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpServerData>
class CZExpServerData : public inheritClass
{
public:
						CZExpServerData(IPMUnknown * boss);
	virtual				~CZExpServerData();

	void				FetchTitles();
	void				FetchEditions(
							const PMString &		inTitleId);

	const ZExpIDPairList &	GetTitles() const;
	const ZExpIDPairList &	GetEditions() const;

	const PMString &			GetTitleIDByIndex(
									const int				inIndex) const;
	const PMString &			GetEditionIDByIndex(
									const int				inIndex) const;

	void				GetConfig();

protected:
private:
	typedef KeyValuePair<PMString, PMString> KeyValue;

	ZExpIDPairList mTitles;		//Key, Value
	ZExpIDPairList mEditions;		//Key, Value
};


CREATE_PMINTERFACE(CZExpServerData, kZExpServerDataImpl)

CZExpServerData::CZExpServerData(IPMUnknown * boss)
: inheritClass(boss)
{
	mTitles.clear();
	mEditions.clear();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpServerData::~CZExpServerData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchTitles
//----------------------------------------------------------------------------------------
void
CZExpServerData::FetchTitles()
{
	mEditions.clear();		//Clear editions list

	CZExpFetchTitleListRequest theReq;
	CZExpFetchTitleListResponse * theResp = (CZExpFetchTitleListResponse*)((AZExpWSRequest*)(&theReq))->CreateResponseObj();
	StPtrDeleter<CZExpFetchTitleListResponse> autoDelResp( theResp, true );
	Utils<IZExpWSFacade>()->GetTitles( &theReq, theResp);

	if( theResp->GetErrorCode() != kSuccess )
	{
		ErrorCode err = kIZExpWSFailError;
		PMString errStr("Server Error: ");
		errStr.AppendNumber( theResp->GetErrorCode() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDescription() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDetails() );
		errStr.SetTranslatable( kFalse );
		ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
	}
	
	mTitles = theResp->GetTitleList();
}

//----------------------------------------------------------------------------------------
// FetchEditions
//----------------------------------------------------------------------------------------
void
CZExpServerData::FetchEditions(
	const PMString &		inTitleId)
{
	mEditions.clear();
	CZExpFetchTitleEditionsRequest theReq;
	CZExpFetchTitleEditionsResponse * theResp = (CZExpFetchTitleEditionsResponse*)((AZExpWSRequest*)(&theReq))->CreateResponseObj();
	StPtrDeleter<CZExpFetchTitleEditionsResponse> autoDelResp( theResp, true );
	theReq.SetTitleId(inTitleId);
	Utils<IZExpWSFacade>()->GetEditions( &theReq, theResp);

	if( theResp->GetErrorCode() != kSuccess )
	{
		ErrorCode err = kIZExpWSFailError;
		PMString errStr("Server Error: ");
		errStr.AppendNumber( theResp->GetErrorCode() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDescription() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDetails() );
		errStr.SetTranslatable( kFalse );
		ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
	}

	mEditions = theResp->GetEditionList();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitles
//----------------------------------------------------------------------------------------
const ZExpIDPairList &
CZExpServerData::GetTitles()const
{
	return mTitles;
}

//----------------------------------------------------------------------------------------
// GetEditions
//----------------------------------------------------------------------------------------
const ZExpIDPairList &
CZExpServerData::GetEditions()const
{
	return mEditions;
}

//----------------------------------------------------------------------------------------
// GetTitleIDByIndex
//----------------------------------------------------------------------------------------
const PMString &
CZExpServerData::GetTitleIDByIndex(
	const int				inIndex) const
{
	if (inIndex < 0 || inIndex >= mTitles.size())
		return kNullString;

	return mTitles[inIndex].mID;
}

//----------------------------------------------------------------------------------------
// GetEditionIDByIndex
//----------------------------------------------------------------------------------------
const PMString &
CZExpServerData::GetEditionIDByIndex(
	const int				inIndex) const
{
	if (inIndex < 0 || inIndex >= mEditions.size())
		return kNullString;

	return mEditions[inIndex].mID;
}

//----------------------------------------------------------------------------------------
// GetConfig
//----------------------------------------------------------------------------------------
void
CZExpServerData::GetConfig()
{
	CZExpGetConfigRequest theReq;
	CZExpGetConfigResponse * theResp = (CZExpGetConfigResponse*)((AZExpWSRequest*)(&theReq))->CreateResponseObj();
	StPtrDeleter<CZExpGetConfigResponse> autoDelResp( theResp, true );
	Utils<IZExpWSFacade>()->GetConfig( &theReq, theResp);
	
	if( theResp->GetErrorCode() != kSuccess )
	{
		ErrorCode err = kIZExpWSFailError;
		PMString errStr("Server Error: ");
		errStr.AppendNumber( theResp->GetErrorCode() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDescription() );
		errStr.Append("\n");
		errStr.Append( theResp->GetErrorDetails() );
		errStr.SetTranslatable( kFalse );
		ErrorUtils::PMSetGlobalErrorCode( err, &errStr );
		return;
	}

#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif

	InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );
	gGlobalData->SetDPIHighRes( theResp->GetDPIHighRes());
	gGlobalData->SetDPILowRes( theResp->GetDPILowRes());
	gGlobalData->SetExportPDF( theResp->IsExportPDF());
	gGlobalData->SetExportTiff( theResp->IsExportTiff());
	gGlobalData->SetStyleName( theResp->GetStyleName());
	gGlobalData->SetExportLowRes( theResp->IsExportLowRes());
	gGlobalData->SetExportPagePreview( theResp->IsExportPagePreview());
}
