//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPInDnUtils.cpp $
//	$Revision: 4000 $
//	$Date: 2012-04-03 11:39:02 +0200 (Tue, 03 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPDFExptStyleListMgr.h"
#include "IStringData.h"

//SDK General includes
#include "LocaleSetting.h"
#include "PreferenceUtils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPInDnUtils>
class CZPInDnUtils : public inheritClass
{
public:
						CZPInDnUtils(IPMUnknown * boss);
	virtual				~CZPInDnUtils();

	bool16				IsHostAppInCopy() const;
	bool16				IsHostAppInDesign() const;
	bool16				IsHostAppWithUI() const; //True if incopy or indesign.
	bool16				IsHostAppInDesignServer() const;
	int					GetPDFStyleIndex(
							const PMString &				inPdfStyleName,
							const IDocument *				inDoc ) const;
	UIDRef				GetPDFExportStyle(
							int								inStyleIndex ) const;
	int					GetCurrentPDFExportStyleIndex() const;
	const PMString &	GetCurrentPDFExportStyleName() const;
	PMString			GetPDFExportStyleName(
							int								inStyleIndex ) const;

protected:
private:
};


CREATE_PMINTERFACE(CZPInDnUtils, kZPInDnUtilsImpl)

CZPInDnUtils::CZPInDnUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPInDnUtils::~CZPInDnUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsHostAppInCopy
//----------------------------------------------------------------------------------------
bool16
CZPInDnUtils::IsHostAppInCopy() const
{
	static bool16 gIsInCopy = (kInCopyProductFS == LocaleSetting::GetLocale().GetProductFS());
	return gIsInCopy;
}

//----------------------------------------------------------------------------------------
// IsHostAppInDesign
//----------------------------------------------------------------------------------------
bool16
CZPInDnUtils::IsHostAppInDesign() const
{
	static bool16 gIsInDesign = (kInDesignProductFS == LocaleSetting::GetLocale().GetProductFS());
	return gIsInDesign;
}

//----------------------------------------------------------------------------------------
// IsHostAppUI
//----------------------------------------------------------------------------------------
bool16
CZPInDnUtils::IsHostAppWithUI() const //True if incopy or indesign.
{
	if( kFalse == IsHostAppInDesignServer() )
		return kTrue;
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsHostAppInDesignServer
//----------------------------------------------------------------------------------------
bool16
CZPInDnUtils::IsHostAppInDesignServer() const
{
	static bool16 gIsInDesignServer = (kInDesignServerProductFS == LocaleSetting::GetLocale().GetProductFS());
	return gIsInDesignServer;
}

//----------------------------------------------------------------------------------------
// GetPDFStyleIndex
//----------------------------------------------------------------------------------------
int
CZPInDnUtils::GetPDFStyleIndex(
	const PMString &				inPdfStyleName,
	const IDocument *				inDoc = nil) const
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
CZPInDnUtils::GetCurrentPDFExportStyleIndex() const
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
CZPInDnUtils::GetCurrentPDFExportStyleName()const
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
// GetPDFExportStyle
//----------------------------------------------------------------------------------------
UIDRef
CZPInDnUtils::GetPDFExportStyle(
	int								inStyleIndex) const
{
	UIDRef toReturn = UIDRef::gNull;
	do
	{
		InterfacePtr<IPDFExptStyleListMgr> styleMgr((IPDFExptStyleListMgr*)
											::QuerySessionPreferences(IPDFExptStyleListMgr::kDefaultIID));
		
		ASSERT( styleMgr );
		if( !styleMgr )
			break;
		
		int32 numStyles = styleMgr->GetNumStyles();
		if( inStyleIndex < numStyles && inStyleIndex >= 0)
			toReturn = styleMgr->GetNthStyleRef( inStyleIndex );
	} while (false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetPDFExportStyleName
//----------------------------------------------------------------------------------------
PMString
CZPInDnUtils::GetPDFExportStyleName(
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
