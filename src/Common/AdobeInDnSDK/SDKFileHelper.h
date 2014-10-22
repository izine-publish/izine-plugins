//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/common/SDKFileHelper.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: rajkumar.sehrawat $
//  
//  $DateTime: 2007/02/15 13:27:55 $
//  
//  $Revision: 1288 $
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

#ifndef __SDKFileHelper_H_DEFINED__
#define __SDKFileHelper_H_DEFINED__

#include "IOpenFileCmdData.h"
class IOpenManager;
class IImportManagerOptions;
class IImportManager;

/** 
	Cross-platform abstraction of file that eases working with files. Intended to obviate
	need for many of the methods in SDKUtilities. Becomes particularly powerful when
	the mechanism to choose a file is considered, eliminating the need for difficult to
	write boilerplate to parameterise the file selection dialogs.
	
	
	@ingroup sdk_common
*/
class SDKFileHelper {

friend class SDKFileHelperUnitTest;

public:


	/** Constructor to use when the path is already known as a string.
		@param path path to file of interest
		@pre path should exist and should not be an empty string
		@post GetIDFile will return a IDFile giving file's location 
		@post GetPath will return the path used in constructor argument
	*/
	SDKFileHelper(const PMString& path);

	/** Constructor if a IDFile is available 
		@param file specifies the file of interest
		@pre file should be valid
		@post GetIDFile will return a IDFile used in constructor argument 
		@post GetPath will return the path of the file 
	*/
	SDKFileHelper(const IDFile& file);

	/** Destructor */
	virtual ~SDKFileHelper();
	
	/** Tests whether the file exists. If the file has not yet been created or
		an invalid path or IDFile has been specified then this should be false.
		
		@return kTrue if file exists, kFalse otherwise
	*/
	bool16 IsExisting() const;

	/**  Determine if a IDFile has been acquired as a result of a 'Choice' by the user. Convenience method for the
		SDKxxxChooser methods to represent whether a choice occurred and expose this.
		@return kTrue if this file has been chosen, kFalse otherwise (e.g. if it has been initialised by a string/IDFile in 
		the constructor
	*/
	bool16 IsChosen() const;

	/** Return a string-based representation of the path to this file.
		@return path associated with this file 
	*/
	PMString GetPath() const;

	/** Return a IDFile that represents the file location. Note that IDFile is a PMString on Windows, 
		but on Macintosh  encapsulates the 'old' Mac file system specifics.
	
		@return IDFile corresponding to the file
	*/
	IDFile GetIDFile() const;

	/** Return a IDFile that represents a file relative to this file. Replaces the old SDKUtilities::GetAbsolutePath which was
		potentially confusing to use.
		
		@param relativePath for instance, ../images/something.gif
		@return IDFile representing relative location
	*/
	IDFile GetAbsoluteFromRelative(const PMString& relativePath) const;
	
	/** @return path to folder containing this file.
	 */
	PMString GetParentFolderAsString() const;
	
protected:
	/** Default constructor: invoked only from subclasses*/
	SDKFileHelper();
	

	/**  May be invoked from this or derived class when we have the path as a PMString and want to find a corresponding IDFile
		@pre this->fPath is not empty
		@post this->fSysFile should refer to a valid file
	*/
	void initFromString();


	/**  May be invoked from this or derived class when we have file specified as IDFile and want PMString equivalent
		@pre this->fSysFile refers to valid file
		@post this->fPath is not empty
	*/
	void initFromSysFile();


	/**  Report problems in creating a IDFile from a PMString 
		@param e IN error number for core filename initialisation (see ICoreFilename)
	*/
	void traceInitializeDiagnostics(int32 e) const;
		
	/**  Helper to provide a default file name (Untitled, translated per locale, for instance).
		@return a string that can be used as a default file name e.g. for save operation
	*/
	PMString calcDefaultFileName() const;
	
	/**  Mutator for the 'chosen' attribute; this is protected because client code should not be
		able to indicate that an end-user has made a choice.
		@param chsen IN specifies the new state for this attribute
	*/
	void setChosen(bool16 chsen);
	
	/**  Mutator for the IDFile stored in the instance; deliberately not exposed to client code. We need
		this method to let the derived classes that return a IDFile from a choice set the state of the
		instance.
		
		@param file IN path in file system to file.
		@pre IDFile valid
		@post the fPath won't be initialised, only fSysFile changed
	*/
	void setIDFile(const IDFile& file);

