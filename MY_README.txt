#Running google profiler
export LD_PRELOAD=/usr/local/lib/libprofiler.so
gcc-4.7 -std=c99 -g && CPUPROFILE=foo.prof ./a.out
pprof --text ./a.out foo.prof
