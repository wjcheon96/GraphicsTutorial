// GetScaleFactor.mm

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

extern "C" {
    float getScaleFactor() {
        @autoreleasepool {
            // 현재 포커스된 화면 가져오기
            // NSScreen을 통해 macOS에서 디스플레이의 화면 정보를 가져올 수 있는데, 이 중mainScreen의 정보를 가져온다.
            NSScreen *currentScreen = [NSScreen mainScreen];
            // 마우스 커서의 현재 위치를 가져옴.
            NSPoint mouseLocation = [NSEvent mouseLocation];
            // 현재 연결된 전체 스크린의 정보를 모두 가져온다.(NSScreen screens)
            for (NSScreen *screen in [NSScreen screens]) {
                NSRect frame = [screen frame];
                // 이 중에서 현재 마우스 커서가 위치한 프레임을 찾아, 마우스가 있으면 해당 화면의 정보를 가져온다.
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