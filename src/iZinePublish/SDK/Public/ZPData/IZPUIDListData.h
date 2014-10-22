//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPUIDListData.h $
//	$Revision: 1626 $
//	$Date: 2010-07-04 11:27:11 +0200 (Sun, 04 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
// Copied from CS3 SDK source/sdksamples/persistentlist/IPstLstUIDList.h
//========================================================================================
#ifndef _h_IZPUIDListData_
#define _h_IZPUIDListData_
#pragma once


class IZPUIDListData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUIDLISTDATA };

	/**
		@return UID list this class maintains.
	*/
	virtual const UIDList* GetUIDList() const = 0;
		
	/**
		Given an index, this routine will returns its corresponding UIDRef of the object as referenced by the
		index.
		@param index the index of the object in the UID list that the user is interested in.
		@return the UIDRef of the object that the user is inquiring about.
	*/
	virtual UIDRef GetRef(int32 index) = 0;
		
	/** 
		@return the database that stores the UID list 
	*/
	virtual IDataBase* GetDataBase() = 0;
		
	/**
		This function appends an uid of an object onto the UID list that it maintains.
		@param uid the UID of the object to be appened to the UID list.
	*/
	virtual void Append(const UID& uid) = 0;
		
	/**
		This function removes an uid of an object from the UID list that it maintains.
		@param uid the UID of the object to be removed from the UID list.
	*/
	virtual void Remove(const UID& uid) = 0;
};


//----------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------
#endif //_h_IZPUIDListData_
