//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/IZExp_ClassDefID.h $
//	$Revision: 4014 $
//	$Date: 2012-05-23 07:42:13 +0200 (Wed, 23 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZExp_ClassDefID_
#define _h_IZExp_ClassDefID_
#pragma once

#pragma mark -
//// ClassIDs:
	//General
DECLARE_PMID(kClassIDSpace, kZExpStartupShutdownBoss,														kIZExpPrefix + 1 )
DECLARE_PMID(kClassIDSpace, kZExpAppActionComponentBoss,													kIZExpPrefix + 2 )
	//Responders

	//Commands

	//Services
DECLARE_PMID(kClassIDSpace, kZExpXMLExportProviderBoss,														kIZExpPrefix + 40 )
DECLARE_PMID(kClassIDSpace, kZExpErrorStringServiceBoss,													kIZExpPrefix + 41 )

	//Scripting
//DECLARE_PMID(kClassIDSpace, kZPAssetDataScriptProviderBoss ,												kIZExpPrefix + 85 )

	//Dialogs
DECLARE_PMID(kClassIDSpace, kZExpExportDialogBoss ,															kIZExpUIPrefix + 41 )
	//Others
DECLARE_PMID(kClassIDSpace, kZExpSavePrefsCmdBoss ,															kIZExpPrefix + 42 )
DECLARE_PMID(kClassIDSpace, kZExpPrefsDialogBoss,															kIZExpUIPrefix + 43 )
	//Other common controls
DECLARE_PMID(kClassIDSpace, kZExpExpanderRollOverIconBtnWidgetBoss,											kIZExpUIPrefix + 50 )

#pragma mark -
// InterfaceIDs:
	//General
//DECLARE_PMID(kInterfaceIDSpace, IID_IZPAPP,																	kIZExpPrefix + 0 )

	//Persit interfaces' Don't change IDs of the following
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPPREFS,																kIZExpPrefix + 1 )
	//Utils
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPXMLWRITEFACADE,													kIZExpPrefix + 30 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPPAGEITEMUTILS,														kIZExpPrefix + 31 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPTEXTFRAMEUTILS,													kIZExpPrefix + 32 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPSTORYUTILS,														kIZExpPrefix + 33 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPDOCUTILS,															kIZExpPrefix + 34 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPFILEUTILS,															kIZExpPrefix + 35 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPINDNUTILS,															kIZExpPrefix + 36 )
	//Commands

	//Data Stored within document/story/assignment ; Persist/Non-Persist
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPGLOBALDATA,														kIZExpPrefix + 37 )

	//Others
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPWSFACADE,															kIZExpPrefix + 50 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPSERVERDATA,														kIZExpPrefix + 51 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPEXPORTDLGHELPER,													kIZExpPrefix + 52 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPDISCLOSURETRIANGLEOPTIONS,											kIZExpUIPrefix + 90 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZEXPISEXPANDED,														kIZExpUIPrefix + 91 )

//Debugging Intefaces.

#pragma mark -
// ImplementationIDs:
	//General
DECLARE_PMID(kImplementationIDSpace, kZExpStartupShutdownImpl,												kIZExpPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kZExpActionComponentImpl,												kIZExpPrefix + 2 )
	//Workspace persist data
DECLARE_PMID(kImplementationIDSpace, kZExpPrefsPersistImpl,													kIZExpPrefix + 3 )
	//Utils
DECLARE_PMID(kImplementationIDSpace, kZExpXMLWriteFacadeImpl,												kIZExpPrefix + 20 )
DECLARE_PMID(kImplementationIDSpace, kZExpPageItemUtilsImpl,												kIZExpPrefix + 21 )
DECLARE_PMID(kImplementationIDSpace, kZExpTextFrameUtilsImpl,												kIZExpPrefix + 22 )
DECLARE_PMID(kImplementationIDSpace, kZExpStoryUtilsImpl,													kIZExpPrefix + 23 )
DECLARE_PMID(kImplementationIDSpace, kZExpDocUtilsImpl,														kIZExpPrefix + 24 )
DECLARE_PMID(kImplementationIDSpace, kZExpFileUtilsImpl,													kIZExpPrefix + 25 )
DECLARE_PMID(kImplementationIDSpace, kZExpWSFacadeImpl,														kIZExpPrefix + 26 )
DECLARE_PMID(kImplementationIDSpace, kZExpInDnUtilsImpl,													kIZExpPrefix + 27 )
DECLARE_PMID(kImplementationIDSpace, kZExpGlobalDataImpl,													kIZExpPrefix + 28 )

	//Commands
DECLARE_PMID(kImplementationIDSpace, kZExpSavePrefsCmdImpl,													kIZExpPrefix + 40 )
	//Services
DECLARE_PMID(kImplementationIDSpace, kZExpDocXMLExportProviderImpl,											kIZExpPrefix + 50 )
DECLARE_PMID(kImplementationIDSpace, kZExpErrorStringServiceImpl,											kIZExpPrefix + 51 )

	//Responders

	//Debugging delegator impls

	//Export Dialog
DECLARE_PMID(kImplementationIDSpace, kZExpExportDialogControllerImpl,										kIZExpUIPrefix + 61 )
DECLARE_PMID(kImplementationIDSpace, kZExpExportDialogObserverImpl,											kIZExpUIPrefix + 62 )
DECLARE_PMID(kImplementationIDSpace, kZExpExportDlgHelperImpl,												kIZExpUIPrefix + 63 )
		//Expander button, disclosure TriAngle
DECLARE_PMID(kImplementationIDSpace, kZExpDisclosureTriAngleOptionsImpl,									kIZExpUIPrefix + 64 )
DECLARE_PMID(kImplementationIDSpace, kZExpDisclosureTriAngleObserverImpl,									kIZExpUIPrefix + 65 )

	//Prefs Dialog
DECLARE_PMID(kImplementationIDSpace, kZExpPrefsDialogControllerImpl,										kIZExpUIPrefix + 70 )
DECLARE_PMID(kImplementationIDSpace, kZExpPrefsPanelCreatorImpl,											kIZExpUIPrefix + 71 )
DECLARE_PMID(kImplementationIDSpace, kZExpPrefsDialogObserverImpl,											kIZExpUIPrefix + 72 )
	//Others
DECLARE_PMID(kImplementationIDSpace, kZExpPrefsImpl,														kIZExpPrefix + 81 )
DECLARE_PMID(kImplementationIDSpace, kZExpServerDataImpl,													kIZExpPrefix + 82 )
	//Scripting

	//Data Stored within document/story/assignment ; Persist/Non-Persist

	//Alias



#endif //_h_IZExp_ClassDefID_
