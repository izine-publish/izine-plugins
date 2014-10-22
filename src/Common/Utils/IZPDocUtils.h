//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPDocUtils.h $
//	$Revision: 3653 $
//	$Date: 2011-11-16 13:16:58 +0100 (Wed, 16 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPDocUtils_
#define _h_IZPDocUtils_
#pragma once

#include "IOpenFileCmdData.h"

class IDocument;
class IDocumentList;
class IManageableAsset;

class IZPDocUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPDOCUTILS };

	virtual ErrorCode	DisablePreviewGeneration(
							IDocument *					inDoc ) const = 0;

	virtual ErrorCode	SaveDocToFile(
							const IDocument *			inDoc,
							const IDFile &				inFilePath,
							bool						inAsTemplate = false,
							bool						inPerformSaveAs = false) const = 0;

	virtual void		SaveACopy(
							const IDocument *			inDoc,
							const IDFile &				inFilePath,
							UIFlags						inUIFlags,
							FileTypeInfoID				inFileTypeID) const = 0;

	virtual ErrorCode	OpenDocumentFile(
							const IDFile &				inFilePath,
							IOpenFileCmdData::OpenFlags	inOpenFlags,
							UIDRef &					oDocRef ) const = 0;

	virtual ErrorCode	OpenDocumentWindow(
							const UIDRef &				inDocRef ) const = 0;

	virtual ErrorCode	SetDocumentName(
							IDocument *					inDoc,
							const PMString &			inDocName ) const = 0;

	virtual ErrorCode	SetAssignmentName(
							const UIDRef &				inAssignmentRef,
							const PMString &			inNewName,
							bool16						inDirty) const = 0;

	virtual ErrorCode	CreatePageSnapshot(
							const IDocument *			inDoc,
							UID							inPageUID,
							IPMStream *					oStream ) const = 0;

	//Warning: pdf creation might not complete before returning from this function.
	virtual ErrorCode	CreatePDF(
							const IDocument *			inDoc,
							const IDFile &				inPdfFile,
							const PMString &			inPdfStyleName ) const = 0;

	virtual ErrorCode	SetFilePathWithAssignment(
							const UIDRef &				inAssignmentRef,
							const IDFile &				inFile ) const = 0;

	virtual IAssignment *	QueryAssignmentForFilePath(
							const IDocument *			inDoc,
							const IDFile &				inAssignmentFile ) const = 0;

	virtual IAssignment *	QueryAssignmentForAsset(
								const IDocument *			inDoc,
								const IManageableAsset *	inAssignmentAsset ) const = 0;
	virtual IAssignment *	QueryAssignmentFromAssignmentDoc(
								const IDocument *			inAssignmentDoc ) const = 0;

	virtual IAssignment *	QueryAssignmentByName(
							const IDocument *			inDoc,
							const PMString &			inAssignmentName ) const = 0;

	virtual ErrorCode		RemoveAllAssignmentsOnlyFromDoc(
								const IDocument *			inDoc) const = 0;

	virtual void			AllAssignmentsSetDirty(
								const IDocument *			inDoc,
								bool						inIsDirty) const = 0;

	//Will be nil till data link is created. For new assignment till it is check-in it will be nil.
	virtual IManageableAsset *	QueryManageableAssetForAssignment(
							const IDocument *			inDoc,
							const IAssignment *			inAssignment ) const = 0;

	virtual IDocumentList *	QueryOpenDocList() const = 0;
	virtual void			EnsureLatestAllAssignments(
								const IDocument *			inDoc) const = 0;
	virtual void			EnsureLatestAllLinkedStories(
								const IDocument *			inDoc) const = 0;
	virtual ErrorCode		RemoveAllStoriesOnlyFromDoc(
								const IDocument *			inDoc) const = 0;
	virtual void			RemoveAllStyles(
								const UIDRef				inDocRef,
								const PMIID &				inStyleGroupManager) const = 0;

	virtual bool			CanCheckOutAnyStory(
								const IDocument *			inDoc) const = 0;
	virtual void			CheckOutAllStories(
								const IDocument *			inDoc) const = 0;
};


#endif //_h_IZPDocUtils_
