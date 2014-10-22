//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/ZPTaskHistoryWinController.mm $
//	$Revision: 3001 $
//	$Date: 2011-05-11 15:00:05 +0200 (Wed, 11 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#import "ZPTaskHistoryWinController.h"

#include "ZPTaskHistoryWinMgr.h"

ZPTaskHistoryWinController * gTaskWinController =  nil;


@implementation ZPTaskHistoryWinController

//- (void)awakeFromNib
//{
//	[super awakeFromNib];
//}

- (id)init
{
	// initialize NSApplication, using an entry point that is specific to bundles
	// this is a no-op for Cocoa apps, but is required by Carbon apps
	NSApplicationLoad();
	NSLog(@"Application Loaded\n");
	
	self = [super init];
	
	return self;
}

- (void)dealloc
{
	NSLog(@"Removing task history window conroller\n");
	gTaskWinController = nil;
	[super dealloc];
}

-(void) setHistoryContents: ( const unsigned char * ) inHistoryContent
{
	//[mTextView setString: @"this is history"];
	NSMutableAttributedString * htmlContentString = [NSMutableAttributedString alloc];
	[htmlContentString initWithHTML: [NSData dataWithBytes: inHistoryContent length:strlen((const char*)inHistoryContent) ] documentAttributes: nil ];
	
	[[mTextView textStorage] setAttributedString: htmlContentString];
}

-(void) setHistoryContentsWithData: ( NSData * ) inHistoryContent
{
	NSMutableAttributedString * htmlContentString = [NSMutableAttributedString alloc];
	[htmlContentString initWithHTML: inHistoryContent documentAttributes: nil ];
	
	[[mTextView textStorage] setAttributedString: htmlContentString];
}


-(void) loadWindowNib
{
	NSLog(@"Loading resource\n");
	//[self initWithWindowNibName: [self getNibName] ];
	if (![NSBundle loadNibNamed: [self getNibName] owner: self])
	{
		NSLog(@"Failed to load Task history resource.");
		return;
	}
	[self showWindow: self];
}

- (void)windowWillClose:(NSNotification *)notification;
{
	NSLog(@"history window will close\n");
	[self release];
	HandleHistoryPanelClose();
}

-(NSString*) getNibName
{
	return @"TaskHistory";
}

//It must release the data
-(void)
LoadAndShowTaskHistoryWindow:
(NSData *)		inHistoryContent 
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];		//All functions called from Cpp must have auto release pool
	[gTaskWinController loadWindowNib];
	
	[inHistoryContent autorelease];
	[gTaskWinController setHistoryContentsWithData: inHistoryContent];
	[gTaskWinController showWindow: gTaskWinController];
	[[gTaskWinController window] orderFront: gTaskWinController];
	[pool release];
}

@end


//Called by Cpp code.
void
ShowTaskHistoryWindow(
	const unsigned char *		inHistoryContent )
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];		//All functions called from Cpp must have auto release pool
	if( !gTaskWinController )
	{
		NSLog(@"Creating new controller\n");
		gTaskWinController = [ZPTaskHistoryWinController alloc];
		[gTaskWinController init];
		//TODO: find if current thread is main thread or not.
		[gTaskWinController loadWindowNib];
		
		//This is run on main thread as loadWindowNib throws exception here.
		//NSData * str = [NSData dataWithBytes: inHistoryContent length:strlen((const char*)inHistoryContent)];
		//[gTaskWinController performSelectorOnMainThread: @selector(LoadAndShowTaskHistoryWindow:) withObject:[str retain] waitUntilDone: NO];
		
		//return;
	}
	
	[gTaskWinController setHistoryContents: inHistoryContent];
	[gTaskWinController showWindow: gTaskWinController];
	[[gTaskWinController window] orderFront: gTaskWinController];
	//[[gTaskWinController window] makeKeyWindow ];
	[pool release];
}
