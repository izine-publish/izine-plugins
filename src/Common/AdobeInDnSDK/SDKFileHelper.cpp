//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/common/SDKFileHelper.cpp $
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

#include "VCPlugInHeaders.h"

// Interface includes
#include "ISaveFileDialog.h"
#include "IOpenFileDialog.h"
#include "ICoreFilename.h"
#include "IPMStream.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IImportManager.h"
#include "IImportManagerOptions.h"

#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IOpenManager.h"
#include "ITriStateData.h"
#include "IOpenFileCmdData.h"
#include "ISelectFolderDialog.h"
// General includes:
#include "StreamUtil.h"
#include "FileUtils.h"
#include "FileTypeRegistry.h"
#include "SysFileList.h"
#include "OpenPlaceID.h"
#include "Trace.h"
#include "K2Vector.h"
#include "K2Vector.tpp"

// Project includes:
#include "SDKFileHelper.h"
/*
	This file implements all the classes from SDKFileHelper. Their implementation code 
	is in the following order in this file:
		1. SDKFileHelper
		2. SDKFileOpenChooser
		3. SDKFileDocumentChooser
		4. SDKFileSaveChooser
		5. SDKFilePlaceChooser
		6. SDKFolderChooser
*/

// --------------------------------- SDKFileHelper -------------------------------------

const static PlatformChar SDKFileHelper_UNIXDELCHAR =('/');
const static PlatformChar  SDKFileHelper_MACDELCHAR =(':');
const static PlatformChar  SDKFileHelper_WINDELCHAR =('\\');
const static PlatformChar SDKFileHelper_GENERICDIRECTORYCHAR =('.');
const static PMString  SDKFileHelper_UNIXDELIMITER("/");
const static PMString   SDKFileHelper_WINDELIMITER ("\\");
const static PMString   SDKFileHelper_MACDELIMITER (":");
const static PMString   SDKFileHelper_UNIX_WINPARENTDIR ("..");
const static PMString   SDKFileHelper_MACPARENTDIR ("::");

/* Constructor
*/
SDKFileHelper::SDKFileHelper(const IDFile& sysFile) : fSysFile(sysFile),  fChosen(kFalse)
{
	initFromSysFile();
}

/*
*/
SDKFileHelper::SDKFileHelper(const PMString& path) : fPath(path), fChosen(kFalse)
{
	initFromString();
}

/*
*/
SDKFileHelper::SDKFileHelper() : fChosen(kFalse) 
{
}

/* Destructor
*/
SDKFileHelper::~SDKFileHelper() 
{
}

/*
*/
bool16 SDKFileHelper::IsChosen()  const
{ 
	return this->fChosen; 
}

/*
// prot.
*/
void SDKFileHelper::setChosen(bool16 chsn)
{
	this->fChosen = chsn;
}	

/*
*/
void SDKFileHelper::setIDFile(const IDFile& file) 
{
	this->fSysFile = file;
	this->fPath = FileUtils::SysFileToPMString(file);
}

/*
*/
void SDKFileHelper::setPath(const PMString& path) 
{
	this->fPath = path;
	FileUtils::PMStringToIDFile(path, this->fSysFile);  
	// ipaterso+ should be OK for SDKFileHelper to have a non-existent file, see IsExisting()
}


/*
*/
bool16 SDKFileHelper::IsExisting()  const
{
	bool16 retval = kFalse;
	do {
	
		InterfacePtr<IPMStream> pmStream(StreamUtil::CreateFileStreamRead(this->fSysFile));
		if (pmStream == nil) {
			break;
		}
		pmStream->Close();
		retval = kTrue;
	} while(false);
	return retval;
}

/*
*/
PMString SDKFileHelper::GetPath() const
{
	return FileUtils::SysFileToPMString(this->fSysFile);
}

/*
*/
IDFile SDKFileHelper::GetIDFile()  const
{
	return this->fSysFile;
}

/*
*/
void SDKFileHelper::initFromSysFile()
{
	this->fPath = FileUtils::SysFileToPMString(this->fSysFile);
}

/*
*/
void SDKFileHelper::initFromString()
{
	FileUtils::PMStringToIDFile(this->fPath, this->fSysFile);  
	// ipaterso+ should be OK for SDKFileHelper to have a non-existent file, see IsExisting()
}

/*
*/
void SDKFileHelper::traceInitializeDiagnostics(int32 e) const
{
	switch(e) {
		case 1:
			TRACEFLOW("SDKFileHelper","Invalid filename or volume name.");
		break;

        case 2:
			 TRACEFLOW("SDKFileHelper","FSMakeFSSpec() failed.");
  		break;

		case 3:
			 TRACEFLOW("SDKFileHelper","Invalid path or filename.");
 		break;

		case 4:
			TRACEFLOW("SDKFileHelper","Invalid dirID.");
      	break;

		case 5:
			TRACEFLOW("SDKFileHelper","Problem with delimiter.");
		break;
	}
}

