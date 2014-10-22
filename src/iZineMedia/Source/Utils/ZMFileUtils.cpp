//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/ZMFileUtils.cpp $
//	$Revision: 2906 $
//	$Date: 2011-04-29 15:21:22 +0200 (Fri, 29 Apr 2011) $
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
#include "IWorkspace.h"

//SDK General includes
#include "FileUtils.h"
#include "Utils.h"
#include "SDKFileHelper.h"
#include "PlatformFileSystemIterator.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMFileUtils.h"
#include "IZMPrefs.h"
#include "IZMAdFileFacade.h"

//IZP General includes
#include "StUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMFileUtils>
class ZMFileUtils : public inheritClass
{
public:
						ZMFileUtils(IPMUnknown * boss);
	virtual				~ZMFileUtils();

	bool						FetchEditionsFromPath(
									const PMString&						parentPath, 
									ZMstEditionSList &					oEditionList) const;
	void						FetchListFromPath(
									const PMString&						parentPath, 
									ZMstTitleSList &					oTitleList) const;
	bool						FetchAdsFileFromPath(
									const PMString&						parentPath, 
									ZMPMStringSList &					oAdsList,
									const PMString&						inFilePrefix) const;
	void						GetAllDirectoryNames(
									const PMString&						parentPath, 
									ZMPMStringSList &					oDirectoryNames) const;
#ifdef DEBUG
	void						PrintToLog(
									ZMstTitleSList &					titleList) const;
#endif
	bool16						ValidPath(
									const PMString&						path) const;
	bool16						VerifyPrefixAndSuffix(
									const PMString&						inFileName,
									const PMString&						inFilePrefix,
									const PMString&						inFileExtension) const;
	PMString					ReplaceSlashWithColon(
									const PMString &					inPath ) const;
	PMString					MacPosixToUnixPath(
									const PMString &					inPath ) const;
protected:
private:
};


CREATE_PMINTERFACE(ZMFileUtils, kZMFileUtilsImpl)

ZMFileUtils::ZMFileUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMFileUtils::~ZMFileUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchEditionsFromPath
//----------------------------------------------------------------------------------------
bool
ZMFileUtils::FetchEditionsFromPath(
	const PMString&						parentPath,
	ZMstEditionSList &					oEditionList) const
{
	LogFunctionEnterExit;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "From path : %s", parentPath.GrabCString());
	SDKFileHelper fileHelper(parentPath);
	IDFile fileId = fileHelper.GetIDFile();
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(fileId))
		return false;

	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	InterfacePtr<const IZMPrefs> workspacePrefs( iWorkspace, UseDefaultIID() );
	ASSERT( workspacePrefs );
	
	const PMString & filePrefixName = workspacePrefs->GetAdFileNamePrefix();
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File prefix name : %s", filePrefixName.GrabCString());
	
	bool result = false;
	ZMPMStringSList editionList;
	this->GetAllDirectoryNames(parentPath, editionList);
	ZMPMStringSListCIter editionIter = editionList.begin();
	ZMPMStringSListCIter editionIterEnd = editionList.end();
	for(; editionIter != editionIterEnd; ++editionIter)
	{
		stEdition *currEdition = new stEdition();
		StPtrDeleter<stEdition> autoDel( currEdition, true );

		const PMString edition(**editionIter);
		PMString editionFullPath (parentPath);
		editionFullPath.Append(FileUtils::GetDirectorySeperator());
		editionFullPath.Append(edition);

		if (this->FetchAdsFileFromPath(editionFullPath, currEdition->mXmlFiles, filePrefixName))
		{
			result = true;
			currEdition->mName = edition;
			oEditionList.push_back(currEdition);
			autoDel.Forget();
		}
	}

	return result;
}

