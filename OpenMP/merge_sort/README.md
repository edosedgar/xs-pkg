# OpenMP Merge sort information 
* *Compile:*  
gcc -o merge_sort ./merge_sort.c -fopenmp -O3  

* *Usage:*  
./merge_sort ElemNum MaxOpenMPDepth CritSize  

  _ElemNum_ - number of elements in array  
  _MaxOpenMPDepth_ - number of nested parallel tasks while dividing into 2 parts  
  _CritSize_ - minimal size when merge is not efficient and it is reasonable to  
               use more lightweight and simpler sort algorithm, like bubble sort  

* *Perfomance analysis:*  
OMP_NUM_THREADS=1  ./merge_sort 100000000 0 8 ==============> 12.968146 sec  
OMP_NUM_THREADS=2  ./merge_sort 100000000 1 8 ==============> 7.100586 sec  
OMP_NUM_THREADS=4  ./merge_sort 100000000 2 8 ==============> 5.475003 sec  
OMP_NUM_THREADS=8  ./merge_sort 100000000 3 8 ==============> 5.348581 sec  
OMP_NUM_THREADS=16 ./merge_sort 100000000 4 8 ==============> 5.040582 sec  

  |  CPU Num. |  Time  |  Speedup  | Efficiency |
  |-----------|--------|-----------|------------|
  |     1     |  12.97 |    1.0    |      1.0   |
  |     2     |   7.1  |    1.8    |      0.9   |
  |     4     |  5.47  |    2.37   |     0.59   |
  |     8     |  5.34  |    2.42   |      0.3   |
  |    16     |    5   |    2.59   |     0.16   |
