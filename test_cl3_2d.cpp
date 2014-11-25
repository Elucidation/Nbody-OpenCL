#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <utility>
#include <fstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
using namespace cl;

//////////////////////////
void timestamp();
void showArray(int array[], int n);
//////////////////////////

int main(int argc, char *argv[])
{
    unsigned int n;

    // arg inputs
    if (argc < 2) { 
        std::cout << "Usage: test_cl3_2d.cpp <N>" << std::endl
             << "Where N = length of array" << std::endl;
        return -1;
    }
    n = atoi(argv[1]);
    n = n < 0 ? 0 : n > 20000 ? 20000 : n; // clamp [0 - 20000]
    std::cout << "Set n = " << n << " (" << n/1000 << "K)" << std::endl;

    // 1 - PRE
    std::vector<Device> devices;
    Program program;

    try {
        // Set up opencl gpu stuff

        // Get all platforms
        std::vector<Platform> platforms;
        Platform::get(&platforms);
        // printPlatformListInfo(platforms);

        // Select the default platform and create a context using this platform and the GPU
        cl_context_properties cps[3] = { 
            CL_CONTEXT_PLATFORM, 
            (cl_context_properties)(platforms[0])(), 
            0 
        };
        Context context( CL_DEVICE_TYPE_GPU, cps);

         // Get a list of devices on this platform
        devices = context.getInfo<CL_CONTEXT_DEVICES>();
        // printDeviceListInfo(devices);
 
        // Create a command queue and use the first device
        CommandQueue queue = CommandQueue(context, devices[0]);

        // Read source file
        std::ifstream sourceFile("matrix_test1.cl");
        std::string sourceCode(
            std::istreambuf_iterator<char>(sourceFile),
            (std::istreambuf_iterator<char>()));
        Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

        // Make program from source code in the context
        program = Program(context, source);

        // Build program for these specific devices
        program.build(devices);

        // Make kernel
        Kernel kernel(program, "matrix_test1");

        // Create memory buffers
        Buffer bufferA = Buffer(context, CL_MEM_WRITE_ONLY, n*n * sizeof(int));
        Buffer bufferB = Buffer(context, CL_MEM_WRITE_ONLY, n*n * sizeof(int));

        // Set arguments to kernel
        kernel.setArg(0, bufferA);
        kernel.setArg(1, bufferB);


        // 2 - SIM
        //////////////////////////////////////////////////////////////////////////
        std::cout << "Running sim..." << std::endl;
        timestamp();
        clock_t t1 = clock();

        // Run the kernel on specific ND range
        NDRange global(n,n);
        NDRange local(2,2);
        //  (Kernel &kernel, NDRange &offset, NDRange &global, NDRange &local)
        queue.enqueueNDRangeKernel(kernel, NullRange, global, local);

        clock_t t2 = clock();
        std::cout << "Simulation completed in " << double(t2-t1)/CLOCKS_PER_SEC << " CPU seconds.\n";
        timestamp();
        std::cout << "Done." << std::endl;
        //////////////////////////////////////////////////////////////////////////

        // Read data and display
        int *mat = new int[n*n]; // 2D array of size [N x N]
        for (unsigned int i = 0; i < n*n; ++i)
        {
            mat[i] = 0;
        }

        // Read Buffer A
        queue.enqueueReadBuffer(bufferA, CL_TRUE, 0, n*n * sizeof(int), mat);
        if (n < 100)
        {
            std::cout << "A : " << std::endl;
            showArray(mat, n);
        }

        for (unsigned int i = 0; i < n*n; ++i)
        {
            mat[i] = 0;
        }
        queue.enqueueReadBuffer(bufferB, CL_TRUE, 0, n*n * sizeof(int), mat);
        if (n < 100)
        {
            std::cout << "B : " << std::endl;
            showArray(mat, n);
        }

        delete[] mat;

    }
    catch (Error error)
      {
          std::cout << "CL ERROR: " << error.what() << "(" << error.err() << ")" << std::endl;
          if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
            std::cout << "Build log:" << std::endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
          }   
          throw error;
      }

    // 3 - POST
    // None

    return 0;
}

// Helper functions
///////////////////////////////////////////////////////////////////
void timestamp()
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  // size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << std::endl;

  return;
# undef TIME_SIZE
}

void showArray(int array[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cout << std::setw(5) << array[i*n + j] << " ";
        }
        std::cout << std::endl;
    }
    // std::cout << "Transposed: " << std::endl;
    // for (int i = 0; i < n; ++i)
    // {
    //     for (int j = 0; j < n; ++j)
    //     {
    //         std::cout << std::setw(5) << array[j*n + i] << " ";
    //     }
    //     std::cout << std::endl;
    // }
}