//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/CZPTVColumnsInfo.cpp $
//	$Revision: 3004 $
//	$Date: 2011-05-12 12:32:17 +0200 (Thu, 12 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVColumnsInfo.h"

//IZP General includes
#include "ZPUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

const ZPTVColumnInfo		kZPEmptyTVColumnInfo;

#define inheritClass CPMUnknown<IZPTVColumnsInfo>
class CZPTVColumnsInfo : public inheritClass
{
public:
						CZPTVColumnsInfo(IPMUnknown * boss);
	virtual				~CZPTVColumnsInfo();

	void				InitCoumnsInfo(
							int							inTreeType );

	int					GetColumnCount() const;
	const ZPTVColumnInfo &	GetNthColumnInfo(
							const int					inColumnIndex ) const;
	const ZPTVColumnInfo &	GetColumnInfoForColID(
							const int					inColID ) const;

	bool				ContainsColumnInfoForColID(
							const int					inColID ) const;

	const WidgetID &	GetColumnWidgetStartID() const;

protected:
	ZPTVColumnInfoArr	mColumnsInfo;
	WidgetID			mColStartWidgetID;
private:
};


CREATE_PMINTERFACE(CZPTVColumnsInfo, kZPTVColumnsInfoImpl)

CZPTVColumnsInfo::CZPTVColumnsInfo(IPMUnknown * boss)
: inheritClass(boss)
, mColStartWidgetID( kZPUITVColumnHeaderStartWidgetID )
{
	
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVColumnsInfo::~CZPTVColumnsInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitCoumnsInfo
//----------------------------------------------------------------------------------------
void
CZPTVColumnsInfo::InitCoumnsInfo(
	int							inTreeType)
{
	if( inTreeType == 1 )
		GetTaskTreeColumnsInfo( mColumnsInfo );
	else if( inTreeType == 2 )
		GetAssetsTreeColumnsInfo( mColumnsInfo );
}

//----------------------------------------------------------------------------------------
// GetColumnCount
//----------------------------------------------------------------------------------------
int
CZPTVColumnsInfo::GetColumnCount()const
{
	return mColumnsInfo.Length();
}

//----------------------------------------------------------------------------------------
// GetNthColumnInfo
//----------------------------------------------------------------------------------------
const ZPTVColumnInfo &
CZPTVColumnsInfo::GetNthColumnInfo(
	const int					inColumnIndex) const
{
	ASSERT( inColumnIndex >= 0 && inColumnIndex < mColumnsInfo.Length() );

	return mColumnsInfo[inColumnIndex];
}

//----------------------------------------------------------------------------------------
// GetColumnInfoForColID
//----------------------------------------------------------------------------------------
const ZPTVColumnInfo &
CZPTVColumnsInfo::GetColumnInfoForColID(
	const int					inColID) const
{
	int theColCount = mColumnsInfo.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZPTVColumnInfo & currInfo = mColumnsInfo[i];
		if( currInfo.mColID == inColID )
			return currInfo;
	}
	return kZPEmptyTVColumnInfo;

}

