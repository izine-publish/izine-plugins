//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/paneltreeview/WinFileSystemIterator.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: rajkumar.sehrawat $
//  
//  $DateTime: 2007/02/15 13:27:55 $
//  
//  $Revision: 3114 $
//  
//  $Change: 505962 $
//  
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPluginHeaders.h"

#ifdef WINDOWS 
#include "WinFileSystemIterator.h"

// General includes:
#include "K2Vector.tpp"
#include "CAlert.h"
#include "FileUtils.h"

/* SetStartingPath
*/
void WinFileSystemIterator::SetStartingPath(const IDFile& fileSpec)
{
	fSpec = fileSpec;
}


/* FindFirstFile
*/
bool16 WinFileSystemIterator::FindFirstFile(IDFile& resultFile, PMString filter)
{
	fFilter = filter;

	fFilesFound=0;
	if (fhFindFile != INVALID_HANDLE_VALUE)
	{
		::FindClose(fhFindFile);
		fhFindFile = INVALID_HANDLE_VALUE;
	}

	WIN32_FIND_DATA findData;
    PMString fldrFilter = fSpec.GetString();
    fldrFilter.Append(filter);
	fhFindFile = ::FindFirstFile(fldrFilter.GrabTString(), &findData);
	
	if (fhFindFile != INVALID_HANDLE_VALUE)
	{
		PMString fileName;
		fileName.SetTString((LPTSTR)&findData.cFileName);

		resultFile = fSpec ;
		FileUtils::AppendPath(&resultFile, fileName);
		fFilesFound++;
	}

	return fhFindFile != INVALID_HANDLE_VALUE;
}

	
/* FindNextFile
*/
bool16 WinFileSystemIterator::FindNextFile(IDFile& resultFile)
{
	//RJK: no limit on files being iterated.
	//if (fFilesFound > eMaxFolderItemsPerSingleIteration)
	//{
	//	return kFalse;
	//}

	WIN32_FIND_DATA findData;
	bool16 foundNext = ::FindNextFile(fhFindFile, &findData);
	if (foundNext)
	{
		PMString fileName;
		fileName.SetTString(findData.cFileName);
	
		resultFile = fSpec ;
		FileUtils::AppendPath(&resultFile, fileName);

		fFilesFound++;
	}

	if(!foundNext)
	{
		::FindClose (fhFindFile);
		fhFindFile = INVALID_HANDLE_VALUE;
	}
	return foundNext;
}


/* IsDirectory
*/
bool16 WinFileSystemIterator::IsDirectory(const IDFile& fileSpec)
{
	return fileSpec.GetAttribute(IDFile::kDirectory);
}
 

/* GetImmediateChildren
*/
void WinFileSystemIterator::GetImmediateChildren(
	const IDFile& parentSysFile, 
	K2Vector<IDFile>& outFileSpecs,
	const int32 maxFolderItemsPerBulkCall){
	const PMString dot(".");
	const PMString doubleDot("..");
	
	outFileSpecs.clear();

	do
	{

		WIN32_FIND_DATA findData;
		PMString filter("\\*.*");
		PMString fldrFilter = parentSysFile.GetString();
		fldrFilter.Append(filter);
		HANDLE hSearch = ::FindFirstFile(fldrFilter.GrabTString(), &findData); 
		if (hSearch == INVALID_HANDLE_VALUE)
		{ 
			break;
		} 
 
		bool16 hasNext =kTrue;
		int32 filesFound=0;
		while (hasNext && filesFound < maxFolderItemsPerBulkCall) 
		{ 
			
			IDFile resultFile;
			PMString fileName;
			fileName.SetTString(findData.cFileName);
			if(fileName != dot && fileName != doubleDot)
			{				
				resultFile = parentSysFile ;
				FileUtils::AppendPath(&resultFile, fileName);
				outFileSpecs.push_back(resultFile);
				filesFound++;
			}
 
			hasNext = ::FindNextFile(hSearch, &findData);
		} 
 
		// Close the search handle. 
		::FindClose(hSearch);

	} while(kFalse);
}

#endif

//	end, File: WinFileSystemIterator.cpp
