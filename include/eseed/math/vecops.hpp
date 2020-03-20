#include "vec.hpp"

namespace esdm {

// -- SPECIAL FLOATING POINT VALUES -- //

// Check if all components are infinity
template <size_t L, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool allinf(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (!isinf(v[i])) return false;
    return true;
}

// Check if any components are infinity
template <size_t L, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool anyinf(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (isinf(v[i])) return true;
    return false;
}

// Check if all components are NaN
template <size_t L, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool allnan(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (!isnan(v[i])) return false;
    return true;
}

// Check if any components are NaN
template <size_t L, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool anynan(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (isnan(v[i])) return true;
    return false;
}

// -- GENERAL FUNCTIONS -- //

// Absolute value all components
template <size_t L, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr Vec<L, T> abs(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = abs(v[i]);
    return out;
}

// Square all components
template <size_t L, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr Vec<L, T> sq(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sq(v[i]);
    return out;
}

// Square root all components
template <size_t L, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline Vec<L, T> sqrt(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sqrt(v[i]);
    return out;
}

// Raise components to a power
template <size_t L, typename T0, typename T1>
constexpr Vec<L, PowT<T0, T1>> pow(const Vec<L, T0>& b, T1 e) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sq(v[i]);
    return out;
}

// Dot product
template <size_t L, typename T0, typename T1>
constexpr decltype(T0(0) * T1(0)) dot(const Vec<L, T0>& a, const Vec<L, T1>& b) {
    decltype(T0(0) * T1(0)) out = 0;
    for (size_t i = 0; i < L; i++) out += a[i] * b[i];
    return out;
}

// Cross product
template <typename T0, typename T1>
constexpr Vec3<decltype(T0(0) * T1(0))> cross(const Vec3<T0>& a, const Vec3<T1>& b) {
    return Vec3<decltype(T0(0) * T1(0))>(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// -- ROUNDING -- //

// Truncate all components
template <size_t L, typename T>
inline Vec<L, T> trunc(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = trunc(v[i]);
    return out;
}

// Floor all components
template <size_t L, typename T>
inline Vec<L, T> floor(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = floor(v[i]);
    return out;
}

// Ceil all components
template <size_t L, typename T>
inline Vec<L, T> ceil(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = ceil(v[i]);
    return out;
}

// Round all components
template <size_t L, typename T>
inline Vec<L, T> round(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = round(v[i]);
    return out;
}

// See ops.hpp for "i" functions explanation

// Direct-to-int trunc all components
template <typename I, size_t L, typename T>
constexpr Vec<L, I> itrunc(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = itrunc<I>(v[i]);
    return out;
}

// Direct-to-int floor all components
template <typename I, size_t L, typename T>
constexpr Vec<L, I> ifloor(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = ifloor<I>(v[i]);
    return out;
}

// Direct-to-int ceil all components
template <typename I, size_t L, typename T>
constexpr Vec<L, I> iceil(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = iceil<I>(v[i]);
    return out;
}

// Direct-to-int round all components
template <typename I, size_t L, typename T>
constexpr Vec<L, I> iround(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = iround<I>(v[i]);
    return out;
}

}