	/**  Mutator for the PMString path stored in the instance; deliberately not exposed to client code. We need
		
		@param path IN path to file
		@pre should be valid path in filesystem
		@post the fSysFile won't be initialised, only fPath changed
	*/
	void setPath(const PMString& path);

	/** Make an absolute path given a folder and a relative path 
		@param folder folder relative to which we're going to navigate
		@param path specifies path of interest
		@return absolute path representation of supplied path if we could do it, empty if not
	*/
	PMString absoluteFromRelative(const PMString& folder, const PMString& path) const;
	
	
	/** Determine if the passed in string represents an absolute pathname.
		A win absolute pathname looks like "\\this\\that\\file"
		A mac absolute pathname looks like "this:that:file"
		a unix absolute pathname looks like "/this/that/file"
		@param path the path passed in.
		@return kTrue if the string represents an absolute pathname 
	*/
	bool16 isAbsolutePath(const PMString& path) const;
		
	/** Determines if the passed in string represents a UNIX path.
	This is not a firm test of what constitutes a UNIX path, more just a heuristic that answers, "given
	a path, is it plausible it is a UNIX path?".
	@param path the path used for the test. 
	@return true if the path could be a unix path.
	*/
	bool16 isUnixPath(const PMString& path) const;

	/** Determines whether the path passed in can be a mac path. 
	This is not a firm test of what constitutes a mac path, more just a heuristic that answers, "given
	a path, is it plausible it is a mac path?".
	@param path the path used for the test.
	@return true if this string could be a mac path. 
    */
	bool16 isMacPath(const PMString& path) const;

	/** Determines whether the path passed in could be a win path.
	This is not a firm test of what constitutes a win path, more just a heuristic that answers, "given
	a path, is it plausible it is a win path?".
	@param path the path used for the test.
	@return true if this string could be a win path. 
	*/
	bool16 isWinPath(const PMString& path) const;

	/** return string minus the last element from the pathname.
	So "\\this\\that\\file" becomes "\\this\\that"
	@param path
	@return shortened from
	*/
	PMString getMinusLastElement(const PMString& path) const;

	/** Return string minus the first element from the pathname.
	So "\\this\\that\\file" becomes "\\that\\file" and "../../file" becomes "../file"
	@param path
	@return shortened form
	*/
	PMString getMinusFirstElement(const PMString& path)  const;
		
	/** Return the path as a Macintosh format path (pre OS X) 
		@param path specifies the path of interest
		@return representation in Mac format
	*/
	PMString getAsMacPath(const PMString& path) const;

	/** Return the path as a WIndows format path (not UNC)
		@param path specifies the path of interest
		@return representation in Windows format
	*/
	PMString getAsWinPath(const PMString& path) const;

private:
	PMString fPath;
	IDFile fSysFile;
	bool16 fChosen;
};


/**	Provides a mechanism for the user to choose a file to be opened and allows
	filtering of the files presented, note this class does not actually open the
	file, it just lets the user choose a file.

	If you do not provide file filter, by default, the file open chooser allow you
	choose either publication files (InDesign documents if you are running under InDesign, 
	InCopy documents if you are running under InCopy) or all files. See code below:
	<pre>
	SDKFileOpenChooser fileChooser;
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>

	The code below pops a file open chooser that shows jpg or gif files.
	<pre>
	SDKFileOpenChooser fileChooser;
	fileChooser.AddFilter(kJPEGFileTypeInfoID, "JPEG files(jpg)");
	fileChooser.AddFilter(kGIFFileTypeInfoID, "GIFf files(gif)");
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>
	
	Note that the filter names, e.g. "JPEG files(jpg)", must either
	be translatable string keys with a value in a StringTable
	(or in no translation StringTable). Manully call PMString::SetTranslatable(kFalse)
	no longer works.
	You do not need to provide translation for file extensions.

	The code below pops a file open chooser for a text or txt file. Note
	that the FileTypeRegistry uses the extension "text" for text files.
	In the code below an additional filter is applied that will show files
	with the extension "txt" aswell.
	<pre>
	SDKFileOpenChooser fileChooser;
	fileChooser.AddFilter(kTEXTFileTypeInfoID, "Text file(text)");
	fileChooser.AddFilter('TEXT', "txt", "Text file(txt)");
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>

	
	@see ShuksanID.h for available FileTypeInfoID's
	@see FileTypeRegistry for access to FileTypeInfo
	@see IOpenFileDialog
	@see kOpenFileDialogBoss
	@ingroup sdk_common
 */
