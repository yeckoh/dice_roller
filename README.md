# dice_roller v2.6
# its unoptimized and doesn't necessarily follow good programming conventions and practices.
compound statement dice simulating

written in C++ for windows machines

supports 7 digit 'particles'

up to 10 separate dice groups (d8+5d6)

unlimited modifiers, + or - are valid (d6+1+2-3+5-7+999)

has optional flags, such as: crit, floor, verbose, clear, etc.

![Image of use](https://i.imgur.com/Z938wwi.png)


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
