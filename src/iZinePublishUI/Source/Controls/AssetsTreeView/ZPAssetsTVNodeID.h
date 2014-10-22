//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/ZPAssetsTVNodeID.h $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Base class for Assets TreeView node ID
//========================================================================================
#ifndef _h_ZPAssetsTVNodeID_
#define _h_ZPAssetsTVNodeID_
#pragma once

#include "NodeID.h"
#include "ZPTreeNodePtr.h"

typedef enum 
{
	eTVNodeRoot = 1,
	eTVNodeStatus,
	eTVNodeAsset
}enZPAssetTVNodeType;

/** Represents a node in some notional file system; this could be a platform file
	system, or a file within a repository such as Perforce.
	Overriding this class lets us populate the tree with our own data type that
	represents whatever we want on each node.
*/
class ZPAssetsTVNodeID : public NodeIDClass
{
	public:
		enum { kNodeType = eTVNodeRoot };


		/**	Factory method to create
		new object of this class
			@return	new instance of this class
		 */
		static	NodeID_rv Create(
							enZPAssetTVNodeType inType );

		/**	Factory method to create new object of this class
			@param path [IN] specifies initial path
			@return  new instance of this class
		 */
		static	NodeID_rv Create(
							enZPAssetTVNodeType inType, const PMString& inID);
		
		/**	Destructor
		 */
		virtual			~ZPAssetsTVNodeID();
		

		/**	@return type of this node
		 */
		virtual	NodeType GetNodeType() const
							{ return kNodeType; }

		/**	Comparator function
			@param NodeID [IN] specifies the node that we should compare against
			@return  Comparison results
		 */
		virtual int32	Compare(
							const NodeIDClass* NodeID) const;

		/**	Create a copy of this
			@return
		 */
		virtual NodeIDClass* Clone() const;

		/**	Read this from the given stream
			@param stream [IN] specified stream
			@return
		 */
		virtual void	Read(
							IPMStream*					stream);

		/**	Write this to the given stream
			@param [OUT] stream
			@return
		 */
		virtual void	Write(
							IPMStream*					stream) const;

		/**	Accessor for the path associated with this node
			@return const PMString&
		 */
		const PMString& GetID() const 
							{ return mID; }
		void			SetID(
							const PMString &			inID );

	protected:
		// Note we're keeping the destructor private to force use of the factory methods
						ZPAssetsTVNodeID();

						ZPAssetsTVNodeID(
							const PMString & inID);

		PMString		mID;
};

#endif //_h_ZPAssetsTVNodeID_
