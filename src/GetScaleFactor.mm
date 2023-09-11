// GetScaleFactor.mm

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

extern "C" {
    float getScaleFactor() {
        @autoreleasepool {
            // 현재 포커스된 화면 가져오기
            NSScreen *currentScreen = [NSScreen mainScreen];
            NSPoint mouseLocation = [NSEvent mouseLocation];
            for (NSScreen *screen in [NSScreen screens]) {
                NSRect frame = [screen frame];
                if (NSPointInRect(mouseLocation, frame)) {
                    currentScreen = screen;
                    break;
                }
            }

            // 현재 화면의 스케일 팩터 가져오기
            return [currentScreen backingScaleFactor];
        }
    }
}