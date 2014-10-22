//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/ZMAdFileFacade.cpp $
//	$Revision: 2908 $
//	$Date: 2011-04-29 15:27:49 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 4-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IK2ServiceProvider.h"
#include "IK2ServiceRegistry.h"
#include "ISAXContentHandler.h"
#include "ISAXEntityResolver.h"
#include "ISAXParserOptions.h"
#include "ISAXServices.h"
#include "IShowAlertRegistryUtils.h"
#include "IWorkspace.h"

//SDK General includes
#include "CAlert.h"
#include "ErrorUtils.h"
#include "SDKFileHelper.h"
#include "StInDesignUtils.h"
#include "StreamUtil.h"
#include "Utils.h"
#include "FileUtils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdFileFacade.h"
#include "IZMAdFileData.h"
#include "IZMAdInfo.h"
#include "IZMPrefs.h"
#include "IZMFileUtils.h"

//IZP General includes
#include "ZMConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMAdFileFacade>
class ZMAdFileFacade : public inheritClass
{
public:
						ZMAdFileFacade(IPMUnknown * boss);
	virtual				~ZMAdFileFacade();
	bool16						CanBeValidXMLFile(
									const PMString&						path) const;
	bool						ParseAdXMLFile(
									const PMString &					path, 
									ZMstAdSList &						oAdsList,
									uint32 *							oLastUpdateDate) const;
								//Replaces %path% with repository path. Return empty if repository path is empty
	PMString					MakeFullAdFilePath(
									const IZMAdInfo *					inAdInfo ) const;
protected:
private:
};


CREATE_PMINTERFACE(ZMAdFileFacade, kZMAdFileFacadeImpl)

ZMAdFileFacade::ZMAdFileFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMAdFileFacade::~ZMAdFileFacade()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanBeValidXMLFile
//----------------------------------------------------------------------------------------
bool16
ZMAdFileFacade::CanBeValidXMLFile(
	const PMString&						path) const
{
	SDKFileHelper fileHelper(path);
	IDFile fileId = fileHelper.GetIDFile();
	InterfacePtr<IPMStream> fileStream((IPMStream*)StreamUtil::CreateFileStreamRead(fileId));
	if (! fileStream)
		return kFalse;

	StStreamCloser autoReleaseStream(fileStream);
	const char *strToCompare = STRXMLTag_ClassWizardImposition;
	char str[105];
	fileStream->XferByte((uchar *)str, 100);
	str[104] = '\0';

	if (strstr(str, strToCompare))
		return kTrue;

	return kFalse;
}

