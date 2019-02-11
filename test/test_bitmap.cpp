#include "bmp_util.hpp"

int main() {

  { // create copy of original image
    BmpUtil bmp;
    bmp.Load("./image/test.bmp");
    bmp.SetPixel(0, 0, BmpUtil::RGB(255, 0, 0));
    bmp.SetPixel(bmp.GetImageWidth() - 1, 0, BmpUtil::RGB(0, 255, 0));
    bmp.SetPixel(bmp.GetImageWidth() - 1, bmp.GetImageHeight() - 1, BmpUtil::RGB(0, 0, 255));
    bmp.SetPixel(0, bmp.GetImageHeight() - 1, BmpUtil::RGB(255, 255, 0));
    bmp.Write("./image/copy_test.bmp");
  }

  { // create new image
    BmpUtil bmp(2, 2);
    bmp.SetPixel(0, 0, BmpUtil::RGB(255, 0, 0));
    bmp.SetPixel(0, 1, BmpUtil::RGB(0, 255, 0));
    bmp.SetPixel(1, 0, BmpUtil::RGB(0, 0, 255));
    bmp.SetPixel(1, 1, BmpUtil::RGB(255, 0, 255));
    bmp.Write("./image/new_image.bmp");
  }

  { // invalid usage
    BmpUtil bmp(2, 2);
    bmp.Load("./image/test.bmp");
  }
}


