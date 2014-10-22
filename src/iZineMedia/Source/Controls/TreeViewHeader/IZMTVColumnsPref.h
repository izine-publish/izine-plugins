//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/IZMTVColumnsPref.h $
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
#ifndef _h_IZMTVColumnsPref_
#define _h_IZMTVColumnsPref_
#pragma once

#include "ZMUITypes.h"

struct ZMTVColumnPref
{
	typedef  base_type   data_type;	//To support K2Vector

	int32			mColID;
	int32			mWidth;
	int32			mHeight;
	int32			mPositionIndex;		//Position is not the visible poisition.
	bool			mIsVisible;
	ZPColumnPoint	mColumnLeft;	//Left position of the column, not to be persisted.

	ZMTVColumnPref()
		: mColID( 0 ), mWidth( 0 ), mHeight( 0 ), mPositionIndex( -1 ), mIsVisible( false )
		, mColumnLeft( 0 )
	{}
};
typedef K2Vector<ZMTVColumnPref>	ZMTVColumnPrefArr;
extern const ZMTVColumnPref kZPTVEmptyColumnPref;

class IZMTVColumnsPref : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMTVCOLUMNSPREF };

	virtual const int			GetSortColumn() const = 0;
	virtual void				SetSortColumn(
									const int					inSortColumn ) = 0;
	virtual bool				IsSortAssending() const = 0;
	virtual void				SetSortAssending(
									const bool					inIsAssending ) = 0;

	virtual const ZMTVColumnPref & GetColumnPrefByColID(
									const int					inColID ) const = 0;
	virtual const ZMTVColumnPref & GetColumnPrefByPosition(
									const int					inColPositionIndex ) const = 0;
	virtual const ZMTVColumnPref & GetColumnPrefByVisibleIndex(
							const int					inColVisibleIndex ) const = 0;

	virtual int					GetVisibleColumnCount() const = 0;
	virtual bool				IsColumnVisible(
									const int					inColID ) const = 0;

	virtual ZPColumnPoint		GetLeftPositionByVisibleIndex(
									const int					inColVisibleIndex ) const = 0;
	virtual ZPColumnPoint		GetTotalWidthOfVisibleColumns() const = 0;
								

	virtual void				AddRemoveColumnPrefs() = 0;		//Sync with IZMTVColumnsInfo count
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


#endif //_h_IZMTVColumnsPref_
