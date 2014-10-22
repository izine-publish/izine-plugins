//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/IZPTVColumnsPref.h $
//	$Revision: 2454 $
//	$Date: 2010-11-03 16:14:12 +0100 (Wed, 03 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTVColumnsPref_
#define _h_IZPTVColumnsPref_
#pragma once

#include "ZPUITypes.h"

struct ZPTVColumnPref
{
	typedef  base_type   data_type;	//To support K2Vector

	int32			mColID;
	int32			mWidth;
	int32			mHeight;
	int32			mPositionIndex;		//Position is not the visible poisition.
	bool			mIsVisible;
	ZPColumnPoint	mColumnLeft;	//Left position of the column, not to be persisted.

	ZPTVColumnPref()
		: mColID( 0 ), mWidth( 0 ), mHeight( 0 ), mPositionIndex( -1 ), mIsVisible( false )
		, mColumnLeft( 0 )
	{}
};
typedef K2Vector<ZPTVColumnPref>	ZPTVColumnPrefArr;
extern const ZPTVColumnPref kZPTVEmptyColumnPref;

class IZPTVColumnsPref : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTVCOLUMNSPREF };

	virtual const int			GetSortColumn() const = 0;
	virtual void				SetSortColumn(
									const int					inSortColumn ) = 0;
	virtual bool				IsSortAssending() const = 0;
	virtual void				SetSortAssending(
									const bool					inIsAssending ) = 0;

	virtual const ZPTVColumnPref & GetColumnPrefByColID(
									const int					inColID ) const = 0;
	virtual const ZPTVColumnPref & GetColumnPrefByPosition(
									const int					inColPositionIndex ) const = 0;
	virtual const ZPTVColumnPref & GetColumnPrefByVisibleIndex(
							const int					inColVisibleIndex ) const = 0;

	virtual int					GetVisibleColumnCount() const = 0;
	virtual bool				IsColumnVisible(
									const int					inColID ) const = 0;

	virtual ZPColumnPoint		GetLeftPositionByVisibleIndex(
									const int					inColVisibleIndex ) const = 0;
	virtual ZPColumnPoint		GetTotalWidthOfVisibleColumns() const = 0;
								

	virtual void				AddRemoveColumnPrefs() = 0;		//Sync with IZPTVColumnsInfo count
	virtual void				ValidateSize() = 0;
	virtual void				ValidatePositionIndexes() = 0;
	virtual void				MoveColumnPositionIndex(
									int							inPositionFrom,
									int							inPositionTo) = 0;
	virtual void				RecalculatePositions() = 0;			//Calculates the left origins.


	virtual void				ToggleColumnVisibility(
									const int							inColID ) = 0;

	virtual int					GetVersion() const = 0;		//Used to check if info changed or not.
};


#endif //_h_IZPTVColumnsPref_
