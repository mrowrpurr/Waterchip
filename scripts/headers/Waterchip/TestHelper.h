#pragma once

/*
    Helper for Waterchip's tests for Waterchip (testing itself!)
*/

#include "Waterchip.h"

#define get_key_or_none(map, key) \
    (map[key] if map_contains_key(map, key) else 0)

#define import_test_data(variable_name) \
    import variable variable_name;

#define get_test(test_suite, test_name) \
    (first_equal_map_field_value(test_suite.tests, "name", test_name))

procedure first_equal_map_field_value(variable array, variable key_name, variable desired_value) begin
    variable element;
    foreach element in array begin
        if get_key_or_none(element, key_name) == desired_value then
            return element;
    end
    return {};
end
