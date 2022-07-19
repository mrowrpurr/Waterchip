# Waterchip 

> Micro test framework for Fallout 2

```pas
#include "Waterchip.h"

describe("Inventory Helper") begin

    variable who = dude_obj;

    setup begin
        call clear_inventory(who);
    end

    test("inventory should be empty") begin
        expect(inven_count(who)) to_equal(0);
        expect(inven_count(who)) not_to_equal(69);
    end

    test("can add weapon to dude's inventory") begin
        call give_weapon(who, PID_SHOTGUN);
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

Create a new global script for each group of tests you would like to create.

e.g. `scripts/gl_MyMod_InventoryHelperTest.ssl`

Then `#include 

In your script, do **NOT** create a `procedure start`.

Instead, initialize the script using `describe("name of your test group")`

```pas
#include "Waterchip.h"

// Describe whatever it is that you're testing
describe("My mod's Inventory Helper") begin

end
```

# Usage


