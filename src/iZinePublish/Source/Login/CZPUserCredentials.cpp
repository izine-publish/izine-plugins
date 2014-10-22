//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPUserCredentials.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPUserCredentials.h"

//IZP General includes
#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPUserCredentials : public CPMUnknown<IZPUserCredentials>
{
public:
				CZPUserCredentials(IPMUnknown * boss);
	virtual		~CZPUserCredentials();

	void						Copy(
									const IZPUserCredentials *	inValuesFrom );

	virtual const PMString &	GetUserName() const;
	virtual void				SetUserName( 
									const PMString &		inUserName);
	virtual void				SetUserName(
									const char *			inUserName);

	virtual const PMString &	GetPwd() const;
	virtual void				SetPwd( 
									const PMString &		inPwd);
	virtual void				SetPwd(
									const char *			inPwd);

	/** Persistence-related method */ 
	virtual void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
								{ CPMUnknown<IZPUserCredentials>::PreDirty( prop, allowModification ); }
	void ReadWrite(IPMStream *s, ImplementationID prop);
protected:
private:
	PMString		mUserName;
	PMString		mPwd;
};

class CZPUserCredentialsNonPersist : public CZPUserCredentials
{
public:
						CZPUserCredentialsNonPersist(IPMUnknown * boss);
	virtual				~CZPUserCredentialsNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZPUserCredentialsNonPersist, kZPUserCredentialsImpl)
CREATE_PERSIST_PMINTERFACE(CZPUserCredentials, kZPUserCredentialsPersistImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUserCredentialsNonPersist::CZPUserCredentialsNonPersist(IPMUnknown * boss)
:CZPUserCredentials(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUserCredentialsNonPersist::~CZPUserCredentialsNonPersist()
{
}

#pragma mark -


CZPUserCredentials::CZPUserCredentials(IPMUnknown * boss) 
: CPMUnknown<IZPUserCredentials>(boss)
{

}

CZPUserCredentials::~CZPUserCredentials()
{

}

//----------------------------------------------------------------------------------------
// GetUserName
//----------------------------------------------------------------------------------------
const PMString &
CZPUserCredentials::GetUserName(
	) const
{
	return mUserName;
}

//----------------------------------------------------------------------------------------
// SetUserName
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::SetUserName(
	const PMString &		inUserName) 
{
	PreDirty();
	mUserName = inUserName;
}

//----------------------------------------------------------------------------------------
// SetUserName
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::SetUserName(
	const char *			inUserName)
{
	PreDirty();
#if defined(InDnCS5) || defined(InDnCS5_5)
	mUserName.SetCString( inUserName );
#else
	mUserName.SetCString( inUserName, PMString::kNoTranslate );
#endif
}

//----------------------------------------------------------------------------------------
// GetPwd
//----------------------------------------------------------------------------------------
const PMString &
CZPUserCredentials::GetPwd(
	) const
{
	return mPwd;
}

//----------------------------------------------------------------------------------------
// SetPwd
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::SetPwd(
	const PMString &		inPwd)
{
	PreDirty();
	mPwd = inPwd;
}

//----------------------------------------------------------------------------------------
// SetPwd
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::SetPwd(
	const char *			inPwd)
{
	PreDirty();
#if defined(InDnCS5) || defined(InDnCS5_5)
	mPwd.SetCString( inPwd );
#else
	mPwd.SetCString( inPwd, PMString::kNoTranslate );
#endif
}

//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::ReadWrite(
	IPMStream *s, ImplementationID prop)
{
	mUserName.ReadWrite(s);
	mPwd.ReadWrite(s);
}


//----------------------------------------------------------------------------------------
// Copy
//----------------------------------------------------------------------------------------
void
CZPUserCredentials::Copy(
	const IZPUserCredentials *	inValuesFrom)
{
	PreDirty();
	this->mUserName		= inValuesFrom->GetUserName();
	this->mPwd			= inValuesFrom->GetPwd();
}


