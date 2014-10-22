//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSConfig.cpp $
//	$Revision: 3101 $
//	$Date: 2011-05-31 08:45:55 +0200 (Tue, 31 May 2011) $
//	$Author: rajkumar.sehrawat $
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
#include "IPMStream.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSConfig.h"

//IZP General includes
#include "ZPPresistDefaults.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPWSConfig : public CPMUnknown<IZPWSConfig>
{
public:
				CZPWSConfig(IPMUnknown * boss);
	virtual		~CZPWSConfig();
	
	void				Copy(
							const IZPWSConfig *			inValuesFrom );

	const PMString &	GetURL() const;
	void				SetURL(
							const PMString &			inURL);

	bool16				NeedsProxyServer() const;
	void				SetNeedsProxyServer(
							bool16						inNeedsProxy);

	int32				GetProxyPort() const;
	void				SetProxyPort(
							int32						inPort);

	const PMString &	GetProxyServer() const;
	void				SetProxyServer(
							const PMString &			inProxyServer);

	virtual void		PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
						{ CPMUnknown<IZPWSConfig>::PreDirty( prop, allowModification ); }
	void ReadWrite(IPMStream* stream, ImplementationID implementation);

protected:
private:
	PMString	mWSURL;

	bool16		mUseProxyServer;
	int32		mProxyPort;
	PMString	mProxyServer;
};

class CZPWSConfigNonPersist : public CZPWSConfig
{
public:
						CZPWSConfigNonPersist(IPMUnknown * boss);
	virtual				~CZPWSConfigNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZPWSConfigNonPersist, kZPWSConfigImpl)
CREATE_PERSIST_PMINTERFACE(CZPWSConfig, kZPWSConfigPersistImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPWSConfigNonPersist::CZPWSConfigNonPersist(IPMUnknown * boss)
: CZPWSConfig(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSConfigNonPersist::~CZPWSConfigNonPersist()
{

}

#pragma mark -

CZPWSConfig::CZPWSConfig(IPMUnknown * boss) :
CPMUnknown<IZPWSConfig>(boss)
, mUseProxyServer( kZPWSConfigDefault_UseProxy )
, mProxyPort( kZPWSConfigDefault_ProxyPort )
{
}

CZPWSConfig::~CZPWSConfig()
{
}

//template override to avoid assert message for non-persist impl
//template <>
//void CPMUnknown<IZPWSConfig>::PreDirty(ImplementationID prop, bool16 allowModification)
//{
//	::PreDirty(this, prop, allowModification);
//}

//----------------------------------------------------------------------------------------
// GetURL
//----------------------------------------------------------------------------------------
const PMString &	
CZPWSConfig::GetURL(
	) const
{
	return mWSURL;
}


//----------------------------------------------------------------------------------------
// SetURL
//----------------------------------------------------------------------------------------
void
CZPWSConfig::SetURL(
	const PMString &			inURL)
{
	PreDirty();
	mWSURL = inURL;
	//TODO: send message kZPWSConfigChanged
}


//----------------------------------------------------------------------------------------
// NeedsProxyServer
//----------------------------------------------------------------------------------------
bool16				
CZPWSConfig::NeedsProxyServer(
	) const
{
	return mUseProxyServer;
}


//----------------------------------------------------------------------------------------
// SetNeedsProxyServer
//----------------------------------------------------------------------------------------
void
CZPWSConfig::SetNeedsProxyServer(
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
CZPWSConfig::GetProxyPort(
						  )const
{
	return mProxyPort;
}


//----------------------------------------------------------------------------------------
// SetProxyPort
//----------------------------------------------------------------------------------------
void
CZPWSConfig::SetProxyPort(
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
CZPWSConfig::GetProxyServer(
							)const
{
	return mProxyServer;
}


//----------------------------------------------------------------------------------------
// SetProxyServer
//----------------------------------------------------------------------------------------
void
CZPWSConfig::SetProxyServer(
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
CZPWSConfig::ReadWrite(
	IPMStream* stream, ImplementationID implementation)
{
	mWSURL.ReadWrite(stream);

	stream->XferBool(mUseProxyServer);
	stream->XferInt32( mProxyPort );
	mProxyServer.ReadWrite( stream );
}

//----------------------------------------------------------------------------------------
// Copy
//----------------------------------------------------------------------------------------
void
CZPWSConfig::Copy(
	const IZPWSConfig *	inValuesFrom)
{
	PreDirty();
	this->mWSURL			= inValuesFrom->GetURL();
	this->mUseProxyServer	= inValuesFrom->NeedsProxyServer();
	this->mProxyServer		= inValuesFrom->GetProxyServer();
	this->mProxyPort		= inValuesFrom->GetProxyPort();
}


