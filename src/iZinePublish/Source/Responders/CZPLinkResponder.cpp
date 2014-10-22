//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Responders/CZPLinkResponder.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDocument.h"
#include "ILinkCmdSignalData.h"
#include "ISignalMgr.h"
#include "IStoryLabel.h"
#include "IWorkgroupStoryFacade.h"

//SDK General includes
#include "CResponder.h"
#include "UIDList.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "ILinkResource.h"
#include "ILink.h"
#include "IWorkgroupFacade.h"
#endif
//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Responder
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CResponder
class CZPLinkResponder : public inheritClass
{
public:
						CZPLinkResponder(IPMUnknown * boss);
	virtual				~CZPLinkResponder();

	void				Respond(
							ISignalMgr*					signalMgr);
protected:
	void				RemoveLabelDuringUnlinkStories(
							const ICommand*				inCommand);
private:
};


CREATE_PMINTERFACE(CZPLinkResponder, kZPLinkResponderImpl)

CZPLinkResponder::CZPLinkResponder(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResponder::~CZPLinkResponder()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Respond
//----------------------------------------------------------------------------------------
void
CZPLinkResponder::Respond(
	ISignalMgr*					signalMgr)
{
	do {
		// TRACE the service raising the signal.
		const ServiceID serviceID = signalMgr->GetServiceID();
#ifdef DEBUG
		DebugClassUtilsBuffer serviceBuf;
		const char* serviceName = DebugClassUtils::GetIDName(&serviceBuf, serviceID);
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "serviceName=%s, ", serviceName);
#endif

		// TRACE the command associated with the signal.
		InterfacePtr<ILinkCmdSignalData> linkCmdSignalData(signalMgr, UseDefaultIID());
		if (linkCmdSignalData == nil)
			break;
		
		ICommand* signallingCommand = linkCmdSignalData->GetCommand();
		ASSERT(signallingCommand);
		if (!signallingCommand)
			break;
		
		const ICommand::CommandState cmdState = signallingCommand->GetCommandState();
#ifdef DEBUG
		DebugClassUtilsBuffer classBuf;
		const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass(signallingCommand));
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "signalling cmd=%s, cmdState=%d\n", className,	cmdState);
#endif

		// Non-document databases should be ignored. For instance, the app might be adding
		// a link to a book (kBookBoss)
		IDataBase *db = linkCmdSignalData->GetDataBase();
		UIDRef docRef = UIDRef(db, db->GetRootUID());
		InterfacePtr<IDocument> theDocument(docRef, UseDefaultIID());
		if (theDocument == nil)
			break;
		
		switch(serviceID.Get())
		{
			case kBeforeAddLinkSignalResponderService:
				break;

			case  kAfterAddLinkSignalResponderService:	
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kAddLinkUndoRedoSignalResponderService:
				break;

			case kBeforeAddLinksSignalResponderService:
				break;

			case kAfterAddLinksSignalResponderService:
				break;

			case kAddLinksUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeAddExtraLinkSignalResponderService:
				break;

			case kAfterAddExtraLinkSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kAddExtraLinkUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeAttachDataLinkSignalResponderService:
				break;
				
			case kAfterAttachDataLinkSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kAttachDataLinkUndoRedoSignalResponderService:
				break;

			case kBeforeMoveLinkSignalResponderService:
				break;

			case kAfterMoveLinkSignalResponderService:
				break;

			case kMoveLinkUndoRedoSignalResponderService:
				break;

			case kBeforeRefreshLinkSignalResponderService:
				break;

			case kAfterRefreshLinkSignalResponderService:
				break;
#endif

			case kBeforeRemoveLinksSignalResponderService:
				this->RemoveLabelDuringUnlinkStories(signallingCommand);
				break;

			case kAfterRemoveLinksSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kRemoveLinksUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeRestoreLinkSignalResponderService:
				break;

			case kAfterRestoreLinkSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kRestoreLinkUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeUpdateLinkSignalResponderService:
				break;

			case kAfterUpdateLinkSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kUpdateLinkUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeAddEmbedSignalResponderService:
				break;

			case kAfterAddEmbedSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kAddEmbedUndoRedoSignalResponderService:
				break;
#endif

			case kBeforeRemoveEmbedSignalResponderService:
				break;

			case kAfterRemoveEmbedSignalResponderService:
				break;

#ifdef InDnCS3	//TODO: CS5 Porting
			case kRemoveEmbedUndoRedoSignalResponderService:
				break;

			case kBeforeSetLinkURLSignalResponderService:
				break;

			case kAfterSetLinkURLSignalResponderService:
				break;

			case kSetLinkURLUndoRedoSignalResponderService:
				break;
#endif
		}
	} while(false);
}

//----------------------------------------------------------------------------------------
// RemoveLabelDuringUnlinkStories
//----------------------------------------------------------------------------------------
void
CZPLinkResponder::RemoveLabelDuringUnlinkStories(
	const ICommand*				inCommand)
{
	if (!inCommand)
		return;

	if (Utils<IZPInDnUtils>()->IsHostAppInDesign() == false)
		return;

	const UIDList * datalinkUIDList = inCommand->GetItemList();
	for(int x = 0; x < datalinkUIDList->Length(); ++x)
	{
		UIDRef storyRef;
#if defined(InDnCS5) || defined(InDnCS5_5)
		InterfacePtr<ILink> link(datalinkUIDList->GetRef( x ), UseDefaultIID());		
		ASSERT(link);		
		
		InterfacePtr<ILinkResource> res(::GetDataBase(link), link->GetResource(), UseDefaultIID()); 
		ASSERT(res);
		
		storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef(::GetUIDRef(res));
		Utils<Facade::IWorkgroupFacade>()->StopMonitoring(::GetUIDRef(res));
#else
		storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef(datalinkUIDList->GetRef( x ));
#endif
		if (storyRef != UIDRef::gNull)
		{
			InterfacePtr<IStoryLabel> storyLabel( storyRef, IID_ISTORYLABEL);
			if( storyLabel )
				storyLabel->SetLabel(kNullString);
		}
	}
}
