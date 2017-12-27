#include "system_meter.h"
#include <iostream>
#include <sstream>
#include <thread>

int main(int argc, char ** argv) {
  using namespace xul::time_meter;
  using namespace std;

  if (argc != 2) {
    cerr << "Wrong format" << endl;
    cerr << argv[0] << " milisecs" << endl;
    return 1;
  }

  unsigned int delay;
  istringstream(argv[1]) >> delay;
  cout << "Delay: " << delay << endl;

  system_meter<chrono::system_clock> m;
  m.start();
  this_thread::sleep_for(chrono::milliseconds(delay));
  m.stop();
  cout << "Time: " << m.count<chrono::milliseconds>() << " milliseconds" << endl;

  return 0;
}
