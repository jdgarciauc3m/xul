#include "endian_converter.h"
#include <fstream>

template <xul::endian::endian_type ET>
void write_file(std::string fname) {
  using namespace std;
  using namespace xul::endian;

  ofstream file(fname, ios::binary);
  static_endian_converter<ET> ec;

  ec.from_host(1).write(file);
  ec.from_host(1.5f).write(file);
}

int main() {
  using namespace xul::endian;
  write_file<endian_type::little>("static_little.dat");
  write_file<endian_type::big>("static_big.dat");
  
  return 0;
}
