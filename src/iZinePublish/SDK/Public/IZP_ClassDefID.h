//========================================================================================
//  
//  $HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/IZP_ClassDefID.h $
//  
//  Owner: Rahul Rastogi
//  
//  $Author: mindfire $
//  
//  $Date: 2012-10-12 08:52:24 +0200 (Fri, 12 Oct 2012) $
//  
//  $Revision: 4080 $
//..
//	$Copyright: 2008-2010 iZine Solutions. All rights reserved. $
//========================================================================================

#ifndef _h_IZP_ClassDefID_
#define _h_IZP_ClassDefID_
#pragma once

#pragma mark -
//// ClassIDs:
	//General
DECLARE_PMID(kClassIDSpace, kZPAppBoss,																		kIZPPrefix + 0 )
DECLARE_PMID(kClassIDSpace, kZPAppInitializerBoss,															kIZPPrefix + 1 )
DECLARE_PMID(kClassIDSpace, kZPStartupShutdownBoss,															kIZPPrefix + 2 )
DECLARE_PMID(kClassIDSpace, kZPConversionProviderBoss,														kIZPPrefix + 3 )
DECLARE_PMID(kClassIDSpace, kZPErrorStringServiceBoss,														kIZPPrefix + 4 )

DECLARE_PMID(kClassIDSpace, kZPUserSessionBoss,																kIZPPrefix + 20 )
DECLARE_PMID(kClassIDSpace, kZPAssetLocalInfoBoss,															kIZPPrefix + 21 )

	//Responders
DECLARE_PMID(kClassIDSpace, kZPLinkResponderBoss,															kIZPPrefix + 30 )
DECLARE_PMID(kClassIDSpace, kZPDocSignalResponderBoss,														kIZPPrefix + 31 )

	//TODO: renumber
DECLARE_PMID(kClassIDSpace, kZPLinkResourceBoss,															kIZPPrefix + 32 )
DECLARE_PMID(kClassIDSpace, kZPLinkResourceHandlerBoss,														kIZPPrefix + 33 )
DECLARE_PMID(kClassIDSpace, kZPLinkResourceProviderBoss,													kIZPPrefix + 34 )

	//WS Handling
DECLARE_PMID(kClassIDSpace, kZPSoapRequestBoss,																kIZPPrefix + 40 )
DECLARE_PMID(kClassIDSpace, kZPSyncSoapRequestBoss,															kIZPPrefix + 41 )
DECLARE_PMID(kClassIDSpace, kZPWSThreadBoss,																kIZPPrefix + 42 )

	//DB Cached object managers
DECLARE_PMID(kClassIDSpace, kZPDataMgrBoss,																	kIZPPrefix + 50 )
	//DB Cached Objects
DECLARE_PMID(kClassIDSpace, kZPDBObjectBoss,																kIZPPrefix + 55 )
DECLARE_PMID(kClassIDSpace, kZPDataTitleBoss,																kIZPPrefix + 56 )
DECLARE_PMID(kClassIDSpace, kZPDataAssetBoss,																kIZPPrefix + 57 )
DECLARE_PMID(kClassIDSpace, kZPDataTitleStatusBoss,															kIZPPrefix + 58 )
DECLARE_PMID(kClassIDSpace, kZPDataTitleEditionBoss,														kIZPPrefix + 59 )
DECLARE_PMID(kClassIDSpace, kZPDataTaskBoss,																kIZPPrefix + 60 )
DECLARE_PMID(kClassIDSpace, kZPDataTaskStatusBoss,															kIZPPrefix + 61 )
DECLARE_PMID(kClassIDSpace, kZPDataTaskCategoryBoss,														kIZPPrefix + 62 )
DECLARE_PMID(kClassIDSpace, kZPDataUserBoss,																kIZPPrefix + 63 )

	//Commands
