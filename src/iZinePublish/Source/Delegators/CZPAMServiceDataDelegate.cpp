//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Delegators/CZPAMServiceDataDelegate.cpp $
//	$Revision: 3962 $
//	$Date: 2012-02-28 11:21:39 +0100 (Tue, 28 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"

//SDK General includes
#include "FileUtils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL	//TODO: create a type for delegate
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IAMServiceData>
class CZPAMServiceDataDelegate : public inheritClass
{
public:
						CZPAMServiceDataDelegate(IPMUnknown * boss);
	virtual				~CZPAMServiceDataDelegate();

	/** Retrieves a service data parameter of type IDFile.
		@param dataID is a unique key to identify the data.
		@param outFile will be populated with the IDFile.
		@return kTrue if the data was found; kFalse otherwise.
	*/	
	virtual bool16 Get( int32 dataID, IDFile & outFile ) const;
	
	/** Retrieves a service data parameter of type int32.
		@param dataID is a unique key to identify the data.
		@param outInt32 will be populated with the int32.
		@return kTrue if the data was found; kFalse otherwise.
	*/	
	virtual bool16 Get( int32 dataID, int32 & outInt32 ) const;
	
	/** Retrieves a service data parameter of type void *.
		@param dataID is a unique key to identify the pointer data.
		@return non-nil void * pointer if the data was found; nil otherwise.
	*/	
	virtual void * Get( int32 dataID ) const;
	
	/** Retrieves a service data parameter of type PMString.
		@param dataID is a unique key to identify the data.
		@param outPMString will be populated with the PMString.
		@return kTrue if the data was found; kFalse otherwise.
	*/		
	virtual bool16 Get( int32 dataID, PMString & outPMString ) const;
	
	/** Retrieves a service data parameter of type UIDRef.
		@param dataID is a unique key to identify the data.
		@param outUIDRef will be populated with the UIDRef.
		@return kTrue if the data was found; kFalse otherwise.
	*/		
	virtual bool16 Get( int32 dataID, UIDRef & outUIDRef ) const;
		
	/** Retrieves a service data parameter of type UIFlags.
		@param dataID is a unique key to identify the data.
		@param outUiFlags will be populated with the UIFlags.
		@return kTrue if the data was found; kFalse otherwise.
	*/
	virtual bool16 Get( int32 dataID, UIFlags & outUiFlags ) const;
		
	/** Retrieves a service data parameter of type IAMUIService::FollowupAction.
		@param dataID is a unique key to identify the data.
		@param outAction will be populated with the IAMUIService::FollowupAction.
		@return kTrue if the data was found; kFalse otherwise.
	*/	
	virtual bool16 Get( int32 dataID, IAMUIService::FollowupAction & outAction ) const;
		
	/** Retrieves an IAMService::enTargetAsset service data parameter of type IAMServiceData::UIDRefList.
		@param outUIDRefList will be populated with one or more UIDRefs.
		@return kTrue if the data was found; kFalse otherwise.
	*/		
	virtual bool16 Get( IAMServiceData::UIDRefList & outUIDRefList );
	
	/** Retrieves an IAMService::enTargetAsset service data parameter of type IAMServiceData::IDFileList.
		@param outIDFileList will be populated with one or more IDFiles.
		@return kTrue if the data was found; kFalse otherwise.
	*/		
	virtual bool16 Get( IAMServiceData::IDFileList & outIDFileList );
	
	/** Retrieves a binary flag value (kTrue or kFalse).
		@param dataID is a unique key to identify the flag.
		@return kTrue if the flag is set; kFalse otherwise.
	*/		
	virtual bool16 GetFlag( int32 dataID ) const;


	/** Sets the value of a service data parameter of type IDFile.
		@param dataID is a unique key to identify the data.
		@param inFile the contents of this are copied and saved.
	*/	
	virtual void Set( int32 dataID, const IDFile & inFile );
	
	/** Sets the value of a service data parameter of type int32.
		@param dataID is a unique key to identify the data.
		@param inInt32 the contents of this are copied and saved.
	*/	
	virtual void Set( int32 dataID, int32 inInt32 );

	/** Sets the value of a service data parameter of type void *.
		@param dataID is a unique key to identify the pointer.
		@param inInterface the contents of this are copied and saved.
	*/		
	virtual void Set( int32 dataID, void * inInterface );
	
	/** Sets the value of a service data parameter of type PMString.
		@param dataID is a unique key to identify the data.
		@param inPMString the contents of this are copied and saved.
	*/	
	virtual void Set( int32 dataID, const PMString & inPMString );
	
	/** Sets the value of a service data parameter of type UIDRef.
		@param dataID is a unique key to identify the data.
		@param inUIDRef the contents of this are copied and saved.
	*/	
	virtual void Set( int32 dataID, const UIDRef & inUIDRef );
	
