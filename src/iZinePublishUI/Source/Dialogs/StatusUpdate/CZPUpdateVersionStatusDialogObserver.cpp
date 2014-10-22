//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/StatusUpdate/CZPUpdateVersionStatusDialogObserver.cpp $
//	$Revision: 3278 $
//	$Date: 2011-06-17 10:17:36 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 16-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IDialog.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "IWidgetParent.h"

//SDK General includes
#include "CDialogObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZPUpdateVersionStatusDialogObserver : public inheritClass
{
public:
						CZPUpdateVersionStatusDialogObserver(IPMUnknown * boss);
	virtual				~CZPUpdateVersionStatusDialogObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				DoAttachDetach(
							const WidgetID&				widgetID,
							const PMIID&				pmiid,
							bool16						attaching);

	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;

private:
};


CREATE_PMINTERFACE(CZPUpdateVersionStatusDialogObserver, kZPUpdateVersionStatusDialogObserverImpl)

CZPUpdateVersionStatusDialogObserver::CZPUpdateVersionStatusDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateVersionStatusDialogObserver::~CZPUpdateVersionStatusDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogObserver::AutoAttach()
{
	inheritClass::AutoAttach();
	do
	{
		this->DoAttachDetach(kZPUIStatusUpdateDlgPopStatusWidgetID, IStringListControlData::kDefaultIID, kTrue);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	do
	{
		this->DoAttachDetach(kZPUIStatusUpdateDlgPopStatusWidgetID, IStringListControlData::kDefaultIID, kFalse);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		inheritClass::Update(theChange, theSubject, protocol, changedBy);
	}while(false);
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogObserver::DoAttachDetach(
	const WidgetID&				widgetID,
	const PMIID&				pmiid,
	bool16						attaching)
{
	do
	{
		InterfacePtr<const IPanelControlData> panelControlData(this, UseDefaultIID());
		ASSERT(panelControlData);
		if (!panelControlData)
			break;

		IControlView* view = panelControlData->FindWidget(widgetID);
		ASSERT(view);
		if (!view)
			break;

		InterfacePtr<ISubject> subject(view, UseDefaultIID());
		ASSERT(subject);
		if (!subject)
			break;
		
		if (attaching)
		{
			if (!subject->IsAttached(this, pmiid))
				subject->AttachObserver(this, pmiid);
		}
		else
		{
			if (subject->IsAttached(this, pmiid))
				subject->DetachObserver(this, pmiid);
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPUpdateVersionStatusDialogObserver::GetWidgetView(
	const WidgetID &			inWidgetID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( inWidgetID );
		ASSERT(widgetView);
		return widgetView;
	}
	return NULL;
}

