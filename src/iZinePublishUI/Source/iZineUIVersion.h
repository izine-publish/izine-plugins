//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/iZineUIVersion.h $
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

#ifndef _iZineUIVersion_h_
#define _iZineUIVersion_h_

#define kIZPUIMajorVersionForRez		3
#define kIZPUIMinorVersionForRez		0
#define kIZPUIBuildNumberForRez			30

#define kIZPUIMajorVersion		RezLong(kIZPUIMajorVersionForRez)
#define kIZPUIMinorVersion		RezLong(kIZPUIMinorVersionForRez)
#define kIZPUIBuildNumber		RezLong(kIZPUIBuildNumberForRez)

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

#define kIZPUISVNNumberStr "$Revision: 4098 $"
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
//static const char * kZPUISVNNumberStr = endPtrAtSpace( kIZPUISVNNumberStr + 11 );
#endif

/*
// Plug-in version string used in About box and file properties. In the debug build, we append 'D' at the end of the version string.
*/
#ifdef DEBUG
#define kIZPUIPluginVersionString IZPMAKESTR_VERSION(kIZPUIMajorVersionForRez, kIZPUIMinorVersionForRez, 0, kIZPUIBuildNumberForRez) " D"
#else
#define kIZPUIPluginVersionString IZPMAKESTR_VERSION(kIZPUIMajorVersionForRez, kIZPUIMinorVersionForRez, 0, kIZPUIBuildNumberForRez)
#endif

#define kIZPUIPluginFileVersion	IZPMAKESTR(kIZPUIMajorVersionForRez) "." IZPMAKESTR(kIZPUIMinorVersionForRez)
#ifndef kIZPProductVersionString
#define kIZPProductVersionString IZPMAKESTR_VERSION(kIZPProductMajorVersionForRez, kIZPProductMinorVersionForRez, 0, kIZPProductBuildNumberForRez)
#endif
#define kIZPUIPlnFileDescStr		"Adobe InDesign/InCopy CS5.5 Plug-in"

#ifndef IZPMAKESTR3A
#define IZPMAKESTR3A( a, b, c )			a##b##c
#define IZPMAKESTR3( a, b, c )			IZPMAKESTR3A(a, b, c )
#define IZPMAKESTR5A( a, b, c, d, e )	a##b##c##d##e
#define IZPMAKESTR5( a, b, c, d, e)		IZPMAKESTR5A(a, b, c, d, e )
#endif
#define IZPUIMAC_CFBUNDLE_SHORT_VERSION_STRING	IZPMAKESTR3( kIZPUIMajorVersionForRez, . , kIZPUIMinorVersionForRez )
#define IZPUIMAC_CFBUNDLE_BUILD_VERSION			IZPMAKESTR5( kIZPUIMajorVersionForRez, . , kIZPUIMinorVersionForRez, . , kIZPUIBuildNumberForRez )

#endif
