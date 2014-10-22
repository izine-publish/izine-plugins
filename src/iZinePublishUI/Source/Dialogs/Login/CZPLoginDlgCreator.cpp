//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Login/CZPLoginDlgCreator.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDialogMgr.h"
#include "IApplication.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "LocaleSetting.h"
#include "CoreResTypes.h"
#include "RsrcSpec.h"

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPLoginDlgCreator.h"
#include "IZPUserCredentials.h"
#include "IZPWSConfig.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPLoginDlgCreator>
class CZPLoginDlgCreator : public inheritClass
{
public:
				CZPLoginDlgCreator(IPMUnknown * boss);
	virtual		~CZPLoginDlgCreator();

	void			ShowLoginDialog(
						const IZPWSConfig *					inWSConfig,
						const IZPUserCredentials *			inUserCeredentials );
protected:
private:
};


CREATE_PMINTERFACE(CZPLoginDlgCreator, kZPLoginDlgCreatorImpl)

CZPLoginDlgCreator::CZPLoginDlgCreator(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLoginDlgCreator::~CZPLoginDlgCreator()
{

}

//----------------------------------------------------------------------------------------
// ShowLoginDialog
//----------------------------------------------------------------------------------------
void
CZPLoginDlgCreator::ShowLoginDialog(
	const IZPWSConfig *					inWSConfig,
	const IZPUserCredentials *			inUserCeredentials)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		// Get the application interface and the DialogMgr.
		InterfacePtr<IApplication> application(gSession->QueryApplication());
		ASSERT(application);
		if (application == nil) {
			break;
		}
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		ASSERT(dialogMgr);
		if (dialogMgr == nil) {
			break;
		}
			
		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,						// Locale index from PMLocaleIDs.h.
			kIZPUIPluginID,				// Our Plug-in ID
			kViewRsrcType,					// This is the kViewRsrcType.
			kZPUILoginDialogResourceID,	// Resource ID for our dialog.
			kTrue							// Initially visible.
		);
		
		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).
		IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		ASSERT(dialog);
		
		IControlView* dialogPanel = dialog->GetDialogPanel();
		ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
		
		//Copy Values
		InterfacePtr<IZPUserCredentials> iZPUserCredentials(dialogPanel, UseDefaultIID());
		ASSERT(iZPUserCredentials);
		
		InterfacePtr<IZPWSConfig> iZPWSConfig( dialogPanel, UseDefaultIID() );
		ASSERT(iZPWSConfig);

		iZPUserCredentials->Copy( inUserCeredentials );
		iZPWSConfig->Copy( inWSConfig );

		// Open the dialog.
		dialog->Open();
		dialog->WaitForDialog();

		//Dialog will close automatically on Cancel or successful login.
	}while(kFalse);
}

