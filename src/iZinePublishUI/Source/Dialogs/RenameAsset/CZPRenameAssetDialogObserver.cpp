//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/RenameAsset/CZPRenameAssetDialogObserver.cpp $
//	$Revision: 3510 $
//	$Date: 2011-07-19 12:20:33 +0200 (Tue, 19 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 19-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "ITextControlData.h"

//SDK General includes
#include "CDialogObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZPRenameAssetDialogObserver : public inheritClass
{
public:
						CZPRenameAssetDialogObserver(IPMUnknown * boss);
	virtual				~CZPRenameAssetDialogObserver();

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


CREATE_PMINTERFACE(CZPRenameAssetDialogObserver, kZPRenameAssetDialogObserverImpl)

CZPRenameAssetDialogObserver::CZPRenameAssetDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPRenameAssetDialogObserver::~CZPRenameAssetDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPRenameAssetDialogObserver::AutoAttach()
{
	inheritClass::AutoAttach();
	do
	{
		this->DoAttachDetach(kZPUIRenameAssetDlgTxtNewNameWidgetID, ITextControlData::kDefaultIID, kTrue);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPRenameAssetDialogObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	do
	{
		this->DoAttachDetach(kZPUIRenameAssetDlgTxtNewNameWidgetID, ITextControlData::kDefaultIID, kFalse);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPRenameAssetDialogObserver::Update(
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
CZPRenameAssetDialogObserver::DoAttachDetach(
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
CZPRenameAssetDialogObserver::GetWidgetView(
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
