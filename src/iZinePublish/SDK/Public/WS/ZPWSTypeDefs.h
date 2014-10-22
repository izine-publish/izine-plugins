//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/ZPWSTypeDefs.h $
//	$Revision: 3541 $
//	$Date: 2011-07-26 14:33:45 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_ZPWSTypeDefs_
#define _h_ZPWSTypeDefs_
#pragma once

#include "ZPSmartPtrVector.h"

typedef enum
{	//Note: All numbers must be in sequence, count is used as array bound.
	eNoRequest = 0,
	eReq_Login = 1,
	eReq_AcquireLock,
	eReq_CanUpdateAssetName,
	eReq_CheckInAsset,
	eReq_CreateAsset,
	eReq_CreateTask,
	eReq_DeleteAsset,
	eReq_DeleteTask,
	eReq_DownloadThumbnail,
	eReq_DownloadVersion,
	eReq_FetchAsset,
	eReq_FetchAssetList,
	eReq_FetchAssets,
	eReq_FetchEditionList,
	eReq_FetchLockInfo,
	eReq_FetchTitleList,
	eReq_FetchTitleStatus,
	eReq_GetRole,
	eReq_GetTaskList,
	eReq_GetTaskCategoryList,
	eReq_GetTaskHistory,
	eReq_GetTaskStatusList,
	eReq_GetUsersForTitle,
	eReq_ReleaseLock,
	eReq_RelinkAsset,
	eReq_UpdateAsset,
	eReq_UpdateAssetName,
	eReq_UpdateAssetStatus,
	eReq_UpdateTask,
	eReq_UploadPagePreview,
	eReq_UploadPDF,
	eReq_UploadVersion,
	eReq_UploadExport,
	eReqCount
} enRequestType;

typedef enum
{
	eResp_Login = 1,
	eResp_AcquireLock,
	eResp_CanUpdateAssetName,
	eResp_CheckInAsset,
	eResp_CreateAsset,
	eResp_CreateTask,
	eResp_DeleteAsset,
	eResp_DeleteTask,
	eResp_DownloadThumbnail,
	eResp_DownloadVersion,
	eResp_FetchAsset,
	eResp_FetchAssetList,
	eResp_FetchAssets,
	eResp_FetchEditionList,
	eResp_FetchLockInfo,
	eResp_FetchTitleList,
	eResp_FetchTitleStatus,
	eResp_GetRole,
	eResp_GetTaskList,
	eResp_GetTaskCategoryList,
	eResp_GetTaskHistory,
	eResp_GetTaskStatusList,
	eResp_GetUsersForTitle,
	eResp_ReleaseLock,
	eResp_RelinkAsset,
	eResp_UpdateAsset,
	eResp_UpdateAssetName,
	eResp_UpdateAssetStatus,
	eResp_UpdateTask,
	eResp_UploadPagePreview,
	eResp_UploadPDF,
	eResp_UploadVersion,
	eResp_UploadExport,
	eRespCount
} enResponseType;

typedef enum
{
	eReqDiscard_None = 0,
	eReqDiscard_Old,
	eReqDiscard_New
} enRequestDiscardType;

typedef int	ZPWSError;

typedef enum
{
	eWSErr_Success = 0,	//SOAP_OK
	//0 to 42 are soap errors
	/* gSOAP HTTP response status codes 100 to 599 are reserved */
	eWSErr_HTTP_AuthenticationError = 401,
	/* Codes 600 to 999 are user definable */
	/* Exceptional gSOAP HTTP response status codes >= 1000 */
	// these are till 1003
	// So keeping iZinePublish Errors above 2000

	// Soap error categories, as defined in stdsoap2.h
	eWSErr_XMLError = 6000,
	eWSErr_IncompatibleServer,
	eWSErr_NetworkError,
	eWSErr_SSLError,
	eWSErr_CompressionError,
	eWSErr_HTTPError,
		
	//programming errors
	eWSErr_UnknownError,
	eWSErr_InvalidRequest,
	eWSErr_InvalidRequestType,
	eWSErr_NoRequestProcessor,
	
	//Common errors
	eWSErr_InvalidResponseObj,
	eWSErr_InvalidRequestParams,
	eWSErr_UserNotLogedIn,
	eWSErr_DuplicateRequest,
	eWSErr_RequestDiscarded, //due to cancel login or logout in progress.
	eWSErr_ObjectAllocError,
	
	//Request specific errors
	//eWSErr_InvalidResponseFromServer,
};


