//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Responders/CZPDocSignalResponder.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDocumentSignalData.h"
#include "ISignalMgr.h"
#include "IWorkgroupFacade.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CResponder.h"
#include "TextID.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPCommandFacade.h"
#include "IZPDocUtils.h"
#include "IZPInDnUtils.h"
#include "IZPPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Responder
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CResponder
class CZPDocSignalResponder : public inheritClass
{
public:
						CZPDocSignalResponder(IPMUnknown * boss);
	virtual				~CZPDocSignalResponder();

	void				Respond(
							ISignalMgr*					signalMgr);
protected:
	void				HandleDuringSaveACopy(
							const UIDRef				inDocRef) const;
private:
	void				RemoveStylesFromNewDocumentDuringOpen(
							const UIDRef				inDocRef) const;
};


CREATE_PMINTERFACE(CZPDocSignalResponder, kZPDocSignalResponderImpl)

CZPDocSignalResponder::CZPDocSignalResponder(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDocSignalResponder::~CZPDocSignalResponder()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Respond
//----------------------------------------------------------------------------------------
void
CZPDocSignalResponder::Respond(
	ISignalMgr*					signalMgr)
{
	// Get the service ID from the signal manager
	ServiceID serviceTrigger = signalMgr->GetServiceID();
#ifdef DEBUG
	DebugClassUtilsBuffer serviceBuf;
	const char* serviceName = DebugClassUtils::GetIDName(&serviceBuf, serviceTrigger);
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "serviceName=%s, ", serviceName);
#endif
	// Get a UIDRef for the document.  It will be an invalid UIDRef
	// for BeforeNewDoc, BeforeOpenDoc, AfterSaveACopy, and AfterCloseDoc because
	// the document doesn't exist at that point.
	InterfacePtr<IDocumentSignalData> docSignalData(signalMgr, UseDefaultIID());

	if( !docSignalData )
		return;

	UIDRef docRef = docSignalData->GetDocument();

	// Take action based on the service ID
	switch (serviceTrigger.Get())
	{
		//case kBeforeNewDocSignalResponderService:
		//{
		//	break;
		//}
		//case kDuringNewDocSignalResponderService:
		//{
		//	break;
		//}
		case kAfterNewDocSignalResponderService:
		{
			this->RemoveStylesFromNewDocumentDuringOpen( docRef );
			break;
		}
		case kBeforeOpenDocSignalResponderService:
		{
			Utils<IZPAMHelper>()->HandleDocSignal_BeforeOpen( docSignalData->GetFile() );
			break;
		}
		case kDuringOpenDocSignalResponderService:
		{
			Utils<IZPAMHelper>()->HandleDocSignal_DuringOpen( docRef, docSignalData->GetFile() );
			break;
		}
		case kAfterOpenDocSignalResponderService:
		{
			Utils<IZPAMHelper>()->HandleDocSignal_AfterOpen( docRef, docSignalData->GetFile() );
			break;
		}
		case kBeforeSaveDocSignalResponderService:
		{
			break;
		}
		case kAfterSaveDocSignalResponderService:
		{
			break;
		}
		//case kBeforeSaveAsDocSignalResponderService:
		//{
		//	break;
		//}
		//case kAfterSaveAsDocSignalResponderService:
		//{
		//	break;
		//}
		//case kBeforeSaveACopyDocSignalResponderService:
		//{
		//	break;
		//}
		case kDuringSaveACopyDocSignalResponderService:
		{
			this->HandleDuringSaveACopy(docRef);
			break;
		}
		//case kAfterSaveACopyDocSignalResponderService:
		//{
		//	break;
		//}
		//case kBeforeRevertDocSignalResponderService:
		//{
		//	break;
		//}
		case kAfterRevertDocSignalResponderService:
		{
			Utils<IZPAMHelper>()->HandleDocSignal_AfterRevert( docRef, docSignalData->GetFile() );
			break;
		}
		case kBeforeCloseDocSignalResponderService:
		{
			Utils<IZPAMHelper>()->HandleDocSignal_BeforeClose(docRef, docSignalData->GetFile());
			break;
		}
		//case kAfterCloseDocSignalResponderService:
		//{
		//	break;
		//}

		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// HandleDuringSaveACopy
//----------------------------------------------------------------------------------------
void
CZPDocSignalResponder::HandleDuringSaveACopy(
	const UIDRef				inDocRef) const
{
	InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
	ASSERT( theDocument );
	if(theDocument && Utils<IZPInDnUtils>()->IsHostAppInDesign())
	{
		Utils<IZPDocUtils>()->RemoveAllStoriesOnlyFromDoc(theDocument);
		Utils<IZPDocUtils>()->RemoveAllAssignmentsOnlyFromDoc(theDocument);
	}

	Utils<IZPCommandFacade>()->SaveAssetData(nil, theDocument);		//Clear asset data
}

//----------------------------------------------------------------------------------------
// RemoveStylesFromNewDocumentDuringOpen
//----------------------------------------------------------------------------------------
void
CZPDocSignalResponder::RemoveStylesFromNewDocumentDuringOpen(
	const UIDRef				inDocRef) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	
	InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(zpPrefs);

	if (zpPrefs->GetRemoveStyles())
	{
		Utils<IZPDocUtils> docUtils;
		docUtils->RemoveAllStyles(inDocRef, IID_IPARASTYLEGROUPMANAGER);	//Para style
		docUtils->RemoveAllStyles(inDocRef, IID_ICHARSTYLEGROUPMANAGER);	//Char style
	}
}
