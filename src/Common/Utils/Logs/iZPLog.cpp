/*
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/Logs/iZPLog.cpp $

//	Owner: Rajkumar Sehrawat

//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $

//	$Revision: 4088 $

//	$Author: mindfire $

//	Description: Writes log to a file and provides easy logging functions.

//	Copyright: 2010 iZine Solutions. All rights reserved.
*/


#include "VCPlugInHeaders.h"

//InDesign SDK includes
#include "StringUtils.h"
#include "LocaleSetting.h"

//OS includes
#ifdef WINDOWS
#include "Windows.h"
#include <shlobj.h>
#else
#include "FileUtils.h"
#endif

//Project includes
#include "iZPLog.h"

#include "StUtils.h"

int IZPLog::gLogCurrentIndent = 0;
static FILE* gFPCLog = NULL;
bool IZPLog::gLogInNextLine = true;

//Set gobal value for major area to enable logging for.
eIZPMajorLogArea IZPLog::gLogMajorArea = (eIZPMajorLogArea)
	(
		enLMA_None	//No Logging for any Area, will be effective only if all the following lines are commented
			//Application
		//| enLMA_App				//= 1 << 01,	//ZP application handling
		//| enLMA_Idle				//= 1 << 02,	//
		//| enLMA_Scripting			//= 1 << 03,	//Scripting
		//| enLMA_Responder			//= 1 << 04,	//Responder
		//| enLMA_Commands			//= 1 << 05,	//Commands
		//| enLMA_WS				//= 1 << 06,	//Web Service
		//| enLMA_DBObjects			//= 1 << 07,	//Database objects and its Managers
			//Asset Management
		//| enLMA_AMSysP			//= 1 << 08,	//Asset Management System Provider
		//| enLMA_AMSrvP			//= 1 << 09,	//Asset Management Service Provider
		//| enLMA_AMUISrvP			//= 1 << 10,	//Asset Management UI service provider
		//| enLMA_AMService			//= 1 << 11,	//Asset Management service
		//| enLMA_AMUIService		//= 1 << 12,	//Asset Management UI service
		//| enLMA_AssetMonitor		//= 1 << 13,	//Asset Monitor
		//| enLMA_AM				//= 1 << 14,	//General group for Asset Management
		//	Utils
		//| enLMA_ICExport			//= 1 << 15,	//Export Provider
		//| enLMA_Utils				//= 1 << 15,	//Utils
		//| enLMA_Story				//= 1 << 16,	//General group for Story
			//UI
		//| enLMA_ActionComponent	//= 1 << 17,	//Action Component, Action Filter, Dynamic menu.
		//| enLMA_Dialogs			//= 1 << 18,	//All Dialogs
		//| enLMA_UI				//= 1 << 19,	//General group for UI
		//| enLMA_DnD				//= 1 << 20,	//Drag n Drop
		//| enLMA_PanelAssets		//= 1 << 21,	//Assets Panel
		//| enLMA_PanelTasks		//= 1 << 22,	//Tasks Panel
		//| enLMA_AssetsTV			//= 1 << 23,	//Assets TreeView
		//| enLMA_TasksTV			//= 1 << 24,	//TasksTV
		//| enLMA_TVHeader			//= 1 << 25,	//TreeView Header control

		//| enLMA_ALL	//Enable logging for all areas, irrespective of value commented above.
	);

eIZPLogType IZPLog::gLogType = (eIZPLogType)
	(
		enLT_None	//No Logging for any type, will be effective only if all the following lines are commented
		| enLT_Function				//Log function Enter/Exit
		| enLT_Error
		| enLT_Warning
		| enLT_Details
		| enLT_DebugInfo			//Mostly deals with debug build, logging function params and returns.
		//| enLT_ObjectCreations	//Tracking object creation/destruction

		//| enLT_All	//Enable logging for all type, irrespective of value commented above.
		
	);
#pragma mark -

#define STR_FunctionStart	"Enter"
#define STR_FunctionExit	"Exit"

StFunctionNameLogger::StFunctionNameLogger(
	eIZPMajorLogArea		inLMA,
	eIZPLogType				inLogType,
	const char *			inFuncName,
	const int				inFromLine)
: mLMA (inLMA )
, mLogType (inLogType )
, mFuncName( inFuncName )
, mFromLine (inFromLine )
{
	IZPLog::WriteLog( inLMA, inLogType, inFuncName, inFromLine, STR_FunctionStart );
	IZPLog::IncrementCurrentIndent( inLMA, inLogType );
#if EnableFunctionTimeLogging
	mFunctionStartTime = clock();
	//time(&mFunctionStartTime);
#endif
	mCurrLogFilePosition = IZPLog::CurrentLogFilePosition();
}

