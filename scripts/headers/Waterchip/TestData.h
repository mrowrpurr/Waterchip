// Exported variable for Waterchip.h
export variable __waterchip_data;

// Local variable for accessing __waterchip_data using Map accessors.
// Workaround for SSLC bug: Map accessors don't work on import/export variables.
variable waterchip_data;

inline procedure TestData_Initialize begin
    // Setup map for all test data (persist in game session)
    __waterchip_data = {};
    fix_array(__waterchip_data);
    waterchip_data = __waterchip_data;

    // Test suites provide data bags for each test suite's status data
    waterchip_data.test_suites = {};
    fix_array(waterchip_data.test_suites);

    // Might include .current_test_name and .current_describe and stuff
    // like that in addition to storing the test results etc
    // ...
end