DECLARE_PMID(kClassIDSpace, kZPSaveWSConfigCmdBoss,															kIZPPrefix + 70 )
DECLARE_PMID(kClassIDSpace, kZPSaveUserCredentialCmdBoss,													kIZPPrefix + 71 )
DECLARE_PMID(kClassIDSpace, kZPSaveAssetDataCmdBoss,														kIZPPrefix + 72 )
DECLARE_PMID(kClassIDSpace, kZPModifyPersistUIDListCmdBoss,													kIZPPrefix + 73 )
DECLARE_PMID(kClassIDSpace, kZPModifyAssetLocalInfoCmdBoss,													kIZPPrefix + 74 )
DECLARE_PMID(kClassIDSpace, kZPSavePrefsCmdBoss,															kIZPPrefix + 75 )

	//Scripting
DECLARE_PMID(kClassIDSpace, kZPAssetDataScriptProviderBoss ,												kIZPPrefix + 85 )

	//Asset Management
		//AM Providers
DECLARE_PMID(kClassIDSpace, kZPAMSPDocumentBoss,															kIZPPrefix + 87 )
DECLARE_PMID(kClassIDSpace, kZPAMSPAssignmentBoss,															kIZPPrefix + 88 )
DECLARE_PMID(kClassIDSpace, kZPAMSPLinkedStoryBoss,															kIZPPrefix + 89 )
DECLARE_PMID(kClassIDSpace, kZPAMSPICDocumentBoss,															kIZPPrefix + 90 )
		//AM Services - Common
DECLARE_PMID(kClassIDSpace, kZPAMServiceBoss,																kIZPPrefix + 91 )
DECLARE_PMID(kClassIDSpace, kZPAMManagedStatusBoss,															kIZPPrefix + 92 )
DECLARE_PMID(kClassIDSpace, kZPAMDocManagedStatusBoss,														kIZPPrefix + 93 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryManagedStatusBoss,												kIZPPrefix + 94 )
DECLARE_PMID(kClassIDSpace, kZPDocFileHandlerBoss,															kIZPPrefix + 95 )
		//AM Services - Document
DECLARE_PMID(kClassIDSpace, kZPAMDocumentInitializeServiceBoss,												kIZPPrefix + 96 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentCheckInServiceBoss,												kIZPPrefix + 97 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentCheckOutServiceBoss,												kIZPPrefix + 98 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentCancelCheckOutServiceBoss,											kIZPPrefix + 99 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentRevertServiceBoss,													kIZPPrefix + 100 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentEnsureLatestVersionServiceBoss,									kIZPPrefix + 101 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentAfterSaveServiceBoss,												kIZPPrefix + 102 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentUpdateStoredAssetRefServiceBoss,									kIZPPrefix + 103 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentUpdateProxyVersionNumServiceBoss,									kIZPPrefix + 104 )
DECLARE_PMID(kClassIDSpace, kZPAMDocumentTerminateServiceBoss,												kIZPPrefix + 105 )
		//AM Services - IC Document
