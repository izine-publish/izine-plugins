//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/iZineExpVersion.h $
//	$Revision: 4039 $
//	$Date: 2012-06-22 11:58:17 +0200 (Fri, 22 Jun 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: File that controls the verions information of the plugin.
//========================================================================================

#ifndef _h_iZineExpVersion_
#define _h_iZineExpVersion_
#pragma once

#define kIZExpMajorVersionForRez		1
#define kIZExpMinorVersionForRez		0
#define kIZExpBuildNumberForRez			21

#define kIZExpMajorVersion		RezLong(kIZExpMajorVersionForRez)
#define kIZExpMinorVersion		RezLong(kIZExpMinorVersionForRez)
#define kIZExpBuildNumber			RezLong(kIZExpBuildNumberForRez)

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
#define kIZExpPluginVersionString IZPMAKESTR_VERSION(kIZExpMajorVersionForRez, kIZExpMinorVersionForRez, 0, kIZExpBuildNumberForRez) " D"
#else
#define kIZExpPluginVersionString IZPMAKESTR_VERSION(kIZExpMajorVersionForRez, kIZExpMinorVersionForRez, 0, kIZExpBuildNumberForRez)
#endif

#define kIZExpPluginFileVersion	IZPMAKESTR(kIZExpMajorVersionForRez) "." IZPMAKESTR(kIZExpMinorVersionForRez)
#ifndef kIZPProductVersionString
#define kIZPProductVersionString IZPMAKESTR_VERSION(kIZPProductMajorVersionForRez, kIZPProductMinorVersionForRez, 0, kIZPProductBuildNumberForRez)
#endif
#define kIZExpPlnFileDescStr		"Adobe InDesign/InCopy CS3 Plug-in"

#ifndef IZPMAKESTR3A
#define IZPMAKESTR3A( a, b, c )			a##b##c
#define IZPMAKESTR3( a, b, c )			IZPMAKESTR3A(a, b, c )
#define IZPMAKESTR5A( a, b, c, d, e )	a##b##c##d##e
#define IZPMAKESTR5( a, b, c, d, e)		IZPMAKESTR5A(a, b, c, d, e )
#endif
#define IZExpMAC_CFBUNDLE_SHORT_VERSION_STRING	IZPMAKESTR3( kIZExpMajorVersionForRez, . , kIZExpMinorVersionForRez )
#define IZExpMAC_CFBUNDLE_BUILD_VERSION			IZPMAKESTR5( kIZExpMajorVersionForRez, . , kIZExpMinorVersionForRez, . , kIZExpBuildNumberForRez )

//Persist data format version.
//Add comment why a format version was changed.
	// Initial data format version numbers, till 2010Feb02
#define kIZExpFirstMajorFormatNumber  RezLong(1)
#define kIZExpFirstMinorFormatNumber  RezLong(0)

#define kIZExpCurrentMajorFormatNumber kIZExpFirstMajorFormatNumber
#define kIZExpCurrentMinorFormatNumber kIZExpFirstMinorFormatNumber

#endif //_h_iZineExpVersion_
