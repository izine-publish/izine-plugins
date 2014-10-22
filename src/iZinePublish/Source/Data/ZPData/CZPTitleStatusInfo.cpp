//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPTitleStatusInfo.cpp $
//	$Revision: 3213 $
//	$Date: 2011-06-13 12:17:48 +0200 (Mon, 13 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPTitleStatusInfo.h"

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTitleStatusInfo>
class CZPTitleStatusInfo : public inheritClass
{
public:
						CZPTitleStatusInfo(IPMUnknown * boss);
	virtual				~CZPTitleStatusInfo();

	enTitleStatusState	GetState() const;
	void				SetState(
							const enTitleStatusState	inState );
	void				SetState(
							const PMString &			inStateID );

	const bool			GetIsForLayout() const;
	void				SetIsForLayout(
							const bool					inIsForLayout );

	const bool			GetIsForText() const;
	void				SetIsForText(
							const bool					inIsForText );

	const int			GetSortIndex() const;
	void				SetSortIndex(
							const int					inSortIndex );

protected:
private:
	enTitleStatusState	mState;
	bool				mIsForLayout;
	bool				mIsForText;
	int					mSortIndex;
};


CREATE_PMINTERFACE(CZPTitleStatusInfo, kZPTitleStatusInfoImpl)

CZPTitleStatusInfo::CZPTitleStatusInfo(IPMUnknown * boss)
: inheritClass(boss)
, mState( eTitleStatusState_UnKnown )
, mIsForLayout( true )
, mIsForText( true )
, mSortIndex( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleStatusInfo::~CZPTitleStatusInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetStateFromStateID
//----------------------------------------------------------------------------------------
enTitleStatusState
IZPTitleStatusInfo::GetStateFromStateID(
	const PMString &			inStateID)
{
	if( inStateID.Compare( kFalse, kStatusState_Design ) == 0 )
		return eTitleStatusState_Design;
	else if( inStateID.Compare( kFalse, kStatusState_Editing ) == 0 )
		return eTitleStatusState_Editing;
	else if( inStateID.Compare( kFalse, kStatusState_Editorial ) == 0 )
		return eTitleStatusState_Editorial;
	else if( inStateID.Compare( kFalse, kStatusState_Final ) == 0 )
		return eTitleStatusState_Final;

	return eTitleStatusState_UnKnown;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetState
//----------------------------------------------------------------------------------------
enTitleStatusState
CZPTitleStatusInfo::GetState()const
{
	return mState;
}

//----------------------------------------------------------------------------------------
// SetState
//----------------------------------------------------------------------------------------
void
CZPTitleStatusInfo::SetState(
	const enTitleStatusState	inState)
{
	mState = inState;
}

//----------------------------------------------------------------------------------------
// SetState
//----------------------------------------------------------------------------------------
void
CZPTitleStatusInfo::SetState(
	const PMString &			inStateID)
{
	mState = GetStateFromStateID( inStateID );
}

//----------------------------------------------------------------------------------------
// GetIsForLayout
//----------------------------------------------------------------------------------------
const bool
CZPTitleStatusInfo::GetIsForLayout()const
{
	return mIsForLayout;
}

//----------------------------------------------------------------------------------------
// SetIsForLayout
//----------------------------------------------------------------------------------------
void
CZPTitleStatusInfo::SetIsForLayout(
	const bool					inIsForLayout)
{
	mIsForLayout = inIsForLayout;
}

//----------------------------------------------------------------------------------------
// GetIsForText
//----------------------------------------------------------------------------------------
const bool
CZPTitleStatusInfo::GetIsForText()const
{
	return mIsForText;
}

//----------------------------------------------------------------------------------------
// SetIsForText
//----------------------------------------------------------------------------------------
void
CZPTitleStatusInfo::SetIsForText(
	const bool					inIsForText)
{
	mIsForText = inIsForText;
}

//----------------------------------------------------------------------------------------
// GetSortIndex
//----------------------------------------------------------------------------------------
const int
CZPTitleStatusInfo::GetSortIndex()const
{
	return mSortIndex;
}

//----------------------------------------------------------------------------------------
// SetSortIndex
//----------------------------------------------------------------------------------------
void
CZPTitleStatusInfo::SetSortIndex(
	const int					inSortIndex)
{
	mSortIndex = inSortIndex;
}