DECLARE_PMID(kClassIDSpace, kZPAMICDocumentInitializeServiceBoss,											kIZPPrefix + 106 )
DECLARE_PMID(kClassIDSpace, kZPAMICDocumentCheckInServiceBoss,												kIZPPrefix + 107 )
DECLARE_PMID(kClassIDSpace, kZPAMICDocumentCheckOutServiceBoss,												kIZPPrefix + 108 )
DECLARE_PMID(kClassIDSpace, kZPAMICDocumentAfterSaveServiceBoss,											kIZPPrefix + 109 )
		//AM Services - Assignment
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentCheckInServiceBoss,												kIZPPrefix + 116 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentCheckOutServiceBoss,												kIZPPrefix + 117 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentCancelCheckOutServiceBoss,										kIZPPrefix + 118 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentRevertServiceBoss,												kIZPPrefix + 119 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentEnsureLatestVersionServiceBoss,									kIZPPrefix + 120 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentAfterSaveServiceBoss,											kIZPPrefix + 121 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentUpdateStoredAssetReferenceServiceBoss,							kIZPPrefix + 122 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentUpdateProxyVersionNumberServiceBoss,								kIZPPrefix + 123 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentInitializeServiceBoss,											kIZPPrefix + 124 )
DECLARE_PMID(kClassIDSpace, kZPAMAssignmentTerminateServiceBoss,											kIZPPrefix + 125 )
		//AM Services - ID Story
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryCheckInServiceBoss,												kIZPPrefix + 136 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryCheckOutServiceBoss,											kIZPPrefix + 137 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryCancelCheckOutServiceBoss,										kIZPPrefix + 138 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryRevertServiceBoss,												kIZPPrefix + 139 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryEnsureLatestVersionServiceBoss,									kIZPPrefix + 140 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryAfterSaveServiceBoss,											kIZPPrefix + 141 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryUpdateStoredAssetReferenceServiceBoss,							kIZPPrefix + 142 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryUpdateProxyVersionNumberServiceBoss,							kIZPPrefix + 143 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryInitializeServiceBoss,											kIZPPrefix + 144 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryTerminateServiceBoss,											kIZPPrefix + 145 )
DECLARE_PMID(kClassIDSpace, kZPAMLinkedStoryAfterCheckInServiceBoss,										kIZPPrefix + 146 )

	//Others

#pragma mark -
// InterfaceIDs:
	//General
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAPP,																	kIZPPrefix + 0 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAPPCONTROLLER,														kIZPPrefix + 1 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPCONFIGCONTROLLER,													kIZPPrefix + 2 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPLOGINCONTROLLER,														kIZPPrefix + 3 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETSLOCALINFOMGR,													kIZPPrefix + 4 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIDLISTDATA,															kIZPPrefix + 5 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPPREVIEWMGR,															kIZPPrefix + 6 )
	//Persit interfaces' Don't change IDs of the following
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETLOCALINFOUIDLIST,												kIZPPrefix + 11 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPPREFS,																kIZPPrefix + 12 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPLASTCLEARCACHEDATE,													kIZPPrefix + 13 )
	//Login/Session
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDEFAULTSESSION,														kIZPPrefix + 15 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPISVALID,																kIZPPrefix + 16 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSESSIONDATA,															kIZPPrefix + 17 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUSERCREDENTIALS,														kIZPPrefix + 18 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUSERROLE,															kIZPPrefix + 19 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPLOGINDLGCREATOR,														kIZPPrefix + 20 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSLOGINRESPONSEHANDLER,												kIZPPrefix + 21 )
	//WS Controller/Handling
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSOAPREQUEST,															kIZPPrefix + 30 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSOAPRESPONSE,														kIZPPrefix + 31 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPLASTREQUESTTIME,														kIZPPrefix + 32 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSCONFIG,															kIZPPrefix + 33 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSCALLHELPER,														kIZPPrefix + 34 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSCONTROLLER,														kIZPPrefix + 35 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSTHREADHANDLER,														kIZPPrefix + 36 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSRESPONSEHANDLER,													kIZPPrefix + 37 )
	//Utils
DECLARE_PMID(kInterfaceIDSpace, IID_IZPALERTUTILS,															kIZPPrefix + 50 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETDOCUTILS,														kIZPPrefix + 51 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETUTILS,															kIZPPrefix + 52 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPCOMPRESSIONUTILS,													kIZPPrefix + 53 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDOCUTILS,															kIZPPrefix + 54 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPFILEUTILS,															kIZPPrefix + 55 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPINDNUTILS,															kIZPPrefix + 56 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSTORYUTILS,															kIZPPrefix + 57 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPWSUTILS,																kIZPPrefix + 58 )
	//Commands
