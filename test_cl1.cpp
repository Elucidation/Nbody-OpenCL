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
void printPlatformInfo(std::vector<Platform> & platforms);
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
		printPlatformInfo(platforms);

		// Select the default platform and create a context using this platform and the GPU
	    cl_context_properties cps[3] = { 
	        CL_CONTEXT_PLATFORM, 
	        (cl_context_properties)(platforms[0])(), 
	        0 
	    };
	    Context context( CL_DEVICE_TYPE_GPU, cps);


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

	delete[] A;
	delete[] B;
	delete[] C;

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

void printPlatformInfo(std::vector<Platform> & platforms)
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