//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeStatus/CZPAssetsTVStatusNodeData.cpp $
//	$Revision: 1429 $
//	$Date: 2010-06-15 06:22:30 +0200 (Tue, 15 Jun 2010) $
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

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUITVNodeData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVNodeData>
class CZPAssetsTVStatusNodeData : public inheritClass
{
public:
						CZPAssetsTVStatusNodeData(IPMUnknown * boss);
	virtual				~CZPAssetsTVStatusNodeData();

	const IPMUnknown *	GetDataObj() const;
							
	void				SetDataObj(
							const IPMUnknown *			inDataObj );

protected:
private:
	InterfacePtr<const IPMUnknown>	mData;
};


CREATE_PMINTERFACE(CZPAssetsTVStatusNodeData, kZPAssetsTVStatusNodeDataImpl)

CZPAssetsTVStatusNodeData::CZPAssetsTVStatusNodeData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVStatusNodeData::~CZPAssetsTVStatusNodeData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataObj
//----------------------------------------------------------------------------------------
const IPMUnknown *
CZPAssetsTVStatusNodeData::GetDataObj()const
{
	return mData;
}

//----------------------------------------------------------------------------------------
// SetDataObj
//----------------------------------------------------------------------------------------
void
CZPAssetsTVStatusNodeData::SetDataObj(
	const IPMUnknown *			inDataObj)
{
	if( inDataObj )
		inDataObj->AddRef();
	mData.reset(inDataObj);
}


