//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPStoryUtils.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 17-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssignmentMgr.h"
#include "IBoolData.h"
#include "IDataLink.h"
#include "IDataLinkHelper.h"
#include "IDataLinkReference.h"
#include "IDocument.h"
#ifdef InDnCS3	//TODO: CS5 Porting
#include "IInCopyBridgeCmdSuite.h"
#endif
#include "IManageableAsset.h"
#include "IStoryLabel.h"
#include "IStoryList.h"
#include "IStrand.h"
#include "IStringData.h"
#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILiveEditFacade.h"	//TODO: CS5 Porting
#include "ILinkUtils.h"
#include "ILinkResource.h"
#include "ILinkResourceStateUpdateCmdData.h"
#include "ILinkFacade.h"
#include "ILinkManager.h"
#include "LinkQuery.h"
#endif

//SDK General includes
#include "InCopySharedID.h"
#include "NameInfo.h"
#include "TextIterator.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPFileUtils.h"
#include "IZPStoryUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Story
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPStoryUtils>
class CZPStoryUtils : public inheritClass
{
public:
						CZPStoryUtils(IPMUnknown * boss);
	virtual				~CZPStoryUtils();

	ErrorCode			ReLinkOutOfDateStories(
							const IDocument *			inDoc ) const;

	ErrorCode			SetDataLinkFileLocation(
							IDataLink *					inLink,
							const IDFile &				inNewLocation ) const;

	bool				GetDataLinkFileLocation(
							IDataLink *					inLink,
							IDFile &					oLocation ) const;
#if defined(InDnCS5) || defined(InDnCS5_5)
	bool				GetLinkResourceFileLocation(
							ILinkResource *				inLinkResource,
							IDFile &					oLocation ) const;
	ErrorCode			UpdateLinkResourceURI(
							ILinkResource *				inLinkResource,
							URI &					resID) const;
	ErrorCode			UpdateICStoryLinks(const IDocument *			inDoc) const;
#endif

	IAssignment *		QueryAssignmentForStoryAsset(
							const IManageableAsset *	inStoryAsset ) const;

	PMString			GetStoryFirstNChars(
							const ITextModel *			inTextModel,
							int							inLengthToReturn,
							bool						inTrimLeadingSpaces,
							CharTextFilterPtr			inFilterFunction ) const;

	void				CheckoutAssignmentAllStories(
							const IAssignment *			inAssignment) const;

	ErrorCode			SetStoryName(
							const UIDRef &				inStoryRef,
							const PMString &			inNewName ) const;

protected:
private:
};


CREATE_PMINTERFACE(CZPStoryUtils, kZPStoryUtilsImpl)

CZPStoryUtils::CZPStoryUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPStoryUtils::~CZPStoryUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReLinkOutOfDateStories
//----------------------------------------------------------------------------------------
ErrorCode
CZPStoryUtils::ReLinkOutOfDateStories(
	const IDocument *			inDoc) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
#ifdef InDnCS3	//TODO: CS5 Porting
		InterfacePtr<IInCopyBridgeCmdSuite> iInCopyCmdSuite( ::CreateObject2<IInCopyBridgeCmdSuite>(kICBridgeCmdSuiteBoss));
		if(!iInCopyCmdSuite)
			break;
