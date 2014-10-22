//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMedia_ClassDefID.h $
//	$Revision: 2901 $
//	$Date: 2011-04-29 14:32:58 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZMedia_ClassDefID_
#define _h_IZMedia_ClassDefID_
#pragma once

#pragma mark -
//// ClassIDs:
	//General
DECLARE_PMID(kClassIDSpace, kZMStartupShutdownBoss,															kIZMediaPrefix + 1 )
	//Responders

	//Commands
DECLARE_PMID(kClassIDSpace, kZMSavePrefsCmdBoss,															kIZMediaPrefix + 10 )

	//Services
DECLARE_PMID(kClassIDSpace, kZMSAXAdContentHandlerBoss,														kIZMediaPrefix + 30 )
DECLARE_PMID(kClassIDSpace, kZMErrorStringServiceBoss,														kIZMediaPrefix + 31 )
DECLARE_PMID(kClassIDSpace, kZMConversionProviderBoss,														kIZMediaPrefix + 32 )

	//Data Model
DECLARE_PMID(kClassIDSpace, kZMDataMgrBoss,																	kIZMediaPrefix + 41 )
DECLARE_PMID(kClassIDSpace, kZMDataObjectBoss,																kIZMediaPrefix + 43 )
DECLARE_PMID(kClassIDSpace, kZMDataTitleBoss,																kIZMediaPrefix + 44 )
DECLARE_PMID(kClassIDSpace, kZMDataTitleEditionBoss,														kIZMediaPrefix + 45 )
DECLARE_PMID(kClassIDSpace, kZMAdFileContentBoss,															kIZMediaPrefix + 46 )
DECLARE_PMID(kClassIDSpace, kZMDataAdBoss,																	kIZMediaPrefix + 47 )

	//Scripting
//DECLARE_PMID(kClassIDSpace, kZPAssetDataScriptProviderBoss ,												kIZMediaPrefix + 85 )

	//Others

#pragma mark -
// InterfaceIDs:
	//General
DECLARE_PMID(kInterfaceIDSpace, IID_IZMAPP,																	kIZMediaPrefix + 0 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMMODELCONTROLLER,														kIZMediaPrefix + 1 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMAPPOBSERVER,															kIZMediaPrefix + 2 )

	//Persit interfaces' Don't change IDs of the following after client release
DECLARE_PMID(kInterfaceIDSpace, IID_IZMPREFS,																kIZMediaPrefix + 30 )

	//Utils
DECLARE_PMID(kInterfaceIDSpace, IID_IZMFILEUTILS,															kIZMediaPrefix + 40 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMCOMMANDFACADE,														kIZMediaPrefix + 41 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADFILEFACADE,														kIZMediaPrefix + 42 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMPAGEITEMUTILS,														kIZMediaPrefix + 43 )


	//Data Model Mgr
DECLARE_PMID(kInterfaceIDSpace, IID_IZMTITLEMGR,															kIZMediaPrefix + 50 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMEDITIONMGR,															kIZMediaPrefix + 51 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADFILEMGR,															kIZMediaPrefix + 52 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADMGR,																kIZMediaPrefix + 53 )
	//Data Model
DECLARE_PMID(kInterfaceIDSpace, IID_IZMDATAID,																kIZMediaPrefix + 60 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMDATANAME,															kIZMediaPrefix + 61 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMDATAOBJCONTROLLER,													kIZMediaPrefix + 62 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMCACHEMGRCOUNT,														kIZMediaPrefix + 63 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMEDITIONLIST,															kIZMediaPrefix + 64 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADFILELIST,															kIZMediaPrefix + 65 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADLIST,																kIZMediaPrefix + 66 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMPUBLICATIONINFO,														kIZMediaPrefix + 67 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMFILEINFO,															kIZMediaPrefix + 68 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADINFO,																kIZMediaPrefix + 69 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADFILEID,															kIZMediaPrefix + 70 )

	//Commands

	//Data Stored within document/story/assignment ; Persist/Non-Persist

	//Others
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADFILEDATA,															kIZMediaPrefix + 80 )

	//Debugging Intefaces.

#pragma mark -
// ImplementationIDs:
	//General
