//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPStoryUtils.h $
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
#ifndef _h_IZPStoryUtils_
#define _h_IZPStoryUtils_
#pragma once

class IDataLink;
class IDocument;
class IAssignment;
class IManageableAsset;
class ITextModel;

typedef bool	(*CharTextFilterPtr)( const UTF32TextChar &	inValue );

bool StoryNameFilter( const UTF32TextChar &	inValue );	//Filters the chars for story label, to be shown in assignment panel

class IZPStoryUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPSTORYUTILS };

	virtual ErrorCode	ReLinkOutOfDateStories(
							const IDocument *			inDoc ) const = 0;

	virtual ErrorCode	SetDataLinkFileLocation(
							IDataLink *					inLink,
							const IDFile &				inNewLocation ) const = 0;

	virtual bool		GetDataLinkFileLocation(
							IDataLink *					inLink,
							IDFile &					oLocation ) const = 0;
#if defined(InDnCS5) || defined(InDnCS5_5)
	virtual bool		GetLinkResourceFileLocation(
							ILinkResource *				inLinkResource,
							IDFile &					oLocation ) const = 0;
	virtual ErrorCode	UpdateLinkResourceURI(
							ILinkResource *				inLinkResource,
							URI &						resID) const = 0;
	virtual ErrorCode	UpdateICStoryLinks(
							const IDocument *			inDoc) const = 0;
#endif

	virtual IAssignment *	QueryAssignmentForStoryAsset(
							const IManageableAsset *	inStoryAsset ) const = 0;

	virtual PMString	GetStoryFirstNChars(
							const ITextModel *			inTextModel,
							int							inLengthToReturn,	//Length after filtering
							bool						inTrimLeadingSpaces = true,
							CharTextFilterPtr			inFilterFunction = nil ) const = 0;

	virtual void		CheckoutAssignmentAllStories(
							const IAssignment *			inAssignment) const = 0;

	virtual ErrorCode	SetStoryName(
							const UIDRef &				inStoryRef,
							const PMString &			inNewName ) const = 0;
};


#endif //_h_IZPStoryUtils_