class AZPSoapRequest;
class AZPSoapResponse;
//Function that process the request i.e makes the WS call, block the thread till result is available.
typedef ZPWSError	(*ZPSoapRequestProcessor)( const AZPSoapRequest * inReq, AZPSoapResponse * oResponse);

struct ZPIDPair
{
	PMString		mID;
	PMString		mName;
};

typedef ZPSmartPtrVector<ZPIDPair> ZPIDPairSList;		//ZPIDPair smart list

struct ZPTitleStatusDetail
{
	bool		mIsLayout;
	bool		mIsText;
	PMString	mStateID;
	int			mSortIndex;

	ZPTitleStatusDetail()
		: mIsLayout( false )
		, mIsText (false )
		, mSortIndex ( 0 )
	{	}
};
typedef ZPSmartPtrVector<ZPTitleStatusDetail> ZPTitleStatusDetailSList;

struct ZPTitleStatusInfo
{
	PMString		mTitleID;
	ZPIDPairSList	mStatusList;
	ZPTitleStatusDetailSList	mStatusDetailList;
};

typedef ZPSmartPtrVector<ZPTitleStatusInfo> ZPTitleStatusInfoSList;

struct ZPTitleEditionInfo
{
	PMString		mTitleID;
	ZPIDPairSList	mEditionList;
};

typedef ZPSmartPtrVector<ZPTitleEditionInfo> ZPTitleEditionInfoSList;

struct ZPAssetDetailInfo
{
	ZPIDPair		mDocInfo;	//LinkedWithAsset
	//PMString		mLockID;
	PMString		mTypeID;
	PMString		mType;
	//Version info
	PMString		mVersionID;
	ZPIDPair		mVersionUser;
	time_t			mVersionDate;
	int				mVersionNumber;
	PMString		mStatusID;
	ZPAssetDetailInfo()
		: mVersionDate( 0 )
		, mVersionNumber( 0 )
	{
	}

	bool operator ==( const ZPAssetDetailInfo & inVal )
	{
		bool toReturn = false;
		do
		{
			if( mDocInfo.mID != inVal.mDocInfo.mID )
				break;
			if( mDocInfo.mName != inVal.mDocInfo.mName )
				break;
			if( mTypeID != inVal.mTypeID )
				break;
			if( mType != inVal.mType )
				break;
			
			if( mVersionID != inVal.mVersionID )
				break;
			if( mVersionUser.mID != inVal.mVersionUser.mID )
				break;
			if( mVersionUser.mName != inVal.mVersionUser.mName )
				break;
			if( mVersionDate != inVal.mVersionDate )
				break;
			if( mVersionNumber != inVal.mVersionNumber )
				break;
			if( mStatusID != inVal.mStatusID )
				break;

			toReturn = true;
		}while(false);
		return toReturn;
	}
};

typedef ZPSmartPtrVector<ZPAssetDetailInfo> ZPAssetDetailSList;


struct ZPAssetInfo
{
	ZPIDPair	mAsset;
	PMString	mEditionID;
	ZPAssetDetailInfo mAssetDetailInfo;
};
typedef ZPSmartPtrVector<ZPAssetInfo> ZPAssetInfoSList;


struct ZPAssetLockInfo
{
	PMString	mAssetID;
	PMString	mLockID;
	PMString	mDocumentID;
	PMString	mDocumentName;	//Should we store it here? When AssetID will ready be in cache with AssetMgr.
	PMString	mApplicationName;	//Someday: made it shared string;
	PMString	mUserID;
	PMString	mUserName;	//Someday: create users mgr class, to save memory used by duplicate objects having same value.
};
typedef ZPSmartPtrVector<ZPAssetLockInfo> ZPAssetLockInfoSList;

struct ZPTaskInfo
{
	PMString		mDescription;
	PMString		mDocumentID;
	PMString		mAssignedToUserID;
	PMString		mStatusID;
	PMString		mCategoryID;
	PMString		mSpread;
	PMString		mPage;

	ZPIDPair		mLastUpdatingUser;
	time_t			mLastUpdatedDate;
	int				mCurrVersionNumber;
	ZPTaskInfo()
		: mLastUpdatedDate( 0 )
		, mCurrVersionNumber( 0 )
	{
	}
};
typedef ZPSmartPtrVector<ZPTaskInfo> ZPTaskInfoSList;

#endif //_h_ZPWSTypeDefs_
