//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpExportDialogObserver.cpp $
//	$Revision: 3976 $
//	$Date: 2012-03-06 12:56:58 +0100 (Tue, 06 Mar 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IControlView.h"
#include "ISubject.h"
#include "IPanelControlData.h"
#include "IStringListControlData.h"
#include "ITextControlData.h"
#include "ITriStateControlData.h"

//SDK General includes
#include "CDialogObserver.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpExportDlgHelper.h"
#include "IZExpServerData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZExpExportDialogObserver : public inheritClass
{
public:
						CZExpExportDialogObserver(IPMUnknown * boss);
	virtual				~CZExpExportDialogObserver();

	virtual void		AutoAttach();

	virtual void		AutoDetach();

	virtual void		DoAttachDetach(
							const WidgetID& widgetID, const PMIID& pmiid, bool16 attaching);

	virtual void		Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID&				protocol,
							void*						changedBy );

protected:
	PMString			GetSelectedTitle() const;
	PMString			GetSelectedEdition() const;
	
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;
	
	void				RefreshEditionPopup();
	void				TitleSelectionChange();
	void				EditionSelectionChange();

	void				ShowHideAdvanceControls();

private:
	void				Initialize();
};


CREATE_PMINTERFACE(CZExpExportDialogObserver, kZExpExportDialogObserverImpl)

