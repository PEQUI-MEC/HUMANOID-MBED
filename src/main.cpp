#include "mbed.h"
#include "SerialPins.h"
#include <iostream>
#include "Dense"

using namespace Eigen;
using namespace std;

Serial pc(USBTX, USBRX, 115200);

int main() {
  Timer timer;

  MatrixXd u1 = MatrixXd::Random(1,10) * 1000000;
  MatrixXd v1 = MatrixXd::Random(10,1) * 1000000;
  MatrixXd t1 = v1*u1;
  
  MatrixXd u2 = MatrixXd::Random(1,10) * 1000000;
  MatrixXd v2 = MatrixXd::Random(10,1) * 1000000;
  MatrixXd t2 = v2*u2;

  cout << "v1 = " << v1 << endl;
  cout << "u1 = " << u1 << endl;
  cout << "u1 * v1 =" << endl <<  t1 << endl << endl;

  cout << "v2 = " << v2 << endl;
  cout << "u2 = " << u2 << endl;
  cout << "u2 * v2 =" << endl <<  t2 << endl << endl;

  timer.start();
  MatrixXd t3 = t1*t2;
  timer.stop();

  cout << "resultado:" << endl << t3 << endl;
  cout << "tempo = " << timer.read_us() << " us" << endl;
}
