//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLExport_Articles.cpp $
//	$Revision: 4038 $
//	$Date: 2012-06-22 11:57:06 +0200 (Fri, 22 Jun 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#ifdef InDnCS55
#include "IArticleList.h"
#include "IArticleMemberList.h"
#endif
#include "IDocument.h"
#include "IStringData.h"
#include "IXMLOutStream.h"

//SDK General includes
#include "AttributeBossList.h"
#ifdef InDnCS55
#include "ArticleID.h"
#endif

//OS / ThirdParty includes

//#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Articles.h"
#include "StInDnXmlUtils.h"
#include "StUtils.h"
#include "ZExpStyleAttrConstants.h"
#include "ZExpXMLConstants.h"

#pragma mark -

#define CZExpXMLExport_Articles CZExpXMLExportUtils

#ifdef InDnCS55
namespace CZExpXMLExport_Articles
{

}

//----------------------------------------------------------------------------------------
// AddArticlesInDoc
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Articles::AddArticlesInDoc(
	IXMLOutStream *				ioStream,
	const IDocument *			inDoc)
{
	ASSERT( ioStream && inDoc );

	WideString elementStr( STRXMLTag_Articles );

	ioStream->IndentLevel(kTrue);
	ioStream->Push( elementStr, kTrue );
	StXMLPopper autoPopParaStyles( ioStream );

	InterfacePtr<IArticleList> articleList( inDoc, UseDefaultIID() );
	ASSERT( articleList );

	int32 articleCount = articleList->CountArticles();
	UIDRef articleRef( ::GetDataBase( inDoc ), kInvalidUID );
	for( int32 i = 0; i < articleCount ; ++i )
	{
		articleRef.ResetUID( articleList->GetNthArticleUID( i ) );
		if( articleRef.GetUID() != kInvalidUID )
		{
			InterfacePtr<IArticleMemberList> articleMemberList( articleRef, UseDefaultIID() );
			ASSERT( articleMemberList );

			AddArticle( ioStream, articleMemberList );
		}
	}
}

//----------------------------------------------------------------------------------------
// AddArticle
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Articles::AddArticle(
	IXMLOutStream *				ioStream,
	const IArticleMemberList *	inArticleMembers)
{
	ASSERT( ioStream && inArticleMembers );

	WideString elementStr( STRXMLTag_Article );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;
	
	//ID
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_ID) );
	CZExpXMLAttrUtils::SetValue_UID( attr, ::GetUID(inArticleMembers) );
	attrList.push_back( attr );

	//Article Name
	InterfacePtr<const IStringData> articleName( inArticleMembers, IID_IARTICLENAME );
	if( articleName )
	{
		attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
		attr.fValue = WideString(articleName->Get());
		attrList.push_back( attr );
	}

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopCharStyle( ioStream );

	int32 memberCount = inArticleMembers->CountMembers();

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Name) );
	elementStr = WideString( STRXMLTag_ArticleItem );

	//IDataBase * db = ::GetDataBase( inArticleMembers );

	for( int i = 0; i < memberCount ; ++i )
	{
		IXMLOutStream::AttributeList attrList1;
			
		//attr.fValue = WideString( currAttrExportInfo.mAttrName );
		//attrList1.push_back( attr );

		ioStream->IndentLevel(kTrue);
		ioStream->PushWithAttributes( elementStr, attrList1 );
		StXMLIndentDisabler disableIndent( ioStream );	//Disable indent as story content is written in data
		StXMLPopper autoPopStyleAttr( ioStream );
		UID memberUID = inArticleMembers->GetNthMemberUID( i );
		PMString tempStr;
		tempStr.AppendNumber( memberUID.Get() );
		ioStream->Write( WideString(tempStr) );
	}
}

#pragma mark -

#endif //InDnCS55
