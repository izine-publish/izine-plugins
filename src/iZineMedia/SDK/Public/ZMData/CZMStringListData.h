//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/CZMStringListData.h $
//	$Revision: 2738 $
//	$Date: 2011-04-07 13:08:10 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "IStringListData.h"
#include "ZMTypes.h"

#define inheritClass_ZPStringListData CPMUnknown<Interface>

//Note: Interface must be derived from IStringListData
template <class Interface>
class CZMStringListData_ : public inheritClass_ZPStringListData
{
public:
						CZMStringListData_(IPMUnknown * boss);
	virtual				~CZMStringListData_();

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
CZMStringListData_<Interface>::CZMStringListData_(IPMUnknown * boss)
: inheritClass_ZPStringListData(boss)
{
	IStringListData * selfPtr = this;	//used to force this class to be derived from IStringListData.
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
template <class Interface>
CZMStringListData_<Interface>::~CZMStringListData_()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
template <class Interface>
void
CZMStringListData_<Interface>::SetStringList(
	const PMStringList & newList)
{
	mStringList = newList;
}



//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
template <class Interface>
const PMStringList &
CZMStringListData_<Interface>::GetStringList()
{
	return mStringList;
}


//----------------------------------------------------------------------------------------
// RemoveItem
//----------------------------------------------------------------------------------------
template <class Interface>
void
CZMStringListData_<Interface>::RemoveItem(
	const PMString &			inVal)
{
	int32 itemIndex = mStringList.Location( inVal );
	if( itemIndex >= 0 )
	{
		mStringList.erase( mStringList.begin() + itemIndex );
	}
}