DECLARE_PMID(kInterfaceIDSpace, IID_IZPCOMMANDFACADE,														kIZPPrefix + 65 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSIGNMENTCMDINTERCEPTOR,											kIZPPrefix + 66 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTEMPLATECMDINTERCEPTOR,												kIZPPrefix + 67 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPMODIFYASSETLOCALINFOFLAGS,											kIZPPrefix + 68 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPRUNBYPUBLISH,														kIZPPrefix + 69 )
	//Observers
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDOCCMDOBSERVER,														kIZPPrefix + 85 )
//DB Object Managers
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETMGR,															kIZPPrefix + 90 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUSERMGR,																kIZPPrefix + 91 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPEDITIONMGR,															kIZPPrefix + 92 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTITLEMGR,															kIZPPrefix + 93 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTITLESTATUSMGR,														kIZPPrefix + 94 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKMGR,																kIZPPrefix + 95 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKSTATUSMGR,														kIZPPrefix + 96 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKCATEGORYMGR,														kIZPPrefix + 97 )
//DB Objects
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDATAID,																kIZPPrefix + 110 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDATANAME,															kIZPPrefix + 111 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPCACHEMGRCOUNT,														kIZPPrefix + 112 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDBOBJCONTROLLER,														kIZPPrefix + 113 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETLIST,															kIZPPrefix + 114 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPEDITIONLIST,															kIZPPrefix + 115 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSTATUSLIST,															kIZPPrefix + 116 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETINFO,															kIZPPrefix + 117 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETLOCKINFO,														kIZPPrefix + 118 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTITLESTATUSINFO,														kIZPPrefix + 119 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKINFO,															kIZPPrefix + 120 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUSERLIST,															kIZPPrefix + 121 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKLIST,															kIZPPrefix + 122 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETMONITOROBSERVECOUNTER,											kIZPPrefix + 123 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPPARENTASSETID,														kIZPPrefix + 124 )
	//Data Stored within document/story/assignment ; Persist/Non-Persist
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETDATA,															kIZPPrefix + 190 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETCHECKINDATA,													kIZPPrefix + 191 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETVERSIONINFODATA,												kIZPPrefix + 192 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPSTORYCHECKINMADE,													kIZPPrefix + 193 )
	//AM
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAMDEBUGHELPER,														kIZPPrefix + 210 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAMHELPER,															kIZPPrefix + 211 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAMWSUTILS,															kIZPPrefix + 212 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETLOCALINFO,														kIZPPrefix + 213 )
	//Others
DECLARE_PMID(kInterfaceIDSpace, IID_IZPISTEMPLATECOPY,														kIZPPrefix + 230 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPGLOBALTEMPDATA,														kIZPPrefix + 231 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETTYPE,															kIZPPrefix + 240 )
	//Debugging Intefaces.
DECLARE_PMID(kInterfaceIDSpace, IID_IZPStringDataDelegate,													kIZPPrefix + 241 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPMANAGEDSTATUS,														kIZPPrefix + 242 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPAMSERVICEDATADELEGATE,												kIZPPrefix + 243 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPICEXPORTSUITE,														kIZPPrefix + 244 )

#pragma mark -
// ImplementationIDs:
	//General
DECLARE_PMID(kImplementationIDSpace, kZPAppImpl,															kIZPPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kZPAppEarlyInitializerImpl,											kIZPPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kZPStartupShutdownImpl,												kIZPPrefix + 2 )
DECLARE_PMID(kImplementationIDSpace, kZPAppControllerImpl,													kIZPPrefix + 3 )
DECLARE_PMID(kImplementationIDSpace, kZPConfigControllerImpl,												kIZPPrefix + 4 )
DECLARE_PMID(kImplementationIDSpace, kZPLoginControllerImpl,												kIZPPrefix + 5 )
DECLARE_PMID(kImplementationIDSpace, kZPAppIdleTaskImpl,													kIZPPrefix + 6 )
DECLARE_PMID(kImplementationIDSpace, kZPAppObserverImpl,													kIZPPrefix + 7 )
	//Workspace persist data
