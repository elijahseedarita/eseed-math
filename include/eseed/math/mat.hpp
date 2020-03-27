// Copyright (c) 2020 Elijah Seed Arita
//
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal 
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.

#pragma once

#include "vec.hpp"
#include "concepts.hpp"

#include <ostream>
#include <iostream>
#include <array>

namespace esdm {

template <std::size_t M, std::size_t N, typename T>
class MatData
{
public:
    std::array<Vec<N, T>, M> data;
};

// Forward declaration for shorthand aliases
template <std::size_t M, std::size_t N, typename T>
class Mat;

// Shorthand aliases

template <typename T>
using Mat1x1 = Mat<1, 1, T>;

template <typename T>
using Mat1x2 = Mat<1, 2, T>;

template <typename T>
using Mat1x3 = Mat<1, 3, T>;

template <typename T>
using Mat1x4 = Mat<1, 4, T>;

template <typename T>
using Mat2x1 = Mat<2, 1, T>;

template <typename T>
using Mat2x2 = Mat<2, 2, T>;

template <typename T>
using Mat2x3 = Mat<2, 3, T>;

template <typename T>
using Mat2x4 = Mat<2, 4, T>;

template <typename T>
using Mat3x1 = Mat<3, 1, T>;

template <typename T>
using Mat3x2 = Mat<3, 2, T>;

template <typename T>
using Mat3x3 = Mat<3, 3, T>;

template <typename T>
using Mat3x4 = Mat<3, 4, T>;

template <typename T>
using Mat4x1 = Mat<4, 1, T>;

template <typename T>
using Mat4x2 = Mat<4, 2, T>;

template <typename T>
using Mat4x3 = Mat<4, 3, T>;

template <typename T>
using Mat4x4 = Mat<4, 4, T>;

template <typename T>
using Mat1 = Mat1x1<T>;

template <typename T>
using Mat2 = Mat2x2<T>;

template <typename T>
using Mat3 = Mat3x3<T>;

template <typename T>
using Mat4 = Mat4x4<T>;

// Matrices are stored COLUMN-MAJOR
template <std::size_t M, std::size_t N, typename T>
class Mat : public MatData<M, N, T> {
public:
    using Col = Vec<M, T>;
    using Row = Vec<N, T>;

    // Mat<2, 2, T>() =>
    // | 0, 0 |
    // | 0, 0 |
    constexpr Mat() {}

    // Repeated single element
    // Mat<2, 2, T>(v) =>
    // | v, 0 |
    // | 0, v |
    constexpr explicit Mat(T component) {
        for (std::size_t i = 0; i < (M > N ? M : N); i++)
            data[i][i] = component;
    }

    // Multi element
    // Mat<2, 2, T>(a, b, c, d) =>
    // | a, c |
    // | b, d |
    template <ConvertibleTo<T>... Ts> requires (sizeof...(Ts) == M * N)
    constexpr Mat(const Ts &... components) {
        T expanded[M * N] = {(T)components...};
        for (std::size_t i = 0; i < M; i++)
            for (std::size_t j = 0; j < N; j++) 
                data[i][j] = expanded[i * M + j];
    }

    // Type / size conversion (explicit)
    // If length is smaller, trailing elements are cut
    // If length is larger, additional elements are default initialized
    template <ConvertibleTo<T> T1, std::size_t M1, std::size_t N1>
    constexpr explicit Mat(const Mat<M1, N1, T1>& other) {
        for (std::size_t i = 0; i < std::min(M, M1); i++) 
            data[i] = Col(other[i]);
    }

    // Type conversion only (implicit)
    template <ConvertibleTo<T> T1>
    constexpr explicit Mat(const Mat<M, N, T1>& other) {
        for (std::size_t i = 0; i < M; i++)
            data[i] = Col(other[i]);
    }

    constexpr Col getCol(std::size_t j) const {
        Col col;
        for (std::size_t i = 0; i < M; i++)
            col[i] = data[i][j];
        return col;
    }

    constexpr Row getRow(std::size_t i) const {
        Row row;
        for (std::size_t j = 0; j < N; j++)
            row[j] = data[i][j];
        return row;
    }

    constexpr const Col &operator[](std::size_t i) const {
        return data[i];
    }

    constexpr Col &operator[](std::size_t i) {
        return data[i];
    }

    constexpr std::string toString() const {
        std::string out = "[";
        for (std::size_t i = 0; i < M; i++) {
            out += data[i].toString();
            if (i < M - 1) out += ", ";
        }
        out += "]";
        return out;
    }

