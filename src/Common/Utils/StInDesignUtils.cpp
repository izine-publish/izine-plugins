//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StInDesignUtils.cpp $
//	$Revision: 3432 $
//	$Date: 2011-07-01 06:41:39 +0200 (Fri, 01 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "ISubject.h"
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "StInDesignUtils.h"

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StMuteChangeBroadcasting::StMuteChangeBroadcasting(
	ISubject*	inSubject )
:	mSubject( inSubject )
{
	if( inSubject )
		mWasActive = ! inSubject->IsMuted();
	if(mSubject && mWasActive)
		mSubject->Mute(kTrue);
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StMuteChangeBroadcasting::~StMuteChangeBroadcasting()
{
	if(mSubject && mWasActive )
		mSubject->Mute(kFalse);
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StMuteListening::StMuteListening(
	IObserver*	inObserver )
:	mObserver( inObserver )
{
	mWasActive = inObserver->IsEnabled() == kTrue;
	if(mObserver && mWasActive)
		mObserver->SetEnabled(kFalse);
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StMuteListening::~StMuteListening()
{
	if(mObserver && mWasActive )
		mObserver->SetEnabled(kTrue);
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
StStreamCloser::StStreamCloser(
	IPMStream *					inStream )
{
	if( inStream )
		inStream->AddRef();
	mStream.reset( inStream );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
StStreamCloser::~StStreamCloser()
{
	if( mStream )
		mStream->Close();
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
IPMStream *
StStreamCloser::Forget()
{
	return mStream.forget();
}

//----------------------------------------------------------------------------------------
// Reset
//----------------------------------------------------------------------------------------
void
StStreamCloser::Reset(
	IPMStream *					inStream)
{
	if( inStream )
		inStream->AddRef();
	mStream.reset( inStream );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// StRetainModifiedFlag
//----------------------------------------------------------------------------------------
StRetainModifiedFlag::StRetainModifiedFlag(
	IDataBase *					inDB)
: mDB(nil)
, mWasModified( kTrue )
{
	this->Reset( inDB );
}

//----------------------------------------------------------------------------------------
// StRetainModifiedFlag
//----------------------------------------------------------------------------------------
StRetainModifiedFlag::StRetainModifiedFlag(
	IPMUnknown *				inTarget)
: mDB( ::GetDataBase( inTarget ) )
, mWasModified( kTrue )
{
	this->Reset( mDB );
}

//----------------------------------------------------------------------------------------
// StRetainModifiedFlag
//----------------------------------------------------------------------------------------
StRetainModifiedFlag::~StRetainModifiedFlag()
{
	if( mDB )
	{
		if( mDB->IsModified() != mWasModified )
			mDB->SetModified( mWasModified );
	}
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
IDataBase *
StRetainModifiedFlag::Forget()
{
	IDataBase * toReturn = mDB;
	mDB = nil;
	return toReturn;
}

//----------------------------------------------------------------------------------------
// Reset
//----------------------------------------------------------------------------------------
void
StRetainModifiedFlag::Reset(
	IDataBase *					inDB)
{
	mDB = inDB;
	if( mDB )
		mWasModified = mDB->IsModified();
}

#pragma mark -
