//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StInDesignUtils.h $
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
#ifndef _h_StInDesignUtils_
#define _h_StInDesignUtils_
#pragma once

class ISubject;
class IObserver;

class StMuteChangeBroadcasting
{
public:
						StMuteChangeBroadcasting(
							ISubject*					inSubject );
						~StMuteChangeBroadcasting();

private:
	ISubject*	mSubject;
	bool		mWasActive;
};

#pragma mark -
class StMuteListening
{
public:
						StMuteListening(
							IObserver *					inObserver );
						~StMuteListening();
private:
	IObserver *		mObserver;
	bool			mWasActive;
};

#pragma mark -
class StStreamCloser
{
public:
						StStreamCloser(
							IPMStream *					inStream );
						~StStreamCloser();

	IPMStream *			Forget();
	void				Reset(
							IPMStream *					inStream );
private:
	InterfacePtr<IPMStream>		mStream;
};

#pragma mark -
class StRetainModifiedFlag
{
public:
						StRetainModifiedFlag(
							IDataBase *					inDB );
						StRetainModifiedFlag(
							IPMUnknown *				inTarget );
						~StRetainModifiedFlag();

	IDataBase *			Forget();
	void				Reset(
							IDataBase *					inDB );
private:
	IDataBase *			mDB;
	bool16				mWasModified;
};
#endif //_h_StInDesignUtils_