class SDKFileOpenChooser : public SDKFileHelper {
public:

	/** Constructor.
	*/
	SDKFileOpenChooser();
	
	/** Destructor.
	*/
	virtual ~SDKFileOpenChooser();

	/**	Add filter by FileTypeInfoID, if the FileTypeRegistry knows about the kind of file you want 
		use this call.
		@param fileTypeInfoID IN FileTypeRegistry ID, for example kXMLFileTypeInfoID, 
			see ShuksanID.h for available IDs.
		@param filterName IN name displayed in the UI to let the user to apply the filter, 
			(must be translatable strings).
		@post the user will be able to apply this filter to control the files shown 
			by SDKFileOpenChooser::ShowDialog.
	*/
	void AddFilter(const FileTypeInfoID& fileTypeInfoID, const PMString& filterName);
	
	/** Add filter by macFileType and extension, if you know the specific Mac file type and the Win
		file extension you want you can use this call. It's worth checking if the FileTypeRegistry has a 
		FileTypeInfoID for the kind of file you want before using this call, then you can avoid
		hard wiring file type and extension information in your code.
		@param macFileType IN Mac file type, 'TEXT' or 'JPEG' for example.
		@param extension IN file extension, "txt" or "jpg" for example.
		@param filterName IN name displayed in the UI to let the user to apply the filter, 
			(must be translatable strings).
		@post the user will be able to apply this filter to control the files shown 
			by SDKFileOpenChooser::ShowDialog.
	*/
	void AddFilter(const SysOSType& macFileType, const PMString& extension, const PMString& filterName);
	
	/**	Add filter that allows the user to turn off filtering and show all files.
		@post the user will be able to turn off filtering 
			when SDKFileOpenChooser::ShowDialog is called.
	*/
	void AddAllFiles();
	
	/**	Set the title of the dialog.
		@param title IN dialog title (translatable string).
	*/
	void SetTitle(const PMString& title);
	
	/**	Pops a file browse dialog to allow the user to choose a file to be opened. If any filters have been
		specified by the caller (SDKFileOpenChooser::AddFilter, SDKFileOpenChooser::AddAllFiles), the
		user will be able to apply them to control the files shown in the chooser.
		Otherwise two default filters will be made available; the first filter will show
		publication documents (InDesign documents under InDesign, InCopy documents under InCopy);
		the second filter will show all files.
		@post SDKFileHelper::IsChosen is kTrue if a file was chosen, kFalse otherwise.
		@post SDKFileHelper::GetIDFile contains the chosen file if SDKFileHelper::IsChosen is kTrue.
		@post SDKFileHelper::GetPath contains the path to the chosen file if SDKFileHelper::IsChosen is kTrue.
		@see IOpenFileDialog
		@see kOpenFileDialogBoss
	*/
	void ShowDialog();

private:
	/**	Represents the filter capability provided by IOpenFileDialog/kOpenFileDialogBoss.
		@see IOpenFileDialog.
		@see kOpenFileDialogBoss.

	 */
	class Filter {
		public:
			typedef object_type data_type;	
		public:
		Filter(const SysOSType& macFileType, 
			const PMString& extension, 
			const PMString& filterName);
		virtual ~Filter();
		SysOSType fMacFileType;
		PMString	fExtension;
		PMString	fFilterName; 
	};

	/** List of Filter's.
	 */
	typedef K2Vector<Filter> Filters;

private:
	Filters fFilters;
	PMString fTitle;
	bool16 fIsAllFilesEnabled;
	IOpenFileCmdData::OpenFlags fOpenFlags;
};

