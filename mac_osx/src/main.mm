#include <stdio.h>
#include <AppKit/AppKit.h>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "keyboard.h" // mac os x specific

#include "video.h"
#include "game_module.h"
#include "game_controller.h"

static bool Running = true;

static uint32_t globalRenderWidth = 1078U;
static uint32_t globalRenderHeight = 768U;

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
  NSLog(@"Starting up");
  MainWindowDelegate* MainDele = [[MainWindowDelegate alloc] init];

  NSRect screenRect = [[NSScreen mainScreen] frame];

  NSRect initialFrame = NSMakeRect((screenRect.size.width - globalRenderWidth) * 0.5,
				   (screenRect.size.height - globalRenderHeight) * 0.5,
				   globalRenderWidth,
				   globalRenderHeight);

  NSWindow* window = [[HandmadeKeyIgnoringWindow alloc]
		       initWithContentRect : initialFrame
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

  OSXController* ck = nil;
  OSXController* kk = nil;

  macOSInitGameControllers(ck, kk);

  ModuleFunctions blueFuncs; 
  // todo(brandon): determine way of getting current path / loading dylibs inside an mac os x bundle. 
  LoadModule(blueFuncs, std::string(DYLIB_DIR) + "blue_d.dylib");

  ModuleFunctions towerFuncs;
  LoadModule(towerFuncs, std::string(DYLIB_DIR) + "tower_d.dylib");

  ScreenData currentScreen;
  currentScreen.buffer = 0;
  MainDele.screenRef = &currentScreen;
  currentScreen.bytesPerPixel = 4;
  RefreshBuf(window, currentScreen);


  GameState mahState;
  GameInputController mahKeyboard;
  GameInputControllerInit(&mahKeyboard);
  mahState.platformData = new uint8_t[100];

  if(towerFuncs.GameInit != NULL)
  {
    int res = towerFuncs.GameInit(&mahState);
    if(res != 0)
    {
      NSLog(@"Failed to init tower");
      return 1;
    }
  }
  int offsetX = 10;

  while(Running)
  {
    // updates the temporary buffer with data.
    if(towerFuncs.GameUpdate != NULL)
    {
      towerFuncs.GameUpdate(0, &currentScreen, &mahState, &mahKeyboard);
    }

    if(mahState.module_data != NULL)
    {
      uint16_t* tmpP = reinterpret_cast<uint16_t*>(mahState.module_data);
      NSLog(@"Current offset: %d", tmpP[0]);
    }

    // takes the buffer data and puts it onto the screen.
    ReDrawBuf(window, currentScreen.buffer, currentScreen.width, currentScreen.height, currentScreen.pitch);

    NSEvent* event;
    do {
      event = [NSApp nextEventMatchingMask: NSEventMaskAny
				 untilDate: nil
				    inMode: NSDefaultRunLoopMode
				   dequeue: YES];

      if(event != nil &&
	 (event.type == NSEventTypeKeyDown ||
	  event.type == NSEventTypeKeyUp))
      {
	updateKeyboardControllerWith(event, &mahKeyboard);
      }

      switch([event type]) {
      default:
	[NSApp sendEvent: event];
      }
    }while(event != nil);
  }



  printf("finished running\n");

}
