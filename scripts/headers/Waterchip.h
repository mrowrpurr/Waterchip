/*
    Library for Waterchip test/spec framework ~ by Mrowr Purr ~
*/

// Imported variable for Waterchip test runner.
import variable __waterchip_data;

// Local variable for accessing ___waterchip_data using Map accessors.
// Workaround for SSLC bug: Map accessors don't work on import/export variables.
variable waterchip_data;

// Stores the name provided via describe("") or context("") or test_suite("")
variable __waterchip__testsuite_name = "Tests";

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