DECLARE_PMID(kImplementationIDSpace, kZPAssetLocalInfoImpl,													kIZPPrefix + 8 )
DECLARE_PMID(kImplementationIDSpace, kZPUIDListPersistImpl,													kIZPPrefix + 9 )
DECLARE_PMID(kImplementationIDSpace, kZPPrefsPersistImpl,													kIZPPrefix + 10 )
DECLARE_PMID(kImplementationIDSpace, kZPDateTimeDataPersistImpl,											kIZPPrefix + 11 )
DECLARE_PMID(kImplementationIDSpace, kZPPersistGlobalTimeDataAliasImpl,										kIZPPrefix + 12 )

DECLARE_PMID(kImplementationIDSpace, kZPAssetsLocalInfoMgrImpl,												kIZPPrefix + 13 )
DECLARE_PMID(kImplementationIDSpace, kZPPreviewMgrImpl,														kIZPPrefix + 14 )

	//Login/Session
DECLARE_PMID(kImplementationIDSpace, kZPDefaultSessionImpl,													kIZPPrefix + 15 )
DECLARE_PMID(kImplementationIDSpace, kZPSessionDataImpl,													kIZPPrefix + 16 )
DECLARE_PMID(kImplementationIDSpace, kZPUserCredentialsImpl,												kIZPPrefix + 17 )
DECLARE_PMID(kImplementationIDSpace, kZPUserCredentialsPersistImpl,											kIZPPrefix + 18 )
DECLARE_PMID(kImplementationIDSpace, kZPUserRoleImpl,														kIZPPrefix + 19 )
DECLARE_PMID(kImplementationIDSpace, kZPWSLoginResponseHandlerImpl,											kIZPPrefix + 20 )
DECLARE_PMID(kImplementationIDSpace, kZPLoginUIServiceProviderImpl,											kIZPPrefix + 21 )

	//Selection suite
DECLARE_PMID(kImplementationIDSpace, kZPICExportSuiteASBImpl,												kIZPPrefix + 22 )
DECLARE_PMID(kImplementationIDSpace, kZPICExportSuiteLayoutImpl,											kIZPPrefix + 23 )


	//Services
DECLARE_PMID(kImplementationIDSpace, kZPErrorStringServiceImpl,												kIZPPrefix + 25 )
DECLARE_PMID(kImplementationIDSpace, kZPSchemaBasedConversionImpl,											kIZPPrefix + 26 )
	//TODO: renumber
//DECLARE_PMID(kImplementationIDSpace, kZPLinkResourceImpl,													kIZPPrefix + 27 )
//DECLARE_PMID(kImplementationIDSpace, kZPLinkResourceHandlerImpl,											kIZPPrefix + 28 )
//DECLARE_PMID(kImplementationIDSpace, kZPLinkResourceStateUpdaterImpl,										kIZPPrefix + 29 )
//DECLARE_PMID(kImplementationIDSpace, kZPLinkResourceFactoryImpl,											kIZPPrefix + 24 )

	//WS Controller/Handling
DECLARE_PMID(kImplementationIDSpace, kZPSoapRequestImpl,													kIZPPrefix + 30 )
DECLARE_PMID(kImplementationIDSpace, kZPSoapResponseImpl,													kIZPPrefix + 31 )
DECLARE_PMID(kImplementationIDSpace, kZPWSConfigImpl,														kIZPPrefix + 32 )
DECLARE_PMID(kImplementationIDSpace, kZPWSConfigPersistImpl,												kIZPPrefix + 33 )
DECLARE_PMID(kImplementationIDSpace, kZPWSCallHelperImpl,													kIZPPrefix + 34 )
DECLARE_PMID(kImplementationIDSpace, kZPWSControllerImpl,													kIZPPrefix + 35 )
DECLARE_PMID(kImplementationIDSpace, kZPWSThreadHandlerImpl,												kIZPPrefix + 36 )
DECLARE_PMID(kImplementationIDSpace, kZPWSIdleTaskThreadImpl,												kIZPPrefix + 37 )
DECLARE_PMID(kImplementationIDSpace, kZPWSResponseHandlerImpl,												kIZPPrefix + 38 )
DECLARE_PMID(kImplementationIDSpace, kZPWSResponseHolderImpl,												kIZPPrefix + 39 )
	//Utils
