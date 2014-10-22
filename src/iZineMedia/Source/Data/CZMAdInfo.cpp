//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMAdInfo.cpp $
//	$Revision: 2747 $
//	$Date: 2011-04-08 09:40:09 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdInfo.h"

//IZP General includes
//#include "ZMConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMAdInfo>
class CZMAdInfo : public inheritClass
{
public:
								CZMAdInfo(IPMUnknown * boss);
	virtual						~CZMAdInfo();

	void						SetAdDetail(
									const stAd &						inDetail );
											
	const PMReal &				GetAdHeight() const;
	const PMReal &				GetAdWidth() const;
	const PMString &			GetComments() const;
	const PMString &			GetUrl() const;
protected:
private:
	PMReal				mHeight;
	PMReal				mWidth;
	PMString			mUrl;
	PMString			mComment;
};


CREATE_PMINTERFACE(CZMAdInfo, kZMAdInfoImpl)

CZMAdInfo::CZMAdInfo(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdInfo::~CZMAdInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetAdDetail
//----------------------------------------------------------------------------------------
void
CZMAdInfo::SetAdDetail(
	const stAd &						inDetail)
{
	mHeight	= inDetail.mHeight;
	mWidth	= inDetail.mWidth;
	mUrl	= inDetail.mUrl;
	mComment =inDetail.mComment;
	
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZMAdInfoChangedMsg, IZMAdInfo::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetAdHeight
//----------------------------------------------------------------------------------------
const PMReal &
CZMAdInfo::GetAdHeight()const
{
	return mHeight;
}

//----------------------------------------------------------------------------------------
// GetAdWidth
//----------------------------------------------------------------------------------------
const PMReal &
CZMAdInfo::GetAdWidth()const
{
	return mWidth;
}

//----------------------------------------------------------------------------------------
// GetComments
//----------------------------------------------------------------------------------------
const PMString &
CZMAdInfo::GetComments()const
{
	return mComment;
}

//----------------------------------------------------------------------------------------
// GetUrl
//----------------------------------------------------------------------------------------
const PMString &
CZMAdInfo::GetUrl()const
{
	return mUrl;
}
