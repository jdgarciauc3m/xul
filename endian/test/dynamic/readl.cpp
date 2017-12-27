#include "endian_converter.h"
#include <fstream>
#include <iostream>

template<xul::endian::endian_type ET>
void read_file(std::string fname)
{
  using namespace std;
  using namespace xul::endian;

  ifstream file(fname, ios::binary);
  dynamic_endian_converter<ET> ec;

  byte_sequence<4> x;
  x.read(file);
  byte_sequence<4> y;
  y.read(file);

  int32_t xvalue = ec.template to_host<int32_t>(x);
  float yvalue = ec.template to_host<float>(y);

  cout << xvalue << endl;
  cout << yvalue << endl;

}

int main() {
  using namespace xul::endian;
  read_file<endian_type::little>("little.dat");
  read_file<endian_type::big>("big.dat");

  return 0;
}
