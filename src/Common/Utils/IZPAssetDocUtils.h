//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPAssetDocUtils.h $
//	$Revision: 3669 $
//	$Date: 2011-11-23 11:26:51 +0100 (Wed, 23 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetDocUtils_
#define _h_IZPAssetDocUtils_
#pragma once

#include "ZPTypes.h"

class IDocument;
class UIDList;

class IZPAssetDocUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETDOCUTILS };

	virtual IAssignment *	CreateAssignment(
								const IDocument *		inDocument,
								const PMString &		inAssignmentName,
								bool					inShowCheckinUI ) const = 0;
	virtual IDocument *		GetDocumentForDocAssetID(
								const PMString &		inDocAssetID ) const = 0;

	virtual ErrorCode		GetDocumentAssignments(
								const IDocument *		inDocument,
								ZPAssetIDsList *		oAssignmentAssetIDs,
								UIDList *				oAssignments ) const = 0;

	virtual void			CheckAndUpdateAssignmentStoriesStatus(	//Set all stories status to be "Assigned"
								IAssignment*			inAssignment ) const = 0;
	virtual void			CheckAndUpdateUnAssignmentStoriesStatus(	//Set all unassigned stories status to be "UnAssigned"
								IDocument*				inDocument) const = 0;

	virtual int				CountLinkExistForAssetID(
								const IDocument *		inDocument,
								const PMString &		inAssetID) const = 0;

	virtual bool			IsNewAssignmentAllowed(
								const IDocument *		inDocument ) const = 0;

	virtual void			GetAssetIdsUsedInDoc(
								const IDocument *		inDocument,
								ZPAssetIDsList &		oAssetIdList) const = 0;

	virtual void			GetAssignmentAssetIdsUsedInDoc(
								const IDocument *		inDocument,
								ZPAssetIDsList &		oAssetIdList) const = 0;

	virtual void			GetStoryAssetIdsUsedInDoc(
								const IDocument *		inDocument,
								ZPAssetIDsList &		oAssetIdList) const = 0;

	virtual bool			IsOurAssetDocument(
								const IDocument *		inDocument) const = 0;

	virtual void			ValidateAssignmentsName(
								const IDocument *		inDocument) const = 0;

	virtual void			ValidateStoriesName(
								const IDocument *		inDocument) const = 0;
};


#endif //_h_IZPAssetDocUtils_
