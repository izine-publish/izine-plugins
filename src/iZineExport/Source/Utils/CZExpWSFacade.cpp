//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpWSFacade.cpp $
//	$Revision: 4018 $
//	$Date: 2012-05-23 11:10:24 +0200 (Wed, 23 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 7-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IWorkspace.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "FileUtils.h"
#include "K2Vector.tpp"
#include "Utils.h"

//OS / ThirdParty includes
extern "C"
{
#include "curl/curl.h"
}
#include "stdio.h"
#include "tinyxml.h"
#include "openssl/MD5.h"
#include "IZExpID.h"

//IZP Interfaces
#include "IZExpFileUtils.h"
#include "IZExpGlobalData.h"
#include "IZExpPrefs.h"
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

	ZExpWSError				ValidateMediaContent(
								const AZExpWSRequest *	inReq,
								AZExpWSResponse *		oResp);

	ZExpWSError				GetConfig(
								const AZExpWSRequest *	inReq,
								AZExpWSResponse *		oResp);

protected:
	void					SetProxy(
								CURL *					ioCurl );
private:
};

#pragma mark -
//Function to read data from buffer according to size
static int writer (char * data, size_t size, size_t nmemb, std:: string * buffer) 
{
	int result = 0;
	if (buffer)
	{
		buffer-> append (data, size * nmemb);	// Append the data to the buffer 
		
		result = size * nmemb;
	}

	return result; 
}

size_t fileReadfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE*)stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}

struct curlDataReadHelper
{
	const char * currentPtr;
	const char * endPtr;
};

size_t readData(void *buffer, size_t size, size_t nmemb, void *data)
{
	if (data == 0)
		return 0;

	curlDataReadHelper* dataObj = (curlDataReadHelper *)data;
	if (dataObj->currentPtr < dataObj->endPtr)
	{
		if (nmemb > (dataObj->endPtr - dataObj->currentPtr))
			nmemb = (dataObj->endPtr - dataObj->currentPtr);
	}
	else
		return 0;

	const char *str = (const char *)dataObj->currentPtr;
	strncpy((char*)buffer, str, nmemb);
	dataObj->currentPtr = dataObj->currentPtr + nmemb;
	return size * nmemb;
}
#pragma mark -

class StCleanupCurl
{
public:
	StCleanupCurl( CURL * inCurl )
		: mCurl ( inCurl )
	{ }
	~StCleanupCurl()
	{
		if( mCurl )
			curl_easy_cleanup( mCurl );
	}
private:
	CURL * mCurl;
};

#pragma mark -

CREATE_PMINTERFACE(CZExpWSFacade, kZExpWSFacadeImpl)

