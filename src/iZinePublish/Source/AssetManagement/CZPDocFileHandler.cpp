//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPDocFileHandler.cpp $
//	$Revision: 3567 $
//	$Date: 2011-08-04 12:24:05 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDocFileHandler.h"
#include "IInCopyDocUtils.h"
#include "IStandAloneDoc.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IDocFileHandler>
class CZPDocFileHandler : public inheritClass
{
public:
						CZPDocFileHandler(IPMUnknown * boss);
	virtual				~CZPDocFileHandler();

		// Should return kTrue if the document is modified or unsaved.
		bool8 CanSave(const UIDRef& doc);		
		// Save the document.
		// If it is unsaved Save should call SaveAs
		void Save(const UIDRef& doc, UIFlags uiFlags = kFullUI);

		// Can save as.
		bool8 CanSaveAs(const UIDRef& doc);
		// Save the doc as destFile.
		// If destFile is nil SaveAs should ask the user (if uiFlags allow) or fail.
		// The asStationery flag should be considered as Mac only and will only be honored
		// if destFile is not nil.
		void SaveAs(const UIDRef& doc, const IDFile *destFile = nil,
												UIFlags uiFlags = kFullUI,
												bool8 asStationery = kFalse,
												FileTypeInfoID fileTypeID = kInvalidFileTypeInfoID);

		// Can save a copy.
		bool8 CanSaveACopy(const UIDRef& doc);
		// Save a copy of the doc as destFile.
		// If destFile is nil SaveACopy should ask the user (if uiFlags allow) or fail.
		// The asStationery flag should be considered as Mac only and will only be honored
		// if destFile is not nil.
		void SaveACopy(const UIDRef& doc, const IDFile *destFile = nil,
												UIFlags uiFlags = kFullUI,
												bool8 asStationery = kFalse,
												FileTypeInfoID fileTypeID = kInvalidFileTypeInfoID);

		// Can revert should return kFalse if the doc is unmodified or unsaved.
		bool8 CanRevert(const UIDRef& doc);
		// Revert should ask the user (if uiFlags allow).
		void Revert(const UIDRef& doc, UIFlags uiFlags = kFullUI);

		// Can close.
		bool8 CanClose(const UIDRef& doc);
		// Close should close any open window and finaly the document.
		// If uiFlags allow the user should get a chance to save a modified document.

		void Close(const UIDRef& doc, UIFlags uiFlags = kFullUI, bool8 allowCancel = kTrue, CloseCmdMode cmdMode = kSchedule);


		// Methods implemented in a standard way by the default DocFileHandler. Other handlers
		// should call these to insure consistent behavior.

		// Check to see if the file is dirty on close and, if so, ask the user whether
		// it should be saved.
		CloseOptions CheckOnClose(const UIDRef& doc, UIFlags uiFlags, bool8 allowCancel);
		// Get the appropriate default SaveAs file name.
		void GetCopyDefaultName(const UIDRef& doc, IDFile *name, bool16& useSystemDefaultDir);
protected:
private:
};


CREATE_PMINTERFACE(CZPDocFileHandler, kZPDocFileHandlerImpl)

