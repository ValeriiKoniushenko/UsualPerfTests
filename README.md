```
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
CustomStrStr                             0.202 ns        0.201 ns   3442169216
StdStrStr                                0.202 ns        0.202 ns   3433508949
CustomCaseLessStrStr                      33.0 ns         33.0 ns     20638661
CustomCaseLessStrStrWithPredictions       33.5 ns         33.5 ns     20774945
NonStaticLambda                          39924 ns        39820 ns        17598
StaticLambda                             39668 ns        39667 ns        17526
ArrWithoutDefaultZero                    40315 ns        40198 ns        17542
ArrWithDefaultZero                       39949 ns        39929 ns        17543
StructWithoutDefaultZero                 27149 ns        27087 ns        25696
StructWithDefaultZero                    33636 ns        33593 ns        20817
JustVariableAdd                          13365 ns        13319 ns        52552
HeapPointerAdd                           14104 ns        14039 ns        49244
HeadRestrictPointerAdd                   13662 ns        13607 ns        51040
HeapSmartPointerAdd                      13928 ns        13903 ns        49126
StackPointerAdd                          13392 ns        13345 ns        53026
StackRefAdd                              13961 ns        13933 ns        50962
```