/*
*/
PMString SDKFileHelper::calcDefaultFileName() const
{
	PMString docNameTemplate("Untitled", PMString::kTranslateDuringCall);
	// There is a translation for this in every local (in the application string-table)
	return docNameTemplate;
}

PMString SDKFileHelper::GetParentFolderAsString() const
{

	PMString retval;
	do {
  		PMString myPath = this->GetPath();
  		retval = this->getMinusLastElement(myPath);
  	} while(kFalse);

	return retval;
}

IDFile SDKFileHelper::GetAbsoluteFromRelative(const PMString& relativePath) const
{
	IDFile retval;
	do {
		PMString folderName = this->GetParentFolderAsString();

#ifdef MACINTOSH
		PMString path = this->getAsMacPath(relativePath); // the path might not be macintosh though
		PMString absolPath = this->absoluteFromRelative(folderName, path);
		ASSERT(absolPath.CharCount()>0);
		IDFile tmpSysFile;
		FileUtils::SpecInfoToIDFile(this->GetIDFile().GetVRefNum(), 0, absolPath, tmpSysFile);
#else 
		PMString path = this->getAsWinPath(relativePath);
		PMString absolPath = this->absoluteFromRelative(folderName, path);
		ASSERT(absolPath.CharCount()>0);
		IDFile tmpSysFile;
		tmpSysFile.SetString(absolPath);
#endif
		retval = tmpSysFile;

	} while(kFalse);

	return retval;
}


bool16 SDKFileHelper::isUnixPath(const PMString& path) const
{
	// if it has a mac or win delimeter then it can't be a unix path (at least not
	// one that is qualified by a directory)
	return !(path.Contains(SDKFileHelper_WINDELIMITER)|| path.Contains(SDKFileHelper_MACDELIMITER));
}
	



bool16 SDKFileHelper::isMacPath(const PMString& path) const
{
	// if it has a unix or win delimeter then it can't be a old-style (pre OS X) mac path (at least not
	// one that is qualified by a directory)
	return !(path.Contains(SDKFileHelper_WINDELIMITER)|| path.Contains(SDKFileHelper_UNIXDELIMITER));
}
	

bool16 SDKFileHelper::isWinPath(const PMString& path) const
{
	bool16 simpleCase = (path.Contains(SDKFileHelper_UNIXDELIMITER)|| (path.Contains(SDKFileHelper_MACDELIMITER)));
	if(!simpleCase) {
		// No alien chars in sight; fine
		return kTrue;
	}
	// We might have c: and I don't want to discount this possibility
	if( path.CharCount()>2 && path[0] != SDKFileHelper_MACDELCHAR  && path[1]== SDKFileHelper_MACDELCHAR && path[2]==SDKFileHelper_WINDELCHAR) {
		return kTrue;
	}
	
	return kFalse;
}
	

bool16 SDKFileHelper::isAbsolutePath(const PMString& path) const
{
	bool16 retval = kFalse;
	do {
		if(path.empty()) {
			break;
		}
		// if the path is a unix, win and mac path, it must be a relative filename (current dir)
		if (this->isUnixPath(path) && this->isMacPath(path)) {
			// two out of three ...
			retval = kFalse;
			break;
		}
		
		if ((this->isUnixPath(path)) && (path[0]==SDKFileHelper_UNIXDELCHAR)) {
			retval=kTrue;
			break;
		}
		
		// Accept C: etc.
		if ( (this->isWinPath(path)) && ( (path[0]==SDKFileHelper_WINDELCHAR) || 
													( path.CharCount()>2 && path[0] != SDKFileHelper_MACDELCHAR  
																				&& path[1]== SDKFileHelper_MACDELCHAR 
																				&& path[2]==SDKFileHelper_WINDELCHAR))) {
			retval = kTrue;
			break;
		}
		
		if ((this->isMacPath(path)) && (path[0] !=SDKFileHelper_MACDELCHAR))  {
#ifdef MACINTOSH	
			// mac paths are different, they don't start with a  colon
			IDFile	theFile;
			FileUtils::PMStringToIDFile(path, theFile);
			if (theFile.GetVRefNum() != 0)
				retval = kTrue;
#endif			
			break;
		}
		
	} while(kFalse);
	
	return retval;
}


