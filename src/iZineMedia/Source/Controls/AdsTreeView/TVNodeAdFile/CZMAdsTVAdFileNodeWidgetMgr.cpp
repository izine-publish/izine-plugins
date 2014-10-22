//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAdFile/CZMAdsTVAdFileNodeWidgetMgr.cpp $
//	$Revision: 2772 $
//	$Date: 2011-04-11 12:03:16 +0200 (Mon, 11 Apr 2011) $
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
#include "IStringData.h"
#include "IPanelControlData.h"

//SDK General includes
#include "IconRsrcDefs.h"
#include "SysControlIds.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
//#include "IZMUITVWithHdrNodeWidgetMgr.h"
#include "IZMTVColumnsInfo.h"
#include "IZMTVColumnsPref.h"
#include "IZMUITVNodeData.h"

//IZP General includes
#include "CZMUITVWithHdrNodeWidgetMgr.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMUITVWithHdrNodeWidgetMgr
class CZMAdsTVAdFileNodeWidgetMgr : public inheritClass
{
public:
						CZMAdsTVAdFileNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZMAdsTVAdFileNodeWidgetMgr();

						//Updates the UI controls of this node.
	void				Refresh() const;

	void				AddRemoveColumnWidgets(
							const IZMTVColumnsPref *	inColPrefs );
	void				RepositionColumnWidgets(
							const IZMTVColumnsPref *	inColPrefs ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZMAdsTVAdFileNodeWidgetMgr, kZMAdsTVAdFileNodeWidgetMgrImpl)

CZMAdsTVAdFileNodeWidgetMgr::CZMAdsTVAdFileNodeWidgetMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVAdFileNodeWidgetMgr::~CZMAdsTVAdFileNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdFileNodeWidgetMgr::Refresh()const
{
	InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	const IStringData * adFileID = (const IStringData*)nodeData->GetDataObj();

	if( !adFileID )
		return;
	InterfacePtr<const IStringData> statusName( adFileID, IID_IZMDATANAME );
	ASSERT( statusName );
	
	PMString theAdFileName;
	if( statusName )
		theAdFileName = statusName->Get( );

	theAdFileName.SetTranslatable( kFalse );
	this->SetColumnText( kZMUIAdsTVNodeNameWidgetID, theAdFileName );

	//This icon is fixed, then why are we setting this each time.
	this->SetIcon( kZMUIAdsTVNodeIconWidgetID, kApplicationRsrcPluginID, kGenericFolderRolloverIconID);
}

//----------------------------------------------------------------------------------------
// AddRemoveColumnWidgets
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdFileNodeWidgetMgr::AddRemoveColumnWidgets(
	const IZMTVColumnsPref *	inColPrefs)
{
	//DO nothing as name column does not changes its position.
}

//----------------------------------------------------------------------------------------
// RepositionColumnWidgets
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdFileNodeWidgetMgr::RepositionColumnWidgets(
	const IZMTVColumnsPref *	inColPrefs) const
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZMUIAdsTVNodeFirstColumnWidgetID;	//First column widget ID

	if( !inColPrefs )
		return;

	//Move the ad file name widget in-align to asset name widget. i.e eTVColAds_Name column.
	const ZMTVColumnPref & nameColPref = inColPrefs->GetColumnPrefByColID( eTVColAds_Name );

	InterfacePtr<const IPanelControlData> nodePanelCtrlData( selfControlView, UseDefaultIID() );
	ASSERT( nodePanelCtrlData );
	IControlView * nameColumnWidgetView = nodePanelCtrlData->FindWidget( kZMUIAdsTVNodeNameWidgetID );
	ASSERT( nameColumnWidgetView );
	if( nameColumnWidgetView )
	{
		//Set the Column widget width & position
		PMRect	theNameColRect = nameColumnWidgetView->GetFrame();
		PMRect	theNameColBBoxRect = nameColumnWidgetView->GetBBox();
		PMRect	theNodeBBoxRect = selfControlView->GetBBox();
		PMReal theLeftPosition = nameColPref.mColumnLeft - theNameColBBoxRect.Left() + theNodeBBoxRect.Left() + theNameColRect.Left();
		if( theNameColRect.Left() != theLeftPosition )//nameColPref.mColumnLeft )
		{
			theNameColRect.MoveTo( theLeftPosition, theNameColRect.Top() );
			nameColumnWidgetView->SetFrame( theNameColRect );
		}
	}
}
