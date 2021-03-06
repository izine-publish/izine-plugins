//========================================================================================
//  
//  $File: //depot/indesign_CS5.5/source/sdksamples/paneltreeview/PlatformFileSystemIterator.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2010/02/23 03:46:08 $
//  
//  $Revision: #1 $
//  
//  $Change: 742150 $
//  
//  Copyright 1997-2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//  
//  Purpose:
//  
//========================================================================================

#ifndef __PlatformFileSystemIterator_H_DEFINED__
#define __PlatformFileSystemIterator_H_DEFINED__



#ifdef WINDOWS
#include "WinFileSystemIterator.h"
#define PlatformFileSystemIterator WinFileSystemIterator
#endif
#ifdef MACINTOSH
#include "MacFileSystemIterator.h"
#define PlatformFileSystemIterator MacFileSystemIterator
#endif


#endif

