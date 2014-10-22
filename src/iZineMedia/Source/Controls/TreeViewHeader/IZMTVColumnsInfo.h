//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/IZMTVColumnsInfo.h $
//	$Revision: 2770 $
//	$Date: 2011-04-11 12:00:23 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMTVColumnsInfo_
#define _h_IZMTVColumnsInfo_
#pragma once

enum enTVColumnType
{
	eTVColType_Empty,	//No widget created, used for indentation.
	eTVColType_Label,
	eTVColType_Chkbox,
	eTVColType_Icon,
	eTVColType_RolloverIconButton,
	eTVColTypeCount
};

struct ZMTVColumnInfo
{
	typedef  object_type   data_type;	//To support K2Vector

	int				mColID;	//Logical ID of column.
	int				mMinWidth;		//0 Means no limit
	int				mMaxWidth;		//0 Means no limit
	int				mMinHeight;		//0 Means no limit
	int				mMaxHeight;		//0 Means no limit
	bool			mCanHide;		//false if always visible, like ad id
	bool			mCanChangePosition;	//true if DnD is allowed.
	enTVColumnType	mColType;
	PMString		mColDispName;		//Display name for the column in header/context menu.
	PMString		mContextMenuDispName;	//If empty then mColDispName is used

	ZMTVColumnInfo()
		: mColID(0), mMinWidth( 0 ), mMaxWidth( 0 ), mMinHeight( 0 ), mMaxHeight( 0 )
		, mCanHide( true ), mCanChangePosition( true ), mColType( eTVColType_Label )
	{}
};

class IZMTVColumnsInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMTVCOLUMNSINFO };

	typedef K2Vector<ZMTVColumnInfo>	ZMTVColumnInfoArr;

	virtual void		InitCoumnsInfo(
							int							inTreeType ) = 0;	//1 Ads

	virtual	int			GetColumnCount() const = 0;
	virtual const ZMTVColumnInfo &	GetNthColumnInfo(
							const int					inColumnIndex ) const = 0;
	virtual const ZMTVColumnInfo &	GetColumnInfoForColID(
							const int					inColID ) const = 0;
	virtual bool		ContainsColumnInfoForColID(
							const int					inColID ) const = 0;

	virtual const WidgetID &	GetColumnWidgetStartID() const = 0;

	static	void		GetAdsTreeColumnsInfo(
							ZMTVColumnInfoArr &			oColumnsInfo );
};

extern const ZMTVColumnInfo		kZMEmptyTVColumnInfo;
#endif //_h_IZMTVColumnsInfo_
