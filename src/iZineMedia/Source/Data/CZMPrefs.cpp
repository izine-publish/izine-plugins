//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMPrefs.cpp $
//	$Revision: 2860 $
//	$Date: 2011-04-27 15:07:13 +0200 (Wed, 27 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Prefs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMPrefs>
class CZMPrefs : public inheritClass
{
public:
						CZMPrefs(IPMUnknown * boss);
	virtual				~CZMPrefs();

	const PMString &	GetRepositoryPath( ) const;
	void				SetRepositoryPath(
							const PMString &					inDuration );

	const PMString &	GetAdFileNamePrefix() const;
	void				SetAdFileNamePrefix(
									const PMString &			inAdFileNamePrefix );

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
	virtual void		PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
						{ inheritClass::PreDirty( prop, allowModification ); }
protected:
private:
	PMString	mRepoPath;
	PMString	mAdFileNamePrefix;
};

class CZMPrefsNonPersist : public CZMPrefs
{
public:
						CZMPrefsNonPersist(IPMUnknown * boss);
	virtual				~CZMPrefsNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZMPrefsNonPersist, kZMPrefsImpl)
CREATE_PERSIST_PMINTERFACE(CZMPrefs, kZMPrefsPersistImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZMPrefsNonPersist::CZMPrefsNonPersist(IPMUnknown * boss)
: CZMPrefs(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMPrefsNonPersist::~CZMPrefsNonPersist()
{

}

#pragma mark -


CZMPrefs::CZMPrefs(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMPrefs::~CZMPrefs()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetRepositoryPath
//----------------------------------------------------------------------------------------
const PMString &
CZMPrefs::GetRepositoryPath() const
{
	return mRepoPath;
}

//----------------------------------------------------------------------------------------
// SetRepositoryPath
//----------------------------------------------------------------------------------------
void
CZMPrefs::SetRepositoryPath(
	const PMString &					inPath)
{
	PreDirty();
	mRepoPath = inPath;
}

//----------------------------------------------------------------------------------------
// GetAdFileNamePrefix
//----------------------------------------------------------------------------------------
const PMString &
CZMPrefs::GetAdFileNamePrefix()const
{
	return mAdFileNamePrefix;
}

//----------------------------------------------------------------------------------------
// SetAdFileNamePrefix
//----------------------------------------------------------------------------------------
void
CZMPrefs::SetAdFileNamePrefix(
	const PMString &			inAdFileNamePrefix)
{
	PreDirty();
	mAdFileNamePrefix = inAdFileNamePrefix;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZMPrefs::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	mRepoPath.ReadWrite( stream );
	mAdFileNamePrefix.ReadWrite( stream );
}