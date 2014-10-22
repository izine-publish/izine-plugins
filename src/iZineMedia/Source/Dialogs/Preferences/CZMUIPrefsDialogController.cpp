//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Dialogs/Preferences/CZMUIPrefsDialogController.cpp $
//	$Revision: 2910 $
//	$Date: 2011-04-29 16:02:51 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IWorkspace.h"

//SDK General includes
#include "CDialogController.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMPrefs.h"
#include "IZMCommandFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZMUIPrefsDialogController : public inheritClass
{
public:
						CZMUIPrefsDialogController(IPMUnknown * boss);
	virtual				~CZMUIPrefsDialogController();

	/** Initializes each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 @param dlgContext
	 */
	void				InitializeDialogFields(IActiveContext* dlgContext);
	
	/**	Retrieve the values from the widgets and act on them.
	 @param myContext
	 @param widgetId identifies the widget on which to act.
	 */
	void				ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
protected:
private:
};


CREATE_PMINTERFACE(CZMUIPrefsDialogController, kZMUIPrefsDialogControllerImpl)

CZMUIPrefsDialogController::CZMUIPrefsDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMUIPrefsDialogController::~CZMUIPrefsDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
	InterfacePtr<const IZMPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(zpPrefs);

	PMString thePath(zpPrefs->GetRepositoryPath());
	thePath.SetTranslatable(kFalse);
	this->SetTextControlData( kZMPrefsRepositoryPathEditBoxWidgetID, thePath );
	
	PMString theFilePrefix (zpPrefs->GetAdFileNamePrefix());
	theFilePrefix.SetTranslatable(kFalse);
	this->SetTextControlData( kZMPrefsAdFileNamePrefixEditBoxWidgetID, theFilePrefix);
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZMUIPrefsDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	InterfacePtr<ICommand> zpPrefsSaveCmd( Utils<IZMCommandFacade>()->CreateCommand_SaveZMPrefs( ) );
	if(!zpPrefsSaveCmd)
		return;

	InterfacePtr<IZMPrefs> zpPrefs( zpPrefsSaveCmd, UseDefaultIID() );
	ASSERT(zpPrefs);

	PMString thePath;	
	thePath.SetTranslatable(kFalse);
	thePath.Append(this->GetTextControlData( kZMPrefsRepositoryPathEditBoxWidgetID ));
	zpPrefs->SetRepositoryPath( thePath );

	PMString theAdFileNamePrefix;
	theAdFileNamePrefix.SetTranslatable(kFalse);
	theAdFileNamePrefix.Append(this->GetTextControlData( kZMPrefsAdFileNamePrefixEditBoxWidgetID ));
	zpPrefs->SetAdFileNamePrefix( theAdFileNamePrefix );
	
	CmdUtils::ProcessCommand( zpPrefsSaveCmd );
}
