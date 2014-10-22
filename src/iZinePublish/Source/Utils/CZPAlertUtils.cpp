//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPAlertUtils.cpp $
//	$Revision: 1109 $
//	$Date: 2010-05-05 06:59:07 +0200 (Wed, 05 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
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
#include "IZPAlertUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAlertUtils>
class CZPAlertUtils : public inheritClass
{
public:
						CZPAlertUtils(IPMUnknown * boss);
	virtual				~CZPAlertUtils();

	void				ShowAlert(
							const PMString &			inMainError,
							const PMString &			inErrorDescription,
							const PMString &			inErrorDetail,
							CAlert::eAlertIcon			inIcon );

protected:
private:
};


CREATE_PMINTERFACE(CZPAlertUtils, kZPAlertUtilsImpl)

CZPAlertUtils::CZPAlertUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAlertUtils::~CZPAlertUtils()
{

}

//----------------------------------------------------------------------------------------
// ShowAlert
//----------------------------------------------------------------------------------------
void
CZPAlertUtils::ShowAlert(
	const PMString &			inMainError,
	const PMString &			inErrorDescription,
	const PMString &			inErrorDetail,
	CAlert::eAlertIcon			inIcon )
{
	PMString errorMessage( inMainError );
	errorMessage.Append("\n");
	errorMessage.Append( inErrorDescription );
	errorMessage.Append("\n");
	errorMessage.Append( inErrorDetail);
	errorMessage.SetTranslatable( kFalse);

	switch( inIcon )
	{
	case CAlert::eInformationIcon:
		CAlert::InformationAlert(errorMessage);
		break;
	case CAlert::eWarningIcon:
		CAlert::WarningAlert(errorMessage);
		break;
	case CAlert::eErrorIcon:
		CAlert::ErrorAlert(errorMessage);
		break;
	case CAlert::eQuestionIcon:
	case CAlert::eNoIcon:
	default:
		CAlert::InformationAlert(errorMessage);
	}
}


