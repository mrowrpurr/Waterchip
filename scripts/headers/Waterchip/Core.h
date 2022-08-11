#pragma once

/*
    Core Waterchip necessary variables, macros, and procedures.

    Nothing works without the Core *except* assertions,
    which may be used outsideof Waterchip if so desired.

    Macros like "describe" and "TestFixture" are found in the Syntax\ folder.
    Macros like "to_equal" and "to_contain" are find in the Assertions\ folder.
*/

variable __waterchip_data;

#define WATERCHIP_TEST_RESULT_FAILED  "FAIL"
#define WATERCHIP_TEST_RESULT_PASSED  "PASS"
#define WATERCHIP_TEST_RESULT_SKIPPED "SKIP"
#define WATERCHIP_TEST_RESULT_NOT_RUN "NOT_RUN"

#define define_tests_proc(proc_name) \
    procedure proc_name(variable __waterchip_test_suite_info, variable WATERCHIP_TEST)

#define invoke_define_tests_proc(test_suite_name, proc_name, test_run_info) \
    call proc_name(__waterchip_initialize_test_suite(test_suite_name), test_run_info)

#define define_skipped_test_no_body(test_name) \
    if not WATERCHIP_TEST then call __waterchip_declare_test(__waterchip_test_suite_info, test_name, true)

#define define_test(test_name) \
    if not WATERCHIP_TEST then call __waterchip_declare_test(__waterchip_test_suite_info, test_name); \
    else if WATERCHIP_TEST.name == test_name then

#define define_skipped_test(test_name) \
    if not WATERCHIP_TEST then call __waterchip_declare_test(__waterchip_test_suite_info, test_name, true); \
    else if false then

#define define_skipped_test_without_body(test_name) \
    if not WATERCHIP_TEST then call __waterchip_declare_test(__waterchip_test_suite_info, test_name, true)

procedure __waterchip_initialize_data begin
    if __waterchip_data then return;

    // Top-level structure for storing this script's Waterchip-related data
    __waterchip_data = {};
    fix_array(__waterchip_data);

    // Array for output for when waterchip_store_output is configured
    variable stored_output = [];
    fix_array(stored_output);
    __waterchip_data.stored_output = stored_output;

    // Map of test suite data, keyed on test suite description
    variable test_suites = {};
    fix_array(test_suites);
    __waterchip_data.test_suites = test_suites;
end

procedure __waterchip_initialize_test_suite(variable test_suite_name) begin
    if not __waterchip_data then
        call __waterchip_initialize_data;

    variable test_suites = __waterchip_data.test_suites;
    variable test_suite = test_suites[test_suite_name];

    if not test_suite then begin
        // Test suite
        test_suite = { "name": test_suite_name };
        fix_array(test_suite);

        // Defined tests with their test results (when run)
        variable test_suite_results = [];
        fix_array(test_suite_results);
        test_suite.tests = test_suite_results;

        test_suites[test_suite_name] = test_suite;
    end

    return test_suite;
end

procedure __waterchip_destroy_test_suite begin

end

procedure __waterchip_destroy_data begin

end

procedure __waterchip_declare_test(variable test_suite, variable test_name, variable skip = false) begin
    // Setup representation of this test and its result
    variable test_result = { "name": test_name };
    fix_array(test_result);

    if skip then
        test_result.status = WATERCHIP_TEST_RESULT_SKIPPED;
    else
        test_result.status = WATERCHIP_TEST_RESULT_NOT_RUN;

    // Add to the full list of tests for this test suite
    variable test_count = len_array(test_suite.tests);
    resize_array(test_suite.tests, test_count + 1);
    test_suite.tests[test_count] = test_result;

    // Setup an object for expectation info, e.g. failure message
    variable expectations_info = {};
    fix_array(expectations_info);
    test_result.expectations = expectations_info;
end