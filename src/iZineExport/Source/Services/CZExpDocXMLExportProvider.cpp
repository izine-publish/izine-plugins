//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Services/CZExpDocXMLExportProvider.cpp $
//	$Revision: 3592 $
//	$Date: 2011-08-17 08:48:52 +0200 (Wed, 17 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IExportProvider.h"
#include "IXMLStreamUtils.h"
#include "IXMLOutStream.h"
#include "IPMStream.h"

//SDK General includes
#include "CExportProvider.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpXMLWriteFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CExportProvider
class CZExpDocXMLExportProvider : public inheritClass
{
public:
						CZExpDocXMLExportProvider(IPMUnknown * boss);
	virtual				~CZExpDocXMLExportProvider();

	void				ExportToFile(
							const IDFile&				inSysFile,
							IDocument*					inDoc,
							IPMUnknown*					inTargetboss,
							const PMString&				inFormatName,
							UIFlags						inUIFlags);

	/**
	 Exports the current selection to the given stream.
	 The provider will generate the specified file format. \\
	 Note: Some export providers may display UI and may export more than just
		   the current selection (i.e. PDF export, EPS export, etc.).
	 @param stream		IN Streaam to export to.
	 @param doc			IN Document we're exporting from.
	 @param targetboss	IN Target boss.
	 @param formatName	IN Name of the format we're exporting.
	 @param uiFlags		IN UI flags denoting how to handle UI events. (see BaseType.h)
	 */
	virtual void ExportToStream(IPMStream* stream, IDocument* doc, IPMUnknown* targetboss, const PMString& formatName, UIFlags uiFlags = kFullUI);
	
	/**
	 The export provider indicates whether it can export the current selection in the
	 specified file format (i.e. a text export file would return kFalse for a page item selection
	 and kTrue for a text selection).
	 @param doc			IN Document we're exporting from.
	 @param targetboss	IN Target boss.
	 @param formatName	IN Name of the format we're exporting.
	 @return kTrue if the provider supports exporting in the current context, else kFalse.
	 */
	virtual bool16 CanExportThisFormat(IDocument* doc, IPMUnknown* targetboss, const PMString& formatName);
	
	/**
	 Returns the number of file formats supported by this export provider.
	 @return Number of file formats supported.
	 */
	virtual int32 CountFormats();
		
	/**
	 Returns the name (localized string) of the n'th file format supported by this export provider.
	 @param n	IN Index of the format name to return.
	 @return Name of the n'th format.
	 */
	virtual PMString GetNthFormatName(int32 n);
		
	/**
	 Returns the preferred file extension of the format name.
	 @param formatName	IN Format name whose preferred file extension to return.
	 @return Preferred file extension of the format name.
	 */
	virtual PMString GetExtensionForFormatName(const PMString& formatName);

#ifdef InDnCS5
	/**
	 Returns the ScriptID for the given format's enumerator in the kExportFormatEnumScriptElement
	 @param formatName	IN Format name whose ScriptID to return
	 @param context is the scripting request context
	 @return the ScriptID (or kInvalidScriptID if this format is not supported by enum)
	 */
	virtual ScriptID GetScriptIDForFormatName(const PMString& formatName, const ScriptInfo::RequestContext& context);
#endif
protected:
private:
};


CREATE_PMINTERFACE(CZExpDocXMLExportProvider, kZExpDocXMLExportProviderImpl)

