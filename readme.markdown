Open CL test
---

Array addition with/without OpenCL on the GPU `C = A + B` where A, B, C = float arrays of length n.

```
A[ 0 1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 ]
+
B[ 10.123 9.023 7.923 6.823 5.723 4.623 3.523 2.423 1.323 0.223 ]
=
C[ 10.123 10.123 10.123 10.123 10.123 10.123 10.123 10.123 10.123 10.123 ]
```

Speed Comparison for array of length n = 10 million :
---

Using GPU for 10M floats results in a ~50% speedup

```
./test_nocl1 10000000
Set n = 10000000 (10M)
Running sim...
16 November 2014 11:44:48 PM
Simulation completed in 0.05201 seconds.
16 November 2014 11:44:48 PM
Done.
```

```
./test_cl1 10000000
Set n = 10000000 (10M)
Running sim...
16 November 2014 11:45:04 PM
Simulation completed in 0.000346 seconds.
16 November 2014 11:45:04 PM
Done.
```