The switch sketches are implemented on a P4 programmable switch equipped with a 3.2 Tbps Intel Tofino chip. To compile the P4 code deployed, use the following command: `./p_4_build.sh`. After the code is compiled, you can check the resource usage by running: `./p4i.sh`. These commands allow you to build the code and monitor the resource usage.

This project's CPU version requires a C++11 or higher version. The command `g++ -std=c++17 -O3 -o main main.cpp` can be used for compilation.

**Switch** refers to the code deployed on the programmable switch, while **cpu** contains its CPU version. **Server** refers to the method for training parameters on the server, with corresponding tests (in actual applications, testing is not required; only training parameters are needed).

**cpu/sketch** includes all the sketches that need to be used.

**cpu/report** includes the validation of flow size distribution estimation and throughput validation.

**cpu/tool** includes the hash functions required and methods for loading datasets.

**cpu/main** can initiate different comparison experiment functions from CPU/report, allowing specific experiments to be run by setting a corresponding flag to 1.

**server/samplerate** and **server/trainingfrequency** represent experiments conducted on sampling rate and training frequency configurations, mainly targeting flow size estimation.

The control plane can modify the RMT table through BFShell.
