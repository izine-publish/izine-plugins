//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/IZPTVColumnsInfo.h $
//	$Revision: 2644 $
//	$Date: 2011-03-31 09:47:39 +0200 (Thu, 31 Mar 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTVColumnsInfo_
#define _h_IZPTVColumnsInfo_
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

struct ZPTVColumnInfo
{
	typedef  object_type   data_type;	//To support K2Vector

	int				mColID;	//Logical ID of column.
	int				mMinWidth;		//0 Means no limit
	int				mMaxWidth;		//0 Means no limit
	int				mMinHeight;		//0 Means no limit
	int				mMaxHeight;		//0 Means no limit
	bool			mCanHide;		//false if always visible, like Task Subject
	bool			mCanChangePosition;	//true if DnD is allowed.
	enTVColumnType	mColType;
	PMString		mColDispName;		//Display name for the column in header/context menu.
	PMString		mContextMenuDispName;	//If empty then mColDispName is used

	ZPTVColumnInfo()
		: mColID(0), mMinWidth( 0 ), mMaxWidth( 0 ), mMinHeight( 0 ), mMaxHeight( 0 )
		, mCanHide( true ), mCanChangePosition( true ), mColType( eTVColType_Label )
	{}
};

class IZPTVColumnsInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTVCOLUMNSINFO };

	typedef K2Vector<ZPTVColumnInfo>	ZPTVColumnInfoArr;

	virtual void		InitCoumnsInfo(
							int							inTreeType ) = 0;	//1 Tasks, 2 Assets

	virtual	int			GetColumnCount() const = 0;
	virtual const ZPTVColumnInfo &	GetNthColumnInfo(
							const int					inColumnIndex ) const = 0;
	virtual const ZPTVColumnInfo &	GetColumnInfoForColID(
							const int					inColID ) const = 0;
	virtual bool		ContainsColumnInfoForColID(
							const int					inColID ) const = 0;

	virtual const WidgetID &	GetColumnWidgetStartID() const = 0;

	static	void		GetTaskTreeColumnsInfo(
							ZPTVColumnInfoArr &			oColumnsInfo );
	static	void		GetAssetsTreeColumnsInfo(
							ZPTVColumnInfoArr &			oColumnsInfo );
};

extern const ZPTVColumnInfo		kZPEmptyTVColumnInfo;
#endif //_h_IZPTVColumnsInfo_
