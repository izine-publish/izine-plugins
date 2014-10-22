//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSCallProcessors.cpp $
//	$Revision: 4005 $
//	$Date: 2012-04-06 05:55:20 +0200 (Fri, 06 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Utils.h"

#include "IZPID.h"
#include "ZPTypes.h"

//IZP Interfaces
#include "IZPAssetUtils.h"
#include "IZPCompressionUtils.h"
#include "IZPFileUtils.h"
#include "IZPUserCredentials.h"
#include "IZPWSConfig.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "AZPSoapResponse.h"
#include "CZPAcquireLockRequest.h"
#include "CZPAcquireLockResponse.h"
#include "CZPCanUpdateAssetNameRequest.h"
#include "CZPCanUpdateAssetNameResponse.h"
#include "CZPCheckInAssetRequest.h"
#include "CZPCheckInAssetResponse.h"
#include "CZPCreateAssetRequest.h"
#include "CZPCreateAssetResponse.h"
#include "CZPDeleteAssetRequest.h"
#include "CZPDeleteAssetResponse.h"
#include "CZPDownloadThumbnailRequest.h"
#include "CZPDownloadThumbnailResponse.h"
#include "CZPDownloadVersionRequest.h"
#include "CZPDownloadVersionResponse.h"
#include "CZPFetchAssetListRequest.h"
#include "CZPFetchAssetListResponse.h"
#include "CZPFetchAssetRequest.h"
#include "CZPFetchAssetResponse.h"
#include "CZPFetchAssetsRequest.h"
#include "CZPFetchAssetsResponse.h"
#include "CZPFetchEditionListRequest.h"
#include "CZPFetchEditionListResponse.h"
#include "CZPFetchLockInfoRequest.h"
#include "CZPFetchLockInfoResponse.h"
#include "CZPFetchTitleListResponse.h"
#include "CZPFetchTitleStatusRequest.h"
#include "CZPFetchTitleStatusResponse.h"
#include "CZPGetRoleResponse.h"
#include "CZPLoginResponse.h"
#include "CZPReleaseLockRequest.h"
#include "CZPReleaseLockResponse.h"
#include "CZPRelinkAssetRequest.h"
#include "CZPRelinkAssetResponse.h"
#include "CZPUpdateAssetNameRequest.h"
#include "CZPUpdateAssetNameResponse.h"
#include "CZPUpdateAssetRequest.h"
#include "CZPUpdateAssetResponse.h"
#include "CZPUpdateAssetStatusRequest.h"
#include "CZPUpdateAssetStatusResponse.h"
#include "CZPUploadExportRequest.h"
#include "CZPUploadExportResponse.h"
#include "CZPUploadPagePreviewRequest.h"
#include "CZPUploadPagePreviewResponse.h"
#include "CZPUploadPDFRequest.h"
#include "CZPUploadPDFResponse.h"
#include "CZPUploadVersionRequest.h"
#include "CZPUploadVersionResponse.h"
#include "CZPWSCallProcessors.h"
#include "StUtils.h"
#include "ZPConstants.h"

#define USE_CUSTOM_BINDING	1
#if USE_CUSTOM_BINDING
#include "CustomBinding_USCOREserver.nsmap"
#include "soapCustomBinding_USCOREserverProxy.h"
#define BindingServerClass	CustomBinding_USCOREserver
#else
#include "basichttpbinding_uscoreserver.nsmap"
#include "soapBasicHttpBinding_USCOREserverProxy.h"
#define BindingServerClass	BasicHttpBinding_USCOREserver
#endif

//#include "cacerts.h"
//#include "soaph.h"
//#include "stdsoap2.h"
#include "wsseapi.h"

#pragma mark -
#define WSStruct(st)					ns3##st

#define WSSt_AssetDTO					WSStruct(__AssetDTO)
#define WSSt_EditionTitlesDTO			WSStruct(__EditionTitlesDTO)
#define WSSt_LockDTO					WSStruct(__LockDTO)
#define WSSt_StatusDTO					WSStruct(__StatusDTO)
#define WSSt_TaskDTO					WSStruct(__TaskDTO)
#define WSSt_TitleStatusDTO				WSStruct(__TitleStatusDTO)
#define WSSt_UserDTO					WSStruct(__UserDTO)
#define WSSt_VersionDTO					WSStruct(__VersionDTO)
#define WSSt_VersionInfo				WSStruct(__VersionInfo)

#define WSSt_ArrayOfAssetDTO			WSStruct(__ArrayOfAssetDTO)
#define WSSt_ArrayOfEditionTitlesDTO	WSStruct(__ArrayOfEditionTitlesDTO)
#define WSSt_ArrayOfLockDTO				WSStruct(__ArrayOfLockDTO)
#define WSSt_ArrayOfShelveDTO			WSStruct(__ArrayOfShelveDTO)
#define WSSt_ArrayOfStatusDTO			WSStruct(__ArrayOfStatusDTO)
#define WSSt_ArrayOfTaskDTO				WSStruct(__ArrayOfTaskDTO)
#define WSSt_ArrayOfTitleDTO			WSStruct(__ArrayOfTitleDTO)
#define WSSt_ArrayOfTitleStatusDTO		WSStruct(__ArrayOfTitleStatusDTO)
#define WSSt_ArrayOfUserDTO				WSStruct(__ArrayOfUserDTO)

#define WSSt_ArrayOfIdNameDTO			WSStruct(__ArrayOfIdNameDTO)

#define WSSt_ArrayOfguid				ns3__ArrayOfGuid


typedef StSmartBuff<wchar_t,45>			StBuffForID;

#pragma mark -
void	ExtactValues_AssetDTO(
			const WSSt_AssetDTO &			inAssetDTO,
			ZPIDPair *						oAssetInfo,
			ZPAssetDetailInfo &				oAssetDetail );
void	ExtactValues_VersionInfo(
			const WSSt_VersionInfo &		inVersionInfo,
			ZPAssetDetailInfo &				oAssetDetail);
void	ExtactValues_LockInfo(
			const WSSt_LockDTO &			inLockInfo,
			ZPAssetLockInfo &				oAssetLockInfo);

void	MemsetZero(
			WSSt_AssetDTO &					inAssetDTO );
void	MemsetZero(
			WSSt_VersionInfo &				inVersionInfo);
void	MemsetZero(
			WSSt_VersionDTO &				inVersionDTO );

void InitStruct_AssetDTO(
		WSSt_AssetDTO &					oAssetDTO,
		const ZPAssetDetailInfo &		inAssetDetail,
		const PMString &				inTitleID,
		const PMString &				inEditionID);
void InitStruct_VersionInfo(
		WSSt_VersionInfo &				oVersionInfo,
		const ZPAssetDetailInfo &		inAssetDetail );


void DeInitStruct_AssetDTO(
		WSSt_AssetDTO &						ioAssetDTO );
