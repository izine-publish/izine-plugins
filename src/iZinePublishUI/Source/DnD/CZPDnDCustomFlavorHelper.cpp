//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/DnD/CZPDnDCustomFlavorHelper.cpp $
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
#include "IDragDropTarget.h"

//SDK General includes
#include "CDragDropTargetFlavorHelper.h"
#include "DataObjectIterator.h"
#include "ErrorUtils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDragDropTargetFlavorHelper
class CZPDnDCustomFlavorHelper : public inheritClass
{
public:
						CZPDnDCustomFlavorHelper(IPMUnknown * boss);
	virtual				~CZPDnDCustomFlavorHelper();

	/**
		Determines whether we can handle the flavors in the drag.
		@param target IN the target the mouse is currently over.
		@param dataIter IN iterator providing access to the data objects within the drag.
		@param fromSource IN the source of the drag.
		@param controller IN the drag drop controller mediating the drag.
		@return a target response (either won't accept or drop will copy).

		@see DragDrop::TargetResponse
	*/
	virtual DragDrop::TargetResponse	
						CouldAcceptTypes(const IDragDropTarget* target, DataObjectIterator* dataIter, const IDragDropSource* fromSource, const IDragDropController* controller) const;

	/**
		performs the actual drag. Because we know that our custom flavor is masquerading and is 
		really a page item we must take a copy of the page item, then move this copy to the drop zone.
		We know we have been dropped on a widget (because we are called as part of that widget responding to
		a drop of the custom flavor).
		@param target IN the target for this drop.
		@param controller IN the drag drop controller that is mediating the drag.
		@param action IN what the drop means (i.e. copy, move etc)
		@return kSuccess if the drop is executed without error, kFailure, otherwise.
	*/
	virtual ErrorCode		ProcessDragDropCommand(IDragDropTarget*, IDragDropController*, DragDrop::eCommandType);

	DECLARE_HELPER_METHODS()
protected:
private:
};


CREATE_PMINTERFACE(CZPDnDCustomFlavorHelper, kZPDnDCustomFlavorHelperImpl)
DEFINE_HELPER_METHODS(CZPDnDCustomFlavorHelper)

CZPDnDCustomFlavorHelper::CZPDnDCustomFlavorHelper(IPMUnknown * boss)
: inheritClass(boss), HELPER_METHODS_INIT(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDnDCustomFlavorHelper::~CZPDnDCustomFlavorHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CouldAcceptTypes 
//----------------------------------------------------------------------------------------
DragDrop::TargetResponse 
CZPDnDCustomFlavorHelper::CouldAcceptTypes(
	const IDragDropTarget* target, DataObjectIterator* dataIter,
	const IDragDropSource* fromSource, const IDragDropController* controller) const
{
	// Check the available external flavors to see if we can handle any of them
	if (dataIter != nil)
	{
		// Test for swatches in the drag
		DataExchangeResponse response;
		
		response = dataIter->FlavorExistsWithPriorityInAllObjects(kIZPAssetIDDragDropCustomFlavor);
		if (response.CanDo())
			return DragDrop::TargetResponse(response, DragDrop::kDropWillCopy);
	}
	return DragDrop::kWontAcceptTargetResponse;
}

//----------------------------------------------------------------------------------------
// ProcessDragDropCommand
// process the drop, this method called if CouldAcceptTypes returns valid response 
//----------------------------------------------------------------------------------------
ErrorCode	
CZPDnDCustomFlavorHelper::ProcessDragDropCommand (
	IDragDropTarget* 			target, 
	IDragDropController* 		controller,
	DragDrop::eCommandType		action )
{	
	ErrorCode stat = kFailure;
	ICommandSequence* sequence = CmdUtils::BeginCommandSequence();
	if (sequence == nil)
	{
		ASSERT_FAIL("Cannot create command sequence?");
		return stat;
	}
#if defined(InDnCS5) || defined(InDnCS5_5)
	sequence->SetName(PMString("Update Asset Status Command", PMString::kDontTranslateDuringCall));
#else
	sequence->SetName(PMString("Update Asset Status Command", -1, PMString::kNoTranslate));
#endif

	do
	{
		if (action != DragDrop::kDropCommand) {
			break;
		}

		InterfacePtr<IDataExchangeHandler> handler(controller->QueryTargetHandler());
		ASSERT(handler);
		if (!handler) {
			stat = kFailure;
			break;
		}
		
		//TODO: do update the asset status
	} while(false);

	if (stat == kSuccess) 
	{
		// Everything completed so end the command sequence
		CmdUtils::EndCommandSequence(sequence);
		
	} else
	{
		// Abort the sequence and roll back the changes.
		ErrorUtils::PMSetGlobalErrorCode(stat);
		CmdUtils::EndCommandSequence(sequence);
	}

	return stat;
}