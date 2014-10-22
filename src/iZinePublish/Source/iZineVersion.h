//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/iZineVersion.h $
//	$Author: mindfire $
//	$Date: 2013-08-16 14:05:19 +0200 (Fri, 16 Aug 2013) $
//	$Revision: 4098 $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2010 Feb 02
//	Copyright 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: File that controls the verions information of the plugin.
//========================================================================================

#ifndef _iZineVersion_h_
#define _iZineVersion_h_

#define kIZPMajorVersionForRez		3
#define kIZPMinorVersionForRez		0
#define kIZPBuildNumberForRez		30

#define kIZPMajorVersion		RezLong(kIZPMajorVersionForRez)
#define kIZPMinorVersion		RezLong(kIZPMinorVersionForRez)
#define kIZPBuildNumber			RezLong(kIZPBuildNumberForRez)

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

#define kIZPSVNNumberStr "$Revision: 4098 $"
#if !defined(__endPtrAtSpace__) && defined(__cplusplus)
#define __endPtrAtSpace__
inline char * endPtrAtSpace( char * inVersionStr )
{
	char * endPtr = inVersionStr;
	while( endPtr && *endPtr ) if( *endPtr == ' ') *endPtr = 0; else ++endPtr;
	return inVersionStr;
}
#endif
#if defined(__cplusplus)
//static const char * kZPSVNNumberStr = endPtrAtSpace( kIZPSVNNumberStr + 11 );
#endif

/*
// Plug-in version string used in About box and file properties. In the debug build, we append 'D' at the end of the version string.
*/
#ifdef DEBUG
#define kIZPPluginVersionString IZPMAKESTR_VERSION(kIZPMajorVersionForRez, kIZPMinorVersionForRez, 0, kIZPBuildNumberForRez) " D"
#else
#define kIZPPluginVersionString IZPMAKESTR_VERSION(kIZPMajorVersionForRez, kIZPMinorVersionForRez, 0, kIZPBuildNumberForRez)
#endif

#define kIZPPluginFileVersion	IZPMAKESTR(kIZPMajorVersionForRez) "." IZPMAKESTR(kIZPMinorVersionForRez)
#ifndef kIZPProductVersionString
#define kIZPProductVersionString IZPMAKESTR_VERSION(kIZPProductMajorVersionForRez, kIZPProductMinorVersionForRez, 0, kIZPProductBuildNumberForRez)
#endif
#define kIZPPlnFileDescStr		"Adobe InDesign/InCopy CS5.5 Plug-in"

#ifndef IZPMAKESTR3A
#define IZPMAKESTR3A( a, b, c )			a##b##c
#define IZPMAKESTR3( a, b, c )			IZPMAKESTR3A(a, b, c )
#define IZPMAKESTR5A( a, b, c, d, e )	a##b##c##d##e
#define IZPMAKESTR5( a, b, c, d, e)		IZPMAKESTR5A(a, b, c, d, e )
#endif
#define IZPMAC_CFBUNDLE_SHORT_VERSION_STRING	IZPMAKESTR3( kIZPMajorVersionForRez, . , kIZPMinorVersionForRez )
#define IZPMAC_CFBUNDLE_BUILD_VERSION			IZPMAKESTR5( kIZPMajorVersionForRez, . , kIZPMinorVersionForRez, . , kIZPBuildNumberForRez )

//Persist data format version.
//Add comment why a format version was changed.
	// Initial data format version numbers, till 2010Feb02
#define kIZPFirstMajorFormatNumber  RezLong(1)
#define kIZPFirstMinorFormatNumber  RezLong(0)

	//2010Feb03	Plugin Prefix changed, All IDs renumbered.
	//IDs split to UI and non-UI.
#define kIZPSecondMajorFormatNumber  RezLong(2)
#define kIZPSecondMinorFormatNumber  RezLong(0)

//2010Feb03	Plugin Prefix changed, All IDs renumbered.
	//IDs split to UI and non-UI.
#define kIZPThirdMajorFormatNumber  RezLong(3)
#define kIZPThirdMinorFormatNumber  RezLong(0)
	//2010Sep14 - ZPPrefs impl added.
#define kIZPThirdMinorFormatNumber_1	RezLong(1)		//Added Auto Unlock in prefs
#define kIZPThirdMinorFormatNumber_2	RezLong(2)

#define kIZPCurrentMajorFormatNumber kIZPThirdMajorFormatNumber
#define kIZPCurrentMinorFormatNumber kIZPThirdMinorFormatNumber

#endif
