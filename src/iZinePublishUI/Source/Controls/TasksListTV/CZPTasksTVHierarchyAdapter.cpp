//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/CZPTasksTVHierarchyAdapter.cpp $
//	$Revision: 2173 $
//	$Date: 2010-09-27 15:31:34 +0200 (Mon, 27 Sep 2010) $
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
#include "ITreeViewHierarchyAdapter.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTasksTVDataModel.h"

//IZP General includes
#include "ZPTasksTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ITreeViewHierarchyAdapter>
class CZPTasksTVHierarchyAdapter : public inheritClass
{
public:
						CZPTasksTVHierarchyAdapter(IPMUnknown * boss);
	virtual				~CZPTasksTVHierarchyAdapter();

	NodeID_rv			GetRootNode() const;
	NodeID_rv			GetParentNode(
							const NodeID& node) const;
	int32				GetNumChildren(
							const NodeID& node) const;
	NodeID_rv			GetNthChild(
							const NodeID& node,
							const int32& nth) const;
	int32				GetChildIndex(
							const NodeID& parent,
							const NodeID& child) const;
	NodeID_rv			GetGenericNodeID() const;

protected:
private:
};


CREATE_PMINTERFACE(CZPTasksTVHierarchyAdapter, kZPTasksTVHierarchyAdapterImpl)

CZPTasksTVHierarchyAdapter::CZPTasksTVHierarchyAdapter(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksTVHierarchyAdapter::~CZPTasksTVHierarchyAdapter()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetRootNode
//----------------------------------------------------------------------------------------
NodeID_rv
CZPTasksTVHierarchyAdapter::GetRootNode() const
{
	// When we call ITreeViewMgr::ChangeRoot, then this is the first
	// method that gets called
	
	return ZPTasksTVNodeID::Create( eTVNodeRoot );
}

//----------------------------------------------------------------------------------------
// GetParentNode
//----------------------------------------------------------------------------------------
NodeID_rv
CZPTasksTVHierarchyAdapter::GetParentNode(const NodeID& node) const
{
	do
	{
		ZPTreeNodePtr<const ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;
		
		if( nodeID->GetNodeType() == eTVNodeRoot )			// The root has no parent: this will be a general result
			break;
		else if ( nodeID->GetNodeType() == eTVNodeTask )	// Task node parent is root node
			return ZPTasksTVNodeID::Create( eTVNodeRoot );
	} while(kFalse);
	
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetNumChildren
//----------------------------------------------------------------------------------------
int32
CZPTasksTVHierarchyAdapter::GetNumChildren(const NodeID& node) const
{
	int32 retVal = 0;
	do
	{
		ZPTreeNodePtr< const ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for Task node
		if( nodeID->GetNodeType() == eTVNodeTask )
			return retVal;

		InterfacePtr<const IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return the number of task for current title
		if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const ZPTaskIDsList * taskList = selfModel->GetTaskIDList();
			if( taskList )
				return taskList->size();
			return 0;
		}

	} while(kFalse);
	
	return retVal;
}

//----------------------------------------------------------------------------------------
// GetNthChild
//----------------------------------------------------------------------------------------
NodeID_rv
CZPTasksTVHierarchyAdapter::GetNthChild( const NodeID& node, const int32& nth) const
{
	do
	{
		ZPTreeNodePtr< const ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		//No child for task node
		ASSERT( nodeID->GetNodeType() != eTVNodeTask );
		
		InterfacePtr<const IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		//Return nth task id
		//Commented check as we have only one type of node
		//if( nodeID->GetNodeType() == eTVNodeRoot )
		{
			const PMString * taskID = selfModel->GetNthTaskID( nth );
			ASSERT( taskID != nil );
			return ZPTasksTVNodeID::Create( eTVNodeTask, *taskID );
		}
	} while(kFalse);
	return kInvalidNodeID;
}

//----------------------------------------------------------------------------------------
// GetChildIndex
//----------------------------------------------------------------------------------------
int32
CZPTasksTVHierarchyAdapter::GetChildIndex( const NodeID& parent, const NodeID& child) const
{
	do
	{
		ZPTreeNodePtr< const ZPTasksTVNodeID> parentNodeID(parent);
		ASSERT(parentNodeID != nil);
		if (parentNodeID == nil)
			break;

		//No child for task node
		ASSERT( parentNodeID->GetNodeType() != eTVNodeTask );
		
		ZPTreeNodePtr< const ZPTasksTVNodeID> childNodeID(child);
		ASSERT(childNodeID != nil);
		if (childNodeID == nil)
			break;

		//Root cannot be child node
		ASSERT( childNodeID->GetNodeType() != eTVNodeRoot );

		InterfacePtr<const IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);

		const PMString & childNodeIDStr = childNodeID->GetID();
		int32 toReturn = -1;

		//Return the index of task for current title
		//Commented check as we have only one type of node
		//if( parentNodeID->GetNodeType() == eTVNodeRoot )
		{
			toReturn = selfModel->GetTaskIDIndex( childNodeIDStr );
			return toReturn;
		}
	} while(kFalse);
	
	return (-1);
}

//----------------------------------------------------------------------------------------
// GetGenericNodeID
//----------------------------------------------------------------------------------------
NodeID_rv
CZPTasksTVHierarchyAdapter::GetGenericNodeID() const
{
	return ZPTasksTVNodeID::Create( eTVNodeRoot );
}