/**	Pops an open document dialog to allow the user to choose a document,
	note that this class does not actually open the chosen file, it just lets you
	discover which file was chosen. The files presented include all readable document files,
	publication, template, book and library files for example.

	The dialog used(kOpenDocDialogBoss) is the same one used by the application's File>Open menu.
	This dialog is managed by the IOpenManager service. Each type of document the application can open
	has a corresponding open provider (an IImportProvider service with ServiceID=kOpenProviderService).
	All the kinds of files supported by registered open providers will be available in the dialog.
	
	Once the user has chosen a document file you can use the IOpenManager service, kOpenFileCmdBoss
	or kOpenFileWithWindowCmdBoss to open it. Note that opening one of these files will not necessarily 
	give you an ordinary document, you might be opening a library or a book for example.

	The code below pops a document chooser dialog that shows all readable documents.
	<pre>
	SDKFileDocumentChooser fileChooser;
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
		IOpenFileCmdData::OpenFlags openFlags = fileChooser.GetOpenFlags();
	}
	</pre>

	
	@see IOpenManager
	@see kOpenDocDialogBoss
	@see IOpenFileDialog
	@see IOpenFileCmdData
	@see kOpenFileWithWindowCmdBoss
	@see kOpenFileCmdBoss
	@ingroup sdk_common
 */
class SDKFileDocumentChooser : public SDKFileHelper {
public:

	/** Constructor.
	*/
	SDKFileDocumentChooser();
	
	/** Destructor.
	*/
	virtual ~SDKFileDocumentChooser();
		
	/**	Pops an open document dialog to allow the user to choose a document to be opened.
	
		@post SDKFileHelper::IsChosen is kTrue if a document was chosen, kFalse otherwise.
		@post SDKFileHelper::GetIDFile contains the chosen document if SDKFileHelper::IsChosen is kTrue.
		@post SDKFileHelper::GetPath contains the path to the chosen document if SDKFileHelper::IsChosen is kTrue.
		@see IOpenFileDialog
		@see kOpenDocDialogBoss
	*/
	void ShowDialog();
	
	/**	Return the open flag specified by the user in the open document dialog.
		@return the open flag specified by the user in the open document dialog.
		@precondition SDKFileDocumentChooser::ShowDialog should be called before calling this method.
	*/
	IOpenFileCmdData::OpenFlags GetOpenFlags();

	/** Query the service registry for the IOpenManager service.
		@return IOpenManager interface on success, nil otherwise.
		@post the caller is responsible for releasing the interface reference.
	 */
	IOpenManager* QueryOpenManager();

private:
	IOpenFileCmdData::OpenFlags fOpenFlags;
};

/**	Provides a mechanism for the user to choose a file to be saved to and allows
	filtering of the files presented, note this class does not actually save the
	file, it just lets the user choose a file.

	The code below pops a file save chooser dialog for a publication file
	(an InDesign document if you are running under InDesign, an InCopy document if
	you are running under InCopy).
	<pre>
	SDKFileSaveChooser fileChooser;
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>

	The code below pops a file save chooser for a jpg or gif file.
	<pre>
	SDKFileSaveChooser fileChooser;
	fileChooser.AddFilter(kJPEGFileTypeInfoID, "JPEG file(jpg)");
	fileChooser.AddFilter(kGIFFileTypeInfoID, "GIF file(gif)");
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>

	Note that the filter names, e.g. "JPEG files(jpg)", must 
	be translatable string keys with a value in a StringTable.
	
	@see ShuksanID.h for available FileTypeInfoID's
	@see FileTypeRegistry for access to FileTypeInfo
	@see ISaveFileDialog
	@see kSaveFileDialogBoss
	@ingroup sdk_common
 */
class SDKFileSaveChooser : public SDKFileHelper {
public:
	/** Constuctor
	*/
	SDKFileSaveChooser();
	
	/** Destructor
	*/
	virtual ~SDKFileSaveChooser();
	
	/**	Add filter by FileTypeInfoID, if the FileTypeRegistry knows about the kind of file you want 
		use this call.
		@param fileTypeInfoID IN FileTypeRegistry ID, for example kXMLFileTypeInfoID, 
			see ShuksanID.h for available IDs.
		@param filterName IN name displayed in the UI to let the user to apply the filter 
			(must be translatable strings).
		@post the user will be able to apply this filter to control the files shown 
			by SDKFileSaveChooser::ShowDialog.
	*/
	void AddFilter(const FileTypeInfoID& fileTypeInfoID, const PMString& filterName);

