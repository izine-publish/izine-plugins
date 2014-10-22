//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/App/CZPUIContentIteratorRegister.cpp $
//	$Revision: 1556 $
//	$Date: 2010-06-28 09:41:38 +0200 (Mon, 28 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CContentIteratorReg.h"
#include "K2Debugging.h"
#include "HelperInterface.h"
#include "SchemaBasedIterator.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

// List of the objects that we represent
//Required that save IDs
static const ImplementationID implTable[] =
								{
									// These are Schema-based iterators
									ImplementationID( kZPUIActionFilterDataImpl )
								};

#define inheritClass CContentIteratorRegister
class CZPUIContentIteratorRegister : public inheritClass
{
public:
						CZPUIContentIteratorRegister(IPMUnknown * boss);
	virtual				~CZPUIContentIteratorRegister();

	/** 
		Get the implementation ID count.
	*/
	virtual uint32		GetImplementationIDCount()
						{return sizeof(implTable) / sizeof(implTable[0]);}

	/** 
		Look through the implementation ID table to find the nth implementation ID.
	*/
	virtual void		GetNthImplementationID(
							uint32 n, ImplementationID& id);

	/** 
		Create a new schema based read iterator.
		@param version The version number of this persistent implementation on read data
	*/
	virtual IContentReadIterator* NewNthImplementationReadIterator(
							uint32 n, const VersionID& version, IPMStream* s);

	/**
		Create a new schema based write iterator.
		@param version The version number of this persistent implementation on write data
	*/
	virtual IContentWriteIterator* NewNthImplementationWriteIterator(
							uint32 n, const VersionID& version, IPMStream* s);

	DECLARE_HELPER_METHODS()
protected:
private:
};


CREATE_PMINTERFACE(CZPUIContentIteratorRegister, kZPUIContentIteratorRegisterImpl)

DEFINE_HELPER_METHODS(CZPUIContentIteratorRegister)


CZPUIContentIteratorRegister::CZPUIContentIteratorRegister(IPMUnknown * boss)
: HELPER_METHODS_INIT(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIContentIteratorRegister::~CZPUIContentIteratorRegister()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetNthImplementationID
//----------------------------------------------------------------------------------------
void
CZPUIContentIteratorRegister::GetNthImplementationID(
	uint32 n, ImplementationID& id)
{
	ASSERT_MSG((n < GetImplementationIDCount()), FORMAT_ARGS("CZPUIContentIteratorRegister::GetNthImplementationID() - Invalid index: %d", n));
	id = implTable[n];
}


//----------------------------------------------------------------------------------------
// NewNthImplementationReadIterator
//----------------------------------------------------------------------------------------
IContentReadIterator*
CZPUIContentIteratorRegister::NewNthImplementationReadIterator(
	uint32 n, const VersionID& version, IPMStream* s)
{
	ASSERT_MSG((n < GetImplementationIDCount()), FORMAT_ARGS("CZPUIContentIteratorRegister::NewNthImplementationReadIterator() - Invalid index: %d", n));

	return NewSchemaReadIterator(implTable[n], version, s);
}

//----------------------------------------------------------------------------------------
// NewNthImplementationWriteIterator
//----------------------------------------------------------------------------------------
IContentWriteIterator*
CZPUIContentIteratorRegister::NewNthImplementationWriteIterator(
	uint32 n, const VersionID& version, IPMStream* s)
{
	ASSERT_MSG((n < GetImplementationIDCount()), FORMAT_ARGS("CZPUIContentIteratorRegister::NewNthImplementationWriteIterator() - Invalid index: %d", n));

	return new SchemaBasedWriteIterator(s);
}

