#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <utility>
#include <fstream>
#include <ctime>
#include <iostream>
#include <string>
using namespace cl;

// *tip* Use ldconfig after install of amd opencl to update libraries.

//////////////////////////
void timestamp();
void showArray(double array[], int n);
void printPlatformListInfo(std::vector<Platform> & platforms);
void printDeviceListInfo(std::vector<Device> & devices);
//////////////////////////

int main(int argc, char *argv[])
{
    unsigned int n;

    // arg inputs
    if (argc < 2) { 
        std::cout << "Usage: test1_cl.cpp <N>" << std::endl
             << "Where N = length of array" << std::endl;
        return -1;
    }
    n = atoi(argv[1]);
    n = n < 0 ? 0 : n > 100000000 ? 100000000 : n; // clamp [0 - 100000000]
    std::cout << "Set n = " << n << " (" << n/1000000 << "M)" << std::endl;

    // 1 - PRE
    double *a = new double[n];
    double *b = new double[n];
    double *c = new double[n];

    for (unsigned int i = 0; i < n; ++i)
    {
        a[i] = i;
        b[i] = n-i;
        c[i] = 0;
    }

    if (n < 100)
    {
        std::cout << "A"; showArray(a, n); std::cout << std::endl;
        std::cout << "B"; showArray(b, n); std::cout << std::endl;
    }

    try {
        // Set up opencl gpu stuff

        // Get all platforms
        std::vector<Platform> platforms;
        Platform::get(&platforms);
        printPlatformListInfo(platforms);

        // Select the default platform and create a context using this platform and the GPU
        cl_context_properties cps[3] = { 
            CL_CONTEXT_PLATFORM, 
            (cl_context_properties)(platforms[0])(), 
            0 
        };
        Context context( CL_DEVICE_TYPE_GPU, cps);

         // Get a list of devices on this platform
        std::vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        printDeviceListInfo(devices);
 
        // Create a command queue and use the first device
        CommandQueue queue = CommandQueue(context, devices[0]);

        // Read source file
        std::ifstream sourceFile("vector_add_kernel.cl");
        std::string sourceCode(
            std::istreambuf_iterator<char>(sourceFile),
            (std::istreambuf_iterator<char>()));
        Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

        // Make program from source code in the context
        Program program = Program(context, source);

        // Build program for these specific devices
        program.build(devices);

        // Make kernel
        Kernel kernel(program, "vector_add");

        // Create memory buffers
        Buffer bufferA = Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
        Buffer bufferB = Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
        Buffer bufferC = Buffer(context, CL_MEM_WRITE_ONLY, n * sizeof(int));

    }
    catch (Error error)
    {
        std::cout << "CL ERROR: " << error.what() << "(" << error.err() << ")" << std::endl;
    }

    // 2 - SIM
    //////////////////////////////////////////////////////////////////////////
    std::cout << "Running sim..." << std::endl;
    timestamp();

    for (unsigned int i = 0; i < n; ++i)
    {
        c[i] += a[i] + b[i];
    }

    timestamp();
    std::cout << "Done." << std::endl;
    //////////////////////////////////////////////////////////////////////////

    // 3 - POST
    if (n < 100)
    {
        std::cout << "C"; showArray(c, n); std::cout << std::endl;
    }

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}

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

void showArray(double array[], int n)
{
    std::cout << "[ ";
    for (int i = 0; i < n; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << "]";
}

void printPlatformListInfo(std::vector<Platform> & platforms)
{
    std::cout << "Got " << platforms.size() << " platforms." << std::endl;
    std::cout << "-----------------" << std::endl;
    for (std::vector<Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it)
    {
        STRING_CLASS name, extensions, profile, vendor, version;
        (*it).getInfo(CL_PLATFORM_NAME, &name);
        (*it).getInfo(CL_PLATFORM_EXTENSIONS, &extensions);
        (*it).getInfo(CL_PLATFORM_PROFILE, &profile);
        (*it).getInfo(CL_PLATFORM_VENDOR, &vendor);
        (*it).getInfo(CL_PLATFORM_VERSION, &version);
        std::cout << "  Platform: " << name << std::endl;
        std::cout << "  Extensions: " << extensions << std::endl;
        std::cout << "  Profile: " << profile << std::endl;
        std::cout << "  Vendor: " << vendor << std::endl;
        std::cout << "  Version: " << version << std::endl;
        if ((it+1) != platforms.end()) {std::cout << std::endl;}
    }
    std::cout << "-----------------" << std::endl;
}

void printDeviceListInfo(std::vector<Device> & devices)
{
    std::cout << "Got " << devices.size() << " devices." << std::endl;
    std::cout << "-----------------" << std::endl;
    for (std::vector<Device>::iterator it = devices.begin(); it != devices.end(); ++it)
    {
        STRING_CLASS built_in_kernels, extensions, name, opencl_c_version, profile, vendor, version, driver_version;
        (*it).getInfo(CL_DEVICE_BUILT_IN_KERNELS, &built_in_kernels);
        (*it).getInfo(CL_DEVICE_EXTENSIONS, &extensions);
        (*it).getInfo(CL_DEVICE_NAME, &name);
        (*it).getInfo(CL_DEVICE_OPENCL_C_VERSION, &opencl_c_version);
        (*it).getInfo(CL_DEVICE_PROFILE, &profile);
        (*it).getInfo(CL_DEVICE_VENDOR, &vendor);
        (*it).getInfo(CL_DEVICE_VERSION, &version);
        (*it).getInfo(CL_DRIVER_VERSION, &driver_version);
        std::cout << "  Built_in_kernels: " << built_in_kernels << std::endl;
        std::cout << "  Extensions: " << extensions << std::endl;
        // std::cout << "  Max_work_item_sizes: " << max_work_item_sizes << std::endl;
        std::cout << "  Name: " << name << std::endl;
        std::cout << "  Opencl_c_version: " << opencl_c_version << std::endl;
        // std::cout << "  Partition_properties: " << partition_properties << std::endl;
        // std::cout << "  Partition_type: " << partition_type << std::endl;
        std::cout << "  Profile: " << profile << std::endl;
        std::cout << "  Vendor: " << vendor << std::endl;
        std::cout << "  Version: " << version << std::endl;
        std::cout << "  Driver Version: " << driver_version << std::endl;
        if ((it+1) != devices.end()) {std::cout << std::endl;}
    }
    std::cout << "-----------------" << std::endl;
}