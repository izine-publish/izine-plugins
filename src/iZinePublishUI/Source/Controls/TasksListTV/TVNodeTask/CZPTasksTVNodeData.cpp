//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/TVNodeTask/CZPTasksTVNodeData.cpp $
//	$Revision: 2245 $
//	$Date: 2010-10-06 15:42:21 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-9-2010
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

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVNodeData>
class CZPTasksTVNodeData : public inheritClass
{
public:
						CZPTasksTVNodeData(IPMUnknown * boss);
	virtual				~CZPTasksTVNodeData();

	const IPMUnknown *	GetDataObj() const;
							
	void				SetDataObj(
							const IPMUnknown *			inDataObj );

protected:
private:
	InterfacePtr<const IPMUnknown>	mData;
};


CREATE_PMINTERFACE(CZPTasksTVNodeData, kZPTasksTVNodeDataImpl)

CZPTasksTVNodeData::CZPTasksTVNodeData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksTVNodeData::~CZPTasksTVNodeData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataObj
//----------------------------------------------------------------------------------------
const IPMUnknown *
CZPTasksTVNodeData::GetDataObj()const
{
	return mData;
}

//----------------------------------------------------------------------------------------
// SetDataObj
//----------------------------------------------------------------------------------------
void
CZPTasksTVNodeData::SetDataObj(
	const IPMUnknown *			inDataObj)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPUITasksTVNodeDataWillChangeMsg, IZPUITVNodeData::kDefaultIID, nil );
	
	if( inDataObj )
		inDataObj->AddRef();
	mData.reset( inDataObj );

	selfSubject->Change( kZPUITasksTVNodeDataChangedMsg, IZPUITVNodeData::kDefaultIID, nil );
	
}