CZPDocFileHandler::CZPDocFileHandler(IPMUnknown * boss)
: inheritClass(boss)
{
	ASSERT(Utils<IZPInDnUtils>()->IsHostAppInCopy());
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDocFileHandler::~CZPDocFileHandler()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanSave
//----------------------------------------------------------------------------------------
bool8
CZPDocFileHandler::CanSave(
	const UIDRef& doc)
{
	LogFunctionEnterExit;
	return Utils<IInCopyDocUtils>()->CanDoSave(doc);
}

//----------------------------------------------------------------------------------------
// Save
//----------------------------------------------------------------------------------------
void
CZPDocFileHandler::Save(
	const UIDRef& doc, UIFlags uiFlags)
{
	LogFunctionEnterExit;
	Utils<IInCopyDocUtils>()->DoSaveAll(doc, uiFlags);
}
//----------------------------------------------------------------------------------------
// CanSaveAs
//----------------------------------------------------------------------------------------
bool8
CZPDocFileHandler::CanSaveAs(
	const UIDRef& doc)
{
	LogFunctionEnterExit;
	//----- Save as only available for standalone docs
	InterfacePtr<IStandAloneDoc> standAlone(doc, IID_ISTANDALONEDOC);
	if (standAlone)
		return standAlone->IsStandAloneDoc();// && !this->IsStandAloneGraphicDoc(doc);
		
	return kFalse;
}
//----------------------------------------------------------------------------------------
// SaveAs
//----------------------------------------------------------------------------------------
void
CZPDocFileHandler::SaveAs(
	const UIDRef& doc, const IDFile *destFile,
	UIFlags uiFlags,
	bool8 asStationery,
	FileTypeInfoID fileTypeID)
{
	LogFunctionEnterExit;
	//----- We don't do stationary in InCopy
	Utils<IInCopyDocUtils>()->DoSaveAs(doc, kFalse, destFile, uiFlags, kFalse, fileTypeID);
}
//----------------------------------------------------------------------------------------
// CanSaveACopy
//----------------------------------------------------------------------------------------
bool8
CZPDocFileHandler::CanSaveACopy(
	const UIDRef& doc)
{
	LogFunctionEnterExit;
	//----- Save a copy only available for standalone docs
	InterfacePtr<IStandAloneDoc> standAlone(doc, IID_ISTANDALONEDOC);
	if (standAlone)
		return standAlone->IsStandAloneDoc();// && !this->IsStandAloneGraphicDoc(doc);
		
	return kFalse;
}

//----------------------------------------------------------------------------------------
// SaveACopy
//----------------------------------------------------------------------------------------
void
CZPDocFileHandler::SaveACopy(
	const UIDRef& doc, const IDFile *destFile,
	UIFlags uiFlags,
	bool8 asStationery,
	FileTypeInfoID fileTypeID)
{
	LogFunctionEnterExit;
	//----- We don't do stationary in InCopy
	Utils<IInCopyDocUtils>()->DoSaveAs(doc, kTrue, destFile, uiFlags, kFalse, fileTypeID);
}


//----------------------------------------------------------------------------------------
// CanRevert
//----------------------------------------------------------------------------------------
bool8
CZPDocFileHandler::CanRevert(
	const UIDRef& doc)
{
	return Utils<IInCopyDocUtils>()->CanDoRevert(doc);
}

//----------------------------------------------------------------------------------------
// Revert
//----------------------------------------------------------------------------------------
void
CZPDocFileHandler::Revert(
	const UIDRef& doc, UIFlags uiFlags)
{
	LogFunctionEnterExit;
	Utils<IInCopyDocUtils>()->DoRevert(doc, uiFlags);
}

//----------------------------------------------------------------------------------------
// CanClose
//----------------------------------------------------------------------------------------
bool8
CZPDocFileHandler::CanClose(
	const UIDRef& doc)
{
	LogFunctionEnterExit;
	return kTrue;
}
//----------------------------------------------------------------------------------------
// Close
//----------------------------------------------------------------------------------------
//	This hackery is intended to suppress a cascade of dialogs which can happen when 
//	throwing your open and modified standalone file into the trash.  When that happens, a UI event from the system
//	makes us notice that our files were moved, so we close them, making the "do you want to save" dialog
//	appear. This causes a UI event which makes us notice that our files were moved, so we close them,
//	making the "do you want to save" dialog layer on top of the previous one, ad nausium.
//
//	So, we keep a little list of database pointers.  You only get called to close if you aren't in the list.
//	Then we put you in the list and call close, where the dialog hangs and we get reentered.  The next time,
//	you are already in the list, so you don't get Close called and everything is fine.  It's a list because
//	you could have several documents open and tossed into the trash all at once.
//
namespace icdfh
{
	K2Vector<IDataBase*> dbList;
}
void
CZPDocFileHandler::Close(
	const UIDRef& doc, UIFlags uiFlags, bool8 allowCancel, CloseCmdMode cmdMode)
{
	LogFunctionEnterExit;
	IDataBase* db = doc.GetDataBase();
	K2Vector<IDataBase*>::iterator dbiter = std::find(icdfh::dbList.begin(), icdfh::dbList.end(), db);
	if (dbiter == icdfh::dbList.end())
	{
		icdfh::dbList.push_back(db);

		Utils<IInCopyDocUtils>()->DoClose(doc, uiFlags, allowCancel, cmdMode);

		K2Vector<IDataBase*>::iterator dbitez = std::find(icdfh::dbList.begin(), icdfh::dbList.end(), db);
		icdfh::dbList.erase(dbitez);
	}
}

//----------------------------------------------------------------------------------------
// CheckOnClose
//----------------------------------------------------------------------------------------
IDocFileHandler::CloseOptions
CZPDocFileHandler::CheckOnClose(
	const UIDRef& doc, UIFlags uiFlags, bool8 allowCancel)
{
	LogFunctionEnterExit;
	return Utils<IInCopyDocUtils>()->CheckOnClose(doc, uiFlags, allowCancel);
}

//----------------------------------------------------------------------------------------
// GetCopyDefaultName
//----------------------------------------------------------------------------------------
void
CZPDocFileHandler::GetCopyDefaultName(
	const UIDRef& doc, IDFile *name, bool16& useSystemDefaultDir)
{
	LogFunctionEnterExit;
	Utils<IInCopyDocUtils>()->GetCopyDefaultName(doc, name, useSystemDefaultDir);
}
