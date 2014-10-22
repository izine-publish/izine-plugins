//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/ZPAssetsTVNodeID.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
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

//IZP General includes
#include "ZPAssetsTVNodeID.h"
#include "ZPAssetsTVStatusNodeID.h"
#include "ZPAssetsTVAssetNodeID.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_None	//enLMA_UIRepoTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/**	Factory method to create
	new object of this class
	@return	new instance of this class
 */
NodeID_rv
ZPAssetsTVNodeID::Create(
	enZPAssetTVNodeType inType )
{
	switch( inType )
	{
	case eTVNodeStatus:
		return new ZPAssetsTVStatusNodeID();
	case eTVNodeAsset:
		return new ZPAssetsTVAssetNodeID();
	case eTVNodeRoot:
	default:
		return new ZPAssetsTVNodeID();
	}
}

/**	Factory method to create new object of this class
	@param path [IN] specifies initial path
	@return  new instance of this class
 */
NodeID_rv
ZPAssetsTVNodeID::Create(
	enZPAssetTVNodeType inType, const PMString& inID)
{
	NodeID_rv toReturn = Create( inType );
	(static_cast<ZPAssetsTVNodeID*>(toReturn.Get()))->SetID( inID );
	return toReturn.Relinquish();
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZPAssetsTVNodeID::ZPAssetsTVNodeID()
{
}

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZPAssetsTVNodeID::ZPAssetsTVNodeID(const PMString & inID)
: mID( inID )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsTVNodeID::~ZPAssetsTVNodeID()
{
}

#pragma mark -


//----------------------------------------------------------------------------------------
// Compare
//----------------------------------------------------------------------------------------
int32
ZPAssetsTVNodeID::Compare(
	const NodeIDClass* nodeID) const
{
	//TODO: should we compare the type first?
	const ZPAssetsTVNodeID* fileNode = static_cast<const ZPAssetsTVNodeID*>(nodeID);
	ASSERT(nodeID);
	return (const_cast<ZPAssetsTVNodeID*>(this)->mID.Compare(kFalse, fileNode->mID));
}

//----------------------------------------------------------------------------------------
// Clone
//----------------------------------------------------------------------------------------
NodeIDClass*
ZPAssetsTVNodeID::Clone() const
{
	LogFunctionEnterExit;
	
	return ZPAssetsTVNodeID::Create( (enZPAssetTVNodeType)this->GetNodeType(), this->GetID()).Relinquish();
}

//----------------------------------------------------------------------------------------
// SetID
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeID::SetID(
	const PMString &			inID)
{
	mID = inID;
}



//----------------------------------------------------------------------------------------
// Read
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeID::Read(
	IPMStream*	stream)
{
	LogFunctionEnterExit;
	
	mID.ReadWrite(stream);
}


//----------------------------------------------------------------------------------------
// Write
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeID::Write(
	IPMStream*	stream) const
{
	LogFunctionEnterExit;
	
	(const_cast<ZPAssetsTVNodeID*>(this)->mID).ReadWrite(stream);
}

//	end, File:	ZPAssetsTVNodeID.cpp
