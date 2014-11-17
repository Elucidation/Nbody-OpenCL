#include <ctime>
#include <iostream>
using namespace std;

#include <CL/cl.h>

//////////////////////////
void timestamp();
void showArray(double array[], int n);
//////////////////////////

int main(int argc, char *argv[])
{
	// 1 - PRE
	timestamp();

	int n = 10;

	double *a = new double[n];
	double *b = new double[n];
	double *c = new double[n];

	for (int i = 0; i < n; ++i)
	{
		a[i] = i;
		b[i] = 3 + 2*i;
	}

	cout << "A"; showArray(a, n); cout << endl;
	cout << "B"; showArray(b, n); cout << endl;

	// 2 - SIM
	for (int i = 0; i < n; ++i)
	{
		c[i] = a[i] + b[i];
	}
	cout << "C"; showArray(c, n); cout << endl;

	// 3 - POST
	timestamp();

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

  cout << time_buffer << endl;

  return;
# undef TIME_SIZE
}

void showArray(double array[], int n)
{
	cout << "[ ";
	for (int i = 0; i < n; ++i)
	{
		cout << array[i] << " ";
	}
	cout << "]";
}