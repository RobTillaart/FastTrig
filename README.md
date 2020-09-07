# FastTrig

Arduino library with interpolated lookup for sin() and cos(). Trades speed for accuracy.

## Description

**Warning: The library trades speed for accuracy so use at own risk**

The library provides one lookup table that is used for both
**isin(degrees)** and **icos(degrees)** and **itan(degrees)** (see remarks below). 


These functions are to be used as replacements for **sin(radians)** and **cos(radians)**. Important to know is that they are NOT direct replaceable as the parameter differs factor (PI/180.0) or its inverse.

Similar to ```cos(x) == sin(x + PI)``` it is also true that ```icos(x) == isin(x + 90)```, so **icos()** can use the very same lookup table at the cost of a single addition.

The **i** in the names stands for **int** and **interpolated** as the core is using integer math and lookuptable of 91 uint16_t = 182 bytes. By folding and mirroring the whole 360 degrees can be handled. If **isin(x)** is called and ```x == int(x)``` then the library will not interpolate and this will improve performance. If x is not a whole number the library will linear interpolate between **isin(int(x))** and **isin(int(x+1))**. Of course this introduces an error but it is quite fast.

The lookup tables are optimized to minimize the error when using the interpolation, this implies that the points in the table might not be optimal when you use only discrete angles. This is on the TODO list.

#### Lookup tables

The lookup tables used by **isin()** and **icos()** can be used directly in your program, the names are:
- **isinTable16\[\]** index 0..90, values need to be (float) divided by 65535
- **isinTable8\[\]** index 0..90, values need to be (float) divided by 255

The **isinTable8** is not really accurate for doing accurate math, 
however it is great to use in a LEDstrip or motor movements that use less accuracy.

Although the tables can be written to, it is advised not to do so

*OK, the optimize example does a write to improve the table to minimize errors*

## Performance

time in us  (based upon 360 calls - lib version 0.1.3)

| function | UNO 16MHz | ESP32 240 MHz |
|:----:|----:|----:|
|  sin |  120.43 | 10.90 |
| isin |  44.34 | 1.10 |
|  cos |  120.27 | 10.81 |
| icos |  60.92 | 1.17 |
|  tan |  147.59 | 18.07 |
| itan |  131.26 | 3.05 |

So there is quite some performance gain except for **itan()** on AVR 
as that has 2 lookups and an expensive division making it 3x slower than isin().
Same performance penalty on the ESP32 but as the overall performance is better
there is still a serious gain (at the price of accuracy).


## Accuracy

See example sketch.


## ITAN()

The library provides an **itan()** which is simply **isin()/icos()**.
Although this works it is on AVR (UNO) not substantially faster.
Also the accuracy is not very good. 

## TODO

- Design a way to improve the accuracy of the whole degrees, as now the table is optimized for interpolation.
- investigate **itan()**, how to improve on that (check notes.txt)
- 

## Operation

See examples