CZExpExportDialogObserver::CZExpExportDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{
	this->Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpExportDialogObserver::~CZExpExportDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::AutoAttach()
{
	// Call the base class AutoAttach() function so that default behavior
	// will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoAttach();

	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData)
			break;

		this->DoAttachDetach(kZExpExportTitleDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach(kZExpExportEditionDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);

		//Attach Disclosure button
		this->AttachToWidget(kZExpExportDlgAdvOptionExpanderWidgetID, ITriStateControlData::kDefaultIID, panelControlData);
		
		ShowHideAdvanceControls();
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::AutoDetach()
{
	// Call base class AutoDetach() so that default behavior will occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoDetach();
	do
	{
		this->DoAttachDetach(kZExpExportTitleDropDownWidgetID, IStringListControlData::kDefaultIID, kFalse);
		this->DoAttachDetach(kZExpExportEditionDropDownWidgetID, IStringListControlData::kDefaultIID, kFalse);

		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData)
			break;
		
		//Detach Disclosure button
		this->DetachFromWidget(kZExpExportDlgAdvOptionExpanderWidgetID, ITriStateControlData::kDefaultIID, panelControlData);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::DoAttachDetach(
	const WidgetID& widgetID, const PMIID& pmiid, bool16 attaching)
{
	do
	{
		InterfacePtr<const IPanelControlData> panelControlData(this, UseDefaultIID());
		ASSERT(panelControlData);
		if (!panelControlData)
		{
			break;
		}
		IControlView* view = panelControlData->FindWidget(widgetID);
		ASSERT(view);
		if (!view)
		{
			break;
		}
		InterfacePtr<ISubject> subject(view, UseDefaultIID());
		ASSERT(subject);
		if (!subject)
		{
			break;
		}
		
		if (attaching)
		{
			if (!subject->IsAttached(this, pmiid))
			{
				subject->AttachObserver(this, pmiid);
			}
		}
		else
		{
			if (subject->IsAttached(this, pmiid))
			{
				subject->DetachObserver(this, pmiid);
			}
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID&				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	// Call the base class Update function so that default behavior will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);
	do
	{
		InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
		ASSERT(controlView);

		WidgetID theChangedWidgetID = controlView->GetWidgetID();
		if( theChange == kPopupChangeStateMessage && theChangedWidgetID.Get() == kZExpExportTitleDropDownWidgetID )
		{
			this->TitleSelectionChange();
		}
		else if ( theChange == kPopupChangeStateMessage &&  theChangedWidgetID.Get() == kZExpExportEditionDropDownWidgetID )
		{
			this->EditionSelectionChange();
		}
		else if(theChange == kZPUIDisclosureBtnStateChangedMsg && protocol == ITriStateControlData::kDefaultIID)
		{
			this->ShowHideAdvanceControls();
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// GetSelectedTitle
//----------------------------------------------------------------------------------------
PMString
CZExpExportDialogObserver::GetSelectedTitle()const
{
	InterfacePtr<ITextControlData> iTextControlData(this->GetWidgetView(kZExpExportTitleDropDownWidgetID), UseDefaultIID());
	if (iTextControlData != nil)
	{
		return iTextControlData->GetString();
	}

	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetSelectedEdition
//----------------------------------------------------------------------------------------
PMString
CZExpExportDialogObserver::GetSelectedEdition()const
{
	InterfacePtr<ITextControlData> iTextControlData(this->GetWidgetView(kZExpExportEditionDropDownWidgetID), UseDefaultIID());
	if (iTextControlData != nil)
	{
		return iTextControlData->GetString();
	}

	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZExpExportDialogObserver::GetWidgetView(
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

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitle = this->GetSelectedTitle();
	if( theTitle.empty() == kFalse )
	{
		InterfacePtr<const IZExpExportDlgHelper> exportDlgHelper(this, UseDefaultIID());
		exportDlgHelper->FillEditions();
	}
}

//----------------------------------------------------------------------------------------
// TitleSelectionChange
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::TitleSelectionChange()
{
	LogFunctionEnterExit;

	const PMString theTitle = this->GetSelectedTitle();
	if( theTitle.empty() == kFalse )
	{
		this->RefreshEditionPopup();
	}
}

//----------------------------------------------------------------------------------------
// EditionSelectionChange
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::EditionSelectionChange()
{
	LogFunctionEnterExit;

	const PMString theEdition = this->GetSelectedEdition();
	if( theEdition.empty() == kFalse )
	{
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Initialize
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::Initialize()
{
	LogFunctionEnterExit;
	
	do
	{
		char version[20] = {0};
		sprintf(version, "Ver. %d.%02d.%02d", kIZExpMajorVersion, kIZExpMinorVersion, kIZExpBuildNumber);
		PMString strVersion(version);
		strVersion.SetTranslatable(kFalse);

		InterfacePtr<ITextControlData> iTextControlData(this->GetWidgetView(kZExpExportVersionWidgetID), UseDefaultIID());
		if (iTextControlData != nil)
			iTextControlData->SetString(strVersion);

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// ShowHideAdvanceControls
//----------------------------------------------------------------------------------------
void
CZExpExportDialogObserver::ShowHideAdvanceControls()
{
#ifdef ExportWithAdvanceOptions
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		if(!iPanelControlData)
			break;

		InterfacePtr<IControlView> iAdvOptionGroupView(iPanelControlData->FindWidget(kZExpExportDlgAdvOptionGroupWidgetID), UseDefaultIID());
		if(!iAdvOptionGroupView)
			break;

		InterfacePtr<IBoolData> isExpandedBoolData(iPanelControlData->FindWidget(kZExpExportDlgAdvOptionExpanderWidgetID), IID_IZEXPISEXPANDED);
		if(!isExpandedBoolData)
			break;

		InterfacePtr<IControlView> iSperatorView(iPanelControlData->FindWidget(kZExpExportDlgAdvOptionSperatorWidgetID), UseDefaultIID());
		if(!iSperatorView)
			break;

		bool flagAdvOptionsVisible = false;
		if ( isExpandedBoolData->Get() )
		{
			iAdvOptionGroupView->Show();
			iSperatorView->Hide();
			flagAdvOptionsVisible = true;
		}
		else
		{
			iAdvOptionGroupView->Hide();
			iSperatorView->Show();
			flagAdvOptionsVisible = false;
		}

		//Manage height of dialog and others
		InterfacePtr<IControlView> selfControlView(iPanelControlData->FindWidget(kZExpExportDialogWidgetID), UseDefaultIID());
		if(!selfControlView)
			break;

		InterfacePtr<IControlView> okButtonView(iPanelControlData->FindWidget(kOKButtonWidgetID), UseDefaultIID());
		if(!okButtonView)
			break;

		PMRect groupFrame = iAdvOptionGroupView->GetFrame();
		PMRect loginDialogFrame = selfControlView->GetFrame();
		PMRect okButtonFrame = okButtonView->GetFrame();

		if (flagAdvOptionsVisible)
			okButtonFrame.Top(groupFrame.Bottom() + 10);
		else
			okButtonFrame.Top(groupFrame.Top() + 20);

		okButtonFrame.Bottom(okButtonFrame.Top() + 20);

		loginDialogFrame.SetHeight(okButtonFrame.Bottom() + 20);
		selfControlView->Resize(loginDialogFrame.Dimensions() );
		selfControlView->SetFrame(loginDialogFrame);

	}while(kFalse);
#endif
}
