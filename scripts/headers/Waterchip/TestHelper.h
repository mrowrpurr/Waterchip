#pragma once

/*
    Helper for Waterchip's tests for Waterchip (testing itself!)
*/

#include "Waterchip.h"

#define get_key_or_none(map, key) \
    (map[key] if map_contains_key(map, key) else 0)

#define import_test_data(variable_name) \
    import variable variable_name;
