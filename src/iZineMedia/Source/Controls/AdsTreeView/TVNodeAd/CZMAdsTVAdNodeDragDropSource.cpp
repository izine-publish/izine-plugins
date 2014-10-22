//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAd/CZMAdsTVAdNodeDragDropSource.cpp $
//	$Revision: 2894 $
//	$Date: 2011-04-29 10:34:51 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
//#include "IDataExchangeHandler.h"
//#include "IDragDropController.h"
//#include "IPMDataObject.h"
#include "IStringData.h"
#include "ISysFileListData.h"
#include "ITreeNodeIDData.h"

//SDK General includes
#include "CAlert.h"
#include "CDragDropSource.h"
#include "DocumentID.h"
#include "FileUtils.h"
#include "SDKFileHelper.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdInfo.h"
#include "IZMUITVNodeData.h"
#include "IZMAdFileFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DnD
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDragDropSource
class CZMAdsTVAdNodeDragDropSource : public inheritClass
{
public:
						CZMAdsTVAdNodeDragDropSource(IPMUnknown * boss);
	virtual				~CZMAdsTVAdNodeDragDropSource();

	/** Returns kTrue if a unique key is associated with the widget.
	 @param event [IN] that signifies the drag.
	 @return kTrue if a drag can be initiated, otherwise kFalse.
	 */
	bool16 WillDrag(IEvent* event) const;
	
	/** Adds the unique key to the drag.
	 @param controller [IN] in charge of this widget's DND capabilities.
	 @return kTrue if valid content has been added to the drag, kFalse otherwise.
	 */
	bool16 DoAddDragContent(IDragDropController* controller);
protected:
private:
	
	/** Adds  data to the drag
	 @param controller [IN] in charge of this widget's DND capabilities.
	 @return kTrue if valid content has been added to the drag,
	 kFalse otherwise.
	 */
	bool16 doAddMyDragContent(IDragDropController* controller);

	/** Gets the IDFile associated with this widget.
		@param contentFile [OUT] specifies associated IDFile.
		@return kTrue if a draggable IDFile can be identified, kFalse otherwise.
	 */
	bool16 getContentFile(IDFile& contentFile) const;


	/** Adds kPMSysFileFlavor data to the drag describing the IDFile associated
		with this widget.
		@param controller [IN] in charge of this widget's DND capabilities.
		@return kTrue if valid content has been added to the drag, kFalse otherwise.
	 */
	bool16 doAddSysFileDragContent(IDragDropController* controller);
};


CREATE_PMINTERFACE(CZMAdsTVAdNodeDragDropSource, kZMAdsTVAdNodeDragDropSourceImpl)

