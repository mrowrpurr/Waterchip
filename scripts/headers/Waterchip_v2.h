/*
    Waterchip - a test framework for Fallout 2

    by Mrowr Purr
*/

/******************************************
   Domain Specific Language for Waterchip
******************************************/

#define describe(test_suite_name) \
    procedure __waterchip_tests(variable current_test); \
    procedure start begin \
        call __waterchip_initialize(test_suite_name); \
        call __waterchip_tests(0); \
        variable test_count = len_array(__waterchip_data.test_results); \
        if test_count then begin \
            test_output(sprintf("[%s tests defined]", test_count)); \
        end else begin \
            test_output("[No tests defined]"); \
        end \
    end \
    procedure __waterchip_tests(variable current_test)

#define it(test_name)

#define xit(test_name)

#define todo(test_name)

#define setup_once

#define setup

#define teardown

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

procedure __waterchip_declare_test(variable test_name) begin
    variable test_result = { "name": test_name };
    fix_array(test_result);
    variable test_count = len_array(__waterchip_data.test_results);
    resize_array(__waterchip_data.test_results, test_count + 1);
    __waterchip_data.test_results[test_count] = test_result;
end
