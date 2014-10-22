//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/CZMTVColumnsInfo.cpp $
//	$Revision: 2802 $
//	$Date: 2011-04-12 12:44:12 +0200 (Tue, 12 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMTVColumnsInfo.h"

//IZP General includes
#include "ZMUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

const ZMTVColumnInfo		kZPEmptyTVColumnInfo;

#define inheritClass CPMUnknown<IZMTVColumnsInfo>
class CZMTVColumnsInfo : public inheritClass
{
public:
						CZMTVColumnsInfo(IPMUnknown * boss);
	virtual				~CZMTVColumnsInfo();

	void				InitCoumnsInfo(
							int							inTreeType );

	int					GetColumnCount() const;
	const ZMTVColumnInfo &	GetNthColumnInfo(
							const int					inColumnIndex ) const;
	const ZMTVColumnInfo &	GetColumnInfoForColID(
							const int					inColID ) const;

	bool				ContainsColumnInfoForColID(
							const int					inColID ) const;

	const WidgetID &	GetColumnWidgetStartID() const;

protected:
	ZMTVColumnInfoArr	mColumnsInfo;
	WidgetID			mColStartWidgetID;
private:
};


CREATE_PMINTERFACE(CZMTVColumnsInfo, kZMTVColumnsInfoImpl)

CZMTVColumnsInfo::CZMTVColumnsInfo(IPMUnknown * boss)
: inheritClass(boss)
, mColStartWidgetID( kZMUITVColumnHeaderStartWidgetID )
{
	
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTVColumnsInfo::~CZMTVColumnsInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitCoumnsInfo
//----------------------------------------------------------------------------------------
void
CZMTVColumnsInfo::InitCoumnsInfo(
	int							inTreeType)
{
	if( inTreeType == 1 )
		GetAdsTreeColumnsInfo( mColumnsInfo );
}

//----------------------------------------------------------------------------------------
// GetColumnCount
//----------------------------------------------------------------------------------------
int
CZMTVColumnsInfo::GetColumnCount()const
{
	return mColumnsInfo.Length();
}

//----------------------------------------------------------------------------------------
// GetNthColumnInfo
//----------------------------------------------------------------------------------------
const ZMTVColumnInfo &
CZMTVColumnsInfo::GetNthColumnInfo(
	const int					inColumnIndex) const
{
	ASSERT( inColumnIndex >= 0 && inColumnIndex < mColumnsInfo.Length() );

	return mColumnsInfo[inColumnIndex];
}

//----------------------------------------------------------------------------------------
// GetColumnInfoForColID
//----------------------------------------------------------------------------------------
const ZMTVColumnInfo &
CZMTVColumnsInfo::GetColumnInfoForColID(
	const int					inColID) const
{
	int theColCount = mColumnsInfo.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZMTVColumnInfo & currInfo = mColumnsInfo[i];
		if( currInfo.mColID == inColID )
			return currInfo;
	}
	return kZPEmptyTVColumnInfo;

}

//----------------------------------------------------------------------------------------
// ContainsColumnInfoForColID
//----------------------------------------------------------------------------------------
bool
CZMTVColumnsInfo::ContainsColumnInfoForColID(
	const int					inColID) const
{
	int theColCount = mColumnsInfo.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZMTVColumnInfo & currInfo = mColumnsInfo[i];
		if( currInfo.mColID == inColID )
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
// GetColumnWidgetStartID
//----------------------------------------------------------------------------------------
const WidgetID &
CZMTVColumnsInfo::GetColumnWidgetStartID()const
{
	return mColStartWidgetID;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAdsTreeColumnsInfo
//----------------------------------------------------------------------------------------
void
IZMTVColumnsInfo::GetAdsTreeColumnsInfo(
	ZMTVColumnInfoArr &			oColumnsInfo)
{
	oColumnsInfo.clear();

	ZMTVColumnInfo	colInfo;

	colInfo.mColID				=	eTVColAds_Expander;
	colInfo.mMinWidth			=	11;
	colInfo.mMaxWidth			=	11;
	colInfo.mMinHeight			=	1;
	colInfo.mMaxHeight			=	1;
	colInfo.mCanHide			=	false;
	colInfo.mCanChangePosition	=	false;
	colInfo.mColType			=	eTVColType_Empty;
	colInfo.mColDispName		=	kNullString;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAds_NodeIcon;
	colInfo.mMinWidth			=	18;
	colInfo.mMaxWidth			=	18;
	colInfo.mMinHeight			=	18;
	colInfo.mMaxHeight			=	18;
	colInfo.mCanHide			=	false;
	colInfo.mCanChangePosition	=	false;
	colInfo.mColType			=	eTVColType_Icon;
	colInfo.mColDispName		=	kNullString;
	
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAds_Name;
	colInfo.mMinWidth			=	200;
	colInfo.mMaxWidth			=	300;
	colInfo.mColDispName		=	kIZMAdsColHeaderNameKey;
	colInfo.mContextMenuDispName=	kNullString;
	colInfo.mColType			=	eTVColType_Label;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAds_Height;
	colInfo.mMinWidth			=	50;
	colInfo.mMaxWidth			=	50;
	//colInfo.mCanHide			=	true;	//TODO: uncomment to show following columns in show/hide menu on right click
	//colInfo.mCanChangePosition	=	true;
	colInfo.mColDispName		=	kIZMAdsColHeaderHeightKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAds_Width;
	colInfo.mColDispName		=	kIZMAdsColHeaderWidthKey;
	//colInfo.mContextMenuDispName=	kIZMAdsColHeaderMenuVersionNumberIconKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAds_Comment;
	colInfo.mMinWidth			=	100;
	colInfo.mMaxWidth			=	400;
	colInfo.mColDispName		=	kIZMAdsColHeaderCommentsKey;
	colInfo.mContextMenuDispName=	kNullString;
	oColumnsInfo.push_back( colInfo );
}
