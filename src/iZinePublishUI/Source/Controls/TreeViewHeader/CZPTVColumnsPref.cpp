//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/CZPTVColumnsPref.cpp $
//	$Revision: 3709 $
//	$Date: 2011-12-21 08:08:06 +0100 (Wed, 21 Dec 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVColumnsPref.h"
#include "IZPTVColumnsInfo.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

const ZPTVColumnPref	kZPTVEmptyColumnPref;

#define inheritClass CPMUnknown<IZPTVColumnsPref>
class CZPTVColumnsPref : public inheritClass
{
public:
						CZPTVColumnsPref(IPMUnknown * boss);
	virtual				~CZPTVColumnsPref();

	const int			GetSortColumn() const;
	void				SetSortColumn(
							const int					inSortColumn );

	bool				IsSortAssending() const;
	void				SetSortAssending(
							const bool					inIsAssending );

	const ZPTVColumnPref & GetColumnPrefByColID(
							const int					inColID ) const;
	const ZPTVColumnPref & GetColumnPrefByPosition(
							const int					inColPositionIndex ) const;
	const ZPTVColumnPref & GetColumnPrefByVisibleIndex(
							const int					inColVisibleIndex ) const;
	bool				ContainsColumnPrefForColID(
							const int					inColID ) const;

	int					GetVisibleColumnCount() const;
	bool				IsColumnVisible(
							const int					inColID ) const;

	ZPColumnPoint		GetLeftPositionByVisibleIndex(
							const int					inColVisibleIndex ) const;
	ZPColumnPoint		GetTotalWidthOfVisibleColumns() const;

	void				AddRemoveColumnPrefs();		//Sync with IZPTVColumnsInfo count
	void				ValidateSize();
	void				ValidatePositionIndexes();
	void				MoveColumnPositionIndex(
							int							inPositionFrom,
							int							inPositionTo);

	void				RecalculatePositions();			//Calculates the left origins.

	void				ToggleColumnVisibility(
							const int							inColID );

	int					GetVersion() const;

	void				ReadWrite(
							IPMStream *					s,
							ImplementationID			prop);
protected:
	void				UpdateVisibleColIndexes();
	void				ValidateSize(
							ZPTVColumnPref &			inColPref,
							const ZPTVColumnInfo &		inColInfo );
private:
	int32		mSortColumn;
	bool16		mIsSortAssending;
	ZPTVColumnPrefArr	mColumnsPrefs;
	ZPColumnPoint		mTotalColWidth;
	K2Vector<int>		mVisibleColIndexes;

	int			mVersion;	//Used to check if information changed or not.
};


CREATE_PERSIST_PMINTERFACE(CZPTVColumnsPref, kZPTVColumnsPrefPersistImpl)

CZPTVColumnsPref::CZPTVColumnsPref(IPMUnknown * boss)
: inheritClass(boss)
, mSortColumn( 0 ), mIsSortAssending( kTrue )
, mTotalColWidth( 0 )
, mVersion( 1 )
{
	IZPLog_Str_( thisFileLA, enLT_Details, "TreeViewHeader: created at :%x", this );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVColumnsPref::~CZPTVColumnsPref()
{
	IZPLog_Str_( thisFileLA, enLT_Details, "TreeViewHeader: destructed at :%x", this );
}

#pragma mark -
//Used to compare the column position index, -1 goes last, rest comes assending.
struct	ComparePositionIndex {
	
	bool operator()(
			const ZPTVColumnPref &		lhs,
			const ZPTVColumnPref &		rhs) const 
	{
		const int & leftPosition = lhs.mPositionIndex;
		const int &	rightPosition = rhs.mPositionIndex;

		if( ( leftPosition != -1) 
		&&  ( rightPosition != -1) ){	// if both are +ve, then make then assending.
			return leftPosition < rightPosition ;
		}
		else if( (leftPosition == -1) 
		&&		(rightPosition == -1)){	// if both are new decide according to column ID.
			return lhs.mColID < rhs.mColID ;
		}
		else if( (leftPosition == -1) 
			&&	(rightPosition != -1)){	// if right position is old make it first.
			return false;
		}
		else {
			return true;				// else left position is old, so make it first.
		}
	}
};
#pragma mark -
//----------------------------------------------------------------------------------------
// GetSortColumn
//----------------------------------------------------------------------------------------
const int
CZPTVColumnsPref::GetSortColumn()const
{
	return mSortColumn;
}

//----------------------------------------------------------------------------------------
// SetSortColumn
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::SetSortColumn(
	const int					inSortColumn)
{
	PreDirty();
	mSortColumn = inSortColumn;
}

//----------------------------------------------------------------------------------------
// IsSortAssending
//----------------------------------------------------------------------------------------
bool
CZPTVColumnsPref::IsSortAssending() const
{
	return mIsSortAssending;
}

//----------------------------------------------------------------------------------------
// SetSortAssending
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::SetSortAssending(
	const bool					inIsAssending)
{
	LogFunctionEnterExit;
	PreDirty();
	mIsSortAssending = inIsAssending;
}

//----------------------------------------------------------------------------------------
// GetColumnPrefByColID
//----------------------------------------------------------------------------------------
const ZPTVColumnPref &
CZPTVColumnsPref::GetColumnPrefByColID(
	const int					inColID) const
{
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( currPref.mColID == inColID )
			return currPref;
	}
	
	return kZPTVEmptyColumnPref;
}

