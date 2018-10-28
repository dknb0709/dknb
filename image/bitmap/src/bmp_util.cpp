#include "bmp_util.hpp"

class BmpUtil::BmpHeader {
private:
  /** FileHeader */
  ushort bfType = 19778;    // 0-2
  uint bfSize;        // 2-6
  ushort bfReserved1 = 0;    // 6-8
  ushort bfReserved2 = 0;    // 8-10
  uint bfOffBits = 54;    // 10-14

  /** InfoHeader */
  uint biSize = 40;      // 14-18
  uint biWidth;        // 18-22
  uint biHeight;        // 22-26
  ushort biPlanes = 0;    // 26-28
  ushort biBitCount = 24;    // 28-30
  uint biCompression = 0;    // 30-34
  uint biImageSize;      // 34-38
  uint biXPixPerMeter = 0;  // 38-42
  uint biYPixPerMeter = 0;  // 42-44
  uint biColorUsed = 0;    // 46-50
  uint biColorImportant = 0;  // 50-54

public:
  static const uint BMP_HEADER_SIZE = 54;

public:
  void SetBfSize(uint bfSize) { this->bfSize = bfSize; }
  uint GetBfSize() { return this->bfSize; }
      
  void SetBiWidth(uint biWidth) { this->biWidth = biWidth; }
  uint GetBiWidth() { return this->biWidth; }

  void SetBiHeight(uint biHeight) { this->biHeight = biHeight; }
  uint GetBiHeight() { return this->biHeight; }

  void SetBiImageSize(uint biImageSize) { this->biImageSize = biImageSize; }
  uint GetBiImageSize() { return this->biImageSize; }

public:
  BmpHeader();
  BmpHeader(uint w, uint h);
  ~BmpHeader();

public:
  void GenerateHeaderBuf(char (&headerBuf)[BMP_HEADER_SIZE]);
  void ParseHeaderBuf(char (&headerBuf)[BMP_HEADER_SIZE]);
};

BmpUtil::BmpHeader::BmpHeader() {}

BmpUtil::BmpHeader::BmpHeader(uint w, uint h) {
  uint imageSize = (w * 3 + w % 4) * h;
  SetBiWidth(w);
  SetBiHeight(h);
  SetBiImageSize(imageSize);
  SetBfSize(imageSize + BMP_HEADER_SIZE);
}

BmpUtil::BmpHeader::~BmpHeader() {}

void BmpUtil::BmpHeader::GenerateHeaderBuf(char (&headerBuf)[BMP_HEADER_SIZE]) {
  memcpy(headerBuf + 0, &bfType, sizeof(bfType));
  memcpy(headerBuf + 2, &bfSize, sizeof(bfSize));
  memcpy(headerBuf + 6, &bfReserved1, sizeof(bfReserved1));
  memcpy(headerBuf + 8, &bfReserved2, sizeof(bfReserved2));
  memcpy(headerBuf + 10, &bfOffBits, sizeof(bfOffBits));
  memcpy(headerBuf + 14, &biSize, sizeof(biSize));
  memcpy(headerBuf + 18, &biWidth, sizeof(biWidth));
  memcpy(headerBuf + 22, &biHeight, sizeof(biHeight));
  memcpy(headerBuf + 26, &biPlanes, sizeof(biPlanes));
  memcpy(headerBuf + 28, &biBitCount, sizeof(biBitCount));
  memcpy(headerBuf + 30, &biCompression, sizeof(biCompression));
  memcpy(headerBuf + 34, &biImageSize, sizeof(biImageSize));
  memcpy(headerBuf + 38, &biXPixPerMeter, sizeof(biXPixPerMeter));
  memcpy(headerBuf + 42, &biYPixPerMeter, sizeof(biYPixPerMeter));
  memcpy(headerBuf + 46, &biColorUsed, sizeof(biColorUsed));
  memcpy(headerBuf + 50, &biColorImportant, sizeof(biColorImportant));
}

