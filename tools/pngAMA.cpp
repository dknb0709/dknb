#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

int main (int argc, char** argv) {

  if (argc != 2) {
    std::cout << "[usage] pngAMA.exe <path to your file>" << std::endl;
    return 1;
  }

  std::ifstream fin(argv[1], std::ios::binary);
  if (!fin.is_open()) {
    std::cout << "[error] failed to open file" << std::endl;
    return 1;
  }

  char buf[256];
  fin.read(buf, 256);

  int header[8] = {137,  80,  78,  71,  13,  10,  26,  10};
  for (int i = 0; i < 8; ++i) {
    if (header[i] != (int)(0xFF & buf[i])) {
      std::cout << "[error] specified file is not a png" << std::endl;
      return 1;
    }
  }

  // gAMA, PLTE | IDAT
  std::string sbuf = "";
  for (int i = 0; i < 256; ++i) {
    unsigned char ch = static_cast<unsigned char>(buf[i]);
    if (32 < (int)ch && (int)ch < 127) {
      sbuf += (unsigned char)ch;
    } else {
      sbuf += '.';
    }
  }

  int found = sbuf.find("gAMA");
  if (found == std::string::npos) {
    return 0;
    std::cout << "gamma not been set" << std::endl;
  }

  int accumulated = 0;
  for (int i = 0; i < 4; ++i) {
    accumulated += static_cast<int>(0xFF & buf[found + i + 4]) << ((3 - i) * 8);
  }

  std::cout << (float) 100000.0 / accumulated << std::endl;

}
