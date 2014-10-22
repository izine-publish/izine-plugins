//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Dialogs/Preferences/CZMUIPrefsDialogObserver.cpp $
//	$Revision: 2699 $
//	$Date: 2011-04-06 08:45:59 +0200 (Wed, 06 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 1-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBooleanControlData.h"
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "ITextControlData.h"

//SDK General includes
#include "AbstractDialogObserver.h"
#include "CDialogController.h"
#include "SDKFileHelper.h"
#include "Widgetid.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AbstractDialogObserver
class CZMUIPrefsDialogObserver : public inheritClass
{
public:
						CZMUIPrefsDialogObserver(IPMUnknown * boss);
	virtual				~CZMUIPrefsDialogObserver();

	virtual void		Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);
	virtual void		AutoAttach();
	virtual void		AutoDetach();
protected:
private:
	void SelectPath();
};


CREATE_PMINTERFACE(CZMUIPrefsDialogObserver, kZMUIPrefsDialogObserverImpl)

CZMUIPrefsDialogObserver::CZMUIPrefsDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMUIPrefsDialogObserver::~CZMUIPrefsDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogObserver::AutoAttach()
{
	do
	{
		// Get the IPanelControlData interface for the dialog:
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("IPanelControlData::AutoAttach() panelControlData invalid");
			break;
		}

		AttachToWidget(kZMPrefsBrowseBtnWidgetID, IBooleanControlData::kDefaultIID, panelControlData);
	} while (false);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogObserver::AutoDetach()
{
	do
	{
		// Get the IPanelControlData interface for the dialog:
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("IPanelControlData::AutoDetach() panelControlData invalid");
			break;
		}

		DetachFromWidget(kZMPrefsBrowseBtnWidgetID, IBooleanControlData::kDefaultIID, panelControlData);
	} while (false);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject, 
	const PMIID &				protocol, 
	void*						changedBy)
{
	do
	{
		if (theChange == kTrueStateMessage)
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
					
			// Get the button ID from the view.
			WidgetID widgetID = controlView->GetWidgetID();
			switch( widgetID.Get() )
			{
				case kZMPrefsBrowseBtnWidgetID:
					this->SelectPath();
				break;
			}
		}
	} while (false);
}


//----------------------------------------------------------------------------------------
// SelectPath
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogObserver::SelectPath()
{
	SDKFolderChooser folderChooser;
	folderChooser.ShowDialog();
	if (folderChooser.IsChosen()) 
	{
		IDFile chosenFolder = folderChooser.GetIDFile();
		PMString path = folderChooser.GetPath();
		if (!path.IsNull() && !path.IsEmpty())
		{
			do
			{
				InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
				ASSERT(panelControlData);
				if(!panelControlData)
					break;

				IControlView* iWidgetView = panelControlData->FindWidget(kZMPrefsRepositoryPathEditBoxWidgetID);
				if(!iWidgetView)
					break;

				InterfacePtr<ITextControlData> textControlData(iWidgetView, UseDefaultIID());
				ASSERT(textControlData);
				if(!textControlData)
					break;

				path.SetTranslatable(false);
				textControlData->SetString(path);

			} while (false);
		}
	}
}

