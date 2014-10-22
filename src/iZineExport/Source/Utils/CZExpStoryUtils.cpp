//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpStoryUtils.cpp $
//	$Revision: 2561 $
//	$Date: 2010-11-26 15:11:38 +0100 (Fri, 26 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ITextModel.h"
#include "IItemStrand.h"
#include "IInlineData.h"

//SDK General includes
#include "TextIterator.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpStoryUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpStoryUtils>
class CZExpStoryUtils : public inheritClass
{
public:
						CZExpStoryUtils(IPMUnknown * boss);
	virtual				~CZExpStoryUtils();

	void				GetStoryTextContent(
							const ITextModel *			inTextModel,
							WideString &				oTextContent,
							int							inStartIndex = 0,
							int							inEndIndex = -1 ) const;

	void				FixStoryTextIndexWithInlineItems(
							const ITextModel *			inTextModel,
							WideString &				ioTextContent,
							int							inStartIndex = 0,
							int							inEndIndex = -1 ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpStoryUtils, kZExpStoryUtilsImpl)

CZExpStoryUtils::CZExpStoryUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpStoryUtils::~CZExpStoryUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetStoryTextContent
//----------------------------------------------------------------------------------------
void
CZExpStoryUtils::GetStoryTextContent(
	const ITextModel *			inTextModel,
	WideString &				oTextContent,
	int							inStartIndex,
	int							inEndIndex) const
{
	if( !inTextModel )
		return;

	if( inStartIndex < 0 )
		inStartIndex = 0;

	if( inEndIndex < 0 )
		inEndIndex = inTextModel->TotalLength();

	// use STL-style iterators to get text from text model (using TextIterator)
	TextIterator beginTextChunk(inTextModel, inStartIndex);
	TextIterator endTextChunk(inTextModel, inEndIndex);

	// retval.reserve(threadLength);	// Watson #1140943 reserve no longer functional now strings do copy-on-write
	std::copy(beginTextChunk, endTextChunk, std::back_inserter(oTextContent));
}

//----------------------------------------------------------------------------------------
// FixStoryTextIndexWithInlineItems
// Add extra char for inline object so that text index becomes correct, i.e according to text runs
//----------------------------------------------------------------------------------------
void
CZExpStoryUtils::FixStoryTextIndexWithInlineItems(
	const ITextModel *			inTextModel,
	WideString &				ioTextContent,
	int							inStartIndex,
	int							inEndIndex ) const
{
	if( !inTextModel )
		return;

	if( inStartIndex < 0 )
		inStartIndex = 0;

	if( inEndIndex < 0 )
		inEndIndex = inTextModel->TotalLength();

	InterfacePtr<const IItemStrand> itemStrand((const IItemStrand*)inTextModel->QueryStrand(kOwnedItemStrandBoss, IItemStrand::kDefaultIID));
	ASSERT(itemStrand != nil);
	if(!itemStrand)
		return;

	// Get the list of owned items and iterate over them looking 
	// for inline frames.
	IDataBase* database = ::GetDataBase( inTextModel );
	OwnedItemDataList ownedList;
	itemStrand->CollectOwnedItems(inStartIndex, inEndIndex, &ownedList);
	int32 ownedItemCount = ownedList.size();
	int32 inlineFrameCount = 0;
	for( int32 ownedItemIndex = 0; ownedItemIndex < ownedItemCount ; ++ownedItemIndex )
	{
		UID inlineUID = ownedList[ownedItemIndex].fUID;
		// Inline frames are rooted on a boss class 
		// with the signature interface IInlineData (normally a kInlineBoss).
		InterfacePtr<const IInlineData> inlineData(database, inlineUID, UseDefaultIID());
		if (!inlineData) {
			continue; 			// not an inline item, so skip
		}

		//Add extra char for this inline object in text story string
		TextIndex itemTextIndex = ownedList[ownedItemIndex].fAt;
		ioTextContent.Insert( WideString(L"?"), itemTextIndex );
	} // iterate owned items
}