StFunctionNameLogger::~StFunctionNameLogger()
{
#if EnableFunctionTimeLogging
	clock_t	endTime = clock();
	//time_t endTime;
	//time(&endTime);
#endif
	IZPLog::DecrementCurrentIndent( mLMA, mLogType );
#if EnableFunctionTimeLogging
	//double diffTime = difftime(endTime, mFunctionStartTime);
	double diffTime = endTime - mFunctionStartTime;
	
	char logTextFormatted[100];
	char * logText = STR_FunctionExit;
	if( diffTime > 50 )
	{
		diffTime = 1.0*diffTime/CLOCKS_PER_SEC;
		sprintf( logTextFormatted, " %s: %.2lf", logText, diffTime);
		logText = logTextFormatted;
	}
#else
	const char * logText = STR_FunctionExit;
#endif
	long int currLogPos = IZPLog::CurrentLogFilePosition();
	if( currLogPos != mCurrLogFilePosition )
	{
		IZPLog::WriteLog( mLMA, mLogType , mFuncName, mFromLine, logText );
	}
	else
	{
		IZPLog::gLogInNextLine = false;
		IZPLog::WriteLog( mLMA, mLogType , NULL, mFromLine, "/%s", logText );
		IZPLog::gLogInNextLine = true;
	}
}

#pragma mark -
#if defined(MACINTOSH) && defined(EnableLogging)
IZPVargsHelper::IZPVargsHelper(
	const char*		inFuncName,
	int				inFromLine )
: mFuncName( inFuncName )
, mFromLine( inFromLine )
{
}

void
IZPVargsHelper::operator()(
	eIZPMajorLogArea	inLMA,
	eIZPLogType			inLogType,
	const char *		inLogStr, ...)
{
	va_list args ;
	va_start( args, inLogStr );
	IZPLog::WriteLogVaList( inLMA, inLogType, mFuncName, mFromLine, inLogStr, args );
	va_end( args );
}
#endif
#pragma mark -

extern "C"
{
//Also called form stdsoap2.h
const char * GetLogFileNamePrefix( void )
{
#if defined(InDnCS5) || defined(InDnCS5_5)
	if( kInDesignProductFS == LocaleSetting::GetLocale().GetProductFS() )
		return "iZineIDPluginCS5";
	else
		return "iZineICPluginCS5";
#else
	if( kInDesignProductFS == LocaleSetting::GetLocale().GetProductFS() )
		return "iZineIDPlugin";
	else
		return "iZineICPlugin";
#endif
}
}//extern "C"

#pragma mark -
namespace IZPLog
{
const char *
GetLogFilePath(
	char * 		outPath,
	int 		inMaxSize)
{
	const char * toReturn = NULL;
	outPath[0] = 0;
#ifdef WINDOWS
	TCHAR folderPath[255] = {0};
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, folderPath);

	lstrcat( folderPath, TEXT("\\iZine"));
	CreateDirectory(folderPath, NULL);
	const char * pathSep = "\\";
#else
	char folderPath[255];
	strcpy( folderPath, "~/Library/Preferences/iZine" );
#if defined(InDnCS5) || defined(InDnCS5_5)
	PMString folderPathToResolve( folderPath, PMString::kDontTranslateDuringCall );
#else
	PMString folderPathToResolve( folderPath, -1, PMString::kNoTranslate);
#endif

	PMString * resolvedPath = FileUtils::CreateResolvedPathName( folderPathToResolve );
	StPtrDeleter<PMString> autoDelPathStr( resolvedPath );
	if( resolvedPath )
		strcpy( folderPath, resolvedPath->GrabCString() );
	else	
		strcpy( folderPath, "/iZine");
	
	//Create the folder if required.
#if defined(InDnCS5) || defined(InDnCS5_5)
	PMString folderPathResolved( folderPath, PMString::kDontTranslateDuringCall );
#else
	PMString folderPathResolved( folderPath, -1, PMString::kNoTranslate);
#endif

	IDFile folderPathIDFile;
	FileUtils::PMStringToIDFile( folderPathResolved, folderPathIDFile );
	FileUtils::CreateFolderIfNeeded( folderPathIDFile, kTrue );
	const char * pathSep = "/";
#endif
	const char * logFileName = "Log.txt";
	const char * logFileNamePrefix = GetLogFileNamePrefix();

#ifndef MACINTOSH	
	if( sizeof(TCHAR) == sizeof(char))
#endif
	{
		//TODO: check inMaxSize
		strcpy( outPath, (const char *)folderPath );
		strcat( outPath, pathSep );
		strcat( outPath, logFileNamePrefix );
		strcat( outPath, logFileName );
	}
#ifndef MACINTOSH	
	else
	{
		int len = lstrlen( folderPath );
		WideString pathWStr( folderPath );
		pathWStr.Append( WideString(pathSep) );
		pathWStr.Append( WideString( logFileNamePrefix ) );
		pathWStr.Append( WideString(logFileName) );

		std::string utf8Path;
		StringUtils::ConvertWideStringToUTF8  ( pathWStr, utf8Path );
		
		//TODO: check inMaxSize
		strcpy( outPath, utf8Path.c_str() );
	}
#endif
	toReturn = outPath;
	return toReturn;
}