DECLARE_PMID(kImplementationIDSpace, kZPAlertUtilsImpl,														kIZPPrefix + 50 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetDocUtilsImpl,													kIZPPrefix + 51 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetUtilsImpl,														kIZPPrefix + 52 )
DECLARE_PMID(kImplementationIDSpace, kZPCompressionUtilsImpl,												kIZPPrefix + 53 )
DECLARE_PMID(kImplementationIDSpace, kZPDocUtilsImpl,														kIZPPrefix + 54 )
DECLARE_PMID(kImplementationIDSpace, kZPFileUtilsImpl,														kIZPPrefix + 55 )
DECLARE_PMID(kImplementationIDSpace, kZPInDnUtilsImpl,														kIZPPrefix + 56 )
DECLARE_PMID(kImplementationIDSpace, kZPStoryUtilsImpl,														kIZPPrefix + 57 )
DECLARE_PMID(kImplementationIDSpace, kZPWSUtilsImpl,														kIZPPrefix + 58 )
	//Commands
DECLARE_PMID(kImplementationIDSpace, kZPCommandFacadeImpl,													kIZPPrefix + 65 )
DECLARE_PMID(kImplementationIDSpace, kZPSaveAssetDataCmdImpl,												kIZPPrefix + 66 )
DECLARE_PMID(kImplementationIDSpace, kZPSaveUserCredentialsCmdImpl,											kIZPPrefix + 67 )
DECLARE_PMID(kImplementationIDSpace, kZPSaveWSConfigCmdImpl,												kIZPPrefix + 68 )
DECLARE_PMID(kImplementationIDSpace, kZPAssignmentCmdInterceptorImpl,										kIZPPrefix + 69 )
DECLARE_PMID(kImplementationIDSpace, kZPTemplateCmdInterceptorImpl,											kIZPPrefix + 70 )
DECLARE_PMID(kImplementationIDSpace, kZPModifyPersistUIDListCmdImpl,										kIZPPrefix + 71 )
DECLARE_PMID(kImplementationIDSpace, kZPModifyAssetLocalInfoCmdImpl,										kIZPPrefix + 72 )
DECLARE_PMID(kImplementationIDSpace, kZPSavePrefsCmdImpl,													kIZPPrefix + 73 )
	//Responders
DECLARE_PMID(kImplementationIDSpace, kZPLinkResponderServiceProviderImpl,									kIZPPrefix + 80 )
DECLARE_PMID(kImplementationIDSpace, kZPLinkResponderImpl,													kIZPPrefix + 81 )
DECLARE_PMID(kImplementationIDSpace, kZPDocResponderServiceProviderImpl,									kIZPPrefix + 82 )
DECLARE_PMID(kImplementationIDSpace, kZPDocSignalResponderImpl,												kIZPPrefix + 83 )
	//Observers
DECLARE_PMID(kImplementationIDSpace, kZPDocCmdObserverImpl,													kIZPPrefix + 87 )
	//DB Object Managers
DECLARE_PMID(kImplementationIDSpace, kZPAssetMgrImpl,														kIZPPrefix + 90 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetStatusMgrImpl,													kIZPPrefix + 91 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionMgrImpl,														kIZPPrefix + 92 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleMgrImpl,														kIZPPrefix + 93 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleStatusMgrImpl,													kIZPPrefix + 94 )
DECLARE_PMID(kImplementationIDSpace, kZPDataIdleTaskImpl,													kIZPPrefix + 95 )
DECLARE_PMID(kImplementationIDSpace, kZPDataObserverImpl,													kIZPPrefix + 96 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskMgrImpl,														kIZPPrefix + 97 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskStatusMgrImpl,													kIZPPrefix + 98 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskCategoryMgrImpl,												kIZPPrefix + 99 )
DECLARE_PMID(kImplementationIDSpace, kZPUserMgrImpl,														kIZPPrefix + 100 )

	//DB Objects
