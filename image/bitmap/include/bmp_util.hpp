#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;

class BmpUtil {
public:
  struct RGB {
    uint r, g, b;
    RGB() : r(0), g(0), b(0) {}
    RGB(uint _r, uint _g, uint _b) : r(_r), g(_g), b(_b) {}
    ~RGB() {}
    void operator=(const RGB &other) {
      this->r = other.r;
      this->g = other.g;
      this->b = other.b;
    }
  };
private:
  class BmpHeader;
  std::unique_ptr<BmpHeader> bmpHeader;
  RGB *m_imageData;

public:
  BmpUtil();
  BmpUtil(uint width, uint height);
  ~BmpUtil();
  void Load(const std::string &fname);
  void Write(const std::string &fname); 
  RGB &GetPixel(uint x, uint y);
  void SetPixel(uint x, uint y, const RGB &color);
  uint GetImageWidth();
  uint GetImageHeight();
};


