//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/ZExpWSTypeDefs.h $
//	$Revision: 3941 $
//	$Date: 2012-02-24 07:36:48 +0100 (Fri, 24 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZExpWSTypeDefs_
#define _h_ZExpWSTypeDefs_
#pragma once

#include "ZPSmartPtrVector.h"
#include "K2Vector.tpp"

typedef enum
{	//Note: All numbers must be in sequence, count is used as array bound.
	eNoRequest = 0,
	eReq_FetchTitleList = 1,
	eReq_FetchTitleEditions,
	eReq_UploadExport,
	eReq_GetConfig,
	eReq_ValidateMediaContent,
	eReqCount
} enWSRequestType;

typedef enum
{
	eResp_FetchTitleList = 1,
	eResp_FetchTitleEditions,
	eResp_UploadExport,
	eResp_GetConfig,
	eResp_ValidateMediaContent,
	eRespCount
} enResponseType;

typedef int	ZExpWSError;

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
	eWSErr_NoFileToUpload,
	
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

class AZExpWSRequest;
class AZExpWSResponse;

//Function that process the request i.e makes the WS call, block the thread till result is available.
//typedef ZPWSError	(*ZPSoapRequestProcessor)( const AZPSoapRequest * inReq, AZPSoapResponse * oResponse);

struct ZExpIDPair
{
	typedef object_type data_type;
	PMString		mID;
	PMString		mName;
	ZExpIDPair() {}
	ZExpIDPair ( const PMString & inID, const PMString & inName )
		: mID( inID ), mName( inName )
	{ }
};

typedef K2Vector<ZExpIDPair> ZExpIDPairList;		//ZExpIDPair list

#endif //_h_ZExpWSTypeDefs_
