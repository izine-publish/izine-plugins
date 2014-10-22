//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVAssetNodeDragDropSource.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
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
#include "IObserver.h"
#include "IStringData.h"
#include "ISysFileListData.h"
#include "ITreeNodeIDData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "CDragDropSource.h"
#include "DocumentID.h"
#include "SDKFileHelper.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetInfo.h"
#include "IZPInDnUtils.h"
#include "IZPUITVNodeData.h"
#include "IZPGlobalTempData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DnD
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDragDropSource
class CZPAssetsTVAssetNodeDragDropSource : public inheritClass
{
public:
						CZPAssetsTVAssetNodeDragDropSource(IPMUnknown * boss);
	virtual				~CZPAssetsTVAssetNodeDragDropSource();

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
	bool16				CanDropAssetOnDocument(
							const IStringData *							inAssetID ) const;
private:
	
	/** Adds  data to the drag
	 @param controller [IN] in charge of this widget's DND capabilities.
	 @return kTrue if valid content has been added to the drag,
	 kFalse otherwise.
	 */
	bool16 				doAddMyDragContent(
							IDragDropController* 						controller,
							IPMDataObject * & 							oDataObj);

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
	bool16 				doAddSysFileDragContent(
								IDragDropController* 					controller,
								IPMDataObject * 						inDataObj);
};


CREATE_PMINTERFACE(CZPAssetsTVAssetNodeDragDropSource, kZPAssetsTVAssetNodeDragDropSourceImpl)

CZPAssetsTVAssetNodeDragDropSource::CZPAssetsTVAssetNodeDragDropSource(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVAssetNodeDragDropSource::~CZPAssetsTVAssetNodeDragDropSource()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// CanDropAssetOnDocument
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVAssetNodeDragDropSource::CanDropAssetOnDocument(
	const IStringData *							inAssetID) const
{
	bool16 toReturn = kFalse;
	do
	{
		if( inAssetID == nil || Utils<IZPInDnUtils>()->IsHostAppInDesign() != kTrue )
			break;

		InterfacePtr<const IZPAssetInfo> assetDetail( inAssetID, UseDefaultIID() );
		ASSERT( assetDetail );

		if( assetDetail && (assetDetail->GetType() == eAssetType_ICDocument || assetDetail->GetType() == eAssetType_IDDocument) )
		{
			toReturn = kTrue;
		}
	}while(kFalse);

	return toReturn;
}

/* WillDrag
 */
bool16
CZPAssetsTVAssetNodeDragDropSource::WillDrag(IEvent* event) const
{
	bool16 toReturn = kFalse;
	do
	{
		//if( Utils<IZPInDnUtils>()->IsHostAppInDesign() != kTrue )
		//	break;

		InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
		ASSERT(nodeData);
		const IPMUnknown * assetID = nodeData->GetDataObj();

		InterfacePtr<const IZPAssetInfo> assetDetail( assetID, UseDefaultIID() );
		ASSERT( assetDetail );

		if( assetDetail && assetDetail->GetType() == eAssetType_IDTemplate )
			toReturn = kFalse;
		else
			toReturn = kTrue;
	}while(kFalse);

	return toReturn;
}


/* DoAddDragContent
 */
bool16
CZPAssetsTVAssetNodeDragDropSource::DoAddDragContent(
	IDragDropController* controller)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	const IPMUnknown * assetID = nodeData->GetDataObj();

	IPMDataObject * theDataObj = nil;
	bool16 dragContentAdded = this->doAddMyDragContent(controller, theDataObj);

	InterfacePtr<IZPGlobalTempData> zpGlobalData( gSession, UseDefaultIID() );
	ASSERT( zpGlobalData );
	zpGlobalData->SetLastDraggedAssetID( kNullString );

	if( this->CanDropAssetOnDocument( (const IStringData *)assetID ) )
	{
		bool16 filePathAdded = this->doAddSysFileDragContent( controller, theDataObj );
		if( filePathAdded )
		{
			if( zpGlobalData )
				zpGlobalData->SetLastDraggedAssetID( ((const IStringData *)assetID)->Get() );
			InterfacePtr<IObserver> zpDnDObserver( gSession, IID_IZPUIASSETDNDOBSERVER );
			ASSERT( zpDnDObserver );
			zpDnDObserver->AutoAttach();
		}
		return filePathAdded;
	}
	else
		return dragContentAdded;
}


/* doAddMyDragContent
 */
bool16
CZPAssetsTVAssetNodeDragDropSource::doAddMyDragContent(
	IDragDropController* controller,
	IPMDataObject * &		oDataObj)
{
	LogFunctionEnterExit;
	
	bool16 result = kFalse;
	
	do
	{
		// Point the controller at the handler.
		InterfacePtr<IDataExchangeHandler> dataExchangeHandler(controller->QueryHandler(kIZPAssetIDDragDropCustomFlavor));
		ASSERT(dataExchangeHandler);
		if (!dataExchangeHandler)
			break;
		
		if (dataExchangeHandler->IsEmpty() == kFalse)
			dataExchangeHandler->Clear();
		
		controller->SetSourceHandler(dataExchangeHandler);
		
		// Add the unique key for asset to be dragged.
		InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * selfAssetID = (const IStringData *)nodeData->GetDataObj();

		InterfacePtr<IStringData> assetID(dataExchangeHandler, IID_IZPDATAID);
		ASSERT(assetID);
		if (!assetID)
			break;
		assetID->Set(selfAssetID->Get());
		
		// Indicate the flavour in the drag object.
		IPMDataObject * dataObj = controller->GetDragItem(1);
		if( dataObj == nil )
		{
			InterfacePtr<IPMDataObject>
				pmDataObject(controller->AddDragItem(1));
			ASSERT(pmDataObject);
			dataObj = pmDataObject;
		}
		oDataObj = dataObj;

		// no flavor flags
		PMFlavorFlags flavorFlags = kNormalFlavorFlag;
		
		// Set the type (flavour) in the drag object
		dataObj->PromiseFlavor(kIZPAssetIDDragDropCustomFlavor, flavorFlags);
		
		result = kTrue;
	} while(false);
	
	return result;
}


//----------------------------------------------------------------------------------------
// getContentFile
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVAssetNodeDragDropSource::getContentFile(
	IDFile& contentFile) const
{
	bool16 result = kFalse;
	do {
		InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
		ASSERT(nodeData);
		
		InterfacePtr<const IStringData> dbAssetID( nodeData->GetDataObj(), IID_IZPDATAID);
		ASSERT( dbAssetID );
		
		if( Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, contentFile ) )
			result = kTrue;
	} while(false);
	return result;
}

//----------------------------------------------------------------------------------------
// doAddSysFileDragContent
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVAssetNodeDragDropSource::doAddSysFileDragContent(
	IDragDropController* controller,
	IPMDataObject *		inDataObj)
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
		IPMDataObject * dataObj = controller->GetDragItem(1);	//Someday: Find why this is coming nil each time?
		if( dataObj == nil )
			dataObj = inDataObj;
		if( dataObj == nil )
		{
			InterfacePtr<IPMDataObject>
				pmDataObject(controller->AddDragItem(1));
			ASSERT(pmDataObject);
			dataObj = pmDataObject;
		}
		uint32 val = dataObj->GetItemID();

		PMFlavorFlags flavorFlags = kNormalFlavorFlag;
		dataObj->PromiseFlavor(kPMSysFileFlavor, flavorFlags);

		result = kTrue;

	} while(false);

	return result; 
}
