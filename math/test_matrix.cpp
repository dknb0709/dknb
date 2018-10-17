#include <iostream>
#include "matrix.hpp"

template <int N>
void p(float mat[N][N]) {
  for (int row = 0; row < N; ++row) {
    for (int col = 0; col < N; ++col) {
      if (col) std::cout << ", ";
      std::cout << mat[row][col];
    }
    std::cout << std::endl;
  }
}

int main () {
  dknb::vec3<float> v1 = { 1.0f, 2.0f, 3.0f };
  dknb::vec3<float> v2 = { 3.0f, 3.0f, 3.0f };

  std::cout << v1.length() << std::endl;
  std::cout << v1.dot_product(v2) << std::endl;

  v1.normalize();
  std::cout << v1.length() << std::endl;

  dknb::mat44 m1 = {
    1.0f, 2.0f, 0.0f, 1.0f,
    3.0f, 2.0f, 0.0f, 1.0f,
    8.0f, 1.0f, 2.0f, 2.0f,
    2.0f, 4.0f, 0.0f, 1.0f
  };

  std::cout << "=========" << std::endl;
  std::cout << "print original:" << std::endl;
  p<4>(m1.p);

  std::cout << "=========" << std::endl;
  std::cout << "make_cofactor" << std::endl;
  float cofactor[3][3];
  m1.make_cofactor<4>(cofactor, 0, 0);
  p<3>(cofactor);

  std::cout << "=========" << std::endl;
  std::cout << "calc determinant:" << std::endl;
  std::cout << m1.det() << std::endl;

  std::cout << "=========" << std::endl;
  std::cout << "invert()" << std::endl;
  dknb::mat44 m2 = m1;
  m2.invert();
  p<4>(m2.p);

  std::cout << "=========" << std::endl;
  std::cout << "A * (inv A) should be E" << std::endl;
  dknb::mat44 m3 = m1 * m2;
  p<4>(m3.p);

}
