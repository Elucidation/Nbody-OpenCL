#include <ctime>
#include <iostream>

typedef float arr_type;

//////////////////////////
void timestamp();
void showArray(arr_type array[], int n);
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
    arr_type *a = new arr_type[n];
    arr_type *b = new arr_type[n];
    arr_type *c = new arr_type[n];

    for (unsigned int i = 0; i < n; ++i)
    {
        a[i] = float(i) + float(i)/10.0;
        b[i] = n-a[i] + 0.123;
        c[i] = 0;
    }

    if (n < 100)
    {
        std::cout << "A"; showArray(a, n); std::cout << std::endl;
        std::cout << "B"; showArray(b, n); std::cout << std::endl;
    }

    // 2 - SIM
    //////////////////////////////////////////////////////////////////////////
    std::cout << "Running sim..." << std::endl;
    timestamp();
    clock_t t1 = clock();
    
    // Original implementation
    for (unsigned int i = 0; i < n; ++i)
    {
        c[i] += a[i] + b[i];
    }

    clock_t t2 = clock();
    std::cout << "Simulation completed in " << double(t2-t1)/CLOCKS_PER_SEC << " seconds.\n";
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

void showArray(arr_type array[], int n)
{
    std::cout << "[ ";
    for (int i = 0; i < n; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << "]";
}