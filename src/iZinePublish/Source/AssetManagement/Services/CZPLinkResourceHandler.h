//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResourceHandler.h $
//	$Revision: 3923 $
//	$Date: 2012-02-16 05:25:15 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 13-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "ILinkResourceHandler.h"

#define inheritClass CPMUnknown<ILinkResourceHandler>
class CZPLinkResourceHandler : public inheritClass
{
public:
						CZPLinkResourceHandler(IPMUnknown * boss);
	virtual				~CZPLinkResourceHandler();

	/**	@see ILinkResourceHandler::Init
	 */
	virtual bool Init(const UIDRef& ref, const URI& uri) { return true; }

	/**	@see ILinkResourceHandler::IsResourceURIValid
	 */
	virtual bool IsResourceURIValid(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::AreResourceIdsEquivalent
	 */
	virtual bool AreResourceIdsEquivalent(const ILinkResource::ResourceId& id1, const ILinkResource::ResourceId& id2) const;
	/**	@see ILinkResourceHandler::GetResourceDataType
	 */
	virtual FileTypeInfo GetResourceDataType(const UIDRef& ref, const URI& uri) const;

	/**	@see ILinkResourceHandler::GetShortResourceName
	 */
	virtual WideString GetShortResourceName(const UIDRef& ref, const URI& uri, bool bUIName) const;
	/**	@see ILinkResourceHandler::GetLongResourceName
	 */
	virtual WideString GetLongResourceName(const UIDRef& ref, const URI& uri, bool bUIName) const;

	/**	@see ILinkResourceHandler::GetResourceStateInfo
	 */
	virtual PMString GetResourceStateInfo(const UIDRef& ref, const URI& uri, ILinkResource::ResourceState state, ILinkResource::ResourceStoreState storeState) const { return PMString(); }

	/**	@see ILinkResourceHandler::AreStampsEquivalent
	 */
	virtual bool AreStampsEquivalent(const WideString& stamp1, const WideString& stamp2) const;

	/**	@see ILinkResourceHandler::CanReadResource
	 */
	virtual bool CanReadResource(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CanWriteResource
	 */
	virtual bool CanWriteResource(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CanReadWriteResource
	 */
	virtual bool CanReadWriteResource(const UIDRef& ref, const URI& uri) const;

	/**	@see ILinkResourceHandler::CanCreateResourceStream
	 */
	virtual bool CanCreateResourceStream(const UIDRef& ref, const URI& uri, ILinkResource::AccessMode mode) const;
	/**	@see ILinkResourceHandler::CreateResourceReadStream
	 */
	virtual IPMStream* CreateResourceReadStream(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CreateResourceWriteStream
	 */
	virtual IPMStream* CreateResourceWriteStream(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CreateResourceReadWriteStream
	 */
	virtual IPMStream* CreateResourceReadWriteStream(const UIDRef& ref, const URI& uri) const;

	/**	@see ILinkResourceHandler::CanCacheResource
	 */
	virtual bool CanCacheResource(const UIDRef& ref, const URI& uri) const { return true; }
	/**	@see ILinkResourceHandler::CanEmbedResource
	 */
	virtual bool CanEmbedResource(const UIDRef& ref, const URI& uri) const { return true; }
	/**	@see ILinkResourceHandler::CanUnembedResource
	 */
	virtual bool CanUnembedResource(const UIDRef& ref, const URI& uri) const { return true; }

	/**	
		@see ILinkResourceHandler::CanCopyToFile
	 */
	virtual bool CanCopyToFile(const UIDRef& ref, const URI& uri) const;

	/**	@see ILinkResourceHandler::CopyToFile
	 */
	virtual ErrorCode CopyToFile(const UIDRef& ref, const URI& uri, IDFile& file) const;

	/**	@see ILinkResourceHandler::CanEditResource
	 */
	virtual bool CanEditResource(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::EditResource
	 */
	virtual ErrorCode EditResource(const UIDRef& ref, const URI& uri, const AppInfo& appInfo, PMString* errorString) const;

	/**	@see ILinkResourceHandler::CanRevealResource
	 */
	virtual bool CanRevealResource(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::RevealResource
	 */
	virtual ErrorCode RevealResource(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CanRevealResourceInBridge
	 */
	virtual bool CanRevealResourceInBridge(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::RevealResourceInBridge
	 */
	virtual ErrorCode RevealResourceInBridge(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::CanRevealResourceInMiniBridge
	 */	
	virtual bool CanRevealResourceInMiniBridge(const UIDRef& ref, const URI& uri) const;
	/**	@see ILinkResourceHandler::RevealResourceInMiniBridge
	 */
	virtual ErrorCode RevealResourceInMiniBridge(const UIDRef& ref, const URI& uri) const;

protected:
private:
	// Prevent copy construction and assignment.
	CZPLinkResourceHandler(const CZPLinkResourceHandler&);
	CZPLinkResourceHandler& operator=(const CZPLinkResourceHandler&);
};