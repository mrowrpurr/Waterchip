#pragma once

// TODO remote all sprintf for any expected/actual which could be a float (basically anything)
//      because %s displays floats incorrectly!

// TODO move alternative syntax aliases to Waterchip/Something.h
//      to not pollute the namespace with lots of named defines!

/*
    Library for Waterchip test/spec framework ~ by Mrowr Purr ~

    # BDD Expectations

    This library provides a number of expect(x) to_y() BDD-style expections.

    This is easy to extend:

    #define to_be_awesome(value) <<---- TODO add example synta

    # xUnit Assertions

    xUnit-style `assert_equal(a, b)` assertions are not included in the library.

    Here is why:
    https://stackoverflow.com/questions/2404978/why-are-assertequals-parameters-in-the-order-expected-actual

    tl;dr - is it assert_eq(expected, actual) or assert_eq(actual, expected) ???

    No one can decide. So it's best to avoid this syntax and provide a more
    expressive DSL using expect() expectations.

    Feel free to write your own assert_x() xUnit-style macros!

    # FAQ

    Question: "WTF THIS IS TOTAL MADNESS!? WTH??"

    Answer: Yep, Agreed ;)

*/

#include "sfall/sfall.h"
#include "sfall/lib.arrays.h"
#include "sfall/lib.strings.h"

procedure start;

// Path to Waterchip configuration file 
#define WATERCHIP_INI                 "mods/Waterchip.ini"
#define WATERCHIP_INI_ENABLED_SETTING (WATERCHIP_INI + "|Waterchip|enabled")

// Stores the configuration read from the .ini file, when present
variable __waterchip_config;

// Stores data about the test suite for the runner, e.g. all test names
variable __waterchip_testsuite_data;

// Tracks if script has been configured to run itself (in 1 frame) after setup
variable __waterchip_testsuite_repeat_initialized = false;

// Name of the currentlt running test
variable __waterchip_testsuite_currently_running_test_name;

// Are we running the teardown? Teardown is executed via separate function FYI.
// It can NOT access variables defined inside of the describe() block.
// This is a "surprising" feature, so we'll try to change it later!
variable __waterchip_testsuite_currently_running_teardown;

// Was a teardown defined?
variable __waterchip_testsuite_teardown_defined;

// Whether or not the test currently being defined is a 'pending'/'skip' test
variable __waterchip_testsuite_currently_defining_skipped_test;

// Is this the first test() which is being run in the suite?
// If so, the setup_once is evaluated, if defined.
variable __waterchip_testsuite_first_test_run = true;

// Whether or not the script is currently running tests
// Set to true when it detects it's running via repeat (1 frame after setup)
// After which the repeat is disabled for the script
variable __waterchip_testsuite_currently_running_tests;

// A little bit of unnecessary backup protection in cast the script can be
// invoked via repeat when it's already running. I don't think this is needed.
variable __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run = 0;

// Current value selected via expect() for running assertions/expections on.
variable __waterchip_testsuite_current_expect_value;

// Configurable prefix added to each debug_msg output for easy filtering
variable __waterchip_output_prefix = "[Waterchip] ";

// Variables for looping over each line in a failure message
// So we can __waterchip_debug_test each line (for debug log filtering)
variable __waterchip_testsuite_currently_running_test_failure_message_line;
variable __waterchip_testsuite_currently_running_test_failure_message_lines;

// For with_msg to provide your own failure expectation message
variable __waterchip_testsuite_current_expect_failure_message;

// Counts for outputting full test results.
// Either we increment counters or we loop over the test_results
// which would also require the use of variables.
// Might as well have these variables.
variable __waterchip_testsuite_total_passed;
variable __waterchip_testsuite_total_failed;
variable __waterchip_testsuite_total_skipped;

// Set the prefix for Watership debug output messages
#define set_waterchip_debug_prefix(prefix) __waterchip_output_prefix = prefix

// Cleqr the prefix for Watership debug output messages
#define clear_waterchip_debug_prefix __waterchip_output_prefix = ""

// Test state names stored in __waterchip_data.testsuites[suite].test_results
#define WATERCHIP_TEST_RESULT_FAILED  "FAIL"
#define WATERCHIP_TEST_RESULT_PASSED  "PASS"
#define WATERCHIP_TEST_RESULT_SKIPPED "SKIP"
#define WATERCHIP_TEST_RESULT_NOT_RUN "NOT_RUN"

