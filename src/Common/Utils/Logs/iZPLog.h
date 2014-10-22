/*
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/Logs/iZPLog.h $

//	Owner: Rajkumar Sehrawat

//	$Date: 2011-07-07 14:37:12 +0200 (Thu, 07 Jul 2011) $

//	$Revision: 3469 $

//	$Author: aman.alam $

//	Description: Writes log to a file and provides easy logging functions.

//	$Copyright: 2010 iZine Solutions. All rights reserved. $
*/

#ifndef _IZPLOG_H_
#define _IZPLOG_H_

#ifdef WINDOWS
#include "StdArg.h"
#endif

#define EnableFunctionTimeLogging	1

enum eIZPMajorLogArea
{
	//Application
	enLMA_App				= 1 << 01,	//ZP application handling
	enLMA_Idle				= 1 << 02,	//
	enLMA_Scripting			= 1 << 03,	//Scripting
	enLMA_Commands			= 1 << 04,	//Commands
	enLMA_Responder			= 1 << 05,	//Responder
	enLMA_WS				= 1 << 06,	//Web Service
	enLMA_DBObjects			= 1 << 07,	//Database objects and its Managers
	//Asset Management
	enLMA_AMSysP			= 1 << 8,	//Asset Management System Provider
	enLMA_AMSrvP			= 1 << 9,	//Asset Management Service Provider
	enLMA_AMUISrvP			= 1 << 10,	//Asset Management UI service provider
	enLMA_AMService			= 1 << 11,	//Asset Management service
	enLMA_AMUIService		= 1 << 12,	//Asset Management UI service
	enLMA_AssetMonitor		= 1 << 13,	//
	enLMA_AM				= 1 << 14,	//General group for Asset Management
	//Utils
	enLMA_ICExport			= 1 << 15,	//Export Provider
	enLMA_Utils				= 1 << 15,	//Utils
	enLMA_Story				= 1 << 16,	//General group for Story
	//UI
	enLMA_ActionComponent	= 1 << 17,	//Action Component, Action Filter, Dynamic menu.
	enLMA_Dialogs			= 1 << 18,	//All Dialogs
	enLMA_UI				= 1 << 19,	//General group for UI
	enLMA_DnD				= 1 << 20,	//Drag n Drop
	enLMA_PanelAssets		= 1 << 21,	//Assets Panel
	enLMA_PanelTasks		= 1 << 22,	//Tasks Panel
	enLMA_AssetsTV			= 1 << 23,	//Assets TreeView
	enLMA_TasksTV			= 1 << 24,	//TasksTV
	enLMA_TVHeader			= 1 << 25,	//TreeView Header control
	
	enLMA_ALL				= -1,
	enLMA_None				= 0
};

enum eIZPLogType
{
	enLT_Function		= 1 << 0,		//Log function Enter/Exit
	enLT_Error			= 1 << 1,		
	enLT_Warning		= 1 << 2,		
	enLT_Details		= 1 << 3,		//For both Debug & Release builds. Mostly tracking flow.
	enLT_DebugInfo		= 1 << 4,		//Mostly deals with debug build, logging function params and returns.
	enLT_ObjectCreations	= 1 << 5,	//Tracking object creation/destruction

	//enLT_			= 1 << 5,
	//enLT_			= 1 << 6,
	//enLT_			= 1 << 7,
	//enLT_			= 1 << 8,
	//enLT_			= 1 << 9,
	//enLT_			= 1 << 10,
	//enLT_			= 1 << 11,
	//enLT_			= 1 << 12,
	//enLT_			= 1 << 13,
	//enLT_			= 1 << 14,
	//enLT_			= 1 << 15,
	//enLT_			= 1 << 16,
	//enLT_			= 1 << 17,
	//enLT_			= 1 << 18,
	//enLT_			= 1 << 19,
	//enLT_			= 1 << 20,
	//enLT_			= 1 << 21,
	//enLT_			= 1 << 22,
	//enLT_			= 1 << 23,
	//enLT_			= 1 << 24,
	//enLT_			= 1 << 25,
	//enLT_			= 1 << 26,
	//enLT_			= 1 << 27,
	//enLT_			= 1 << 28,
	//enLT_			= 1 << 29,
	//enLT_			= 1 << 30,
	//enLT_			= 1 << 31,

	enLT_All			= -1,
	enLT_None			= 0
};

namespace IZPLog
{
	//Main functions
	bool		CanWriteLog(
					eIZPMajorLogArea		inLMA,
					eIZPLogType				inLogType);
					
	void PLUGIN_DECL
				WriteLog(
					eIZPMajorLogArea		inLMA,
					eIZPLogType				inLogType,
					const char *			inFuncName,
					const int				inFromLine,
					const char *			inLogStr,
					...);

