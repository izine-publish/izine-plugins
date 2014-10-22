//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/StUtils.h $
//	$Revision: 3577 $
//	$Date: 2011-08-12 09:35:21 +0200 (Fri, 12 Aug 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Stack based classes.
//========================================================================================
#ifndef _h_StUtils_
#define _h_StUtils_
#pragma once

#pragma mark -
//To be used with intrinsic/basic data types only
template <class valType>
class StRestoreValue
{
public:
					StRestoreValue( valType & inValRef, const valType inVal);
					StRestoreValue( valType & inValRef );
	virtual			~StRestoreValue();

	valType			Forget();
protected:
private:
	const valType	mOldVal;
	valType *		mPtrToVal;
};

// Used for auto deletion of a pointer
template <class pointerType>
class StPtrDeleter
{
public:
						StPtrDeleter( 
							const pointerType *			inPtr = NULL,
							bool						inAllocatedSigle = false); //false if new [] is used
						~StPtrDeleter();

	const pointerType *	Get() const;
	void				Set(
							const pointerType *			inPtr,
							bool						inAllocatedSigle = false); //false if new [] is used
	
	const pointerType *	Forget();

protected:
private:
	void				DeletePtr();
	
	
	const pointerType*	mPtr;
	bool				mAllocatedSingle;
};

#pragma mark -

template <class T, const int stackSize = 255>
class StSmartBuff
{
public:
						StSmartBuff(
							int							inSize = 1 );
						~StSmartBuff();

	T*					Get() const;

private:
	T*					mBuff;
	T					mBuffOnStack[stackSize];
	T*					mBuffAllocated;
};

#pragma mark -
class StFileCloser
{
public:
						StFileCloser(
							FILE *					inFile );
						~StFileCloser();

	FILE *				Forget();
	void				Reset(
							FILE *					inFile );
private:
	FILE *		mFile;
};
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <class valType>
StRestoreValue<valType>::StRestoreValue(
	valType & inValRef, const valType inVal )
: mOldVal( inValRef )
, mPtrToVal ( &inValRef )
{
	inValRef = inVal;
}

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <class valType>
StRestoreValue<valType>::StRestoreValue(
	valType & inValRef )
: mOldVal( inValRef )
, mPtrToVal ( &inValRef )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <class valType>
StRestoreValue<valType>::~StRestoreValue()
{
	if( mPtrToVal )
		*mPtrToVal = mOldVal;
}

//----------------------------------------------------------------------------------------
// Forget
//----------------------------------------------------------------------------------------
template <class valType> valType
StRestoreValue<valType>::Forget()
{
	valType toReturn;
	if( mPtrToVal )
		toReturn = *mPtrToVal;
	mPtrToVal = NULL;
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <class pointerType>
StPtrDeleter<pointerType>::StPtrDeleter(
	const pointerType *			inPtr,
	bool						inAllocatedSigle)
: mPtr( inPtr )
, mAllocatedSingle( inAllocatedSigle )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <class pointerType>
StPtrDeleter<pointerType>::~StPtrDeleter()
{
	DeletePtr();
}

template <class pointerType> void
StPtrDeleter<pointerType>::Set(
	const pointerType *			inPtr,
	bool						inAllocatedSigle)
{
	DeletePtr();
	mPtr = inPtr;
	mAllocatedSingle = inAllocatedSigle;
}

template <class pointerType> const pointerType *
StPtrDeleter<pointerType>::Get() const
{
	return mPtr;
}

template <class pointerType> const pointerType *
StPtrDeleter<pointerType>::Forget()
{
	const pointerType * toReturn = mPtr;
	mPtr = NULL;
	return toReturn;
}

template <class pointerType> void
StPtrDeleter<pointerType>::DeletePtr()
{
	if( mPtr )
	{
		if( mAllocatedSingle )
			delete mPtr;
		else
			delete [] mPtr;
	}
	mPtr = NULL;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <class T, const int stackSize>
StSmartBuff<T, stackSize>::StSmartBuff( int inBuffSize )
:
	mBuff( NULL ),
	mBuffAllocated( NULL )
{
	if( inBuffSize <= sizeof( mBuffOnStack )/sizeof(T) )
		mBuff = mBuffOnStack;
	else
		mBuff = mBuffAllocated = new T[ inBuffSize ];
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <class T, const int stackSize>
StSmartBuff<T, stackSize>::~StSmartBuff()
{
	if( mBuffAllocated )
		delete [] mBuffAllocated;
	mBuffAllocated = nil;
}

template <class T, const int stackSize> T *
StSmartBuff<T, stackSize>::Get() const
{
	return mBuff;
}

/**
	Usage will be like
	StSmartBuffers<int32>::Buff32		aBuff( 25 );
*/
template <class T>
struct StSmartBuffers
{
	typedef StSmartBuff<T, 10>	Buff10;
	typedef StSmartBuff<T, 16>	Buff16;
	typedef StSmartBuff<T, 32>	Buff32;
	typedef StSmartBuff<T, 64>	Buff64;
	typedef StSmartBuff<T, 128>	Buff128;
	typedef StSmartBuff<T, 256>	Buff256;
	typedef StSmartBuff<T, 512>	Buff512;
};
#endif //_h_StUtils_
