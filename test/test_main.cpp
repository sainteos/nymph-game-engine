#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

int main( int argc, char** argv)
{
  START_EASYLOGGINGPP(argc, argv);
  int result = Catch::Session().run( argc, argv );

  return result;
}