> work in progress, hold tight...

---

# Waterchip 

> Micro test framework for Fallout 2

```pas
#include "Waterchip.h"

describe("Inventory Helper") begin

    variable who = dude_obj;

    setup begin
        clear_inventory(who);
    end

    test("inventory should be empty") begin
        expect(inven_count(who)) to_equal(0);
        expect(inven_count(who)) not_to_equal(69);
    end

    test("can add weapon to dude's inventory") begin
        give_weapon(who, PID_SHOTGUN);
        expect(inven_count(who)) to_equal(1);
    end

    xtest("can equip weapon in dude's hands") begin
        print("hello. I am in test");
    end

    todo("Pending test without a body");

    teardown begin
        print("something that runs after each test");
    end
    
end
```

# Installation

Copy the latest [`Waterchip.h`](https://raw.githubusercontent.com/mrowrpurr/Waterchip/main/scripts/headers/Waterchip.h) into your `scripts/HEADERS/` folder.

# Getting Started

## Creating a new test script

1. Create a new global script for each group of tests you would like to create.
   > e.g. `scripts/gl_MyMod_InventoryHelperTest.ssl`

2. Include the Waterchip testing framework:
    ```pas
    #include "Waterchip.h"
    ```

3. Include your own code from header files:
    ```pas
    // You should generally pull the code you want to test out into headers.
    // There are also ways to interact with your running scripts, however.
    #include "MyMod/InventoryHelper.h"
    ```

4. Add a `describe()` to describe your group of tests in this file:
    ```pas
    // Let's say that you have inventory helper procedures you want to test
    describe("MyMod Inventory Helper") begin

    end
    ```