PMString SDKFileHelper::getMinusLastElement(const PMString& path) const
{
	PMString retval = path;
	// takes off the last element from the path. We search for the last delimeter and knock off the
	// stuff that comes after. We lose this data so beware!
	int lastpos = 0;
	for (int i = 0 ; i< retval.CharCount();i++) {
		if ((retval[i] == SDKFileHelper_MACDELCHAR) || (retval[i] == SDKFileHelper_UNIXDELCHAR) || (retval[i] == SDKFileHelper_WINDELCHAR)) {
			lastpos = i;
		}
	}
	// At this point lastpos should point to the last delimeter, knock off the rest of the string.
	retval.Truncate(retval.CharCount() - lastpos);
	return retval;
}


PMString SDKFileHelper::getMinusFirstElement(const PMString& path) const
{
	PMString retval = path;
	do {
	
		// if it is a relative path, we might need to take off two delimeter chars...
		ASSERT(retval.CharCount()>=2);
		if (retval[0] == retval[1])
		{	
			if (retval[0] == SDKFileHelper_MACDELCHAR) // we know we start with a "::"
			{
				retval.Remove(0,2);
				break;
			} 
			
			if  (retval[0] == SDKFileHelper_GENERICDIRECTORYCHAR && retval[1] == SDKFileHelper_GENERICDIRECTORYCHAR) // we know we start with a ".."
			{
				ASSERT(retval.CharCount()>=3);
				retval.Remove(0,3);
				break;
			} 
			// TODO what if we have ./, say
		}
		// we have a name at the start if the path. Need to remove it...			
		// first of all, count the number of characters to the first delimeter. 
		int delpos = 0;
		for (int i = 0; i<retval.CharCount(); i++) {
		
			if ((retval[i] == SDKFileHelper_MACDELCHAR) || (retval[i] == SDKFileHelper_UNIXDELCHAR) || (retval[i] == SDKFileHelper_WINDELCHAR))
			{
				delpos = i;
				break;
			}
		}
		retval.Remove(0,delpos);
	} while(kFalse);
	
	return retval;
}


/*
*/
PMString SDKFileHelper::absoluteFromRelative(const PMString& folder, const PMString& path) const
{
	PMString retval;
	do {
	
		// this method takes in a base path, and a relative path. It merges the two to give a new absolute path.
		// I want to return the path in the same format as it's in. Success depends on the format of both the base
		// and relative bits being the same type.
		// Also the base path must be a directory name, not a filename
		if (this->isAbsolutePath(path))
		{	
			// Then we don't need to convert
			retval = path;
			break;
		}

		if (	!((this->isUnixPath(folder) && this->isUnixPath(path)) 
			|| (this->isMacPath(folder) && this->isMacPath(path)) 
			|| (this->isWinPath(folder) && this->isWinPath(path)))) {
			ASSERT(0); // The conversion failed since none of the paths match. We cannot convert
			break;
		}
		// At this point we know that the path types coincide. Need to strip off the elements coinciding with the "..". 
		// take local copies to work over...
		PMString thebase = folder;
		PMString thepath = path;
		while (thepath.Contains(SDKFileHelper_UNIX_WINPARENTDIR) || thepath.Contains(SDKFileHelper_MACPARENTDIR))
		{
			//strip off the last from the base, and the first from the path.
			thepath = this->getMinusFirstElement(thepath);
			thebase = this->getMinusLastElement(thebase);
		}
		// at this point, we need to merge the remaining parts of the path.
		retval = thebase;
		if ((this->isMacPath(folder) && this->isMacPath(path))) {
			retval.Append(SDKFileHelper_MACDELIMITER);
		}
		else {
			retval.Append(SDKFileHelper_WINDELIMITER);
		}
		retval.Append(thepath);
		
		} while(kFalse);
	return retval;
}


PMString SDKFileHelper::getAsMacPath(const PMString& path) const
{
	PMString retval=path;
	
	do {
	
		if (this->isMacPath(retval)) {
			break;
		}
		// Replace all instances of .. with ::
		int32 position = retval.IndexOfString(SDKFileHelper_UNIX_WINPARENTDIR);
		while (position!=-1)
		{
			retval.Remove(position,2); // remove the ".."
			retval.Insert(SDKFileHelper_MACPARENTDIR,position);
			position = retval.IndexOfString(SDKFileHelper_UNIX_WINPARENTDIR);
		}
		// first of all, get all the delimeters sorted.
		for (int32 i = 0; i<retval.CharCount(); i++)
			if ((retval[i] == SDKFileHelper_UNIXDELCHAR) || (retval[i] == SDKFileHelper_WINDELCHAR)) // want the mac delimeter
			{
				retval.Remove(i);
				// only do the insert if the next and previous chars aren't the SDKFileHelper_MACDELCHAR!
				if ((i>0) && (retval[i-1]!=SDKFileHelper_MACDELCHAR) && (retval[i+1] !=SDKFileHelper_MACDELCHAR))
					retval.Insert(SDKFileHelper_MACDELCHAR,i);
			}
	
	} while(kFalse);
	
	return retval;
}


