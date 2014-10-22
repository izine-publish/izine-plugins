//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPAssetLocalInfo.cpp $
//	$Revision: 1627 $
//	$Date: 2010-07-04 11:28:27 +0200 (Sun, 04 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetLocalInfo.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetLocalInfo>
class CZPAssetLocalInfo : public inheritClass
{
public:
						CZPAssetLocalInfo(IPMUnknown * boss);
	virtual				~CZPAssetLocalInfo();

	int32				GetFileVersionNumber() const;
	
	void				SetFileVersionNumber(
							int32						inVersionNumber );

	bool16				GetFileIsLocallyModified() const;
	
	void				SetFileIsLocallyModified(
							bool16						inIsModified );

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
protected:
private:

	PMString	mFilePath;	//TODO: Is it required?
	bool16		mIsLocallyModified;
	int32		mVersionNumber;
};


CREATE_PERSIST_PMINTERFACE(CZPAssetLocalInfo, kZPAssetLocalInfoImpl)

CZPAssetLocalInfo::CZPAssetLocalInfo(IPMUnknown * boss)
: inheritClass(boss)
, mIsLocallyModified( kFalse )
, mVersionNumber(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetLocalInfo::~CZPAssetLocalInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetFileVersionNumber
//----------------------------------------------------------------------------------------
int32
CZPAssetLocalInfo::GetFileVersionNumber() const
{
	return mVersionNumber;
}

//----------------------------------------------------------------------------------------
// SetFileVersionNumber
//----------------------------------------------------------------------------------------
void
CZPAssetLocalInfo::SetFileVersionNumber(
	int32						inVersionNumber)
{
	PreDirty();
	mVersionNumber = inVersionNumber;
}

//----------------------------------------------------------------------------------------
// GetFileIsLocallyModified
//----------------------------------------------------------------------------------------
bool16
CZPAssetLocalInfo::GetFileIsLocallyModified() const
{
	return mIsLocallyModified;
}

//----------------------------------------------------------------------------------------
// SetFileIsLocallyModified
//----------------------------------------------------------------------------------------
void
CZPAssetLocalInfo::SetFileIsLocallyModified(
	bool16						inIsModified)
{
	PreDirty();
	mIsLocallyModified = inIsModified;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPAssetLocalInfo::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{

	mFilePath.ReadWrite( stream );
	stream->XferInt32( mVersionNumber );
	stream->XferBool( mIsLocallyModified );
}
