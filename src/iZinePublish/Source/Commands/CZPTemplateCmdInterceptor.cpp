//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPTemplateCmdInterceptor.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ICommandInterceptor.h"
#include "IDocument.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "assignmentID.h"
#include "InCopyBridgeUIID.h"
#include "LinksID.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ICommandInterceptor>
class CZPTemplateCmdInterceptor : public inheritClass
{
public:
						CZPTemplateCmdInterceptor(IPMUnknown * boss);
	virtual				~CZPTemplateCmdInterceptor();

	InterceptResult		InterceptProcessCommand(ICommand *cmd);
	InterceptResult		InterceptScheduleCommand(ICommand *cmd) { return kCmdNotHandled; }
	
	InterceptResult		InterceptExecuteDynamic(ICommand * cmd) { return kCmdNotHandled; }

	//InterceptResult		InterceptUndoDynamic(ICommand * cmd) { return kCmdNotHandled; }
	//InterceptResult		InterceptRedoDynamic(ICommand * cmd) { return kCmdNotHandled; }

	// install/deinstall itself as command interceptor
	void		InstallSelf();
	void		DeinstallSelf();

	InterceptResult		InterceptExecuteImmediate(ICommand * cmd) { return kCmdNotHandled; }

protected:
	IDocument *			GetTargetDocument(ICommand * cmd) const;
	
private:
};


CREATE_PMINTERFACE(CZPTemplateCmdInterceptor, kZPTemplateCmdInterceptorImpl)

CZPTemplateCmdInterceptor::CZPTemplateCmdInterceptor(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTemplateCmdInterceptor::~CZPTemplateCmdInterceptor()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InterceptProcessCommand
//----------------------------------------------------------------------------------------
ICommandInterceptor::InterceptResult
CZPTemplateCmdInterceptor::InterceptProcessCommand(
	ICommand *cmd)
{
	//LogFunctionEnterExit;
	InterceptResult result = kCmdNotHandled;
	ClassID cmdClass = GetClass( cmd );
	do
	{
		if(( cmdClass == kAssignDocCmdBoss )
		||( cmdClass == kRemoveLinksCmdBoss ) )
		{
			LogFunctionEnterExit;

			//Check if this document is from our replica folder or not.

			const IDocument * theTargetDoc = this->GetTargetDocument( cmd );
			if( !theTargetDoc )
				return result;

			InterfacePtr<const IDocument> selfDoc( this, UseDefaultIID() );
			ASSERT( selfDoc );
			if( selfDoc != theTargetDoc )
				return result;	//Will be handled by some other interceptor

			result = kCmdHandled;
		}
	}while(kFalse);

	return result;
}


//----------------------------------------------------------------------------------------
// InstallSelf
//----------------------------------------------------------------------------------------
void
CZPTemplateCmdInterceptor::InstallSelf()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommandProcessor> proc ( gSession->QueryCommandProcessor() );
	proc->InstallInterceptor( this );
}


//----------------------------------------------------------------------------------------
// DeinstallSelf
//----------------------------------------------------------------------------------------
void
CZPTemplateCmdInterceptor::DeinstallSelf()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommandProcessor> proc ( gSession->QueryCommandProcessor() );
	proc->DeinstallInterceptor( this );
}

//----------------------------------------------------------------------------------------
// GetTargetDocument
//----------------------------------------------------------------------------------------
IDocument *
CZPTemplateCmdInterceptor::GetTargetDocument(
	ICommand * cmd )const
{
	//LogFunctionEnterExit;
	IDocument * toReturn = nil;

	const UIDRef & cmdTarget = cmd->GetTarget();

	InterfacePtr<IDocument> targetDoc( cmdTarget, UseDefaultIID() );
	toReturn = targetDoc;

	return toReturn;
}