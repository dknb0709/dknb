#ifndef _DKNB_MATH_MATRIX_HPP_
#define _DKNB_MATH_MATRIX_HPP_

#include <cmath>
#include <vector>

namespace dknb {

template <typename T>
struct vec3 {
  using value_type = T;
  value_type vx, vy, vz;
  vec3() = default;
  vec3<value_type> operator+(const vec3<value_type>& rhs) {
    return { vx + rhs.vx, vy + rhs.vy, vz + rhs.vz };
  }
  vec3<value_type> operator-(const vec3<value_type>& rhs) {
    return { vx - rhs.vx, vy - rhs.vy, vz - rhs.vz };
  }
  vec3<value_type> operator*(value_type scale) {
    return { vx * scale, vy * scale, vz * scale };
  }
  vec3<value_type> operator/(value_type scale) {
    return { vx / scale, vy / scale, vz / scale };
  }
  value_type dot_product(const vec3<value_type>& o) {
    return vx * o.vx + vy * o.vy + vz * o.vz;
  }
  vec3<value_type> cross_product(const vec3<value_type>& o) {
    return {
      vy * o.vz - vz * o.vy,
      vz * o.vx - vx * o.vz,
      vx * o.vy - vy * o.vx
    };
  }
  value_type length() {
    return sqrt(dot_product(*this));
  }
  vec3<value_type> normalize() {
    value_type l = this->length();
    (*this) = (*this) / l;
    return (*this) / l;
  }
};

template <typename T>
struct triangle {
  using vec3_t = vec3<T>;
  vec3_t p[3];
  vec3_t norm() {
    vec3_t v1 = p[2] - p[0], v2 = p[1] - p[0];
    return v1.cross_product(v2).normalize();
  }
};

template <typename T>
struct mesh {
  std::vector<triangle<T>> triangles;
};

struct mat44 {

  float p[4][4];

  mat44() = default;
  mat44(const mat44 &rhs) = default;

  mat44 operator+(const mat44& rhs) {
    return {
      p[0][0] + rhs.p[0][0], p[0][1] + rhs.p[0][1], p[0][2] + rhs.p[0][2], p[0][3] + rhs.p[0][3],
      p[1][0] + rhs.p[1][0], p[1][1] + rhs.p[1][1], p[1][2] + rhs.p[1][2], p[1][3] + rhs.p[1][3],
      p[2][0] + rhs.p[2][0], p[2][1] + rhs.p[2][1], p[2][2] + rhs.p[2][2], p[2][3] + rhs.p[2][3],
      p[3][0] + rhs.p[3][0], p[3][1] + rhs.p[3][1], p[3][2] + rhs.p[3][2], p[3][3] + rhs.p[3][3]
    };
  }
  mat44 operator-(const mat44& rhs) {
    return {
      p[0][0] - rhs.p[0][0], p[0][1] - rhs.p[0][1], p[0][2] - rhs.p[0][2], p[0][3] - rhs.p[0][3],
      p[1][0] - rhs.p[1][0], p[1][1] - rhs.p[1][1], p[1][2] - rhs.p[1][2], p[1][3] - rhs.p[1][3],
      p[2][0] - rhs.p[2][0], p[2][1] - rhs.p[2][1], p[2][2] - rhs.p[2][2], p[2][3] - rhs.p[2][3],
      p[3][0] - rhs.p[3][0], p[3][1] - rhs.p[3][1], p[3][2] - rhs.p[3][2], p[3][3] - rhs.p[3][3]
    };
  }
  mat44 operator*(const mat44& rhs) {
    mat44 ret = { 0.0f };
    for (int row = 0; row < 4; ++row) {
      for (int col = 0; col < 4; ++col) {
        for (int i = 0; i < 4; ++i) {
          ret.p[row][col] += p[row][i] * rhs.p[i][col];
        }
      }
    }
    return ret;
  }

  mat44 operator*(float scale) {
    return {
      p[0][0] * scale, p[0][1] * scale, p[0][2] * scale, p[0][3] * scale,
      p[1][0] * scale, p[1][1] * scale, p[1][2] * scale, p[1][3] * scale,
      p[2][0] * scale, p[2][1] * scale, p[2][2] * scale, p[2][3] * scale,
      p[3][0] * scale, p[3][1] * scale, p[3][2] * scale, p[3][3] * scale
    };
  }

  mat44 operator/(float scale) {
    return {
      p[0][0] / scale, p[0][1] / scale, p[0][2] / scale, p[0][3] / scale,
      p[1][0] / scale, p[1][1] / scale, p[1][2] / scale, p[1][3] / scale,
      p[2][0] / scale, p[2][1] / scale, p[2][2] / scale, p[2][3] / scale,
      p[3][0] / scale, p[3][1] / scale, p[3][2] / scale, p[3][3] / scale
    };
  }

  template <int N>
  constexpr float sarrus(float mat[N][N]) {
    float ret = 0.0f;
    for (int col = 0; col < N; ++col) {
      float plus = 1.0f, minus = 1.0f;
      for (int row = 0; row < N; ++row) {
        plus *= mat[row][(col + row) % N];
        minus *= mat[row][(col - row + N) % N];
      }
      ret += (plus - minus);
    }
    return ret;
  }

  template <int N>
  void make_cofactor(float mat_out[N - 1][N - 1], int row, int col) {
    for (int r = 0; r < N; ++r) {
      for (int c = 0; c < N; ++c) {
        if (r == row || c == col) continue;
        mat_out[r > row ? r - 1 : r][c > col ? c - 1 : c] = p[r][c];
      }
    }
  }

  template <int N>
  constexpr float det(float mat[N][N]) {
    float ret = 0.0f;
    if constexpr (N > 3) {
      float cofactor[N - 1][N - 1] = {};
      for (int i = 0; i < N; ++i) {
        make_cofactor<N>(cofactor, 0, i);
        float coef = (i % 2 == 0) ? mat[0][i] : -mat[0][i];
        ret += coef * det<N - 1>(cofactor);
      }
      return ret;
    } else {
      return sarrus<N>(mat);
    }
  }

  float det() {
    return det<4>(p);
  }

  void invert() {
    mat44 tmp = *this;
    for (int row = 0; row < 4; ++row) {
      for (int col = 0; col < 4; ++col) {
        float cofactor[3][3];
        tmp.make_cofactor<4>(cofactor, row, col);
        p[col][row] = (row + col) % 2 == 0 ? det<3>(cofactor) : -det<3>(cofactor);
      }
    }
    *this = *this / tmp.det();
  }
};

} // namespace dknb

#endif // _DKNB_MATH_MATRIX_HPP_