	/** Add filter by macFileCreator, macFileType and extension, if you know the specific Mac file creator, Mac file type and the Win
		file extension you want you can use this call. It's worth checking if the FileTypeRegistry has a 
		FileTypeInfoID for the kind of file you want before using this call, then you can avoid
		hard wiring file creator, type and extension information in your code.
		@param macFileCreator IN Mac file creator, 'CWIE' for example.
		@param macFileType IN Mac file type, 'TEXT' or 'JPEG' for example.
		@param extension IN file extension, "txt" or "jpg" for example.
		@param filterName IN name displayed in the UI to let the user to apply the filter (must be translatable strings). 
		@post the user will be able to apply this filter to control the files shown 
			by SDKFileSaveChooser::ShowDialog.
	*/
	void AddFilter(const SysOSType& macFileCreator, const SysOSType& macFileType, const PMString& extension, const PMString& filterName);
	
	/**	Set the title of the dialog.
		@param title IN dialog title (translatable string).
	*/
	void SetTitle(const PMString& title);
	
	/** Set the name of the file suggested to the user.
		@param filename IN name of the file suggested to the user as the file to save to 
		(translatable string keys passed here will be translated before display in the dialog).
	*/
	void SetFilename(const PMString& filename);

	/**	Pops a file browse dialog to allow the user to choose a file to be saved to. If any filters have been
		specified by the caller (SDKFileSaveChooser::AddFilter, SDKFileSaveChooser::AddAllFiles), the
		user will be able to apply them to control the files shown in the chooser. Otherwise a filter will be 
		applied that will show publication documents (InDesign documents under InDesign, InCopy documents under InCopy).

		@post SDKFileHelper::IsChosen is kTrue if a file was chosen, kFalse otherwise.
		@post SDKFileHelper::GetIDFile contains the chosen file if SDKFileHelper::IsChosen is kTrue.
		@post SDKFileHelper::GetPath contains the path to the chosen file if SDKFileHelper::IsChosen is kTrue.
		@see ISaveFileDialog
		@see kSaveFileDialogBoss
	*/
	void ShowDialog();

private:
	/**	Represents the filter capability provided by ISaveFileDialog/kSaveFileDialogBoss.
		@see ISaveFileDialog.
		@see kSaveFileDialogBoss.
	 */
	class Filter {
		public:
			typedef object_type data_type;	
		public:
		Filter(const SysOSType& macFileCreator, 
			const SysOSType& macFileType, 
			const PMString& extension, 
			const PMString& filterName);
		virtual ~Filter();
		SysOSType fMacFileCreator;
		SysOSType fMacFileType;
		PMString	fExtension;
		PMString	fFilterName; 
	};

	/** List of Filter's.
	 */
	typedef K2Vector<Filter> Filters;

private:

	Filters fFilters;
	PMString fTitle;
	PMString fFilename;
};

/** Provides a mechanism for the user to choose a file to be placed,
	note this class does not actually place the file, 
	it just lets the user choose a file.

	The code below pops a file place chooser dialog for all importable files.
	<pre>
	SDKFilePlaceChooser fileChooser;
	fileChooser.ShowDialog();
	if (fileChooser.IsChosen()) {
		IDFile chosenFile = fileChooser.GetIDFile();
	}
	</pre>

	The options available on the file place chooser dialog can be set by
	the caller before the dialog is opened. For example call 
	SDKFilePlaceChooser::SetShowPreview to control the state of the preview
	checkbox. Once the dialog has been dismissed the options given by
	the user can be obtained, for example call SDKFilePlaceChooser::SetShowFilterPrefs
	to see if the import provider's option dialog should be shown when the file
	is actually imported.

	
	@see IImportManager
	@see IImportManagerOptions.
	@ingroup sdk_common
 */
class SDKFilePlaceChooser : public SDKFileHelper {
public:
	/** Constructor.
	 */
	SDKFilePlaceChooser();
	/** Destructor.
	 */
	virtual ~SDKFilePlaceChooser();

public:
	/**	Pops a file browse dialog to allow the user to choose a file to be placed. 

		@post SDKFileHelper::IsChosen is kTrue if a file was chosen, kFalse otherwise.
		@post SDKFileHelper::GetIDFile contains the chosen file if SDKFileHelper::IsChosen is kTrue.
		@post SDKFileHelper::GetPath contains the path to the chosen file if SDKFileHelper::IsChosen is kTrue.
		@post SDKFilePlaceChooser contains the state of the IImportManagerOptions entered by the
			user, for example call SDKFilePlaceChooser::GetShowFilterPrefs to determine if the import
			filter's options dialog should be shown when the file is imported.
		@see IImportManager
		@see IImportManagerOptions
	*/
	void ShowDialog();

public:
	// Mutators for the place file dialog options.

