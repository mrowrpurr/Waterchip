> work in progress, hold tight...

---

# Waterchip 

> Micro test framework for Fallout 2

```pas
#include "Waterchip.h"
#include "MyMod/InventoryHelper.h"

describe("Inventory Helper") begin
    variable critter;

    setup begin
        critter = create_object(PID_PHIL, 0, 0);
        clear_inventory(critter);
    end

    test("inventory should be empty") begin
        expect(inven_count(critter)) to_equal(0);
        expect(inven_count(critter)) not_to_equal(69);
    end

    test("can add weapon to dude's inventory") begin
        give_weapon(critter, PID_SHOTGUN);
        expect(inven_count(critter)) to_equal(1);
    end
end
```

# Installation

## Archive

1. Download the latest [`Waterchip.zip`](https://github.com/mrowrpurr/Waterchip/releases/latest/download/Waterchip.zip) release.
2. Extract it into your Fallout 2 folder.
3. Done!

> The archive includes 2 files:
> 
> - `scripts/HEADERS/Waterchip.h`
> - `mods/Waterchip.dat`

## Manual

1. Copy the latest [`Waterchip.h`](https://raw.githubusercontent.com/mrowrpurr/Waterchip/main/scripts/headers/Waterchip.h) into your `scripts/HEADERS/` folder.
2. Copy [`gl___waterchip_test_runner.int`](https://github.com/mrowrpurr/Waterchip/raw/main/data/scripts/gl___waterchip_test_runner.int) into your `data/scripts/` folder.
3. Done!

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
