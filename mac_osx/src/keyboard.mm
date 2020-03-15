
#import <AppKit/AppKit.h>
#import <IOKit/hid/IOHIDLib.h>
#import <AudioToolbox/AudioToolbox.h>
#import "keyboard.h"

static IOHIDManagerRef HIDManager = NULL;

OSXController* connectedController = nil;
OSXController* keyboardController = nil;


@implementation HandmadeKeyIgnoringWindow
-(void)keyDown:(NSEvent*)theEvent {}
@end

@implementation OSXController {
  CFIndex _dpadLUsageID;
  CFIndex _dpadRUsageID;
  CFIndex _dpadDUsageID;
  CFIndex _dpadUUsageID;
  
  CFIndex _buttonAUsageID;
  CFIndex _buttonBUsageID;
  CFIndex _buttonXUsageID;
  CFIndex _buttonYUsageID;

  CFIndex _lShoulderUsageID;
  CFIndex _rShoulderUsageID;
}

void macOSInitGameController()
{
  HIDManager = IOHIDManagerCreate(kCFAllocatorDefault, 0);
  connectedController = [[OSXController alloc] init];
  keyboardController = [[OSXController alloc] init];
}

@end
