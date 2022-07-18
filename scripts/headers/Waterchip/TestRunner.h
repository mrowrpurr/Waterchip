/*
    Test Runner for Waterchip test/spec framework ~ by Mrowr Purr ~
*/

#define DEBUG_NAME "Waterchip"

#include "Waterchip/DebugHelper.h"
#include "Waterchip/TestData.h"
#include "Waterchip/TestReporter.h"

inline procedure TestRunner_Run begin
    debug("loading...");
    call TestData_Initialize;
    debug("loaded.");
end