void DeInitStruct_VersionInfo(
		WSSt_VersionInfo &					ioVersionInfo );

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPWSCallProcessors::CZPWSCallProcessors()
{//Never called
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSCallProcessors::~CZPWSCallProcessors()
{//Never called
}

//----------------------------------------------------------------------------------------
// InitProcessorsArray
//----------------------------------------------------------------------------------------
void
CZPWSCallProcessors::InitProcessorsArray(
	ZPSoapRequestProcessor 			oArr[eReqCount])
{
	oArr[eReq_AcquireLock]		= DoAcquireLock;
	oArr[eReq_CanUpdateAssetName] = DoCanUpdateAssetName;
	oArr[eReq_CheckInAsset]		= DoCheckInAsset;
	oArr[eReq_CreateAsset]		= DoCreateAsset;
	oArr[eReq_DeleteAsset]		= DoDeleteAsset;
	oArr[eReq_DownloadThumbnail]= DoDownloadThumbnail;
	oArr[eReq_DownloadVersion]	= DoDownloadVersion;
	oArr[eReq_FetchAsset]		= DoFetchAsset;
	oArr[eReq_FetchAssets]		= DoFetchAssets;
	oArr[eReq_FetchAssetList]	= DoFetchAssetList;
	oArr[eReq_FetchEditionList]	= DoFetchEditions;
	oArr[eReq_FetchLockInfo]	= DoFetchLockInfo;
	oArr[eReq_FetchTitleList]	= DoFetchTitleList;
	oArr[eReq_FetchTitleStatus]	= DoFetchTitleStatusList;
	oArr[eReq_GetRole]			= DoGetRole;
	oArr[eReq_Login]			= DoLogin;
	oArr[eReq_ReleaseLock]		= DoReleaseLock;
	oArr[eReq_RelinkAsset]		= DoRelinkAsset;
	oArr[eReq_UpdateAsset]		= DoUpdateAsset;
	oArr[eReq_UpdateAssetName]	= DoUpdateAssetName;
	oArr[eReq_UpdateAssetStatus]= DoUpdateAssetStatus;
	oArr[eReq_UploadPagePreview]= DoUploadPagePreview;
	oArr[eReq_UploadPDF]		= DoUploadPDF;
	oArr[eReq_UploadVersion]	= DoUploadVersion;
	oArr[eReq_UploadExport]		= DoUploadExport;

	//Task Handling
	oArr[eReq_CreateTask]			= DoCreateTask;
	oArr[eReq_DeleteTask]			= DoDeleteTask;
	oArr[eReq_GetTaskCategoryList]	= DoFetchTaskCategoryList;
	oArr[eReq_GetTaskHistory]		= DoGetTasksHistory;
	oArr[eReq_GetTaskList]			= DoFetchEditionTasksList;
	oArr[eReq_GetTaskStatusList]	= DoFetchTaskStatusList;
	oArr[eReq_GetUsersForTitle]		= DoFetchTitleUsersList;
	oArr[eReq_UpdateTask]			= DoUpdateTask;
}

#pragma mark -
//Common macro used for processing all requests.
#define DefineAndInitServiceVar( inRequest )											\
	BindingServerClass _service;														\
	const IZPUserCredentials *	theUserCredentials = inRequest->GetUserCredentials();	\
	const IZPWSConfig *	theWSConfig = inRequest->GetWSConfig();							\
	InitSoapStruct( _service.soap, theUserCredentials, theWSConfig );					\
	_service.endpoint = theWSConfig->GetURL().GrabCString();


//Copy the error details to response object
#ifdef DEBUG
#define CopyErrorDetailIfAny( inResponse )													\
	if( _service.soap->error )																\
	{																						\
		const char ** theFaultString = soap_faultstring(_service.soap);						\
		if( theFaultString )																\
		{																					\
			PMString theErrDetails(*theFaultString, PMStringCharPtrParam);					\
			const char ** errDetail = soap_faultdetail(_service.soap);						\
			if( errDetail && *errDetail)													\
			{																				\
				theErrDetails.Append("\n");													\
				theErrDetails.Append(PMString(*errDetail, PMStringCharPtrParam));			\
			}																				\
			inResponse->SetErrorDetails( theErrDetails );									\
		}																					\
	}
#else
#define CopyErrorDetailIfAny( inResponse )													\
	if( _service.soap->error )																\
	{																						\
		const char ** theFaultString = soap_faultstring(_service.soap);						\
		if( theFaultString )																\
		{																					\
			inResponse->SetErrorDetails( *theFaultString );									\
		}																					\
	}
#endif

#define CopyWCharPtrToPMString( oPMStr, inCharPtr )				\
	if( inCharPtr )												\
		oPMStr = WideString( inCharPtr );						\
	oPMStr.SetTranslatable( kFalse );

#define CreateZeroInitWChar_tArray(pointerToInit,arraySize)		\
	pointerToInit = new wchar_t[ arraySize ];					\
	pointerToInit[0] = 0;

#define CreateZeroInitIntArray(pointerToInit,arraySize)		\
	pointerToInit = new int[ arraySize ];					\
	::memset( pointerToInit, 0, sizeof(int) * arraySize);

#include "CZPWSCallProcessors_Tasks.hpp"

/** gSoap flags.
Flag 	Description
SOAP_IO_FLUSH		in: disable buffering and flush output (default for all file-based output)
SOAP_IO_BUFFER		in: enable buffering (default for all socket-oriented connections)
SOAP_IO_STORE		in: store entire message to calculate HTTP content length
SOAP_IO_CHUNK		out: use HTTP chunking
SOAP_IO_LENGTH		out: (internal flag) require apriori calculation of content length
SOAP_IO_KEEPALIVE	in&out: attempt to keep socket connections alive (open)
SOAP_IO_UDP			in&out: use UDP (datagram) transport, maximum message length is SOAP_BUFLEN
SOAP_ENC_XML		out: use plain XML encoding without HTTP headers (useful with SOAP_ENC_ZLIB)
SOAP_ENC_DIME		out: use DIME encoding (automatic when DIME attachments are used)
SOAP_ENC_MIME		out: use MIME encoding (activate using soap_set_mime)
SOAP_ENC_MTOM		out: use MTOM XOP attachments (instead of DIME)
SOAP_ENC_ZLIB		out: compress encoding with Zlib (deflate or gzip format)
SOAP_ENC_SSL		in&out: encrypt with SSL (automatic with "https:" endpoints)
SOAP_XML_INDENT		out: produces indented XML output
SOAP_XML_CANONICAL	out: produces canonical XML output
SOAP_XML_DEFAULTNS	out: produces xmlns="..." default binding namespaced output
SOAP_XML_IGNORENS	in: ignores the use of XML namespaces in input
SOAP_XML_STRICT		in: XML strict validation
SOAP_XML_TREE		out: serialize data as XML trees (no multi-ref, duplicate data when necessary)
					in: ignore id attributes (do not resolve id-ref)
SOAP_XML_GRAPH		out: serialize data as an XML graph with inline multi-ref (SOAP 1.2 default)
SOAP_XML_NIL		out: serialize NULL data as xsi:nil elements (omit by default)
SOAP_C_NOIOB		in: do not fault with SOAP_IOB
SOAP_C_UTFSTRING	in&out: (de)serialize 8-bit strings "as is" (strings MUST have UTF-8 encoded content)
SOAP_C_MBSTRING		in&out: enable multibyte character support (depends on locale)
SOAP_C_NILSTRING	out: serialize empty strings as nil (ommited element) 
*/
//----------------------------------------------------------------------------------------
// InitSoapStruct
//----------------------------------------------------------------------------------------
void
CZPWSCallProcessors::InitSoapStruct(
	soap *						oSoap,
	const IZPUserCredentials *	inUserCredentials,
	const IZPWSConfig *			inWSConfig )
{
	//soap_init( oSoap );	//Init is used in C, not in C++ as it already called during creation of object.
	soap_register_plugin( oSoap, soap_wsse);
	soap_set_omode(oSoap, SOAP_XML_INDENT);
	soap_set_omode( oSoap, SOAP_XML_CANONICAL);
	
	//soap_set_omode( oSoap, SOAP_ENC_ZLIB );	//Send the request compressed. Not working on server side.
	// soap_imode( oSoap, SOAP_ENC_ZLIB); // optional: gzip is detected automatically
	// soap_omode( oSoap, SOAP_ENC_ZLIB); // optional: gzip is detected automatically
	
	//	soap_ssl_client_cacerts( oSoap );
	soap_ssl_client_context( oSoap, SOAP_SSL_SKIP_HOST_CHECK, NULL, NULL, NULL, NULL, NULL);

	if(inWSConfig->NeedsProxyServer())
	{
		oSoap->proxy_host = inWSConfig->GetProxyServer().GrabCString();
		oSoap->proxy_port = inWSConfig->GetProxyPort();
	}
	else
	{
		oSoap->proxy_host = NULL;
		oSoap->proxy_port = NULL;
	}

	soap_wsse_add_Timestamp( oSoap, "Time", 0);
	soap_wsse_add_UsernameTokenText( oSoap, "User", inUserCredentials->GetUserName().GrabCString(), inUserCredentials->GetPwd().GrabCString());
	
}

#pragma mark - the Request processors -
//Use this macro to define function which do nothing with request & response structures
#define DefineDefaultCallProcessor( inFunctionName, inStructName )							\
ZPWSError																				\
CZPWSCallProcessors::inFunctionName(													\
	const AZPSoapRequest *		inRequest,												\
	AZPSoapResponse *			oResponse )												\
{																						\
	DefineAndInitServiceVar(inRequest);													\
	_ns3__##inStructName theRequest;													\
	_ns3__##inStructName##Response theResponse;											\
																						\
	ZPWSError toReturn = _service.__ns1__##inStructName( &theRequest, &theResponse);	\
	CopyErrorDetailIfAny(oResponse);													\
																						\
	return toReturn;																	\
}

#pragma mark -
#if 1
//----------------------------------------------------------------------------------------
// DoLogin
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoLogin(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
#if 1	//0 while debugging the macro
	DefineAndInitServiceVar(inRequest);
#else
	BindingServerClass _service;
	const IZPUserCredentials *	theUserCredentials = inRequest->GetUserCredentials();
	const IZPWSConfig *	theWSConfig = inRequest->GetWSConfig();
	InitSoapStruct( _service.soap, theUserCredentials, theWSConfig );
	_service.endpoint = theWSConfig->GetURL().GrabCString();
#endif

	_ns3__Login loginRequest;
	_ns3__LoginResponse loginResponse;
	
	ZPWSError toReturn = _service.__ns1__Login( &loginRequest, &loginResponse);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPLoginResponse * theResponse = dynamic_cast<CZPLoginResponse*>(oResponse);
		ASSERT(theResponse);
		const WSSt_UserDTO * theLoginResult = loginResponse.LoginResult;
		PMString userID;
		PMString displayName;
		if( theLoginResult )
		{
			CopyWCharPtrToPMString( userID, theLoginResult->userid);
			CopyWCharPtrToPMString( displayName, theLoginResult->name);
		}
		theResponse->SetUserID( userID );
		theResponse->SetDisplayName( displayName );
	}

	return toReturn;
}
#else
DefineDefaultCallProcessor( DoLogin , Login )
#endif