// Helpers for waterchip output
//
// TODO - do NOT use sprintf_array (so that people can use sprintf_array)
//        you cannot sprintf_array inside of a sprintf_array
//
#define __waterchip_debug_testsuite(text) debug_msg(sprintf_array("%s[%s] %s\n",        [__waterchip_output_prefix, __waterchip__testsuite_name, text]))
#define __waterchip_debug_test(text)      debug_msg(sprintf_array("%s[%s] [%s] [%s]\n", [__waterchip_output_prefix, __waterchip__testsuite_name, __waterchip_testsuite_currently_running_test_name, text]))

// Print a message to the debug output (in the same format as Waterchip)
#define print(text) \
    if __waterchip_testsuite_data then \
        if __waterchip_testsuite_currently_running_test_name then \
            __waterchip_debug_test(text); \
        else \
            __waterchip_debug_testsuite(text); \
    else \
        debug_msg(__waterchip_output_prefix + " [] [] " + sprintf("[%s]", text))

// Helper for print(sprintf())
#define printf(text, argument) print(sprintf(text, argument))

/***********************************
    DSL for defining test suites
************************************/

// variable __waterchip__testsuite_name (created by the describe macro)
// Stores the name provided via describe("") or context("") or testsuite("")
//
// Setup a test suite (creates the start procedure under the covers)
#define describe(testsuite_name) \
    variable __waterchip__testsuite_name = testsuite_name; \
    procedure start

// Runs a block of code *once* (when the first test is executed)
#define setup_once if __waterchip_testsuite_currently_running_test_name and __waterchip_testsuite_first_test_run then

// Simply runs the provided block if any test is running. Merely semantics.
// MUST be placed BEFORE all tests
#define setup if __waterchip_testsuite_currently_running_test_name and not __waterchip_testsuite_currently_running_teardown then

// Simply runs the provided block if any test is running. Merely semantics.
// MUST be placed AFTER all
#define teardown __waterchip_testsuite_teardown_defined = true; \
    if __waterchip_testsuite_currently_running_teardown then

// Register a test
#define test(test_name) \
    __waterchip_test_macro_start(test_name) else if __waterchip_testsuite_currently_running_tests and not __waterchip_testsuite_currently_running_teardown and __waterchip_testsuite_currently_running_test_name == test_name and __waterchip_testsuite_data.test_results[test_name].status != WATERCHIP_TEST_RESULT_SKIPPED then

// BDD-ish alias for the xUnit test()
#define it(example_name) test(example_name)

#define skip(test_name) \
    __waterchip_testsuite_currently_defining_skipped_test = true; \
    test(test_name)

// xtest common alias for skipping a test() easily
#define xtest(test_name) skip(test_name)

// xit common alias for skipping a it() example easily
#define xit(test_name) skip(test_name)

// Like xit, xtest, skip: but no body - just copy/pasted test and removed 'then'
#define todo(test_name) \
    __waterchip_testsuite_currently_defining_skipped_test = true; \
    __waterchip_test_macro_start(test_name) \
    false

// In Waterchip v1, a context is just a prefix to add to ... no, it doesn't exist at all ;)

/**************************
        EXPECTATIONS
***************************/

// Used by expectations or call yourself
#define fail(message) \
    begin \
        __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status = WATERCHIP_TEST_RESULT_FAILED; \
        if __waterchip_testsuite_current_expect_failure_message then begin \
            __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].failure_message = __waterchip_testsuite_current_expect_failure_message + ": " + message; \
        end else \
            __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].failure_message = message; \
        return; \
    end \
    false

// Provide your own message to prepend to the expectation failure message
#define with_msg(text) __waterchip_testsuite_current_expect_failure_message = text;

// TODO
// #define assert(boolean, message)

// Mark a value for inspection via expectatins
#define expect(value) \
    begin \
        __waterchip_testsuite_current_expect_value = value; \
        __waterchip_testsuite_current_expect_failure_message = 0; \
    end

// Expectation for equality
#define to_equal(expected) \
    begin \
        if __waterchip_testsuite_current_expect_value != expected then \
            fail("Expected to equal.\nExpected: '" + expected + "'\nActual: '" + __waterchip_testsuite_current_expect_value + "'"); \
    end \
    false

// Expectation for inequality
#define not_to_equal(expected) \
    if __waterchip_testsuite_current_expect_value == expected then \
        fail("Expected not to equal.\nExpected: '" + expected + "'\nActual: '" + __waterchip_testsuite_current_expect_value + "'")

