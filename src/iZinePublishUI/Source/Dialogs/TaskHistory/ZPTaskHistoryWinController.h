//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/ZPTaskHistoryWinController.h $
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

#import <Cocoa/Cocoa.h>


@interface ZPTaskHistoryWinController : NSWindowController {

	IBOutlet NSTextView    *mTextView;
}

-(void) setHistoryContents: ( const unsigned char * ) inHistoryContent;

-(void) setHistoryContentsWithData: ( NSData * ) inHistoryContent;

-(void) loadWindowNib;

-(NSString*) getNibName;

@end