//----------------------------------------------------------------------------------------
// FetchListFromPath
//----------------------------------------------------------------------------------------
void
ZMFileUtils::FetchListFromPath(
	const PMString&						parentPath,
	ZMstTitleSList &					oTitleList) const
{
	LogFunctionEnterExit;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "From path : %s", parentPath.GrabCString());
	SDKFileHelper fileHelper(parentPath);
	IDFile fileId = fileHelper.GetIDFile();
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(fileId))
		return;

	ZMPMStringSList titleList;
	this->GetAllDirectoryNames(parentPath, titleList);
	ZMPMStringSListCIter titleIter = titleList.begin();
	ZMPMStringSListCIter titleIterEnd = titleList.end();

	for(; titleIter != titleIterEnd; ++titleIter)
	{
		stTitle *currTitle = new stTitle();
		StPtrDeleter<stTitle> autoDel( currTitle, true);

		const PMString & title = (**titleIter);
		PMString titleFullPath(parentPath);
		titleFullPath.Append(FileUtils::GetDirectorySeperator());
		titleFullPath.Append(title);

		if ( this->FetchEditionsFromPath(titleFullPath, currTitle->mEditions) )
		{
			currTitle->mName = title;
			oTitleList.push_back(currTitle);
			autoDel.Forget();
		}
	}
}

//----------------------------------------------------------------------------------------
// FetchAdsFileFromPath
//----------------------------------------------------------------------------------------
bool
ZMFileUtils::FetchAdsFileFromPath(
	const PMString&						parentPath,
	ZMPMStringSList &					oAdsList,
	const PMString&						inFilePrefix) const
{
	LogFunctionEnterExit;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "From path : %s", parentPath.GrabCString());
	SDKFileHelper fileHelper(parentPath);
	IDFile fileId = fileHelper.GetIDFile();
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(fileId))
		return false;

	iter.SetStartingPath(fileId);

	Utils<IZMAdFileFacade> adFileUtil;
	bool result = false;
	IDFile xmlIdFile;

	PMString filter(inFilePrefix);
	filter.Append("*.xml");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	bool16 hasNext = iter.FindFirstFile(xmlIdFile, filter);

	while(hasNext) {
		SDKFileHelper xmlFileHelper(xmlIdFile);
		
		PMString xmlFileName = xmlIdFile.GetFileName();
		if(ValidPath(xmlFileName))
		{
#ifdef MACINTOSH
			PMString extStr(".xml");
			extStr.SetTranslatable(kFalse);
			if (! VerifyPrefixAndSuffix(xmlFileName, inFilePrefix, extStr))
			{
				hasNext= iter.FindNextFile(xmlIdFile);
				continue;
			}
#endif
			
			PMString xmlFullPath (parentPath);
			xmlFullPath.Append(FileUtils::GetDirectorySeperator());
			xmlFullPath.Append(xmlFileName);

			if (adFileUtil->CanBeValidXMLFile(xmlFullPath))
			{
				oAdsList.push_back(new PMString(xmlIdFile.GetFileName()));
				result = true;
			}
		}

		hasNext= iter.FindNextFile(xmlIdFile);
	}

	return result;
}

//----------------------------------------------------------------------------------------
// GetAllDirectoryNames
//----------------------------------------------------------------------------------------
void
ZMFileUtils::GetAllDirectoryNames(
	const PMString&						parentPath,
	ZMPMStringSList &					oDirectoryNames) const
{
	SDKFileHelper parentFileHelper(parentPath);
	IDFile parIDFile = parentFileHelper.GetIDFile();
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(parIDFile))
		return;

	iter.SetStartingPath(parIDFile);
	
	IDFile idFile;
	PMString filter("*.*");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	bool16 hasNext= iter.FindFirstFile(idFile, filter);

	while(hasNext) {
		if (iter.IsDirectory (idFile))
		{
			SDKFileHelper fileHelper(idFile);
			
			PMString fileNamePath = idFile.GetFileName();
			if(ValidPath(fileNamePath))
			{
				oDirectoryNames.push_back(new PMString(fileNamePath));
			}
		}

		hasNext= iter.FindNextFile(idFile);
	}
}