//----------------------------------------------------------------------------------------
// DoGetRole
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoGetRole(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetRole theReqData;
	_ns3__GetRoleResponse theRespData;

	ZPWSError toReturn = _service.__ns1__GetRole( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetRoleResponse * theResponse = dynamic_cast<CZPGetRoleResponse*>(oResponse);
		ASSERT(theResponse);
		PMString role;
		CopyWCharPtrToPMString( role, theRespData.GetRoleResult);
		theResponse->SetRole( role );
	}

	return toReturn;
}


//----------------------------------------------------------------------------------------
// DoFetchTitleList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchTitleList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetTitleList theReqData;
	_ns3__GetTitleListResponse theRespData;

	ZPWSError toReturn = _service.__ns1__GetTitleList( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchTitleListResponse * theResponse = dynamic_cast<CZPFetchTitleListResponse*>(oResponse);
		ASSERT(theResponse);
		
		ZPPMStringSList & titleKeys = theResponse->GetTitleKeys();
		ZPPMStringSList & titleNames = theResponse->GetTitleNames();

		WSSt_ArrayOfTitleDTO *getTitleListResult = theRespData.GetTitleListResult;
		titleKeys.reserve(getTitleListResult->__sizeTitleDTO ); //titleKeys.SetOwnsItem();
		titleNames.reserve(getTitleListResult->__sizeTitleDTO ); //titleNames.SetOwnsItem();

		for (int i = 0 ; i < getTitleListResult->__sizeTitleDTO ; ++i)
		{
			PMString * titleKey = new PMString(WideString(getTitleListResult->TitleDTO[i]->constantid));
			PMString * title = new PMString(WideString(getTitleListResult->TitleDTO[i]->name));
			titleKeys.push_back( titleKey );
			titleNames.push_back( title );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchEditions
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchEditions(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetEditionsByTitle theReqData;
	_ns3__GetEditionsByTitleResponse theRespData;

#if 1
	StBuffForID	titleid(MaxSize_TitleID);
	theReqData.titleid = titleid.Get();
#else
	CreateZeroInitWChar_tArray(theReqData.titleid, MaxSize_TitleID)
	StPtrDeleter<wchar_t> autoDel_titleid( theReqData.titleid );
#endif

	const CZPFetchEditionListRequest * theRequest = dynamic_cast<const CZPFetchEditionListRequest*> ( inRequest );
	ASSERT( theRequest );

	theRequest->GetTitleID().GetWChar_tString(theReqData.titleid, MaxSize_TitleID);


	// get editions for title
	ZPWSError toReturn = _service.__ns1__GetEditionsByTitle( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchEditionListResponse * theResponse = dynamic_cast<CZPFetchEditionListResponse*>(oResponse);
		ASSERT(theResponse);
	
		ZPTitleEditionInfoSList &	titleEditionsList = theResponse->GetInfoList();

		WSSt_ArrayOfEditionTitlesDTO * responseList = theRespData.GetEditionsByTitleResult;

		for (int i = 0; i < responseList->__sizeEditionTitlesDTO ; ++i)
		{
			WSSt_EditionTitlesDTO * currTitleData = responseList->EditionTitlesDTO[i];

			WSSt_ArrayOfShelveDTO * editionDTOList = currTitleData->editions;
		
			ZPTitleEditionInfo * currTitleEditionInfo = new ZPTitleEditionInfo;
		
			CopyWCharPtrToPMString( currTitleEditionInfo->mTitleID, currTitleData->titleid );

			if( currTitleEditionInfo->mTitleID.Compare( kFalse, kEmptyGUID) != 0 
				|| editionDTOList->__sizeShelveDTO )					//TODO: remove when updated from new wsdl
			{
				titleEditionsList.push_back( currTitleEditionInfo );
			}


			ZPIDPairSList & currEditionsList = currTitleEditionInfo->mEditionList;

			for (int i = 0; i < editionDTOList->__sizeShelveDTO ; ++i)
			{
				ZPIDPair * newEdition = new ZPIDPair();
				currEditionsList.push_back( newEdition );

				CopyWCharPtrToPMString( newEdition->mID, editionDTOList->ShelveDTO[i]->shelveid);
				CopyWCharPtrToPMString( newEdition->mName, editionDTOList->ShelveDTO[i]->name);
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchTitleStatusList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchTitleStatusList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetStatusByTitle theReqData;
	_ns3__GetStatusByTitleResponse theRespData;

	StBuffForID	titleid(MaxSize_TitleID);
	theReqData.titleid = titleid.Get();

	const CZPFetchTitleStatusRequest * theRequest = dynamic_cast<const CZPFetchTitleStatusRequest*> ( inRequest );
	ASSERT( theRequest );

	theRequest->GetTitleId().GetWChar_tString(theReqData.titleid, MaxSize_TitleID);


	ZPWSError toReturn = _service.__ns1__GetStatusByTitle( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchTitleStatusResponse * theResponse = dynamic_cast<CZPFetchTitleStatusResponse*>(oResponse);
		ASSERT(theResponse);

		ZPTitleStatusInfoSList &	titleStatusList = theResponse->GetInfoList();
		
		WSSt_ArrayOfTitleStatusDTO *getTitleStatusListResult = theRespData.GetStatusByTitleResult;
		for (int i = 0 ; i < getTitleStatusListResult->__sizeTitleStatusDTO ; ++i)
		{
			WSSt_TitleStatusDTO * currTitleDTO = getTitleStatusListResult->TitleStatusDTO[i];

			ZPTitleStatusInfo * currTitleStatusInfo = new ZPTitleStatusInfo;
			titleStatusList.push_back( currTitleStatusInfo );

		
			CopyWCharPtrToPMString( currTitleStatusInfo->mTitleID, currTitleDTO->titleid ); //new wsdl not being used yet

			ZPIDPairSList & currStatusList = currTitleStatusInfo->mStatusList;
			ZPTitleStatusDetailSList & currStatusDetailList = currTitleStatusInfo->mStatusDetailList;

			WSSt_ArrayOfStatusDTO *statusDTOList = currTitleDTO->statuses;
			for (int j = 0 ; j < statusDTOList->__sizeStatusDTO ; ++j)
			{
				const WSSt_StatusDTO * currStatusDTO = statusDTOList->StatusDTO[j];
				ZPIDPair * newStatus = new ZPIDPair();
				ZPTitleStatusDetail *	newStatusDetail = new ZPTitleStatusDetail();
				currStatusList.push_back( newStatus );
				currStatusDetailList.push_back( newStatusDetail );
				
				CopyWCharPtrToPMString( newStatus->mID, currStatusDTO->statusId);
				CopyWCharPtrToPMString( newStatus->mName, currStatusDTO->statusName);

				newStatusDetail->mIsLayout =  currStatusDTO->layout;
				newStatusDetail->mIsText = currStatusDTO->text;
				newStatusDetail->mSortIndex = currStatusDTO->sortIndex;
				CopyWCharPtrToPMString( newStatusDetail->mStateID, currStatusDTO->stateId );
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchEditions
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchAssetList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetAssetList theReqData;
	_ns3__GetAssetListResponse theRespData;

	const CZPFetchAssetListRequest * theRequest = dynamic_cast<const CZPFetchAssetListRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	editionIDBuff(MaxSize_EditionID);
	theReqData.shelveid = editionIDBuff.Get();
	theRequest->GetEditionID().GetWChar_tString(theReqData.shelveid, MaxSize_EditionID);

	const K2Vector<PMString> & inAssetTypes = theRequest->GetAssetTypes();
	
	WSSt_ArrayOfguid typesArray;
	typesArray.__sizeguid = inAssetTypes.size();
	StSmartBuff<wchar_t*>	typesArrayBuff(typesArray.__sizeguid);
	typesArray.guid = typesArrayBuff.Get();
	ZPSmartPtrVector<wchar_t> autoDelTypes;
	
	for (int i = 0; i < typesArray.__sizeguid; ++i)
	{
		const PMString & currAssetType = inAssetTypes[i];
		CreateZeroInitWChar_tArray( typesArray.guid[i], MaxSize_AssetType );
		autoDelTypes.push_back( typesArray.guid[i] );
		currAssetType.GetWChar_tString(typesArray.guid[i], MaxSize_AssetType);
	}
	theReqData.types = &typesArray;

	// get assets for edition
	ZPWSError toReturn = _service.__ns1__GetAssetList( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchAssetListResponse * theResponse = dynamic_cast<CZPFetchAssetListResponse*>(oResponse);
		ASSERT(theResponse);
	
		ZPIDPairSList &		theAssetList = theResponse->GetAssetList();
		ZPAssetDetailSList &	theAssetDetailList = theResponse->GetAssetDetailList();

		WSSt_ArrayOfAssetDTO* assetsdto = theRespData.GetAssetListResult;
		
		//TODO: move this to a function
		ZPIDPair * currAssetInfo = nil;
		ZPAssetDetailInfo * currAssetDetail = nil;

		for (int i=0; i<assetsdto->__sizeAssetDTO; i++)
		{
			WSSt_AssetDTO* asset = assetsdto->AssetDTO[i];
			currAssetInfo = new ZPIDPair;
			currAssetDetail = new ZPAssetDetailInfo;

			if( asset )
				ExtactValues_AssetDTO( *asset, currAssetInfo, *currAssetDetail );

			theAssetList.push_back( currAssetInfo );
			theAssetDetailList.push_back( currAssetDetail );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetAsset theReqData;
	_ns3__GetAssetResponse theRespData;

	const CZPFetchAssetRequest * theRequest = dynamic_cast<const CZPFetchAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	WSSt_ArrayOfguid arrayOfAssetIDs;
	theReqData.assetid = &arrayOfAssetIDs;
	arrayOfAssetIDs.__sizeguid = 1;
	arrayOfAssetIDs.guid = new wchar_t*[arrayOfAssetIDs.__sizeguid];
	StPtrDeleter<wchar_t*>	autoDelArray(arrayOfAssetIDs.guid);
	StBuffForID	assetID(MaxSize_AssetID);
	arrayOfAssetIDs.guid[0] = assetID.Get();

	theRequest->GetAssetID().GetWChar_tString(arrayOfAssetIDs.guid[0], MaxSize_AssetID);

	// get asset info
	ZPWSError toReturn = _service.__ns1__GetAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchAssetResponse * theResponse = dynamic_cast<CZPFetchAssetResponse*>(oResponse);
		ASSERT(theResponse);

		WSSt_ArrayOfAssetDTO * assetDTOArr = theRespData.GetAssetResult;
		for( int i = 0; i < assetDTOArr->__sizeAssetDTO; ++i)
		{
			WSSt_AssetDTO* currAssetDTO = assetDTOArr->AssetDTO[i];
		
			if (currAssetDTO)
			{
				ZPIDPair assetIDName;
				ZPAssetDetailInfo & assetDetailInfo = theResponse->GetDetailInfo();

				ExtactValues_AssetDTO( *currAssetDTO, &assetIDName, assetDetailInfo );
				theResponse->SetName( assetIDName.mName );

				PMString editionID;
				CopyWCharPtrToPMString (editionID, currAssetDTO->shelveid);

				theResponse->SetEditionID( editionID );
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchAssets
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchAssets(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetAsset theReqData;
	_ns3__GetAssetResponse theRespData;

	const CZPFetchAssetsRequest * theRequest = dynamic_cast<const CZPFetchAssetsRequest*> ( inRequest );
	ASSERT( theRequest );

	const K2Vector<PMString> & inAssetIDs = theRequest->GetAssetIDs();
	
	WSSt_ArrayOfguid idsArrInfo;
	idsArrInfo.__sizeguid = inAssetIDs.size();
	StSmartBuff<wchar_t*>	typesArrayBuff(idsArrInfo.__sizeguid);
	idsArrInfo.guid = typesArrayBuff.Get();
	ZPSmartPtrVector<wchar_t> autoDelTypes;
	
	for (int i = 0; i < idsArrInfo.__sizeguid; ++i)
	{
		const PMString & currAssetID = inAssetIDs[i];
		CreateZeroInitWChar_tArray( idsArrInfo.guid[i], MaxSize_AssetID );
		autoDelTypes.push_back( idsArrInfo.guid[i] );
		currAssetID.GetWChar_tString(idsArrInfo.guid[i], MaxSize_AssetID);
	}

	theReqData.assetid = &idsArrInfo;

	ZPWSError toReturn = _service.__ns1__GetAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchAssetsResponse * theResponse = dynamic_cast<CZPFetchAssetsResponse*>(oResponse);
		ASSERT(theResponse);

		ZPAssetInfoSList & outAssets = theResponse->GetAssets();

		WSSt_ArrayOfAssetDTO * assetDTOArr = theRespData.GetAssetResult;
		for( int i = 0; i < assetDTOArr->__sizeAssetDTO; ++i)
		{
			WSSt_AssetDTO* currAssetDTO = assetDTOArr->AssetDTO[i];
		
			if (currAssetDTO)
			{
				ZPAssetInfo * newAssetInfo = new ZPAssetInfo;

				ZPIDPair & assetIDName = newAssetInfo->mAsset;
				ZPAssetDetailInfo & assetDetailInfo = newAssetInfo->mAssetDetailInfo;

				ExtactValues_AssetDTO( *currAssetDTO, &assetIDName, assetDetailInfo );
				
				PMString & editionID = newAssetInfo->mEditionID;
				CopyWCharPtrToPMString (editionID, currAssetDTO->shelveid);

				outAssets.push_back( newAssetInfo );
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoAcquireLock
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoAcquireLock(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__LockAsset theReqData;
	_ns3__LockAssetResponse theRespData;

	const CZPAcquireLockRequest * theRequest = dynamic_cast<const CZPAcquireLockRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	WSSt_LockDTO lockDTO;
	theReqData.lockDTO = &lockDTO;

	StSmartBuff<wchar_t>	appName(50);
	StBuffForID	linkedWithDocID(MaxSize_AssetID);
	StSmartBuff<wchar_t>	linkedWithDocName(100);

	lockDTO.applicationname = appName.Get();
	theRequest->GetAppName().GetWChar_tString( lockDTO.applicationname, 50 );

	if( theRequest->GetLinkedWithDocID().IsEmpty() == false )
	{
		lockDTO.documentid = linkedWithDocID.Get();
		theRequest->GetLinkedWithDocID().GetWChar_tString( lockDTO.documentid, MaxSize_AssetID );
	}
	
	if( theRequest->GetLinkedWithDocID().IsEmpty() == false )
	{
		lockDTO.documentname = linkedWithDocName.Get();
		theRequest->GetLinkedWithDocName().GetWChar_tString( lockDTO.documentname, 100 );
	}

	// Acquire lock for Asset
	ZPWSError toReturn = _service.__ns1__LockAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPAcquireLockResponse * theResponse = dynamic_cast<CZPAcquireLockResponse*>(oResponse);
		ASSERT(theResponse);

		const WSSt_LockDTO * theLockInfo = theRespData.LockAssetResult;
		//store lock info with response.
		if( theLockInfo )
			ExtactValues_LockInfo( *theLockInfo, theResponse->GetLockInfo() );
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoReleaseLock
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoReleaseLock(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);
	
	_ns3__UnlockAsset theReqData;
	_ns3__UnlockAssetResponse theRespData;

	const CZPReleaseLockRequest * theRequest = dynamic_cast<const CZPReleaseLockRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();
	theReqData.forceUnlock = nil;

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	// Release lock for Asset
	ZPWSError toReturn = _service.__ns1__UnlockAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPReleaseLockResponse * theResponse = dynamic_cast<CZPReleaseLockResponse*>(oResponse);
		ASSERT(theResponse);

		const bool unLockResult = theRespData.UnlockAssetResult;
		//TODO: store lock info with unlock response.
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchLockInfo
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchLockInfo(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetLock theReqData;
	_ns3__GetLockResponse theRespData;

	const CZPFetchLockInfoRequest * theRequest = dynamic_cast<const CZPFetchLockInfoRequest*> ( inRequest );
	ASSERT( theRequest );

	const K2Vector<PMString> & inAssetIDs = theRequest->GetAssetIDs();
	
	WSSt_ArrayOfguid idsArrInfo;
	idsArrInfo.__sizeguid = inAssetIDs.size();
	StSmartBuff<wchar_t*>	typesArrayBuff(idsArrInfo.__sizeguid);
	idsArrInfo.guid = typesArrayBuff.Get();
	ZPSmartPtrVector<wchar_t> autoDelTypes;
	
	for (int i = 0; i < idsArrInfo.__sizeguid; ++i)
	{
		const PMString & currAssetID = inAssetIDs[i];
		CreateZeroInitWChar_tArray( idsArrInfo.guid[i], MaxSize_AssetID );
		autoDelTypes.push_back( idsArrInfo.guid[i] );
		currAssetID.GetWChar_tString(idsArrInfo.guid[i], MaxSize_AssetID);
	}

	theReqData.assetids = &idsArrInfo;

	// Acquire lock for Asset
	ZPWSError toReturn = _service.__ns1__GetLock( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPFetchLockInfoResponse * theResponse = dynamic_cast<CZPFetchLockInfoResponse*>(oResponse);
		ASSERT(theResponse);

		ZPAssetLockInfoSList & outLocks = theResponse->GetLockInfoList();

		const WSSt_ArrayOfLockDTO * theLocks = theRespData.GetLockResult;
		if( theLocks && theLocks->__sizeLockDTO > 0)
		{
			for( int i = 0; i < theLocks->__sizeLockDTO ; ++i )
			{
				const WSSt_LockDTO * currLockInfo = theLocks->LockDTO[i];
				//store lock info with response.
				if( currLockInfo )
				{
					ZPAssetLockInfo * newLockInfo = new ZPAssetLockInfo;
					ExtactValues_LockInfo( *currLockInfo, *newLockInfo );
					outLocks.push_back( newLockInfo );
				}
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoCreateAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoCreateAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__CreateAsset theReqData;
	_ns3__CreateAssetResponse theRespData;

	const CZPCreateAssetRequest * theRequest = dynamic_cast<const CZPCreateAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	WSSt_AssetDTO assetDTO;
	//::memset( &assetDTO, 0, sizeof( assetDTO ) );
	theReqData.assetDTO = &assetDTO;

	MemsetZero( assetDTO );
	InitStruct_AssetDTO( assetDTO, theRequest->GetDetailInfo(), kNullString, theRequest->GetEditionID() );
	
	CreateZeroInitWChar_tArray( assetDTO.name, 50 );	//TODO: remove const value
	theRequest->GetAssetName().GetWChar_tString(assetDTO.name, 50 );

	// Create a new Asset
	ZPWSError toReturn = _service.__ns1__CreateAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPCreateAssetResponse * theResponse = dynamic_cast<CZPCreateAssetResponse*>(oResponse);
		ASSERT(theResponse);

		WSSt_AssetDTO* currAssetDTO = theRespData.CreateAssetResult;
		
		if (currAssetDTO)
		{
			ZPIDPair assetIDInfo;
			ZPAssetDetailInfo & assetDetailInfo = theResponse->GetDetailInfo();

			ExtactValues_AssetDTO( *currAssetDTO, &assetIDInfo, assetDetailInfo );
			theResponse->SetAssetID( assetIDInfo.mID );
			theResponse->SetName( assetIDInfo.mName );

			PMString editionID;
			CopyWCharPtrToPMString (editionID, currAssetDTO->shelveid);
			theResponse->SetEditionID( editionID );
		}
	}

	DeInitStruct_AssetDTO( assetDTO );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUpdateAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUpdateAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UpdateAsset theReqData;
	_ns3__UpdateAssetResponse theRespData;

	const CZPUpdateAssetRequest * theRequest = dynamic_cast<const CZPUpdateAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	WSSt_AssetDTO assetDTO;
	//::memset( &assetDTO, 0, sizeof( assetDTO ) );
	theReqData.assetdto = &assetDTO;

	MemsetZero( assetDTO );
	InitStruct_AssetDTO( assetDTO, theRequest->GetDetailInfo(), kNullString, theRequest->GetEditionID());
	
	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();
	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	CreateZeroInitWChar_tArray( assetDTO.name, 50 );	//TODO: remove const value
	theRequest->GetAssetName().GetWChar_tString(assetDTO.name, 50 );

	WSSt_VersionInfo versionInfo;
	InitStruct_VersionInfo( versionInfo, theRequest->GetDetailInfo() );
	
	assetDTO.Head = &versionInfo;

	// Update Asset
	ZPWSError toReturn = _service.__ns1__UpdateAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUpdateAssetResponse * theResponse = dynamic_cast<CZPUpdateAssetResponse*>(oResponse);
		ASSERT(theResponse);

		//WSSt_AssetDTO* currAssetDTO = theRespData.CreateAssetResult;
		//
		//if (currAssetDTO)
		//{
		//	ZPIDPair assetIDInfo;
		//	ZPAssetDetailInfo & assetDetailInfo = theResponse->GetDetailInfo();

		//	ExtactValues_AssetDTO( *currAssetDTO, &assetIDInfo, assetDetailInfo );
		//	theResponse->SetAssetID( assetIDInfo.mID );
		//	theResponse->SetName( assetIDInfo.mName );

		//	PMString editionID;
		//	CopyWCharPtrToPMString (editionID, currAssetDTO->shelveid);
		//	theResponse->SetEditionID( editionID );
		//}
	}

	DeInitStruct_VersionInfo( versionInfo );
	DeInitStruct_AssetDTO( assetDTO );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUpdateAssetName
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUpdateAssetName(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UpdateAssetName theReqData;
	_ns3__UpdateAssetNameResponse theRespData;

	const CZPUpdateAssetNameRequest * theRequest = dynamic_cast<const CZPUpdateAssetNameRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();
	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	int size = theRequest->GetAssetName().CharCount() + 1;
	CreateZeroInitWChar_tArray( theReqData.name, size );
	StPtrDeleter<wchar_t> autoDel_assetName( theReqData.name );
	theRequest->GetAssetName().GetWChar_tString(theReqData.name, size );

	// Update Asset Name
	ZPWSError toReturn = _service.__ns1__UpdateAssetName( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUpdateAssetNameResponse * theResponse = dynamic_cast<CZPUpdateAssetNameResponse*>(oResponse);
		ASSERT(theResponse);
		//theResponse->SetIsSuccessful(true);
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUpdateAssetStatus
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUpdateAssetStatus(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UpdateAssetStatus theReqData;
	_ns3__UpdateAssetStatusResponse theRespData;

	const CZPUpdateAssetStatusRequest * theRequest = dynamic_cast<const CZPUpdateAssetStatusRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();
	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	StBuffForID	assetOldStatusID(MaxSize_AssetID);
	theReqData.oldstatusid = assetOldStatusID.Get();
	theRequest->GetOldStatusID().GetWChar_tString(theReqData.oldstatusid, MaxSize_AssetID);

	StBuffForID	assetNewStatusID(MaxSize_AssetID);
	theReqData.newstatusid = assetNewStatusID.Get();
	theRequest->GetNewStatusID().GetWChar_tString(theReqData.newstatusid, MaxSize_AssetID);

	// Update Asset
	ZPWSError toReturn = _service.__ns1__UpdateAssetStatus( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUpdateAssetStatusResponse * theResponse = dynamic_cast<CZPUpdateAssetStatusResponse*>(oResponse);
		ASSERT(theResponse);
		theResponse->SetIsSuccessful(true);
	}

	return toReturn;
}
//----------------------------------------------------------------------------------------
// DoDeleteAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoDeleteAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__DeleteAsset theReqData;
	_ns3__DeleteAssetResponse theRespData;

	const CZPDeleteAssetRequest * theRequest = dynamic_cast<const CZPDeleteAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	// Delete an asset.
	ZPWSError toReturn = _service.__ns1__DeleteAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPDeleteAssetResponse * theResponse = dynamic_cast<CZPDeleteAssetResponse*>(oResponse);
		ASSERT(theResponse);
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoRelinkAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoRelinkAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);
	
	_ns3__RelinkAsset theReqData;
	_ns3__RelinkAssetResponse theRespData;

	const CZPRelinkAssetRequest * theRequest = dynamic_cast<const CZPRelinkAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	linkedWithAssetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();
	
	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);

	if( theRequest->GetLinkedWithAssetID().IsEmpty() == false )
	{
		theReqData.documentid = linkedWithAssetID.Get();
		theRequest->GetLinkedWithAssetID().GetWChar_tString(theReqData.documentid, MaxSize_AssetID);
	}

	// Release lock for Asset
	ZPWSError toReturn = _service.__ns1__RelinkAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPRelinkAssetResponse * theResponse = dynamic_cast<CZPRelinkAssetResponse*>(oResponse);
		ASSERT(theResponse);

		//const bool * unLockResult = theRespData.R;
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUploadVersion
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUploadVersion(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UploadVersion theReqData;
	_ns3__UploadVersionResponse theRespData;

	const CZPUploadVersionRequest * theRequest = dynamic_cast<const CZPUploadVersionRequest*> ( inRequest );
	ASSERT( theRequest );

	theReqData.versionDto = new WSSt_VersionDTO;
	StPtrDeleter<WSSt_VersionDTO>	autoDelVersionDTO(theReqData.versionDto, true);
	MemsetZero( *theReqData.versionDto );

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	versionID(MaxSize_VersionID);
	theReqData.assetid = assetID.Get();
	theReqData.versionDto->versionId = versionID.Get();
	bool isCompressed = theRequest->GetCompressData();
	theReqData.versionDto->isCompressed = isCompressed;

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);
	theRequest->GetVersionID().GetWChar_tString( theReqData.versionDto->versionId, MaxSize_VersionID );

	// change to mtom with xop
	theReqData.versionDto->data = new xsd__base64Binary();
	StPtrDeleter<xsd__base64Binary> autoDelData( theReqData.versionDto->data , true );

	theReqData.versionDto->data->__ptr = (unsigned char *) theRequest->GetDataBuff();	//TODO: Will soap own this pointer? Otherwise why is it non const?
	theReqData.versionDto->data->__size = theRequest->GetDataLen();
	
	soap_set_omode( _service.soap, SOAP_ENC_MTOM );
	theReqData.versionDto->data->type = "binary";	//Note that the type field must be set to transmit MTOM attachments, otherwise plain base64 XML will be used.
	
	// Upload a version of asset.
	ZPWSError toReturn = _service.__ns1__UploadVersion( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUploadVersionResponse * theResponse = dynamic_cast<CZPUploadVersionResponse*>(oResponse);
		ASSERT(theResponse);
		PMString versionId;
		CopyWCharPtrToPMString (versionId , theRespData.UploadVersionResult);
		theResponse->SetVersionId(versionId);
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUploadExport
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUploadExport(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UploadExport theReqData;
	_ns3__UploadExportResponse theRespData;

	const CZPUploadExportRequest * theRequest = dynamic_cast<const CZPUploadExportRequest*> ( inRequest );
	ASSERT( theRequest );

	//theReqData.versionDto = new WSSt_VersionDTO;
	//StPtrDeleter<WSSt_VersionDTO>	autoDelVersionDTO(theReqData.versionDto, true);
	//MemsetZero( *theReqData.versionDto );

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	versionID(MaxSize_VersionID);
	theReqData.assetid = assetID.Get();
	theReqData.versionid = versionID.Get();
	bool isCompressed = theRequest->GetCompressData();
	theReqData.isCompressed = isCompressed;
	theReqData.isXML = theRequest->GetIsXML();
	theReqData.isHighRes = theRequest->GetIsHighRes();
	const PMString & imageFileName = theRequest->GetImageFileName();
	int fileNameSize = imageFileName.CharCount();
	StSmartBuff<wchar_t>	fileNameBuff(fileNameSize + 1);
	if (fileNameSize)
	{
		theReqData.imageFileName = fileNameBuff.Get();
		imageFileName.GetWChar_tString(theReqData.imageFileName , fileNameSize + 1);
	}
	else
		theReqData.imageFileName = nil;

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);
	theRequest->GetVersionID().GetWChar_tString( theReqData.versionid, MaxSize_VersionID );

	// change to mtom with xop
	theReqData.data = new xsd__base64Binary();
	StPtrDeleter<xsd__base64Binary> autoDelData( theReqData.data , true );

	theReqData.data->__ptr = (unsigned char *) theRequest->GetDataBuff();	//TODO: Will soap own this pointer? Otherwise why is it non const?
	theReqData.data->__size = theRequest->GetDataLen();
	
	soap_set_omode( _service.soap, SOAP_ENC_MTOM );
	theReqData.data->type = "binary";	//Note that the type field must be set to transmit MTOM attachments, otherwise plain base64 XML will be used.
	
	// Upload a version of asset.
	ZPWSError toReturn = _service.__ns1__UploadExport( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUploadExportResponse * theResponse = dynamic_cast<CZPUploadExportResponse*>(oResponse);
		ASSERT(theResponse);
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUploadPDF
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUploadPDF(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UploadPDF theReqData;
	_ns3__UploadPDFResponse theRespData;

	const CZPUploadPDFRequest * theRequest = dynamic_cast<const CZPUploadPDFRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	versionID(MaxSize_VersionID);
	theReqData.assetid = assetID.Get();
	theReqData.versionid = versionID.Get();
	
	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);
	theRequest->GetVersionID().GetWChar_tString( theReqData.versionid, MaxSize_VersionID );

	// change to mtom with xop
	theReqData.data = new xsd__base64Binary();
	StPtrDeleter<xsd__base64Binary> autoDelData( theReqData.data , true );

	theReqData.data->__ptr = (unsigned char *) theRequest->GetDataBuff();	//TODO: Will soap own this pointer? Otherwise why is it non const?
	theReqData.data->__size = theRequest->GetDataLen();
	
	soap_set_omode( _service.soap, SOAP_ENC_MTOM );
	theReqData.data->type = "application/pdf";	//Note that the type field must be set to transmit MTOM attachments, otherwise plain base64 XML will be used.
	
	// Upload a version of asset.
	ZPWSError toReturn = _service.__ns1__UploadPDF( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUploadPDFResponse * theResponse = dynamic_cast<CZPUploadPDFResponse*>(oResponse);
		ASSERT(theResponse);
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUploadPagePreview
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUploadPagePreview(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UploadPage theReqData;
	_ns3__UploadPageResponse theRespData;

	const CZPUploadPagePreviewRequest * theRequest = dynamic_cast<const CZPUploadPagePreviewRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	versionID(MaxSize_VersionID);
	StBuffForID	pageID(MaxSize_PageID);
	StBuffForID	pageTypeID(MaxSize_PageTypeID);

	//theReqData.assetid = assetID.Get();
	theReqData.versionid = versionID.Get();
	theReqData.pageid = pageID.Get();
	theReqData.pagetypeid = pageTypeID.Get();
	
	bool isCompressed = false;
	int pageNumber = theRequest->GetPageNumber();
	theReqData.isCompressed = isCompressed;
	theReqData.number = pageNumber;

	//theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);
	theRequest->GetVersionID().GetWChar_tString( theReqData.versionid, MaxSize_VersionID );
	theRequest->GetPageID().GetWChar_tString( theReqData.pageid, MaxSize_PageID );
	theRequest->GetPageTypeID().GetWChar_tString( theReqData.pagetypeid, MaxSize_PageTypeID );

	// change to mtom with xop
	theReqData.data = new xsd__base64Binary();
	StPtrDeleter<xsd__base64Binary> autoDelData( theReqData.data , true );

	theReqData.data->__ptr = (unsigned char *) theRequest->GetDataBuff();	//TODO: Will soap own this pointer? Otherwise why is it non const?
	theReqData.data->__size = theRequest->GetDataLen();
	
	soap_set_omode( _service.soap, SOAP_ENC_MTOM );
	theReqData.data->type = "image/jpg";	//Note that the type field must be set to transmit MTOM attachments, otherwise plain base64 XML will be used.
	
	// Upload a version of asset.
	ZPWSError toReturn = _service.__ns1__UploadPage( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUploadPagePreviewResponse * theResponse = dynamic_cast<CZPUploadPagePreviewResponse*>(oResponse);
		ASSERT(theResponse);
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoDownloadVersion
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoDownloadVersion(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__DownloadVersion theReqData;
	_ns3__DownloadVersionResponse theRespData;

	const CZPDownloadVersionRequest * theRequest = dynamic_cast<const CZPDownloadVersionRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	versionID(MaxSize_VersionID);
	theReqData.versionid = versionID.Get();

	theRequest->GetVersionID().GetWChar_tString( theReqData.versionid, MaxSize_VersionID );
	
	//if( _service.soap->version == 2 )
	soap_set_imode( _service.soap, SOAP_ENC_MTOM );	//Required for soap 1.2

	// download the given version
	ZPWSError toReturn = _service.__ns1__DownloadVersion( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPDownloadVersionResponse * theResponse = dynamic_cast<CZPDownloadVersionResponse*>(oResponse);
		ASSERT(theResponse);

		ASSERT(theRespData.DownloadVersionResult);
		xsd__base64Binary* downloadVersionResult = theRespData.DownloadVersionResult->data;
		
		
		//if( theRequest->GetNeedDecompression() )
		if( theRespData.DownloadVersionResult->isCompressed && (theRespData.DownloadVersionResult->isCompressed) )
		{
			unsigned int targetLen = 0;
			uchar * targetBuff = nil;
			Utils<IZPCompressionUtils>()->Decompress(downloadVersionResult->__ptr, downloadVersionResult->__size, &targetBuff, &targetLen);
		
			if (Utils<IZPFileUtils>()->WriteFile( theRequest->GetOutputFile(), targetBuff, targetLen)!= kSuccess)
			{	//TODO: set error with response.
			}
			if( targetBuff )
				free( targetBuff );
		}
		else
		{
			if (Utils<IZPFileUtils>()->WriteFile( theRequest->GetOutputFile(), downloadVersionResult->__ptr, downloadVersionResult->__size )!= kSuccess)
			{	//TODO: set error with response.
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoDownloadThumbnail
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoDownloadThumbnail(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetThumbnail theReqData;
	_ns3__GetThumbnailResponse theRespData;

	const CZPDownloadThumbnailRequest * theRequest = dynamic_cast<const CZPDownloadThumbnailRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	theReqData.assetid = assetID.Get();

	theRequest->GetAssetID().GetWChar_tString( theReqData.assetid, MaxSize_AssetID );

	//if( _service.soap->version == 2 )
	soap_set_imode( _service.soap, SOAP_ENC_MTOM );	//Required for soap 1.2

	// download the given version
	ZPWSError toReturn = _service.__ns1__GetThumbnail( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPDownloadThumbnailResponse * theResponse = dynamic_cast<CZPDownloadThumbnailResponse*>(oResponse);
		ASSERT(theResponse);

		//ASSERT(theRespData.GetThumbnailResult);
		xsd__base64Binary* downloadedThumbnailInfo = theRespData.GetThumbnailResult;
		
		
		//if( theRequest->GetNeedDecompression() )
#if 0
		if( theRespData.GetThumbnailResult->isCompressed && *(theRespData.GetThumbnailResult->isCompressed) )
		{
			unsigned int targetLen = 0;
			uchar * targetBuff = nil;
			Utils<IZPCompressionUtils>()->Decompress(downloadedThumbnailInfo->__ptr, downloadedThumbnailInfo->__size, &targetBuff, &targetLen);
		
			if (Utils<IZPFileUtils>()->WriteFile( theRequest->GetOutputFile(), targetBuff, targetLen)!= kSuccess)
			{	//TODO: set error with response.
			}
		}
		else
#endif
		{
			Utils<IZPFileUtils> fileUtils;
#ifdef CacheWithFolder
			PMString assetId;
			fileUtils->GetParentDirectoryName(theRequest->GetOutputFile(), assetId);
			Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(assetId);
#endif
			if ( !downloadedThumbnailInfo || fileUtils->WriteFile( theRequest->GetOutputFile(), downloadedThumbnailInfo->__ptr, downloadedThumbnailInfo->__size )!= kSuccess)
			{	//TODO: set error with response.
			}
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoCanUpdateAssetName
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoCanUpdateAssetName(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__CanUpdateAssetName theReqData;
	_ns3__CanUpdateAssetNameResponse theRespData;

	const CZPCanUpdateAssetNameRequest * theRequest = dynamic_cast<const CZPCanUpdateAssetNameRequest*> ( inRequest );
	ASSERT( theRequest );

	StBuffForID	assetID(MaxSize_AssetID);
	if( theRequest->GetAssetID() != kNullString )
	{
		theReqData.assetId = assetID.Get();
		theRequest->GetAssetID().GetWChar_tString(theReqData.assetId, MaxSize_AssetID);
	}
	else
		theReqData.assetId = nil;

	StBuffForID	editionID(MaxSize_AssetID);
	if( theRequest->GetEditionID() != kNullString )
	{
		theReqData.editionId = editionID.Get();
		theRequest->GetEditionID().GetWChar_tString(theReqData.editionId, MaxSize_AssetID);
	}
	else
		theReqData.editionId = nil;

	StBuffForID	typeID(MaxSize_AssetID);
	if( theRequest->GetTypeID() != kNullString )
	{
		theReqData.typeId = typeID.Get();
		theRequest->GetTypeID().GetWChar_tString(theReqData.typeId, MaxSize_AssetID);
	}
	else
		theReqData.typeId = nil;

	StBuffForID	parentID(MaxSize_AssetID);
	if( theRequest->GetParentID() != kNullString )
	{
		theReqData.parentId = parentID.Get();
		theRequest->GetParentID().GetWChar_tString(theReqData.parentId, MaxSize_AssetID);
	}
	else
		theReqData.parentId = nil;

	int size = theRequest->GetAssetName().CharCount() + 1;
	CreateZeroInitWChar_tArray( theReqData.name, size );
	StPtrDeleter<wchar_t> autoDel_assetName( theReqData.name );
	theRequest->GetAssetName().GetWChar_tString(theReqData.name, size );

	// Update Asset Name
	ZPWSError toReturn = _service.__ns1__CanUpdateAssetName( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPCanUpdateAssetNameResponse * theResponse = dynamic_cast<CZPCanUpdateAssetNameResponse*>(oResponse);
		ASSERT(theResponse);
		if(theRespData.CanUpdateAssetNameResult)
			theResponse->SetIsSuccessful(true);
	}

	return toReturn;
}
//----------------------------------------------------------------------------------------
// DoCheckInAsset
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoCheckInAsset(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__CheckInAsset theReqData;
	_ns3__CheckInAssetResponse theRespData;

	const CZPCheckInAssetRequest * theRequest = dynamic_cast<const CZPCheckInAssetRequest*> ( inRequest );
	ASSERT( theRequest );

	int commentsLen = theRequest->GetComment().ByteLength();
	if( commentsLen )
		commentsLen += 1;

	StBuffForID	assetID(MaxSize_AssetID);
	StBuffForID	versionID(MaxSize_VersionID);
	StBuffForID	statusID(MaxSize_StatusID);
	StBuffForID	checkinComment(commentsLen);

	theReqData.assetid = assetID.Get();
	theReqData.versionId = versionID.Get();
	theReqData.statusid = statusID.Get();
	int headVersion = theRequest->GetHeadVersion();	//Used to make sure that check-in is being made after updating to head.
	theReqData.headVersion = headVersion;
	
	if( commentsLen )
		theReqData.comment = checkinComment.Get();
	else
		theReqData.comment = nil;

	theRequest->GetAssetID().GetWChar_tString(theReqData.assetid, MaxSize_AssetID);
	theRequest->GetNewVersionID().GetWChar_tString(theReqData.versionId, MaxSize_VersionID);
	theRequest->GetStatusID().GetWChar_tString(theReqData.statusid, MaxSize_StatusID);
	if(commentsLen)
		theRequest->GetComment().GetWChar_tString(theReqData.comment, commentsLen);

	// Check-in the asset
	ZPWSError toReturn = _service.__ns1__CheckInAsset( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPCheckInAssetResponse * theResponse = dynamic_cast<CZPCheckInAssetResponse*>(oResponse);
		ASSERT(theResponse);
	}

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ExtactValues_AssetDTO
//----------------------------------------------------------------------------------------
void
ExtactValues_AssetDTO(
	const WSSt_AssetDTO &			inAssetDTO,
	ZPIDPair *						oAssetInfo,
	ZPAssetDetailInfo &				oAssetDetail)
{
	CopyWCharPtrToPMString (oAssetInfo->mID , inAssetDTO.assetid);
	CopyWCharPtrToPMString (oAssetInfo->mName, inAssetDTO.name);
	
	CopyWCharPtrToPMString (oAssetDetail.mDocInfo.mID, inAssetDTO.documentid);
	CopyWCharPtrToPMString (oAssetDetail.mDocInfo.mName, inAssetDTO.documentname);

	//if (asset.titleid)
	//	CopyWCharPtrToPMString( izpasset.titleid, inAssetDTO.titleid);
	//
	//if (asset.shelveid)
	//	CopyWCharPtrToPMString( izpasset.shelveid, inAssetDTO.shelveid);

	//CopyWCharPtrToPMString (oAssetDetail.mLockID, inAssetDTO.lockid);
	CopyWCharPtrToPMString (oAssetDetail.mTypeID, inAssetDTO.typeid_);
	CopyWCharPtrToPMString (oAssetDetail.mType, inAssetDTO.type);
	
	if( inAssetDTO.Head )
	{
		ExtactValues_VersionInfo( *inAssetDTO.Head, oAssetDetail );
	}
	else
	{
		oAssetDetail.mVersionDate = 0;
		oAssetDetail.mVersionNumber = 0;
	}
}

//----------------------------------------------------------------------------------------
// ExtactValues_VersionInfo
//----------------------------------------------------------------------------------------
void
ExtactValues_VersionInfo(
	const WSSt_VersionInfo &		inVersionInfo,
	ZPAssetDetailInfo &				oAssetDetail)
{
	CopyWCharPtrToPMString (oAssetDetail.mStatusID, inVersionInfo.statusid);
	CopyWCharPtrToPMString (oAssetDetail.mVersionID, inVersionInfo.versionid);
	if( inVersionInfo.date )
		oAssetDetail.mVersionDate = *(inVersionInfo.date);
	else
		oAssetDetail.mVersionDate = 0;

	if( inVersionInfo.number )
		oAssetDetail.mVersionNumber = *(inVersionInfo.number);
	else
		oAssetDetail.mVersionNumber = 0;

	if( inVersionInfo.user )
	{
		CopyWCharPtrToPMString (oAssetDetail.mVersionUser.mID, inVersionInfo.user->userid);
		CopyWCharPtrToPMString (oAssetDetail.mVersionUser.mName, inVersionInfo.user->name);
	}
}

//----------------------------------------------------------------------------------------
// ExtactValues_LockInfo
//----------------------------------------------------------------------------------------
void
ExtactValues_LockInfo(
	const WSSt_LockDTO &			inLockInfo,
	ZPAssetLockInfo &				oAssetLockInfo)
{
	CopyWCharPtrToPMString( oAssetLockInfo.mAssetID, inLockInfo.assetid );
	CopyWCharPtrToPMString( oAssetLockInfo.mLockID, inLockInfo.lockid );
	CopyWCharPtrToPMString( oAssetLockInfo.mDocumentID, inLockInfo.documentid );
	CopyWCharPtrToPMString( oAssetLockInfo.mDocumentName, inLockInfo.documentname );
	CopyWCharPtrToPMString( oAssetLockInfo.mApplicationName, inLockInfo.applicationname );
	CopyWCharPtrToPMString( oAssetLockInfo.mUserID, inLockInfo.userid );
	CopyWCharPtrToPMString( oAssetLockInfo.mUserName, inLockInfo.username );
}

//----------------------------------------------------------------------------------------
// MemsetZero
//----------------------------------------------------------------------------------------
void
MemsetZero(
WSSt_AssetDTO &				inAssetDTO)
{
	inAssetDTO.assetid = nil;
	inAssetDTO.documentid = nil;
	inAssetDTO.documentname = nil;
	inAssetDTO.lockid = nil;
	inAssetDTO.name = nil;
	inAssetDTO.shelveid = nil;
	inAssetDTO.titleid = nil;
	inAssetDTO.type = nil;
	inAssetDTO.typeid_ = nil;
	inAssetDTO.Head = nil;
}

//----------------------------------------------------------------------------------------
// MemsetZero
//----------------------------------------------------------------------------------------
void
MemsetZero(
	WSSt_VersionInfo &			inVersionInfo)
{
	inVersionInfo.date = nil;
	inVersionInfo.number = nil;
	inVersionInfo.versionid = nil;
	inVersionInfo.statusid = nil;
	inVersionInfo.user = nil;
}

//----------------------------------------------------------------------------------------
// MemsetZero
//----------------------------------------------------------------------------------------
void
MemsetZero(
	WSSt_VersionDTO &			inVersionDTO)
{
	inVersionDTO.data = nil;
	inVersionDTO.isCompressed = nil;
	inVersionDTO.versionId = nil;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitStruct_AssetDTO
//----------------------------------------------------------------------------------------
void
InitStruct_AssetDTO(
	WSSt_AssetDTO &					oAssetDTO,
	const ZPAssetDetailInfo &		inAssetDetail,
	const PMString &				inTitleID,
	const PMString &				inEditionID )
{
	if (!inAssetDetail.mDocInfo.mID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.documentid, MaxSize_AssetID );
		inAssetDetail.mDocInfo.mID.GetWChar_tString(oAssetDTO.documentid, MaxSize_AssetID);
	}

	if (!inAssetDetail.mDocInfo.mName.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.documentname, 50 );
		inAssetDetail.mDocInfo.mName.GetWChar_tString(oAssetDTO.documentname, 50);
	}
	
	if (!inTitleID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.titleid, MaxSize_TitleID );
		inTitleID.GetWChar_tString(oAssetDTO.titleid, MaxSize_TitleID );
	}
	
	if (!inEditionID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.shelveid, MaxSize_EditionID );
		inEditionID.GetWChar_tString(oAssetDTO.shelveid, MaxSize_EditionID );
	}
	
	//if (!inAssetDetail.mLockID.IsEmpty()) {
	//	CreateZeroInitWChar_tArray( oAssetDTO.lockid, 40 );
	//	inAssetDetail.mLockID.GetWChar_tString(oAssetDTO.lockid, 40);
	//}

	if (!inAssetDetail.mTypeID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.typeid_, 40 );
		inAssetDetail.mTypeID.GetWChar_tString(oAssetDTO.typeid_, 40);
	}
	
	if (!inAssetDetail.mType.IsEmpty()) {
		CreateZeroInitWChar_tArray( oAssetDTO.type, 50 );
		inAssetDetail.mType.GetWChar_tString(oAssetDTO.type, 50);
	}
}

//----------------------------------------------------------------------------------------
// InitStruct_VersionInfo
//----------------------------------------------------------------------------------------
void
InitStruct_VersionInfo(
	WSSt_VersionInfo &				oVersionInfo,
	const ZPAssetDetailInfo &		inAssetDetail)
{
	if (!inAssetDetail.mStatusID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oVersionInfo.statusid, MaxSize_StatusID );
		inAssetDetail.mStatusID.GetWChar_tString(oVersionInfo.statusid, MaxSize_StatusID);
	}

	if (!inAssetDetail.mVersionID.IsEmpty()) {
		CreateZeroInitWChar_tArray( oVersionInfo.versionid, MaxSize_VersionID );
		inAssetDetail.mVersionID.GetWChar_tString(oVersionInfo.versionid, MaxSize_VersionID);
	}

	//Moved to userDTO
	//if (!inAssetDetail.mUserInfo.mID.IsEmpty()) {
	//	CreateZeroInitWChar_tArray( oAssetDTO->userid, 40 );
	//	inAssetDetail.mUserInfo.mID.GetWChar_tString(oAssetDTO->userid, 40);
	//}
	//
	//if (!inAssetDetail.mUserInfo.mName.IsEmpty()) {
	//CreateZeroInitWChar_tArray( oAssetDTO->username, 50 );
	//	inAssetDetail.mUserInfo.mName.GetWChar_tString(oAssetDTO->username, 50);
	//}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// DeInitStruct_AssetDTO
//----------------------------------------------------------------------------------------
void
DeInitStruct_AssetDTO(
	WSSt_AssetDTO &					oAssetDTO)
{
	if (oAssetDTO.assetid)
		delete [] oAssetDTO.assetid;
	oAssetDTO.assetid = nil;
	
	if (oAssetDTO.name)
		delete [] oAssetDTO.name;
	oAssetDTO.name = nil;
	
	if (oAssetDTO.documentid)
		delete [] oAssetDTO.documentid;
	oAssetDTO.documentid = nil;
	
	if (oAssetDTO.documentname)
		delete [] oAssetDTO.documentname;
	oAssetDTO.documentname = nil;
	
	if (oAssetDTO.titleid)
		delete [] oAssetDTO.titleid;
	oAssetDTO.titleid = nil;
	
	if (oAssetDTO.shelveid)
		delete [] oAssetDTO.shelveid;
	oAssetDTO.shelveid = nil;
	
	if (oAssetDTO.lockid)
		delete [] oAssetDTO.lockid;
	oAssetDTO.lockid = nil;
	
	if (oAssetDTO.typeid_)
		delete [] oAssetDTO.typeid_;
	oAssetDTO.typeid_ = nil;
	
	if (oAssetDTO.type)
		delete [] oAssetDTO.type;
	oAssetDTO.type = nil;

}

//----------------------------------------------------------------------------------------
// DeInitStruct_VersionInfo
//----------------------------------------------------------------------------------------
void
DeInitStruct_VersionInfo(
	WSSt_VersionInfo &				ioVersionInfo)
{
	if (ioVersionInfo.versionid)
		delete [] ioVersionInfo.versionid;
	ioVersionInfo.versionid = nil;

	if( ioVersionInfo.statusid )
		delete [] ioVersionInfo.statusid;
	ioVersionInfo.statusid = nil;

}
