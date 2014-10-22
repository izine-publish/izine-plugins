//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPUIDListData.cpp $
//	$Revision: 1626 $
//	$Date: 2010-07-04 11:27:11 +0200 (Sun, 04 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes
#include "UIDList.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPUIDListData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUIDListData>
class CZPUIDListData : public inheritClass
{
public:
						CZPUIDListData(IPMUnknown * boss);
	virtual				~CZPUIDListData();

	/**	Appends a UID of an object to the UID list of persistent list items.
	 * 
	 * 	NOTE: This method must be called from within a command, as it 
	 * 	internally dirties the database. Client code must process the 
	 * 	kPstLstNewDataCmdBoss instead of calling this method directly.
	 * 
	 * 	@param uid IN The UID of the object to be appened to the UID list.
	 * 	@see kPstLstNewDataCmdBoss 
	*/
	virtual void Append(const UID& uid);
	
	/**	Removes a UID of an object from the UID list of persistent list items.
	 * 
	 * 	NOTE: This method must be called from within a command, as it 
	 * 	internally dirties the database. Client code must process the 
	 * 	kPstLstDeleteDataCmdBoss instead of calling this method directly.
	 * 
	 * 	@param uid IN The UID of the object to be removed from the UID list.
	 * 	@see kPstLstDeleteDataCmdBoss
	*/
	virtual void Remove(const UID& uid);

	/** Gets the list of persistent list items maintained.
		@return UIDList maintained by this class.
	*/
	virtual const UIDList* GetUIDList() const;


	/**	Given an index, this routine will return the corresponding object's UIDRef.
	 * 	@param index IN The index of the object in the persistent item list.
	 * 	@return The UIDRef of the desired object.
	*/
	virtual UIDRef GetRef(int32 index);
	
	/** Gets the database associated with the persistent list.
		@return the database that stores the UID list 
	*/
	virtual IDataBase* GetDataBase();

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
protected:
private:
	UIDList* fUIDList;
	void ReadWriteUIDList(IPMStream* s, ImplementationID prop);
};


CREATE_PERSIST_PMINTERFACE(CZPUIDListData, kZPUIDListPersistImpl)

CZPUIDListData::CZPUIDListData(IPMUnknown * boss)
: inheritClass(boss)
, fUIDList(nil)
{
	fUIDList = new UIDList(::GetDataBase(this));
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIDListData::~CZPUIDListData()
{
	if (fUIDList != nil)
		delete fUIDList;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPUIDListData::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	this->ReadWriteUIDList( stream, implementation );
}

//----------------------------------------------------------------------------------------
// GetDataBase
//----------------------------------------------------------------------------------------
IDataBase*
CZPUIDListData::GetDataBase()
{
	return fUIDList->GetDataBase();
}

//----------------------------------------------------------------------------------------
// GetRef
//----------------------------------------------------------------------------------------
UIDRef
CZPUIDListData::GetRef(
	int32 index)
{
	UIDRef uidRef;
	if (index >= 0 && index < fUIDList->Length())
		uidRef = fUIDList->GetRef(index);
	return uidRef;
}

//----------------------------------------------------------------------------------------
// GetUIDList
//----------------------------------------------------------------------------------------
const UIDList*
CZPUIDListData::GetUIDList() const
{
	return fUIDList;
}

//----------------------------------------------------------------------------------------
// Append
//----------------------------------------------------------------------------------------
void
CZPUIDListData::Append(
	const UID& uid)
{
	PreDirty();
	fUIDList->Append(uid);
}

//----------------------------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------------------------
void
CZPUIDListData::Remove(
	const UID& uid)
{
	int32 index = fUIDList->Location(uid);
	if (index >= 0)
	{
		PreDirty();
		fUIDList->Remove(index);
	}
}

//----------------------------------------------------------------------------------------
// ReadWriteUIDList
//----------------------------------------------------------------------------------------
void
CZPUIDListData::ReadWriteUIDList(
	IPMStream* s, ImplementationID prop)
{
	int32 length = fUIDList->Length();
	s->XferInt32(length);

	if (s->IsReading())
	{
		fUIDList->Clear();
		fUIDList->Preallocate(length);

		for (int32 i = 0; i < length; i++)	
		{
			UID		u;
			fUIDList->Append(s->XferObject(u));
		}
	}
	else
	{
		for (int32 i = 0; i < length; i++)
			s->XferObject(fUIDList->At(i));
	}
}
