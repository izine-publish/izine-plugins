//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/CZPStringListData.h $
//	$Revision: 2391 $
//	$Date: 2010-10-26 11:09:32 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "IStringListData.h"
#include "ZPTypes.h"

#define inheritClass_ZPStringListData CPMUnknown<Interface>

//Note: Interface must be derived from IStringListData
template <class Interface>
class CZPStringListData_ : public inheritClass_ZPStringListData
{
public:
						CZPStringListData_(IPMUnknown * boss);
	virtual				~CZPStringListData_();

	/** Set the value of the list of strings.
		@param newList - the list of strings that will be the new value
	*/
	virtual void SetStringList(const PMStringList& newList);

	/** Return the value of the list.
		@return the list of strings 
	*/
	virtual const PMStringList& GetStringList();

	virtual void		RemoveItem(
							const PMString &			inVal );
protected:
	PMStringList	mStringList;
private:
};

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
template <class Interface>
CZPStringListData_<Interface>::CZPStringListData_(IPMUnknown * boss)
: inheritClass_ZPStringListData(boss)
{
	IStringListData * selfPtr = this;	//used to force this class to be derived from IStringListData.
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <class Interface>
CZPStringListData_<Interface>::~CZPStringListData_()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
template <class Interface>
void
CZPStringListData_<Interface>::SetStringList(
	const PMStringList & newList)
{
	mStringList = newList;
}



//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
template <class Interface>
const PMStringList &
CZPStringListData_<Interface>::GetStringList()
{
	return mStringList;
}


//----------------------------------------------------------------------------------------
// RemoveItem
//----------------------------------------------------------------------------------------
template <class Interface>
void
CZPStringListData_<Interface>::RemoveItem(
	const PMString &			inVal)
{
	int32 taskItemIndex = mStringList.Location( inVal );
	if( taskItemIndex >= 0 )
	{
		mStringList.erase( mStringList.begin() + taskItemIndex );
	}
}



