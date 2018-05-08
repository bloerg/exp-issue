# exp-issue
A problem showing during the port of a CUDA programme to OpenCL. The functions expf() in C++ and CUDA give different results than exp() in OpenCL. This is for the same typewidth. Where the numbers differ, the observed difference can be found in the last digit.

## usage

The following commands should clone the git repo, compile the two source code files, generate two lists of each 10000 random numbers and 
feed the exp() and the expf() functions with those numbers. The results are written in two csv files. Afterwards the differing entries
are printed to the screen. 

```
git clone ssh://git@git.bloerg.de:60022/studium/exp-issue.git
cd exp-issue
make clean && make all
./exp_cl > output_opencl.csv
./exp_cpp > output_cpp.csv
diff output_cpp.csv output_opencl.csv
```