PMString SDKFileHelper::getAsWinPath(const PMString& path) const
{
	PMString retval = path;
	do {
	
		if (this->isWinPath(retval)) {
			break;
		}
		// Replace all instances of :: with ..
		int32 position = retval.IndexOfString(SDKFileHelper_MACPARENTDIR);
		while (position!=(-1))
		{
			retval.Remove(position,2); // remove the "::"
			retval.Insert(SDKFileHelper_UNIX_WINPARENTDIR,position);
			retval.Insert(SDKFileHelper_WINDELCHAR,position+2); // replace it with "..\"
			position = retval.IndexOfString(SDKFileHelper_MACPARENTDIR);
		}

		//  get all the MAC delimeters sorted.
		bool16 haveReplacedDeviceChar = kFalse;
		const int32 devicePosition = 1;
		position = retval.IndexOfString(SDKFileHelper_MACDELIMITER);
		if(position == devicePosition)  {
			// i.e. we're looking at e.g. c:
			haveReplacedDeviceChar = kTrue;
		}
		while (position!=(-1))
		{
			retval.Remove(position); // remove the ":"
			retval.Insert(SDKFileHelper_WINDELIMITER,position); // and put in a '\'
			position = retval.IndexOfString(SDKFileHelper_MACDELIMITER);
		}
		
		if(haveReplacedDeviceChar) {
			// Then put it back.
			retval.Remove(devicePosition);
			retval.Insert(SDKFileHelper_MACDELIMITER, devicePosition);
		}
		// now get the unix delimeter sorted.
		position = retval.IndexOfString(SDKFileHelper_UNIXDELIMITER);
		while (position!=(-1))
		{
			retval.Remove(position); // remove the "/"
			retval.Insert(SDKFileHelper_WINDELIMITER,position); // and put in a '\'
			position = retval.IndexOfString(SDKFileHelper_UNIXDELIMITER);
		}
	} while(kFalse);
	return retval;
}


// --------------------------------- SDKFileOpenChooser -------------------------------------
/* Constructor.
*/
SDKFileOpenChooser::SDKFileOpenChooser() :
	fIsAllFilesEnabled(kFalse)
{
	this->SetTitle("Open a File");
}

/* Destructor.
*/
SDKFileOpenChooser::~SDKFileOpenChooser()
{
}

/* SDKFileOpenChooser's Filter Constructor.
*/
SDKFileOpenChooser::Filter::Filter(const SysOSType& macFileType, 
	const PMString& extension, 
	const PMString& filterName) : 
	fMacFileType(macFileType), 
	fExtension(extension), 
	fFilterName(filterName) 
{	
	this->fFilterName.Translate();
}

/* SDKFileOpenChooser's Filter Destructor.
*/
SDKFileOpenChooser::Filter::~Filter()
{
}

/*
*/
void SDKFileOpenChooser::AddAllFiles()
{
	fIsAllFilesEnabled = kTrue;
}

/*
*/
void SDKFileOpenChooser::AddFilter(const FileTypeInfoID& fileTypeInfoID, const PMString& filterName)
{
	FileTypeInfo currentFiletypeInfo = FileTypeRegistry::GetCurrentFileTypeInfo(fileTypeInfoID);
	fFilters.push_back(Filter(currentFiletypeInfo.GetFileType(), 
						currentFiletypeInfo.GetFileExtension(),
						filterName));
}

/*
*/
void SDKFileOpenChooser::AddFilter(const SysOSType& macFileType, const PMString& extension, const PMString& filterName)
{
	fFilters.push_back(Filter(macFileType, extension, filterName));
}

/*
*/
void SDKFileOpenChooser::SetTitle(const PMString& title)
{
	this->fTitle = title;
	this->fTitle.Translate();
}

