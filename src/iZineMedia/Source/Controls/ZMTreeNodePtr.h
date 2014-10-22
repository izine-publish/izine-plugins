//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/ZMTreeNodePtr.h $
//	$Revision: 2738 $
//	$Date: 2011-04-07 13:08:10 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZMTreeNodePtr_
#define _h_ZMTreeNodePtr_
#pragma once

//Copied from nodeid.h
template <class TNode>
class ZMTreeNodePtr
{
	public:
//	explicit TreeNodePtr(const NodeID& nodeID) { TNode::kNodeType == nodeID.GetNodeType() ? fNodeClass = static_cast<TNode*>(nodeID.Get()) : nil; }
//	explicit TreeNodePtr(const NodeID_rv& nodeID) { TNode::kNodeType == static_cast<TNode*>(nodeID.Get())->GetNodeType() ? fNodeClass = static_cast<TNode*>(nodeID.Relinquish()) : nil; }
	explicit ZMTreeNodePtr(const NodeID& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Get()); }
	explicit ZMTreeNodePtr(const NodeID_rv& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Relinquish()); }
//	explicit TreeNodePtr(const NodeID& nodeID);
//	explicit TreeNodePtr(const NodeID_rv& nodeID);
	TNode*	operator->() const { ASSERT_MSG(fNodeClass, "About to use nil TreeNodePtr!"); return fNodeClass; }
	operator TNode*() const { return fNodeClass; }
	
	void	reset(const NodeID& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Get()); }
	private:
		TNode*	fNodeClass;
};

#endif //_h_ZMTreeNodePtr_
