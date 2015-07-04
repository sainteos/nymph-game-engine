#include <iostream>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  LOG(INFO)<<"Butts";
  std::cout<<"Hello world.\n";
  return 0;
}
