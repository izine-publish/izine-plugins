//========================================================================================
//  
//  $File: //depot/indesign_5.0/highprofile/source/sdksamples/common/SDKUtilities.h $
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

#ifndef __SDKUtilities_h__
#define __SDKUtilities_h__

#include "IPMStream.h"

#ifdef ID_DEPRECATED

/** SDKUtilities
    !!! This utility class is obsoleted. 
	Please use methods from CAlert.h FileUtis.h or SDKFileHelper.h 
	
*/
class SDKUtilities
{
	public:
		
		/** Constructor */
		SDKUtilities() { };

		/** Pop an About box using a string and CAlert.

		Obsoleted. Please use CAlert::ModelAlert()directly.
		@param alertStringID the string to be rendered into the alert box.
		*/
		static void InvokePlugInAboutBox(const PMString alertStringID);

		// The next set of methods allow for the manipulation of string filenames:
		
		/** generate the pathname as a PMString from a IDFile.

		Obsoleted. Please use FileUtils::SysFileToPMString() directly.
		@param spec the file whose path we want.
		@param name the string that will hold the pathname for the file. */
		static void GetPathName(const IDFile spec, PMString &name);

		/** Determine if the passed in string represents an absolute pathname.
		A win absolute pathname looks like "\\this\\that\\file"
		A mac absolute pathname looks like "this:that:file"
		a unix absolute pathname looks like "/this/that/file"

		Obsoleted. Please use SDKFileHelper::IsAbsolutePath() directly.
		@param thisString the path passed in.
		@return true if the string represents an absolute pathname */
		static bool IsAbsolutePath(PMString thisString);
		
		/** Determines if the passed in string represents a UNIX path.
		This is not a firm test of what constitutes a UNIX path, more just a heuristic that answers, "given
		a path, is it plausible it is a UNIX path?".

		Obsoleted. Please use SDKFileHelper::IsUnixPath() directly.
		@param thisstring the path used for the test. 
		@return true if the path could be a unix path.
		*/
		static bool IsUnixPath(PMString thisstring);
	
		/** Determines whether the path passed in can be a mac path. 
		This is not a firm test of what constitutes a mac path, more just a heuristic that answers, "given
		a path, is it plausible it is a mac path?".

		Obsoleted. Please use SDKFileHelper::IsMacPath() directly.
		@param thisstring the path used for the test.
		@return true if this string could be a mac path. 
        */
		static bool IsMacPath(PMString thisstring);

		/** Determines whether the path passed in could be a win path.
		This is not a firm test of what constitutes a win path, more just a heuristic that answers, "given
		a path, is it plausible it is a win path?".

		Obsoleted. Please use SDKFileHelper::IsWinPath() directly.
		@param thisstring the path used for the test.
		@return true if this string could be a win path. 
		*/
		static bool IsWinPath(PMString thisstring);

		/** takes the last element from the pathname described by fromthis.
		So "\this\that\file" becomes "\this\that"

		Obsoleted. Please use SDKFileHelper::RemoveLastElement() directly.
		@param fromthis the string to be truncated by a path element.
		*/
		static void RemoveLastElement(PMString& fromthis);

		/** takes the first element from the pathname described by thisstring.
		So "\this\that\file" becomes "\that\file" and "../../file" becomes "../file"

		Obsoleted. Please use SDKFileHelper::RemoveFirstElement() directly.
		@param fromthis the string to be shortened by a path element.
		*/
		static void RemoveFirstElement(PMString& fromthis);

		/** taking the pathname of a base and some relative path. 
		A new (absolute) path is constructed that represents the path relative to the base.

		Obsoleted. Please use SDKFileHelper::GetAbsoluteFromRelative() directly.
		@param base the (absolute) base path used to construct the new path. It identifies a folder/directory.
		@param path a relative path that is applied to the base.
		@param out the result of applying the relative path to the base. An absolute path is created.
		@return kSuccess if an absolute path can be created (or if the path is already in absolute form).
		*/
		static ErrorCode ToAbsolutePath(PMString base, PMString path, PMString& out);

		/** taking a IDFile as a base and a string which represents a relative path.
		A new IDFile is generated which represents this relative path in conjunction with the base.

		Obsoleted. Please use SDKFileHelper::GetAbsoluteFromRelative() directly.
		@param base the (absolute) base path used to construct the new path. It identifies a folder/directory.
		@param path a relative path that is applied to the base.
		@param out the result of applying the relative path to the base. An absolute path is created.
		@return kSuccess if an absolute path can be created (or if the path is already in absolute form).
		*/
		static ErrorCode ToAbsolutePath(IDFile base, PMString path, IDFile& out);

