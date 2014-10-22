//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/IZExp_StringKeys.h $
//	$Revision: 4013 $
//	$Date: 2012-05-22 15:06:00 +0200 (Tue, 22 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZExp_StringKeys_
#define _h_IZExp_StringKeys_
#pragma once


//KBSE areas
#define kZExpActionArea					"KBSCE iZineExp"

//Menu Path and Item Name Key
#define kIZExpFileMenuPath				"Main:&File"
	// "About Plug-ins" sub-menu:
#define kIZExpAboutMenuPath				kSDKDefStandardAboutMenuPath kIZExpCompanyKey
#define kIZExpAboutMenuKey				kIZExpStringPrefix "kIZExpAboutMenuKey"

//#define kIZExpMainMenuPath				"Main" kSDKDefDelimitMenuPath kIZExpPluginsMenuKey

#define kIZExpExportFrontDocMenuKey		kIZExpStringPrefix "kIZExpExportFrontDocMenuKey"
#define kIZExpExportMenuKey				kIZExpStringPrefix "kIZExpExportMenuKey"
#define kIZExpExportPrefsMenuKey		kIZExpStringPrefix "kIZExpExportPrefsMenuKey"

	// Prefs Panel
#define kZExpPrefsPanelTitleKey			kIZExpStringPrefix "kZExpPrefsPanelTitleKey"
#define kZExpPrefsPanelUserNameKey		kIZExpStringPrefix "kZExpPrefsPanelUserNameKey"
#define kZExpPrefsPanelPasswordKey		kIZExpStringPrefix "kZExpPrefsPanelPasswordKey"
#define kZExpPrefsPanelPluginIDKey		kIZExpStringPrefix "kZExpPrefsPanelPluginIDKey"
#define kZExpPrefsDlgUseProxyServerCheckBoxKey	kIZExpStringPrefix "kZExpPrefsDlgUseProxyServerCheckBoxKey"
#define kZExpPrefsDlgProxyServerKey		kIZExpStringPrefix "kZExpPrefsDlgProxyServerKey"
#define kZExpPrefsDlgProxyPortKey		kIZExpStringPrefix "kZExpPrefsDlgProxyPortKey"

	//Export dlg
#define kZExpExportDialogTitleKey		kIZExpStringPrefix "kZExpExportDialogTitleKey"
#define kZExpExportButtonKey			kIZExpStringPrefix "kZExpExportButtonKey"
#define kZExpExportTitleKey				kIZExpStringPrefix "kZExpExportTitleKey"
#define kZExpExportEditionKey			kIZExpStringPrefix "kZExpExportEditionKey"
#define kZExpExportDlgAdvOptionGrpKey			kIZExpStringPrefix "kZExpExportDlgAdvOptionGrpKey"
#define kZExpExportDlgAdvOptionBothKey			kIZExpStringPrefix "kZExpExportDlgAdvOptionBothKey"
#define kZExpExportDlgAdvOptionSkipCreateKey	kIZExpStringPrefix "kZExpExportDlgAdvOptionSkipCreateKey"
#define kZExpExportDlgAdvOptionSkipUploadKey	kIZExpStringPrefix "kZExpExportDlgAdvOptionSkipUploadKey"

	//Progress bar labels
#define kZExpProgRedfiveExportTitleKey	kIZExpStringPrefix "kZExpProgRedfiveExportTitleKey"
#define kZExpProgFetchingConfigKey		kIZExpStringPrefix "kZExpProgFetchingConfigKey"
#define kZExpProgCreatingXMLKey			kIZExpStringPrefix "kZExpProgCreatingXMLKey"
#define kZExpProgUploadXMLKey			kIZExpStringPrefix "kZExpProgUploadXMLKey"
#define kZExpProgUploadPDFKey			kIZExpStringPrefix "kZExpProgUploadPDFKey"
#define kZExpProgUploadLowResImageKey	kIZExpStringPrefix "kZExpProgUploadLowResImageKey"
#define kZExpProgUploadHighResImageKey	kIZExpStringPrefix "kZExpProgUploadHighResImageKey"
#define kZExpProgCreatingPDFKey			kIZExpStringPrefix "kZExpProgCreatingPDFKey"
#define kZExpProgFinishingUploadKey		kIZExpStringPrefix "kZExpProgFinishingUploadKey"

	//Messages for UI
#define	kZExpMsgAbortedByUserKey		kIZExpStringPrefix "kZExpMsgAbortedByUserKey"
#define	kZExpMsgUploadSuccessKey		kIZExpStringPrefix "kZExpMsgUploadSuccessKey"
#define	kZExpMsgExportCreateSuccessKey		kIZExpStringPrefix "kZExpMsgExportCreateSuccessKey"
#define kZExpMsgRetryImageUploadStringKey	kIZExpUIStringPrefix "kZExpMsgRetryImageUploadStringKey"

	//Error messages
#define	kIZExpMsgFileWritingFailKey		kIZExpStringPrefix "kIZExpMsgFileWritingFailKey"
#define	kIZExpParamFailKey				kIZExpStringPrefix "kIZExpParamFailKey"
#define	kZExpMsgWSFailKey				kIZExpStringPrefix "kZExpMsgWSFailKey"
#define	kZExpMsgXMLFileCreationFailKey	kIZExpStringPrefix "kZExpMsgXMLFileCreationFailKey"
#define	kZExpMsgPDFFileCreationFailKey	kIZExpStringPrefix "kZExpMsgPDFFileCreationFailKey"
#define	kZExpMsgXMLReadFailKey			kIZExpStringPrefix "kZExpMsgXMLReadFailKey"
#define	kZExpMsgXMLUploadingFailKey		kIZExpStringPrefix "kZExpMsgXMLUploadingFailKey"
#define	kZExpMsgPDFUploadingFailKey		kIZExpStringPrefix "kZExpMsgPDFUploadingFailKey"
#define	kZExpMsgHighResImageUploadFailKey	kIZExpStringPrefix "kZExpMsgHighResImageUploadFailKey"
#define	kZExpMsgLowResImageUploadFailKey	kIZExpStringPrefix "kZExpMsgLowResImageUploadFailKey"
#define	kZExpMsgUploadFinishFailKey			kIZExpStringPrefix "kZExpMsgUploadFinishFailKey"
#define	kZExpMsgPluginIdEmptyErrorKey	kIZExpStringPrefix "kZExpMsgPluginIdEmptyErrorKey"
#define	kZExpMsgConfigDataFetchFailKey	kIZExpStringPrefix "kZExpMsgConfigDataFetchFailKey"

	// Other StringKeys:
#define kIZExpAboutBoxStringKey			kIZExpStringPrefix "kIZExpAboutBoxStringKey"

#pragma mark -
// Translatable Strings
#define kIZExpRetryBtnStringKey			kIZExpUIStringPrefix "kIZExpRetryBtnStringKey"
#define kIZExpAbortBtnStringKey			kIZExpUIStringPrefix "kIZExpAbortBtnStringKey"

//Format type
#define kIZExpXMLFormatStringKey		kIZExpStringPrefix "kIZExpXMLFormatStringKey"









// Non-Translatable String
	

#endif //_h_IZExp_StringKeys_
