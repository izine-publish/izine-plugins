//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpInDnUtils.cpp $
//	$Revision: 3407 $
//	$Date: 2011-06-29 09:44:58 +0200 (Wed, 29 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 28-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPDFExptStyleListMgr.h"
#include "IStringData.h"

//SDK General includes
#include "PreferenceUtils.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpInDnUtils>
class CZExpInDnUtils : public inheritClass
{
public:
						CZExpInDnUtils(IPMUnknown * boss);
	virtual				~CZExpInDnUtils();

	int					GetPDFStyleIndex(
							const PMString &				inPdfStyleName,
							const IDocument *				inDoc ) const;
	int					GetCurrentPDFExportStyleIndex() const;
	const PMString &	GetCurrentPDFExportStyleName() const;
	PMString			GetPDFExportStyleName(
							int								inStyleIndex ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpInDnUtils, kZExpInDnUtilsImpl)

CZExpInDnUtils::CZExpInDnUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpInDnUtils::~CZExpInDnUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPDFStyleIndex
//----------------------------------------------------------------------------------------
int
CZExpInDnUtils::GetPDFStyleIndex(
	const PMString &				inPdfStyleName,
	const IDocument *				inDoc) const
{
	int toReturn = -1;
	do
	{
		//TODO: Check inDoc prefs.
		InterfacePtr<IPDFExptStyleListMgr> styleMgr((IPDFExptStyleListMgr*)
											::QuerySessionPreferences(IPDFExptStyleListMgr::kDefaultIID));
		
		ASSERT( styleMgr );
		if( !styleMgr )
			break;
		
		int32 numStyles = styleMgr->GetNumStyles();
		for (int32 i = 0 ; i < numStyles ; i++)
		{
			PMString currStyleName;
			if( styleMgr->GetNthStyleName(i, &currStyleName) != kSuccess )
				continue;
			
			if( currStyleName.Compare(kFalse, inPdfStyleName) == 0 )
			{
				toReturn = i;
				break;
			}
		}
	} while (false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetCurrentPDFExportStyleIndex
//----------------------------------------------------------------------------------------
int
CZExpInDnUtils::GetCurrentPDFExportStyleIndex()const
{
	int32 toReturn = -1;
	do {
		// get the IPDFExptStyleListMgr
		InterfacePtr<IPDFExptStyleListMgr> styleMgr	((IPDFExptStyleListMgr*)::QuerySessionPreferences(IPDFExptStyleListMgr::kDefaultIID));
		if (styleMgr == nil)
		{
			ASSERT_FAIL("Failed to get IPDFExptStyleListMgr");
			break;
		}
		
		const PMString & lastStyleName = this->GetCurrentPDFExportStyleName();
		if (lastStyleName.empty()) //No style name found
			break;
		
		// get the index of the last used style
		toReturn = styleMgr->GetStyleIndexByName( lastStyleName );

		ASSERT( toReturn < styleMgr->GetNumStyles() && toReturn >= 0);

	} while (false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetCurrentPDFExportStyleName
//----------------------------------------------------------------------------------------
const PMString &
CZExpInDnUtils::GetCurrentPDFExportStyleName()const
{
	// get the last use export style
	do
	{
		InterfacePtr<const IStringData> lastPDFExportStyleUsed((IStringData*)::QuerySessionPreferences(IID_IPDFEXPORTSTYLELASTUSED));
		if (lastPDFExportStyleUsed == nil)
		{
			ASSERT_FAIL("Failed to get IStringData");
			break;
		}
		const PMString & toReturn = lastPDFExportStyleUsed->Get();
		return toReturn;

	}while(kFalse);
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetPDFExportStyleName
//----------------------------------------------------------------------------------------
PMString
CZExpInDnUtils::GetPDFExportStyleName(
	int								inStyleIndex) const
{
	PMString toReturn;
	do
	{
		//TODO: Check inDoc prefs.
		InterfacePtr<IPDFExptStyleListMgr> styleMgr((IPDFExptStyleListMgr*)
											::QuerySessionPreferences(IPDFExptStyleListMgr::kDefaultIID));
		
		ASSERT( styleMgr );
		if( !styleMgr )
			break;
		
		int32 numStyles = styleMgr->GetNumStyles();
		if( inStyleIndex < numStyles && inStyleIndex >= 0)
			styleMgr->GetNthStyleName( inStyleIndex, &toReturn);
	} while (false);
	return toReturn;
}