	/** Set state of IImportManagerOption's showFilterPrefs option.
		@param showFilterPrefs IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetShowFilterPrefs(bool16 showFilterPrefs);
	
	/**	Set state of IImportManagerOption's retainFormatoption.
		@param retainFormat IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetRetainFormat(bool16 retainFormat);
	
	/**	Set state of IImportManagerOption's convertQuotes option.
		@param convertQuotes IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetConvertQuotes(bool16 convertQuotes);

	/**	Set state of IImportManagerOption's replaceSelectedItem option.
		@param replaceSelectedItem IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetReplaceSelectedItem(bool16 replaceSelectedItem);
	
	/**	Set state of IImportManagerOption's applyCJKGrid option.
		@param applyGrid IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetApplyCJKGrid(bool16 applyGrid);
	
	/**	Set state of IImportManagerOption's showPreview option.
		@param showPreview IN kTrue if dialog checkbox should be checked, kFalse unchecked.
	*/
	void SetShowPreview(bool16 showPreview);

public:
	// Accessors for the place file dialog options.

	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetShowFilterPrefs() const;
	
	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetRetainFormat() const;
	
	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetConvertQuotes() const;	
	
	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetReplaceSelectedItem() const;
	
	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetApplyCJKGrid() const;
	
	/**	@return kTrue if this option's checkbox on the place file dialog was checked, kFalse unchecked.
	*/
	bool16 GetShowPreview() const;

protected:
	/** Query the service registry for the IImportManager service.
		@return IImportManager interface on success, nil otherwise.
		@post the caller is responsible for releasing the interface reference.
	 */
	IImportManager* queryImportManager();

	/** Query the service registry for the import manager service's
		options.
		@return IImportManagerOptions interface on success, nil otherwise.
		@post the caller is responsible for releasing the interface reference.
	 */
	IImportManagerOptions* queryImportManagerOptions();

private:
	// IImportManagerOptions cached by this object so that the caller
	// can set the options before the dialog is opened and find out the
	// options the user specified once the dialog is dismissed.
	bool16 fShowFilterPrefs;
	bool16 fRetainFormat;
	bool16 fConvertQuotes;	
	bool16 fReplaceSelectedItem;
	bool16 fApplyCJKGrid;
	bool16 fShowPreview;
};

/**	Provides a mechanism for the user to choose a folder.

	The code below pops a folder chooser dialog.
	<pre>
	SDKFolderChooser folderChooser;
	folderChooser.ShowDialog();
	if (folderChooser.IsChosen()) {
		IDFile chosenFolder = folderChooser.GetIDFile();
	}
	</pre>

	
	@see ISelectFolderDialog
	@see kSelectFolderDialogBoss
	@ingroup sdk_common
 */
class SDKFolderChooser : public SDKFileHelper {
public:
	/** Constructor.
	 */
	SDKFolderChooser();

	/** Destructor.
	 */
	virtual ~SDKFolderChooser();

	/**	Pops a dialog to allow the user to choose a folder. 

		@post SDKFileHelper::IsChosen is kTrue if a folder was chosen, kFalse otherwise.
		@post SDKFileHelper::GetIDFile contains the chosen folder if SDKFileHelper::IsChosen is kTrue.
		@post SDKFileHelper::GetPath contains the path to the chosen folder if SDKFileHelper::IsChosen is kTrue.
	*/
	void ShowDialog();

	/**	Set the title of the dialog.
		@param title IN dialog title (translatable string keys passed here will be translated
			before display in the dialog).
	*/
	void SetTitle(const PMString& title);

protected:
	void initFromSysFile();
private:
#ifdef MACINTOSH
	void macInitFolderFromSysFile();
#endif

private:
	PMString fTitle;

};


#endif // __SDKFile_H_DEFINED__


//  Code generated by DollyXS code generator