/*
*/
void SDKFileOpenChooser::ShowDialog()
{
	do {
		this->setChosen(kFalse);

		// Create the open file dialog.
		InterfacePtr<IOpenFileDialog> openFileDialog(::CreateObject2<IOpenFileDialog>(kOpenFileDialogBoss));
		ASSERT(openFileDialog);
		if (!openFileDialog){
			break;
		}	

		// Set up the dialog to display and allow the file types
		// the caller has requested.
		if (fFilters.size() == 1 && !fIsAllFilesEnabled) 
		{ // If one filter, simplest way, just add, and we are done.
			openFileDialog->AddType(fFilters[0].fMacFileType);
			openFileDialog->AddExtension(&fFilters[0].fFilterName, &fFilters[0].fExtension);
		}
		else
		{
			if (fFilters.size() == 0 && !fIsAllFilesEnabled) 
			{
				// Default to browsing for publication documents
				// with a second option of all documents.
	#ifdef MACINTOSH
				PMString filterName("All Readable Documents");
	#endif
	#ifdef WINDOWS
				PMString filterName("PubFileFamily");
	#endif
				this->AddFilter(kPublicationFileTypeInfoID, filterName);
				this->AddAllFiles();
			}

			for(Filters::const_iterator iter = fFilters.begin(); iter < fFilters.end(); ++iter) 
			{
				openFileDialog->AddType(iter->fMacFileType);
	#ifdef MACINTOSH
				openFileDialog->AddCustomMenu(iter->fFilterName, kNavGenericSignature, iter->fMacFileType);
	#endif
				openFileDialog->AddExtension(&iter->fFilterName, &iter->fExtension);
			}

			if (fIsAllFilesEnabled) 
			{
				PMString filterName("All Documents", PMString::kTranslateDuringCall); 
	#ifdef MACINTOSH
				openFileDialog->AddCustomMenu(filterName, kAllFileTypes, kAllFileTypes);
	#endif
				openFileDialog->AppendAllFilesToFilterList();
			}
		}

		// Show the dialog to let the user choose a file.
		SysFileList resultFiles;
		if (openFileDialog->DoDialog
		(
			nil,			// Default file (none).
			resultFiles, 	// Our result list.
			kFalse,			// don't allow multiple selection
			&this->fTitle
		) != kTrue) {
			// the user cancelled.
			break;
		}
		
		// Remember the file the user chose.	
		this->setIDFile(*resultFiles.GetNthFile(0));
		this->setChosen(kTrue);
	} while(false);
}

// --------------------------------- SDKFileDocumentChooser -------------------------------------
/* Constructor.
*/
SDKFileDocumentChooser::SDKFileDocumentChooser() :
	fOpenFlags(IOpenFileCmdData::kOpenDefault)
{
}

/* Destructor.
*/
SDKFileDocumentChooser::~SDKFileDocumentChooser()
{
}

/*
*/
IOpenFileCmdData::OpenFlags SDKFileDocumentChooser::GetOpenFlags()
{
	return fOpenFlags;
}

/*
*/
void SDKFileDocumentChooser::ShowDialog()
{
	do {
		this->setChosen(kFalse);

		InterfacePtr<IOpenManager>	openManager(this->QueryOpenManager()); // no kDefaultIID
		ASSERT(openManager);
		if (!openManager) {
			break;
		}
			
		// Show the dialog to let the user choose a file.
		SysFileList resultFiles;
		if (openManager->DoOpenDialog
		(
			resultFiles, 	// Our result list.
			kFalse			// don't allow multiple selection
		) != kTrue) {
			// the user cancelled.
			break;
		}
		
		// Get the open flag set by the user in case the caller is interested.
		InterfacePtr<ITriStateData>	triStateData(openManager, IID_ITRISTATEDATA); // no kDefaultIID
		ASSERT(triStateData);
		if (!triStateData) {
			break;
		}
		switch (triStateData->GetState())
		{
			case ITriStateData::kUnknown:
				this->fOpenFlags = IOpenFileCmdData::kOpenDefault;
				break;
			case ITriStateData::kSelected:
				this->fOpenFlags = IOpenFileCmdData::kOpenOriginal;
				break;
			case ITriStateData::kUnselected:
				this->fOpenFlags = IOpenFileCmdData::kOpenCopy;
				break;
		}	
			
		// Remember the file the user chose.	
		this->setIDFile(*resultFiles.GetNthFile(0));
		this->setChosen(kTrue);
	} while(false);
}

/*
*/
IOpenManager* SDKFileDocumentChooser::QueryOpenManager()
{
	do {
		// Locate the open manager service via the service registry.
		InterfacePtr<IK2ServiceRegistry>	serviceRegistry(gSession, UseDefaultIID());
		ASSERT(serviceRegistry);
		if (!serviceRegistry) {
			break;
		}
		InterfacePtr<IK2ServiceProvider> openManagerService(serviceRegistry->QueryDefaultServiceProvider(kOpenManagerService));
		ASSERT(openManagerService);
		if (!openManagerService) {
			break;
		}
		InterfacePtr<IOpenManager>	openManager(openManagerService, IID_IOPENMANAGER); // no kDefaultIID
		ASSERT(openManager);
		if (!openManager) {
			break;
		}	
		return openManager.forget();
	} while(false);
	return nil;
}

// --------------------------------- SDKFileSaveChooser -------------------------------------
/* Constructor.
*/
SDKFileSaveChooser::SDKFileSaveChooser()
{
	this->SetTitle("Save As");
}

/* Destructor.
*/
SDKFileSaveChooser::~SDKFileSaveChooser()
{
}

