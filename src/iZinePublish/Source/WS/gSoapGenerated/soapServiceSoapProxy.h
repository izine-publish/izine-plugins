/* soapServiceSoapProxy.h
   Generated by gSOAP 2.7.12 from service.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapServiceSoapProxy_H
#define soapServiceSoapProxy_H
#include "soapH.h"
class ServiceSoap
{   public:
	/// Runtime engine context allocated in constructor
	struct soap *soap;
	/// Endpoint URL of service 'ServiceSoap' (change as needed)
	const char *endpoint;
	/// Constructor allocates soap engine context, sets default endpoint URL, and sets namespace mapping table
	ServiceSoap()
	{ soap = soap_new(); endpoint = "http://localhost/izine/service.asmx"; if (soap && !soap->namespaces) { static const struct Namespace namespaces[] = 
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns2", "http://microsoft.com/wsdl/types/", NULL, NULL},
	{"ns3", "http://izine-publish.net/server/ServiceSoap", NULL, NULL},
	{"ns1", "http://izine-publish.net/server/", NULL, NULL},
	{"ns4", "http://izine-publish.net/server/ServiceSoap12", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap->namespaces = namespaces; } };
	/// Destructor frees deserialized data and soap engine context
	virtual ~ServiceSoap() { if (soap) { soap_destroy(soap); soap_end(soap); soap_free(soap); } };
	/// Invoke 'Ping' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__Ping(_ns1__Ping *ns1__Ping, _ns1__PingResponse *ns1__PingResponse) { return soap ? soap_call___ns3__Ping(soap, endpoint, NULL, ns1__Ping, ns1__PingResponse) : SOAP_EOM; };
	/// Invoke 'GetStatusList' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetStatusList(_ns1__GetStatusList *ns1__GetStatusList, _ns1__GetStatusListResponse *ns1__GetStatusListResponse) { return soap ? soap_call___ns3__GetStatusList(soap, endpoint, NULL, ns1__GetStatusList, ns1__GetStatusListResponse) : SOAP_EOM; };
	/// Invoke 'GetTitleList' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetTitleList(_ns1__GetTitleList *ns1__GetTitleList, _ns1__GetTitleListResponse *ns1__GetTitleListResponse) { return soap ? soap_call___ns3__GetTitleList(soap, endpoint, NULL, ns1__GetTitleList, ns1__GetTitleListResponse) : SOAP_EOM; };
	/// Invoke 'GetRole' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetRole(_ns1__GetRole *ns1__GetRole, _ns1__GetRoleResponse *ns1__GetRoleResponse) { return soap ? soap_call___ns3__GetRole(soap, endpoint, NULL, ns1__GetRole, ns1__GetRoleResponse) : SOAP_EOM; };
	/// Invoke 'GetCommentList' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetCommentList(_ns1__GetCommentList *ns1__GetCommentList, _ns1__GetCommentListResponse *ns1__GetCommentListResponse) { return soap ? soap_call___ns3__GetCommentList(soap, endpoint, NULL, ns1__GetCommentList, ns1__GetCommentListResponse) : SOAP_EOM; };
	/// Invoke 'GetTags' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetTags(_ns1__GetTags *ns1__GetTags, _ns1__GetTagsResponse *ns1__GetTagsResponse) { return soap ? soap_call___ns3__GetTags(soap, endpoint, NULL, ns1__GetTags, ns1__GetTagsResponse) : SOAP_EOM; };
	/// Invoke 'GetAssetList' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetAssetList(_ns1__GetAssetList *ns1__GetAssetList, _ns1__GetAssetListResponse *ns1__GetAssetListResponse) { return soap ? soap_call___ns3__GetAssetList(soap, endpoint, NULL, ns1__GetAssetList, ns1__GetAssetListResponse) : SOAP_EOM; };
	/// Invoke 'GetShelveList' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetShelveList(_ns1__GetShelveList *ns1__GetShelveList, _ns1__GetShelveListResponse *ns1__GetShelveListResponse) { return soap ? soap_call___ns3__GetShelveList(soap, endpoint, NULL, ns1__GetShelveList, ns1__GetShelveListResponse) : SOAP_EOM; };
	/// Invoke 'Logoff' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__Logoff(_ns1__Logoff *ns1__Logoff, _ns1__LogoffResponse *ns1__LogoffResponse) { return soap ? soap_call___ns3__Logoff(soap, endpoint, NULL, ns1__Logoff, ns1__LogoffResponse) : SOAP_EOM; };
	/// Invoke 'Login' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__Login(_ns1__Login *ns1__Login, _ns1__LoginResponse *ns1__LoginResponse) { return soap ? soap_call___ns3__Login(soap, endpoint, NULL, ns1__Login, ns1__LoginResponse) : SOAP_EOM; };
	/// Invoke 'GetUser' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetUser(_ns1__GetUser *ns1__GetUser, _ns1__GetUserResponse *ns1__GetUserResponse) { return soap ? soap_call___ns3__GetUser(soap, endpoint, NULL, ns1__GetUser, ns1__GetUserResponse) : SOAP_EOM; };
	/// Invoke 'GetThumbnail' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetThumbnail(_ns1__GetThumbnail *ns1__GetThumbnail, _ns1__GetThumbnailResponse *ns1__GetThumbnailResponse) { return soap ? soap_call___ns3__GetThumbnail(soap, endpoint, NULL, ns1__GetThumbnail, ns1__GetThumbnailResponse) : SOAP_EOM; };
	/// Invoke 'Lock' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__Lock(_ns1__Lock *ns1__Lock, _ns1__LockResponse *ns1__LockResponse) { return soap ? soap_call___ns3__Lock(soap, endpoint, NULL, ns1__Lock, ns1__LockResponse) : SOAP_EOM; };
	/// Invoke 'Unlock' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__Unlock(_ns1__Unlock *ns1__Unlock, _ns1__UnlockResponse *ns1__UnlockResponse) { return soap ? soap_call___ns3__Unlock(soap, endpoint, NULL, ns1__Unlock, ns1__UnlockResponse) : SOAP_EOM; };
	/// Invoke 'GetLock' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetLock(_ns1__GetLock *ns1__GetLock, _ns1__GetLockResponse *ns1__GetLockResponse) { return soap ? soap_call___ns3__GetLock(soap, endpoint, NULL, ns1__GetLock, ns1__GetLockResponse) : SOAP_EOM; };
	/// Invoke 'DeleteAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__DeleteAsset(_ns1__DeleteAsset *ns1__DeleteAsset, _ns1__DeleteAssetResponse *ns1__DeleteAssetResponse) { return soap ? soap_call___ns3__DeleteAsset(soap, endpoint, NULL, ns1__DeleteAsset, ns1__DeleteAssetResponse) : SOAP_EOM; };
	/// Invoke 'NewId' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__NewId(_ns1__NewId *ns1__NewId, _ns1__NewIdResponse *ns1__NewIdResponse) { return soap ? soap_call___ns3__NewId(soap, endpoint, NULL, ns1__NewId, ns1__NewIdResponse) : SOAP_EOM; };
	/// Invoke 'CreateAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__CreateAsset(_ns1__CreateAsset *ns1__CreateAsset, _ns1__CreateAssetResponse *ns1__CreateAssetResponse) { return soap ? soap_call___ns3__CreateAsset(soap, endpoint, NULL, ns1__CreateAsset, ns1__CreateAssetResponse) : SOAP_EOM; };
	/// Invoke 'UpdateAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__UpdateAsset(_ns1__UpdateAsset *ns1__UpdateAsset, _ns1__UpdateAssetResponse *ns1__UpdateAssetResponse) { return soap ? soap_call___ns3__UpdateAsset(soap, endpoint, NULL, ns1__UpdateAsset, ns1__UpdateAssetResponse) : SOAP_EOM; };
	/// Invoke 'UpdateAssetName' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__UpdateAssetName(_ns1__UpdateAssetName *ns1__UpdateAssetName, _ns1__UpdateAssetNameResponse *ns1__UpdateAssetNameResponse) { return soap ? soap_call___ns3__UpdateAssetName(soap, endpoint, NULL, ns1__UpdateAssetName, ns1__UpdateAssetNameResponse) : SOAP_EOM; };
	/// Invoke 'GetAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__GetAsset(_ns1__GetAsset *ns1__GetAsset, _ns1__GetAssetResponse *ns1__GetAssetResponse) { return soap ? soap_call___ns3__GetAsset(soap, endpoint, NULL, ns1__GetAsset, ns1__GetAssetResponse) : SOAP_EOM; };
	/// Invoke 'UploadVersion' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__UploadVersion(_ns1__UploadVersion *ns1__UploadVersion, _ns1__UploadVersionResponse *ns1__UploadVersionResponse) { return soap ? soap_call___ns3__UploadVersion(soap, endpoint, NULL, ns1__UploadVersion, ns1__UploadVersionResponse) : SOAP_EOM; };
	/// Invoke 'DownloadVersion' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__DownloadVersion(_ns1__DownloadVersion *ns1__DownloadVersion, _ns1__DownloadVersionResponse *ns1__DownloadVersionResponse) { return soap ? soap_call___ns3__DownloadVersion(soap, endpoint, NULL, ns1__DownloadVersion, ns1__DownloadVersionResponse) : SOAP_EOM; };
	/// Invoke 'UnlinkAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__UnlinkAsset(_ns1__UnlinkAsset *ns1__UnlinkAsset, _ns1__UnlinkAssetResponse *ns1__UnlinkAssetResponse) { return soap ? soap_call___ns3__UnlinkAsset(soap, endpoint, NULL, ns1__UnlinkAsset, ns1__UnlinkAssetResponse) : SOAP_EOM; };
	/// Invoke 'CheckInAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__CheckInAsset(_ns1__CheckInAsset *ns1__CheckInAsset, _ns1__CheckInAssetResponse *ns1__CheckInAssetResponse) { return soap ? soap_call___ns3__CheckInAsset(soap, endpoint, NULL, ns1__CheckInAsset, ns1__CheckInAssetResponse) : SOAP_EOM; };
	/// Invoke 'RelinkAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__RelinkAsset(_ns1__RelinkAsset *ns1__RelinkAsset, _ns1__RelinkAssetResponse *ns1__RelinkAssetResponse) { return soap ? soap_call___ns3__RelinkAsset(soap, endpoint, NULL, ns1__RelinkAsset, ns1__RelinkAssetResponse) : SOAP_EOM; };
	/// Invoke 'CheckOutAsset' of service 'ServiceSoap' and return error code (or SOAP_OK)
	virtual int __ns3__CheckOutAsset(_ns1__CheckOutAsset *ns1__CheckOutAsset, _ns1__CheckOutAssetResponse *ns1__CheckOutAssetResponse) { return soap ? soap_call___ns3__CheckOutAsset(soap, endpoint, NULL, ns1__CheckOutAsset, ns1__CheckOutAssetResponse) : SOAP_EOM; };
};
#endif