		/** Create a mac path from either a unix or win path.
		take the string thisstring and render it as a mac path
		So "this/that/file" becomes the relative path "this:that:file"
		"/this/that/file" becomes the absolute path "this:that:file"
		"../../file" becomes "::::file" 

		Obsoleted. You may consult SDKFileHelper::getAsMacPath().
		@param thisstring the path to convert.
		*/
		static void convertToMacPath(PMString& thisstring);

		/** Create a win path from either a unix or mac path.

		Obsoleted. You may consult SDKFileHelper::getAsWinPath().
		@param thisstring the path to convert.
		*/
		static void convertToWinPath(PMString& thisstring);

        /** Converts a IDFile to a PMString.

		Obsoleted. You may use FileUtils::SysFileToPMString() directly.
		*/
		static PMString SysFileToPMString(const IDFile* sFile);

        /** Convert a PMString to a IDFile.

		Obsoleted. You may use FileUtils::PMStringToSysFile() directly.
		*/
        static IDFile PMStringToSysFile(PMString* fileName);
		
		/* dumps to trace (notepad on win, DebugWindow on mac) information about the boss class.

		Obsoleted. You may consult the implementation code for reference.
		@param for_this_if defines an interface on the boss class of interest.
		*/
		static void DumpBoss(IPMUnknown * for_this_if);

		/** dumps information to trace (notepad on win, DebugWindow on mac) about the boss class.


		Obsoleted. You may consult the implementation code for reference.
		@param for_this_object is a UIDRef that identifies the boss class. 
		*/
		static void DumpBoss(UIDRef for_this_object);
		
		/** Call this method from an IObserver::Update() method to get diagnostic
		 trace about the notification. 

		Obsoleted. You may consult the implementation code for reference.
		 A call like this:
		 <pre>
		 SDKUtilities::DumpMessage("MyObserver::Update()", theChange, protocol);
		 </pre>
		 would create Trace output something like this...
		 <pre>
		 MyObserver::Update(), theChange=kSelectTextCmd, protocol=IID_ISELECTION
		 </pre>
		*/
		  static void DumpMessage(const char* msg, const ClassID& theChange, const PMIID& protocol);

		/** This routine is a Find and Replace for a single occurrence of an
		 item in a PMString.

		 Obsoleted. You may consult the implementation code for reference.
		 @param original original string to search in.
		 @param target text to search for.
		 @param replace text to replace with.
		 */
		static void Replace(
			PMString& original, 		 
			const PMString& target, 	 
			const PMString& replace		 
		);

		/** Get the absolute path to the InDesign application folder

		Obsoleted. You may use FileUtils::GetAppFolder() directly.
		*/
		static ErrorCode GetApplicationFolder(PMString& appFolder);

		/** Convert an absolute path to a IDFile

		Obsoleted. You may use FileUtils::PMStringToSysFile() directly.
		*/
		static ErrorCode AbsolutePathToSysFile(PMString path, IDFile& out);

		/** Create a read stream from an absolute path

		Obsoleted. You may use StreamUtil::CreateFileStreamRead()
		*/
		static IPMStream* CreateFileStreamRead(
            const PMString& fullPath, 
            uint32 mode = kOpenIn, 
            OSType fileType = 0x3f3f3f3f, 
            OSType creator = 0x3f3f3f3f
            );

		/** Create a write stream from an absolute path

		Obsoleted. You may use StreamUtil::CreateFileStreamWrite()
		*/
		static IPMStream* CreateFileStreamWrite(
            const PMString& fullPath, 
            uint32 mode = kOpenOut|kOpenTrunc, 
            OSType fileType = 'TEXT', 
            OSType creator = 'CWIE' 
            );

		/** Check file exists for read stream access

		Obsoleted. You may use StreamUtil::CreateFileStreamRead() 
		and check the stream. See example at CHLUtils.cpp
		*/
		static ErrorCode FileExistsForRead(const PMString& fullPath);

		/** Appends path separator to supplied path (: on Mac, \ on Win) 

		Obsoleted. Refer to implementation 
		*/
		static void AppendPathSeparator(PMString& path);

		/** Get the file creator type. 
		@param spec the file whose type and creator we want.
		@param fileType the return file type for the spec.
		@param creator the return creator for the spec.

		Obsoleted. FileUtils::GetFileCreatorType() directly. 
		*/
		static void GetFileCreatorType(const IDFile spec, SysOSType* fileType, SysOSType* creator);

		/** Get the file extension for the spec.
		@param spec the file whose extension we want.

		Obsoleted. FileUtils::GetExtension() directly. 
		*/
		static PMString GetExtension(const IDFile spec);			
};
#endif ID_DEPRECATED

#endif // __SDKUtilities_h__

// End, SDKUtilities.h.