/* SDKFileSaveChooser's Filter Constructor.
*/
SDKFileSaveChooser::Filter::Filter(const SysOSType& macFileCreator, 
	const SysOSType& macFileType, 
	const PMString& extension, 
	const PMString& filterName) : 
	fMacFileCreator(macFileCreator), 
	fMacFileType(macFileType), 
	fExtension(extension), 
	fFilterName(filterName)
{
		this->fFilterName.Translate();
}

/* SDKFileSaveChooser's Filter Destructor.
*/
SDKFileSaveChooser::Filter::~Filter()
{
}
	
/*
*/
void SDKFileSaveChooser::AddFilter(const FileTypeInfoID& fileTypeInfoID, const PMString& filterName)
{
	FileTypeInfo currentFiletypeInfo = FileTypeRegistry::GetCurrentFileTypeInfo(fileTypeInfoID);
	fFilters.push_back(Filter(currentFiletypeInfo.GetFileCreator(), 
						currentFiletypeInfo.GetFileType(), 
						currentFiletypeInfo.GetFileExtension(),
						filterName));
}

/*
*/
void SDKFileSaveChooser::AddFilter(const SysOSType& macFileCreator, const SysOSType& macFileType, const PMString& extension, const PMString& filterName)
{
	fFilters.push_back(Filter(macFileCreator, macFileType, extension, filterName));
}

/*
*/
void SDKFileSaveChooser::SetTitle(const PMString& title)
{
	this->fTitle = title;
	this->fTitle.Translate();
}

/*
*/
void SDKFileSaveChooser::SetFilename(const PMString& filename)
{
	this->fFilename = filename;
	this->fFilename.Translate();
}

/*
*/
void SDKFileSaveChooser::ShowDialog()
{
	do {
		this->setChosen(kFalse);

		// Create the dialog.
		InterfacePtr<ISaveFileDialog> saveFileDialog((ISaveFileDialog*) CreateObject(kSaveFileDialogBoss, IID_ISAVEFILEDIALOG)); // kDefaultIID missing
		ASSERT(saveFileDialog);
		if (!saveFileDialog) {
			break;
		}

		// If the caller has not indicated the type of file to
		// be saved then default to saving a publication file.
		if (fFilters.size() == 0) {
			PMString filterName("PubFileFamily"); // a string provided by the application.
			this->AddFilter(kPublicationFileTypeInfoID, filterName);
		}

		// Add the requested file types to the dialog.
		for(Filters::const_iterator iter = fFilters.begin(); iter < fFilters.end(); ++iter) {
			saveFileDialog->AddFileTypeInfo(iter->fFilterName, 
				iter->fMacFileCreator, 
				iter->fMacFileType, 
				iter->fExtension);
		}

		// Set up a default filename.
		PMString defaultFileName;
		if (fFilename.empty()) {
			defaultFileName = this->calcDefaultFileName(); 
		}
		else {
			defaultFileName = fFilename;
		}

#ifdef WINDOWS
		saveFileDialog->SetAdditionalFlags(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN);
		IDFile defaultFile;
		defaultFile.SetString(defaultFileName);
#endif
#ifdef MACINTOSH
		saveFileDialog->SetAdditionalFlags(kNavDontAddTranslateItems | kNavAllowPreviews | kNavDontAutoTranslate); 
		IDFile defaultFile;
		FileUtils::SpecInfoToIDFile(0, 0, defaultFileName, defaultFile);
#endif

		// Show the dialog to let the user choose a file.
		int32 selectedIndex = 0;
		IDFile resFile;
		if(saveFileDialog->DoDialog
			(
				&defaultFile, 
				&resFile, 
				&selectedIndex, 
				kTrue, // Use system default folder.
				kTrue, // Show type menu.
				&this->fTitle
			) == kFalse)  
		{
			// Cancel.
			break;
		}

		// Remember the file the user chose.
		this->setIDFile(resFile);
		this->setChosen(kTrue);

	} while(false);
}

// --------------------------------- SDKFilePlaceChooser -------------------------------------
/* Constructor
*/
SDKFilePlaceChooser::SDKFilePlaceChooser()
{
	// Initialise the import manager's options cached by this object.
	InterfacePtr<IImportManagerOptions> importManagerOptions(this->queryImportManagerOptions());
	ASSERT(importManagerOptions);
	if(importManagerOptions) {
		fShowFilterPrefs = importManagerOptions->GetShowFilterPrefs();
		fRetainFormat = importManagerOptions->GetRetainFormat();
		fConvertQuotes = importManagerOptions->GetConvertQuotes();
		fReplaceSelectedItem = importManagerOptions->GetReplaceSelectedItem();
		fApplyCJKGrid = importManagerOptions->GetApplyCJKGrid();
		fShowPreview = importManagerOptions->GetShowPreview();
	}
}