#endif

		IDataBase* docDB = ::GetDataBase( inDoc );
		ASSERT( docDB );
		
		InterfacePtr<IStoryList> iStoryList( inDoc, UseDefaultIID());	//RJK: StoryList have non-exported stories, it should be InCopyStoryList
		if(!iStoryList)
			break;

		// relink stories
		UIDList storyList( docDB );
		
		int storyCount = iStoryList->GetUserAccessibleStoryCount();
		for(int i = 0; i < storyCount; ++i )
		{
			UIDRef storyRef = iStoryList->GetNthUserAccessibleStoryUID( i );

			if( storyRef.GetUID() == kInvalidUID )
				continue;
#if defined(InDnCS5) || defined(InDnCS5_5)
			UIDRef storyAssetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
			if( storyAssetRef.GetUID() == kInvalidUID )
				continue;

			//TODO: check link state and add to storyList
			InterfacePtr<ILinkResource> linkRes( storyAssetRef, UseDefaultIID() );
			if(!linkRes)
				continue;
			
			if(linkRes->GetState() != ILinkResource::kPending )
				continue;

			storyList.push_back(storyRef.GetUID());
#else
			InterfacePtr<IDataLinkReference> iDataReference( storyRef, UseDefaultIID() );
			if(!iDataReference)
				continue;

			UID dataLinkUID = iDataReference->GetUID();
			if( dataLinkUID == 0 )
				continue;
			InterfacePtr<IDataLink> iDataLink( docDB, dataLinkUID, UseDefaultIID() );
			if(!iDataLink)
				continue;

			// only relink if out of date.
			if( iDataLink->GetCurrentState( nil, nil, nil, nil, nil) != IDataLink::kLinkOutOfDate )
				continue;
				
			storyList.push_back(storyRef.GetUID());
#endif
		}
		
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
		Utils<Facade::ILiveEditFacade>()->UpdateStoryLink(storyList);
#else
		iInCopyCmdSuite->Relink(storyList);
#endif
	}while( false );

	return toReturn;
}


//----------------------------------------------------------------------------------------
// SetDataLinkFileLocation
//----------------------------------------------------------------------------------------
ErrorCode
CZPStoryUtils::SetDataLinkFileLocation(
	IDataLink *					inLink,
	const IDFile &				inNewLocation) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		ASSERT( inLink );

		NameInfo ni;
		if( !Utils<IZPFileUtils>()->ConvertIDFileToNameInfo( inNewLocation, ni ) )
			break;
		
		PMString formatStr;
		uint32 fileType;
		inLink->GetNameInfo( nil, &formatStr, &fileType );

		inLink->SetNameInfo( &ni, &formatStr, fileType );
		toReturn = kSuccess;
	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDataLinkFileLocation
