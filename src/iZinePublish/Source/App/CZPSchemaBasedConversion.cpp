//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPSchemaBasedConversion.cpp $
//	$Revision: 3566 $
//	$Date: 2011-08-04 12:14:39 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CConversionProvider.h"
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CConversionProvider
class CZPSchemaBasedConversion : public inheritClass
{
public:
						CZPSchemaBasedConversion(IPMUnknown * boss);
	virtual				~CZPSchemaBasedConversion();

	int32				CountConversions() const;
	void				GetNthConversion(
							int32 n, VersionID *fromVersion, VersionID *toVersion) const;
protected:
private:
	K2Vector<VersionID>			mVersions;
};


CREATE_PMINTERFACE(CZPSchemaBasedConversion, kZPSchemaBasedConversionImpl)

CZPSchemaBasedConversion::CZPSchemaBasedConversion(IPMUnknown * boss)
: inheritClass(kIZPPluginID, boss)
{
	LogFunctionEnterExit;
	VersionID version;
	version.SetPluginID( kIZPPluginID );

	version.SetFormat( FormatID(kIZPThirdMajorFormatNumber, kIZPThirdMinorFormatNumber ));
	mVersions.push_back( version );

	version.SetFormat( FormatID(kIZPThirdMajorFormatNumber, kIZPThirdMinorFormatNumber_1 ));
	mVersions.push_back( version );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSchemaBasedConversion::~CZPSchemaBasedConversion()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CountConversions
//----------------------------------------------------------------------------------------
int32
CZPSchemaBasedConversion::CountConversions()const
{
	LogFunctionEnterExit;
	if( mVersions.size() > 1)
		return mVersions.size() -1;
	return 0;
}

//----------------------------------------------------------------------------------------
// GetNthConversion
//----------------------------------------------------------------------------------------
void
CZPSchemaBasedConversion::GetNthConversion(
	int32 n, VersionID *fromVersion, VersionID *toVersion) const
{
	LogFunctionEnterExit;
	ASSERT( n < mVersions.size() );

	*fromVersion = mVersions[n];
	*toVersion = mVersions[n+1];
}