/* Destructor
*/
SDKFilePlaceChooser::~SDKFilePlaceChooser()
{

}

/*
*/
IImportManager* SDKFilePlaceChooser::queryImportManager()
{
	do {
		InterfacePtr<IK2ServiceRegistry> serviceRegistry(gSession, UseDefaultIID());
		ASSERT(serviceRegistry);
		if (serviceRegistry == nil) {
			break;
		}
		InterfacePtr<IK2ServiceProvider> importManagerService(serviceRegistry->QueryDefaultServiceProvider(kImportManagerService));
		ASSERT(importManagerService);
		if (importManagerService == nil) {
			break;
		}
		InterfacePtr<IImportManager> importManager(importManagerService, UseDefaultIID());
		ASSERT(importManager);
		if (importManager == nil) {
			break;
		}
	
		return importManager.forget();
	} while(false);
	return nil;
}

/*
*/
IImportManagerOptions* SDKFilePlaceChooser::queryImportManagerOptions()
{
	do {
		InterfacePtr<IImportManager> importManager(this->queryImportManager());
		if (importManager == nil) {
			break;
		}
		InterfacePtr<IImportManagerOptions> importManagerOptions(importManager, IID_IIMPORTMANAGEROPTIONS); // no kDefaultIID
		ASSERT(importManagerOptions);
		if (importManagerOptions == nil) {
			break;
		}
		return importManagerOptions.forget();
	} while(false);
	return nil;
}

/*
*/
void SDKFilePlaceChooser::ShowDialog()
{
	do {
		this->setChosen(kFalse);

		// Set up the import manager options.
		InterfacePtr<IImportManager> importManager(this->queryImportManager());
		ASSERT(importManager);
		if (importManager == nil) {
			break;
		}
		InterfacePtr<IImportManagerOptions> importManagerOptions(importManager, IID_IIMPORTMANAGEROPTIONS); // no kDefaultIID
		ASSERT(importManagerOptions);
		if(!importManagerOptions) {
			break;
		}
		importManagerOptions->Set(fShowFilterPrefs, fRetainFormat, fConvertQuotes);
		importManagerOptions->SetReplaceSelectedItem(fReplaceSelectedItem);
		importManagerOptions->SetApplyCJKGrid(fApplyCJKGrid);
		importManagerOptions->SetShowPreview(fShowPreview);

		// Show the dialog to let the user choose a file.
		SysFileList files;
		if (!importManager->DoImportDialog(&files, kFalse)) {
			// the user cancelled.
			break;
		}

		// Save the options selected by the user in case caller is interested.
		fShowFilterPrefs = importManagerOptions->GetShowFilterPrefs();
		fRetainFormat = importManagerOptions->GetRetainFormat();
		fConvertQuotes = importManagerOptions->GetConvertQuotes();
		fReplaceSelectedItem = importManagerOptions->GetReplaceSelectedItem();
		fApplyCJKGrid = importManagerOptions->GetApplyCJKGrid();
		fShowPreview = importManagerOptions->GetShowPreview();

		// Finish up.
		this->setIDFile(*files.GetNthFile(0));
		this->setChosen(kTrue);
	} while(false);
}

/*
*/
bool16 SDKFilePlaceChooser::GetShowFilterPrefs() const
{
	return fShowFilterPrefs;
}

/*
*/
void SDKFilePlaceChooser::SetShowFilterPrefs(bool16 showFilterPrefs)
{
	this->fShowFilterPrefs = showFilterPrefs;
}

/*
*/
bool16 SDKFilePlaceChooser::GetRetainFormat() const
{
	return fRetainFormat;
}

/*
*/
void SDKFilePlaceChooser::SetRetainFormat(bool16 retainFormat)
{
	this->fRetainFormat = retainFormat;
}

/*
*/
bool16 SDKFilePlaceChooser::GetConvertQuotes() const	
{
	return fConvertQuotes;
}

/*
*/
void SDKFilePlaceChooser::SetConvertQuotes(bool16 convertQuotes)
{
	this->fConvertQuotes = convertQuotes;
}

/*
*/
bool16 SDKFilePlaceChooser::GetReplaceSelectedItem() const
{
	return fReplaceSelectedItem;
}

/*
*/
void SDKFilePlaceChooser::SetReplaceSelectedItem(bool16 replaceSelected)
{
	this->fReplaceSelectedItem = replaceSelected;
}

/*
*/
bool16 SDKFilePlaceChooser::GetApplyCJKGrid() const
{
	return fApplyCJKGrid;
}

/*
*/
void SDKFilePlaceChooser::SetApplyCJKGrid(bool16 applyGrid)
{
	this->fApplyCJKGrid = applyGrid;
}

