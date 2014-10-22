//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAd/CZMAdsTVAdNodeObserver.cpp $
//	$Revision: 2759 $
//	$Date: 2011-04-08 14:22:51 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
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
#include "ISubject.h"
#include "ITriStateControlData.h"

//SDK General includes
#include "CWidgetObserver.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdInfo.h"
#include "IZMUITVNodeData.h"
#include "IZMUITVNodeWidgetMgr.h"

//IZP General includes
#include "ZMUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class ZMAdsTVAdNodeObserver : public inheritClass
{
public:
						ZMAdsTVAdNodeObserver(IPMUnknown * boss);
	virtual				~ZMAdsTVAdNodeObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);

	void				AttachToAd();
	void				DetachFromAd();

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


CREATE_PMINTERFACE(ZMAdsTVAdNodeObserver, kZMAdsTVAdNodeObserverImpl)

ZMAdsTVAdNodeObserver::ZMAdsTVAdNodeObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMAdsTVAdNodeObserver::~ZMAdsTVAdNodeObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	if( selfSubject->IsAttached( this, IZMUITVNodeData::kDefaultIID ) == false )	//Don't Detach it.
	{
		selfSubject->AttachObserver( this, IZMUITVNodeData::kDefaultIID );
		this->AttachToAd();
	}

	//Attach to any button or other child control of this node.
	//this->AttachToWidget( kZMUIAdsTVNodeFirstColumnWidgetID + eTVColAds_Save, ITriStateControlData::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::AutoDetach()
{
	//Detach to any button or other child control of this node.
	//this->DetachFromWidget( kZMUIAdsTVNodeFirstColumnWidgetID + eTVColAds_Save, ITriStateControlData::kDefaultIID )
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if( theChange == kZMUITVNodeColumn_BtnPressedMsg )
	{	//Note: we also do the same thing on kTrueStateMessage here.
		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		InterfacePtr<IControlView> senderControlView(theSubject, UseDefaultIID());
		ASSERT( senderControlView );
		if(! senderControlView )
			return;
				
		// Get the button ID from the view.
		WidgetID widgetID = senderControlView->GetWidgetID();
		int colID = widgetID - kZMUIAdsTVNodeFirstColumnWidgetID;
		//switch( colID )
		{
			//case eTVColAds_Delete:
			//	selfSubject->Change( kZMUIAdsTV_AdNodeDeleteBtnPressedMsg, IObserver::kDefaultIID, theSubject );
			//	break;
		}
	}
	else if( theChange == kZMUIAdsTVNodeDataWillChangeMsg )
		this->DetachFromAd();
	else if( theChange == kZMUIAdsTVNodeDataChangedMsg )
		this->AttachToAd();
	else if ( theChange == kZMAdInfoChangedMsg )
		this->RefreshSelfNodeWidget();
}

//----------------------------------------------------------------------------------------
// AttachToAd
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::AttachToAd()
{
	InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);

	InterfacePtr<ISubject> assetSubject( nodeData->GetDataObj(), UseDefaultIID());
	if( assetSubject )
	{
		if( assetSubject->IsAttached( this, IZMAdInfo::kDefaultIID ) )
			return;
#ifdef DEBUG
		InterfacePtr<const IControllingUnknown> controlling( assetSubject, UseDefaultIID() );
		IZPLog_Str_( thisFileLA, enLT_Details, "Ad Node : %08X attaching to asset: %08X", this, controlling.get() );
#endif
		assetSubject->AttachObserver( this, IZMAdInfo::kDefaultIID );
	}
}


//----------------------------------------------------------------------------------------
// DetachFromAd
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::DetachFromAd()
{
	InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	
	InterfacePtr<ISubject> assetSubject( nodeData->GetDataObj(), UseDefaultIID());
	if( assetSubject )
	{
#ifdef DEBUG
		InterfacePtr<const IControllingUnknown> controlling( assetSubject, UseDefaultIID() );
		IZPLog_Str_( thisFileLA, enLT_Details, "Ad Node : %08X detaching from asset: %08X", this, controlling.get() );
#endif
		assetSubject->DetachObserver(this, IZMAdInfo::kDefaultIID);
	}
}

//----------------------------------------------------------------------------------------
// AttachToWidget
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeObserver::AttachToWidget(
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
ZMAdsTVAdNodeObserver::DetachFromWidget(
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
ZMAdsTVAdNodeObserver::RefreshSelfNodeWidget()
{
	InterfacePtr<const IZMUITVNodeWidgetMgr> nodeWdgtMgr(this, UseDefaultIID());
	ASSERT(nodeWdgtMgr);
	nodeWdgtMgr->Refresh();
}
