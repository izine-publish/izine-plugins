//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Responders/CZPLinkResponderServiceProvider.cpp $
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

//SDK General includes
#include "CServiceProvider.h"
#include "LinksID.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Responder
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CServiceProvider
class CZPLinkResponderServiceProvider : public inheritClass
{
public:
						CZPLinkResponderServiceProvider(IPMUnknown * boss);
	virtual				~CZPLinkResponderServiceProvider();

	// See IK2ServiceProvider::GetName
	virtual void GetName(PMString* pName);

	// See IK2ServiceProvider::GetServiceID
	virtual ServiceID GetServiceID();

	// See IK2ServiceProvider::IsDefaultServiceProvider
	virtual bool16 IsDefaultServiceProvider();
	
	// See IK2ServiceProvider::GetInstantiationPolicy
	virtual InstancePerX GetInstantiationPolicy();

	// See IK2ServiceProvider::HasMultipleIDs
	virtual bool16 HasMultipleIDs() const;

	// See IK2ServiceProvider::GetServiceIDs
	virtual void GetServiceIDs(K2Vector<ServiceID>& serviceIDs);
protected:
private:
	K2Vector<ServiceID> fServices;
};


CREATE_PMINTERFACE(CZPLinkResponderServiceProvider, kZPLinkResponderServiceProviderImpl)

CZPLinkResponderServiceProvider::CZPLinkResponderServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	//	kAddLinkCmdBoss
	fServices.push_back(kBeforeAddLinkSignalResponderService);
	fServices.push_back(kAfterAddLinkSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kAddLinkUndoRedoSignalResponderService);
	//	kAddLinksCmdBoss
	fServices.push_back(kBeforeAddLinksSignalResponderService);
	fServices.push_back(kAfterAddLinksSignalResponderService);
	fServices.push_back(kAddLinksUndoRedoSignalResponderService);
#endif
	//	kAddExtraLinkCmdBoss
	fServices.push_back(kBeforeAddExtraLinkSignalResponderService);
	fServices.push_back(kAfterAddExtraLinkSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kAddExtraLinkUndoRedoSignalResponderService);
#endif
	// kAttachDataLinkCmdBoss
	fServices.push_back(kBeforeAttachDataLinkSignalResponderService);
	fServices.push_back(kAfterAttachDataLinkSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kAttachDataLinkUndoRedoSignalResponderService);
	//	kMoveLinkCmdBoss
	fServices.push_back(kBeforeMoveLinkSignalResponderService);
	fServices.push_back(kAfterMoveLinkSignalResponderService);
	fServices.push_back(kMoveLinkUndoRedoSignalResponderService);
	//	kRefreshLinkCmdBoss
	fServices.push_back(kBeforeRefreshLinkSignalResponderService);
	fServices.push_back(kAfterRefreshLinkSignalResponderService);
#endif	
	//	kRemoveLinksCmdBoss
	fServices.push_back(kBeforeRemoveLinksSignalResponderService);
	fServices.push_back(kAfterRemoveLinksSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kRemoveLinksUndoRedoSignalResponderService);
#endif

	//	kRestoreLinkCmdBoss
	fServices.push_back(kBeforeRestoreLinkSignalResponderService);
	fServices.push_back(kAfterRestoreLinkSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kRestoreLinkUndoRedoSignalResponderService);
#endif

	//	kUpdateLinkCmdBoss
	fServices.push_back(kBeforeUpdateLinkSignalResponderService);
	fServices.push_back(kAfterUpdateLinkSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kUpdateLinkUndoRedoSignalResponderService);
#endif

	//	kAddEmbedCmdBoss
	fServices.push_back(kBeforeAddEmbedSignalResponderService);
	fServices.push_back(kAfterAddEmbedSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kAddEmbedUndoRedoSignalResponderService);
#endif

	//	kRemoveEmbedCmdBoss
	fServices.push_back(kBeforeRemoveEmbedSignalResponderService);
	fServices.push_back(kAfterRemoveEmbedSignalResponderService);
#ifdef InDnCS3	//TODO: CS5 Porting
	fServices.push_back(kRemoveEmbedUndoRedoSignalResponderService);

	//	kWFSetLinkURLCommandBoss
	fServices.push_back(kBeforeSetLinkURLSignalResponderService);
	fServices.push_back(kAfterSetLinkURLSignalResponderService);
	fServices.push_back(kSetLinkURLUndoRedoSignalResponderService);
#endif
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResponderServiceProvider::~CZPLinkResponderServiceProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
void
CZPLinkResponderServiceProvider::GetName(
	PMString* pName)
{
	ASSERT_UNIMPLEMENTED();
}

//----------------------------------------------------------------------------------------
// GetServiceID
//----------------------------------------------------------------------------------------
ServiceID
CZPLinkResponderServiceProvider::GetServiceID()
{
	return fServices.size() > 0 ? fServices[0] : kInvalidService;
}

//----------------------------------------------------------------------------------------
// IsDefaultServiceProvider
//----------------------------------------------------------------------------------------
bool16
CZPLinkResponderServiceProvider::IsDefaultServiceProvider()
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// GetInstantiationPolicy
//----------------------------------------------------------------------------------------
IK2ServiceProvider::InstancePerX
CZPLinkResponderServiceProvider::GetInstantiationPolicy()
{
	return IK2ServiceProvider::kInstancePerSession;
}

//----------------------------------------------------------------------------------------
// HasMultipleIDs
//----------------------------------------------------------------------------------------
bool16
CZPLinkResponderServiceProvider::HasMultipleIDs()const
{
	return kTrue;
}

//----------------------------------------------------------------------------------------
// GetServiceIDs
//----------------------------------------------------------------------------------------
void
CZPLinkResponderServiceProvider::GetServiceIDs(
	K2Vector<ServiceID>& serviceIDs)
{
	std::copy(this->fServices.begin(), this->fServices.end(), std::back_inserter(serviceIDs));
}
