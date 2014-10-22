//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/CZPAssetsTVHierarchyAdapter.cpp $
//	$Revision: 2434 $
//	$Date: 2010-11-02 09:06:55 +0100 (Tue, 02 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISession.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IWorkspace.h"

//SDK General includes
#include "CAlert.h"
#include "CPMUnknown.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPAssetsTVDataModel.h"

//IZP General includes
#include "ZPAssetsTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implementation of ITreeViewHierarchyAdapter, adapts our tree model to the needs of the
 tree-view widget. Implements an adapter for the tree hierarchy; this is where the hierarchy
 is accessed by the application core - you'll find that this is the only implementation
 you provide with methods like GetNumChildren or GetParent.
 
 For further reading on ITreeViewHiearchyAdapter interface, see the Widgets chapter
 of the User Interface technote.
*/
#define inheritClass CPMUnknown<ITreeViewHierarchyAdapter>
class ZPAssetsTVHierarchyAdapter : public inheritClass
{
public:
						ZPAssetsTVHierarchyAdapter(IPMUnknown * boss);
	virtual				~ZPAssetsTVHierarchyAdapter();

	virtual NodeID_rv	GetRootNode() const;
	virtual NodeID_rv	GetParentNode(const NodeID& node) const;
	virtual int32		GetNumChildren(const NodeID& node) const;
	virtual NodeID_rv	GetNthChild(const NodeID& node, const int32& nth) const;
	virtual int32		GetChildIndex(const NodeID& parent, const NodeID& child) const;
	virtual NodeID_rv	GetGenericNodeID() const;

protected:
private:
};


CREATE_PMINTERFACE(ZPAssetsTVHierarchyAdapter, kZPAssetsTVHierarchyAdapterImpl)

ZPAssetsTVHierarchyAdapter::ZPAssetsTVHierarchyAdapter(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsTVHierarchyAdapter::~ZPAssetsTVHierarchyAdapter()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetRootNode
//----------------------------------------------------------------------------------------
NodeID_rv
ZPAssetsTVHierarchyAdapter::GetRootNode() const
{
	// When we call ITreeViewMgr::ChangeRoot, then this is the first
	// method that gets called
	
	return ZPAssetsTVNodeID::Create( eTVNodeRoot );
}

//----------------------------------------------------------------------------------------
// GetParentNode
//----------------------------------------------------------------------------------------
NodeID_rv
ZPAssetsTVHierarchyAdapter::GetParentNode(const NodeID& node) const
{
	do
	{
		ZPTreeNodePtr<const ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;
		
		if( nodeID->GetNodeType() == eTVNodeRoot )			// The root has no parent: this will be a general result
			break;
		else if ( nodeID->GetNodeType() == eTVNodeStatus )	// Status node parent is root node
			return ZPAssetsTVNodeID::Create( eTVNodeRoot );
		else												// Find parent for Asset node
		{
			InterfacePtr<const IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
			ASSERT(selfModel);

			const PMString * statusID = selfModel->GetStatusIDOfAsset( nodeID->GetID() );
			if( statusID )
				return ZPAssetsTVNodeID::Create( eTVNodeStatus, *statusID );
		}
	} while(kFalse);
	
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetNumChildren
//----------------------------------------------------------------------------------------
int32
ZPAssetsTVHierarchyAdapter::GetNumChildren(const NodeID& node) const
{
	int32 retVal = 0;
	do
	{
		ZPTreeNodePtr< const ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for Asset node
		if( nodeID->GetNodeType() == eTVNodeAsset )
			return retVal;

		InterfacePtr<const IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return the number of status for current title
		if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const ZPStatusIDsList * statusList = selfModel->GetStatusIDList();
			if( statusList )
				return statusList->size();
			return 0;
		}

		ASSERT(nodeID->GetNodeType() == eTVNodeStatus);
		//Return the count of assets for current edition.
		const ZPAssetIDsList *	assetList = selfModel->GetAssetIDList( nodeID->GetID() );
		if( assetList )
			retVal = assetList->size();
	} while(kFalse);
	
	return retVal;
}

//----------------------------------------------------------------------------------------
// GetNthChild
//----------------------------------------------------------------------------------------
NodeID_rv
ZPAssetsTVHierarchyAdapter::GetNthChild( const NodeID& node, const int32& nth) const
{
	do
	{
		ZPTreeNodePtr< const ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for Asset node
		ASSERT( nodeID->GetNodeType() != eTVNodeAsset );
		
		InterfacePtr<const IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return nth status id
		if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const PMString * statusID = selfModel->GetNthStatusID( nth );
			ASSERT( statusID != nil );
			return ZPAssetsTVNodeID::Create( eTVNodeStatus, *statusID );
		}

		ASSERT(nodeID->GetNodeType() == eTVNodeStatus);
		//Return nth assets for current edition.
		
		const PMString * assetID = selfModel->GetNthAssetID( nodeID->GetID(), nth);
		//ASSERT( assetID != nil );	//Comes nil when login is done on different db.
		if( assetID )
			return ZPAssetsTVNodeID::Create( eTVNodeAsset, *assetID );
	} while(kFalse);
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetChildIndex
//----------------------------------------------------------------------------------------
int32
ZPAssetsTVHierarchyAdapter::GetChildIndex( const NodeID& parent, const NodeID& child) const
{
	do
	{
		ZPTreeNodePtr< const ZPAssetsTVNodeID> parentNodeID(parent);
		ASSERT(parentNodeID != nil);
		if (parentNodeID == nil)
			break;

		//No child for Asset node
		ASSERT( parentNodeID->GetNodeType() != eTVNodeAsset );
		
		ZPTreeNodePtr< const ZPAssetsTVNodeID> childNodeID(child);
		ASSERT(childNodeID != nil);
		if (childNodeID == nil)
			break;

		//Root cannot be child node
		ASSERT( childNodeID->GetNodeType() != eTVNodeRoot );

		InterfacePtr<const IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		const PMString & childNodeIDStr = childNodeID->GetID();
		int32 toReturn = -1;

		//Return the index of status for current title
		if( parentNodeID->GetNodeType() == eTVNodeRoot )
		{
			toReturn = selfModel->GetStatusIDIndex( childNodeIDStr );
			return toReturn;
		}

		ASSERT(parentNodeID->GetNodeType() == eTVNodeStatus);
		//Return the index of assets for current edition.

		toReturn = selfModel->GetAssetIDIndex( parentNodeID->GetID(), childNodeID->GetID() );
		return toReturn;
	} while(kFalse);
	
	return (-1);
}

//----------------------------------------------------------------------------------------
// GetGenericNodeID
//----------------------------------------------------------------------------------------
NodeID_rv
ZPAssetsTVHierarchyAdapter::GetGenericNodeID() const
{
	return ZPAssetsTVNodeID::Create( eTVNodeRoot );
}

