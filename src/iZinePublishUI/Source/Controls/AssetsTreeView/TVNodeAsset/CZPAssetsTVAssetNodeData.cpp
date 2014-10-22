//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVAssetNodeData.cpp $
//	$Revision: 2137 $
//	$Date: 2010-09-21 11:32:28 +0200 (Tue, 21 Sep 2010) $
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

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUITVNodeData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVNodeData>
class CZPAssetsTVAssetNodeData : public inheritClass
{
public:
						CZPAssetsTVAssetNodeData(IPMUnknown * boss);
	virtual				~CZPAssetsTVAssetNodeData();
	const IPMUnknown *	GetDataObj() const;
							
	void				SetDataObj(
							const IPMUnknown *			inDataObj );

protected:
private:
	InterfacePtr<const IPMUnknown>	mData;
};


CREATE_PMINTERFACE(CZPAssetsTVAssetNodeData, kZPAssetsTVAssetNodeDataImpl)

CZPAssetsTVAssetNodeData::CZPAssetsTVAssetNodeData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVAssetNodeData::~CZPAssetsTVAssetNodeData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataObj
//----------------------------------------------------------------------------------------
const IPMUnknown *
CZPAssetsTVAssetNodeData::GetDataObj()const
{
	return mData;
}

//----------------------------------------------------------------------------------------
// SetDataObj
//----------------------------------------------------------------------------------------
void
CZPAssetsTVAssetNodeData::SetDataObj(
	const IPMUnknown *			inDataObj)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPUIAssetTVNodeDataWillChangeMsg, IZPUITVNodeData::kDefaultIID, nil );
	
	if( inDataObj )
		inDataObj->AddRef();
	mData.reset( inDataObj );

	selfSubject->Change( kZPUIAssetTVNodeDataChangedMsg, IZPUITVNodeData::kDefaultIID, nil );
	
}

