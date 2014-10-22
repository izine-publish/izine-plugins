//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StUtils.cpp $
//	$Revision: 3577 $
//	$Date: 2011-08-12 09:35:21 +0200 (Fri, 12 Aug 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "StUtils.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StFileCloser::StFileCloser(
	FILE *					inFile )
: mFile(inFile)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StFileCloser::~StFileCloser()
{
	if( mFile )
		fclose(mFile);
	mFile = nil;
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
FILE *
StFileCloser::Forget()
{
	FILE * toReturn = mFile;
	mFile = nil;
	return toReturn;
}


//----------------------------------------------------------------------------------------
// Reset
//----------------------------------------------------------------------------------------
void
StFileCloser::Reset(
	FILE *					inFile)
{
	mFile = inFile;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// FunctionNotBeingUsed 
//----------------------------------------------------------------------------------------
//Function used for compiling its header file.
static void FunctionNotBeingUsed()
{
	int a = 5;
	StRestoreValue<int> autoVal(a, 4);

	char * str = new char;
	StPtrDeleter<char>	autoDel( str, true );

	StSmartBuff<char>	buff(50);
}