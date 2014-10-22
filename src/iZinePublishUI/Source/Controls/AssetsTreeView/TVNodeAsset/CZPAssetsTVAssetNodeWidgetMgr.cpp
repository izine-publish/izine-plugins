//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVAssetNodeWidgetMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"
#include "CoreResTypes.h"
#include "IconRsrcDefs.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "SysControlIds.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLocalInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPAssetMgr.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"
#include "IZPTVColumnsInfo.h"
#include "IZPTVColumnsPref.h"
#include "IZPUITVNodeData.h"
#include "IZPUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZPDataHelper.h"
#include "ZPUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVWithHdrNodeWidgetMgr>
class CZPAssetsTVAssetNodeWidgetMgr : public inheritClass
{
public:
						CZPAssetsTVAssetNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZPAssetsTVAssetNodeWidgetMgr();

						//Updates the UI controls of this node.
	void				Refresh() const;

	void				AddRemoveColumnWidgets(
							const IZPTVColumnsPref *	inColPrefs );
	void				RepositionColumnWidgets(
							const IZPTVColumnsPref *	inColPrefs ) const;
protected:
	bool				CreateColumnWidget(	//As all the columns are text, so no need to pass the column type
							IPanelControlData *			inControlPanelData,
							IControlView * &			oControlView,
							const WidgetID &			inWidgetID,
							const enTVColumnType		inWidgetType );

	PMString			GetColumnText(
							const IStringData *			inAssetID,
							const IZPAssetInfo *		inAssetInfo,
							enTVColAssets				inColID ) const;
	PMRsrcID			GetColumnIcon(
							const IStringData *			inAssetID,
							const IZPAssetInfo *		inAssetInfo,
							enTVColAssets				inColID ) const;
	PMString			GetLocalStateDisplayStr(
							const IStringData *			inAssetID,
							const IZPAssetInfo *		inAssetInfo) const;
	
	void				SetColumnText(
							WidgetID					widgetID,
							const PMString &			inValue) const;

	void				SetIcon(
							WidgetID					inWidgetID,
							const PluginID &			inIconPluginID,
							const RsrcID &				inIconID) const;

	void				ShowColumn(
							WidgetID					inWidgetid) const;
	void				HideColumn(
							WidgetID					inWidgetid) const;

private:

	InterfacePtr<const IZPAssetMgr>	mAssetMgr;	//Cache it as all the mgr are on this boss.
};


CREATE_PMINTERFACE(CZPAssetsTVAssetNodeWidgetMgr, kZPAssetsTVAssetNodeWidgetMgrImpl)

CZPAssetsTVAssetNodeWidgetMgr::CZPAssetsTVAssetNodeWidgetMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVAssetNodeWidgetMgr::~CZPAssetsTVAssetNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddRemoveColumnWidgets
// Should be called when column count changes, when any column is shown/hidden.
// Or when new row widget is created.
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::AddRemoveColumnWidgets(
	const IZPTVColumnsPref *	inColPrefs)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZPUIAssetsTVNodeFirstColumnWidgetID;	//First column widget ID

	//Update the row width
	PMRect rowRect = selfControlView->GetFrame();
	//rowRect.SetWidth( inColPrefs->GetTotalWidthOfVisibleColumns() );
	//selfControlView->SetFrame( rowRect );	//Commented as this treeview don't have H scroll bar

	//Hide all the Column header widgets, the are not currently in visible list
	int currControlCount = selfPanelCtrlData->Length();
	int i = 0;
	int currColID = 0;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	for( ; i < currControlCount ; ++i )
	{
		columnWidgetView = selfPanelCtrlData->GetWidget( i );
		currWidgetID = columnWidgetView->GetWidgetID();
		currColID = currWidgetID - widgetStartID;
		
		if( currColID >= 0 && !inColPrefs->IsColumnVisible( currColID ) )
			columnWidgetView->Hide();
	}

	InterfacePtr<const IZPTVColumnsInfo> colsInfo( inColPrefs, UseDefaultIID() );
	ASSERT( colsInfo );

	//For visible columns, show column widget, Create if not already created.
	int theVisibleColCount = inColPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZPTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		if( currColPref.mColID == eTVColAssets_Expander )
			continue;
		currWidgetID = widgetStartID + currColPref.mColID;

		const ZPTVColumnInfo & currColInfo = colsInfo->GetColumnInfoForColID( currColPref.mColID );

		bool isNewCreated = this->CreateColumnWidget( selfPanelCtrlData, columnWidgetView, currWidgetID, currColInfo.mColType );
		ASSERT( columnWidgetView );
		if( !columnWidgetView )
			continue;

		if( isNewCreated )
		{
			//Set the Column widget size & position
			PMRect	theColRect(currColPref.mColumnLeft, 0, currColPref.mColumnLeft + currColPref.mWidth, currColPref.mHeight); //rowRect.Bottom() - rowRect.Top() );
			columnWidgetView->SetFrame( theColRect );
		}

		//Show if not currently visible
		if( !columnWidgetView->IsVisible() )
			columnWidgetView->Show();
	}
}

