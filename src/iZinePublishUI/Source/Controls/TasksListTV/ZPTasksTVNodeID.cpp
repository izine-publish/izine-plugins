//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/ZPTasksTVNodeID.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
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
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "ZPTasksTVNodeID.h"

//IZP General includes

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_None	//enLMA_UIRepoTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )


class ZPTasksTVRootNodeID : public ZPTasksTVNodeID
{
public:
	enum { kNodeType = eTVNodeRoot };

						ZPTasksTVRootNodeID();
	virtual				~ZPTasksTVRootNodeID();

	NodeType			GetNodeType() const
							{ return kNodeType; }
protected:
private:
};


/**	Factory method to create
	new object of this class
	@return	new instance of this class
 */
NodeID_rv
ZPTasksTVNodeID::Create(
	enZPTasksTVNodeType inType )
{
	switch( inType )
	{
	case eTVNodeTask:
		return new ZPTasksTVNodeID();
	case eTVNodeRoot:
	default:
		return new ZPTasksTVRootNodeID();
	}
}

/**	Factory method to create new object of this class
	@param path [IN] specifies initial path
	@return  new instance of this class
 */
NodeID_rv
ZPTasksTVNodeID::Create(
	enZPTasksTVNodeType inType, const PMString& inID)
{
	NodeID_rv toReturn = Create( inType );
	(static_cast<ZPTasksTVNodeID*>(toReturn.Get()))->SetID( inID );
	return toReturn.Relinquish();
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZPTasksTVRootNodeID::ZPTasksTVRootNodeID()
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPTasksTVRootNodeID::~ZPTasksTVRootNodeID()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZPTasksTVNodeID::ZPTasksTVNodeID()
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPTasksTVNodeID::~ZPTasksTVNodeID()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Compare
//----------------------------------------------------------------------------------------
int32
ZPTasksTVNodeID::Compare(
	const NodeIDClass* nodeID) const
{
	//TODO: should we compare the type first?
	const ZPTasksTVNodeID* fileNode = static_cast<const ZPTasksTVNodeID*>(nodeID);
	ASSERT(nodeID);
	return (const_cast<ZPTasksTVNodeID*>(this)->mID.Compare(kFalse, fileNode->mID));
}

//----------------------------------------------------------------------------------------
// Clone
//----------------------------------------------------------------------------------------
NodeIDClass*
ZPTasksTVNodeID::Clone() const
{
	LogFunctionEnterExit;
	
	return ZPTasksTVNodeID::Create( (enZPTasksTVNodeType)this->GetNodeType(), this->GetID()).Relinquish();
}

//----------------------------------------------------------------------------------------
// SetID
//----------------------------------------------------------------------------------------
void
ZPTasksTVNodeID::SetID(
	const PMString &			inID)
{
	mID = inID;
}



//----------------------------------------------------------------------------------------
// Read
//----------------------------------------------------------------------------------------
void
ZPTasksTVNodeID::Read(
	IPMStream*	stream)
{
	LogFunctionEnterExit;
	
	mID.ReadWrite(stream);
}


//----------------------------------------------------------------------------------------
// Write
//----------------------------------------------------------------------------------------
void
ZPTasksTVNodeID::Write(
	IPMStream*	stream) const
{
	LogFunctionEnterExit;
	
	(const_cast<ZPTasksTVNodeID*>(this)->mID).ReadWrite(stream);
}
