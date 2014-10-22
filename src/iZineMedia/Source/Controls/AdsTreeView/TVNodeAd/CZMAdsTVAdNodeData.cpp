//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAd/CZMAdsTVAdNodeData.cpp $
//	$Revision: 2740 $
//	$Date: 2011-04-07 14:23:49 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMUITVNodeData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMUITVNodeData>
class CZMAdsTVAdNodeData : public inheritClass
{
public:
						CZMAdsTVAdNodeData(IPMUnknown * boss);
	virtual				~CZMAdsTVAdNodeData();
	const IPMUnknown *	GetDataObj() const;
							
	void				SetDataObj(
							const IPMUnknown *			inDataObj );

protected:
private:
	InterfacePtr<const IPMUnknown>	mData;
};


CREATE_PMINTERFACE(CZMAdsTVAdNodeData, kZMAdsTVAdNodeDataImpl)

CZMAdsTVAdNodeData::CZMAdsTVAdNodeData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVAdNodeData::~CZMAdsTVAdNodeData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataObj
//----------------------------------------------------------------------------------------
const IPMUnknown *
CZMAdsTVAdNodeData::GetDataObj()const
{
	return mData;
}

//----------------------------------------------------------------------------------------
// SetDataObj
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdNodeData::SetDataObj(
	const IPMUnknown *			inDataObj)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZMUIAdsTVNodeDataWillChangeMsg, IZMUITVNodeData::kDefaultIID, nil );
	
	if( inDataObj )
		inDataObj->AddRef();
	mData.reset( inDataObj );

	selfSubject->Change( kZMUIAdsTVNodeDataChangedMsg, IZMUITVNodeData::kDefaultIID, nil );
	
}