	/** Sets the value of a service data parameter of type UIFlags.
		@param dataID is a unique key to identify the data.
		@param inUiFlags the contents of this are copied and saved.
	*/		
	virtual void Set( int32 dataID, UIFlags inUIFlags );
	
	/** Sets the value of a service data parameter of type IAMUIService::FollowupAction.
		@param dataID is a unique key to identify the data.
		@param inAction the contents of this are copied and saved.
	*/		
	virtual void Set( int32 dataID, IAMUIService::FollowupAction inAction );
	
	/** Sets the value of a service data parameter of type IAMServiceData::UIDRefList. The dataID
		is implicitly IAMService::enTargetAsset.
		@param inUIDRefList the contents of this list are copied and saved.
	*/	
	virtual void Set( const IAMServiceData::UIDRefList & inUIDRefList );
	
	/** Sets the value of a service data parameter of type IAMServiceData::IDFileList. The dataID
		is implicitly IAMService::enTargetAsset.
		@param inIDFileList the contents of this list are copied and saved.
	*/		
	virtual void Set( const IAMServiceData::IDFileList & inIDFileList );
	
	/** Sets or clears a flag service data parameter.
		@param dataID is a unique key to identify the flag.
		@param b flag value; specify kTrue to set the flag, or kFalse to clear it.
	*/	
	virtual void SetFlag( int32 dataID, bool16 b = kTrue );


protected:
private:
	PMIID		mDelegateIID;
};

CREATE_PMINTERFACE(CZPAMServiceDataDelegate, kZPAMServiceDataDelegateImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAMServiceDataDelegate::CZPAMServiceDataDelegate(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZPAMSERVICEDATADELEGATE )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMServiceDataDelegate::~CZPAMServiceDataDelegate()
{

}

#pragma mark -
/** Retrieves a service data parameter of type IDFile.
	@param dataID is a unique key to identify the data.
	@param outFile will be populated with the IDFile.
	@return kTrue if the data was found; kFalse otherwise.
*/	
bool16
CZPAMServiceDataDelegate::Get(
	int32 dataID, IDFile & outFile) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outFile );
		PMString filePath;
		if( toReturn )
			filePath = FileUtils::SysFileToPMString(outFile);
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, file = %s", toReturn, dataID, filePath.GrabCString() );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves a service data parameter of type int32.
	@param dataID is a unique key to identify the data.
	@param outInt32 will be populated with the int32.
	@return kTrue if the data was found; kFalse otherwise.
*/	
bool16
CZPAMServiceDataDelegate::Get(
	int32 dataID, int32 & outInt32) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outInt32 );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, int32 0x%08X", toReturn, dataID, outInt32 );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves a service data parameter of type void *.
	@param dataID is a unique key to identify the pointer data.
	@return non-nil void * pointer if the data was found; nil otherwise.
*/	
void *
CZPAMServiceDataDelegate::Get(
	int32 dataID) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		void * toReturn = orgImpl->Get( dataID );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, ptr = 0x%08X", toReturn, dataID, toReturn );
		return toReturn;
	}
	return ( void * )0;
}
	
/** Retrieves a service data parameter of type PMString.
	@param dataID is a unique key to identify the data.
	@param outPMString will be populated with the PMString.
	@return kTrue if the data was found; kFalse otherwise.
*/		
bool16
CZPAMServiceDataDelegate::Get(
	int32 dataID, PMString & outPMString) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outPMString );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, string = %s", toReturn, dataID, outPMString.GrabCString() );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves a service data parameter of type UIDRef.
	@param dataID is a unique key to identify the data.
	@param outUIDRef will be populated with the UIDRef.
	@return kTrue if the data was found; kFalse otherwise.
*/		
bool16
CZPAMServiceDataDelegate::Get(
	int32 dataID, UIDRef & outUIDRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outUIDRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, UID = 0x%08X", toReturn, dataID, outUIDRef.GetUID().Get() );
		return toReturn;
	}
	return ( bool16 )0;
}

	
/** Retrieves a service data parameter of type UIFlags.
	@param dataID is a unique key to identify the data.
	@param outUiFlags will be populated with the UIFlags.
	@return kTrue if the data was found; kFalse otherwise.
*/
bool16
CZPAMServiceDataDelegate::Get(
	int32 dataID, UIFlags & outUiFlags) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outUiFlags );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, UIFlags = 0x%08X", toReturn, dataID, outUiFlags );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves a service data parameter of type IAMUIService::FollowupAction.
	@param dataID is a unique key to identify the data.
	@param outAction will be populated with the IAMUIService::FollowupAction.
	@return kTrue if the data was found; kFalse otherwise.
