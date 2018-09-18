#include "mbed.h"
#include "config.h"
#include "DataController.h"
#include "init.h"

Serial pc(USBTX, USBRX, 115200); // Usando minicom, o maximo é 115200

int main() {
  init();
  while (true) { Thread::wait(5000); }
}
