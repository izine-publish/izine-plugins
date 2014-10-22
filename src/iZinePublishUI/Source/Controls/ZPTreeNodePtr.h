//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/ZPTreeNodePtr.h $
//	$Revision: 2169 $
//	$Date: 2010-09-27 14:08:14 +0200 (Mon, 27 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPTreeNodePtr_
#define _h_ZPTreeNodePtr_
#pragma once

//Copied from nodeid.h
template <class TNode>
class ZPTreeNodePtr
{
	public:
//	explicit TreeNodePtr(const NodeID& nodeID) { TNode::kNodeType == nodeID.GetNodeType() ? fNodeClass = static_cast<TNode*>(nodeID.Get()) : nil; }
//	explicit TreeNodePtr(const NodeID_rv& nodeID) { TNode::kNodeType == static_cast<TNode*>(nodeID.Get())->GetNodeType() ? fNodeClass = static_cast<TNode*>(nodeID.Relinquish()) : nil; }
	explicit ZPTreeNodePtr(const NodeID& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Get()); }
	explicit ZPTreeNodePtr(const NodeID_rv& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Relinquish()); }
//	explicit TreeNodePtr(const NodeID& nodeID);
//	explicit TreeNodePtr(const NodeID_rv& nodeID);
	TNode*	operator->() const { ASSERT_MSG(fNodeClass, "About to use nil TreeNodePtr!"); return fNodeClass; }
	operator TNode*() const { return fNodeClass; }
	
	void	reset(const NodeID& nodeID) { fNodeClass = static_cast<TNode*>(nodeID.Get()); }
	private:
		TNode*	fNodeClass;
};

#endif //_h_ZPTreeNodePtr_
