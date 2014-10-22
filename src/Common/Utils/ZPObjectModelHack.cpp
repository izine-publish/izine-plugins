//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPObjectModelHack.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IClassInfo.h"
#include "IObjectModel.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "ZPObjectModelHack.h"

#pragma mark -
struct IIDImplInfo
{
	int32 unused;
	int32 mIntefaceIID;
	int32 mImplID;
	int32 mPluginID;
	int32 unused2;
};

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZPObjectModelHack::ZPObjectModelHack()
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPObjectModelHack::~ZPObjectModelHack()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SwapImpls
//----------------------------------------------------------------------------------------
void
ZPObjectModelHack::SwapImpls(
	const ClassID				inClassID,
	PMIID						inInterface1,
	PMIID						inInterface2,
	ImplementationID			inInterface2Impl)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IObjectModel> sessionObjectModel( gSession, UseDefaultIID() );
	ASSERT( sessionObjectModel );

	InterfacePtr<IClassInfo> theClassInfo( sessionObjectModel->QueryClassInfo( inClassID ) );
	ASSERT( theClassInfo );
	if( !theClassInfo )
		return;

	ImplementationID interface1ImplID = theClassInfo->GetImplementation( inInterface1 );
	ImplementationID interface2ImplID = theClassInfo->GetImplementation( inInterface2 );

	if( interface1ImplID == kInvalidImpl || interface2ImplID == kInvalidImpl )
		return;		//Resource not set properly.

	if( inInterface2Impl != kInvalidImpl && inInterface2Impl == interface2ImplID )
		return;		//Already set what is required.

	//else swap is required.

	int32 interface1Index = theClassInfo->GetIndex( inInterface1 );
	int32 interface2Index = theClassInfo->GetIndex( inInterface2 );
	int32 interface1Offset = theClassInfo->GetOffset( interface1Index );
	int32 interface2Offset = theClassInfo->GetOffset( interface2Index );

	//Swap the interface IIDs not the impls.
	int32 * bossInfoBaseAddr = (int32*) theClassInfo.get();
	bossInfoBaseAddr = bossInfoBaseAddr + 1;	//Increament by 4

	IIDImplInfo * structBaseAddr = (IIDImplInfo*) (bossInfoBaseAddr + 12*5 );

	IIDImplInfo * info1Addr = structBaseAddr + interface1Index;
	IIDImplInfo * info2Addr = structBaseAddr + interface2Index;

	info1Addr->mIntefaceIID = inInterface2.Get();
	info2Addr->mIntefaceIID = inInterface1.Get();

}
