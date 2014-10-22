//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSCallProcessors.h $
//	$Revision: 3542 $
//	$Date: 2011-07-26 14:36:13 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPWSCallProcessors_
#define _h_CZPWSCallProcessors_
#pragma once

struct soap;
class IZPUserCredentials;
class IZPWSConfig;

class CZPWSCallProcessors
{
public:
	virtual				~CZPWSCallProcessors();

	static void			InitProcessorsArray(
							ZPSoapRequestProcessor 			oArr[eReqCount] );
	static void			InitSoapStruct(
							soap *							oSoap,
							const IZPUserCredentials *		inUserCredentials,
							const IZPWSConfig *				inWSConfig );

#pragma mark -
#define DeclareCallProcessorFunction( inFunctionName )					\
	static ZPWSError	inFunctionName(									\
							const AZPSoapRequest *			inRequest,	\
							AZPSoapResponse *				oResponse );

	static ZPWSError	DoLogin(
							const AZPSoapRequest *			inRequest,
							AZPSoapResponse *				oResponse );

	DeclareCallProcessorFunction( DoAcquireLock )
	DeclareCallProcessorFunction( DoCanUpdateAssetName )
	DeclareCallProcessorFunction( DoCheckInAsset )
	DeclareCallProcessorFunction( DoCreateAsset )
	DeclareCallProcessorFunction( DoDeleteAsset )
	DeclareCallProcessorFunction( DoDownloadThumbnail )
	DeclareCallProcessorFunction( DoDownloadVersion )
	DeclareCallProcessorFunction( DoFetchAsset )
	DeclareCallProcessorFunction( DoFetchAssets )
	DeclareCallProcessorFunction( DoFetchAssetList )
	DeclareCallProcessorFunction( DoFetchEditions )
	DeclareCallProcessorFunction( DoFetchLockInfo )
	DeclareCallProcessorFunction( DoFetchTitleList )
	DeclareCallProcessorFunction( DoFetchTitleStatusList )
	DeclareCallProcessorFunction( DoGetRole )
	DeclareCallProcessorFunction( DoReleaseLock )
	DeclareCallProcessorFunction( DoRelinkAsset )
	DeclareCallProcessorFunction( DoUpdateAsset )
	DeclareCallProcessorFunction( DoUpdateAssetName )
	DeclareCallProcessorFunction( DoUpdateAssetStatus )
	DeclareCallProcessorFunction( DoUploadPagePreview )
	DeclareCallProcessorFunction( DoUploadPDF )
	DeclareCallProcessorFunction( DoUploadVersion )
	DeclareCallProcessorFunction( DoUploadExport )

	//Task Handling
	DeclareCallProcessorFunction( DoCreateTask )
	DeclareCallProcessorFunction( DoDeleteTask )
	DeclareCallProcessorFunction( DoFetchTaskCategoryList )
	DeclareCallProcessorFunction( DoFetchTaskStatusList )
	DeclareCallProcessorFunction( DoFetchTitleUsersList )
	DeclareCallProcessorFunction( DoFetchEditionTasksList )
	DeclareCallProcessorFunction( DoGetTasksHistory )
	DeclareCallProcessorFunction( DoUpdateTask )


protected:
private:
						CZPWSCallProcessors();	//No object creation allowed.
};

#endif //_h_CZPWSCallProcessors_
