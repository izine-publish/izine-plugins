//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/iZMediaVersion.h $
//	$Revision: 2893 $
//	$Date: 2011-04-29 10:34:17 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description: File that controls the verions information of the plugin.
//========================================================================================

#ifndef _h_iZineExpVersion_
#define _h_iZineExpVersion_
#pragma once

#define kIZMediaMajorVersionForRez		1
#define kIZMediaMinorVersionForRez		0
#define kIZMediaBuildNumberForRez		6

#define kIZMediaMajorVersion		RezLong(kIZMediaMajorVersionForRez)
#define kIZMediaMinorVersion		RezLong(kIZMediaMinorVersionForRez)
#define kIZMediaBuildNumber			RezLong(kIZMediaBuildNumberForRez)

//Product Number will be same for all iZine Publish plugins.
#ifndef kIZPProductMajorVersionForRez
#define kIZPProductMajorVersionForRez	1
#define kIZPProductMinorVersionForRez	0
#define kIZPProductBuildNumberForRez	001
#endif

//Macro to create number to string during compile time.
#ifndef IZP_STRINGIZE2
#define IZP_STRINGIZE2(s) #s
#define IZPMAKESTR(s) IZP_STRINGIZE2(s)

// Macro to create version number in the format "5.0.0.385"
#define IZPMAKESTR_VERSION(verMajor, verMinor, verStep, verBuild) \
			IZPMAKESTR(verMajor)		\
			"." IZPMAKESTR(verMinor)	\
			"." IZPMAKESTR(verStep)		\
			"." IZPMAKESTR(verBuild)
#endif //stringize
/*
// Plug-in version string used in About box and file properties. In the debug build, we append 'D' at the end of the version string.
*/
#ifdef DEBUG
#define kIZMediaPluginVersionString IZPMAKESTR_VERSION(kIZMediaMajorVersionForRez, kIZMediaMinorVersionForRez, 0, kIZMediaBuildNumberForRez) " D"
#else
#define kIZMediaPluginVersionString IZPMAKESTR_VERSION(kIZMediaMajorVersionForRez, kIZMediaMinorVersionForRez, 0, kIZMediaBuildNumberForRez)
#endif

#define kIZMediaPluginFileVersion	IZPMAKESTR(kIZMediaMajorVersionForRez) "." IZPMAKESTR(kIZMediaMinorVersionForRez)
#ifndef kIZPProductVersionString
#define kIZPProductVersionString IZPMAKESTR_VERSION(kIZPProductMajorVersionForRez, kIZPProductMinorVersionForRez, 0, kIZPProductBuildNumberForRez)
#endif
#define kIZMediaPlnFileDescStr		"Adobe InDesign/InCopy CS3 Plug-in"

#ifndef IZPMAKESTR3A
#define IZPMAKESTR3A( a, b, c )			a##b##c
#define IZPMAKESTR3( a, b, c )			IZPMAKESTR3A(a, b, c )
#define IZPMAKESTR5A( a, b, c, d, e )	a##b##c##d##e
#define IZPMAKESTR5( a, b, c, d, e)		IZPMAKESTR5A(a, b, c, d, e )
#endif
#define IZMediaMAC_CFBUNDLE_SHORT_VERSION_STRING	IZPMAKESTR3( kIZMediaMajorVersionForRez, . , kIZMediaMinorVersionForRez )
#define IZMediaMAC_CFBUNDLE_BUILD_VERSION			IZPMAKESTR5( kIZMediaMajorVersionForRez, . , kIZMediaMinorVersionForRez, . , kIZMediaBuildNumberForRez )

//Persist data format version.
//Add comment why a format version was changed.
	// Initial data format version numbers, till 2010Feb02
#define kIZMediaFirstMajorFormatNumber  RezLong(1)
#define kIZMediaFirstMinorFormatNumber  RezLong(0)

#define kIZMediaSecondMinorFormatNumber  RezLong(1)	//Prefs - Ad file name prefix added.

#define kIZMediaCurrentMajorFormatNumber kIZMediaFirstMajorFormatNumber
#define kIZMediaCurrentMinorFormatNumber kIZMediaSecondMinorFormatNumber

#endif //_h_iZineExpVersion_
