//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Export/CZExpXMLExport_Paths.cpp $
//	$Revision: 3617 $
//	$Date: 2011-08-31 17:31:57 +0200 (Wed, 31 Aug 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-08-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDocument.h"
#include "IPathGeometry.h"
#include "IPathUtils.h"
#include "IWorkspace.h"
#include "IXMLOutStream.h"

//SDK General includes

//OS / ThirdParty includes

//#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "StInDnXmlUtils.h"
#include "CZExpXMLAttrUtils.h"
#include "CZExpXMLExport_Paths.h"
#include "StUtils.h"
#include "ZExpXMLConstants.h"
#include "ZExpStyleAttrConstants.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
#define CZExpXMLExport_Paths CZExpXMLExportUtils

namespace CZExpXMLExport_Paths
{

	void				AddPath(
							IXMLOutStream *				ioStream,
							const IPathGeometry *		inPathGeometry,
							int							inPathIndex );

	void				AddPathPoint(
							IXMLOutStream *				ioStream,
							const IPathGeometry *		inPathGeometry,
							const PMPathPoint &			inPathPoint );
}

//----------------------------------------------------------------------------------------
// AddPathInfo
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Paths::AddPathInfo(
	IXMLOutStream *				ioStream,
	const IPathGeometry *		inPathGeometry)
{
	LogFunctionEnterExit;

	ASSERT( ioStream && inPathGeometry );

	if( Utils<IPathUtils>()->IsRectangle( inPathGeometry ) == kTrue )
		return;

	WideString elementStr( STRXMLTag_PathInfo );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	int numPaths = inPathGeometry->GetNumPaths();

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_PathCount) );
	CZExpXMLAttrUtils::SetValue_Int( attr, numPaths );
	attrList.push_back( attr );

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopPathInfo( ioStream );

	for( int i = 0 ; i < numPaths ; ++i )
	{
		CZExpXMLExport_Paths::AddPath( ioStream, inPathGeometry, i );
	}
}

//----------------------------------------------------------------------------------------
// AddPath
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Paths::AddPath(
	IXMLOutStream *				ioStream,
	const IPathGeometry *		inPathGeometry,
	int							inPathIndex )
{
	LogFunctionEnterExit;

	WideString elementStr( STRXMLTag_Path );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	int numPoints = inPathGeometry->GetNumPoints(inPathIndex);

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_PointCount) );
	CZExpXMLAttrUtils::SetValue_Int( attr, numPoints );
	attrList.push_back( attr );	

	bool16 isPathClosed = inPathGeometry->IsPathClosed( inPathIndex );
	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_IsClosed) );
	CZExpXMLAttrUtils::SetValue_Bool( attr, isPathClosed );
	attrList.push_back( attr );	

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopPathInfo( ioStream );

	PMString pointsStr;
	for( int i = 0 ; i < numPoints ; ++i )
	{
		const PMPathPoint & pathPoint = inPathGeometry->GetNthPoint( inPathIndex, i );

		CZExpXMLExport_Paths::AddPathPoint( ioStream, inPathGeometry, pathPoint );
	}
}

//----------------------------------------------------------------------------------------
// AddPathPoint
//----------------------------------------------------------------------------------------
void
CZExpXMLExport_Paths::AddPathPoint(
	IXMLOutStream *				ioStream,
	const IPathGeometry *		inPathGeometry,
	const PMPathPoint &			inPathPoint)
{
	LogFunctionEnterExit;

	WideString elementStr( STRXMLTag_Point );

	IXMLOutStream::Attribute attr;
	IXMLOutStream::AttributeList attrList;

	const PMPoint & point = inPathPoint.GetPoint(kAnchorPt);

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_X) );
	CZExpXMLAttrUtils::SetValue_Real( attr, point.X() );
	attrList.push_back( attr );
	

	attr.fName.SetX16String( TOWSTRING(STRXMLAttr_Y) );
	CZExpXMLAttrUtils::SetValue_Real( attr, point.Y() );
	attrList.push_back( attr );
	

	ioStream->IndentLevel(kTrue);
	ioStream->PushWithAttributes( elementStr, attrList, kTrue );
	StXMLPopper autoPopPathInfo( ioStream );

}
