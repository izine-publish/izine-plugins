//========================================================================================
//  
//  $File: //depot/indesign_CS5.5/source/sdksamples/paneltreeview/FileSystemIterator.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2010/02/23 03:46:08 $
//  
//  $Revision: #1 $
//  
//  $Change: 742150 $
//  
//  Copyright 1997-2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#ifndef __FileSystemIterator_H_DEFINED__
#define __FileSystemIterator_H_DEFINED__

#include "K2Vector.h"

/** Abstract interface to represent capability to traverse a file system or interact
	with it in some way.

	
	@ingroup paneltreeview
*/

class FileSystemIterator
{
public:

	/**	Initialise the iterator with a starting folder
		@param fileSpec [IN] specifies the folder whose immediate dependents we're interested in
	 */
	virtual void SetStartingPath(const IDFile& fileSpec)=0;

	/**	Find the first file given the starting folder, if there is one.
		If the start file isn't a folder then this will return kFalse.
		@param resultFile [OUT] specifies found file
		@param filter [IN] specifies a filter on the files that are represented by this
		@return bool16 kTrue if there is a first file, kFalse otherwise
	 */
	virtual bool16 FindFirstFile(IDFile& resultFile, PMString filter)=0;		

	/**	Find next file in iteration (doesn't descend into child folders).
		Will return kFalse if there is no next file given the current iteration state.
		@param resultFile [OUT] parameter
		@return bool16 kTrue if a file could be found, kFalse otherwise
	 */
	virtual bool16 FindNextFile(IDFile& resultFile)=0;

	/** Determine if specified file is a directory
		@param fileSpec [IN] specifies file of interest
		@return kTrue if directory, kFalse otherwise
	*/
	virtual bool16 IsDirectory(const IDFile& fileSpec)=0;

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
		const int32 maxFolderItemsPerBulkCall=256)=0;
};

#endif

//	end, File: FileSystemIterator.h
