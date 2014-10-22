//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVAssetNodeObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "ISubject.h"
#include "ITriStateControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CWidgetObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"
#include "IZPAssetLocalInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPUITVNodeData.h"
#include "IZPUITVNodeWidgetMgr.h"

//IZP General includes
#include "ZPUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class ZPAssetsTVAssetNodeObserver : public inheritClass
{
public:
						ZPAssetsTVAssetNodeObserver(IPMUnknown * boss);
	virtual				~ZPAssetsTVAssetNodeObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);

	void				AttachToAsset();
	void				DetachFromAsset();

	void				RefreshSelfNodeWidget();

protected:
	void				AttachToWidget(
							const WidgetID &			inWidgetID,
							const PMIID &				inInterestedIn );
	void				DetachFromWidget(
							const WidgetID &			inWidgetID,
							const PMIID &				inInterestedIn );
private:
};


CREATE_PMINTERFACE(ZPAssetsTVAssetNodeObserver, kZPAssetsTVAssetNodeObserverImpl)

ZPAssetsTVAssetNodeObserver::ZPAssetsTVAssetNodeObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsTVAssetNodeObserver::~ZPAssetsTVAssetNodeObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	if( selfSubject->IsAttached( this, IZPUITVNodeData::kDefaultIID ) == false )	//Don't Detach it.
	{
		selfSubject->AttachObserver( this, IZPUITVNodeData::kDefaultIID );
		this->AttachToAsset();
	}

	this->AttachToWidget( kZPUIAssetsTVNodeFirstColumnWidgetID + eTVColAssets_Save, ITriStateControlData::kDefaultIID );
	this->AttachToWidget( kZPUIAssetsTVNodeFirstColumnWidgetID + eTVColAssets_Delete, ITriStateControlData::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::AutoDetach()
{
	this->DetachFromAsset();
	this->DetachFromWidget( kZPUIAssetsTVNodeFirstColumnWidgetID + eTVColAssets_Save, ITriStateControlData::kDefaultIID );
	this->DetachFromWidget( kZPUIAssetsTVNodeFirstColumnWidgetID + eTVColAssets_Delete, ITriStateControlData::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if( theChange == kZPUITVNodeColumn_BtnPressedMsg )
	{	//Note: we also do the same thing on kTrueStateMessage here.
		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		InterfacePtr<IControlView> senderControlView(theSubject, UseDefaultIID());
		ASSERT( senderControlView );
		if(! senderControlView )
			return;
				
		// Get the button ID from the view.
		WidgetID widgetID = senderControlView->GetWidgetID();
		int colID = widgetID - kZPUIAssetsTVNodeFirstColumnWidgetID;
		switch( colID )
		{
			case eTVColAssets_Save:
				selfSubject->Change( kZPUIAssetTV_AssetNodeSaveBtnPressedMsg, IObserver::kDefaultIID, theSubject );
				break;
			case eTVColAssets_Delete:
				selfSubject->Change( kZPUIAssetTV_AssetNodeDeleteBtnPressedMsg, IObserver::kDefaultIID, theSubject );
				break;
		}
	}
	else if( theChange == kZPUIAssetTVNodeDataWillChangeMsg )
		this->DetachFromAsset();
	else if( theChange == kZPUIAssetTVNodeDataChangedMsg )
		this->AttachToAsset();
	else if ( theChange == kZPAssetInfoChangedMsg || theChange == kZPAssetLockInfoChangedMsg || theChange == kZPAssetLocalInfoChangedMsg || theChange == kZPDataNameChangedMsg)
		this->RefreshSelfNodeWidget();
}

//----------------------------------------------------------------------------------------
// AttachToAsset
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::AttachToAsset()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);

	InterfacePtr<ISubject> assetSubject( nodeData->GetDataObj(), UseDefaultIID());
	if( assetSubject )
	{
		if( !assetSubject->IsAttached( this, IZPAssetInfo::kDefaultIID ) )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( assetSubject, UseDefaultIID() );
			IZPLog_Str_( thisFileLA, enLT_Details, "Asset Node : %08X attaching to asset: %08X", this, controlling.get() );
#endif
			assetSubject->AttachObserver( this, IID_IZPDATANAME );
			assetSubject->AttachObserver( this, IZPAssetInfo::kDefaultIID );
			assetSubject->AttachObserver( this, IZPAssetLockInfo::kDefaultIID );
		}
	}

	InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );

	InterfacePtr<const IStringData> assetId(assetSubject , IID_IZPDATAID );
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( assetId->GetString()) );
	if(assetLocalInfo)
	{
		InterfacePtr<ISubject> assetLocalInfoSubject( assetLocalInfo, UseDefaultIID());
		if (assetLocalInfoSubject && !assetLocalInfoSubject->IsAttached( this, IZPAssetLocalInfo::kDefaultIID ) )
			assetLocalInfoSubject->AttachObserver(this, IZPAssetLocalInfo::kDefaultIID );
	}
}