DECLARE_PMID(kImplementationIDSpace, kZPDBObjControllerImpl,												kIZPPrefix + 110 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetListImpl,														kIZPPrefix + 111 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionListImpl,													kIZPPrefix + 112 )
DECLARE_PMID(kImplementationIDSpace, kZPStatusListImpl,														kIZPPrefix + 113 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetInfoImpl,														kIZPPrefix + 114 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetLockInfoImpl,													kIZPPrefix + 115 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetObjControllerImpl,												kIZPPrefix + 116 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionObjControllerImpl,											kIZPPrefix + 117 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleStatusInfoImpl,												kIZPPrefix + 118 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskInfoImpl,														kIZPPrefix + 119 )
DECLARE_PMID(kImplementationIDSpace, kZPUserListImpl,														kIZPPrefix + 120 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskListImpl,														kIZPPrefix + 121 )

//130-140 used in AM

	//Debugging delegator impls
DECLARE_PMID(kImplementationIDSpace, kZPStringDataDelegateImpl,												kIZPPrefix + 140 )
DECLARE_PMID(kImplementationIDSpace, kZPAMManagedStatusDelegateImpl,										kIZPPrefix + 141 )
DECLARE_PMID(kImplementationIDSpace, kZPAMServiceDataDelegateImpl,											kIZPPrefix + 142 )

	//Others
DECLARE_PMID(kImplementationIDSpace, kZPStringListDataImpl,													kIZPPrefix + 150 )
DECLARE_PMID(kImplementationIDSpace, kZPPrefsImpl,															kIZPPrefix + 151 )
DECLARE_PMID(kImplementationIDSpace, kZPGlobalTempDataImpl,													kIZPPrefix + 152 )

	//Scripting
DECLARE_PMID(kImplementationIDSpace, kZPAssetDataScriptProviderImpl,										kIZPPrefix + 155 )

//180 - 185 used in AM InCopy Document

	//Data Stored within document/story/assignment ; Persist/Non-Persist
DECLARE_PMID(kImplementationIDSpace, kZPAssetDataImpl,														kIZPPrefix + 190 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetDataPersistImpl,												kIZPPrefix + 191 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetCheckInDataImpl,												kIZPPrefix + 192 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetVersionInfoDataImpl,											kIZPPrefix + 193 )
	//Alias
DECLARE_PMID(kImplementationIDSpace, kZPDataIDStringDataImpl,												kIZPPrefix + 200 )
DECLARE_PMID(kImplementationIDSpace, kZPDataNameStringDataImpl,												kIZPPrefix + 201 )
DECLARE_PMID(kImplementationIDSpace, kZPFalseInitBoolDataImpl,												kIZPPrefix + 202 )
DECLARE_PMID(kImplementationIDSpace, kZPIntDataAliasImpl,													kIZPPrefix + 203 )

	//Asset Management
DECLARE_PMID(kImplementationIDSpace, kZPAMDebugHelperImpl,													kIZPPrefix + 204 )
DECLARE_PMID(kImplementationIDSpace, kZPAMHelperImpl,														kIZPPrefix + 205 )
DECLARE_PMID(kImplementationIDSpace, kZPAMWSUtilsImpl,														kIZPPrefix + 206 )
DECLARE_PMID(kImplementationIDSpace, kZPAMManagedStatusImpl,												kIZPPrefix + 207 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocLockableImpl,													kIZPPrefix + 208 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLockableImpl,														kIZPPrefix + 209 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocUIEnablementRulesImpl,											kIZPPrefix + 210 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentUIEnablementRulesImpl,									kIZPPrefix + 211 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryUIEnablementRulesImpl,									kIZPPrefix + 212 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocAssetMonitorImpl,												kIZPPrefix + 213 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryAssetMonitorImpl,										kIZPPrefix + 214 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentAssetMonitorImpl,										kIZPPrefix + 215 )
DECLARE_PMID(kImplementationIDSpace, kZPDocFileHandlerImpl,													kIZPPrefix + 216 )

