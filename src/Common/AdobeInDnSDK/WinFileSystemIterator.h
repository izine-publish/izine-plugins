//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/paneltreeview/WinFileSystemIterator.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: aman.alam $
//  
//  $DateTime: 2007/02/15 13:27:55 $
//  
//  $Revision: 2903 $
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

#ifndef __WinFileSystemIterator_H_DEFINED__
#define __WinFileSystemIterator_H_DEFINED__

#include "FileSystemIterator.h"

/** Windows specific class to iterate contents of folder. 
@ingroup paneltreeview
*/
class WinFileSystemIterator : public FileSystemIterator {
public:
#ifdef WINDOWS 
	WinFileSystemIterator() : fhFindFile(INVALID_HANDLE_VALUE), fFilesFound(0) {}
	~WinFileSystemIterator() { fhFindFile = INVALID_HANDLE_VALUE;}
#endif


	/**	Initialise the iterator with a starting folder
		@param fileSpec [IN] specifies the folder whose immediate dependents we're interested in
	 */
	virtual void SetStartingPath(const IDFile& fileSpec);

	/**	Find the first file given the starting folder, if there is one.
		If the start file isn't a folder then this will return kFalse.

		@param resultFile [OUT] parameter
		@param filter [IN] specifies a filter on the files that are represented by this
		@return bool16 kTrue if there is a first file, kFalse otherwise
	 */
	virtual bool16 FindFirstFile(IDFile& resultFile, PMString filter);		

	/**	Find next file in iteration (doesn't descend into child folders).
		Will return kFalse if there is no next file given the current iteration state.

		@param resultFile [OUT] parameter
		@return bool16 kTrue if a file could be found, kFalse otherwise
	 */
	virtual bool16 FindNextFile(IDFile& resultFile);

	/** Determine if specified file is a directory
		@return kTrue if directory, kFalse otherwise
	*/
	virtual bool16 IsDirectory(const IDFile& fileSpec);


	/** Obtain list of files that are immediate descendants of the given parent
		Vector will be cleared and empty if no children exist.
		Otherwise on return it will contain the children of the specified folder.
		@param parentSysFile [IN] specifies the file whose immediate dependents we want
		@param outFileSpecs [OUT] holds the children of the specified folder
		@param maxFolderItemsPerBulkCall [IN] constrains the max number of items that might be returned. 
	*/
	virtual void GetImmediateChildren(
		const IDFile& parentSysFile, 
		K2Vector<IDFile>& outFileSpecs,
		const int32 maxFolderItemsPerBulkCall=256);
private:
	int32 fFilesFound;
	IDFile fSpec;
	PMString fFilter;
#ifdef WINDOWS
	HANDLE  fhFindFile;
#endif
	// Not likely to be useful beyond a modest number
	enum {eMaxFolderItemsPerSingleIteration=256};
};


#endif