*/	
bool16
CZPAMServiceDataDelegate::Get(
							  int32 dataID, IAMUIService::FollowupAction & outAction) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( dataID, outAction );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, dataID = 0x%08X, FollowupAction 0x%08X", toReturn, dataID, outAction );
		return toReturn;
	}
	return ( bool16 )0;
}

	
/** Retrieves an IAMService::enTargetAsset service data parameter of type IAMServiceData::UIDRefList.
	@param outUIDRefList will be populated with one or more UIDRefs.
	@return kTrue if the data was found; kFalse otherwise.
*/		
bool16
CZPAMServiceDataDelegate::Get(
							  IAMServiceData::UIDRefList & outUIDRefList)
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( outUIDRefList );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, UIDRefList count = %d", toReturn, outUIDRefList.size() );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves an IAMService::enTargetAsset service data parameter of type IAMServiceData::IDFileList.
	@param outIDFileList will be populated with one or more IDFiles.
	@return kTrue if the data was found; kFalse otherwise.
*/		
bool16
CZPAMServiceDataDelegate::Get(
							  IAMServiceData::IDFileList & outIDFileList)
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Get( outIDFileList );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd, File list count = %d", toReturn, outIDFileList.size() );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Retrieves a binary flag value (kTrue or kFalse).
	@param dataID is a unique key to identify the flag.
	@return kTrue if the flag is set; kFalse otherwise.
*/		
bool16
CZPAMServiceDataDelegate::GetFlag(
	int32 dataID) const
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetFlag( dataID );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : flag %hd, dataID = 0x%08X", toReturn, dataID );
		return toReturn;
	}
	return ( bool16 )0;
}

/** Sets the value of a service data parameter of type IDFile.
	@param dataID is a unique key to identify the data.
	@param inFile the contents of this are copied and saved.
*/	
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, const IDFile & inFile)
{
	LogFunctionEnterExit;

	PMString filePath = FileUtils::SysFileToPMString(inFile);
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, file = %s", dataID, filePath.GrabCString() );

	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inFile );
}

/** Sets the value of a service data parameter of type int32.
	@param dataID is a unique key to identify the data.
	@param inInt32 the contents of this are copied and saved.
*/	
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, int32 inInt32)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, int32 0x%08X", dataID, inInt32 );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inInt32 );
}

/** Sets the value of a service data parameter of type void *.
	@param dataID is a unique key to identify the pointer.
	@param inInterface the contents of this are copied and saved.
*/		
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, void * inInterface)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, ptr = 0x%08X", dataID, inInterface );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inInterface );
}

/** Sets the value of a service data parameter of type PMString.
	@param dataID is a unique key to identify the data.
	@param inPMString the contents of this are copied and saved.
*/	
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, const PMString & inPMString)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, string = %s", dataID, inPMString.GrabCString() );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inPMString );
}

/** Sets the value of a service data parameter of type UIDRef.
	@param dataID is a unique key to identify the data.
	@param inUIDRef the contents of this are copied and saved.
*/	
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, const UIDRef & inUIDRef)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, UID = 0x%08X", dataID, inUIDRef.GetUID().Get() );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inUIDRef );
}

/** Sets the value of a service data parameter of type UIFlags.
	@param dataID is a unique key to identify the data.
	@param inUiFlags the contents of this are copied and saved.
*/		
void
CZPAMServiceDataDelegate::Set(
	int32 dataID, UIFlags inUIFlags)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, UIFlags = 0x%08X", dataID, inUIFlags );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inUIFlags );
}

/** Sets the value of a service data parameter of type IAMUIService::FollowupAction.
	@param dataID is a unique key to identify the data.
	@param inAction the contents of this are copied and saved.
*/		
void
CZPAMServiceDataDelegate::Set(
							  int32 dataID, IAMUIService::FollowupAction inAction)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, FollowupAction 0x%08X", dataID, inAction );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( dataID, inAction );
}

/** Sets the value of a service data parameter of type IAMServiceData::UIDRefList. The dataID
	is implicitly IAMService::enTargetAsset.
	@param inUIDRefList the contents of this list are copied and saved.
*/	
void
CZPAMServiceDataDelegate::Set(
							  const IAMServiceData::UIDRefList & inUIDRefList)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : UIDRefList count = %d", inUIDRefList.size() );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( inUIDRefList );
}

/** Sets the value of a service data parameter of type IAMServiceData::IDFileList. The dataID
	is implicitly IAMService::enTargetAsset.
	@param inIDFileList the contents of this list are copied and saved.
*/		
void
CZPAMServiceDataDelegate::Set(
							  const IAMServiceData::IDFileList & inIDFileList)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : File list count = %d", inIDFileList.size() );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->Set( inIDFileList );
}

/** Sets or clears a flag service data parameter.
	@param dataID is a unique key to identify the flag.
	@param b flag value; specify kTrue to set the flag, or kFalse to clear it.
*/	
void
CZPAMServiceDataDelegate::SetFlag(
	int32 dataID, bool16 b)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "In : dataID = 0x%08X, flag %hd", dataID, b );
	InterfacePtr<IAMServiceData> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->SetFlag( dataID, b );
}
