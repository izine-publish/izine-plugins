//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAd/CZMAdsTVAdNodeWidgetMgr.cpp $
//	$Revision: 2772 $
//	$Date: 2011-04-11 12:03:16 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
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

//SDK General includes
#include "IconRsrcDefs.h"
#include "SysControlIds.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdInfo.h"
#include "IZMAdFileMgr.h"
#include "IZMTVColumnsInfo.h"
#include "IZMTVColumnsPref.h"
#include "IZMUITVNodeData.h"
#include "IZMUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZMDataHelper.h"

#include "CZMUITVWithHdrNodeWidgetMgr.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMUITVWithHdrNodeWidgetMgr
class CZMAdsTVAdNodeWidgetMgr : public inheritClass
{
public:
						CZMAdsTVAdNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZMAdsTVAdNodeWidgetMgr();

						//Updates the UI controls of this node.
	void				Refresh() const;

	void				AddRemoveColumnWidgets(
							const IZMTVColumnsPref *	inColPrefs );
	void				RepositionColumnWidgets(
							const IZMTVColumnsPref *	inColPrefs ) const;
protected:
	PMString			GetColumnText(
							const IStringData *			inAdID,
							const IZMAdInfo *			inAdInfo,
							enTVColAds					inColID ) const;
	PMRsrcID			GetColumnIcon(
							const IStringData *			inAdID,
							const IZMAdInfo *			inAdInfo,
							enTVColAds					inColID ) const;
	


private:

	InterfacePtr<const IZMAdFileMgr>	mAdFileMgr;	//Cache it as all the mgr are on this boss.
};


CREATE_PMINTERFACE(CZMAdsTVAdNodeWidgetMgr, kZMAdsTVAdNodeWidgetMgrImpl)

CZMAdsTVAdNodeWidgetMgr::CZMAdsTVAdNodeWidgetMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVAdNodeWidgetMgr::~CZMAdsTVAdNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddRemoveColumnWidgets
// Should be called when column count changes, when any column is shown/hidden.
// Or when new row widget is created.
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdNodeWidgetMgr::AddRemoveColumnWidgets(
	const IZMTVColumnsPref *	inColPrefs)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZMUIAdsTVNodeFirstColumnWidgetID;	//First column widget ID

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

	InterfacePtr<const IZMTVColumnsInfo> colsInfo( inColPrefs, UseDefaultIID() );
	ASSERT( colsInfo );

	//For visible columns, show column widget, Create if not already created.
	int theVisibleColCount = inColPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZMTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		if( currColPref.mColID == eTVColAds_Expander )
			continue;
		currWidgetID = widgetStartID + currColPref.mColID;

		const ZMTVColumnInfo & currColInfo = colsInfo->GetColumnInfoForColID( currColPref.mColID );

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
CZMAdsTVAdNodeWidgetMgr::RepositionColumnWidgets(
	const IZMTVColumnsPref *	inColPrefs) const
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZMUIAdsTVNodeFirstColumnWidgetID;	//First column widget ID

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
		const ZMTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		if( currColPref.mColID == eTVColAds_Expander )
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
CZMAdsTVAdNodeWidgetMgr::Refresh()const
{
	InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	const IStringData * assetID = (const IStringData*)nodeData->GetDataObj();

	if( !assetID )
		return;
	InterfacePtr<const IZMAdInfo> assetDetail( assetID, UseDefaultIID() );
	ASSERT( assetDetail );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZMUIAdsTVNodeFirstColumnWidgetID;	//First column widget ID

	//Update each column text
	int currControlCount = selfPanelCtrlData->Length();
	int i = 0;
	enTVColAds currColID = eTVColAds_Name;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	PMString currColText;
	PMRsrcID currColIcon;
	for( ; i < currControlCount ; ++i )
	{
		columnWidgetView = selfPanelCtrlData->GetWidget( i );
		currWidgetID = columnWidgetView->GetWidgetID();
		currColID = (enTVColAds)(currWidgetID - widgetStartID);
		ASSERT( currColID >= eTVColAds_NodeIcon && currColID < eTVColAdsCount );
		
		switch( currColID )	//Someday: we should use col type instead of colID
		{
		case eTVColAds_NodeIcon:
			currColIcon = this->GetColumnIcon( assetID, assetDetail, currColID );
			this->SetIcon( currWidgetID, currColIcon.fPluginId, currColIcon.fId );
			break;
		case eTVColAds_Name:
		case eTVColAds_Height:
		case eTVColAds_Width:
		case eTVColAds_Comment:
			currColText = this->GetColumnText( assetID, assetDetail, currColID );
			this->SetColumnText( currWidgetID, currColText );
			break;
		}
	}
	//InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	//ASSERT(selfControlView);
	
	//if( isNodeDisabled == false )
	//	selfControlView->Enable();
	//else
	//	selfControlView->Disable();

}

//----------------------------------------------------------------------------------------
// GetColumnText
//----------------------------------------------------------------------------------------
PMString
CZMAdsTVAdNodeWidgetMgr::GetColumnText(
	const IStringData *			inAdID,
	const IZMAdInfo *			inAdInfo,
	enTVColAds					inColID) const
{
	PMString toReturn;
	if( !inAdID || !inAdInfo )
		return toReturn;

	switch( inColID )
	{
	case eTVColAds_Name:
		{
			InterfacePtr<const IStringData> assetName( inAdID, IID_IZMDATANAME );
			ASSERT(assetName);
			toReturn = assetName->Get();
			break;
		}
	case eTVColAds_Comment:
		toReturn = inAdInfo->GetComments();
		break;
	case eTVColAds_Height:
		toReturn.AppendNumber( inAdInfo->GetAdHeight(), 3 );
		break;
	case eTVColAds_Width:
		toReturn.AppendNumber( inAdInfo->GetAdWidth(), 3 );
		break;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetColumnIcon
//----------------------------------------------------------------------------------------
PMRsrcID
CZMAdsTVAdNodeWidgetMgr::GetColumnIcon(
	const IStringData *			inAdID,
	const IZMAdInfo *			inAdInfo,
	enTVColAds					inColID) const
{
	PMRsrcID toReturn = {kInvalidRsrcID, kIZMediaPluginID };

	if( !inAdID || !inAdInfo )
		return toReturn;

	switch( inColID )
	{
	case eTVColAds_NodeIcon:
			toReturn.fId = kIZMNodeType_AdIconRsrcID;
		break;
	}
	return toReturn;
}