CZExpWSFacade::CZExpWSFacade(IPMUnknown * boss)
: inheritClass(boss)
{
#ifdef RVExport
	curl_global_init(CURL_GLOBAL_ALL);
#endif //RVExport
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpWSFacade::~CZExpWSFacade()
{
#ifdef RVExport
	curl_global_cleanup();
#endif //RVExport
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetProxy
//----------------------------------------------------------------------------------------
void
CZExpWSFacade::SetProxy(
	CURL *					ioCurl)
{
#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());

	InterfacePtr<const IZExpPrefs> expPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(expPrefs);

	if( expPrefs->NeedsProxyServer() )
	{
		int32 portNum = expPrefs->GetProxyPort();
		const PMString & proxyServer = expPrefs->GetProxyServer();

		const char * proxySrv = proxyServer.GrabCString();

		curl_easy_setopt( ioCurl, CURLOPT_PROXYPORT, portNum );
		curl_easy_setopt( ioCurl, CURLOPT_PROXY, proxySrv );
	}
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
	ZExpWSError toReturn = 0;
#ifdef RVExport
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		StCleanupCurl autoCleanCurl( curl );
		std::string html = "";
		
		const char * baseURL = inReq->GetRequestURL();

		StSmartBuff<char>	urlBuff(1000);
		char * url = urlBuff.Get();

		const PMString & pluginID = inReq->GetPluginID();

		sprintf( url, "%s/titles?pluginid=%s", baseURL, pluginID.GrabCString());

		//curl_easy_setopt(curl, CURLOPT_URL, "http://87.253.130.51/export9348/home/titles?pluginid=6a844793-77bf-4cd5-bd04-355a9fb9a224");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

		SetProxy( curl );

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			toReturn = res;
			oResp->SetErrorCode( res );
			oResp->SetErrorDescription("Problem in retreiving titles.");	//TODO: proper error detail from curl
			oResp->SetErrorDetails( curl_easy_strerror( res ) );
			return toReturn;
		}
		
		if (html.length())
		{
			TiXmlDocument xmlDoc;
			xmlDoc.Parse(html.c_str(), 0, TIXML_ENCODING_UTF8);
			if (xmlDoc.Error())
			{
				toReturn = eWSErr_XMLError;
				oResp->SetErrorCode( eWSErr_XMLError );
				oResp->SetErrorDescription("Problem in retreiving titles.");
				oResp->SetErrorDetails(xmlDoc.ErrorDesc());
				return toReturn;
			}

			TiXmlElement * root = xmlDoc.FirstChildElement();
			ZExpIDPairList & titles = ((CZExpFetchTitleListResponse*)oResp)->GetTitleList();
			if (root)
			{
				TiXmlElement *element = root->FirstChildElement("title");
				while(element)
				{
					const char * titleId = element->Attribute("titleid");
					const char * titleName = element->Attribute("name");

					titles.push_back(ZExpIDPair(titleId, titleName));		//Add to list

					element = element->NextSiblingElement("title");		//Get next node
				}
			}
		}
	}
#endif //RVExport
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetEditions
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::GetEditions(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpWSError toReturn = 0;
#ifdef RVExport
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		StCleanupCurl autoCleanCurl( curl );
		std::string html = "";

		const char * baseURL = inReq->GetRequestURL();

		StSmartBuff<char>	urlBuff(1000);
		char * url = urlBuff.Get();

		const PMString & pluginID = inReq->GetPluginID();
		
		const CZExpFetchTitleEditionsRequest * theReq = (const CZExpFetchTitleEditionsRequest*)inReq;
		const PMString & titleID = theReq->GetTitleId();

		sprintf( url, "%s/editions?pluginid=%s&titleid=%s", baseURL, pluginID.GrabCString(), titleID.GrabCString());

		//curl_easy_setopt(curl, CURLOPT_URL, "http://87.253.130.51/export9348/home/editions?pluginid=6a844793-77bf-4cd5-bd04-355a9fb9a224&titleid=103190a8-818f-4790-9446-8bcdbd3d569f");
		curl_easy_setopt(curl, CURLOPT_URL, url );
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

		SetProxy( curl );

		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK)
		{
			toReturn = res;
			oResp->SetErrorCode( res );
			oResp->SetErrorDescription("Problem in retreiving editions.");	//TODO: proper error detail from curl
			oResp->SetErrorDetails( curl_easy_strerror( res ) );
			return toReturn;
		}
		
		if (html.length())
		{
			TiXmlDocument xmlDoc;
			xmlDoc.Parse(html.c_str(), 0, TIXML_ENCODING_UTF8);
			if (xmlDoc.Error())
			{
				toReturn = eWSErr_XMLError;
				oResp->SetErrorCode( eWSErr_XMLError );
				oResp->SetErrorDescription("Problem in retreiving editions.");
				oResp->SetErrorDetails(xmlDoc.ErrorDesc());
				return toReturn;
			}

			TiXmlElement * root = xmlDoc.FirstChildElement();
			ZExpIDPairList & editions = ((CZExpFetchTitleEditionsResponse*)oResp)->GetEditionList();
			if (root)
			{
				TiXmlElement *element = root->FirstChildElement("edition");
				while(element)
				{
					const char * editionId = element->Attribute("editionid");
					const char * editionName = element->Attribute("name");

					editions.push_back(ZExpIDPair(editionId, editionName));	//Add to list

					element = element->NextSiblingElement("edition");		//Get next node
				}
			}
		}
	}
