

```pas

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

procedure start begin
    call __waterchip_initialize(test_suite_name);
    call __waterchip_tests(0);
    variable test_count = len_array(__waterchip_data.test_results);
    if test_count then begin
        variable test_result;
        foreach test_result in (__waterchip_data.test_results) begin
            variable test_run_info = { "test_result": test_result };
            fix_array(test_run_info);
            if __waterchip_data.has_setup then begin
                call __waterchip_tests(false, test_name, false, true, false);
            end
            call __waterchip_tests(false, test_name, true, false, false);
            if __waterchip_data.has_teardown then call __waterchip_tests(false, test_name, false, false, true);
            free_array(test_run_info);
        end
        test_output(sprintf("[Ran %s tests]", test_count));
    end else begin
        test_output("[No tests defined]");
    end
end

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

// #define todo(test_name) \
//     if not __waterchip_test_run_info then call __waterchip_declare_test(__waterchip_test_suite_info, test_name, true)

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

```