#include <easylogging++.h>
#include <sstream>
#include <iostream>
#include "engine.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE


int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);

  LOG(INFO)<<"Number of args: "<<argc;
  std::string config_path;
  if(argc > 2) {
    if(argv[1] == "-h" || argv[1] == "--help") {
      std::cout<<"Nymph Game Engine USAGE"<<std::endl;
      std::cout<<"Running with no arguments: will attempt to load engine config from nymph_config."<<std::endl;
      std::cout<<"NymphGameEngine ./path/to/config.json: attempts to load engine config from supplied path."<<std::endl;
      std::cout<<"NymphGameEngine -h (or) --help: supplies this help menu."<<std::endl;
      return 0;
    }
    else {
      config_path = std::string(argv[1]);
    }
  }
  else {
    config_path = "./nymph_config.json";
  }

  std::shared_ptr<Engine> engine = std::make_shared<Engine>();

  engine->setup(config_path);
  engine->mainLoop();
  engine->cleanUp();
  return 0;
}