#endif //RVExport
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UploadExport
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::UploadExport(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpWSError toReturn = 0;
#ifdef RVExport
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		StCleanupCurl autoCleanCurl( curl );

		const char * baseURL = inReq->GetRequestURL();

		StSmartBuff<char>	urlBuff(1000);
		char * url = urlBuff.Get();

		const PMString & pluginID = inReq->GetPluginID();
		
		const CZExpUploadExportRequest * theReq = (const CZExpUploadExportRequest*)inReq;
		const PMString & titleID = theReq->GetTitleId();
		const PMString & editionID = theReq->GetEditionId();
		const PMString & articleID = theReq->GetArticleId();
		PMString docName = theReq->GetDocName();
		docName = FileUtils::PMStringToURL( docName );	//make name URL safe
		const uchar * content = theReq->GetContent();
		int contentLength = theReq->GetBuffSize();
		const PMString & filePath = theReq->GetFilePath();

		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "title : %s", titleID.GrabCString() );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "edition : %s", editionID.GrabCString() );

		sprintf( url, "%s/upload?pluginid=%s&titleid=%s&editionid=%s&articleid=%s&name=%s", baseURL, pluginID.GrabCString(), titleID.GrabCString(), editionID.GrabCString(), articleID.GrabCString(), docName.GrabCString());

		if( theReq->GetImageFileName().IsEmpty() == kFalse )
		{
			PMString fileName = theReq->GetImageFileName();
			fileName = FileUtils::PMStringToURL( fileName );	//make name URL safe
			strcat( url, "&filename=" );
			strcat( url, fileName.GrabCString() );
		}

		if( theReq->GetOtherParamStr().IsEmpty() == kFalse )
		{
			PMString otherParams = theReq->GetOtherParamStr();
			strcat( url, otherParams.GrabCString() );
		}

		std::string html = "";

		//curl_easy_setopt(curl, CURLOPT_URL, "http://87.253.130.51/export9348/home/upload?"
												//"pluginid=6a844793-77bf-4cd5-bd04-355a9fb9a224"
												//"&titleid=103190a8-818f-4790-9446-8bcdbd3d569f"
												//"&editionid=92ab50f3-7550-4fc5-80a2-583422dc8c07"
												//"&name=name"
												//"&filename=test.jpg");
		curl_easy_setopt(curl, CURLOPT_URL, url );
		//curl_easy_setopt(curl, CURLOPT_READFUNCTION, &buffReadfunc);
#if 0	//Use POST
		curl_easy_setopt(curl, CURLOPT_POST, true);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content); 
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, contentLength); 
#else	//Use PUT
		//setting follow redirect to true
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
		//using HTTP PUT
		curl_easy_setopt(curl, CURLOPT_PUT, true);


		//setting the input
		curlDataReadHelper data;	//Don't take it in if
		FILE * dataFilePtr = nil;
		StFileCloser autCloseFile( dataFilePtr );//Autoclose the file

		if( content && contentLength > 0 )
		{	//Set data reader function
			data.currentPtr = (const char *)content;
			data.endPtr = (const char *)content + contentLength;
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, readData);
			curl_easy_setopt(curl, CURLOPT_READDATA, &data);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, contentLength);
		}
		else
		{	//set file reader function
			if (filePath.IsEmpty())
			{
				toReturn = eWSErr_NoFileToUpload;
				oResp->SetErrorCode( toReturn );
				oResp->SetErrorDescription("No file to upload.");	//TODO: proper error detail from curl

				return toReturn;
			}

			dataFilePtr = FileUtils::OpenFile(filePath, "rb");
			if (!dataFilePtr)
			{
				toReturn = eWSErr_NoFileToUpload;	//Not able to open file for upload
				oResp->SetErrorCode( toReturn );
				oResp->SetErrorDescription("Not able to open file for upload.");	//TODO: proper error detail from curl

				return toReturn;
			}
			autCloseFile.Reset( dataFilePtr );

			//File size
			fseek(dataFilePtr, 0, SEEK_END);
			contentLength = ftell(dataFilePtr);
			fseek(dataFilePtr, 0L, SEEK_SET);

			if (contentLength == 0)
			{
				toReturn = eWSErr_NoFileToUpload;	//empty file for upload
				oResp->SetErrorCode( toReturn );
				oResp->SetErrorDescription("Empty file for upload.");	//TODO: proper error detail from curl
				return toReturn;
			}
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, fileReadfunc);
			curl_easy_setopt(curl, CURLOPT_READDATA, dataFilePtr);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, contentLength);
		}

		//setting content type to xml
		curl_slist *headers = 0;
		//headers = curl_slist_append(headers, "Content-Type: application/xml");
		headers = curl_slist_append(headers, "Expect:");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
