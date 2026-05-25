**P4** and **FPGA** refer to the code deployed on the programmable switch, while **GPU** and **CPU** contain the GPU and CPU versions of LFE, respectively. **Server** refers to the method for training parameters on the server, together with the corresponding testing process (in practical applications, testing is unnecessary and only parameter training is required).

The P4 programmable switch equipped with a 3.2 Tbps Intel Tofino chip. To compile the P4 code deployed, use the following command: `./p_4_build.sh`. After the code is compiled, you can check the resource usage by running: `./p4i.sh`. These commands allow you to build the code and monitor the resource usage.

The GPU version of this project requires the NVIDIA CUDA Toolkit. It can be compiled using the command `nvcc LFE-Top-K.cu -std=c++11 -O2 -Wno-deprecated-gpu-targets -o LFE-Top-K`

This project's CPU version requires a C++11 or higher version. The command `g++ -std=c++17 -O3 -o main main.cpp` can be used for compilation.

**cpu/sketch** includes all the sketches that need to be used.

**cpu/report** includes the validation of flow size distribution estimation and throughput validation.

**cpu/tool** includes the hash functions required and methods for loading datasets.

**cpu/main** can initiate different comparison experiment functions from CPU/report, allowing specific experiments to be run by setting a corresponding flag to 1.

**server/samplerate** and **server/trainingfrequency** represent experiments conducted on sampling rate and training frequency configurations, mainly targeting flow size estimation.

The control plane can modify the RMT table through BFShell.
