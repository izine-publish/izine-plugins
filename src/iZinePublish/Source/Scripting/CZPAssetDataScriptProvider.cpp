//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Scripting/CZPAssetDataScriptProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CScriptProvider.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetUtils.h"

//IZP General includes
#include "ZPScriptingDefs.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Scripting
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CScriptProvider
class CZPAssetDataScriptProvider : public inheritClass
{
public:
						CZPAssetDataScriptProvider(IPMUnknown * boss);
	virtual				~CZPAssetDataScriptProvider();

#if defined(InDnCS5) || defined(InDnCS5_5)
	//	Override CScriptProvider function
	virtual ErrorCode	HandleMethod(
							ScriptID eventID, IScriptRequestData* data, IScript* script ) ;

	//	Override CScriptProvider function
	virtual ErrorCode	AccessProperty(
							ScriptID propID, IScriptRequestData* data, IScript* script ) ;
#else
	//	Override CScriptProvider function
	virtual ErrorCode	HandleEvent(
							ScriptID eventID, IScriptEventData* data, IScript* script ) ;

	//	Override CScriptProvider function
	virtual ErrorCode	AccessProperty(
							ScriptID propID, IScriptEventData* data, IScript* script ) ;
#endif

protected:
#if defined(InDnCS5) || defined(InDnCS5_5)
	ErrorCode			AccessAssetData(
							IScriptRequestData* data, IScript* script, ScriptID propID);
#else
	ErrorCode			AccessAssetData(
							IScriptEventData* data, IScript* script, ScriptID propID);
#endif
private:
};


CREATE_PMINTERFACE(CZPAssetDataScriptProvider, kZPAssetDataScriptProviderImpl)

CZPAssetDataScriptProvider::CZPAssetDataScriptProvider(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetDataScriptProvider::~CZPAssetDataScriptProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleEvent
//----------------------------------------------------------------------------------------
ErrorCode
#if defined(InDnCS5) || defined(InDnCS5_5)
CZPAssetDataScriptProvider::HandleMethod(
ScriptID eventID, IScriptRequestData* data, IScript* script)
#else
CZPAssetDataScriptProvider::HandleEvent(
ScriptID eventID, IScriptEventData* data, IScript* script)
#endif
{
	LogFunctionEnterExit;
	
	ErrorCode toReturn = kFailure;
	do {
		switch (eventID.Get())
		{
			//case :
			//	break;
			default:
#if defined(InDnCS5) || defined(InDnCS5_5)
				toReturn = inheritClass::HandleMethod( eventID, data, script );
#else
				toReturn = inheritClass::HandleEvent( eventID, data, script );
#endif
				break;
		}
	} while(kFalse);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AccessProperty
//----------------------------------------------------------------------------------------
ErrorCode
CZPAssetDataScriptProvider::AccessProperty(
#if defined(InDnCS5) || defined(InDnCS5_5)
	ScriptID propID, IScriptRequestData* data, IScript* script)
#else
	ScriptID propID, IScriptEventData* data, IScript* script)
#endif
{
	LogFunctionEnterExit;
	
	ErrorCode toReturn = kFailure;
	do {
		switch (propID.Get())
		{
			case p_ZPAssetData:
				toReturn =  this->AccessAssetData( data, script, propID );
				break;
				
			default:
				toReturn = inheritClass::AccessProperty( propID, data, script );
				break;
		}
	} while(kFalse);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AccessAssetData
//----------------------------------------------------------------------------------------
ErrorCode
CZPAssetDataScriptProvider::AccessAssetData(
#if defined(InDnCS5) || defined(InDnCS5_5)
	IScriptRequestData* data, IScript* script, ScriptID propID)
#else
	IScriptEventData* data, IScript* script, ScriptID propID)
#endif
{
	LogFunctionEnterExit;
	
	ErrorCode toReturn = kFailure;

#if DEBUG
	DebugClassUtilsBuffer classBuf;
	const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass( script ));
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " script object class name = %s", className );
#endif
	do {
		InterfacePtr<IZPAssetData> assetData( script, UseDefaultIID());
		ASSERT(assetData);
		if( !assetData )
			break;
		
		ScriptData retScriptData;
		ScriptListData listData;

		if (data->IsPropertyGet())
		{
			
			
			ScriptData tempScriptData;
			tempScriptData.SetPMString( assetData->GetAssetID());
			listData.push_back(tempScriptData);

			tempScriptData.SetPMString(assetData->GetTitleID());
			listData.push_back(tempScriptData);

			tempScriptData.SetPMString(assetData->GetEditionID());
			listData.push_back(tempScriptData);

			retScriptData.SetList(listData);
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
			data->AppendReturnData(script, propID, retScriptData);
#else
			data->SetReturnData(&retScriptData);
#endif
			toReturn = kSuccess;
		}
		else if (data->IsPropertyPut())
		{
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
			toReturn = data->ExtractRequestData( propID, retScriptData );
#else
			toReturn = data->ExtractEventData( propID, retScriptData );
#endif
			if (toReturn != kSuccess)
				break;
			
			toReturn = retScriptData.GetList( listData ) ;
			//if (toReturn != kSuccess)
			//	break;
			
			int32 listIndex = 0;
			
			PMString assetID, titleID, editionID;
			if( listData.size() > listIndex )
				listData[listIndex++].GetPMString(assetID);
			if( listData.size() > listIndex )
				listData[listIndex++].GetPMString(titleID);
			if( listData.size() > listIndex )
				listData[listIndex++].GetPMString(editionID);
			
#if defined CacheWithFolder && defined MACINTOSH
			if (!assetID.IsEmpty())		//Patch on mac
				Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(assetID);
#endif

			ZPCommandHelper::SaveAssetData( assetData, assetID, titleID, editionID );
		}
	} while(kFalse);
	
	return toReturn ;
	
}