//----------------------------------------------------------------------------------------
// ParseAdXMLFile
//----------------------------------------------------------------------------------------
bool
ZMAdFileFacade::ParseAdXMLFile(
	const PMString &					path,
	ZMstAdSList &						oAdsList,
	uint32 *							oLastUpdateDate) const
{
	if (oLastUpdateDate)
		*oLastUpdateDate = 0;

	bool result = false;
	SDKFileHelper fileHelper(path);
	IDFile fileId = fileHelper.GetIDFile();
	InterfacePtr<IPMStream> fileStream((IPMStream*)StreamUtil::CreateFileStreamRead(fileId));
	if (! fileStream)
		return result;

	//Set last update date
	if (oLastUpdateDate)
		FileUtils::GetModificationDate(fileId, oLastUpdateDate);

	InterfacePtr<IK2ServiceRegistry> serviceRegistry(gSession, UseDefaultIID());
	InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));
	InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());

	InterfacePtr<ISAXContentHandler> contentHandler(static_cast<ISAXContentHandler*>(::CreateObject(kZMSAXAdContentHandlerBoss, ISAXContentHandler::kDefaultIID)));
	InterfacePtr<ISAXEntityResolver> entityResolver(static_cast<ISAXEntityResolver*>(::CreateObject(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID)));
	if (Utils<IShowAlertRegistryUtils>())
	{
		Utils<IShowAlertRegistryUtils>()->DoSetShouldShowCmd(kXMLEntityNotResolved, kFalse);
		Utils<IShowAlertRegistryUtils>()->DoSetShouldShowCmd(kXMLPublicIDsNotSupported, kFalse);
	}

	// set parser options
	InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());
	parserOptions->SetNamespacesFeature(kTrue);
	parserOptions->SetNamespacePrefixesFeature(kTrue);
	parserOptions->SetWarnCaseXMLDECLFeature(kFalse);
	parserOptions->SetWarnCaseDTDFeature(kFalse);
	parserOptions->SetValidationFeature(kFalse);

	saxServices->ParseStream(fileStream, contentHandler, entityResolver);
	ErrorCode err = ErrorUtils::PMGetGlobalErrorCode();
	if (err == kIZMInvalidXMLFileError)
	//if (err == kFailure || err == kIZMInvalidXMLFileError)
	{
		PMString errStr(kIZMediaInvalidXMLFileErrorCodeStringKey);
		ErrorUtils::PMSetGlobalErrorCode(kSuccess);
		CAlert::ErrorAlert(errStr);
		//ErrorUtils::PMSetGlobalErrorCode(kFailure, &errStr);

		//ErrorUtils::PMSetGlobalErrorCode(kFailure, &errStr);
		//InterfacePtr<IXMLServicesError> serviceErr (saxServices, UseDefaultIID());
		//err = serviceErr->GetErrorCode ();
		//if (err == kIZMInvalidXMLFileError)
		//	int x =0;
		//PMString errStr("XML Error");
	}

	if (!err)
	{
		InterfacePtr<const IZMAdFileData> zmAdList(contentHandler, IZMAdFileData::kDefaultIID);
		if (zmAdList)
		{
			const ZMstAdSList & adsList= zmAdList->GetAdsList();
			oAdsList.Copy(adsList, true);	//This will take ownership of objects in adsList

			result = true;
			//Testing code for checking values.
			//ZMstAdSListCIter iter = adsList.begin();
			//ZMstAdSListCIter iterEnd = adsList.end();
			//for (; iter != iterEnd; ++iter)
			//{
			//	const stAd & ad = **iter;
			//	PMString customer = ad.mCustomer;
			//}
		}
	}

	return result;
}

//----------------------------------------------------------------------------------------
// MakeFullAdFilePath
//----------------------------------------------------------------------------------------
PMString
ZMAdFileFacade::MakeFullAdFilePath(
	const IZMAdInfo *					inAdInfo) const
{
	LogFunctionEnterExit;
	PMString toReturn;
	do
	{
		if( !inAdInfo )
			break;

		const PMString & adUrl = inAdInfo->GetUrl();
		toReturn = adUrl;

		if( adUrl.CharCount() == 0 )
			break;

		const PMString toFind( "%path%" );

		CharCounter foundIndex = adUrl.IndexOfString( toFind );

		if( foundIndex == -1 )
			break;

		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		InterfacePtr<const IZMPrefs> workspacePrefs( iWorkspace, UseDefaultIID() );
		ASSERT( workspacePrefs );

		const PMString & repositoryLocation = workspacePrefs->GetRepositoryPath();
		if( repositoryLocation.CharCount() == 0 )
			return kNullString;

		toReturn.Remove( foundIndex, toFind.CharCount() );
		if( foundIndex > 0 )
		{
			toReturn.Remove( 0, foundIndex );
			foundIndex = 0;
		}

		toReturn.Insert( repositoryLocation, foundIndex );

#ifdef MACINTOSH
		toReturn = Utils<IZMFileUtils>()->ReplaceSlashWithColon( toReturn );
		//const PMString volumeStr(":Volumes:");
		//toReturn.Insert(volumeStr, 0 );

		//toReturn = Utils<IZMFileUtils>()->MacPosixToUnixPath(toReturn);
#endif

	}while(false);

	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Ad url for dnd = %s", toReturn.GrabCString() );

	return toReturn;
}
