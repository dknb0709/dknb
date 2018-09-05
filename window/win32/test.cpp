#include "util.hpp"
#include "window_base.hpp"
#include <string>
#include <iostream>

using namespace dknb::graphics;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  const int width = 640, height = 400;
  std::string caption = "test window";
  {
    window<width, height> w(caption);
    w.start(hInstance);
  }
}