    friend std::ostream &operator<<(std::ostream &out, const Mat &m) {
        out << m.toString();
        return out;
    }
};

// Operators

template <std::size_t M, std::size_t N, typename T0, typename T1> 
constexpr bool operator==(const Mat<M, N, T0>& a, const Mat<M, N, T1>& b) { 
    for (std::size_t i = 0; i < M; i++) if (a[i] != b[i]) return false; 
    return true; 
}

#define ESEED_MAT_PRE(op)                        \
    template <std::size_t M, std::size_t N, typename T>    \
    constexpr Mat<M, N, T> &operator op(Mat<M, N, T> &m) { \
        for (std::size_t i = 0; i < M; i++) op m[i];                             \
        return m;                                \
    }

#define ESEED_MAT_POST(op)                           \
    template <std::size_t M, std::size_t N, typename T>        \
    constexpr Mat<M, N, T> operator op(Mat<M, N, T> &m, int) { \
        Mat<M, N, T> out = m;                        \
        for (std::size_t i = 0; i < M; i++) m[i] op;                                 \
        return out;                                  \
    }

#define ESEED_MAT_UN(op)                              \
    template <std::size_t M, std::size_t N, typename T>         \
    constexpr Mat<M, N, T> operator op(const Mat<M, N, T> &m) { \
        Mat<M, N, T> out;                             \
        for (std::size_t i = 0; i < M; i++) out[i] = op m[i];                                  \
        return out;                                   \
    }

#define ESEED_MAT_BIN_MM(op)                                                                          \
    template <std::size_t M, std::size_t N, typename T0, typename T1>                                           \
    constexpr Mat<M, N, decltype(T0(0) op T1(0))> operator op(const Mat<M, N, T0> &a, const Mat<M, N, T1> &b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                                      \
        for (std::size_t i = 0; i < M; i++) out[i] = a[i] op b[i];                                                                    \
        return out;                                                                                   \
    }

#define ESEED_MAT_BIN_MS(op)                                                        \
    template <std::size_t M, std::size_t N, typename T0, typename T1>                         \
    constexpr Mat<M, N, decltype(T0(0) op T1(0))> operator op(const Mat<M, N, T0> &a, T1 b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                    \
        for (std::size_t i = 0; i < M; i++) out[i] = a[i] op b;                                                     \
        return out;                                                                 \
    }

#define ESEED_MAT_BIN_SM(op)                                                               \
    template <std::size_t M, std::size_t N, typename T0, typename T1>                                \
    constexpr Mat<M, N, decltype(T0(0) op T1(0))> operator op(const T0 &a, const Mat<M, N, T1> &b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                           \
        for (std::size_t i = 0; i < M; i++) out[i] = a op b[i];                                                            \
        return out;                                                                        \
    }

#define ESEED_MAT_ASSN_MM(op)                                                                    \
    template <std::size_t M, std::size_t N, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Mat<M, N, T0>& operator op##=(Mat<M, N, T0>& a, const Mat<M, N, T1>& b) {                    \
        for (std::size_t i = 0; i < M; i++) a[i] op##= b[i];                                                                        \
        return a;                                                                                \
    }

#define ESEED_MAT_ASSN_MS(op)                                                                    \
    template <std::size_t M, std::size_t N, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Mat<M, N, T0>& operator op##=(Mat<M, N, T0>& a, T1 b) {                                      \
        for (std::size_t i = 0; i < M; i++) a[i] op##= b;                                             \
        return a;                                                                                \
    }

ESEED_MAT_PRE(++)
ESEED_MAT_PRE(--)

ESEED_MAT_POST(--)
ESEED_MAT_POST(++)

ESEED_MAT_UN(!)
ESEED_MAT_UN(~)
ESEED_MAT_UN(-)
ESEED_MAT_UN(+)

ESEED_MAT_BIN_MM(+)
ESEED_MAT_BIN_MM(-)
ESEED_MAT_BIN_MM(*)
ESEED_MAT_BIN_MM(/)
ESEED_MAT_BIN_MM(%)
ESEED_MAT_BIN_MM(&)
ESEED_MAT_BIN_MM(|)
ESEED_MAT_BIN_MM(^)
ESEED_MAT_BIN_MM(<<)
ESEED_MAT_BIN_MM(>>)
ESEED_MAT_BIN_MM(&&)
ESEED_MAT_BIN_MM(||)

ESEED_MAT_BIN_MS(+)
ESEED_MAT_BIN_MS(-)
ESEED_MAT_BIN_MS(*)
ESEED_MAT_BIN_MS(/)
ESEED_MAT_BIN_MS(%)
ESEED_MAT_BIN_MS(&)
ESEED_MAT_BIN_MS(|)
ESEED_MAT_BIN_MS(^)
ESEED_MAT_BIN_MS(<<)
ESEED_MAT_BIN_MS(>>)
ESEED_MAT_BIN_MS(&&)
ESEED_MAT_BIN_MS(||)

ESEED_MAT_BIN_SM(+)
ESEED_MAT_BIN_SM(-)
ESEED_MAT_BIN_SM(*)
ESEED_MAT_BIN_SM(/)
ESEED_MAT_BIN_SM(%)
ESEED_MAT_BIN_SM(&)
ESEED_MAT_BIN_SM(|)
ESEED_MAT_BIN_SM(^)
ESEED_MAT_BIN_SM(<<)
ESEED_MAT_BIN_SM(>>)
ESEED_MAT_BIN_SM(&&)
ESEED_MAT_BIN_SM(||)

ESEED_MAT_ASSN_MM(+)
ESEED_MAT_ASSN_MM(-)
ESEED_MAT_ASSN_MM(*)
ESEED_MAT_ASSN_MM(/)
ESEED_MAT_ASSN_MM(%)
ESEED_MAT_ASSN_MM(&)
ESEED_MAT_ASSN_MM(|)
ESEED_MAT_ASSN_MM(^)
ESEED_MAT_ASSN_MM(<<)
ESEED_MAT_ASSN_MM(>>)

ESEED_MAT_ASSN_MS(+)
ESEED_MAT_ASSN_MS(-)
ESEED_MAT_ASSN_MS(*)
ESEED_MAT_ASSN_MS(/)
ESEED_MAT_ASSN_MS(%)
ESEED_MAT_ASSN_MS(&)
ESEED_MAT_ASSN_MS(|)
ESEED_MAT_ASSN_MS(^)
ESEED_MAT_ASSN_MS(<<)
ESEED_MAT_ASSN_MS(>>)

}