	void		WriteLogVaList(
					eIZPMajorLogArea		inLMA,
					eIZPLogType				inLogType,
					const char *			inFuncName,
					const int				inFromLine,
					const char *			inLogStr,
					va_list					inVaArgs );

	long		CurrentLogFilePosition();

	bool PLUGIN_DECL
				IsEnabledLogArea(
					eIZPMajorLogArea		inLMA );
	void PLUGIN_DECL
				EnableLogArea(
					eIZPMajorLogArea		inLMA );
	void PLUGIN_DECL
				DisableLogArea(
					eIZPMajorLogArea		inLMA );

	//Global Variables
	extern eIZPMajorLogArea	gLogMajorArea;
	extern eIZPLogType		gLogType;
	extern int				gLogCurrentIndent;
	extern bool				gLogInNextLine;
	
	inline void	IncrementCurrentIndent(
					eIZPMajorLogArea		inLMA,
					eIZPLogType				inLogType)
				{
					if(CanWriteLog(inLMA, inLogType) )
						++gLogCurrentIndent;
				}

	inline void	DecrementCurrentIndent(
					eIZPMajorLogArea		inLMA,
					eIZPLogType				inLogType)
				{
					if(CanWriteLog(inLMA, inLogType) && gLogCurrentIndent > 0)
						--gLogCurrentIndent;
				}
};


//Helper class to make function call and exit logging easy.
//Uses stack unwinding to log function exit.
class PLUGIN_DECL StFunctionNameLogger
{
public:
	StFunctionNameLogger(
		eIZPMajorLogArea		inLMA,
		eIZPLogType				inLogType,
		const char *			inFuncName,
		const int				inFromLine);

	~StFunctionNameLogger();

private:
		
	const eIZPMajorLogArea	mLMA;
	const eIZPLogType		mLogType;
	const char*				mFuncName;
	const int				mFromLine;
	long int				mCurrLogFilePosition;
#if EnableFunctionTimeLogging
	//time_t					mFunctionStartTime;
	clock_t					mFunctionStartTime;
#endif
};

#ifdef DEBUG
#define EnableLogging	//Always enable loggin in debug build, for release it is controlled from xcconfig
#endif

//Macros to ease logging
#ifndef	EnableLogging
#ifdef WINDOWS
	#define IZPLog_Str
	#define IZPLog_Str_
#else
	class PLUGIN_DECL IZPNoLogVargsHelper
	{
	public:
		void	operator()(
					eIZPMajorLogArea	inLMA,
					eIZPLogType			inLogType,
					const char *		inLogStr, ...){}
	};

	#define IZPLog_Str IZPNoLogVargsHelper() 
	#define IZPLog_Str_ IZPNoLogVargsHelper()
#endif
	#define IZPLog_func( inLMA )
	
	#define IZPLog_ObjC( inLMA )
	#define IZPLog_ObjD( inLMA )
#else

//On Mac we can have function name with param types
//Enable only one statement of the following two
//#define FUNCTIONNAME __PRETTY_FUNCTION__
#define FUNCTIONNAME __FUNCTION__

#ifdef WINDOWS	//i.e where __VA_ARGS__ is available
#define IZPLog_Str( inLMA, inLogType, inLogStr, ...)		\
	IZPLog::WriteLog( inLMA, inLogType, FUNCTIONNAME, __LINE__, inLogStr, __VA_ARGS__ )
#define IZPLog_Str_( inLMA, inLogType, inLogStr, ...)		\
	IZPLog::WriteLog( inLMA, inLogType, NULL, __LINE__, inLogStr, __VA_ARGS__ )
#else	//Function name will not be printed in VS 2003
	class PLUGIN_DECL IZPVargsHelper
	{
	public:
				IZPVargsHelper(
					const char*			inFuncName,
					int					inFromLine );
		void	operator()(
					eIZPMajorLogArea	inLMA,
					eIZPLogType			inLogType,
					const char *		inLogStr, ...);
		
	private:
		const char*	mFuncName;
		int			mFromLine;
	};

	#define IZPLog_Str (IZPVargsHelper( FUNCTIONNAME, __LINE__) )
	#define IZPLog_Str_ (IZPVargsHelper( NULL, __LINE__) )
#endif

#define IZPLog_func( inLMA )	StFunctionNameLogger autoLog( inLMA, enLT_Function, FUNCTIONNAME, __LINE__ )
#define IZPLog_ObjC( inLMA )		\
	IZPLog::WriteLog( inLMA, enLT_ObjectCreations, NULL, __LINE__, "Object created : %xd, class %s" , this, typeid(this).name() )
#define IZPLog_ObjD( inLMA )		\
	IZPLog::WriteLog( inLMA, enLT_ObjectCreations, NULL, __LINE__, "Object destroyed : %xd, class %s" , this, typeid(this).name() )

#endif	//EnableLogging

#endif	//_IZPLOG_H_
