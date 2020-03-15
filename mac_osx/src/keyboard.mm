
#import <AppKit/AppKit.h>
#import <IOKit/hid/IOHIDLib.h>
#import <AudioToolbox/AudioToolbox.h>
#import "keyboard.h"

static IOHIDManagerRef HIDManager = NULL;

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

void controllerInput(void *context, 
		     IOReturn result, 
		     void *sender, 
		     IOHIDValueRef value) {
  
  if(result != kIOReturnSuccess) {
    return;
  }

  OSXController *controller = (__bridge OSXController *)context;
    
  IOHIDElementRef element = IOHIDValueGetElement(value);    
  uint32 usagePage = IOHIDElementGetUsagePage(element);
  uint32 usage = IOHIDElementGetUsage(element);

  //Buttons
  if(usagePage == kHIDPage_Button)
  {
    BOOL buttonState = (BOOL)IOHIDValueGetIntegerValue(value);
    if(usage == controller->_buttonAUsageID)
    { controller->_buttonAState = buttonState; }
    if(usage == controller->_buttonBUsageID)
    { controller->_buttonBState = buttonState; }
    if(usage == controller->_buttonXUsageID)
    { controller->_buttonXState = buttonState; }
    if(usage == controller->_buttonYUsageID)
    { controller->_buttonYState = buttonState; }
    if(usage == controller->_lShoulderUsageID)
    { controller->_buttonLeftShoulderState = buttonState; }
    if(usage == controller->_rShoulderUsageID)
    { controller->_buttonRightShoulderState = buttonState; }
  }
  //dPad
  else if(usagePage == kHIDPage_GenericDesktop && usage == kHIDUsage_GD_Hatswitch) {
    int dpadState = (int)IOHIDValueGetIntegerValue(value);
    NSInteger dpadX = 0;
    NSInteger dpadY = 0;

    switch(dpadState)
    {
    case 0: dpadX = 0; dpadY = 1; break;
    case 1: dpadX = 1; dpadY = 1; break;
    case 2: dpadX = 1; dpadY = 0; break;
    case 3: dpadX = 1; dpadY = -1; break;
    case 4: dpadX = 0; dpadY = -1; break;
    case 5: dpadX = -1; dpadY = -1; break;
    case 6: dpadX = -1; dpadY = 0; break;
    case 7: dpadX = -1; dpadY = 1; break;
    default: dpadX = 0; dpadY = 0; break;
    }

    controller->_dpadX = dpadX;
    controller->_dpadY = dpadY; 
  }
}



void controllerConnected(void *context, IOReturn result,
			 void *sender, IOHIDDeviceRef device)
{
  if(result != kIOReturnSuccess)
  {
    return;
  }

  NSUInteger vendorID = [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, 
								     CFSTR(kIOHIDVendorIDKey)) unsignedIntegerValue];
  NSUInteger productID = [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, 
								      CFSTR(kIOHIDProductIDKey)) unsignedIntegerValue];

  OSXController *controller = [[OSXController alloc] init];
    
  if(vendorID == 0x054C && productID == 0x5C4)
  {
    NSLog(@"Sony Dualshock 4 detected.");
    controller->_buttonAUsageID = 0x02;
    controller->_buttonBUsageID = 0x03;
    controller->_buttonXUsageID = 0x01;
    controller->_buttonYUsageID = 0x04;
    controller->_lShoulderUsageID = 0x05;
    controller->_rShoulderUsageID = 0x06;
  }

  IOHIDDeviceRegisterInputValueCallback(device, controllerInput, (__bridge void *)controller);  
  IOHIDDeviceSetInputValueMatchingMultiple(device, (__bridge CFArrayRef)@[
									  @{@(kIOHIDElementUsagePageKey): @(kHIDPage_GenericDesktop)},
									  @{@(kIOHIDElementUsagePageKey): @(kHIDPage_Button)},
									  ]);

  // connectedController = controller;
}



@end

const static unsigned short leftArrowKeyCode = 0x7B;

void updateKeyboardControllerWith(NSEvent* event, OSXController* keyboardController)
{
  switch([event type])
  {
  case NSEventTypeKeyDown:
    if(event.keyCode == leftArrowKeyCode &&
       keyboardController.dpadX != 1)
    {
      keyboardController.dpadX = -1;
      NSLog(@"Hello controller left");
      break;
    }
  }
}

void macOSInitGameControllers(OSXController* connected, OSXController* keyboard)
{
  HIDManager = IOHIDManagerCreate(kCFAllocatorDefault, 0);
  connected = [[OSXController alloc] init];
  keyboard = [[OSXController alloc] init];
  if (IOHIDManagerOpen(HIDManager, kIOHIDOptionsTypeNone) != kIOReturnSuccess)
  {
    NSLog(@"Error Initializing OSX Handmade Controllers");
    return;
  }

  IOHIDManagerRegisterDeviceMatchingCallback(HIDManager,
					     controllerConnected, NULL);
  IOHIDManagerSetDeviceMatchingMultiple(HIDManager, (__bridge CFArrayRef)@[
	      @{@(kIOHIDDeviceUsagePageKey):
		@(kHIDPage_GenericDesktop),
		  @(kIOHIDDeviceUsageKey): @(kHIDUsage_GD_GamePad)},
	      @{@(kIOHIDDeviceUsagePageKey): @(kHIDPage_GenericDesktop),
		  @(kIOHIDDeviceUsageKey)
		  : @(kHIDUsage_GD_MultiAxisController)},
									  ]);
  IOHIDManagerScheduleWithRunLoop(HIDManager, 
				  CFRunLoopGetMain(), 
				  kCFRunLoopDefaultMode);
  NSLog(@"OSXhandmade Controller initialized.");
}
