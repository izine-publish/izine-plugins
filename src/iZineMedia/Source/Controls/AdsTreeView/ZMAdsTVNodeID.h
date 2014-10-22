//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/ZMAdsTVNodeID.h $
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
#ifndef _h_ZMAdsTVNodeID_
#define _h_ZMAdsTVNodeID_
#pragma once

#include "NodeID.h"
#include "ZMTreeNodePtr.h"

typedef enum 
{
	eTVNodeRoot = 1,
	eTVNodeAdFile,
	eTVNodeAd
}enZPAdsTVNodeType;

/** Represents a node in some notional file system; this could be a platform file
	system, or a file within a repository such as Perforce.
	Overriding this class lets us populate the tree with our own data type that
	represents whatever we want on each node.
*/
class ZMAdsTVNodeID : public NodeIDClass
{
	public:
		enum { kNodeType = eTVNodeRoot };


		/**	Factory method to create
		new object of this class
			@return	new instance of this class
		 */
		static	NodeID_rv Create(
							enZPAdsTVNodeType inType );

		/**	Factory method to create new object of this class
			@param path [IN] specifies initial path
			@return  new instance of this class
		 */
		static	NodeID_rv Create(
							enZPAdsTVNodeType inType, const PMString& inID);
		
		/**	Destructor
		 */
		virtual			~ZMAdsTVNodeID();
		

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
						ZMAdsTVNodeID();

						ZMAdsTVNodeID(
							const PMString & inID);

		PMString		mID;
};

#endif //_h_ZMAdsTVNodeID_