#endif

		SetProxy( curl );

		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK)
		{
			toReturn = res;
			oResp->SetErrorCode( res );
			oResp->SetErrorDescription("Problem in uploading file.");	//TODO: proper error detail from curl
			oResp->SetErrorDetails( curl_easy_strerror( res ) );
			return toReturn;
		}

		CZExpUploadExportResponse * theResp = (CZExpUploadExportResponse*)oResp;
		theResp->SetUploadSuccess(true);
	}
#endif //RVExport
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ValidateMediaContent
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::ValidateMediaContent(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpWSError toReturn = 0;
#ifdef RVExport
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		StCleanupCurl autoCleanCurl( curl );

		const char * baseURL = inReq->GetRequestURL();

		StSmartBuff<char>	urlBuff(1000);
		char * url = urlBuff.Get();

		const PMString & pluginID = inReq->GetPluginID();
		
		const CZExpValidateMediaContentRequest * theReq = (const CZExpValidateMediaContentRequest*)inReq;
		const PMString & titleID = theReq->GetTitleId();
		const PMString & editionID = theReq->GetEditionId();
		const PMString & articleID = theReq->GetArticleId();
		PMString docName = theReq->GetDocName();
		docName = FileUtils::PMStringToURL( docName );	//make name URL safe
		const uchar * content = theReq->GetContent();
		int contentLength = theReq->GetBuffSize();
		const PMString & filePath = theReq->GetFilePath();

		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "title : %s", titleID.GrabCString() );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "edition : %s", editionID.GrabCString() );

		sprintf( url, "%s/ValidateMediaPresence?pluginid=%s&source=indesignPlugin&insertIfExists=%s&titleid=%s&editionid=%s&articleid=%s&name=%s", baseURL, pluginID.GrabCString(), "true", titleID.GrabCString(), editionID.GrabCString(), articleID.GrabCString(), docName.GrabCString());

		if( theReq->GetImageFileName().IsEmpty() == kFalse )
		{
			PMString fileName = theReq->GetImageFileName();
			fileName = FileUtils::PMStringToURL( fileName );	//make name URL safe
			strcat( url, "&filename=" );
			strcat( url, fileName.GrabCString() );
		}

		if( theReq->GetOtherParamStr().IsEmpty() == kFalse )
		{
			PMString otherParams = theReq->GetOtherParamStr();
			strcat( url, otherParams.GrabCString() );
		}

		StPtrDeleter<uchar>			autoDelFileBuff;
		//Find content if not set
		if( contentLength == 0 || !content )
		{	//Set data reader function
			uchar * dataBuff = nil;
			IDFile path(filePath);
			bool fileRead = Utils<IZExpFileUtils>()->ReadFile(path, dataBuff, contentLength);
			if( !fileRead )
			{
				toReturn = eWSErr_NoFileToUpload;
				oResp->SetErrorCode( toReturn );
				oResp->SetErrorDescription("Content Validation failed, file not found.");	//TODO: proper error detail from curl

				return toReturn;
			}

			content = dataBuff;

			autoDelFileBuff.Set( dataBuff );
		}

		if( content && contentLength > 0 )
		{
			//Get hash code
			char result[(MD5_DIGEST_LENGTH*2)+1];
			unsigned char hash[MD5_DIGEST_LENGTH];
			memset(hash, 0, MD5_DIGEST_LENGTH);
			MD5(content, contentLength, hash);
			for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
				sprintf(result + i * 2, "%02x", hash[i]);
			
			if(!hash)
			{
				toReturn = eWSErr_NoFileToUpload;	//TODO: Set proper error code here
				oResp->SetErrorCode( toReturn );
				oResp->SetErrorDescription("Hash code creation error");	//TODO: proper error detail from curl
				
				return toReturn;
			}

			strcat( url, "&hash=" );
			strcat( url, (char *)result );
		}
		else
		{
			toReturn = eWSErr_NoFileToUpload;
			oResp->SetErrorCode( toReturn );
			oResp->SetErrorDescription("Content Validation failed.");	//TODO: proper error detail from curl

			return toReturn;
		}

		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Full Url: %s", url );

		std::string html = "";

		curl_easy_setopt(curl, CURLOPT_URL, url );
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

		SetProxy( curl );

		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK)
		{
			toReturn = res;
			oResp->SetErrorCode( res );
			oResp->SetErrorDescription("Problem in validating file on server.");	//TODO: proper error detail from curl
			oResp->SetErrorDetails( curl_easy_strerror( res ) );
			return toReturn;
		}
		
		if (html.length())
		{
			TiXmlDocument xmlDoc;
			xmlDoc.Parse(html.c_str(), 0, TIXML_ENCODING_UTF8);
			if (xmlDoc.Error())
			{
				toReturn = eWSErr_XMLError;
				oResp->SetErrorCode( eWSErr_XMLError );
				oResp->SetErrorDescription("Problem in validating file on server.");
				oResp->SetErrorDetails(xmlDoc.ErrorDesc());
				return toReturn;
			}

			TiXmlElement * root = xmlDoc.FirstChildElement();
			if (root )
			{
				bool itemFound = false;
				bool itemAdded = false;
				if( TIXML_SUCCESS == root->QueryBoolAttribute( "itemFound", &itemFound ))
					root->QueryBoolAttribute( "itemAdded", &itemAdded );

				CZExpValidateMediaContentResponse * theResp = (CZExpValidateMediaContentResponse*)oResp;
				theResp->SetIsItemPresent( itemFound );
				theResp->SetIsItemAddedToArticle( itemAdded );
			}
			else
			{
				toReturn = eWSErr_XMLError;
				oResp->SetErrorCode( eWSErr_XMLError );
				oResp->SetErrorDescription("Problem in validating file on server.");
				oResp->SetErrorDetails(html.c_str());
			}
		}
	}
