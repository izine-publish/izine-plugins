//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPAppEarlyInitializer.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IInitializer.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IInitializer>
class CZPAppEarlyInitializer : public inheritClass
{
public:
						CZPAppEarlyInitializer(IPMUnknown * boss);
	virtual				~CZPAppEarlyInitializer();

		/** Return wheter this initializer is an early or late initializer. That is,
		//	is it called before of after the service registry. **/
	InitializerType		GetType(void) const;

		/** Open is called before plug-ins are initialized to allow
			the initializer to set up state that is reused across
			multiple calls to Register. The server will call Open, followed by
			zero or more Register calls, followed by a Close. 
		@param localeId		The locale we're registering
		*/
	void				Open(
							const PMLocaleId&			localeId);

		/** Close is called before plug-ins are initialized to allow
			the initializer to set up and tear down state that is reused across
			multiple calls to Register. The server will call Open, followed by
			zero or more Register calls, followed by a Close. 
		@param localeId		The locale we're registering
		*/
	void				Close(
							const PMLocaleId&			localeId);
	
		/** Called by application once for each plug-in, to register the resources
			supplied by the plug-in. Resource file will already be open. 
		@param plugin		The plug-in we're registering
		@param localeId		The locale we're registering
		*/
	void				Register(
							PluginID					plugin,
							const PMLocaleId&			localeId);

//	virtual Unregister(PluginID plugin) = 0;

		/** Each initializer is called once for each plug-in, and the priority
			controls the order in which the initializers are called for each 
			plug-in. Lower numbers means this initializer is run first; higher 
			numbers move initializer to later in the startup sequence. 
		@return The initializer's priority: specifies whether it is called after or before other initializers.
		*/
	PMReal				Priority() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPAppEarlyInitializer, kZPAppEarlyInitializerImpl)

CZPAppEarlyInitializer::CZPAppEarlyInitializer(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAppEarlyInitializer::~CZPAppEarlyInitializer()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetType
//----------------------------------------------------------------------------------------
IInitializer::InitializerType
CZPAppEarlyInitializer::GetType(void) const
{
	//LogFunctionEnterExit;
	return IInitializer::kEarly;
}

//----------------------------------------------------------------------------------------
// Open
//----------------------------------------------------------------------------------------
void
CZPAppEarlyInitializer::Open(
	const PMLocaleId&			localeId)
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPApp> zpApp( gSession, UseDefaultIID() );	//This will create app object. gZPApp will be nil before this.
	ASSERT( zpApp );

	zpApp->Initialize();
}

//----------------------------------------------------------------------------------------
// Close
//----------------------------------------------------------------------------------------
void
CZPAppEarlyInitializer::Close(
	const PMLocaleId&			localeId)
{
}

//----------------------------------------------------------------------------------------
// Register
//----------------------------------------------------------------------------------------
void
CZPAppEarlyInitializer::Register(
	PluginID					plugin,
								const PMLocaleId&			localeId)
{
}

//----------------------------------------------------------------------------------------
// Priority
//----------------------------------------------------------------------------------------
PMReal
CZPAppEarlyInitializer::Priority()const
{
	//Priority of intializer within plugin. No meaning till we have more than 1 initializer.
	//App must be intialized as early as possible.
	return 1;
}
