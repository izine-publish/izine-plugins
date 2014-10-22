//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/String/ZPString.cpp $
//	$Revision: 3998 $
//	$Date: 2012-04-02 13:17:17 +0200 (Mon, 02 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

#include <string.h>

#include "ZPString.h"


//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <const int charSize>
ZPString_<charSize>::ZPString_(
	const char *		inValStr,
	int					inLen)
: mSize(0)
, mLongVal(0)
, mCapacity(charSize)
{
	mVal[charSize-1] = 0;
	this->SetStr( inValStr, inLen );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <const int charSize>
ZPString_<charSize>::~ZPString_()
{
	if( mLongVal )
		delete [] mLongVal;
}

//----------------------------------------------------------------------------------------
// Copy constructor
//----------------------------------------------------------------------------------------
template <const int charSize>
ZPString_<charSize>::ZPString_(
	const ZPString_ &	inStr )
{
	mCapacity = inStr.mCapacity;
	mSize = inStr.mSize;
	mVal[charSize-1] = 0;
	if( mCapacity > 0 && mCapacity < charSize )
	{
		memcpy(mVal, inStr.mVal, sizeof(mVal));
		mLongVal = NULL;
	}
	else if( inStr.mLongVal )
	{
		mLongVal = new char[inStr.mCapacity];
		memcpy(mLongVal, inStr.mLongVal, mSize);
	}
	else
		mLongVal = NULL;
}

//----------------------------------------------------------------------------------------
// operator =
//----------------------------------------------------------------------------------------
template <const int charSize>
ZPString_<charSize> &
ZPString_<charSize>::operator= (
	const ZPString_ &	inStr )
{
	if( mLongVal )
		delete [] mLongVal ;
	mLongVal = NULL;
	
	mCapacity = inStr.mCapacity;
	mSize = inStr.mSize;
	mVal[charSize-1] = 0;
	
	if( mCapacity > 0 && mCapacity < charSize )
	{
		memcpy(mVal, inStr.mVal, sizeof(mVal));
		mLongVal = NULL;
	}
	else if( inStr.mLongVal )
	{
		mLongVal = new char[inStr.mCapacity];
		memcpy(mLongVal, inStr.mLongVal, mSize);
	}
	else
		mLongVal = NULL;

	return *this;
}

//----------------------------------------------------------------------------------------
// SetStr
//----------------------------------------------------------------------------------------
template <const int charSize>
void
ZPString_<charSize>::SetStr(
	const char *		inStr,
	int					inLen )
{
	if( inLen < 0 )
	{
		if( inStr )
			inLen = strlen( inStr );
		else
			inLen = 0;
	}

	char * theBuffer = mVal;
	if( mLongVal && inLen < mCapacity )
		theBuffer = mLongVal;
	else if( inLen >= sizeof(mVal))
	{
		if( mLongVal )
			delete [] mLongVal;
		mCapacity = inLen + 1;
		theBuffer = mLongVal = new char [mCapacity];
	}

	if( inStr)
		memcpy( theBuffer, inStr, inLen + 1 );
	else
		*theBuffer = 0;
}

//----------------------------------------------------------------------------------------
// operator()
//----------------------------------------------------------------------------------------
template <const int charSize>
const char *
ZPString_<charSize>::operator()() const
{
	if(mLongVal)
		return mLongVal;
	else
		return mVal;
}

//#pragma mark -
//----------------------------------------------------------------------------------------
// SetStr
// Don't delete this function, deleting it will give linking errors,
// though it is not being called from anywhere.
// This function must contain all type of variations of ZPString_
// Done this way to restict the types/variations.
//----------------------------------------------------------------------------------------
//This is required if GCC_OPTIMIZATION_LEVEL is not 0
int dummyFunction();
static int gLinkZPString = dummyFunction();
int dummyFunction()
{
	ZPString64 str1;
	ZPString64 str2(str1);
	str1 = str2;
	str1.SetStr("");
	const char * a = str1();

	ZPString256 str3;
	ZPString256 str4(str3);
	str3 = str4;
	str3.SetStr("");
	const char * a3 = str3();
	
	ZPString512 str5;
	ZPString512 str6(str5);
	str5 = str6;
	str5.SetStr("");
	const char * a5 = str5();

	gLinkZPString += 1;
	return 1;
}

