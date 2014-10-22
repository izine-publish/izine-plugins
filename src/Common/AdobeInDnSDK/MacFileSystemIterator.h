//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/paneltreeview/MacFileSystemIterator.h $
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

#ifndef __PnlTrvMacDirIterator_H_DEFINED__
#define __PnlTrvMacDirIterator_H_DEFINED__
#include "FileSystemIterator.h"

/** Macintosh specific class to iterate a folder's descendants.
@ingroup paneltreeview
 */
class MacFileSystemIterator : public FileSystemIterator {
public:
	MacFileSystemIterator();
	~MacFileSystemIterator();

	/**	Initialise the iterator with a starting folder

		@param fileSpec specifies the folder whose immediate dependents we're interested in
	 */
	virtual void SetStartingPath(const IDFile& fileSpec);

	/**	Find the first file given the starting folder, if there is one.
		If the start file isn't a folder then this will return kFalse.

		@param resultFile [OUT] parameter
		@param filter [IN] specifies a filter on the files that are represented by this. No effect on MAC.
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

		@param parentSysFile specifies the file whose immediate dependents we want
		@param outFileSpecs [OUT] holds the children of the specified folder
		@param maxFolderItemsPerBulkCall constrains the max number of items that might be returned. 

	*/		virtual void GetImmediateChildren(const IDFile& parentSysFile, 
										K2Vector<IDFile>& outFileSpecs,
										const int32 maxFolderItemsPerBulkCall=256 );


protected:
	bool16 areEqual(const IDFile& file1, const IDFile& file2);
	
private:
#ifdef MACINTOSH 
	FSSpec fStartingFileSpec;
	FSIterator fFSIterator;
#endif // MACINTOSH
};


#endif
