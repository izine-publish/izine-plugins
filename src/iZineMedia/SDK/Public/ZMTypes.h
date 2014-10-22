//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMTypes.h $
//	$Revision: 2782 $
//	$Date: 2011-04-11 16:14:31 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZMTypes_
#define _h_ZMTypes_
#pragma once

#include <map>
class IStringData;

#include "ZPSmartPtrVector.h"

typedef K2Vector<PMString>				PMStringList;
typedef PMStringList::iterator			PMStringListIter;
typedef PMStringList::const_iterator	PMStringListCIter;

typedef ZPSmartPtrVector<PMString>		ZMPMStringSList;		//PMString smart list
typedef ZMPMStringSList::iterator		ZMPMStringSListIter;
typedef ZMPMStringSList::const_iterator	ZMPMStringSListCIter;

typedef K2Vector<PMString>					ZMAdFileIDsList;
typedef ZMAdFileIDsList::iterator			ZMAdFileIDsListIter;
typedef ZMAdFileIDsList::const_iterator		ZMAdFileIDsListCIter;

typedef std::map<PMString, const IStringData *>	ZMAdFileIDMap;
typedef ZMAdFileIDMap::iterator				ZMAdFileIDMapIter;
typedef ZMAdFileIDMap::const_iterator		ZMAdFileIDMapCIter;

typedef K2Vector<PMString>					ZMAdIDsList;
typedef ZMAdIDsList::iterator				ZMAdIDsListIter;
typedef ZMAdIDsList::const_iterator			ZMAdIDsListCIter;

typedef std::map<PMString, const IStringData *>	ZMAdIDMap;
typedef ZMAdIDMap::iterator					ZMAdIDMapIter;
typedef ZMAdIDMap::const_iterator			ZMAdIDMapCIter;

class IStringData;

struct stEdition
{
public:
	typedef object_type data_type;

	PMString		mName;
	ZMPMStringSList mXmlFiles;
	
	stEdition () { }
	virtual ~stEdition () { }

private:
	explicit stEdition( const stEdition & inVal )
		: mName( inVal.mName )
		//,mXmlFiles( inVal.mXmlFiles )	//TODO: should we make copy? Which object will be owner of data?
	{
		mXmlFiles.Copy( inVal.mXmlFiles, true );
	}

	stEdition & operator = ( const stEdition & inVal )
	{
		mName = inVal.mName;
		//mXmlFiles = inVal.mXmlFiles;	//should we make copy? Which object will be owner of data?
		mXmlFiles.Copy( inVal.mXmlFiles, true );
		return *this;
	}
};

typedef ZPSmartPtrVector<stEdition>			ZMstEditionSList;		//stEdition smart list
typedef ZMstEditionSList::iterator			ZMstEditionSListIter;
typedef ZMstEditionSList::const_iterator	ZMstEditionSListCIter;

struct stTitle
{
public:
	typedef object_type data_type;

	PMString			mName;
	ZMstEditionSList	mEditions;

	stTitle( ) { }
	virtual ~stTitle( ) { }

private:
	explicit stTitle( const stTitle & inVal )
		: mName( inVal.mName )
		//, mEditions(inVal.mEditions)		//TODO: should we make copy? Which object will be owner of data?
	{
		mEditions.Copy(inVal.mEditions, true);
	}

	stTitle & operator = ( const stTitle & inVal )
	{
		mName = inVal.mName;
		//mEditions = inVal.mEditions;		//should we make copy? Which object will be owner of data?
		mEditions.Copy(inVal.mEditions, true);
		return *this;
	}
};


typedef ZPSmartPtrVector<stTitle>			ZMstTitleSList;		//stTitle smart list
typedef ZMstTitleSList::iterator			ZMstTitleSListIter;
typedef ZMstTitleSList::const_iterator		ZMstTitleSListCIter;


struct stAd
{
public:
	typedef object_type data_type;

	PMString			mId;
	PMString			mRefKey;
	PMString			mCustomer;
	PMReal				mHeight;
	PMReal				mWidth;
	PMString			mUrl;
	PMString			mPUrl;
	PMString			mComment;

	stAd () { }
	virtual ~stAd () { }

private:
	explicit stAd( const stAd & inVal )
		: mId( inVal.mId ),
		mRefKey( inVal.mRefKey ),
		mCustomer(inVal.mCustomer ),
		mUrl( inVal.mUrl ),
		mPUrl( inVal.mPUrl ),
		mComment( inVal.mComment )
	{
		mHeight = inVal.mHeight;
		mWidth = inVal.mWidth;
	}

	stAd & operator = ( const stAd & inVal )
	{
		mId = inVal.mId;
		mRefKey = inVal.mRefKey;
		mCustomer = inVal.mCustomer;
		mHeight = inVal.mHeight;
		mWidth = inVal.mWidth;
		mUrl = inVal.mUrl;
		mPUrl = inVal.mPUrl;
		mComment = inVal.mComment;

		return *this;
	}
};

typedef ZPSmartPtrVector<stAd>				ZMstAdSList;		//stAd smart list
typedef ZMstAdSList::iterator				ZMstAdSListIter;
typedef ZMstAdSList::const_iterator			ZMstAdSListCIter;

#endif //_h_ZMTypes_
