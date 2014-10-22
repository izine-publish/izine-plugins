//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/CZMTreeViewMgr.cpp $
//	$Revision: 2728 $
//	$Date: 2011-04-07 09:43:28 +0200 (Thu, 07 Apr 2011) $
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
#include "IAssociatedWidgetAttributes.h"
#include "IPanelControlData.h"
#include "IWidgetParent.h"

//SDK General includes
#include "CTreeViewMgr.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMTVHeaderController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CTreeViewMgr
class CZMTreeViewMgr : public inheritClass
{
public:
						CZMTreeViewMgr(IPMUnknown * boss);
	virtual				~CZMTreeViewMgr();

	void				HandleScroll( int32 amount, bool16 up );

	void				HandleResize ();
protected:
	const IControlView * FindHeaderControlView()const;
private:
};


CREATE_PERSIST_PMINTERFACE(CZMTreeViewMgr, kZMTreeViewMgrImpl)

CZMTreeViewMgr::CZMTreeViewMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTreeViewMgr::~CZMTreeViewMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleScroll
//----------------------------------------------------------------------------------------
void
CZMTreeViewMgr::HandleScroll(
	int32 amount, bool16 up)
{
	inheritClass::HandleScroll( amount, up );
	const IControlView * headerControlView = this->FindHeaderControlView();
	if( headerControlView )
	{
		InterfacePtr<IZMTVHeaderController> headerController( headerControlView, UseDefaultIID() );
		ASSERT( headerController );
		headerController->HandleTreeViewScrolled( this );
	}
}

//----------------------------------------------------------------------------------------
// HandleResize
//----------------------------------------------------------------------------------------
void
CZMTreeViewMgr::HandleResize ()
{
	inheritClass::HandleResize();
	const IControlView * headerControlView = this->FindHeaderControlView();
	if( headerControlView )
	{
		InterfacePtr<IZMTVHeaderController> headerController( headerControlView, UseDefaultIID() );
		ASSERT( headerController );
		headerController->HandleTreeViewResized( this );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FindHeaderControlView
//----------------------------------------------------------------------------------------
const IControlView *
CZMTreeViewMgr::FindHeaderControlView()const
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

