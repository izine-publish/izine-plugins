//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Delegators/CZPStringDataDelegate.cpp $
//	$Revision: 2058 $
//	$Date: 2010-09-09 10:04:33 +0200 (Thu, 09 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"

//SDK General includes
#include "CPMUnknown.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL	//TODO: create a type for delegate
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IStringData>
class CZPStringDataDelegate : public inheritClass
{
public:
						CZPStringDataDelegate(IPMUnknown * boss);
	virtual				~CZPStringDataDelegate();

	/** Set the string value
		@param newString - new value of the string
	*/
	void Set(const PMString& newString);

	/** Return the string value.
		@return value of the string
	*/
	const PMString& GetString() const;
	
protected:
private:
	PMIID		mDelegateIID;
};


CREATE_PMINTERFACE(CZPStringDataDelegate, kZPStringDataDelegateImpl)

CZPStringDataDelegate::CZPStringDataDelegate(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZPStringDataDelegate )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPStringDataDelegate::~CZPStringDataDelegate()
{
}

#pragma mark -
/** Set the string value
	@param newString - new value of the string
*/
void
CZPStringDataDelegate::Set(
	const PMString& newString)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "string : %s", newString.GrabCString() );
	InterfacePtr<IStringData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( newString );
}

/** Return the string value.
	@return value of the string
*/
const PMString&
CZPStringDataDelegate::GetString()const
{
	LogFunctionEnterExit;
	InterfacePtr<const IStringData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		const PMString& toReturn = orgImpl->GetString(  );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %s", toReturn.GrabCString() );
		return toReturn;
	}
	return kNullString;
}
