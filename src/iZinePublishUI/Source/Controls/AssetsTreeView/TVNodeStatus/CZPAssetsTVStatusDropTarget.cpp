//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeStatus/CZPAssetsTVStatusDropTarget.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CDragDropTarget.h"
#include "DataObjectIterator.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetInfo.h"
#include "IZPUITVNodeData.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDragDropTarget
class CZPAssetsTVStatusDropTarget : public inheritClass
{
public:
						CZPAssetsTVStatusDropTarget(IPMUnknown * boss);
	virtual				~CZPAssetsTVStatusDropTarget();

	void					DoDragEnter();
	/**
		this method defines the target response to the drag. It is called when the mouse enters the panel. We
		inidcate we can accept drags with a kPageItem flavor, that we do not want the default cursor and we do
		want the default panel highlight behaviour.

		@param dataIter IN iterator providing access to the data objects within the drag.
		@param fromSource IN the source of the drag.
		@param controller IN the drag drop controller mediating the drag.
		@return a target response (either won't accept or drop will copy).

		@see DragDrop::TargetResponse
	*/
	DragDrop::TargetResponse CouldAcceptTypes(
								DataObjectIterator* dataIter, const IDragDropSource* fromSource, const IDragDropController* controller) const;
	/**
		When the drop is performed, this method is called. We get the data item from the scrap and test its shape.
		We then change the static text widget associated with the panel to reflect the shape of this widget.
		@param controller IN the drag drop controller mediating the drag.
		@param type IN drag and drop command type
		@see DragDrop::eCommandType
	*/
	ErrorCode				ProcessDragDropCommand(
								IDragDropController* controller, DragDrop::eCommandType type);

	
protected:
	const IStringData *		QuerySourceAssetID(
								const IDragDropController*		controller ) const;
	bool					IsSourceAssetFromDifferentStatus(
								const IDragDropSource*			fromSource,
								const IDragDropController*		controller ) const;
	bool					CanChangeStatusForAsset(
								const IDragDropSource*			fromSource,
								const IDragDropController*		controller ) const;
private:
};


CREATE_PMINTERFACE(CZPAssetsTVStatusDropTarget, kZPAssetsTVStatusDropTargetImpl)

CZPAssetsTVStatusDropTarget::CZPAssetsTVStatusDropTarget(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVStatusDropTarget::~CZPAssetsTVStatusDropTarget()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// QuerySourceAssetID
//----------------------------------------------------------------------------------------
const IStringData *
CZPAssetsTVStatusDropTarget::QuerySourceAssetID(
	const IDragDropController*		controller) const
{
	do
	{
		// get the data exchnage handler for this object...
		InterfacePtr<IDataExchangeHandler> dataExchangeHandler(controller->QueryHandler(kIZPAssetIDDragDropCustomFlavor));
		if (dataExchangeHandler == nil)
			break;
		
		InterfacePtr<IStringData> dndAssetID(dataExchangeHandler, IID_IZPDATAID);
		ASSERT(dndAssetID);

		return dndAssetID.forget();
	}while(false);

	return nil;
}


//----------------------------------------------------------------------------------------
// IsSourceAssetFromDifferentStatus
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVStatusDropTarget::IsSourceAssetFromDifferentStatus(
	const IDragDropSource*			fromSource,
	const IDragDropController*		controller) const
{
	do
	{
		InterfacePtr<const IStringData> dndAssetID(this->QuerySourceAssetID( controller ));
		if( !dndAssetID )
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( dndAssetID->Get() );
		if( !dbAssetID )
			break;
		
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( !dbAssetInfo )
			break;

		InterfacePtr<const IZPUITVNodeData> selfNodeData(this, UseDefaultIID());
		ASSERT(selfNodeData);
		if( !selfNodeData )
			break;

		const IStringData * selfStatusID = (const IStringData*) selfNodeData->GetDataObj();

		const PMString & selfStatusIDStr = selfStatusID->Get();
		const PMString & srcAssetStatusID = dbAssetInfo->GetStatusID();

		if( selfStatusIDStr.Compare( kTrue, srcAssetStatusID ) != 0 )
			return true;

	}while(false);

	return false;
}

//----------------------------------------------------------------------------------------
// CanChangeStatusForAsset
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVStatusDropTarget::CanChangeStatusForAsset(
	const IDragDropSource*			fromSource,
	const IDragDropController*		controller) const
{
	do
	{
		InterfacePtr<const IStringData> dndAssetID(this->QuerySourceAssetID( controller ));
		if( !dndAssetID )
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( dndAssetID->Get() );
		if( !dbAssetID )
			break;

		return Utils<IZPAMHelper>()->CanChangeStatusForAsset(dbAssetID);;
	}while(false);

	return false;
}

//----------------------------------------------------------------------------------------
// DoDragEnter
//----------------------------------------------------------------------------------------
void
CZPAssetsTVStatusDropTarget::DoDragEnter()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IDragDropController> ddController(gSession, IID_IDRAGDROPCONTROLLER);
	ddController->SetTrackingCursorFeedback(CursorSpec(kCrsrOpenHand));
}

//----------------------------------------------------------------------------------------
// CouldAcceptTypes
//----------------------------------------------------------------------------------------
DragDrop::TargetResponse
CZPAssetsTVStatusDropTarget::CouldAcceptTypes(
	DataObjectIterator*				dataIter,
	const IDragDropSource*			fromSource,
	const IDragDropController*		controller) const
{
	DataExchangeResponse response;
	response = dataIter->FlavorExistsWithPriorityInAllObjects(kIZPAssetIDDragDropCustomFlavor);
	if(response.CanDo())
	{
		if(this->CanChangeStatusForAsset(fromSource, controller ) && this->IsSourceAssetFromDifferentStatus( fromSource, controller ) )
			return DragDrop::TargetResponse( response, DragDrop::kDropWillCopy, DragDrop::kUseDefaultTrackingFeedback, DragDrop::kTargetWillProvideCursorFeedback);

	}
	return DragDrop::kWontAcceptTargetResponse;
}

//----------------------------------------------------------------------------------------
// ProcessDragDropCommand
//----------------------------------------------------------------------------------------
ErrorCode
CZPAssetsTVStatusDropTarget::ProcessDragDropCommand(
	IDragDropController*			controller,
	DragDrop::eCommandType			type)
{
	bool16 returnCode = kFailure;
	
	do
	{
		InterfacePtr<const IStringData> dndAssetID(this->QuerySourceAssetID( controller ));
		ASSERT(dndAssetID);
		if (!dndAssetID)
			break;

		const PMString & assetIDStr = dndAssetID->Get();

		if( assetIDStr.IsEmpty() )
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( dndAssetID->Get() );
		if( !dbAssetID )
			break;
		
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( !dbAssetInfo )
			break;

		InterfacePtr<const IZPUITVNodeData> selfNodeData(this, UseDefaultIID());
		ASSERT(selfNodeData);
		if( !selfNodeData )
			break;

		const IStringData * selfStatusID = (const IStringData*) selfNodeData->GetDataObj();

		const PMString & selfStatusIDStr = selfStatusID->Get();
		const PMString & srcAssetStatusID = dbAssetInfo->GetStatusID();
		//call the WS method to update the status of asset.
		Utils<IZPAMWSUtils>()->UpdateAssetStatus_sync(assetIDStr, srcAssetStatusID, selfStatusIDStr);

		returnCode = kSuccess;
	}while(false);
	return returnCode;
}
