# FastTrig

Arduino library with interpolated lookup for sin() and cos(). Trades speed for accuracy.

## Description

**Warning: The library trades speed for accuracy so use at own risk**

The library provides one lookup table that is used for
**isin(degrees)** and **icos(degrees)** and **itan(degrees)**. This lookup table is optimized for interpolation so the values for whole degrees are not optimal. Furthermore the **itan()** on AVR has almost no performance gain over the regular **tan()** so on AVR one is adviced to use **tan()**. On ESP32 the **itan(degrees)** does have a serious performance gain so use it if you need speed.

These functions are to be used as replacements for **sin(radians)**, **cos(radians)**and **tan(radians)**. Important to know is that they are NOT direct replaceable as the parameter differs a factor (PI/180.0) or its inverse.

Similar to ```cos(x) == sin(x + PI)``` it is also true that ```icos(x) == isin(x + 90)```, so **icos()** can use the very same lookup table at the cost of a single addition. In fact it uses ```icos(x) == isin(x - 270)``` as that performs better.

The **i** in the names stands for **int** and **interpolated** as the core is using integer math and lookuptable of 91 uint16_t = 182 bytes. By folding and mirroring the whole 360 degrees can be handled. When **isin(x)** is called and ```x == int(x)``` then the library will not interpolate and this will improve performance. When x is not a whole number the library will linear interpolate between **isin(int(x))** and **isin(int(x+1))**. Of course this introduces an error but it is quite fast.

#### Lookup tables

The lookup tables are optimized (sketch provided) to minimize the error when using the interpolation, this implies that the points in the table might not be optimal when you use only wholde degrees. A sketch that generates lookup tables is in the examples folder. This generator sketch can also generate tables with different resolution e.g. 24, 14, 12 or even 6, 5 or 4 bit lookup tables. So depending on the application these tables can be ideal, but verify they meet your requirements.

The lookup tables used by **isin()** can be used directly in your program, the names are:
- **isinTable16\[\]** index 0..90, values need to be (float) divided by 65535.0
- **isinTable8\[\]** index 0..90, values need to be (float) divided by 255.0

The **isinTable8** is not really for doing accurate math, 
however it is great to use in a LEDstrip or motor movements when less accuracy is needed.

Although the tables can be written to, it is advised not to do so.

*OK, the optimize example does a write to improve the table to minimize errors*

## Performance

time in us - calls 0 - 360 step 1 degree and calls 720 - 1080 (lib version 0.1.4)

| function | UNO 16MHz | ESP32 240 MHz | UNO  (720-1080) | ESP (720-1080) |
|:----:|:----:|:----:|:----:|:----:|
|  sin |  120.43 | 10.90 | 124.19 | 10.91 |
| isin |   44.24 |  1.09 |  85.00 |  1.11 |
|  cos |  120.27 | 10.81 | 123.98 | 10.83 |
| icos |   51.40 |  1.16 |  91.42 |  1.18 |
|  tan |  147.59 | 18.07 | 151.39 | 18.07 |
| itan |  126.73 |  1.31 | 129.93 |  1.29 |

*Note: itan() 0.1.3 was ( 131.23, 3.05 ) so it improved quite a bit on ESP32. *

Performance gain is most evident for the ESP32 processor, and much less on AVR.
The effect of the modulo (360 degrees) can be seen explicitly in AVR. 
Furthermore the **itan()** on AVR is not faster when there is also interpolation (not in table) 

The 0.1.4 version of **itan()** is faster for ESP32 than 0.1.3 version but the 
improvement on AVR is minimal. So this will stay on the TODO list.

Furthermore a lot of gain is lost when the angle is not within 0..360
and needs to be normalized ( expensive modulo on AVR ). It is worth noting that the
original **sin()** **cos()** and **tan()** only have a small overhead for 
values outside the 0..360 range.

Verify the performance to see if it meets your requirements.

## Accuracy

errors - based upon example sketch - lib version 0.1.4

ESP32 calls 0.0 - 360.0 step 0.1 degree

| function | max abs  error | avg abs error | max rel error | avg rel error |
|:----:|----:|----:|----:|----:|
| isin | 0.00012007 | 0.00002303 | 0.02955145 | 0.00035421 |
| icos | 0.00012007 | 0.00002250 | 0.02955145 | 0.00035035 |
| itan | 0.69696045 | 0.00634778 | 0.00144703 | 0.00008933 |

UNO calls 0.0 - 360.0 step 0.1 degree

| function | max abs  error | avg abs error | max rel error | avg rel error |
|:----:|----:|----:|----:|----:|
| isin | 0.00012016 | 0.00002302 | 0.02955145 | 0.00035411 |
| icos | 0.00012052 | 0.00002303 | 0.02949960 | 0.00035423 |
| itan | 0.72760009 | 0.00635350 | 0.00144703 | 0.00036722 | 

*Note: 0.1.3 for AVR was bad:   17.41900634 , 0.02249339 , 0.02953807 for itan() *

Strange that the **itan()** on UNO and ESP32 differs (OK same order of magnitude).
Different implementation of gonio / float math?

Verify the accuracy to see if it meets your requirements.

## 0.1.4

The library (0.1.4) provides an **itan()** which improved accuracy 
upon the (0.1.3) version and performance for the ESP32.  
Performance on AVR (UNO) is still an issue, accuracy is OK.

## TODO

- How to improve the accuracy of the whole degrees, as now the table is optimized for interpolation.
- investigate **itan()** lookup table with interpolation (see notes.txt). Interpolation is not 
trivial for angles between say 60-90 degrees 
- investigate if the inverse asin acos and maybe even atan can be faster with a lookup table. (see notes.txt).

## Operation

See examples

