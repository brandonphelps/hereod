#ifndef CUSTOM_KEYBOARD_H
#define CUSTOM_KEYBOARD_H

#import <AppKit/AppKit.h>
#import <IOKit/hid/IOHIDLib.h>

@interface HandmadeKeyIgnoringWindow : NSWindow
@end

@interface OSXController: NSObject

@property NSInteger dpadX;
@property NSInteger dpaxY;

@property BOOL buttonAState;
@property BOOL buttonBState;
@property BOOL buttonXState;
@property BOOL buttonYState;

@property BOOL buttonLeftShoulderState;
@property BOOL buttonRightShoulderState;

@end



#endif