DECLARE_PMID(kImplementationIDSpace, kZMAppImpl,															kIZMediaPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kZMStartupShutdownImpl,												kIZMediaPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kZMModelControllerImpl,												kIZMediaPrefix + 2 )
DECLARE_PMID(kImplementationIDSpace, kZMAppObserverImpl,													kIZMediaPrefix + 3 )

DECLARE_PMID(kImplementationIDSpace, kZMPrefsImpl,															kIZMediaPrefix + 15 )
	//Workspace persist data
DECLARE_PMID(kImplementationIDSpace, kZMPrefsPersistImpl,													kIZMediaPrefix + 21 )
	//Utils
DECLARE_PMID(kImplementationIDSpace, kZMFileUtilsImpl,														kIZMediaPrefix + 30 )
DECLARE_PMID(kImplementationIDSpace, kZMCommandFacadeImpl,													kIZMediaPrefix + 31 )
DECLARE_PMID(kImplementationIDSpace, kZMAdFileFacadeImpl,													kIZMediaPrefix + 32 )
DECLARE_PMID(kImplementationIDSpace, kZMPageItemUtilsImpl,													kIZMediaPrefix + 33 )
	//Data Model Mgr
DECLARE_PMID(kImplementationIDSpace, kZMTitleMgrImpl,														kIZMediaPrefix + 40 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionMgrImpl,														kIZMediaPrefix + 41 )
DECLARE_PMID(kImplementationIDSpace, kZMAdFileMgrImpl,														kIZMediaPrefix + 42 )
DECLARE_PMID(kImplementationIDSpace, kZMAdMgrImpl,															kIZMediaPrefix + 43 )
	//Data Model
DECLARE_PMID(kImplementationIDSpace, kZMDataObjControllerImpl,												kIZMediaPrefix + 50 )
		//Title
DECLARE_PMID(kImplementationIDSpace, kZMEditionListImpl,													kIZMediaPrefix + 51 )
DECLARE_PMID(kImplementationIDSpace, kZMTitleObjControllerImpl,												kIZMediaPrefix + 52 )
		//Edition
DECLARE_PMID(kImplementationIDSpace, kZMAdFileListImpl,														kIZMediaPrefix + 53 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionObjControllerImpl,											kIZMediaPrefix + 54 )
		//Ad File
DECLARE_PMID(kImplementationIDSpace, kZMAdListImpl,															kIZMediaPrefix + 55 )
DECLARE_PMID(kImplementationIDSpace, kZMAdFileObjControllerImpl,											kIZMediaPrefix + 56 )
DECLARE_PMID(kImplementationIDSpace, kZMPublicationInfoImpl,												kIZMediaPrefix + 57 )
DECLARE_PMID(kImplementationIDSpace, kZMFileInfoImpl,														kIZMediaPrefix + 58 )
		//Ad
DECLARE_PMID(kImplementationIDSpace, kZMAdInfoImpl,															kIZMediaPrefix + 59 )
DECLARE_PMID(kImplementationIDSpace, kZMAdDataImpl,															kIZMediaPrefix + 60 )
DECLARE_PMID(kImplementationIDSpace, kZMAdFileDataImpl,														kIZMediaPrefix + 61 )
	//Commands
DECLARE_PMID(kImplementationIDSpace, kZMSavePrefsCmdImpl,													kIZMediaPrefix + 70 )

	//Services
DECLARE_PMID(kImplementationIDSpace, kZMSAXAdContentHandlerImpl,											kIZMediaPrefix + 90 )
DECLARE_PMID(kImplementationIDSpace, kZMErrorStringServiceImpl,												kIZMediaPrefix + 91 )

	//Responders

	//Debugging delegator impls

	//Others

	//Scripting

	//Data Stored within document/story/assignment ; Persist/Non-Persist

	//Alias
DECLARE_PMID(kImplementationIDSpace, kZMDataIDStringDataImpl,												kIZMediaPrefix + 200 )
DECLARE_PMID(kImplementationIDSpace, kZMDataID1StringDataImpl,												kIZMediaPrefix + 201 )
DECLARE_PMID(kImplementationIDSpace, kZMDataNameStringDataImpl,												kIZMediaPrefix + 202 )

#endif //_h_IZMedia_ClassDefID_
