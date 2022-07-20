#pragma once

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

// Whether or not the test currently being defined is a 'pending'/'skip' test
variable __waterchip_testsuite_currently_defining_skipped_test;

// Whether or not the script is currently running tests
// Set to true when it detects it's running via repeat (1 frame after setup)
// After which the repeat is disabled for the script
variable __waterchip_testsuite_currently_running_tests;

// A little bit of unnecessary backup protection in cast the script can be
// invoked via repeat when it's already running. I don't think this is needed.
variable __waterchip__testsuite_times_run_tests_backup_test_var_to_verify_single_run = 0;

// Read the variable name (lol)
// Some of our macros end on `end` which you cannot add a ; semicolon after.
// So we just finish the macro with:
//    __waterchip__use_in_macro_to_support_semicolon = 0
// It's dumb but it provides a consistent syntax for developer experience
variable __waterchip__use_in_macro_to_support_semicolon;

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

// Test state names stored in waterchip_data.test_suites[suite].test_results
#define WATERCHIP_TEST_RESULT_FAILED  "FAIL"
#define WATERCHIP_TEST_RESULT_PASSED  "PASS"
#define WATERCHIP_TEST_RESULT_SKIPPED "SKIP"
#define WATERCHIP_TEST_RESULT_NOT_RUN "NOT_RUN"

// Helpers for waterchip output
//
// TODO - do NOT use sprintf_array (so that people can use sprintf_array)
//        you cannot sprintf_array inside of a sprintf_array
//
#define __waterchip_debug_test_suite(text) debug_msg(sprintf_array("%s[%s] %s\n",        [__waterchip_output_prefix, __waterchip__testsuite_name, text]))
#define __waterchip_debug_test(text)       debug_msg(sprintf_array("%s[%s] [%s] [%s]\n", [__waterchip_output_prefix, __waterchip__testsuite_name, __waterchip_testsuite_currently_running_test_name, text]))

// Print a message to the debug output (in the same format as Waterchip)
#define print(text) \
    if __waterchip_testsuite_data then \
        if __waterchip_testsuite_currently_running_test_name then \
            __waterchip_debug_test(text); \
        else \
            __waterchip_debug_test_suite(text); \
    else \
        debug_msg(__waterchip_output_prefix + " [] [] " + sprintf("[%s]", text))

// Helper for print(sprintf())
#define printf(text, argument) print(sprintf(text, argument))

// variable __waterchip__testsuite_name (created by the describe macro)
// Stores the name provided via describe("") or context("") or test_suite("")
//
// Setup a test suite (creates the start procedure under the covers)
#define describe(test_suite_name) \
    variable __waterchip__testsuite_name = test_suite_name; \
    procedure start

// xUnit-ish alias for the BDD describe()
#define context(test_suite_name) describe(test_suite_name)

// xUnit-ish alias for the BDD describe()
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
        __waterchip_debug_test_suite(""); \
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
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status != WATERCHIP_TEST_RESULT_SKIPPED then \
                call start; \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_NOT_RUN then begin \
                __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status = WATERCHIP_TEST_RESULT_PASSED; \
                __waterchip_testsuite_total_passed++; \
            end else if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_SKIPPED then \
                __waterchip_testsuite_total_skipped++; \
            __waterchip_debug_test(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status); \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_FAILED then begin \
                __waterchip_testsuite_total_failed++; \
                __waterchip_testsuite_currently_running_test_failure_message_lines = string_split(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].failure_message, "\n"); \
                foreach __waterchip_testsuite_currently_running_test_failure_message_line in __waterchip_testsuite_currently_running_test_failure_message_lines \
                    __waterchip_debug_test(__waterchip_testsuite_currently_running_test_failure_message_line); \
            end \
        end \
        if __waterchip_testsuite_total_failed > 0 then \
            __waterchip_debug_test_suite("[Tests failed] [" + __waterchip_testsuite_total_failed + " failed, " + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
        else \
            __waterchip_debug_test_suite("[Tests passed] [" + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
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
    end else if __waterchip_testsuite_currently_running_tests and __waterchip_testsuite_currently_running_test_name == test_name and __waterchip_testsuite_data.test_results[test_name].status != WATERCHIP_TEST_RESULT_SKIPPED then

// BDD-ish alias for the xUnit test()
#define it(example_name) test(example_name)

// BDD-ish alias for the xUnit test()
#define example(example_name) test(example_name)

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
    if not waterchip_data then waterchip_data = __waterchip_data; \
    if not __waterchip_testsuite_data then begin \
        __waterchip_debug_test_suite(""); \
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
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status != WATERCHIP_TEST_RESULT_SKIPPED then \
                call start; \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_NOT_RUN then begin \
                __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status = WATERCHIP_TEST_RESULT_PASSED; \
                __waterchip_testsuite_total_passed++; \
            end else if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_SKIPPED then \
                __waterchip_testsuite_total_skipped++; \
            __waterchip_debug_test(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status); \
            if __waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].status == WATERCHIP_TEST_RESULT_FAILED then begin \
                __waterchip_testsuite_total_failed++; \
                __waterchip_testsuite_currently_running_test_failure_message_lines = string_split(__waterchip_testsuite_data.test_results[__waterchip_testsuite_currently_running_test_name].failure_message, "\n"); \
                foreach __waterchip_testsuite_currently_running_test_failure_message_line in __waterchip_testsuite_currently_running_test_failure_message_lines \
                    __waterchip_debug_test(__waterchip_testsuite_currently_running_test_failure_message_line); \
            end \
        end \
        if __waterchip_testsuite_total_failed > 0 then \
            __waterchip_debug_test_suite("[Tests failed] [" + __waterchip_testsuite_total_failed + " failed, " + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
        else \
            __waterchip_debug_test_suite("[Tests passed] [" + __waterchip_testsuite_total_passed + " passed, " + __waterchip_testsuite_total_skipped + " skipped]"); \
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
    end \
    __waterchip__use_in_macro_to_support_semicolon = 0

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
    __waterchip__use_in_macro_to_support_semicolon = 0

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
            fail(sprintf_array("Expected to equal.\nExpected: '%s'\nActual: '%s'", [expected, __waterchip_testsuite_current_expect_value])); \
    end \
    __waterchip__use_in_macro_to_support_semicolon = 0

// Expectation for inequality
#define not_to_equal(expected) \
    if __waterchip_testsuite_current_expect_value == expected then \
        fail(sprintf_array("Expected not to equal.\nExpected: '%s'\nActual: '%s'", [expected, __waterchip_testsuite_current_expect_value]))
       
#define to_be_zero to_equal(0)

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
        default: print("not handling typeof " + typeof(__waterchip_testsuite_current_expect_value)); \
    end \
    __waterchip__use_in_macro_to_support_semicolon = 0

// Expectation for whether a string contains a substring or array contains item 
// typeof: 1 (int) 2 (float) 3 (string)
#define to_contain 0