//----------------------------------------------------------------------------------------
// ContainsColumnInfoForColID
//----------------------------------------------------------------------------------------
bool
CZPTVColumnsInfo::ContainsColumnInfoForColID(
	const int					inColID) const
{
	int theColCount = mColumnsInfo.Length();
	for( int i = 0; i < theColCount; ++i )
	{
		const ZPTVColumnInfo & currInfo = mColumnsInfo[i];
		if( currInfo.mColID == inColID )
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
// GetColumnWidgetStartID
//----------------------------------------------------------------------------------------
const WidgetID &
CZPTVColumnsInfo::GetColumnWidgetStartID()const
{
	return mColStartWidgetID;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskTreeColumnsInfo
//----------------------------------------------------------------------------------------
void
IZPTVColumnsInfo::GetTaskTreeColumnsInfo(
	ZPTVColumnInfoArr &			oColumnsInfo)
{
	oColumnsInfo.clear();

	ZPTVColumnInfo	colInfo;
	colInfo.mColID				=	eTVColTasks_Subject;
	colInfo.mMinWidth			=	400;
	colInfo.mMaxWidth			=	500;
	colInfo.mMinHeight			=	18;
	colInfo.mMaxHeight			=	18;
	colInfo.mCanHide			=	false;
	colInfo.mCanChangePosition	=	false;
	colInfo.mColType			=	eTVColType_Label;
	colInfo.mColDispName		=	kIZPTasksColHeaderSubjectKey;
	
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColTasks_Status;
	colInfo.mMinWidth			=	70;
	colInfo.mMaxWidth			=	200;
	colInfo.mCanHide			=	false;//true;
	colInfo.mCanChangePosition	=	true;
	colInfo.mColDispName		=	kIZPTasksColHeaderStatusKey;

	oColumnsInfo.push_back( colInfo );

#if 0 //Disable filtering. Redmine#1764, all filtering controls & columns are made hidden.
	colInfo.mColID				=	eTVColTasks_AssignedTo;
	colInfo.mColDispName		=	kIZPTasksColHeaderAssignedToKey;

	oColumnsInfo.push_back( colInfo );
#endif

	colInfo.mColID				=	eTVColTasks_Document;
	colInfo.mMinWidth			=	200;
	colInfo.mMaxWidth			=	300;
	colInfo.mColDispName		=	kIZPTasksColHeaderDocumentKey;

	oColumnsInfo.push_back( colInfo );

#if 0 //Disable filtering. Redmine#1764, all filtering controls & columns are made hidden.
	colInfo.mColID				=	eTVColTasks_Category;
	colInfo.mColDispName		=	kIZPTasksColHeaderCategoryKey;

	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColTasks_Spread;
	colInfo.mColDispName		=	kIZPTasksColHeaderSpreadKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColTasks_Page;
	colInfo.mColDispName		=	kIZPTasksColHeaderPageKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColTasks_UpdatedBy;
	colInfo.mColDispName		=	kIZPTasksColHeaderUpdatedByKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColTasks_UpdatedOn;
	colInfo.mColDispName		=	kIZPTasksColHeaderUpdatedOnKey;
	oColumnsInfo.push_back( colInfo );
#endif

	//colInfo.mColID				=	eTVColTasks_Creator;
	//colInfo.mColDispName		=	kIZPTasksColHeaderCreatorKey;

	//oColumnsInfo.push_back( colInfo );

	//colInfo.mColID				=	eTVColTasks_CreationDate;
	//colInfo.mColDispName		=	kIZPTasksColHeaderCreationDateKey;

	//oColumnsInfo.push_back( colInfo );
}

//----------------------------------------------------------------------------------------
// GetAssetsTreeColumnsInfo
//----------------------------------------------------------------------------------------
void
IZPTVColumnsInfo::GetAssetsTreeColumnsInfo(
	ZPTVColumnInfoArr &			oColumnsInfo)
{
	oColumnsInfo.clear();

	ZPTVColumnInfo	colInfo;

	colInfo.mColID				=	eTVColAssets_Expander;
	colInfo.mMinWidth			=	11;
	colInfo.mMaxWidth			=	11;
	colInfo.mMinHeight			=	1;
	colInfo.mMaxHeight			=	1;
	colInfo.mCanHide			=	false;
	colInfo.mCanChangePosition	=	false;
	colInfo.mColType			=	eTVColType_Empty;
	colInfo.mColDispName		=	kNullString;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_NodeIcon;
	colInfo.mMinWidth			=	18;
	colInfo.mMaxWidth			=	18;
	colInfo.mMinHeight			=	18;
	colInfo.mMaxHeight			=	18;
	colInfo.mCanHide			=	false;
	colInfo.mCanChangePosition	=	false;
	colInfo.mColType			=	eTVColType_Icon;
	colInfo.mColDispName		=	kNullString;
	
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_LinkTypeIcon;
	colInfo.mCanHide			=	true;
	colInfo.mCanChangePosition	=	true;
	colInfo.mColDispName		=	kIZPAssetsColHeaderLinkTypeIconKey;
	colInfo.mContextMenuDispName=	kIZpAssetsColHeaderMenuLinkTypeIconKey;

	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_LockIcon;
	colInfo.mColDispName		=	kIZpAssetsColHeaderLockIconKey;
	colInfo.mContextMenuDispName=	kIZpAssetsColHeaderMenuLockIconKey;

	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_Save;
	colInfo.mColDispName		=	kNullString;//kIZPAssetsColHeaderSaveKey;
	colInfo.mContextMenuDispName=	kIZpAssetsColHeaderMenuSaveIconKey;
	colInfo.mColType			=	eTVColType_RolloverIconButton;

	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_Delete;
	colInfo.mColDispName		=	kNullString;//kIZPAssetsColHeaderDeleteKey;
	colInfo.mContextMenuDispName=	kIZpAssetsColHeaderMenuDeleteIconKey;

	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_AssetName;
	colInfo.mMinWidth			=	200;
	colInfo.mMaxWidth			=	300;
	colInfo.mColDispName		=	kIZPAssetsColHeaderAssetNameKey;
	colInfo.mContextMenuDispName=	kNullString;
	colInfo.mColType			=	eTVColType_Label;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_Document;
	colInfo.mColDispName		=	kIZPAssetsColHeaderDocumentKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_VersionNum;
	colInfo.mMinWidth			=	20;
	colInfo.mMaxWidth			=	30;
	colInfo.mColDispName		=	kIZPAssetsColHeaderVersionNumKey;
	colInfo.mContextMenuDispName=	kIZpAssetsColHeaderMenuVersionNumberIconKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_VersionAuthor;
	colInfo.mMinWidth			=	100;
	colInfo.mMaxWidth			=	200;
	colInfo.mColDispName		=	kIZpAssetsColHeaderVersionAuthorKey;
	colInfo.mContextMenuDispName=	kNullString;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_VersionDate;
	colInfo.mMinWidth			=	90;
	colInfo.mMaxWidth			=	120;
	colInfo.mColDispName		=	kIZPAssetsColHeaderVersionDateKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_LocalState;
	colInfo.mMinWidth			=	90;
	colInfo.mMaxWidth			=	120;
	colInfo.mColDispName		=	kIZPAssetsColHeaderLocalStateKey;
	oColumnsInfo.push_back( colInfo );

	colInfo.mColID				=	eTVColAssets_Description;
	colInfo.mMinWidth			=	100;
	colInfo.mMaxWidth			=	300;
	colInfo.mColDispName		=	kIZPAssetsColHeaderDescriptionKey;
	oColumnsInfo.push_back( colInfo );
}
