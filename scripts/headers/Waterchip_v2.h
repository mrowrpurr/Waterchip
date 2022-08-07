/*
    Waterchip - a test framework for Fallout 2

    by Mrowr Purr
*/

/******************************************
    Script Variables, global to script
******************************************/

variable __waterchip_data;

// TODO move to waterchip_data
// variable __watership_output_prefix = "[Waterchip] "

/******************************************
   Procedure header declarations
******************************************/

// procedure __waterchip_initialize(variable test_suite_name);
// procedure __waterchip_tests(variable __waterchip_test_run_info);

/******************************************
   main() procedure for test scripts
******************************************/

// procedure start begin
//     call __waterchip_initialize(test_suite_name);
//     call __waterchip_tests(0);
//     variable test_count = len_array(__waterchip_data.test_results);
//     if test_count then begin
//         variable test_result;
//         foreach test_result in (__waterchip_data.test_results) begin
//             variable test_run_info = { "test_result": test_result };
//             fix_array(test_run_info);
//             if __waterchip_data.has_setup then begin
//                 call __waterchip_tests(false, test_name, false, true, false);
//             end
//             call __waterchip_tests(false, test_name, true, false, false);
//             if __waterchip_data.has_teardown then call __waterchip_tests(false, test_name, false, false, true);
//             free_array(test_run_info);
//         end
//         test_output(sprintf("[Ran %s tests]", test_count));
//     end else begin
//         test_output("[No tests defined]");
//     end
// end

/******************************************
   Core macros for defining/running tests
******************************************/

#define define_tests_proc(proc_name) \
    procedure proc_name(variable __waterchip_test_suite_info, variable __waterchip_test_run_info)

#define run_tests_proc(test_suite_name, proc_name) \
    call proc_name(__waterchip_initialize_test_suite(test_suite_name), 0)

#define define_test(test_name)

// #define context "TODO"

/******************************************
   Config, mostly for Waterchip's tests
******************************************/

#define waterchip_configure false

#define waterchip_store_output false

#define waterchip_do_not_display_output false

#define waterchip_preserve_data false

#define waterchip_enable_test_mode false
    // waterchip_configure begin
    //     waterchip_store_output;
    //     waterchip_do_not_display_output;
    //     waterchip_preserve_data;
    // end

/******************************************
   Domain Specific Language for Waterchip
******************************************/

// #define describe(test_suite_name) \
//     procedure __waterchip_tests(variable __waterchip_test_run_info)

// #define it(test_name) \
//     if not __waterchip_test_run_info then call __waterchip_declare_test(test_name); \
//     else if __waterchip_test_run_info.run_test and __waterchip_test_run_info.current_test == test_name then

// #define xit(test_name) \
//     if not __waterchip_test_run_info then call __waterchip_declare_test(test_name, true); \
//     if false then

#define todo(test_name) \
    debug_msg("Suite Info: " + __waterchip_test_suite_info); \
    if not __waterchip_test_run_info then call __waterchip_declare_test(test_name, true)

// #define setup_once

// #define setup \
//     if not __waterchip_test_run_info then __waterchip_data.has_setup = true; \
//     else if __waterchip_test_run_info.run_setup then

// #define teardown \
//     if not __waterchip_test_run_info then __waterchip_data.has_teardown = true; \
//     else if __waterchip_test_run_info.run_teardown then

/******************************************
    Output helpers to print to debug_msg
******************************************/

// #define test_output(text) \
//     begin \
//         if __waterchip_data and __waterchip_data.current_test_name then \
//             debug_msg( \
//                 __watership_output_prefix + \
//                 sprintf(" [%s]", __waterchip_data.test_suite) + \
//                 sprintf(" [%s] ", __waterchip_data.current_test_name) + text \
//             ); \
//         else if __waterchip_data and __waterchip_data.test_suite then begin \
//             debug_msg( \
//                 __watership_output_prefix + \
//                 sprintf(" [%s] ", __waterchip_data.test_suite) + text \
//             ); \
//         end \
//     end \
//     false

/******************************************
    Private procedures
******************************************/

procedure __waterchip_initialize_data begin
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
        test_suite = {
            "name": test_suite_name
        };
        fix_array(test_suite);
        test_suites[test_suite_name] = test_suite;
    end

    return test_suite;
end

procedure __waterchip_destroy_test_suite begin

end

procedure __waterchip_destroy_data begin

end

procedure __waterchip_declare_test(variable test_name, variable skip = false) begin
    debug_msg("Waterchip: DECLARE " + test_name);

    // variable test_result = { "name": test_name };
    // fix_array(test_result);
    // variable test_count = len_array(__waterchip_data.test_results);
    // resize_array(__waterchip_data.test_results, test_count + 1);
    // __waterchip_data.test_results[test_count] = test_result;

    // // test_result.expectations
end

/******************************************
    Assertions
******************************************/

// #define expect(actual_value)
