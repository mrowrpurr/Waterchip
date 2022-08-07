/*
    Waterchip - a test framework for Fallout 2

    by Mrowr Purr
*/

/******************************************
   Domain Specific Language for Waterchip
******************************************/

#define describe(test_suite_name) \
    procedure __waterchip_tests(variable defining_tests, variable current_test, variable run_test, variable run_setup, variable run_teardown); \
    procedure start begin \
        call __waterchip_initialize(test_suite_name); \
        call __waterchip_tests(true, "", false, false, false); \
        variable test_count = len_array(__waterchip_data.test_results); \
        if test_count then begin \
            variable test_result; \
            foreach test_result in (__waterchip_data.test_results) begin \
                variable test_name = test_result.name; \
                if __waterchip_data.has_setup then call __waterchip_tests(false, test_name, false, true, false); \
                call __waterchip_tests(false, test_name, true, false, false); \
                if __waterchip_data.has_teardown then call __waterchip_tests(false, test_name, false, false, true); \
            end \
            test_output(sprintf("[Ran %s tests]", test_count)); \
        end else begin \
            test_output("[No tests defined]"); \
        end \
    end \
    procedure __waterchip_tests(variable defining_tests, variable current_test, variable run_test, variable run_setup, variable run_teardown)

#define it(test_name) \
    if defining_tests then call __waterchip_declare_test(test_name); \
    if run_test and current_test == test_name then

#define xit(test_name) \
    if defining_tests then call __waterchip_declare_test(test_name, true); \
    if false then

#define todo(test_name) \
    if defining_tests then call __waterchip_declare_test(test_name, true)

#define setup_once

#define setup \
    if defining_tests then __waterchip_data.has_setup = true; \
    if run_setup then

#define teardown \
    if defining_tests then __waterchip_data.has_teardown = true; \
    if run_teardown then

/******************************************
    Script Variables, global to script
******************************************/

variable __waterchip_data;

variable __watership_output_prefix = "[Waterchip] "

/******************************************
    Output helpers to print to debug_msg
******************************************/

#define test_output(text) \
    begin \
        if __waterchip_data and __waterchip_data.current_test_name then \
            debug_msg( \
                __watership_output_prefix + \
                sprintf(" [%s]", __waterchip_data.test_suite) + \
                sprintf(" [%s] ", __waterchip_data.current_test_name) + text \
            ); \
        else if __waterchip_data and __waterchip_data.test_suite then begin \
            debug_msg( \
                __watership_output_prefix + \
                sprintf(" [%s] ", __waterchip_data.test_suite) + text \
            ); \
        end \
    end \
    false

/******************************************
    Private procedures to support DSL
******************************************/

procedure __waterchip_initialize(variable test_suite_name) begin
    if not __waterchip_data then begin
        __waterchip_data = { "test_suite": test_suite_name };
        fix_array(__waterchip_data);
        __waterchip_data.test_results = [];
        fix_array(__waterchip_data.test_results);
    end
end

procedure __waterchip_declare_test(variable test_name, variable skip = false) begin
    variable test_result = { "name": test_name };
    fix_array(test_result);
    variable test_count = len_array(__waterchip_data.test_results);
    resize_array(__waterchip_data.test_results, test_count + 1);
    __waterchip_data.test_results[test_count] = test_result;
end
