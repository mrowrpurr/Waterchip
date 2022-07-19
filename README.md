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

1. Download the latest [`Waterchip.zip`](https://github.com/mrowrpurr/Waterchip/releases/latest/download/Waterchip.zip) release.
2. Extract it into your Fallout 2 folder.
3. Done!

> The archive includes 2 files:
> 
> - `scripts/HEADERS/Waterchip.h`
> - `mods/Waterchip.dat`
>
> This is assuming that you use `scripts/HEADERS/` and the `mods/` folder.

### _Manual Installation_

1. _Copy the latest [`Waterchip.h`](https://raw.githubusercontent.com/mrowrpurr/Waterchip/main/scripts/headers/Waterchip.h) into your `scripts/HEADERS/` folder._
2. _Copy [`gl___waterchip_test_runner.int`](https://github.com/mrowrpurr/Waterchip/raw/main/data/scripts/gl___waterchip_test_runner.int) into your `data/scripts/` folder._
3. _Done!_

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
        // Your tests will go here
    end
    ```

5. Add a `test()` to describe a specific piece of functionality you want to test:
    ```pas
    describe("MyMod Inventory Helper") begin
        test("can clear inventory") begin
            // Your test and assertions will go here
        end
    end
    ```

6. Arrange everything that you need for your test to run:
    ```pas
    describe("MyMod Inventory Helper") begin
        // Variables are declared inside of the describe() above tests
        variable critter;

        test("can clear inventory") begin
            // To test clearing inventory, we need a critter!
            critter = create_object(PID_PHIL, 0, 0);
            add_obj_to_inven(critter, create_object(PID_SHOTGUN, 0, 0));
            // Now we have a critter with some inventory!

            // Next, we want to call our inventory helper procedures
            // to verify if they work! 
        end
    end
    ```

7. Now that you have your critter setup, time to call **your code** to verify it works:
    ```pas
    describe("MyMod Inventory Helper") begin
        variable critter;

        test("can clear inventory") begin
            critter = create_object(PID_PHIL, 0, 0);
            add_obj_to_inven(critter, create_object(PID_SHOTGUN, 0, 0));

            // Call *something* from your code to test!
            // This is just a simple example.
            call InventoryHelper_ClearInventory(critter);
        end
    end
    ```

8. Make assertions to verify that your code works:
    ```pas
    describe("MyMod Inventory Helper") begin
        variable critter;

        test("can clear inventory") begin
            critter = create_object(PID_PHIL, 0, 0);
            add_obj_to_inven(critter, create_object(PID_SHOTGUN, 0, 0));

            // Add an assertion *before* your procedure
            // to verify that your test is setup properly
            expect(inven_count(critter)) to_be_greater_than(0)

            call InventoryHelper_ClearInventory(critter);

            // Assert that it worked!
            expect(inven_count(critter)) to_equal(0)
        end
    end
    ```
    > **TODO:** _add `to_be_greater_than` assertion_

9. **Compile your code & run the game!**

10. Look in your `debug.log` and you should see something like this:
    ```
    TODO example output
    ```