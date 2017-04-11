#include <iostream>
#include "application/application.h"
#include "config.h"

using namespace std;

int main(int argc, const char* argv[]) {

  application* app = new application();
  APPLICATION =app;
  app->Init();
  app->Run();

  return 0;
}
