//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/ZMAdsTVNodeID.cpp $
//	$Revision: 2725 $
//	$Date: 2011-04-07 09:32:16 +0200 (Thu, 07 Apr 2011) $
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
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "ZMAdsTVNodeID.h"
#include "ZMAdsTVAdFileNodeID.h"
#include "ZMAdsTVAdNodeID.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_None	//enLMA_UIRepoTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/**	Factory method to create
	new object of this class
	@return	new instance of this class
 */
NodeID_rv
ZMAdsTVNodeID::Create(
	enZPAdsTVNodeType inType )
{
	switch( inType )
	{
	case eTVNodeAdFile:
		return new ZMAdsTVAdFileNodeID();
	case eTVNodeAd:
		return new ZMAdsTVAdNodeID();
	case eTVNodeRoot:
	default:
		return new ZMAdsTVNodeID();
	}
}

/**	Factory method to create new object of this class
	@param path [IN] specifies initial path
	@return  new instance of this class
 */
NodeID_rv
ZMAdsTVNodeID::Create(
	enZPAdsTVNodeType inType, const PMString& inID)
{
	NodeID_rv toReturn = Create( inType );
	(static_cast<ZMAdsTVNodeID*>(toReturn.Get()))->SetID( inID );
	return toReturn.Relinquish();
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZMAdsTVNodeID::ZMAdsTVNodeID()
{
}

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
ZMAdsTVNodeID::ZMAdsTVNodeID(const PMString & inID)
: mID( inID )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMAdsTVNodeID::~ZMAdsTVNodeID()
{
}

#pragma mark -


//----------------------------------------------------------------------------------------
// Compare
//----------------------------------------------------------------------------------------
int32
ZMAdsTVNodeID::Compare(
	const NodeIDClass* nodeID) const
{
	//TODO: should we compare the type first?
	const ZMAdsTVNodeID* fileNode = static_cast<const ZMAdsTVNodeID*>(nodeID);
	ASSERT(nodeID);
	return (const_cast<ZMAdsTVNodeID*>(this)->mID.Compare(kFalse, fileNode->mID));
}

//----------------------------------------------------------------------------------------
// Clone
//----------------------------------------------------------------------------------------
NodeIDClass*
ZMAdsTVNodeID::Clone() const
{
	LogFunctionEnterExit;
	
	return ZMAdsTVNodeID::Create( (enZPAdsTVNodeType)this->GetNodeType(), this->GetID()).Relinquish();
}

//----------------------------------------------------------------------------------------
// SetID
//----------------------------------------------------------------------------------------
void
ZMAdsTVNodeID::SetID(
	const PMString &			inID)
{
	mID = inID;
}



//----------------------------------------------------------------------------------------
// Read
//----------------------------------------------------------------------------------------
void
ZMAdsTVNodeID::Read(
	IPMStream*	stream)
{
	LogFunctionEnterExit;
	
	mID.ReadWrite(stream);
}


//----------------------------------------------------------------------------------------
// Write
//----------------------------------------------------------------------------------------
void
ZMAdsTVNodeID::Write(
	IPMStream*	stream) const
{
	LogFunctionEnterExit;
	
	(const_cast<ZMAdsTVNodeID*>(this)->mID).ReadWrite(stream);
}

//	end, File:	ZMAdsTVNodeID.cpp
