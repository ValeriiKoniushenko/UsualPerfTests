```
------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
FindFirstStruct_CFor                                 0.227 ns        0.227 ns   3092790752
FindFirstStruct_RangeBasedFor_RefCompression         0.226 ns        0.226 ns   3084435732
FindFirstStruct_RangeBasedFor_NonRefCompression      0.226 ns        0.226 ns   3092424989
FindFirstStruct_Find                                 30878 ns        30876 ns        22632
FindFirstStruct_FindIf                               23912 ns        23908 ns        29185
FindFirstStruct_RangesFindIf                         31331 ns        31327 ns        22704
FindFirstStruct_FindIfWithStaticPred                 24371 ns        24369 ns        29148
FindFirstStruct_RangesFindIfWithStaticPred           30992 ns        30988 ns        22422
CustomStrStr                                          7.19 ns         7.19 ns     96685861
StdStrStr                                            0.228 ns        0.227 ns   3095542951
CustomCaseLessStrStr                                  37.4 ns         37.3 ns     18531536
CustomCaseLessStrStrWithPredictions                   37.3 ns         37.2 ns     18880416
NonStaticLambda                                      89309 ns        89204 ns         7644
StaticLambda                                         88987 ns        88982 ns         7873
ArrWithoutDefaultZero                                88948 ns        88940 ns         7872
ArrWithDefaultZero                                   88990 ns        88978 ns         7865
StructWithoutDefaultZero                             59421 ns        59418 ns        11775
StructWithDefaultZero                                74350 ns        74340 ns         9433
JustVariableAdd                                      29807 ns        29803 ns        23525
HeapPointerAdd                                       30658 ns        30628 ns        22920
HeadRestrictPointerAdd                               29714 ns        29713 ns        23400
HeapSmartPointerAdd                                  30537 ns        30533 ns        22985
StackPointerAdd                                      29659 ns        29659 ns        23616
StackRefAdd                                          29992 ns        29988 ns        23484
```