/*
*/
bool16 SDKFilePlaceChooser::GetShowPreview() const
{
	return fShowPreview;
}

/*
*/
void SDKFilePlaceChooser::SetShowPreview(bool16 showPreview)
{
	this->fShowPreview = showPreview;
}



// --------------------------------- SDKFolderChooser -------------------------------------
/* Constructor.
*/
SDKFolderChooser::SDKFolderChooser()
{
	this->SetTitle(PMString("Choose a Folder"));
}

/* Destructor.
*/
SDKFolderChooser::~SDKFolderChooser()
{
}

/*
*/
void SDKFolderChooser::ShowDialog()
{
	do {
		this->setChosen(kFalse);

		// Create the dialog.
		InterfacePtr<ISelectFolderDialog> selectFolderDialog(CreateObject2<ISelectFolderDialog>(kSelectFolderDialogBoss));
		ASSERT(selectFolderDialog);
		if(!selectFolderDialog) {
			break;
		}

#ifdef WINDOWS
		selectFolderDialog->SetBROWSEINFOFlags(BIF_DONTGOBELOWDOMAIN);
#endif
#ifdef MACINTOSH
		selectFolderDialog->SetNavDlgOpFlags(kNavDontAddTranslateItems | kNavAllowStationery | kNavDontAutoTranslate);
#endif

		// Show the dialog to let the user choose a folder.
		SysFileList     resultFolders;
		bool16 folderSelected =  selectFolderDialog->DoDialog(nil, 
															  resultFolders, 
															  kFalse, // only one selection
															  nil,
															  &(this->fTitle));

		if(!folderSelected) {
			// Cancelled.
			break;
		}
		if(resultFolders.GetFileCount() != 1) {
			// We only want one result folder
			break;
		}

		// Remember the folder the user chose.	
		this->setIDFile(*resultFolders.GetNthFile(0));
		this->setChosen(kTrue);

	} while(false);

}

/*
*/
void SDKFolderChooser::SetTitle(const PMString& title)
{
	this->fTitle = title;
	this->fTitle.Translate();
}

/*
*/
void SDKFolderChooser::initFromSysFile()
{
#ifdef WINDOWS
	SDKFileHelper::initFromSysFile();
#endif
#ifdef MACINTOSH
	// We can't pass this IDFile representing a folder
	// to CoreFilename implementation on account of some bug so
	// there's a cheap workaround below
	this->macInitFolderFromSysFile();
#endif
}

#ifdef MACINTOSH
/*
// (irp) Edge code to workaround apparent problem in CoreFilename; I
// can't successfully send the msg ICoreFilename::Initialize given a  
// IDFile representing a folder path
// so what I have to do is calculate a PMString that represents the
// path and then call initFromString with this path. What the problem
// appears to be is that the path representation by default that gets 
// calculated in the CoreFilename has a trailing : and this seems to
// result in an error (somehow CoreFilename thinks this is 'invalid' even though it
// calculated this itself from the IDFile)... so below  I check that the filename is non-empty
// before adding  a trailing colon... if you don't do this you'll get an assert
// from ICoreFilename::Initialize
//
//TODO: I doubt this is needed sdai
*/
void SDKFolderChooser::macInitFolderFromSysFile()
{

	do {
		IDFile spec = this->GetIDFile();
		PMString resultant;	
		CInfoPBRec pb;
		SysChar dirName[256]; // largest mac filepath/name == 256
		PMString directoryName;
		OSErr err;
		
		// Keep going up the directory path, getting the name of each node and 
		// appending it to the filename, until we get to the root.
		pb.dirInfo.ioNamePtr = dirName;
		pb.dirInfo.ioVRefNum = spec.GetVRefNum();
		pb.dirInfo.ioDrParID = spec.GetParId();
		pb.dirInfo.ioFDirIndex  = -1;
		
		PMString fileName(spec.GetFileName());
		bool16 firstIteration = kTrue;
		do {
			pb.dirInfo.ioFDirIndex = -1; 
			pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
			err = PBGetCatInfoSync(&pb);
			if (err != noErr)
			{
				break;
			}
			
			directoryName.SetPString(dirName, PMString::kUnknownEncoding);
			// Don't append a directory char if folder or there will be a prob in ICoreFilename::Initialize 
			// The first time around this loop we're at the leaf
			if( ! (firstIteration && fileName.empty())) {
				directoryName.Append(SDKFileHelper_MACDELIMITER);
			}		
			resultant.Insert(directoryName);
			firstIteration = kFalse;
		} while (pb.dirInfo.ioDrDirID != fsRtDirID); // while we're not at the root
		
		resultant.Append(fileName);
		this->setPath(resultant);
	} while(false);
}
#endif
	

// End, SDKFileHelper.cpp