// Does a string start with something?
#define to_start_with(expected) \
    switch typeof(__waterchip_testsuite_current_expect_value) begin \
        case 1: fail("to_start_with requires string, called with integer: " + __waterchip_testsuite_current_expect_value); \
        case 2: fail("to_start_with requires string, called with float: " + __waterchip_testsuite_current_expect_value); \
        case 3: if not string_starts_with(__waterchip_testsuite_current_expect_value, expected) then \
            fail("Expected string to start with.\nExpectd: '" + expected + "'\nActual: '" + __waterchip_testsuite_current_expect_value + "'"); \
    end \
    false
       
// Helper for asserting 0 value
#define to_be_zero     to_equal(0)
#define not_to_be_zero not_to_equal(0)

// Helper or asserting true (1) value
#define to_be_true     to_equal(true)
#define not_to_be_true not_to_equal(true)

// Helper or asserting false (0) value
#define to_be_false     to_equal(false)
#define not_to_be_false not_to_equal(false)

// TODO
#define to_be_truthy fail("NOT YET IMPLEMENTED: to_be_truthy");

// TODO
#define to_be_falsy fail("NOT YET IMPLEMENTED: to_be_falsy");

// Expectation for whether a string is empty or array has zero elements
// typeof: 1 (int) 2 (float) 3 (string)
#define to_be_empty \
    switch typeof(__waterchip_testsuite_current_expect_value) begin \
        case 1: begin \
            if __waterchip_testsuite_current_expect_value then begin \
                if len_array(__waterchip_testsuite_current_expect_value) == -1 then \
                    fail("to_be_empty called on int which is not an array"); \
                if len_array(__waterchip_testsuite_current_expect_value) > 0 then \
                    fail(sprintf("Expected array to be empty, but contained %s elements/keys", len_array(__waterchip_testsuite_current_expect_value))); \
            end \
        end \
        case 2: fail("to_be_empty called with a float (invalid argument)"); \
        case 3: if strlen(__waterchip_testsuite_current_expect_value) > 0 then \
            fail(sprintf("Expected string to be empty, but was \"%s\"", __waterchip_testsuite_current_expect_value)); \
    end \
    false

// Expectation for whether a string is empty or array has zero elements
// typeof: 1 (int) 2 (float) 3 (string)
#define not_to_be_empty \
    switch typeof(__waterchip_testsuite_current_expect_value) begin \
        case 1: begin \
            if __waterchip_testsuite_current_expect_value then begin \
                if len_array(__waterchip_testsuite_current_expect_value) == -1 then \
                    fail("not_to_be_empty called on int which is not an array"); \
                if len_array(__waterchip_testsuite_current_expect_value) == 0 then \
                    fail("Expected array not to be empty, but had 0 elements"); \
            end \
        end \
        case 2: fail("not_to_be_empty called with a float (invalid argument)"); \
        case 3: if strlen(__waterchip_testsuite_current_expect_value) == 0 then \
            fail("Expected string not to be empty, but had 0 length"); \
    end \
    false

// Expectation for whether a string contains a substring or array contains item 
// typeof: 1 (int) 2 (float) 3 (string)
#define to_contain fail("NOT YET IMPLEMENTED: to_contain");

// Expectation for if a map has a key
#define to_contain_key(expected) \
    switch typeof(__waterchip_testsuite_current_expect_value) begin \
        case 1: begin \
            if __waterchip_testsuite_current_expect_value then begin \
                if len_array(__waterchip_testsuite_current_expect_value) == -1 then \
                    fail("to_contain_key called on int which is not an array"); \
                if not map_contains_key(__waterchip_testsuite_current_expect_value, expected) then \
                    fail("Expected map to contain key.\nExpected key: '" + expected + "'\nMap: " + debug_array_str(__waterchip_testsuite_current_expect_value)); \
            end \
        end \
        case 2: fail("to_contain_key called with a float (invalid argument)"); \
        case 3: fail("to_contain_key called with a string (invalid argument)"); \
    end \
    false

// Expectation of a certain length of array/map or string
#define to_have_length(expected) \
    switch typeof(__waterchip_testsuite_current_expect_value) begin \
        case 1: begin \
            if __waterchip_testsuite_current_expect_value then begin \
                if len_array(__waterchip_testsuite_current_expect_value) == -1 then \
                    fail("to_have_length called on int which is not an array"); \
                if len_array(__waterchip_testsuite_current_expect_value) != expected then \
                    fail("Expected map to have length.\nExpected length: " + expected + "\nActual length: " + \
                    len_array(__waterchip_testsuite_current_expect_value)); \
            end \
        end \
        case 2: fail("to_contain_key called with a float (invalid argument)"); \
        case 3: fail("to_contain_key called with a string (invalid argument)"); \
    end \
    false

