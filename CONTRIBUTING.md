# Contributing guidelines

Code contributions welcomed!<br/>
The guideline is simple:<br/>

1. Conform to the license (MIT)
1. **No dependencies** (C library for C is ok, POSIX is OK, STL for C++ is ok)
1. Document your code and test the doxygen output
1. C++ code should be wrappers only - the actual code must be written in C
1. C++ code should be C++11
1. C++ code must be inline so it does not go into the shared object
1. If possible, add an example (C or C++) in the examples folder, make sure to update the makefile for it
1. Take a look at the code and try to match the coding conventions

That's it, thanks for contributing!