//----------------------------------------------------------------------------------------
// GetColumnPrefByPosition
//----------------------------------------------------------------------------------------
const ZPTVColumnPref &
CZPTVColumnsPref::GetColumnPrefByPosition(
	const int					inColPositionIndex) const
{
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( currPref.mPositionIndex == inColPositionIndex )
			return currPref;
	}
	return kZPTVEmptyColumnPref;
}

//----------------------------------------------------------------------------------------
// GetColumnPrefByVisibleIndex
//----------------------------------------------------------------------------------------
const ZPTVColumnPref &
CZPTVColumnsPref::GetColumnPrefByVisibleIndex(
	const int					inColVisibleIndex) const
{
	ASSERT( inColVisibleIndex >= 0 && inColVisibleIndex < mVisibleColIndexes.Length() );
	if( inColVisibleIndex >= 0 && inColVisibleIndex < mVisibleColIndexes.Length() )
	{
		int colIndex = mVisibleColIndexes[ inColVisibleIndex ];
		ASSERT( colIndex < mColumnsPrefs.Length() );
		const ZPTVColumnPref & currPref = mColumnsPrefs[colIndex];
		return currPref;
	}
	return kZPTVEmptyColumnPref;
}

//----------------------------------------------------------------------------------------
// ContainsColumnPrefForColID
//----------------------------------------------------------------------------------------
bool
CZPTVColumnsPref::ContainsColumnPrefForColID(
	const int					inColID) const
{
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( currPref.mColID == inColID )
			return true;
	}
	
	return false;
}

//----------------------------------------------------------------------------------------
// GetVisibleColumnCount
//----------------------------------------------------------------------------------------
int
CZPTVColumnsPref::GetVisibleColumnCount()const
{
	int theVisibleColCount = mVisibleColIndexes.Length();
	return theVisibleColCount;
}

//----------------------------------------------------------------------------------------
// IsColumnVisible
//----------------------------------------------------------------------------------------
bool
CZPTVColumnsPref::IsColumnVisible(
	const int					inColID) const
{
	const ZPTVColumnPref & columnPref = this->GetColumnPrefByColID( inColID );
	return columnPref.mIsVisible;
}

//----------------------------------------------------------------------------------------
// GetLeftPositionByVisibleIndex
//----------------------------------------------------------------------------------------
ZPColumnPoint
CZPTVColumnsPref::GetLeftPositionByVisibleIndex(
	const int					inColVisibleIndex) const
{
	const ZPTVColumnPref & columnPref = this->GetColumnPrefByVisibleIndex( inColVisibleIndex );
	return columnPref.mColumnLeft;
}

//----------------------------------------------------------------------------------------
// GetTotalWidthOfVisibleColumns
//----------------------------------------------------------------------------------------
ZPColumnPoint
CZPTVColumnsPref::GetTotalWidthOfVisibleColumns()const
{
	return mTotalColWidth;
}

