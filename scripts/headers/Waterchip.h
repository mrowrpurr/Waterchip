/*
    Library for Waterchip test/spec framework ~ by Mrowr Purr ~
*/

#include "sfall/sfall.h"
#include "sfall/lib.arrays.h"

// Imported variable for Waterchip test runner.
import variable __waterchip_data;

procedure start;

// Local variable for accessing ___waterchip_data using Map accessors.
// Workaround for SSLC bug: Map accessors don't work on import/export variables.
variable waterchip_data;

// Reference to the data map for this test suite (in waterchip_data.test_suites)
variable __waterchip_testsuite_data;

// Tracks if script has been configured to run itself (in 1 frame) after setup
variable __waterchip_testsuite_repeat_initialized = false;

// Name of the currentlt running test
variable __waterchip_testsuite_currently_running_test_name;

// Whether or not the script is currently running tests
// Set to true when it detects it's running via repeat (1 frame after setup)
// After which the repeat is disabled for the script
variable __waterchip_testsuite_currently_running_tests;

// A little bit of unnecessary backup protection in cast the script can be
// invoked via repeat when it's already running. I don't think this is needed.
variable __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run = 0;

// variable __waterchip__testsuite_name (created by the describe macro)
// Stores the name provided via describe("") or context("") or test_suite("")
//
// Setup a test suite (creates the start procedure under the covers)
#define describe(test_suite_name) \
    variable __waterchip__testsuite_name = test_suite_name; \
    procedure start

// xUnit-ish alias for the BDD describe
#define context(test_suite_name) describe(test_suite_name)

// xUnit-ish alias for the BDD describe
#define test_suite(test_suite_name) describe(test_suite_name)

// Simply runs the provided block if any test is running. Merely semantics.
// MUST be placed BEFORE all tests
#define setup if __waterchip_testsuite_currently_running_test_name then

// Simply runs the provided block if any test is running. Merely semantics.
// MUST be placed AFTER all
#define teardown if __waterchip_testsuite_currently_running_test_name then

// Register a test
#define test(test_name) \
    if not waterchip_data then waterchip_data = __waterchip_data; \
    if not __waterchip_testsuite_data then begin \
        __waterchip_testsuite_data = {}; \
        fix_array(__waterchip_testsuite_data); \
        waterchip_data[__waterchip__testsuite_name] = __waterchip_testsuite_data; \
        __waterchip_testsuite_data.test_names = []; \
        fix_array(__waterchip_testsuite_data.test_names); \
        __waterchip_testsuite_data.test_results = {}; \
        fix_array(__waterchip_testsuite_data.test_results); \
    end \
    if (not __waterchip_testsuite_currently_running_tests) and scan_array(__waterchip_testsuite_data.test_names, test_name) > -1 and __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run == 0 then begin \
        set_global_script_repeat(0); \
        __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run++; \
        __waterchip_testsuite_currently_running_tests = true; \
        foreach __waterchip_testsuite_currently_running_test_name in (__waterchip_testsuite_data.test_names) begin \
            call start; \
        end \
        return; \
    end \
    if not __waterchip_testsuite_repeat_initialized then begin \
        __waterchip_testsuite_repeat_initialized = true; \
        set_global_script_repeat(1); \
    end \
    if scan_array(__waterchip_testsuite_data.test_names, test_name) == -1 then begin \
        call array_push(__waterchip_testsuite_data.test_names, test_name); \
    end else if __waterchip_testsuite_currently_running_tests and __waterchip_testsuite_currently_running_test_name == test_name then