void BmpUtil::BmpHeader::ParseHeaderBuf(char (&headerBuf)[BMP_HEADER_SIZE]) {
  memcpy(&bfType, headerBuf + 0, sizeof(bfType));
  memcpy(&bfSize, headerBuf + 2, sizeof(bfSize));
  memcpy(&bfReserved1, headerBuf + 6, sizeof(bfReserved1));
  memcpy(&bfReserved2, headerBuf + 8, sizeof(bfReserved2));
  memcpy(&bfOffBits, headerBuf + 10, sizeof(bfOffBits));
  memcpy(&biSize, headerBuf + 14, sizeof(biSize));
  memcpy(&biWidth, headerBuf + 18, sizeof(biWidth));
  memcpy(&biHeight, headerBuf + 22, sizeof(biHeight));
  memcpy(&biPlanes, headerBuf + 26, sizeof(biPlanes));
  memcpy(&biBitCount, headerBuf + 28, sizeof(biBitCount));
  memcpy(&biCompression, headerBuf + 30, sizeof(biCompression));
  memcpy(&biImageSize, headerBuf + 34, sizeof(biImageSize));
  memcpy(&biXPixPerMeter, headerBuf + 38, sizeof(biXPixPerMeter));
  memcpy(&biYPixPerMeter, headerBuf + 42, sizeof(biYPixPerMeter));
  memcpy(&biColorUsed, headerBuf + 46, sizeof(biColorUsed));
  memcpy(&biColorImportant, headerBuf + 50, sizeof(biColorImportant));
}

BmpUtil::BmpUtil() : bmpHeader(new BmpHeader()) {
  m_imageData = nullptr;
}

BmpUtil::BmpUtil(uint width, uint height) : bmpHeader(new BmpHeader(width, height)) {
  uint w = bmpHeader->GetBiWidth();
  uint h = bmpHeader->GetBiHeight();
  
  m_imageData = new RGB[w * h];
}

BmpUtil::~BmpUtil() {
  if (m_imageData != nullptr) {
    delete[] m_imageData;
    m_imageData = nullptr;
  }
}

void BmpUtil::Load(const std::string &fname) {

  if (m_imageData != nullptr) {
    std::cout << "invalid call for Load(fname)" << std::endl;
    return;
  }

  std::ifstream fin(fname, std::ios::binary);
  if (!fin.is_open()) {
    std::cout << "[Error] failed to open file" << std::endl;
    return;
  }
  
  char headerBuf[BmpHeader::BMP_HEADER_SIZE];
  fin.read(headerBuf, sizeof(char) * BmpHeader::BMP_HEADER_SIZE);

  bmpHeader->ParseHeaderBuf(headerBuf);

  uint w = bmpHeader->GetBiWidth();
  uint h = bmpHeader->GetBiHeight();
  uint padding_width = w * 3 + w % 4;

  if (m_imageData != nullptr) {
    delete[] m_imageData;
  }
  m_imageData = new RGB[w * h];

  char line[padding_width];
  for (uint i = 0; i < h; ++i) {
    fin.read(line, padding_width);
    for (uint j = 0; j < w; ++j) {
      m_imageData[(h - i - 1) * w + j].b = line[j * 3 + 0];
      m_imageData[(h - i - 1) * w + j].g = line[j * 3 + 1];
      m_imageData[(h - i - 1) * w + j].r = line[j * 3 + 2];
    }
  }
}

void BmpUtil::Write(const std::string &fname) {

  std::ofstream fout(fname, std::ios::binary | std::ios::trunc);
  if (!fout.is_open()) {
    return;
  }
  char headerBuf[BmpHeader::BMP_HEADER_SIZE];
  bmpHeader->GenerateHeaderBuf(headerBuf);
  fout.write(headerBuf, BmpHeader::BMP_HEADER_SIZE);

  uint w = bmpHeader->GetBiWidth();
  uint h = bmpHeader->GetBiHeight();
  uint padding_width = w * 3 + w % 4;

  char line[padding_width];
  for (uint i = 0; i < h; ++i) {
    for (uint j = 0; j < w; ++j) {
      RGB rgb = m_imageData[(h - i - 1) * w + j];

      line[j * 3 + 0] = rgb.b;
      line[j * 3 + 1] = rgb.g;
      line[j * 3 + 2] = rgb.r;
    }

    for (uint j = w * 3; j < padding_width; ++j) {
      line[j] = 0;
    }
    fout.write(line, padding_width);
  }
  fout.close();
  std::cout << "file created: " << fname << std::endl;
}

BmpUtil::RGB &BmpUtil::GetPixel(uint x, uint y) {
  uint w = bmpHeader->GetBiWidth();
  uint h = bmpHeader->GetBiHeight();
  return m_imageData[(h - y - 1) * w + x];
}

void BmpUtil::SetPixel(uint x, uint y, const RGB &color) {
  uint w = bmpHeader->GetBiWidth();
  uint h = bmpHeader->GetBiHeight();
  m_imageData[(h - y - 1) * w + x] = color;
}

uint BmpUtil::GetImageWidth() {
  return bmpHeader->GetBiWidth();
}

uint BmpUtil::GetImageHeight() {
  return bmpHeader->GetBiHeight();
}