//130-140 used in AM
DECLARE_PMID(kImplementationIDSpace, kZPAMAssetObserverImpl,												kIZPPrefix + 130 )

		//AM Providers
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentSystemProviderImpl,										kIZPPrefix + 217 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentServiceProviderImpl,										kIZPPrefix + 218 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentSystemProviderImpl,										kIZPPrefix + 219 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentServiceProviderImpl,									kIZPPrefix + 220 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStorySystemProviderImpl,									kIZPPrefix + 221 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryServiceProviderImpl,									kIZPPrefix + 222 )
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentServiceProviderImpl,									kIZPPrefix + 223 )
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentSystemProviderImpl,										kIZPPrefix + 224 )
		//AM Document Services
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentInitializeServiceImpl,									kIZPPrefix + 225 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentCheckInServiceImpl,										kIZPPrefix + 226 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentCheckOutServiceImpl,										kIZPPrefix + 227 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentCancelCheckOutServiceImpl,								kIZPPrefix + 228 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentRevertServiceImpl,										kIZPPrefix + 229 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentEnsureLatestVersionServiceImpl,							kIZPPrefix + 230 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentAfterSaveServiceImpl,										kIZPPrefix + 231 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentUpdateStoredRefServiceImpl,								kIZPPrefix + 232 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentUpdateProxyVersionNumServiceImpl,							kIZPPrefix + 233 )
DECLARE_PMID(kImplementationIDSpace, kZPAMDocumentTerminateServiceImpl,										kIZPPrefix + 234 )
		//AM InCopy Document
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentInitializeServiceImpl,									kIZPPrefix + 180 )
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentCheckInServiceImpl,										kIZPPrefix + 181 )
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentCheckOutServiceImpl,									kIZPPrefix + 182 )
DECLARE_PMID(kImplementationIDSpace, kZPAMICDocumentAfterSaveServiceImpl,									kIZPPrefix + 183 )
		//AM Assignment services
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentCheckInServiceImpl,										kIZPPrefix + 235 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentCheckOutServiceImpl,									kIZPPrefix + 236 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentCancelCheckoutServiceImpl,								kIZPPrefix + 237 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentRevertServiceImpl,										kIZPPrefix + 238 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentEnsureLatestVersionServiceImpl,							kIZPPrefix + 239 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentAfterSaveServiceImpl,									kIZPPrefix + 240 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentUpdateStoredRefServiceImpl,								kIZPPrefix + 241 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentUpdateProxyVersionNumServiceImpl,						kIZPPrefix + 242 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentInitializeServiceImpl,									kIZPPrefix + 243 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAssignmentTerminateServiceImpl,									kIZPPrefix + 244 )
		//AM Story services
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryCheckInServiceImpl,									kIZPPrefix + 245 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryCheckOutServiceImpl,									kIZPPrefix + 246 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryCancelCheckoutServiceImpl,								kIZPPrefix + 247 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryRevertServiceImpl,										kIZPPrefix + 248 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryEnsureLatestVersionServiceImpl,						kIZPPrefix + 249 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryAfterSaveServiceImpl,									kIZPPrefix + 250 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryUpdateStoredRefServiceImpl,							kIZPPrefix + 251 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryUpdateProxyVersionNumServiceImpl,						kIZPPrefix + 252 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryInitializeServiceImpl,									kIZPPrefix + 253 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryTerminateServiceImpl,									kIZPPrefix + 254 )
DECLARE_PMID(kImplementationIDSpace, kZPAMLinkedStoryAfterCheckInServiceImpl,								kIZPPrefix + 255 )


#endif // __IZPID_h__

