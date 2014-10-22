//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/ZPSmartPtrVector.h $
//	$Revision: 2657 $
//	$Date: 2011-04-04 09:32:57 +0200 (Mon, 04 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPSmartPtrVector_
#define _h_ZPSmartPtrVector_
#pragma once

#include <vector>

//The objects of class will not be passed to any function by val, it will by refrence/ptr
//Note: this class must be used carefully. If it owns the memory for items, then no item address must be retained at other place.
//If you retain the objects from source list, then delete the objects not being referenced, and remove ownership of source list.

#define ZPSmartPtrVectorBase std::vector<T *>

template <class T>
class ZPSmartPtrVector : public ZPSmartPtrVectorBase
{
public:
						ZPSmartPtrVector(
							bool						inOwnsItem = true)
						{	mIsOwner = inOwnsItem;	}
		virtual			~ZPSmartPtrVector()
						{	if( mIsOwner )
								DeleteData();
						}

						
		void			Forget() const
						{ mIsOwner = false; }
		void			SetOwnsItem()	//Use with caution. Mostly done on object with which data is inserted.
						{	mIsOwner = true; }
		void			Copy(
							const ZPSmartPtrVector &	inValFrom,
							bool						inTakeOwnership )
						{
							if( this == &inValFrom )
								return;

							if( this->mIsOwner )
								DeleteData();
							
							this->mIsOwner = inTakeOwnership;
							if( inTakeOwnership )
								inValFrom.mIsOwner = false;
							
							if( inValFrom.size() <= 0 )
								return;
							
							//Copy the pointers
							ZPSmartPtrVectorBase::operator=( inValFrom );

						}
		void			DeleteData()
						{
							ASSERT(mIsOwner);
							typename ZPSmartPtrVectorBase::iterator	currIter = ZPSmartPtrVectorBase::begin();
							typename ZPSmartPtrVectorBase::iterator	endIter = ZPSmartPtrVectorBase::end();
							while( currIter != endIter )
							{
								delete * currIter;
								++currIter;
							}
							ZPSmartPtrVectorBase::clear();
						}
protected:
						//No public copy constructor and assignment operator
						//Made so to avoid passing this as byval param.
						ZPSmartPtrVector ( const ZPSmartPtrVector & );
	ZPSmartPtrVector &	operator = ( const ZPSmartPtrVector & );
						
private:

	mutable bool		mIsOwner;
};

typedef ZPSmartPtrVector<PMString>	ZPPMStringSList;		//PMString smart/shared list

#endif //_h_ZPSmartPtrVector_