//----------------------------------------------------------------------------------------
// RepositionColumnWidgets
// Should be called when Any column moves or resizes.
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::RepositionColumnWidgets(
	const IZPTVColumnsPref *	inColPrefs) const
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZPUIAssetsTVNodeFirstColumnWidgetID;	//First column widget ID

	//Update the row width
	//PMRect rowRect = selfControlView->GetFrame();
	//rowRect.SetWidth( inColPrefs->GetTotalWidthOfVisibleColumns() );
	//selfControlView->SetFrame( rowRect );		//Commented as this treeview don't have H scroll bar

	//For visible columns, reposition the column widget
	int i = 0;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	int theVisibleColCount = inColPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZPTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		if( currColPref.mColID == eTVColAssets_Expander )
			continue;
		currWidgetID = widgetStartID + currColPref.mColID;

		columnWidgetView = selfPanelCtrlData->FindWidget( currWidgetID );
		ASSERT( columnWidgetView );
		if( !columnWidgetView )
			continue;
		//ASSERT( columnWidgetView->IsVisible() );		//Some col widgets are hidden during value settings. Like roll over btns

		//Set the Column widget width & position
		PMRect	theColRect = columnWidgetView->GetFrame();
		theColRect.Left() = currColPref.mColumnLeft;
		theColRect.SetWidth( currColPref.mWidth );
		columnWidgetView->SetFrame( theColRect );
	}
}

