//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSCallHelper.cpp $
//	$Revision: 1023 $
//	$Date: 2010-04-20 13:32:22 +0200 (Tue, 20 Apr 2010) $
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

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSCallHelper.h"

//IZP General includes
#include "AZPSoapRequest.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPWSCallHelper : public CPMUnknown<IZPWSCallHelper>
{
public:
				CZPWSCallHelper(IPMUnknown * boss);
	virtual		~CZPWSCallHelper();

	enRequestDiscardType	GetRequestDiscardType(
								const AZPSoapRequest *				inRequest ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPWSCallHelper, kZPWSCallHelperImpl)

CZPWSCallHelper::CZPWSCallHelper(IPMUnknown * boss) :
CPMUnknown<IZPWSCallHelper>(boss)
{

}

CZPWSCallHelper::~CZPWSCallHelper()
{

}

//----------------------------------------------------------------------------------------
// GetRequestDiscardType
//----------------------------------------------------------------------------------------
enRequestDiscardType
CZPWSCallHelper::GetRequestDiscardType(
	const AZPSoapRequest *				inRequest) const
{
	switch( inRequest->GetRequestType() )
	{
	case eReq_Login:
		return eReqDiscard_New;
	}
	return eReqDiscard_None;
}