//----------------------------------------------------------------------------------------
// DetachFromAsset
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::DetachFromAsset()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	
	InterfacePtr<ISubject> assetSubject( nodeData->GetDataObj(), UseDefaultIID());
	if( assetSubject )
	{
		if(assetSubject->IsAttached( this, IZPAssetInfo::kDefaultIID ))
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( assetSubject, UseDefaultIID() );
			IZPLog_Str_( thisFileLA, enLT_Details, "Asset Node : %08X detaching from asset: %08X", this, controlling.get() );
#endif
			assetSubject->DetachObserver(this, IID_IZPDATANAME);
			assetSubject->DetachObserver(this, IZPAssetInfo::kDefaultIID);
			assetSubject->DetachObserver(this, IZPAssetLockInfo::kDefaultIID);
		}
	}

	InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );

	InterfacePtr<const IStringData> assetId(assetSubject , IID_IZPDATAID );
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( assetId->GetString()) );
	if(assetLocalInfo)
	{
		InterfacePtr<ISubject> assetLocalInfoSubject( assetLocalInfo, UseDefaultIID());
		if (assetLocalInfoSubject && assetLocalInfoSubject->IsAttached( this, IZPAssetLocalInfo::kDefaultIID ) )
			assetLocalInfoSubject->DetachObserver(this, IZPAssetLocalInfo::kDefaultIID );
	}
}

//----------------------------------------------------------------------------------------
// AttachToWidget
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::AttachToWidget(
	const WidgetID &			inWidgetID,
	const PMIID &				inInterestedIn)
{
	InterfacePtr<const IPanelControlData> selfPanelData(this, UseDefaultIID());
	ASSERT( selfPanelData != nil );

	IControlView * widgetView = selfPanelData->FindWidget( inWidgetID );
	if( !widgetView )
		return;
	InterfacePtr<ISubject> widgetSubject(widgetView, UseDefaultIID());
	ASSERT( widgetSubject );

	if( !widgetSubject->IsAttached( this, inInterestedIn ) )
		widgetSubject->AttachObserver( this, inInterestedIn );
}

//----------------------------------------------------------------------------------------
// DetachFromWidget
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::DetachFromWidget(
	const WidgetID &			inWidgetID,
	const PMIID &				inInterestedIn)
{
	InterfacePtr<const IPanelControlData> selfPanelData(this, UseDefaultIID());
	ASSERT( selfPanelData != nil );

	IControlView * widgetView = selfPanelData->FindWidget( inWidgetID );
	if( !widgetView )
		return;
	InterfacePtr<ISubject> widgetSubject(widgetView, UseDefaultIID());
	ASSERT( widgetSubject );

	if( widgetSubject->IsAttached( this, inInterestedIn ) )
		widgetSubject->DetachObserver( this, inInterestedIn );
}

//----------------------------------------------------------------------------------------
// RefreshSelfNodeWidget
//----------------------------------------------------------------------------------------
void
ZPAssetsTVAssetNodeObserver::RefreshSelfNodeWidget()
{
	InterfacePtr<const IZPUITVNodeWidgetMgr> nodeWdgtMgr(this, UseDefaultIID());
	ASSERT(nodeWdgtMgr);
	nodeWdgtMgr->Refresh();
}
