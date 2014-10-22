//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/ZPTasksTVNodeID.h $
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
#ifndef _h_ZPTasksTVNodeID_
#define _h_ZPTasksTVNodeID_
#pragma once

#include "NodeID.h"
#include "ZPTreeNodePtr.h"

typedef enum
{
	eTVNodeRoot = 1,
	eTVNodeTask,
}enZPTasksTVNodeType;

/** Represents a node in some notional file system; this could be a platform file
	system, or a file within a repository such as Perforce.
	Overriding this class lets us populate the tree with our own data type that
	represents whatever we want on each node.
*/
class ZPTasksTVNodeID : public NodeIDClass
{
	public:
		enum { kNodeType = eTVNodeTask };


		/**	Factory method to create
		new object of this class
			@return	new instance of this class
		 */
		static	NodeID_rv Create(
							enZPTasksTVNodeType inType );

		/**	Factory method to create new object of this class
			@param path [IN] specifies initial path
			@return  new instance of this class
		 */
		static	NodeID_rv Create(
							enZPTasksTVNodeType inType, const PMString& inID);
		
						ZPTasksTVNodeID();

						ZPTasksTVNodeID(
							const PMString & inID);

		/**	Destructor
		 */
		virtual			~ZPTasksTVNodeID();
		

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
		
		PMString		mID;
};
#endif //_h_ZPTasksTVNodeID_