CZExpDocXMLExportProvider::CZExpDocXMLExportProvider(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpDocXMLExportProvider::~CZExpDocXMLExportProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ExportToFile
//----------------------------------------------------------------------------------------
void
CZExpDocXMLExportProvider::ExportToFile(
	const IDFile&				inSysFile,
	IDocument*					inDoc,
	IPMUnknown*					inTargetboss,
	const PMString&				inFormatName,
	UIFlags						inUIFlags)
{
	ASSERT( inFormatName == kIZExpXMLFormatStringKey );

	InterfacePtr<IXMLOutStream> xmlOutStream(Utils<IXMLStreamUtils>()->CreateXMLOutFileStream(inSysFile, IXMLOutStream::kUTF8));
	if( !xmlOutStream || !inDoc )
		return;	//TODO: show error

	Utils<IZExpXMLWriteFacade>()->ExportDocument( xmlOutStream, inDoc );
}

/**
 Exports the current selection to the given stream.
 The provider will generate the specified file format. \\
 Note: Some export providers may display UI and may export more than just
	   the current selection (i.e. PDF export, EPS export, etc.).
 @param stream		IN Streaam to export to.
 @param doc			IN Document we're exporting from.
 @param targetboss	IN Target boss.
 @param formatName	IN Name of the format we're exporting.
 @param uiFlags		IN UI flags denoting how to handle UI events. (see BaseType.h)
 */
//----------------------------------------------------------------------------------------
// ExportToStream
//----------------------------------------------------------------------------------------
void
CZExpDocXMLExportProvider::ExportToStream(
	IPMStream*				ioStream,
	IDocument*				inDoc,
	IPMUnknown*				inTargetboss,
	const PMString&			inFormatName,
	UIFlags					inUIFlags )
{
	ASSERT( inFormatName == kIZExpXMLFormatStringKey );

	return;	//TODO: Display error
}

/**
 The export provider indicates whether it can export the current selection in the
 specified file format (i.e. a text export file would return kFalse for a page item selection
 and kTrue for a text selection).
 @param doc			IN Document we're exporting from.
 @param targetboss	IN Target boss.
 @param formatName	IN Name of the format we're exporting.
 @return kTrue if the provider supports exporting in the current context, else kFalse.
 */
//----------------------------------------------------------------------------------------
// CanExportThisFormat
//----------------------------------------------------------------------------------------
bool16
CZExpDocXMLExportProvider::CanExportThisFormat(
	IDocument*				inDoc,
	IPMUnknown*				inTargetboss,
	const PMString&			inFormatName)
{
	if( inDoc && inFormatName == kIZExpXMLFormatStringKey )
		return kTrue;
	else
		return kFalse;
}

/**
 Returns the number of file formats supported by this export provider.
 @return Number of file formats supported.
 */
//----------------------------------------------------------------------------------------
// CountFormats
//----------------------------------------------------------------------------------------
int32
CZExpDocXMLExportProvider::CountFormats()
{
#ifdef IZExport
	return 1;
#else
	return 0;
#endif
}

/**
 Returns the name (localized string) of the n'th file format supported by this export provider.
 @param n	IN Index of the format name to return.
 @return Name of the n'th format.
 */
//----------------------------------------------------------------------------------------
// GetNthFormatName
//----------------------------------------------------------------------------------------
PMString
CZExpDocXMLExportProvider::GetNthFormatName(
	int32					inIndex)
{
	if( inIndex == 0 )
		return kIZExpXMLFormatStringKey;
	else
		return kNullString;
}

/**
 Returns the preferred file extension of the format name.
 @param formatName	IN Format name whose preferred file extension to return.
 @return Preferred file extension of the format name.
 */
//----------------------------------------------------------------------------------------
// GetExtensionForFormatName
//----------------------------------------------------------------------------------------
PMString
CZExpDocXMLExportProvider::GetExtensionForFormatName(
	const PMString&			inFormatName)
{
	if( inFormatName == kIZExpXMLFormatStringKey )
		return "xml";
	else
		return kNullString;
}

#ifdef InDnCS5
/**
 Returns the ScriptID for the given format's enumerator in the kExportFormatEnumScriptElement
 @param formatName	IN Format name whose ScriptID to return
 @param context is the scripting request context
 @return the ScriptID (or kInvalidScriptID if this format is not supported by enum)
 */
ScriptID
CZExpDocXMLExportProvider::GetScriptIDForFormatName(
	const PMString&						inFormatName,
	const ScriptInfo::RequestContext&	inContext)
{
	return kInvalidScriptID;
}
#endif