#ifdef DEBUG
//----------------------------------------------------------------------------------------
// PrintToLog
//----------------------------------------------------------------------------------------
void
ZMFileUtils::PrintToLog(
	ZMstTitleSList &					titleList) const
{
	LogFunctionEnterExit;
	ZMstTitleSListCIter titleIter = titleList.begin();
	ZMstTitleSListCIter titleIterEnd = titleList.end();
	for(; titleIter != titleIterEnd; ++titleIter)
	{
		const stTitle *title = *titleIter;
		const PMString & nameTitle = title->mName;
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Title name : %s", nameTitle.GrabCString());

		const ZMstEditionSList & editionList = title->mEditions;

		ZMstEditionSListCIter editionIter = editionList.begin();
		ZMstEditionSListCIter editionIterEnd = editionList.end();
		for(; editionIter != editionIterEnd; editionIter++)
		{
			const stEdition *edition = *editionIter;

			const PMString & nameEdition = edition->mName;
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Edition name : %s", nameEdition.GrabCString());

			const ZMPMStringSList & xmlFileList = edition->mXmlFiles;

			ZMPMStringSListCIter xmlFileIter = xmlFileList.begin();
			ZMPMStringSListCIter xmlFileIterEnd = xmlFileList.end();
			for(; xmlFileIter != xmlFileIterEnd; xmlFileIter++)
			{
				const PMString * xmlFile = *xmlFileIter;
				IZPLog_Str_( thisFileLA, enLT_DebugInfo, "XML file name : %s", xmlFile->GrabCString());
			}
		}
	}
}
#endif

//----------------------------------------------------------------------------------------
// ValidPath
//----------------------------------------------------------------------------------------
bool16
ZMFileUtils::ValidPath(
	const PMString&						path) const
{
	const PMString thisDir(".");
	const PMString parentDir("..");
	return path != thisDir && path != parentDir;
}

//----------------------------------------------------------------------------------------
// VerifyPrefixAndSuffix
//----------------------------------------------------------------------------------------
bool16
ZMFileUtils::VerifyPrefixAndSuffix(
	const PMString&						inFileName,
    const PMString&						inFilePrefix,
	const PMString&						inFileExtension) const
{
	bool16 result = kFalse;

	do
	{
		if( inFilePrefix.CharCount() > 0 && inFileName.IndexOfString(inFilePrefix, 0) != 0)
			break;

		if( inFileExtension.CharCount() > 0 )
		{
			int32 pos = inFileName.CharCount() - inFileExtension.CharCount();
			if (pos > 0 && (!inFileName.Contains(inFileExtension, pos)))
				break;
		}
		result = kTrue;
	}while(false);
	
	return result;
}

//----------------------------------------------------------------------------------------
// MacPosixToUnixPath
//----------------------------------------------------------------------------------------
PMString
ZMFileUtils::MacPosixToUnixPath(
	const PMString &					inPath ) const
{
	PMString toReturn( inPath );
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Replacing : with / in path = %s", toReturn.GrabCString() );
		
	const PMString slash("/");
	CharCounter foundIndex = toReturn.IndexOfString( ":" );
	while( foundIndex != -1 )
	{
		toReturn.Remove( foundIndex, 1 );
		toReturn.Insert( slash, foundIndex );
		foundIndex = toReturn.IndexOfString( ":" );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ReplaceSlashWithColon
//----------------------------------------------------------------------------------------
PMString
ZMFileUtils::ReplaceSlashWithColon(
	const PMString &					inPath ) const
{
	PMString toReturn( inPath );
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "Replacing / with : in path = %s", toReturn.GrabCString() );
		
	const PMString slash("/");
	const PMString colon(":");
	CharCounter foundIndex = toReturn.IndexOfString( slash );
	while( foundIndex != -1 )
	{
		toReturn.Remove( foundIndex, 1 );
		toReturn.Insert( colon, foundIndex );
		foundIndex = toReturn.IndexOfString( slash );
	}
	return toReturn;
}


