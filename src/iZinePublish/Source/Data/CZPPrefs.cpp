//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPPrefs.cpp $
//	$Revision: 3534 $
//	$Date: 2011-07-25 09:28:27 +0200 (Mon, 25 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"
#include "IZPPrefs.h"

//IZP General includes
#include "ZPPresistDefaults.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Prefs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPPrefs>
class CZPPrefs : public inheritClass
{
public:
						CZPPrefs(IPMUnknown * boss);
	virtual				~CZPPrefs();

	int					GetAssetMonitorAutoUpdateDuration( ) const;	//In seconds
	void				SetAssetMonitorAutoUpdateDuration(
							int							inDuration );

	bool16				GetAutoUpdateLinkedStoriesContent( ) const;
	void				SetAutoUpdateLinkedStoriesContent(
							bool16						inAutoUpdate );

	bool16				GetAutoUnlockStories() const;
	void				SetAutoUnlockStories(
							bool16						inAutoUnlockStories );

	bool16				GetCreateAssignmentOnStoryImport() const;
	void				SetCreateAssignmentOnStoryImport(
							bool16						inCreateAssignmentOnStoryImport);

	bool16				GetIncludeAllAssignmentsOnDocumentCheckin() const;
	void				SetIncludeAllAssignmentsOnDocumentCheckin(
							bool16						inIncludeAllAssignmentsOnDocumentCheckin );

	bool16				GetRemoveStyles() const;
	void				SetRemoveStyles(
							bool16						inRemoveStyles );

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
	virtual void		PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
						{ inheritClass::PreDirty( prop, allowModification ); }
protected:
private:
	int32		mAssetMonitorAutoUpdateDuration;
	bool16		mAutoUpdateLinkedStoriesContent;
	bool16		mAutoUnlockStories;
	bool16		mCreateAssignmentOnStoryImport;
	bool16		mIncludeAllAssignmentsOnDocumentCheckin;
	bool16		mRemoveStyles;
};

class CZPPrefsNonPersist : public CZPPrefs
{
public:
						CZPPrefsNonPersist(IPMUnknown * boss);
	virtual				~CZPPrefsNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZPPrefsNonPersist, kZPPrefsImpl)
CREATE_PERSIST_PMINTERFACE(CZPPrefs, kZPPrefsPersistImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPPrefsNonPersist::CZPPrefsNonPersist(IPMUnknown * boss)
: CZPPrefs(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPPrefsNonPersist::~CZPPrefsNonPersist()
{

}

#pragma mark -


CZPPrefs::CZPPrefs(IPMUnknown * boss)
: inheritClass(boss)
, mAssetMonitorAutoUpdateDuration( kZPPrefDefault_AssetFetchInterval )
, mAutoUpdateLinkedStoriesContent( kZPPrefDefault_AutoUpdateLinkedStories )
, mAutoUnlockStories( kZPPrefDefault_AutoUnlockLinkedStories )
, mCreateAssignmentOnStoryImport( kZPPrefDefault_CreateAssignmentOnStoryImport )
, mIncludeAllAssignmentsOnDocumentCheckin( kZPPrefDefault_IncludeAllAssignmentsOnDocumentCheckin )
, mRemoveStyles( kZPPrefDefault_RemoveStyles )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPPrefs::~CZPPrefs()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetMonitorAutoUpdateDuration
//----------------------------------------------------------------------------------------
int
CZPPrefs::GetAssetMonitorAutoUpdateDuration() const	//In seconds
{
	return mAssetMonitorAutoUpdateDuration;
}

//----------------------------------------------------------------------------------------
// SetAssetMonitorAutoUpdateDuration
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetAssetMonitorAutoUpdateDuration(
	int							inDuration)
{
	PreDirty();
	mAssetMonitorAutoUpdateDuration = inDuration;
}

//----------------------------------------------------------------------------------------
// GetAutoUpdateLinkedStoriesContent
//----------------------------------------------------------------------------------------
bool16
CZPPrefs::GetAutoUpdateLinkedStoriesContent() const
{
	return mAutoUpdateLinkedStoriesContent;
}

//----------------------------------------------------------------------------------------
// SetAutoUpdateLinkedStoriesContent
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetAutoUpdateLinkedStoriesContent(
	bool16						inAutoUpdate)
{
	PreDirty();
	mAutoUpdateLinkedStoriesContent = inAutoUpdate;
}

//----------------------------------------------------------------------------------------
// GetAutoUnlockStories
//----------------------------------------------------------------------------------------
bool16
CZPPrefs::GetAutoUnlockStories()const
{
	if(! Utils<IZPInDnUtils>()->IsHostAppInCopy())
		return false;

	return mAutoUnlockStories;
}

//----------------------------------------------------------------------------------------
// SetAutoUnlockStories
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetAutoUnlockStories(
	bool16						inAutoUnlockStories)
{
	PreDirty();
	mAutoUnlockStories = inAutoUnlockStories;
}

//----------------------------------------------------------------------------------------
// GetCreateAssignmentOnStoryImport
//----------------------------------------------------------------------------------------
bool16
CZPPrefs::GetCreateAssignmentOnStoryImport()const
{
	if(! Utils<IZPInDnUtils>()->IsHostAppInDesign())
		return false;

	return mCreateAssignmentOnStoryImport;
}

//----------------------------------------------------------------------------------------
// SetCreateAssignmentOnStoryImport
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetCreateAssignmentOnStoryImport(
	bool16						inCreateAssignmentOnStoryImport)
{
	PreDirty();
	mCreateAssignmentOnStoryImport = inCreateAssignmentOnStoryImport;
}

//----------------------------------------------------------------------------------------
// GetIncludeAllAssignmentsOnDocumentCheckin
//----------------------------------------------------------------------------------------
bool16
CZPPrefs::GetIncludeAllAssignmentsOnDocumentCheckin()const
{
	if(! Utils<IZPInDnUtils>()->IsHostAppInDesign())
		return false;

	return mIncludeAllAssignmentsOnDocumentCheckin;
}

//----------------------------------------------------------------------------------------
// SetIncludeAllAssignmentsOnDocumentCheckin
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetIncludeAllAssignmentsOnDocumentCheckin(
	bool16					inIncludeAllAssignmentsOnDocumentCheckin)
{
	PreDirty();
	mIncludeAllAssignmentsOnDocumentCheckin = inIncludeAllAssignmentsOnDocumentCheckin;
}

//----------------------------------------------------------------------------------------
// GetRemoveStyles
//----------------------------------------------------------------------------------------
bool16
CZPPrefs::GetRemoveStyles()const
{
	if(! Utils<IZPInDnUtils>()->IsHostAppInCopy())
		return false;

	return mRemoveStyles;
}

//----------------------------------------------------------------------------------------
// SetRemoveStyles
//----------------------------------------------------------------------------------------
void
CZPPrefs::SetRemoveStyles(
	bool16						inRemoveStyles)
{
	PreDirty();
	mRemoveStyles = inRemoveStyles;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPPrefs::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	stream->XferInt32( mAssetMonitorAutoUpdateDuration );
	stream->XferBool( mAutoUpdateLinkedStoriesContent );
	stream->XferBool( mAutoUnlockStories );
	stream->XferBool( mCreateAssignmentOnStoryImport );
	stream->XferBool( mIncludeAllAssignmentsOnDocumentCheckin );
	stream->XferBool( mRemoveStyles );
}