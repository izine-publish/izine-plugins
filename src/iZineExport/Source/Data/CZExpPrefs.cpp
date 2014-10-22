//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/CZExpPrefs.cpp $
//	$Revision: 4010 $
//	$Date: 2012-05-22 13:56:23 +0200 (Tue, 22 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPrefs.h"

//IZP General includes
#include "ZExpPresistDefaults.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Prefs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpPrefs>
class CZExpPrefs : public inheritClass
{
public:
						CZExpPrefs(IPMUnknown * boss);
	virtual				~CZExpPrefs();

	const PMString &			GetUserName() const;
	void						SetUserName(
									const PMString &			inUserName);

	const PMString &			GetPassword() const;
	void						SetPassword(
									const PMString &			inPassword);

	const PMString &			GetPluginId() const;
	void						SetPluginId(
									const PMString &			inPluginId);

	bool16						NeedsProxyServer() const;
	void						SetNeedsProxyServer(
									bool16						inNeedsProxy);

	int32						GetProxyPort() const;
	void						SetProxyPort(
									int32						inPort);

	const PMString &			GetProxyServer() const;
	void						SetProxyServer(
									const PMString &			inProxyServer);

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
	virtual void		PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
						{ inheritClass::PreDirty( prop, allowModification ); }
protected:
private:
	PMString		mUserName;
	PMString		mPassword;
	PMString		mPluginId;

	bool16			mUseProxyServer;
	int32			mProxyPort;
	PMString		mProxyServer;
};

class CZExpPrefsNonPersist : public CZExpPrefs
{
public:
						CZExpPrefsNonPersist(IPMUnknown * boss);
	virtual				~CZExpPrefsNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZExpPrefsNonPersist, kZExpPrefsImpl)
CREATE_PERSIST_PMINTERFACE(CZExpPrefs, kZExpPrefsPersistImpl)

//----------------------------------------------------------------------------------------
// CZExpPrefsNonPersist
//----------------------------------------------------------------------------------------
CZExpPrefsNonPersist::CZExpPrefsNonPersist(IPMUnknown * boss)
: CZExpPrefs(boss)
{

}

//----------------------------------------------------------------------------------------
// CZExpPrefsNonPersist
//----------------------------------------------------------------------------------------
CZExpPrefsNonPersist::~CZExpPrefsNonPersist()
{

}


CZExpPrefs::CZExpPrefs(IPMUnknown * boss)
: inheritClass(boss)
,mUserName("")
,mPassword("")
,mPluginId("")
,mUseProxyServer( kZExpWSConfigDefault_UseProxy )
,mProxyPort( kZExpWSConfigDefault_ProxyPort )
,mProxyServer("")
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpPrefs::~CZExpPrefs()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetUserName
//----------------------------------------------------------------------------------------
const PMString &
CZExpPrefs::GetUserName()const
{
	return mUserName;
}

//----------------------------------------------------------------------------------------
// SetUserName
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetUserName(
	const PMString &			inUserName)
{
	PreDirty();
	mUserName = inUserName;
}

//----------------------------------------------------------------------------------------
// GetPassword
//----------------------------------------------------------------------------------------
const PMString &
CZExpPrefs::GetPassword()const
{
	return mPassword;
}

//----------------------------------------------------------------------------------------
// SetPassword
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetPassword(
	const PMString &			inPassword)
{
	PreDirty();
	mPassword = inPassword;
}

//----------------------------------------------------------------------------------------
// GetPluginId
//----------------------------------------------------------------------------------------
const PMString &
CZExpPrefs::GetPluginId()const
{
	return mPluginId;
}

//----------------------------------------------------------------------------------------
// SetPluginId
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetPluginId(
	const PMString &			inPluginId)
{
	PreDirty();
	mPluginId = inPluginId;
}

//----------------------------------------------------------------------------------------
// NeedsProxyServer
//----------------------------------------------------------------------------------------
bool16				
CZExpPrefs::NeedsProxyServer(
	) const
{
	return mUseProxyServer;
}


//----------------------------------------------------------------------------------------
// SetNeedsProxyServer
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetNeedsProxyServer(
	bool16						inNeedsProxy)
{
	PreDirty();
	mUseProxyServer = inNeedsProxy;
	//TODO: send message kZPWSConfigChanged
}


//----------------------------------------------------------------------------------------
// GetProxyPort
//----------------------------------------------------------------------------------------
int32
CZExpPrefs::GetProxyPort(
						  )const
{
	return mProxyPort;
}


//----------------------------------------------------------------------------------------
// SetProxyPort
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetProxyPort(
	int32						inPort)
{
	PreDirty();
	mProxyPort = inPort;
	//TODO: send message kZPWSConfigChanged
}


//----------------------------------------------------------------------------------------
// GetProxyServer
//----------------------------------------------------------------------------------------
const PMString &
CZExpPrefs::GetProxyServer(
							)const
{
	return mProxyServer;
}


//----------------------------------------------------------------------------------------
// SetProxyServer
//----------------------------------------------------------------------------------------
void
CZExpPrefs::SetProxyServer(
	const PMString &			inProxyServer)
{
	PreDirty();
	mProxyServer = inProxyServer;
	//TODO: send message kZPWSConfigChanged
}

//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZExpPrefs::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	mUserName.ReadWrite(stream);
	mPassword.ReadWrite(stream);
	mPluginId.ReadWrite(stream);

	stream->XferBool(mUseProxyServer);
	stream->XferInt32( mProxyPort );
	mProxyServer.ReadWrite( stream );
}