//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::Refresh()const
{
	InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	const IStringData * assetID = (const IStringData*)nodeData->GetDataObj();

	if( !assetID )
		return;

	InterfacePtr<const IZPAssetInfo> assetDetail( assetID, UseDefaultIID() );
	ASSERT( assetDetail );

	const ZPAssetDetailInfo & assetDetailInfo = assetDetail->GetAssetDetail();

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

#if 1	//use it when dynamic widgets are created for columns
	const WidgetID & widgetStartID = kZPUIAssetsTVNodeFirstColumnWidgetID;	//First column widget ID

	//Update each column text
	int currControlCount = selfPanelCtrlData->Length();
	int i = 0;
	enTVColAssets currColID = eTVColAssets_AssetName;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	PMString currColText;
	PMRsrcID currColIcon;
	for( ; i < currControlCount ; ++i )
	{
		columnWidgetView = selfPanelCtrlData->GetWidget( i );
		currWidgetID = columnWidgetView->GetWidgetID();
		currColID = (enTVColAssets)(currWidgetID - widgetStartID);
		ASSERT( currColID >= eTVColAssets_NodeIcon && currColID < eTVColAssetsCount );
		
		switch( currColID )	//Someday: we should use col type instead of colID
		{
		case eTVColAssets_NodeIcon:
		case eTVColAssets_LinkTypeIcon:
		case eTVColAssets_LockIcon:
		case eTVColAssets_Save:
		case eTVColAssets_Delete:
			currColIcon = this->GetColumnIcon( assetID, assetDetail, currColID );
			this->SetIcon( currWidgetID, currColIcon.fPluginId, currColIcon.fId );
			if( currColID == eTVColAssets_Save || currColID == eTVColAssets_Delete )
			{
				if( currColIcon.fId == 0 )
					HideColumn( currWidgetID );
				else
					ShowColumn( currWidgetID );
			}
			break;
		case eTVColAssets_AssetName:
		case eTVColAssets_Document:
		case eTVColAssets_VersionNum:
		case eTVColAssets_VersionAuthor:
		case eTVColAssets_VersionDate:
		case eTVColAssets_LocalState:
		case eTVColAssets_Description:
			currColText = this->GetColumnText( assetID, assetDetail, currColID );
			this->SetColumnText( currWidgetID, currColText );
			break;
		}
	}
#else
	SetColumnText( kZPUIAssetsTVNodeDocumentWidgetID, assetDetail->GetLinkedWithDocumentName());
	SetColumnText( kZPUIAssetsTVNodeVersionAuthorWidgetID, assetDetail->GetVersionAuthor());
		
	char versionnumber[5];
	sprintf(&versionnumber[0], "%d", assetDetail->GetVersionNumber());
		
	SetColumnText( kZPUIAssetsTVNodeVersionNumberWidgetID, PMString(versionnumber,-1,PMString::kNoTranslate));
	

	//TODO: is it used?
	//SetColumnText( kZPUIAssetsTVNodeDescriptionWidgetID, assetDetail->GetDescription());
	
	InterfacePtr<const IStringData> assetName( assetID, IID_IZPDATANAME );
	ASSERT(assetName);
	SetColumnText( kZPUIAssetsTVNodeNameWidgetID, assetName->Get());
		
	char date[20];
		
	const time_t & time = assetDetail->GetVersionDate();
	struct tm* ptm;
	ptm = localtime( &time );
	sprintf(&date[0], "%d-%d-%d %d:%02d", ptm->tm_mday, ptm->tm_mon+1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
	SetColumnText( kZPUIAssetsTVNodeVersionDateWidgetID, PMString(date,-1,PMString::kNoTranslate));
	
	RsrcID nodeIconRsrcID;
	if (assetDetail->IsTypeIDDocument())		// document
		nodeIconRsrcID = kIZPAssetType_IDCS3DocumentIconRsrcID;
	else if (assetDetail->IsTypeIDTemplate())// template
		nodeIconRsrcID = kIZPAssetType_IDCS3TemplateIconRsrcID;
	else if (assetDetail->IsTypeIDStory())
		nodeIconRsrcID = kIZPAssetType_IDCS3StoryIconRsrcID;
	else if (assetDetail->IsTypeAssignment())
		nodeIconRsrcID = kIZPAssetType_IDCS3AssignmentIconRsrcID ;
	else if(assetDetail->IsTypeICDocument())
		nodeIconRsrcID = kIZPAssetType_ICCS3DocumentIconRsrcID;
	else if(assetDetail->IsTypeICTemplate())
		nodeIconRsrcID = kIZPAssetType_ICCS3TemplateIconRsrcID;
	
	SetIcon(kZPUIAssetsTVNodeIconWidgetID, kIZPUIPluginID, nodeIconRsrcID );

	if (!assetDetailInfo.mDocInfo.mID.IsEmpty()) {
		//Someday: when the icon does not changes, why not only show/hide
		SetIcon(kZPUIAssetsTVNodeTypeIconWidgetID, kApplicationRsrcPluginID, kChainIconConstrained);
		ShowColumn( kZPUIAssetsTVNodeTypeIconWidgetID);
	} else {
		HideColumn( kZPUIAssetsTVNodeTypeIconWidgetID);
	}
	
	//show better lock icons i.e Locked - by self/other, not locked.
	InterfacePtr<const IZPAssetLockInfo> assetLockInfo( assetID, UseDefaultIID() );
	ASSERT( assetLockInfo );
	if (assetLockInfo->GetLockState() == IAMLockable::enLocked || assetLockInfo->GetLockState()== IAMLockable::enCheckedOut ) {
		//Someday: when the icon does not changes, why not only show/hide
		RsrcID iconID = kZPLinkEditState_EditingHereIconRsrcID;
		if( assetLockInfo->GetLockState() == IAMLockable::enLocked )
			iconID = kZPLinkEditState_EditingByOtherIconRsrcID;
		SetIcon(kZPUIAssetsTVNodeLockIconWidgetID, kStoryListPanelPluginID, iconID );
		ShowColumn( kZPUIAssetsTVNodeLockIconWidgetID);
	} else {
		HideColumn( kZPUIAssetsTVNodeLockIconWidgetID);
	}
	
	enZPUserRole currRole =	ZPDataHelper::GetCurrUserRole();	//user role from current login
	bool isKeyUser =  currRole == eUserRole_Admin || currRole == eUserRole_KeyUser;
	//RJK: commented type checking as all visible types were listed here.

	if (isKeyUser )//&& (assetDetail->IsTypeIDDocument() || assetDetail->IsTypeIDTemplate() || assetDetail->IsTypeAssignment()) )
		ShowColumn( kZPUIAssetsTVNodeDeleteRollOverIconButtonWidgetID);
	else
		HideColumn( kZPUIAssetsTVNodeDeleteRollOverIconButtonWidgetID);
		
	// only export indesign document and template nodes
	if (isKeyUser && (assetDetail->IsTypeIDDocument() || assetDetail->IsTypeIDTemplate()))
		ShowColumn( kZPUIAssetsTVNodeSaveRollOverIconButtonWidgetID);
	else
		HideColumn( kZPUIAssetsTVNodeSaveRollOverIconButtonWidgetID);

#endif
	//Nodes with-in Designing status for InCopy were disabled.
	//Now status's state is check for the same.

	bool isNodeDisabled = ( Utils<IZPInDnUtils>()->IsHostAppInCopy()
							&& assetDetail->IsTypeAssignment()
							&& IZPAssetMgr::GetAssetState( assetID ) == eTitleStatusState_Design );	//Using asset name will work as it is on same object

	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT(selfControlView);
	
	if( isNodeDisabled == false )
		selfControlView->Enable();
	else
		selfControlView->Disable();

}

//----------------------------------------------------------------------------------------
// GetColumnText
//----------------------------------------------------------------------------------------
PMString
CZPAssetsTVAssetNodeWidgetMgr::GetColumnText(
	const IStringData *			inAssetID,
	const IZPAssetInfo *		inAssetInfo,
	enTVColAssets				inColID) const
{
	PMString toReturn;
	if( !inAssetID || !inAssetInfo )
		return toReturn;

	switch( inColID )
	{
	case eTVColAssets_AssetName:
		{
			InterfacePtr<const IStringData> assetName( inAssetID, IID_IZPDATANAME );
			ASSERT(assetName);
			toReturn = assetName->Get();
			break;
		}
	case eTVColAssets_Document:
		toReturn = inAssetInfo->GetLinkedWithDocumentName();
		break;
	case eTVColAssets_VersionNum:
		{
			char versionnumber[5];
			sprintf(&versionnumber[0], "%d", inAssetInfo->GetVersionNumber());
		
#if defined(InDnCS5) || defined(InDnCS5_5)
			toReturn = PMString(versionnumber, PMString::kDontTranslateDuringCall);
#else
			toReturn = PMString(versionnumber, -1, PMString::kNoTranslate);
#endif
		}
		break;
	case eTVColAssets_VersionAuthor:
		toReturn = inAssetInfo->GetVersionAuthor();
		break;
	case eTVColAssets_VersionDate:
		{
			char date[20];
		
			const time_t & time = inAssetInfo->GetVersionDate();
			struct tm* ptm;
			ptm = localtime( &time );
			sprintf(&date[0], "%d-%d-%d %d:%02d", ptm->tm_mday, ptm->tm_mon+1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
#if defined(InDnCS5) || defined(InDnCS5_5)
			toReturn = PMString(date, PMString::kDontTranslateDuringCall);
#else
			toReturn = PMString(date, -1, PMString::kNoTranslate);
#endif
		}
		break;
	case eTVColAssets_LocalState:
		toReturn = this->GetLocalStateDisplayStr(inAssetID, inAssetInfo);
		break;
	case eTVColAssets_Description:
		toReturn = inAssetInfo->GetDescription();
		break;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetColumnIcon
//----------------------------------------------------------------------------------------
PMRsrcID
CZPAssetsTVAssetNodeWidgetMgr::GetColumnIcon(
	const IStringData *			inAssetID,
	const IZPAssetInfo *		inAssetInfo,
	enTVColAssets				inColID) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
	PMRsrcID toReturn;
	toReturn.fId = kInvalidRsrcID;
	toReturn.fPluginId = kIZPUIPluginID;
#else
	PMRsrcID toReturn = {kInvalidRsrcID, kIZPUIPluginID };
#endif

	if( !inAssetID || !inAssetInfo )
		return toReturn;

	switch( inColID )
	{
	case eTVColAssets_NodeIcon:
		if (inAssetInfo->IsTypeIDDocument())		// document
			toReturn.fId = kIZPAssetType_IDCS3DocumentIconRsrcID;
		else if (inAssetInfo->IsTypeIDTemplate())// template
			toReturn.fId = kIZPAssetType_IDCS3TemplateIconRsrcID;
		else if (inAssetInfo->IsTypeIDStory())
			toReturn.fId = kIZPAssetType_IDCS3StoryIconRsrcID;
		else if (inAssetInfo->IsTypeAssignment())
			toReturn.fId = kIZPAssetType_IDCS3AssignmentIconRsrcID ;
		else if(inAssetInfo->IsTypeICDocument())
			toReturn.fId = kIZPAssetType_ICCS3DocumentIconRsrcID;
		else if(inAssetInfo->IsTypeICTemplate())
			toReturn.fId = kIZPAssetType_ICCS3TemplateIconRsrcID;
		break;
	case eTVColAssets_LinkTypeIcon:
		if (!inAssetInfo->GetLinkedWithDocumentID().IsEmpty())
		{
			toReturn.fId = kChainIconConstrained;
			toReturn.fPluginId = kApplicationRsrcPluginID;
		}
		break;
	case eTVColAssets_LockIcon:
		{	//show better lock icons i.e Locked - by self/other, not locked.
			InterfacePtr<const IZPAssetLockInfo> assetLockInfo( inAssetID, UseDefaultIID() );
			ASSERT( assetLockInfo );
			if (assetLockInfo->GetLockState() == IAMLockable::enLocked || assetLockInfo->GetLockState()== IAMLockable::enCheckedOut )
			{	
				toReturn.fId = kZPLinkEditState_EditingHereIconRsrcID;
				toReturn.fPluginId = kStoryListPanelPluginID;
				if( assetLockInfo->GetLockState() == IAMLockable::enLocked )
					toReturn.fId = kZPLinkEditState_EditingByOtherIconRsrcID;
			}
		}
		break;
	case eTVColAssets_Save:
		{
			enZPUserRole currRole =	ZPDataHelper::GetCurrUserRole();	//user role from current login
			bool isKeyUser =  currRole == eUserRole_Admin || currRole == eUserRole_KeyUser;
			// only export indesign document and template nodes
			if (isKeyUser && (inAssetInfo->IsTypeIDDocument() || inAssetInfo->IsTypeIDTemplate()))
			{
				toReturn.fId = kNewObjectButtonIconRollOver;
				toReturn.fPluginId = kApplicationRsrcPluginID;
			}
		}
		break;
	case eTVColAssets_Delete:
		{
			enZPUserRole currRole1 =	ZPDataHelper::GetCurrUserRole();	//user role from current login
			bool isKeyUser1 =  currRole1 == eUserRole_Admin || currRole1 == eUserRole_KeyUser;
			//RJK: commented type checking as all visible types were listed here.

			if (isKeyUser1 )//&& (assetDetail->IsTypeIDDocument() || assetDetail->IsTypeIDTemplate() || assetDetail->IsTypeAssignment()) )
			{
				toReturn.fId = kDeleteObjectButtonIconRollOver;				
				toReturn.fPluginId = kApplicationRsrcPluginID;
			}
		}
		break;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetLocalStateDisplayStr
//----------------------------------------------------------------------------------------
PMString
CZPAssetsTVAssetNodeWidgetMgr::GetLocalStateDisplayStr(
	const IStringData *			inAssetID,
	const IZPAssetInfo *		inAssetInfo) const
{
	PMString result = "";
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if (!inAssetID)
	{
		ASSERT (inAssetID);
		return result;
	}

	const PMString & theAssetIDStr = inAssetID->Get();

	do
	{
		InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
		ASSERT( assetsLocalInfoMgr );

		InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( theAssetIDStr, true ) );
		if (!assetLocalInfo)
		{
			result = kIZpAssetsLocalStateNotFetchedKey;
			break;
		}

		IDFile filePath;
		Utils<IZPAMHelper>()->GetAssetFilePath( inAssetID, filePath );
		if (Utils<IZPFileUtils>()->DoesFileExist(filePath) == kFalse)
		{
			result = kIZpAssetsLocalStateNotFetchedKey;
			break;
		}
		else if (assetLocalInfo->GetFileIsLocallyModified() == kTrue)
		{
			result = kIZpAssetsLocalStateLocallyModifiedKey;
			break;
		}
		else if (assetLocalInfo->GetFileVersionNumber() == inAssetInfo->GetVersionNumber())
		{
			result = kIZpAssetsLocalStateUptoDateKey;
			break;
		}
		else
		{
			result = kIZpAssetsLocalStateOutOfDateKey;
		}
	}while(kFalse);

	result.Translate();

	return result;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetColumnText
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::SetColumnText(
	WidgetID					widgetID,
	const PMString &			inValue) const
{
	do {
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if(selfPanelControlData==nil) {
			break;
		}
		
		IControlView* controlView = selfPanelControlData->FindWidget( widgetID );
		ASSERT(controlView);
		if (controlView==nil)
			break;

		InterfacePtr<ITextControlData> textControlData( controlView, UseDefaultIID() );
		ASSERT(textControlData);

		PMString strValue(inValue);
		strValue.SetTranslatable(kFalse);
		
		textControlData->SetString(strValue);

	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// SetIcon
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::SetIcon(
	WidgetID					inWidgetid,
	const PluginID &			inIconPluginID,
	const RsrcID &				inIconID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData( this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	
	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	bool needsRefresh = controlView->GetRsrcID() != inIconID ;
	controlView->SetRsrcPluginID( inIconPluginID );
	controlView->SetRsrcID( inIconID );
	if( needsRefresh )
		controlView->Invalidate();
}

//----------------------------------------------------------------------------------------
// ShowColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::ShowColumn(
	WidgetID					inWidgetid) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);

	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	controlView->Show();
}

//----------------------------------------------------------------------------------------
// HideColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeWidgetMgr::HideColumn(
	WidgetID					inWidgetid) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);

	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	controlView->Hide();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateColumnWidget
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVAssetNodeWidgetMgr::CreateColumnWidget(
	IPanelControlData *					inControlPanelData,
	IControlView * &					oControlView,
	const WidgetID &					inWidgetID,
	const enTVColumnType				inWidgetType)
{
	do
	{
		oControlView = inControlPanelData->FindWidget( inWidgetID, 2 );
		if( !oControlView )
		{
			InterfacePtr<IControlView> newWidgetView;

			switch( inWidgetType )
			{
				case eTVColType_Label:
					newWidgetView.reset(static_cast<IControlView*>( ::CreateObject( ::GetDataBase(this),
									RsrcSpec( LocaleSetting::GetLocale(), kIZPUIPluginID, kViewRsrcType, kZPUITVColumnTxtWidgetRsrcID),
									IControlView::kDefaultIID ) ));
				break;
				case eTVColType_Icon:
					newWidgetView.reset(static_cast<IControlView*>( ::CreateObject( ::GetDataBase(this),
									RsrcSpec( LocaleSetting::GetLocale(), kIZPUIPluginID, kViewRsrcType, kZPUITVColumnIconWidgetRsrcID),
									IControlView::kDefaultIID ) ));
				break;
				case eTVColType_RolloverIconButton:
					newWidgetView.reset(static_cast<IControlView*>( ::CreateObject( ::GetDataBase(this),
									RsrcSpec( LocaleSetting::GetLocale(), kIZPUIPluginID, kViewRsrcType, kZPUITVColumnRollOverIconButtonWidgetRsrcID),
									IControlView::kDefaultIID ) ));
				break;
			}
			
			ASSERT(newWidgetView);
			if(!newWidgetView)
				break;
			
			inControlPanelData->AddWidget( newWidgetView );
			newWidgetView->SetWidgetID( inWidgetID );
			oControlView = newWidgetView;
			return true;
		}
	}while(kFalse);
	return false;
}
