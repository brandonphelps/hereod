

#ifndef VIDEO_H
#define VIDEO_H

#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>

typedef struct VideoData
{
	
	
} VideoData;

@interface Cocoa_WindowListener : NSResponder <NSWindowDelegate> {
	BOOL observingVisible;
}

-(void) listen:(int* ) data;

@end

struct WindowData
{
	NSWindow* nswindow;
	NSView* sdlContentView;
	NSMutableArray* nscontexts;
	Cocoa_WindowListener* listener;
	struct VideoData* videodata;
};


#endif
