//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/CZPTreeViewMgr.cpp $
//	$Revision: 2295 $
//	$Date: 2010-10-14 12:33:54 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssociatedWidgetAttributes.h"
#include "IPanelControlData.h"
#include "IWidgetParent.h"

//SDK General includes
#include "CTreeViewMgr.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVHeaderController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CTreeViewMgr
class CZPTreeViewMgr : public inheritClass
{
public:
						CZPTreeViewMgr(IPMUnknown * boss);
	virtual				~CZPTreeViewMgr();

	void				HandleScroll( int32 amount, bool16 up );

	void				HandleResize ();
protected:
	const IControlView * FindHeaderControlView()const;
private:
};


CREATE_PERSIST_PMINTERFACE(CZPTreeViewMgr, kZPTreeViewMgrImpl)

CZPTreeViewMgr::CZPTreeViewMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTreeViewMgr::~CZPTreeViewMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleScroll
//----------------------------------------------------------------------------------------
void
CZPTreeViewMgr::HandleScroll(
	int32 amount, bool16 up)
{
	inheritClass::HandleScroll( amount, up );
	const IControlView * headerControlView = this->FindHeaderControlView();
	if( headerControlView )
	{
		InterfacePtr<IZPTVHeaderController> headerController( headerControlView, UseDefaultIID() );
		ASSERT( headerController );
		headerController->HandleTreeViewScrolled( this );
	}
}

//----------------------------------------------------------------------------------------
// HandleResize
//----------------------------------------------------------------------------------------
void
CZPTreeViewMgr::HandleResize ()
{
	inheritClass::HandleResize();
	const IControlView * headerControlView = this->FindHeaderControlView();
	if( headerControlView )
	{
		InterfacePtr<IZPTVHeaderController> headerController( headerControlView, UseDefaultIID() );
		ASSERT( headerController );
		headerController->HandleTreeViewResized( this );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FindHeaderControlView
//----------------------------------------------------------------------------------------
const IControlView *
CZPTreeViewMgr::FindHeaderControlView()const
{
	InterfacePtr<const IWidgetParent> selfWParent(this, UseDefaultIID());
	ASSERT( selfWParent );
	
	const IPMUnknown * parentOfTreeView = selfWParent->GetParent();

	InterfacePtr<const IPanelControlData> tvParentPanelCtrlData( parentOfTreeView, UseDefaultIID() );
	ASSERT( tvParentPanelCtrlData );

	InterfacePtr<const IAssociatedWidgetAttributes> selfLinkedWidgetID( this, IID_ILINKEDWIDGETATTRIBUTES );
	ASSERT( selfLinkedWidgetID );

	ASSERT( selfLinkedWidgetID->GetAssociatedWidgetID() != kInvalidWidgetID );

	const IControlView * headerControlView = tvParentPanelCtrlData->FindWidget( selfLinkedWidgetID->GetAssociatedWidgetID() );
	return headerControlView;
}

