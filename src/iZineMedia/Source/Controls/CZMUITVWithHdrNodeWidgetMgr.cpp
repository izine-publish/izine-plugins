//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/CZMUITVWithHdrNodeWidgetMgr.cpp $
//	$Revision: 2768 $
//	$Date: 2011-04-11 10:51:08 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPanelControlData.h"
#include "ITextControlData.h"

//SDK General includes
#include "LocaleSetting.h"
#include "CoreResTypes.h"
#include "RsrcSpec.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "CZMUITVWithHdrNodeWidgetMgr.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZMUITVWithHdrNodeWidgetMgr::CZMUITVWithHdrNodeWidgetMgr(
	IPMUnknown * boss)
	: inheritClass_TVWithHdrNodeWidgetMgr( boss )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMUITVWithHdrNodeWidgetMgr::~CZMUITVWithHdrNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetColumnText
//----------------------------------------------------------------------------------------
void
CZMUITVWithHdrNodeWidgetMgr::SetColumnText(
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
CZMUITVWithHdrNodeWidgetMgr::SetIcon(
	WidgetID					inWidgetid,
	const PluginID &			inIconPluginID,
	const RsrcID &				inIconID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData( this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	
	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	controlView->SetRsrcPluginID( inIconPluginID );
	controlView->SetRsrcID( inIconID );
}

//----------------------------------------------------------------------------------------
// ShowColumn
//----------------------------------------------------------------------------------------
void
CZMUITVWithHdrNodeWidgetMgr::ShowColumn(
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
CZMUITVWithHdrNodeWidgetMgr::HideColumn(
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
CZMUITVWithHdrNodeWidgetMgr::CreateColumnWidget(
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
									RsrcSpec( LocaleSetting::GetLocale(), kIZMediaPluginID, kViewRsrcType, kZMUITVColumnTxtWidgetRsrcID),
									IControlView::kDefaultIID ) ));
				break;
				case eTVColType_Icon:
					newWidgetView.reset(static_cast<IControlView*>( ::CreateObject( ::GetDataBase(this),
									RsrcSpec( LocaleSetting::GetLocale(), kIZMediaPluginID, kViewRsrcType, kZMUITVColumnIconWidgetRsrcID),
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
