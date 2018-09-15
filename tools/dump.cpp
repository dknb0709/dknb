#include <fstream>
#include <iostream>
#include <iomanip>

int main (int argc, char** argv) {

  if (argc != 2) {
    std::cout << "[usage] dump.exe <path to your file>" << std::endl;
  }

  std::ifstream fin(argv[1], std::ios::binary);
  if (!fin.is_open()) {
    std::cout << "[Error] failed to open file" << std::endl;
    return 1;
  }

  // 出力共通設定
  std::cout << std::hex << std::setfill('0') << std::uppercase;

  unsigned int line = 0u;
  char buf[16];
  fin.read(buf, 16);

  while (!fin.eof()) {
    for (int i = 0; i < 16; ++i) {
      unsigned char byte = static_cast<unsigned char>(buf[i]);
      // 行番号を表示する
      if (i == 0) std::cout << std::setw(8) << line++ << "  ";
      // 先頭と8バイト目には半角スペース
      if (i == 8) std::cout << ' ';
      // 1バイト分を半角スペース区切りで16進数表示(00 -> FF)
      std::cout << std::setw(2) << static_cast<int>(byte) << ' ';
    }

    std::cout << ' ';

    // 内容を解析して表示
    for (int i = 0; i < 16; ++i) {
      unsigned char ch = static_cast<unsigned char>(buf[i]);

      if (32 < ch && ch < 127) {
        // basic ascii codes
        std::cout << ch;
      } else {
        std::cout << '.';
      }
    }

    std::cout << std::endl;
    fin.read(buf, 16);

  }
}