#endif //RVExport
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetConfig
//----------------------------------------------------------------------------------------
ZExpWSError
CZExpWSFacade::GetConfig(
	const AZExpWSRequest *	inReq,
	AZExpWSResponse *		oResp)
{
	ZExpWSError toReturn = 0;
#ifdef RVExport
#if 0
	((CZExpGetConfigResponse*)oResp)->SetDPIHighRes(200);
	((CZExpGetConfigResponse*)oResp)->SetDPILowRes(100);
	((CZExpGetConfigResponse*)oResp)->SetExportPDF(true);
	((CZExpGetConfigResponse*)oResp)->SetExportTiff(true);
	((CZExpGetConfigResponse*)oResp)->SetExportLowRes(false);
	((CZExpGetConfigResponse*)oResp)->SetPagePreview(false);
#else
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl)
	{
		StCleanupCurl autoCleanCurl( curl );
		std::string jsonStr = "";
		
//		const char * baseURL = inReq->GetRequestURL();

		StSmartBuff<char>	urlBuff(1000);
		char * url = urlBuff.Get();

		const PMString & pluginID = inReq->GetPluginID();

		//sprintf( url, "http://mediafacts.area5.nl/applications/trgrcy3hsa/?id=%s", pluginID.GrabCString());
		sprintf( url, "http://87.253.130.51/cms174/applications/trgrcy3hsa/?id=%s", pluginID.GrabCString());

		//curl_easy_setopt(curl, CURLOPT_URL, "http://87.253.130.51/export9348/home/config?pluginid=6a844793-77bf-4cd5-bd04-355a9fb9a224");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonStr);

		SetProxy( curl );

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			toReturn = res;
			oResp->SetErrorCode( res );
			oResp->SetErrorDescription("Problem in retreiving config data.");	//TODO: proper error detail from curl
			oResp->SetErrorDetails( curl_easy_strerror( res ) );
			return toReturn;
		}

		if (jsonStr.length())
		{
			//{"pluginid":"6a844793-77bf-4cd5-bd04-355a9fb9a224","pdf":false,"pdftemplate":"high","tiff":false,"dpilow":72,"dpihigh":72}
			bool exportPdf = GlobalDataDefault_ExportPDF;
			bool exportTiff = GlobalDataDefault_ExportTiff;
			bool exportLowRes = GlobalDataDefault_ExportLowRes;
			bool exportPagePreview = GlobalDataDefault_ExportPagePreview;
			int dpiLow = GlobalDataDefault_DPILowRes;
			int dpiHigh = GlobalDataDefault_DPIHighRes;
			PMString styleName(GlobalDataDefault_StyleName);

			const char blank = ' ';
			const char rep1 = '{';
			const char rep2 = '}';
			const char rep3 = '\"';
			std::replace(jsonStr.begin(), jsonStr.end(), rep1, blank);
			std::replace(jsonStr.begin(), jsonStr.end(), rep2, blank);
			std::replace(jsonStr.begin(), jsonStr.end(), rep3, blank);

			const std::string delim1 = ",";
			const std::string delim2 = ":";
			// Skip delimiters at beginning.
			std::string::size_type lastPos = jsonStr.find_first_not_of(delim1, 0);
			// Find first "non-delimiter".
			std::string::size_type pos = jsonStr.find_first_of(delim1, lastPos);

			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				// Found a token, add it to the vector.
				std::string keyValuePair = jsonStr.substr(lastPos, pos - lastPos);
				// Skip delimiters.  Note the "not_of"
				lastPos = jsonStr.find_first_not_of(delim1, pos);
				// Find next "non-delimiter"
				pos = jsonStr.find_first_of(delim1, lastPos);

				if(keyValuePair.length())
				{
					// Skip delimiters at beginning.
					std::string::size_type lastPosKeyValue = keyValuePair.find_first_not_of(delim2, 0);
					// Find first "non-delimiter".
					std::string::size_type posKeyValue = keyValuePair.find_first_of(delim2, lastPosKeyValue);

					if (std::string::npos != posKeyValue || std::string::npos != lastPosKeyValue)
					{
						// Found a token, add it to the vector.
						std::string key = keyValuePair.substr(lastPosKeyValue, posKeyValue - lastPosKeyValue);
						std::string value = "";

						// Skip delimiters.  Note the "not_of"
						lastPosKeyValue = keyValuePair.find_first_not_of(delim2, posKeyValue);
						// Find next "non-delimiter"
						posKeyValue = keyValuePair.find_first_of(delim2, lastPosKeyValue);

						if (std::string::npos != posKeyValue || std::string::npos != lastPosKeyValue)
							value = keyValuePair.substr(lastPosKeyValue, posKeyValue - lastPosKeyValue);

						//lTrim key
						key.erase(key.begin(), std::find_if(key.begin(), key.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
						//rTrim key
						key.erase(std::find_if(key.rbegin(), key.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), key.end());
						//lTrim value
						value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
						//rTrim value
						value.erase(std::find_if(value.rbegin(), value.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), value.end());

						if(key.length() && value.length())
						{
							if(key == "pdf")
							{
								exportPdf = (value.compare("true") == 0);
							}
							else if(key == "pdftemplate")
							{
								styleName = PMString(value.c_str());
							}
							else if(key == "tiff")
							{
								exportTiff = (value.compare("true") == 0);
							}
							else if(key == "preview")
							{
								exportLowRes = (value.compare("true") == 0);
							}
							else if(key == "pagePreview" || key == "pagePreviews")
							{
								exportPagePreview = (value.compare("true") == 0);
							}
							else if(key == "dpilow")
							{
								dpiLow = atoi(value.c_str());
							}
							else if(key == "dpihigh")
							{
								dpiHigh = atoi(value.c_str());
							}
						}
					}
				}
			}

			((CZExpGetConfigResponse*)oResp)->SetExportPDF(exportPdf);
			((CZExpGetConfigResponse*)oResp)->SetExportTiff(exportTiff);
			((CZExpGetConfigResponse*)oResp)->SetDPILowRes(dpiLow);
			((CZExpGetConfigResponse*)oResp)->SetDPIHighRes(dpiHigh);
			((CZExpGetConfigResponse*)oResp)->SetExportLowRes(exportLowRes);
			((CZExpGetConfigResponse*)oResp)->SetExportPagePreview(exportPagePreview);
			if(styleName.CharCount())
				((CZExpGetConfigResponse*)oResp)->SetStyleName(styleName);
		}
	}
#endif
#endif //RVExport
	return toReturn;
}
