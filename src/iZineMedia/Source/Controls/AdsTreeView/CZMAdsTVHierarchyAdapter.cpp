//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/CZMAdsTVHierarchyAdapter.cpp $
//	$Revision: 2756 $
//	$Date: 2011-04-08 13:50:19 +0200 (Fri, 08 Apr 2011) $
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
#include "ISession.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IWorkspace.h"

//SDK General includes
#include "CAlert.h"
#include "CPMUnknown.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdsTVDataModel.h"

//IZP General includes
#include "ZMAdsTVNodeID.h"

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
class ZMAdsTVHierarchyAdapter : public inheritClass
{
public:
						ZMAdsTVHierarchyAdapter(IPMUnknown * boss);
	virtual				~ZMAdsTVHierarchyAdapter();

	virtual NodeID_rv	GetRootNode() const;
	virtual NodeID_rv	GetParentNode(const NodeID& node) const;
	virtual int32		GetNumChildren(const NodeID& node) const;
	virtual NodeID_rv	GetNthChild(const NodeID& node, const int32& nth) const;
	virtual int32		GetChildIndex(const NodeID& parent, const NodeID& child) const;
	virtual NodeID_rv	GetGenericNodeID() const;

protected:
private:
};


CREATE_PMINTERFACE(ZMAdsTVHierarchyAdapter, kZMAdsTVHierarchyAdapterImpl)

ZMAdsTVHierarchyAdapter::ZMAdsTVHierarchyAdapter(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMAdsTVHierarchyAdapter::~ZMAdsTVHierarchyAdapter()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetRootNode
//----------------------------------------------------------------------------------------
NodeID_rv
ZMAdsTVHierarchyAdapter::GetRootNode() const
{
	// When we call ITreeViewMgr::ChangeRoot, then this is the first
	// method that gets called
	
	return ZMAdsTVNodeID::Create( eTVNodeRoot );
}

//----------------------------------------------------------------------------------------
// GetParentNode
//----------------------------------------------------------------------------------------
NodeID_rv
ZMAdsTVHierarchyAdapter::GetParentNode(const NodeID& node) const
{
	do
	{
		ZMTreeNodePtr<const ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;
		
		if( nodeID->GetNodeType() == eTVNodeRoot )			// The root has no parent: this will be a general result
			break;
		else if ( nodeID->GetNodeType() == eTVNodeAdFile )	// Ad File node parent is root node
			return ZMAdsTVNodeID::Create( eTVNodeRoot );
		else												// Find parent for Ad node
		{
			InterfacePtr<const IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
			ASSERT(selfModel);

			const PMString * adFileID = selfModel->GetAdFileIDOfAd( nodeID->GetID() );
			if( adFileID )
				return ZMAdsTVNodeID::Create( eTVNodeAdFile, *adFileID );
		}
	} while(kFalse);
	
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetNumChildren
//----------------------------------------------------------------------------------------
int32
ZMAdsTVHierarchyAdapter::GetNumChildren(const NodeID& node) const
{
	int32 retVal = 0;
	do
	{
		ZMTreeNodePtr< const ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for Ad node
		if( nodeID->GetNodeType() == eTVNodeAd )
			return retVal;

		InterfacePtr<const IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return the number of adFile for current title
		if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const ZMAdFileIDsList * adFileList = selfModel->GetAdFileIDList();
			if( adFileList )
				return adFileList->size();
			return 0;
		}

		ASSERT(nodeID->GetNodeType() == eTVNodeAdFile);
		//Return the count of assets for current edition.
		const ZMAdIDsList *	assetList = selfModel->GetAdIDList( nodeID->GetID() );
		if( assetList )
			retVal = assetList->size();
	} while(kFalse);
	
	return retVal;
}

//----------------------------------------------------------------------------------------
// GetNthChild
//----------------------------------------------------------------------------------------
NodeID_rv
ZMAdsTVHierarchyAdapter::GetNthChild( const NodeID& node, const int32& nth) const
{
	do
	{
		ZMTreeNodePtr< const ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for Ad node
		ASSERT( nodeID->GetNodeType() != eTVNodeAd );
		
		InterfacePtr<const IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return nth adFile id
		if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const PMString * adFileID = selfModel->GetNthAdFileID( nth );
			ASSERT( adFileID != nil );
			return ZMAdsTVNodeID::Create( eTVNodeAdFile, *adFileID );
		}

		ASSERT(nodeID->GetNodeType() == eTVNodeAdFile);
		//Return nth assets for current edition.
		
		const PMString * assetID = selfModel->GetNthAdID( nodeID->GetID(), nth);
		//ASSERT( assetID != nil );	//Comes nil when login is done on different db.
		if( assetID )
			return ZMAdsTVNodeID::Create( eTVNodeAd, *assetID );
	} while(kFalse);
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetChildIndex
//----------------------------------------------------------------------------------------
int32
ZMAdsTVHierarchyAdapter::GetChildIndex( const NodeID& parent, const NodeID& child) const
{
	do
	{
		ZMTreeNodePtr< const ZMAdsTVNodeID> parentNodeID(parent);
		ASSERT(parentNodeID != nil);
		if (parentNodeID == nil)
			break;

		//No child for Ad node
		ASSERT( parentNodeID->GetNodeType() != eTVNodeAd );
		
		ZMTreeNodePtr< const ZMAdsTVNodeID> childNodeID(child);
		ASSERT(childNodeID != nil);
		if (childNodeID == nil)
			break;

		//Root cannot be child node
		ASSERT( childNodeID->GetNodeType() != eTVNodeRoot );

		InterfacePtr<const IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		const PMString & childNodeIDStr = childNodeID->GetID();
		int32 toReturn = -1;

		//Return the index of adFile for current title
		if( parentNodeID->GetNodeType() == eTVNodeRoot )
		{
			toReturn = selfModel->GetAdFileIDIndex( childNodeIDStr );
			return toReturn;
		}

		ASSERT(parentNodeID->GetNodeType() == eTVNodeAdFile);
		//Return the index of assets for current edition.

		toReturn = selfModel->GetAdIDIndex( parentNodeID->GetID(), childNodeID->GetID() );
		return toReturn;
	} while(kFalse);
	
	return (-1);
}

//----------------------------------------------------------------------------------------
// GetGenericNodeID
//----------------------------------------------------------------------------------------
NodeID_rv
ZMAdsTVHierarchyAdapter::GetGenericNodeID() const
{
	return ZMAdsTVNodeID::Create( eTVNodeRoot );
}