bool
openLogFile(void)
{
	const int maxFilePathSize = 255;
	char filePath[ maxFilePathSize ];
	if( !GetLogFilePath( filePath, maxFilePathSize ) )
		return false;
	
	gFPCLog = fopen( filePath, "w");
	if( !gFPCLog )
		return false;

	//Write UTF-8 BOM
	unsigned char bom[3] = {0xEF, 0xBB, 0xBF};
	fwrite( bom, 3, 1, gFPCLog );
	return true;
}

long
CurrentLogFilePosition()
{
	if( gFPCLog )
		return ftell( gFPCLog );
	return 0;
}

bool
CanWriteLog(
	eIZPMajorLogArea		inLMA,
	eIZPLogType				inLogType)
{
	bool toReturn = (inLMA & gLogMajorArea)
					&& (inLogType & gLogType );

	return toReturn;
}

bool PLUGIN_DECL
IsEnabledLogArea(
	eIZPMajorLogArea		inLMA )
{
	return (inLMA & gLogMajorArea );
}

void PLUGIN_DECL
EnableLogArea(
	eIZPMajorLogArea		inLMA )
{
	gLogMajorArea = (eIZPMajorLogArea)(gLogMajorArea | inLMA);
}

void PLUGIN_DECL
DisableLogArea(
	eIZPMajorLogArea		inLMA )
{
	gLogMajorArea = (eIZPMajorLogArea)(gLogMajorArea & ~inLMA);
}

void PLUGIN_DECL
WriteLog(
	eIZPMajorLogArea		inLMA,
	eIZPLogType				inLogType,
	const char *			inFuncName,
	const int				inFromLine,
	const char *			inLogStr,
	...)
{
	va_list args;
	va_start( args, inLogStr);
	WriteLogVaList( inLMA, inLogType, inFuncName, inFromLine, inLogStr, args );
	va_end( args );
}

void
WriteLogVaList(
	eIZPMajorLogArea		inLMA,
	eIZPLogType				inLogType,
	const char *			inFuncName,
	const int				inFromLine,
	const char *			inLogStr,
	va_list					inVaArgs )
{
	if( !CanWriteLog(inLMA, inLogType) )
		return;
	
	do
	{
		if( gFPCLog && ferror(gFPCLog) )
		{
			fclose( gFPCLog );
			gFPCLog = NULL;
		}
		
		if( !gFPCLog && false == openLogFile() )
			return;
		
		//Prefix indent spaces
		static char spaces[200] = {0};
		if( spaces[0] == 0 )
			memset( spaces, ' ', sizeof(spaces) );

#define TRACEFLOWAREA "iZine"
		if( gLogInNextLine )
		{
			int indentCount = gLogCurrentIndent;
			if( indentCount > 100 )
				indentCount = 100;

			fwrite( "\r\n", 1, 1, gFPCLog );
			fwrite( spaces, indentCount*2, 1, gFPCLog );
#ifdef TRACEFLOWAREA
			TRACEFLOW( TRACEFLOWAREA, "\n" );
			for( int i = 0 ; i < indentCount ; ++i )
				TRACEFLOW( TRACEFLOWAREA, "  " );
#endif
		}
		
		//Write info about calling function and line
		if( inFuncName )
		{
			fwrite( inFuncName, strlen(inFuncName), 1, gFPCLog );
			fprintf( gFPCLog, " #%4d : ", inFromLine );
#ifdef TRACEFLOWAREA
			TRACEFLOW( TRACEFLOWAREA, "iZine : %s #%4d : ", inFuncName, inFromLine );
#endif
		}
		
		vfprintf( gFPCLog, inLogStr, inVaArgs );
#ifdef TRACEFLOWAREA
		char logStr[1000];
		logStr[0] = 'i';logStr[1] = 'Z';logStr[2] = 'i';logStr[3] = 'n';logStr[4] = 'e';logStr[5] = ' ';logStr[6] = ':';logStr[7] = ' ';logStr[8] = 0;
		if( !gLogInNextLine || inFuncName )
			vsnprintf( logStr, sizeof(logStr), inLogStr, inVaArgs );
		else
			vsnprintf( logStr+8, sizeof(logStr)-9, inLogStr, inVaArgs );
		TRACEFLOW( TRACEFLOWAREA, logStr );
#endif
		//Flush file so that all the log contents are visible even after crash.
		fflush( gFPCLog ); 

		//Don't close the file pointer.
		//fclose( gFPCLog ); 
	}while( false);
}

}//Namespace IZPLog
