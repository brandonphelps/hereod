#include <stdio.h>

#include "video.h"

static bool Running = true;

void RefreshBuf(NSWindow* window, ScreenData& screendata)
{
  if(screendata.buffer)
  {
    free(screendata.buffer);
  }
  screendata.width = window.contentView.bounds.size.width;
  screendata.height = window.contentView.bounds.size.height;
  screendata.pitch = screendata.width * screendata.bytesPerPixel;
  screendata.buffer = (uint8_t*)malloc(screendata.pitch * screendata.height);
}

@interface MainWindowDelegate: NSObject<NSWindowDelegate>
@property ScreenData *screenRef;

@end

@implementation MainWindowDelegate
-(void)windowWillClose:(id)sender {
  Running = false;
}

-(void)windowDidResize:(NSNotification*) notification {
  NSWindow* window = (NSWindow*)notification.object;
  RefreshBuf(window, *_screenRef);
}
@end

uint32_t globalRenderWidth = 1078U;
uint32_t globalRenderHeight = 768U;


void ReDrawBuf(NSWindow* window, uint8_t* buffer, uint32_t bitmapWidth, uint32_t bitmapHeight, uint32_t pitch)
{
  @autoreleasepool { 
    NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
				initWithBitmapDataPlanes: &buffer
					      pixelsWide: bitmapWidth
					      pixelsHigh: bitmapHeight
					   bitsPerSample: 8
					 samplesPerPixel: 4
						hasAlpha: YES
						isPlanar: NO
					  colorSpaceName: NSDeviceRGBColorSpace
					     bytesPerRow: pitch
					    bitsPerPixel: 4 * 8] autorelease];
  
    NSSize imageSize = NSMakeSize(bitmapWidth, bitmapHeight);
    NSImage* image = [[[NSImage alloc] initWithSize: imageSize] autorelease];
    [image addRepresentation: rep];
    window.contentView.layer.contents = image;
  }
}

int main(int argc, const char* argv[])
{
  MainWindowDelegate* MainDele = [[MainWindowDelegate alloc] init];

  NSRect screenRect = [[NSScreen mainScreen] frame];

  NSRect initialFrame = NSMakeRect((screenRect.size.width - globalRenderWidth) * 0.5,
				   (screenRect.size.height - globalRenderHeight) * 0.5,
				   globalRenderWidth,
				   globalRenderHeight);

  NSWindow* window = [[NSWindow alloc] initWithContentRect : initialFrame
						 styleMask : NSWindowStyleMaskTitled |
				       NSWindowStyleMaskClosable |
				       NSWindowStyleMaskMiniaturizable |
				       NSWindowStyleMaskResizable
						   backing : NSBackingStoreBuffered
						     defer : NO];

  [window setBackgroundColor: NSColor.redColor];
  [window setTitle: @"Hello World"];
  [window makeKeyAndOrderFront: nil];
  [window setDelegate: MainDele];
  window.contentView.wantsLayer = YES;

  ScreenData currentScreen;
  currentScreen.buffer = 0;
  MainDele.screenRef = &currentScreen;
  currentScreen.bytesPerPixel = 4;
  RefreshBuf(window, currentScreen);

  while(Running) {
    // updates the temporary buffer with data.
    drawBuf(currentScreen.buffer, currentScreen.width, currentScreen.height, currentScreen.pitch);

    // takes the buffer data and puts it onto the screen.
    ReDrawBuf(window, currentScreen.buffer, currentScreen.width, currentScreen.height, currentScreen.pitch);

    NSEvent* event;
    do {
      event = [NSApp nextEventMatchingMask: NSEventMaskAny
				 untilDate: nil
				    inMode: NSDefaultRunLoopMode
				   dequeue: YES];
      switch([event type]) {
      default:
	[NSApp sendEvent: event];
      }
    }while(event != nil);
  }



  printf("finished running\n");

}
