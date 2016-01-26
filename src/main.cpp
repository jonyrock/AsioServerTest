#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

int main() {
  using boost::lexical_cast;
  short num = lexical_cast<short>("123");
  cout << "hello, num + 1: " << num << endl;
}