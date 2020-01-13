# dice_roller v3.0
compound statement dice simulating

written in C++ for windows machines but can compile on linux after removing console color calls

unlimited modifiers, + or - are valid (d6+1+2-3+5-7+999)

3.0 supports subtracting dice groups from eachother, modifiers will be totaled before subtracting

also now supports limited multiplication and division. Order of operations does not apply however


advantage, disadvantage, & crits are now specified per dicegroup.

eg:

a20+7 will roll 2d20, take the higher and add 7.

v20+7 will take the lower result.

c8+4+5c6 will treat both d8 and 5d6 as crits


example of use:

:d20

--> 16

:d20-2
--> 14+-2 ---> 12

:2d20+7

--> 6+7 ---> 13

--> 4+7 ---> 11



:d6+2-3+4+5+6+7+321+3d4+3

--> 338

--> 11



--------> 349



:d8+4+5d6

--> 5

--> 20



--------> 25



:3d6+2d10+d12+8

--> 17

--> 15

--> 19



--------> 51

:_