//----------------------------------------------------------------------------------------
bool
CZPStoryUtils::GetDataLinkFileLocation(
	IDataLink *					inLink,
	IDFile &					oLocation) const
{
	bool toReturn = false;
	ASSERT( inLink );
	if( inLink )
	{
#if 1
		NameInfo ni;
		inLink->GetNameInfo( &ni, nil, nil );
		Utils<IZPFileUtils>()->ConvertNameInfoToIDFile( ni, oLocation );
		toReturn = true;
#else
		InterfacePtr<IDataLinkHelper> iDataLinkHelper( (IDataLinkHelper*)::CreateObject( kDataLinkHelperBoss, IDataLinkHelper::kDefaultIID ) );
		ASSERT( iDataLinkHelper );
		
		ErrorCode err = iDataLinkHelper->GetSysFile( inLink, oLocation );
		toReturn = err == kSuccess;
#endif
	}
	return toReturn;
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLinkResourceFileLocation
//----------------------------------------------------------------------------------------
bool
CZPStoryUtils::GetLinkResourceFileLocation(
	ILinkResource *				inLinkResource,
	IDFile &					oLocation) const
{
	bool toReturn = false;
	ASSERT( inLinkResource );
	if( inLinkResource )
	{
		ILinkResource::ResourceId resID = inLinkResource->GetId();
		toReturn = Utils<ILinkUtils>()->ResourceIdToIDFile( resID, oLocation );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateLinkResourceURI
//----------------------------------------------------------------------------------------
ErrorCode
CZPStoryUtils::UpdateLinkResourceURI(
	ILinkResource *				inLinkResource,
	URI &					resID) const
{
	ErrorCode err = kFailure;
	ASSERT( inLinkResource );
	if( inLinkResource )
	{
		InterfacePtr<ICommand> updateCmd(CmdUtils::CreateCommand(kLinkResourceStateUpdateCmdBoss));
		InterfacePtr<ILinkResourceStateUpdateCmdData> updateCmdData(updateCmd, UseDefaultIID());

		updateCmdData->SetResource(::GetUIDRef(inLinkResource).GetUID());
		updateCmdData->SetDatabase(::GetDataBase(inLinkResource));
		updateCmdData->SetNotify(true);
		updateCmdData->SetUpdateAction(ILinkResourceStateUpdateCmdData::kUpdateURI);
		updateCmdData->SetURI(resID);

		err = CmdUtils::ProcessCommand(updateCmd);
	}
	return err;
}

ErrorCode CZPStoryUtils::UpdateICStoryLinks(const IDocument *			inDoc) const
{
	ErrorCode err = kFailure;

	IDataBase* docDB = ::GetDataBase( inDoc );
	ASSERT( docDB );
	UIDList resultUIDs(docDB);
	UIDList updateLinksUID(docDB);

	InterfacePtr<ILinkManager> linkMgr(inDoc, UseDefaultIID());
	ASSERT(linkMgr);

	LinkQuery query;
	linkMgr->QueryLinks(query, kIDLinkClientID, resultUIDs);

	for(UIDList::iterator iter = resultUIDs.begin(); iter != resultUIDs.end(); ++iter)
	{
		UID linkUID = *iter;
		InterfacePtr<ILink> theLink(docDB, linkUID, UseDefaultIID());
		if(theLink) 
		{	
			UIDRef linkRef(docDB, linkUID);
			Facade::ILinkFacade::LinkStatus status = Utils<Facade::ILinkFacade>()->GetLinkStatus(linkRef, true);				

			if(status != Facade::ILinkFacade::kLinkResourceMissing && (status == Facade::ILinkFacade::kLinkOutOfDate || status == Facade::ILinkFacade::kLinkUpToDate) )
				updateLinksUID.push_back(linkUID);
		}

	}

	if(!updateLinksUID.IsEmpty())
	{
		err = Utils<Facade::ILinkFacade>()->UpdateLinks(updateLinksUID, false, kMinimalUI, false);     
	}		

	return err;
}
#endif

//----------------------------------------------------------------------------------------
// QueryAssignmentForStoryAsset
//----------------------------------------------------------------------------------------
IAssignment *
CZPStoryUtils::QueryAssignmentForStoryAsset(
	const IManageableAsset *	inStoryAsset) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IAssignment * toReturn = nil;
	do
	{
		InterfacePtr<IAssignmentMgr> iAssignMgr(gSession, UseDefaultIID());
		if(!iAssignMgr)
			break;

		
		UIDRef storyAssetRef = ::GetUIDRef( inStoryAsset );
		UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( storyAssetRef );

		if( storyRef == UIDRef::gNull )
			break;

		InterfacePtr<IAssignment> storyAssignment;
		InterfacePtr<IAssignedStory> storyAssignedStory;

		iAssignMgr->QueryAssignmentAndAssignedStory( storyRef, storyAssignment, storyAssignedStory );
		
		toReturn = storyAssignment.forget();
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetStoryFirstNChars
//----------------------------------------------------------------------------------------
PMString
CZPStoryUtils::GetStoryFirstNChars(
	const ITextModel *			inTextModel,
	int							inLengthToReturn,
	bool						inTrimLeadingSpaces,
	CharTextFilterPtr			inFilterFunction) const
{
	PMString toReturn;

	do
	{
		if( !inTextModel )
			break;
		// Navigate to the paragraph strand for this story:
		InterfacePtr<IStrand> paraStrand( (IStrand*)inTextModel->QueryStrand( kParaAttrStrandBoss, IStrand::kDefaultIID ) );
		ASSERT( paraStrand );
						
		if (paraStrand == nil)
			break;

		TextIterator firstChar( inTextModel, 0);
		for(int i=0; !firstChar.IsNull(); ++firstChar)
		{
			const UTF32TextChar currTextChar = *firstChar;

			if( inFilterFunction == nil || inFilterFunction( currTextChar ) )
			{
				toReturn.AppendW( currTextChar );
				++i;
				if( i >= inLengthToReturn )
				{
					if( inTrimLeadingSpaces ) //Strip the leading spaces
						toReturn.StripWhiteSpace( PMString::kLeadingWhiteSpace );
					i = toReturn.ByteLength();
					if( i >= inLengthToReturn )
						break;
				}
			}
		}
	}while(kFalse);

	return toReturn;
}

#pragma mark - Filters -
//----------------------------------------------------------------------------------------
// StoryNameFilter
// Filters the chars for story label, to be shown in assignment panel
//----------------------------------------------------------------------------------------
bool
StoryNameFilter(
	const UTF32TextChar &	inValue)
{
	bool toReturn = false;
	do
	{
		if( inValue == kTextChar_CR
		|| inValue == kTextChar_LF )
		{
			break;
		}
		
		if( inValue == kTextChar_Space
		|| UnicodeClass::IsLetter( inValue )
		|| UnicodeClass::IsPunctuationDash( inValue )
		|| UnicodeClass::IsMidWordPunctuation( inValue )
		|| UnicodeClass::IsRomanDigit( inValue ) )
		{
			toReturn = true;
		}
	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckoutAssignmentAllStories
//----------------------------------------------------------------------------------------
void
CZPStoryUtils::CheckoutAssignmentAllStories(
	const IAssignment *			inAssignment) const
{
	if(!inAssignment)
		return;;

	IDataBase *db = ::GetDataBase(inAssignment);
	if(!db)
		return;

	UIDList assignedStoriesList = inAssignment->GetStories();

	for (int x = 0; x < assignedStoriesList.size(); ++x)
	{
		UIDRef assignedStoryRef = assignedStoriesList.GetRef(x);
		InterfacePtr<IAssignedStory> assignedStory(assignedStoryRef, UseDefaultIID());
		if (!assignedStory)
			continue;

		UIDRef storyRef = assignedStory->GetUIDRef();

		if( storyRef.GetUID() == kInvalidUID )
			continue;
#if defined(InDnCS5) || defined(InDnCS5_5)
		UIDRef storyAssetRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
		if( storyAssetRef.GetUID() == kInvalidUID )
			continue;
		InterfacePtr<IManageableAsset> asset(storyAssetRef, UseDefaultIID());
#else
		// relink causes datalink to be replaced with a new one
		InterfacePtr<IDataLinkReference> iDataReference(storyRef, UseDefaultIID());
		if(!iDataReference)
			continue;
		
		UID dataLinkUID = iDataReference->GetUID();
		if( dataLinkUID == 0 )
			continue;
		InterfacePtr<IDataLink> iDataLink(db, dataLinkUID, UseDefaultIID());
		if(!iDataLink)
			continue;
		
		InterfacePtr<IManageableAsset> asset(iDataLink, UseDefaultIID());
#endif
		if(!asset)
			continue;
	
		if (Utils<Facade::IWorkgroupFacade>()->CanCheckOut(::GetUIDRef(asset)))
			Utils<Facade::IWorkgroupFacade>()->CheckOut(::GetUIDRef(asset), kSuppressUI, NULL);				// check out
	}
}

//----------------------------------------------------------------------------------------
// SetStoryName
//----------------------------------------------------------------------------------------
ErrorCode
CZPStoryUtils::SetStoryName(
	const UIDRef &				inStoryRef,
	const PMString &			inNewName) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	if( inNewName.IsEmpty())
		return toReturn;

	InterfacePtr<IStoryLabel> storyLabel( inStoryRef, IID_ISTORYLABEL);
	if( !storyLabel )
		return toReturn;

	const PMString & currentStoryName = storyLabel->GetLabel();
	if(currentStoryName.Compare(kTrue, inNewName) == 0)
		return kSuccess;	//Same name, no need to change

	InterfacePtr<ICommand> labelCmd(CmdUtils::CreateCommand(kSetStoryLabelCmdBoss));
	ASSERT(labelCmd);

	InterfacePtr<IStringData> labelData(labelCmd, IID_ISTRINGDATA);
	InterfacePtr<IBoolData> boolData(labelCmd, IID_IBOOLDATA);
	boolData->Set(kFalse);
	labelData->Set(inNewName);
	UIDList list( inStoryRef );
	labelCmd->SetItemList(list);
	toReturn = CmdUtils::ProcessCommand(labelCmd);

	return toReturn;
}