/****************************************
   Primary macro for defining tests
*****************************************/

// Shared code between `test` and `todo`
#define __waterchip_test_macro_start(test_name) \
    if not __waterchip_testsuite_data then begin \
        call __waterchip_load_config; \
        __waterchip_testsuite_data = {}; \
        fix_array(__waterchip_testsuite_data); \
        __waterchip_testsuite_data.test_names = []; \
        fix_array(__waterchip_testsuite_data.test_names); \
        __waterchip_testsuite_data.test_results = {}; \
        fix_array(__waterchip_testsuite_data.test_results); \
    end \
    if not __waterchip_config.enabled then return; \
    if not __waterchip_should_run_testsuite(__waterchip__testsuite_name) then return; \
    if (not __waterchip_testsuite_currently_running_tests) and scan_array(__waterchip_testsuite_data.test_names, test_name) > -1 and __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run == 0 then begin \
        set_global_script_repeat(0); \
        __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run++; \
        __waterchip_testsuite_currently_running_tests = true; \
        foreach __waterchip_testsuite_currently_running_test_name in (__waterchip_testsuite_data.test_names) begin \
            if not __waterchip_should_run_test(__waterchip_testsuite_currently_running_test_name) then \
                __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status = WATERCHIP_TEST_RESULT_SKIPPED; \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status != WATERCHIP_TEST_RESULT_SKIPPED then begin \
                call start; \
                __waterchip_testsuite_first_test_run = false; \
                if __waterchip_testsuite_teardown_defined then begin \
                    __waterchip_testsuite_currently_running_teardown = true; \
                    call start; \
                    __waterchip_testsuite_currently_running_teardown = false; \
                end \
            end \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_NOT_RUN then begin \
                __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status = WATERCHIP_TEST_RESULT_PASSED; \
                __waterchip_testsuite_total_passed++; \
            end else if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_SKIPPED then \
                __waterchip_testsuite_total_skipped++; \
            switch (__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status) begin \
                case WATERCHIP_TEST_RESULT_PASSED: if __waterchip_config.output.show_passed != "0" then \
                    __waterchip_debug_test(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status); \
                case WATERCHIP_TEST_RESULT_SKIPPED: if __waterchip_config.output.show_skipped != "0" then \
                    __waterchip_debug_test(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status); \
                case WATERCHIP_TEST_RESULT_FAILED: if __waterchip_config.output.show_failed != "0" then \
                    __waterchip_debug_test(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status); \
            end \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_FAILED then begin \
                __waterchip_testsuite_total_failed++; \
                __waterchip_testsuite_currently_running_test_failure_message_lines = string_split(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].failure_message, "\n"); \
                foreach __waterchip_testsuite_currently_running_test_failure_message_line in __waterchip_testsuite_currently_running_test_failure_message_lines \
                    __waterchip_debug_test(__waterchip_testsuite_currently_running_test_failure_message_line); \
            end \
        end \
        if __waterchip_testsuite_total_failed > 0 then \
            __waterchip_debug_testsuite("[Tests failed] [" + __waterchip_testsuite_total_failed + " failed, " + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
        else \
            __waterchip_debug_testsuite("[Tests passed] [" + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
        return; \
    end \
    if not __waterchip_testsuite_repeat_initialized then begin \
        __waterchip_testsuite_repeat_initialized = true; \
        set_global_script_repeat(1); \
    end \
    if scan_array(__waterchip_testsuite_data.test_names, test_name) == -1 then begin \
        call array_push(__waterchip_testsuite_data.test_names, test_name); \
        __waterchip_testsuite_data.test_results[test_name] = { "status": WATERCHIP_TEST_RESULT_NOT_RUN }; \
        fix_array(__waterchip_testsuite_data.test_results[test_name]); \
        if __waterchip_testsuite_currently_defining_skipped_test then begin \
            __waterchip_testsuite_currently_defining_skipped_test = false; \
            __waterchip_testsuite_data.test_results[test_name].status = WATERCHIP_TEST_RESULT_SKIPPED; \
        end \
    end

/****************************************
     Configuration from Waterchip.ini
*****************************************/

procedure __waterchip_load_config begin
    if __waterchip_config then return;

    __waterchip_config = {};
    fix_array(__waterchip_config);
    
    __waterchip_config.enabled = get_ini_setting(WATERCHIP_INI_ENABLED_SETTING);

    // -1 means the .ini file (or section/key) was not found
    if __waterchip_config.enabled == -1 then
        __waterchip_config.enabled = false; // we don't run tests if no .ini

    // if either the .ini wasn't found or not enabled, return without reading config.
    if not __waterchip_config.enabled then return;

    // Read test suite and test filters...

    // [TestSuites:Include]
    __waterchip_config.include_testsuite_names = get_ini_section(WATERCHIP_INI, "TestSuites:Include");
    if not __waterchip_config.include_testsuite_names then __waterchip_config.include_testsuite_names = {};
    fix_array(__waterchip_config.include_testsuite_names);

    // [TestSuites:Exclude]
    __waterchip_config.exclude_testsuite_names = get_ini_section(WATERCHIP_INI, "TestSuites:Exclude");
    if not __waterchip_config.exclude_testsuite_names then __waterchip_config.exclude_testsuite_names = {};
    fix_array(__waterchip_config.exclude_testsuite_names);

    // [Tests:Include]
    __waterchip_config.include_test_names = get_ini_section(WATERCHIP_INI, "Tests:Include");
    if not __waterchip_config.include_test_names then __waterchip_config.include_test_names = {};
    fix_array(__waterchip_config.include_test_names);

    // [Tests:Exclude]
    __waterchip_config.exclude_test_names = get_ini_section(WATERCHIP_INI, "Tests:Exclude");
    if not __waterchip_config.exclude_test_names then __waterchip_config.exclude_test_names = {};
    fix_array(__waterchip_config.exclude_test_names);

    // Read [Output] configuration
    __waterchip_config.output = get_ini_section(WATERCHIP_INI, "Output");
    if not __waterchip_config.output then __waterchip_config.output = {};
    fix_array(__waterchip_config.output);
end

procedure __waterchip_should_run_testsuite(variable testsuite_name) begin
    variable filter, enabled, included, excluded;
    variable includes_defined = (len_array(__waterchip_config.include_testsuite_names) > 0);
    variable excludes_defined = (len_array(__waterchip_config.exclude_testsuite_names) > 0);

    testsuite_name = string_tolower(testsuite_name);

    if includes_defined and excludes_defined then begin
        foreach filter: enabled in (__waterchip_config.include_testsuite_names) begin
            if is_in_string(testsuite_name, string_tolower(filter)) then begin
                included = atoi(enabled);
                break;
            end
        end
        foreach filter: enabled in (__waterchip_config.exclude_testsuite_names) begin
            if is_in_string(testsuite_name, string_tolower(filter)) then begin
                excluded = atoi(enabled);
                break;
            end
        end
        return included and not excluded;
    end else if includes_defined then begin
        foreach filter: enabled in (__waterchip_config.include_testsuite_names) begin
            if is_in_string(testsuite_name, string_tolower(filter)) then begin
                return atoi(enabled);
            end
        end
        return false;
    end else if excludes_defined then begin
        foreach filter: enabled in (__waterchip_config.exclude_testsuite_names) begin
            if is_in_string(testsuite_name, string_tolower(filter)) then begin
                return not atoi(enabled);
            end
        end
        return true;
    end
    return true;
end

procedure __waterchip_should_run_test(variable test_name) begin
    variable filter, enabled, included, excluded;
    variable includes_defined = (len_array(__waterchip_config.include_test_names) > 0);
    variable excludes_defined = (len_array(__waterchip_config.exclude_test_names) > 0);

    test_name = string_tolower(test_name);

    if includes_defined and excludes_defined then begin
        foreach filter: enabled in (__waterchip_config.include_test_names) begin
            if is_in_string(test_name, string_tolower(filter)) then begin
                included = atoi(enabled);
                break;
            end
        end
        foreach filter: enabled in (__waterchip_config.exclude_test_names) begin
            if is_in_string(test_name, string_tolower(filter)) then begin
                excluded = atoi(enabled);
                break;
            end
        end
        return included and not excluded;
    end else if includes_defined then begin
        foreach filter: enabled in (__waterchip_config.include_test_names) begin
            if is_in_string(test_name, string_tolower(filter)) then begin
                return atoi(enabled);
            end
        end
        return false;
    end else if excludes_defined then begin
        foreach filter: enabled in (__waterchip_config.exclude_test_names) begin
            if is_in_string(test_name, string_tolower(filter)) then begin
                return not atoi(enabled);
            end
        end
        return true;
    end
    return true;
end
