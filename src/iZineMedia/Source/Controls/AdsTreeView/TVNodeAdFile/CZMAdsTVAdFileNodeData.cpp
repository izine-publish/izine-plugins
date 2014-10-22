//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAdFile/CZMAdsTVAdFileNodeData.cpp $
//	$Revision: 2740 $
//	$Date: 2011-04-07 14:23:49 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

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
class CZMAdsTVAdFileNodeData : public inheritClass
{
public:
						CZMAdsTVAdFileNodeData(IPMUnknown * boss);
	virtual				~CZMAdsTVAdFileNodeData();

	const IPMUnknown *	GetDataObj() const;
							
	void				SetDataObj(
							const IPMUnknown *			inDataObj );

protected:
private:
	InterfacePtr<const IPMUnknown>	mData;
};


CREATE_PMINTERFACE(CZMAdsTVAdFileNodeData, kZMAdsTVAdFileNodeDataImpl)

CZMAdsTVAdFileNodeData::CZMAdsTVAdFileNodeData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVAdFileNodeData::~CZMAdsTVAdFileNodeData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataObj
//----------------------------------------------------------------------------------------
const IPMUnknown *
CZMAdsTVAdFileNodeData::GetDataObj()const
{
	return mData;
}

//----------------------------------------------------------------------------------------
// SetDataObj
//----------------------------------------------------------------------------------------
void
CZMAdsTVAdFileNodeData::SetDataObj(
	const IPMUnknown *			inDataObj)
{
	if( inDataObj )
		inDataObj->AddRef();
	mData.reset(inDataObj);
}
