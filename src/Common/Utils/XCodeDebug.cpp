
#import "/Developer/Applications/Xcode.app/Contents/PlugIns/GDBMIDebugging.xcplugin/Contents/Headers/DataFormatterPlugin.h"


#include "FileUtils.h"
#include "IDFile.h"
#pragma mark -

// Plug-in function list (required symbol).
_pbxgdb_plugin_function_list *_pbxgdb_plugin_functions;

static char *nullPluginFunctions = "CFDataFormatter plugin error: _pbxgdb_plugin_functions not set!";

//const char * IdFileToCharPtr(
//				const IDFile &		inIdFile, int identifier)
//{
//	char * toReturn = nil;
//	if (_pbxgdb_plugin_functions)
//		toReturn = (char *)(_pbxgdb_plugin_functions->allocate(identifier, 255));
//	else
//		return nullPluginFunctions;
//	
//	*toReturn = 0;
//	PMString str;
//	FileUtils::IDFileToPMString(inIdFile, str);
//	strncpy(toReturn, str.GrabCString(), 255);
//	
//	return toReturn;
//}
//
//const char * PMStringToCharPtr(
//							 const PMString &		inPMString, int identifier)
//{
//	char * toReturn = nil;
//	if (_pbxgdb_plugin_functions)
//		toReturn = (char *)(_pbxgdb_plugin_functions->allocate(identifier, 255));
//	else
//		return nullPluginFunctions;
//	
//	*toReturn = 0;
//	strncpy(toReturn, inPMString.GrabCString(), 255);
//	
//	return toReturn;
//}
//
//const char * PMStringToCharPtr(
//							   const PMString *		inPMString, int identifier)
//{
//	if (!inPMString)
//		return nil;
//	
//	char * toReturn = nil;
//	if (_pbxgdb_plugin_functions)
//		toReturn = (char *)(_pbxgdb_plugin_functions->allocate(identifier, 255));
//	else
//		return nullPluginFunctions;
//	
//	*toReturn = 0;
//	strncpy(toReturn, inPMString->GrabCString(), 255);
//	
//	return toReturn;
//}
#if 0
const char * IdFileToCharPtr(
							 const IDFile &		inIdFile, int identifier)
{
	static char toReturn[256] = {0};
	PMString str;
	FileUtils::IDFileToPMString(inIdFile, str);
	strncpy(toReturn, str.GrabCString(), 255);
	
	return toReturn;
}

const char * IdFileToCharPtr(
							 IDFile &		inIdFile, int identifier)
{
	char toReturn[256] = {0};
	toReturn[0] = 0;
	PMString str;
	FileUtils::IDFileToPMString(inIdFile, str);
	strncpy(toReturn, str.GrabCString(), 255);
	
	return toReturn;
}

const char * PMStringToCharPtr(
							   const PMString &		inPMString, int identifier)
{
	static char toReturn[256] = {0};
	toReturn[0] = 0;
	strncpy(toReturn, inPMString.GrabCString(), 255);
	
	return toReturn;
}

const char * PMStringToCharPtr(
							   const PMString *		inPMString, int identifier)
{
	if (!inPMString)
		return nil;
	
	static char toReturn[256] = {0};
	toReturn[0] = 0;
	strncpy(toReturn, inPMString->GrabCString(), 255);
	
	return toReturn;
}

#else
char xcodeBuffPtrs[256][100] = {0};
const char *	PMStringToCharPtr(
					const PMString &		inPMString, int identifier);
const char *	PMStringToCharPtr(
					const PMString *		inPMString, int identifier);
		
const char *
IdFileToCharPtr(
	const IDFile &		inIdFile, int identifier)
{
	PMString str;
	FileUtils::IDFileToPMString(inIdFile, str);
	return PMStringToCharPtr( str, identifier);
}

const char * 
IdFileToCharPtr(
	IDFile &		inIdFile, int identifier)
{
	PMString str;
	FileUtils::IDFileToPMString(inIdFile, str);
	return PMStringToCharPtr( str, identifier);
}

const char *
PMStringToCharPtr(
	const PMString &		inPMString, int identifier)
{
	char * toReturn = xcodeBuffPtrs[identifier%100];
	
	toReturn[0] = 0;
	strncpy(toReturn, inPMString.GrabCString(), 255);
	
	return toReturn;
}

const char * PMStringToCharPtr(
	const PMString *		inPMString, int identifier)
{
	if (!inPMString)
		return nil;
	
	char * toReturn = xcodeBuffPtrs[identifier%100];
	toReturn[0] = 0;
	strncpy(toReturn, inPMString->GrabCString(), 255);
	
	return toReturn;
}
#endif




