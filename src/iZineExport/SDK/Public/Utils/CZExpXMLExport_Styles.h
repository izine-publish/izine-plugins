//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/CZExpXMLExport_Styles.h $
//	$Revision: 3706 $
//	$Date: 2011-12-20 11:03:16 +0100 (Tue, 20 Dec 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpXMLExport_Styles_
#define _h_CZExpXMLExport_Styles_
#pragma once

class IAttributeStrand;
class IDocument;
class IGraphicStyleDescriptor;
class ITextModel;
class IWaxRun;
class IXMLOutStream;

namespace CZExpXMLExportUtils
{
	void				AddPragraphStyles(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc );

	void				AddCharaStyles(
							IXMLOutStream *				ioStream,
							const IDocument *			inDoc );

	void				AddStyleOverrides(
							IXMLOutStream *				ioStream,
							IAttributeStrand *			inAttrStrand,
							TextIndex					inTextIndex,
							bool						inIsParaStyle );

	void				AddGraphicStyles(
							IXMLOutStream *				ioStream,
							const IGraphicStyleDescriptor *	inGStyleDesc );

	void				AddAttributesNotComingInStyle(
							IXMLOutStream *				ioStream,
							const ITextModel *			inTextModel,
							const IWaxRun *				inWaxRun);
}
#endif //_h_CZExpXMLExport_Styles_
