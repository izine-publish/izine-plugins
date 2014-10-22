//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPGlobalTempData.cpp $
//	$Revision: 3666 $
//	$Date: 2011-11-23 09:17:50 +0100 (Wed, 23 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPGlobalTempData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPGlobalTempData>
class CZPGlobalTempData : public inheritClass
{
public:
						CZPGlobalTempData(IPMUnknown * boss);
	virtual				~CZPGlobalTempData();

	const PMString &	GetLastDraggedAssetID() const;
	void				SetLastDraggedAssetID(
							const PMString &			inLastDraggedAssetID );

	const PMString &	GetAssignmentNewStatusID() const;
	void				SetAssignmentNewStatusID(
							const PMString &			inAssignmentNewStatusID );

	const bool			GetRemoveStoryNameOnUnlink() const;
	void				SetRemoveStoryNameOnUnlink(
							const bool					inRemoveStoryNameOnUnlink );

	const bool			GetSkipAddLink() const;
	void				SetSkipAddLink(
									const bool					inSkipAddLink);
	const UIDRef		GetStoryUIDRefToSkipAddLink() const;
	void				SetStoryUIDRefToSkipAddLink(
									const UIDRef				inStoryUIDRefToSkipAddLink);

	const int			GetSkipAddLinkCount() const;
	void				SetSkipAddLinkCount(
									const int					inSkipAddLinkCount);

protected:
private:
	PMString	mLastDraggedAssetID;
	PMString	mAssignmentNewStatusID;		//Used during story checkin for updating assignment status
	bool		mRemoveStoryNameOnUnlink;	//Patch for update contents
	bool		mSkipAddLink;				//Patch for igore add link on paste
	UIDRef		mStoryUIDRefToSkipAddLink;
	int			mSkipAddLinkCount;
};


CREATE_PMINTERFACE(CZPGlobalTempData, kZPGlobalTempDataImpl)

CZPGlobalTempData::CZPGlobalTempData(IPMUnknown * boss)
: inheritClass(boss)
, mRemoveStoryNameOnUnlink( true )
, mSkipAddLink( false )
, mStoryUIDRefToSkipAddLink( UIDRef::gNull )
, mSkipAddLinkCount( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGlobalTempData::~CZPGlobalTempData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLastDraggedAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPGlobalTempData::GetLastDraggedAssetID()const
{
	return mLastDraggedAssetID;
}

//----------------------------------------------------------------------------------------
// SetLastDraggedAssetID
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetLastDraggedAssetID(
	const PMString &			inLastDraggedAssetID)
{
	mLastDraggedAssetID = inLastDraggedAssetID;
}

//----------------------------------------------------------------------------------------
// GetAssignmentNewStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPGlobalTempData::GetAssignmentNewStatusID()const
{
	return mAssignmentNewStatusID;
}

//----------------------------------------------------------------------------------------
// SetAssignmentNewStatusID
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetAssignmentNewStatusID(
	const PMString &			inAssignmentNewStatusID)
{
	mAssignmentNewStatusID = inAssignmentNewStatusID;
}

//----------------------------------------------------------------------------------------
// GetRemoveStoryNameOnUnlink
//----------------------------------------------------------------------------------------
const bool
CZPGlobalTempData::GetRemoveStoryNameOnUnlink()const
{
//	LogFunctionEnterExit;
	return mRemoveStoryNameOnUnlink;
}

//----------------------------------------------------------------------------------------
// SetRemoveStoryNameOnUnlink
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetRemoveStoryNameOnUnlink(
	const bool					inRemoveStoryNameOnUnlink)
{
//	LogFunctionEnterExit;
	mRemoveStoryNameOnUnlink = inRemoveStoryNameOnUnlink;
}

//----------------------------------------------------------------------------------------
// GetSkipAddLink
//----------------------------------------------------------------------------------------
const bool
CZPGlobalTempData::GetSkipAddLink()const
{
//	LogFunctionEnterExit;
	return mSkipAddLink;
}

//----------------------------------------------------------------------------------------
// SetSkipAddLink
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetSkipAddLink(
	const bool					inSkipAddLink)
{
//	LogFunctionEnterExit;
	mSkipAddLink = inSkipAddLink;
}

//----------------------------------------------------------------------------------------
// GetStoryUIDRefToSkipAddLink
//----------------------------------------------------------------------------------------
const UIDRef
CZPGlobalTempData::GetStoryUIDRefToSkipAddLink()const
{
	return mStoryUIDRefToSkipAddLink;
}

//----------------------------------------------------------------------------------------
// SetStoryUIDRefToSkipAddLink
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetStoryUIDRefToSkipAddLink(
	const UIDRef				inStoryUIDRefToSkipAddLink)
{
	mStoryUIDRefToSkipAddLink = inStoryUIDRefToSkipAddLink;
}

//----------------------------------------------------------------------------------------
// GetSkipAddLinkCount
//----------------------------------------------------------------------------------------
const int
CZPGlobalTempData::GetSkipAddLinkCount()const
{
	return mSkipAddLinkCount;
}

//----------------------------------------------------------------------------------------
// SetSkipAddLinkCount
//----------------------------------------------------------------------------------------
void
CZPGlobalTempData::SetSkipAddLinkCount(
	const int					inSkipAddLinkCount)
{
	mSkipAddLinkCount = inSkipAddLinkCount;
}
