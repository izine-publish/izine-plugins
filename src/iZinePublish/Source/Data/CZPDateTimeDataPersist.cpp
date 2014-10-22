//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPDateTimeDataPersist.cpp $
//	$Revision: 3148 $
//	$Date: 2011-06-03 14:02:00 +0200 (Fri, 03 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 3-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDateTimeData.h"
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IDateTimeData>
class CZPDateTimeDataPersist : public inheritClass
{
public:
						CZPDateTimeDataPersist(IPMUnknown * boss);
	virtual				~CZPDateTimeDataPersist();

	void				Set( uint64 date );
	uint64				Get() const;
	PMString			GetAsString() const;

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
protected:

	uint64				localXferInt64(
							IPMStream *					stream,
							uint64 &					value);

private:
	uint64				mModifiedDate;
};


CREATE_PERSIST_PMINTERFACE(CZPDateTimeDataPersist, kZPDateTimeDataPersistImpl)

CZPDateTimeDataPersist::CZPDateTimeDataPersist(IPMUnknown * boss)
: inheritClass(boss)
, mModifiedDate(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDateTimeDataPersist::~CZPDateTimeDataPersist()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Set
//----------------------------------------------------------------------------------------
void
CZPDateTimeDataPersist::Set(
	uint64 date)
{
	PreDirty();
	mModifiedDate = date;
}

//----------------------------------------------------------------------------------------
// Get
//----------------------------------------------------------------------------------------
uint64
CZPDateTimeDataPersist::Get()const
{
	return mModifiedDate;
}

//----------------------------------------------------------------------------------------
// GetAsString
//----------------------------------------------------------------------------------------
PMString
CZPDateTimeDataPersist::GetAsString()const
{
	ASSERT_UNIMPLEMENTED();
	return kNullString;
}

//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPDateTimeDataPersist::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	mModifiedDate = localXferInt64(stream, (uint64 &)mModifiedDate);
}

//----------------------------------------------------------------------------------------
// localXferInt64
//----------------------------------------------------------------------------------------
uint64
CZPDateTimeDataPersist::localXferInt64(
	IPMStream *					stream,
	uint64 &					value)
{
	uint32 hiBits = ((uint32)(value >> 32));
	uint32 outH = stream->XferInt32((int32 &)hiBits);
	uint32 loBits = ((uint32)(value & 0xffffffff));
	uint32 outL = stream->XferInt32((int32 &)loBits);
	return ((uint64)outH << 32) + outL;
}