//----------------------------------------------------------------------------------------
// AddRemoveColumnPrefs
//Sync with IZPTVColumnsInfo count
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::AddRemoveColumnPrefs()
{
	LogFunctionEnterExit;

	InterfacePtr<const IZPTVColumnsInfo> selfColInfo( this, UseDefaultIID() );
	ASSERT( selfColInfo );

	++mVersion;
	PreDirty();

	//Remove items that are not present in column info.
	//TODO:

	//Add items that are newly added in column info.
	int colCount = selfColInfo->GetColumnCount();

	for( int i = 0; i < colCount ; ++i )
	{
		const ZPTVColumnInfo & currColInfo = selfColInfo->GetNthColumnInfo( i );
		ASSERT( currColInfo.mColID >= 0 );
		if( !this->ContainsColumnPrefForColID( currColInfo.mColID ) )
		{	//Add pref for this column
			ZPTVColumnPref newColPref;
			newColPref.mColID = currColInfo.mColID;
			newColPref.mIsVisible = true;
			mColumnsPrefs.push_back( newColPref );
			IZPLog_Str_( thisFileLA, enLT_Details, "Column pref added for columnID %d", newColPref.mColID );
		}
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ValidateSize
// Validates size with its column info
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::ValidateSize()
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPTVColumnsInfo> selfColInfo( this, UseDefaultIID() );
	ASSERT( selfColInfo );
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		ZPTVColumnPref & currPref = mColumnsPrefs[i];
		const ZPTVColumnInfo & currColInfo = selfColInfo->GetColumnInfoForColID( currPref.mColID );
		ASSERT( currColInfo.mColID == currPref.mColID );
		
		this->ValidateSize( currPref, currColInfo );
	}
}

//----------------------------------------------------------------------------------------
// ValidatePositionIndexes
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::ValidatePositionIndexes()
{
	LogFunctionEnterExit;
	PreDirty();
	//Sort columns based on position index. -1 comes at end and other become assending.
	std::sort(mColumnsPrefs.begin(),mColumnsPrefs.end(), ComparePositionIndex());

	//Now columns are sorted, Renumber Position indexes
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		ZPTVColumnPref & currPref = mColumnsPrefs[i];
		currPref.mPositionIndex = i;
	}
}

//----------------------------------------------------------------------------------------
// MoveColumnPositionIndex
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::MoveColumnPositionIndex(
	int							inPositionFrom,
	int							inPositionTo)
{
	LogFunctionEnterExit;
	PreDirty();
	//TODO: Required with DnD
}

//----------------------------------------------------------------------------------------
// RecalculatePositions
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::RecalculatePositions()			//Calculates the left origins.
{
	LogFunctionEnterExit;
	//Pre-conditions: Array must be in sync with columns info.
					//position index must be in increasing order.
					//size must already be validated with columns info.

	++mVersion;

	this->UpdateVisibleColIndexes();

	int theColCount = mColumnsPrefs.Length();
	ZPColumnPoint	theColumnStartPoint = 0;
	for( int i = 0; i < theColCount; ++i )
	{
		ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( !currPref.mIsVisible )
			continue;

		currPref.mColumnLeft = theColumnStartPoint;
		theColumnStartPoint += currPref.mWidth + 1;
		IZPLog_Str_( thisFileLA, enLT_Details, "columnID %d has left point %lf", currPref.mColID, currPref.mColumnLeft );
	}
	mTotalColWidth = theColumnStartPoint;
}

//----------------------------------------------------------------------------------------
// UpdateVisibleColIndexes
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::UpdateVisibleColIndexes()
{
	LogFunctionEnterExit;
	mVisibleColIndexes.clear();
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( currPref.mIsVisible )
			mVisibleColIndexes.push_back( i );
	}
}
#pragma mark -
//----------------------------------------------------------------------------------------
// ToggleColumnVisibility
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::ToggleColumnVisibility(
	const int							inColID)
{
	int theColCount = mColumnsPrefs.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		ZPTVColumnPref & currPref = mColumnsPrefs[i];
		if( currPref.mColID == inColID )
		{
			PreDirty();
			currPref.mIsVisible = !currPref.mIsVisible;
			break;
		}
	}
}