CZMAdsTVAdNodeDragDropSource::CZMAdsTVAdNodeDragDropSource(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVAdNodeDragDropSource::~CZMAdsTVAdNodeDragDropSource()
{

}

#pragma mark -

/* WillDrag
 */
bool16
CZMAdsTVAdNodeDragDropSource::WillDrag(IEvent* event) const
{
	bool16 toReturn = kFalse;
	do
	{
		InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
		ASSERT(nodeData);
		const IPMUnknown * adID = nodeData->GetDataObj();

		InterfacePtr<const IZMAdInfo> adInfoID( adID, UseDefaultIID());
		ASSERT( adInfoID );
		
		if( adInfoID && adInfoID->GetUrl().CharCount() > 0 )
		{
			toReturn = kTrue;
		}
	}while(kFalse);

	return toReturn;
}


/* DoAddDragContent
 */
bool16
CZMAdsTVAdNodeDragDropSource::DoAddDragContent(
	IDragDropController* controller)
{
	LogFunctionEnterExit;
	
	//return this->doAddMyDragContent(controller);
	return this->doAddSysFileDragContent( controller );
}


/* doAddMyDragContent
 */
bool16
CZMAdsTVAdNodeDragDropSource::doAddMyDragContent(
	IDragDropController* controller)
{
	LogFunctionEnterExit;
	
	bool16 result = kFalse;
	
	do
	{
#if 0	//TODO: RJK - also in publish UI
		// Stop if we can't determine the asset we are associated with.
		PMString uk;
		if (this->getContentAssetUniqueKey(uk) == kFalse) {
			break;
		}
		
		// Point the controller at the handler.
		InterfacePtr<IDataExchangeHandler>
		dataExchangeHandler(controller->QueryHandler(kIZPDragDropCustomFlavor));
		ASSERT(dataExchangeHandler);
		if (!dataExchangeHandler) {
			break;
		}
		if (dataExchangeHandler->IsEmpty() == kFalse) {
			dataExchangeHandler->Clear();
		}
		controller->SetSourceHandler(dataExchangeHandler);
		
		// Add the unique key for asset to be dragged.
		InterfacePtr<IStringData> uniqueKeyStrungData(dataExchangeHandler, IID_IIZPUNIQUEKEYSTRINGDATA);
		ASSERT(uniqueKeyStrungData);
		if (!uniqueKeyStrungData)
		{
			break;
		}
		uniqueKeyStrungData->Set(uk);
		
		// Indicate the flavour in the drag object.
		InterfacePtr<IPMDataObject>
		pmDataObject(controller->AddDragItem(1));
		ASSERT(pmDataObject);
		// no flavor flags
		PMFlavorFlags flavorFlags = 0;
		
		// Set the type (flavour) in the drag object
		pmDataObject->PromiseFlavor(kIZPDragDropCustomFlavor, flavorFlags);
		
		result = kTrue;
#endif
	} while(false);
	
	return result;
}


//----------------------------------------------------------------------------------------
// getContentFile
//----------------------------------------------------------------------------------------
bool16
CZMAdsTVAdNodeDragDropSource::getContentFile(
	IDFile& contentFile) const
{
	bool16 result = kFalse;
	do {
		InterfacePtr<const IZMUITVNodeData> nodeData(this, UseDefaultIID());
		ASSERT(nodeData);
		
		InterfacePtr<const IStringData> adID( nodeData->GetDataObj(), IID_IZMDATAID);
		ASSERT( adID );

		InterfacePtr<const IZMAdInfo> adInfo( adID, UseDefaultIID());
		ASSERT( adInfo );
		
		if( adInfo && adInfo->GetUrl().CharCount() > 0 )
		{
			PMString fullUrl = Utils<IZMAdFileFacade>()->MakeFullAdFilePath( adInfo );
			if( fullUrl.CharCount() > 0 )
			{
				contentFile = FileUtils::PMStringToSysFile(fullUrl);//contentFile.SetPath( WideString( fullUrl ));//adInfo->GetUrl()) );
				result = kTrue;
			}
		}
	} while(false);
	return result;
}

//----------------------------------------------------------------------------------------
// doAddSysFileDragContent
//----------------------------------------------------------------------------------------
bool16
CZMAdsTVAdNodeDragDropSource::doAddSysFileDragContent(
	IDragDropController* controller)
{
	bool16 result = kFalse;

	do
	{
		// Stop if we can't determine the IDFile we are associated with.
		IDFile contentFile;
		if (this->getContentFile(contentFile) == kFalse)
		{
			break;
		}

		// Point the controller at the handler.
		InterfacePtr<IDataExchangeHandler> 
			dataExchangeHandler(controller->QueryHandler(kPMSysFileFlavor));
		ASSERT(dataExchangeHandler);
		if (!dataExchangeHandler)
		{
			break;
		}
		if (dataExchangeHandler->IsEmpty() == kFalse)
		{
			dataExchangeHandler->Clear();
		}
		controller->SetSourceHandler(dataExchangeHandler);

		// Add the IDFile to be dragged.
		InterfacePtr<ISysFileListData> 
			sysFileListData(dataExchangeHandler, IID_ISYSFILELISTDATA);
		ASSERT(sysFileListData);
		if (!sysFileListData)
		{
			break;
		}
		sysFileListData->Append(contentFile);
			
		// Indicate the flavour in the drag object. 
		InterfacePtr<IPMDataObject>
			pmDataObject(controller->AddDragItem(1));
		ASSERT(pmDataObject);
		PMFlavorFlags flavorFlags = kNormalFlavorFlag;
		pmDataObject->PromiseFlavor(kPMSysFileFlavor, flavorFlags);

		result = kTrue;

	} while(false);

	return result; 
}
