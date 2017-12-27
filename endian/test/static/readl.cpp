#include "endian_converter.h"
#include <fstream>
#include <iostream>
#include <cstdint>

template <xul::endian::endian_type ET>
void read_file(std::string fname)
{
  using namespace std;
  using namespace xul::endian;

  ifstream file(fname, ios::binary);
  static_endian_converter<ET> ec;

  byte_sequence<4> x;
  x.read(file);
  byte_sequence<4> y;
  y.read(file);
  byte_sequence<4> z;
  z.read(file);

  int32_t xvalue = ec.template to_host<int32_t>(x);
  float yvalue = ec.template to_host<float>(y);
  unsigned int zvalue = ec.template to_host<unsigned int>(x);

  cout << xvalue << endl;
  cout << yvalue << endl;
  cout << zvalue << endl;
}

int main() {
  using namespace xul::endian;
  read_file<endian_type::little>("static_little.dat");
  read_file<endian_type::big>("static_big.dat");

  return 0;
}
