//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPSoapResponse.cpp $
//	$Revision: 1326 $
//	$Date: 2010-06-04 09:17:48 +0200 (Fri, 04 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPSoapResponse.h"

//IZP General includes
#include "AZPSoapResponse.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPSoapResponse>
class CZPSoapResponse : public inheritClass
{
public:
						CZPSoapResponse(IPMUnknown * boss);
	virtual				~CZPSoapResponse();

	const AZPSoapResponse *	GetResponseData() const;
	void					SetResponseData(
								const AZPSoapResponse *	inResponseData );
	const ZPWSError			GetWSError() const;
	void					SetWSError(
								const ZPWSError				inWSError );
protected:
private:
	const AZPSoapResponse * mResponseData;
	ZPWSError	mWSError;
};


CREATE_PMINTERFACE(CZPSoapResponse, kZPSoapResponseImpl)

CZPSoapResponse::CZPSoapResponse(IPMUnknown * boss)
: inheritClass(boss)
, mResponseData( nil )
, mWSError ( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSoapResponse::~CZPSoapResponse()
{
	if( mResponseData )
		delete mResponseData;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetResponseData
//----------------------------------------------------------------------------------------
const AZPSoapResponse *
CZPSoapResponse::GetResponseData()const
{
	return mResponseData;
}

//----------------------------------------------------------------------------------------
// SetResponseData
//----------------------------------------------------------------------------------------
void
CZPSoapResponse::SetResponseData(
	const AZPSoapResponse *	inResponseData)
{
	mResponseData = inResponseData;
}

//----------------------------------------------------------------------------------------
// GetWSError
//----------------------------------------------------------------------------------------
const ZPWSError
CZPSoapResponse::GetWSError()const
{
	return mWSError;
}

//----------------------------------------------------------------------------------------
// SetWSError
//----------------------------------------------------------------------------------------
void
CZPSoapResponse::SetWSError(
	const ZPWSError				inWSError)
{
	mWSError = inWSError;
}
