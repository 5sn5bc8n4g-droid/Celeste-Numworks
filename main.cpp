#include "eadk/eadk_vars.h"
#include "eadk/eadkpp.h"
// Some functions from eadk.h aren't in eadkpp.h
#include "eadk/eadk.h"

// The actual game port THX Lemon :)
#include "translation.hpp" 

#include "limiter.hpp"
#include "saveHandler.hpp"

// Saying the namespaces we'll use on a
// semi regularly so it's less of a
// hassle to write common functions etc
using namespace EADK;

bool running = true;

// Some vars for the frameLimiter
uint64_t frameEndTime    = 0;
uint64_t frameStartTime  = 0;

int main(void) {
    // Clearing the screen first
    Display::pushRectUniform(Screen::Rect, 0xFFFFFF);
    Display::drawString("[ 1  0  5 ]", Point(30, 50), false, 0x000000, 0xFFFFFF);
    Display::drawString("[ 0  1  2 ]", Point(30, 75), false, 0x000000, 0xFFFFFF);
    Display::drawString("[ 4  3  9 ]", Point(30, 100), false, 0x000000, 0xFFFFFF);

    int passcode_step = 0;
    while (passcode_step < 4) {
        if (passcode_step == 0 && Keyboard::scan().keyDown(Keyboard::Key::Six)) { passcode_step = 1; Timing::msleep(300); }
        else if (passcode_step == 1 && Keyboard::scan().keyDown(Keyboard::Key::Seven)) { passcode_step = 2; Timing::msleep(300); }
        else if (passcode_step == 2 && Keyboard::scan().keyDown(Keyboard::Key::Six)) { passcode_step = 3; Timing::msleep(300); }
        else if (passcode_step == 3 && Keyboard::scan().keyDown(Keyboard::Key::Seven)) { passcode_step = 4; Timing::msleep(300); }
    }
    // Safety check: Wait for the user to completely release the keypad buttons
    // This stops double-trigger signals from corrupting the engine variables at boot
    while (Keyboard::scan().keyDown(Keyboard::Key::Seven) || Keyboard::scan().keyDown(Keyboard::Key::Six)) {
        Timing::msleep(50);
    }
    Timing::msleep(200); // Allow system cache frames to settle cleanly

    
    while (running) { // You can always exit by double pressing home so \(°-°)/
        if (Keyboard::scan().keyDown(Keyboard::Key::Home)) { running = false; }

        frameStartTime = eadk_timing_millis();

        gameMain();

        frameEndTime = eadk_timing_millis();

        // Time to limit the frame rate Whoooooo
        frameLimiter(frameEndTime - frameStartTime);
    }
    emuShutDown();

    return 0;
}
