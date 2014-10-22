//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLAttrUtils.h $
//	$Revision: 2535 $
//	$Date: 2010-11-17 11:33:05 +0100 (Wed, 17 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpXMLAttrUtils_
#define _h_CZExpXMLAttrUtils_
#pragma once

#include "IXMLOutStream.h"

class PMRect;

namespace CZExpXMLAttrUtils
{
	void				SetValue_UID(
							IXMLOutStream::Attribute &	ioAttr,
							const UID &					inUID );

	void				SetValue_Bool(
							IXMLOutStream::Attribute &	ioAttr,
							const bool					inValue );

	void				SetValue_Bool(
							IXMLOutStream::Attribute &	ioAttr,
							const bool16				inValue );

	void				SetValue_Int(
							IXMLOutStream::Attribute &	ioAttr,
							int							inValue );

	void				SetValue_Real(
							IXMLOutStream::Attribute &	ioAttr,
							const PMReal &				inValue );

#pragma mark -
	void				AddRect(
							IXMLOutStream::AttributeList & ioAttrList,
							const PMRect &				inValue );
};

#endif //_h_CZExpXMLAttrUtils_
