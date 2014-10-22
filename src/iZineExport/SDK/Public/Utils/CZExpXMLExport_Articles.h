//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLExport_Articles.h $
//	$Revision: 4023 $
//	$Date: 2012-05-28 13:26:36 +0200 (Mon, 28 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpXMLExport_Articles_
#define _h_CZExpXMLExport_Articles_
#pragma once

#ifdef InDnCS5
class IDocument;
class IXMLOutStream;
class IArticleMemberList;

namespace CZExpXMLExportUtils
{
	void				AddArticlesInDoc(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc );

	void				AddArticle(
							IXMLOutStream *				ioStream,
							const IArticleMemberList *	inArticleMembers);
}

#endif	//InDnCS5
#endif //_h_CZExpXMLExport_Articles_
