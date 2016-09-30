#include <easylogging++.h>
#include <sstream>
#include "engine.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE


int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  
  LOG(INFO)<<"Number of args: "<<argc;
  std::string config_path;
  if(argc > 2) {
    config_path = std::string(argv[1]);
  }
  else {
    config_path = "./config/default.json";
  }

  std::shared_ptr<Engine> engine = std::make_shared<Engine>();

  engine->setup(config_path);
  engine->mainLoop();
  engine->cleanUp();
  return 0;
}
