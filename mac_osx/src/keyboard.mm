
#import <AppKit/AppKit.h>
#import <IOKit/hid/IOHIDLib.h>
#import "keyboard.h"

@interface HandmadeKeyIgnoringWindow : NSWindow
@end

@implementation HandmadeKeyIgnoringWindow
-(void)keyDown:(NSEvent*)theEvent {}
@end

