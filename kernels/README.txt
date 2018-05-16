This directory contains all kernels (ACF, OpenCL...) used in SDK applications. 

To add a kernel here, just copy its sources into the appropriate architecture and category directory and add a reference into BUILD.mk file.
The kernel can be now used in arbitrary application (the apps BUILD.mk must be set correctly). Please refer to demos to see an example.