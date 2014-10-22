//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ExportProviders/CZPInCopyStoryExportSuite.cpp $
//	$Revision: 4093 $
//	$Date: 2012-12-19 12:18:48 +0100 (Wed, 19 Dec 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntegratorTarget.h"

//SDK General includes
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"

//IZP General includes
#include "StUtils.h"

#if defined(InDnCS5) || defined(InDnCS5_5)
#include "CExportProvider.h"
#endif

#define protected public	//Hack: to access GetExportableItemList of default impl

#include "CZPInCopyStoryExportSuite.h"
#undef protected

#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_ICExport
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPInCopyStoryExportSuite::CZPInCopyStoryExportSuite( IPMUnknown* boss )
: inheritClass_ZPInCopyStoryExport( boss )
, mReturnSingleSelection( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPInCopyStoryExportSuite::~CZPInCopyStoryExportSuite()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ExportToFile
//----------------------------------------------------------------------------------------
void
CZPInCopyStoryExportSuite::ExportToFile(
	const IDFile& sysFile,
	IDocument* doc,
	IPMUnknown* targetboss,
	const PMString& formatName,
	UIFlags uiFlags)
{
	UIDList pendingItems = this->GetExportableItemList( targetboss );
	int oldPendingItemsCount = pendingItems.Length();
	int newPendingItemsCount = 0;
	IDFile currFilePath = sysFile;
	const IDFile emptyPath;
	do
	{
		if( oldPendingItemsCount == 0 )
			break;
		//Create a new asset if path is empty.
		if( currFilePath == emptyPath )
			currFilePath = Utils<IZPAMHelper>()->CreateNewStoryAsset( doc );
		{
			StRestoreValue<bool>	autoRestore( mReturnSingleSelection, true );
			inheritClass_ZPInCopyStoryExport::ExportToFile( currFilePath, doc, targetboss, formatName, uiFlags );
		}

		pendingItems = this->GetExportableItemList( targetboss );
		newPendingItemsCount = pendingItems.Length();
		if( newPendingItemsCount <= 0 || newPendingItemsCount >= oldPendingItemsCount )
			break;	//Don't go in infinite loop, might be due to failure of exporting.
		oldPendingItemsCount = newPendingItemsCount;
		currFilePath = emptyPath;
	}while( true );
}

//----------------------------------------------------------------------------------------
// GetExportableItemList
//----------------------------------------------------------------------------------------
UIDList
CZPInCopyStoryExportSuite::GetExportableItemList (
	IPMUnknown* targetboss) const
{
	UIDList toReturn;

	UIDList listToFilter;
	InterfacePtr<inheritClass_ZPInCopyStoryExport> selfDelegate( this, this->GetDelegateInterfaceID() );
	if( !selfDelegate )
	{	//Try the interface IID on the targetboss, done for layout context
		InterfacePtr<IExportProvider> exportProviderOnTarget( targetboss, this->GetDelegateInterfaceID() );
		if( exportProviderOnTarget )
		{
			inheritClass_ZPInCopyStoryExport * ptr = dynamic_cast<inheritClass_ZPInCopyStoryExport*> ( exportProviderOnTarget.get() );
			if( ptr )
				ptr->AddRef();
			else//Delegate on target is an ASB impl
				ptr = this->QueryDelegateExportProvider( targetboss );

			selfDelegate.reset( ptr );
		}
	}
	if( selfDelegate )
	{
#if DEBUG
		//DebugClassUtilsBuffer classBuf;
		//const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass( selfDelegate ));
		//IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Delegate export provider class name = %s", className );
#endif
		listToFilter = selfDelegate->GetExportableItemList( targetboss );		
	}		

	int32 numStoriesToExport = listToFilter.Length();
	if( numStoriesToExport )
		toReturn = UIDList( listToFilter.GetDataBase() );
	for( int32 curModelIndex = 0; curModelIndex < numStoriesToExport; ++curModelIndex )
	{
		toReturn.Append( listToFilter[curModelIndex] );
		if( mReturnSingleSelection )
			break;
	}	

	return toReturn;
}

//----------------------------------------------------------------------------------------
// QueryDelegateExportProvider
//----------------------------------------------------------------------------------------
CInCopyStoryExportSuite *
CZPInCopyStoryExportSuite::QueryDelegateExportProvider(
	IPMUnknown *				inTargetBoss) const
{
	const PMIID delegateIID = this->GetDelegateInterfaceID();
	inheritClass_ZPInCopyStoryExport * ptr = nil;
	InterfacePtr<IExportProvider> exportProviderOnTarget( inTargetBoss, delegateIID );

	InterfacePtr<const IIntegratorTarget> iIntegratorTarget( exportProviderOnTarget, UseDefaultIID ());
	std::auto_ptr<IIntegratorTarget::TargetSuiteCollection> selectionSuites( iIntegratorTarget->GetTarget( delegateIID ));

	if (selectionSuites->size () == 1) // For the time being, we don't want to support multiple CSB's.  Might change in the future...
	{
		IExportProvider*  exportProvider = (IExportProvider*) ((*selectionSuites)[0].get());
		ptr = dynamic_cast<inheritClass_ZPInCopyStoryExport*>(exportProvider);
		if( ptr )
			ptr->AddRef();
	}
	return ptr;
}