//----------------------------------------------------------------------------------------
// GetVersion
//----------------------------------------------------------------------------------------
int
CZPTVColumnsPref::GetVersion()const
{
	return mVersion;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ValidateSize
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::ValidateSize(
	ZPTVColumnPref &			inColPref,
	const ZPTVColumnInfo &		inColInfo)
{
	LogFunctionEnterExit;

	IZPLog_Str_( thisFileLA, enLT_Details, "Validating size for columnID %d", inColPref.mColID );

	if( inColPref.mWidth <= 0 || inColPref.mWidth < inColInfo.mMinWidth )
	{	//Set minimum width
		inColPref.mWidth = inColInfo.mMinWidth;
		if( inColPref.mWidth <= 0 )
		{
			PreDirty();
			inColPref.mWidth = 1;	//Mininum width
		}
	}
	else if( inColInfo.mMaxWidth > 0 &&  inColPref.mWidth > inColInfo.mMaxWidth )
	{	//Limit the size to maximum
		PreDirty();
		inColPref.mWidth = inColInfo.mMaxWidth;
	}
	//else width is OK

	if( inColPref.mHeight <= 0 || inColPref.mHeight < inColInfo.mMinHeight )
	{	//Set minimum height
		inColPref.mHeight = inColInfo.mMinHeight;
		if( inColPref.mHeight <= 0 )
		{
			//PreDirty();			//Height is not persisted
			inColPref.mHeight = 1;	//Mininum Height
		}
	}
	else if( inColInfo.mMaxHeight > 0 &&  inColPref.mHeight > inColInfo.mMaxHeight )
	{	//Limit the size to maximum
		//PreDirty();			//Height is not persisted
		inColPref.mHeight = inColInfo.mMaxHeight;
	}
	//else height is OK
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPTVColumnsPref::ReadWrite(
	IPMStream *					s,
	ImplementationID			prop)
{
	//Sorting Info
	s->XferInt32( mSortColumn );
	s->XferBool( mIsSortAssending );

	//Column Prefs array
		//Size
		int16 arrSize = mColumnsPrefs.size();
		s->XferInt16( arrSize );
	if( s->IsReading() )
	{
		IZPLog_Str_( thisFileLA, enLT_Details, "Reading prefs.");
		mColumnsPrefs.clear();
		mColumnsPrefs.reserve( arrSize );
		for( int i = 0; i < arrSize ; ++i )
		{
			ZPTVColumnPref thePrefRead;
			s->XferInt32( thePrefRead.mColID );
			s->XferInt32( thePrefRead.mWidth );
			//height is not persisted as it does not changes.
			s->XferInt32( thePrefRead.mPositionIndex );
			s->XferByte( (uchar&)(thePrefRead.mIsVisible) );
			mColumnsPrefs.push_back( thePrefRead );
			IZPLog_Str_( thisFileLA, enLT_Details, "columnID %d has left point %lf, position index : %d, visible: %hd",
				thePrefRead.mColID, thePrefRead.mColumnLeft, thePrefRead.mPositionIndex, (short int)thePrefRead.mIsVisible );
		}
		IZPLog_Str_( thisFileLA, enLT_Details, "Reading prefs done.");
	}
	else
	{
		IZPLog_Str_( thisFileLA, enLT_Details, "Writing prefs.");
		for( int j = 0; j < arrSize ; ++j )
		{
			ZPTVColumnPref & thePref = mColumnsPrefs[j];
			s->XferInt32( thePref.mColID );
			s->XferInt32( thePref.mWidth );
			s->XferInt32( thePref.mPositionIndex );
			s->XferByte( (uchar&)(thePref.mIsVisible) );
			IZPLog_Str_( thisFileLA, enLT_Details, "columnID %d has left point %lf, position index : %d, visible: %hd",
				thePref.mColID, thePref.mColumnLeft, thePref.mPositionIndex, (short int)thePref.mIsVisible );
		}
		IZPLog_Str_( thisFileLA, enLT_Details, "Writing prefs done.");
	}
	//ZPTVColumnPrefArr	mColumnsPrefs;
}
