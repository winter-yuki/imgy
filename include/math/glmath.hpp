#ifndef INCLUDE_MATH_GLMATH_HPP
#define INCLUDE_MATH_GLMATH_HPP

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>


namespace GLMath
{

using SizeT = int64_t;

inline SizeT SIZE()     { static const SizeT x = 4; return x; }
inline SizeT N_COORDS() { static const SizeT x = 3; return x; }


template <class Type>
class Vector {
public:
    static const SizeT SIZE     = 4;
    static const SizeT N_COORDS = 3;

public:
    struct View { Type x, y, z; };

    Vector() = default;
    Vector(Type x1, Type x2, Type x3, Type x4 = 1);

    View view() const { return { data_[0], data_[1], data_[2] }; }
    Type norm() const;
    Vector const & normalize();

    template <class T2>
    auto dot(Vector<T2> const & v) const
    -> decltype (std::declval<Type>() * v[0]);

    template <class T2>
    auto cross(Vector<T2> const & v) const
    -> Vector<decltype (std::declval<Type>() * v[0])>;

    Type & operator[](SizeT pos);
    Type   operator[](SizeT pos) const;

    Vector & operator=(Type x);

private:
    Type data_[SIZE]{};
};


template <class Type>
class Matrix {
public:
    static const SizeT SIZE = 4;

public:
    Matrix();
    Matrix(Vector<Type> v1, Vector<Type> v2, Vector<Type> v3,
           Vector<Type> v4 = {0, 0, 0, 1});

    Type & operator()(SizeT row, SizeT col);
    Type   operator()(SizeT row, SizeT col) const;

    Matrix & translate(Vector<Type> const & r);
    Matrix & rotate(Vector<Type> const & r, Type teta);
    Matrix & view(Vector<Type> const & loc,
                  Vector<Type> const & at,
                  Vector<Type> const & up);
    Matrix & frustum(/* x */ Type l, Type r,
                     /* y */ Type b, Type t,
                     /* z */ Type n, Type f);

    Type * base() { return data_; }

private:
    static const SizeT REAL_SIZE = SIZE * SIZE;
    Type & data(SizeT row, SizeT col) { return (*this)(row, col); }
    Type data_[REAL_SIZE]{};
};



template <class Type>
Vector<Type>::Vector(Type x1, Type x2, Type x3, Type x4)
{
    data_[0] = x1;
    data_[1] = x2;
    data_[2] = x3;
    data_[3] = x4;
}

template <class Type>
Type Vector<Type>::norm() const
{
    Type rez = 0;
    for (SizeT i = 0; i < N_COORDS; ++i) {
        rez += i * i;
    }
    return std::sqrt(rez);
}

template <class Type>
Vector<Type> const & Vector<Type>::normalize()
{
    auto n = norm();
    for (SizeT i = 0; i < N_COORDS; ++i) {
        data_[i] /= n;
    }
    return *this;
}

template <class T1>
template <class T2>
auto Vector<T1>::dot(Vector<T2> const & v) const
-> decltype (std::declval<T1>() * v[0])
{
    using Type = decltype ((*this)[0] * v[0]);
    Type sum{};
    for (SizeT i = 0; i < N_COORDS; ++i) {
        sum += data_[i] * v[i];
    }
    return sum;
}

template <class T1>
template <class T2>
auto Vector<T1>::cross(Vector<T2> const & v) const
-> Vector<decltype (std::declval<T1>() * v[0])>
{
    auto q = v.view();
    Matrix<T2> m(
    { 0,    q.z, -q.y},
    {-q.z,  0,    q.x},
    { q.y, -q.x,  0});

    return *this * m;
}

template <class Type>
Type & Vector<Type>::operator[](SizeT pos)
{
    assert(pos >= 0);
    assert(pos < SIZE);
    return data_[pos];
}

template <class Type>
Type Vector<Type>::operator[](SizeT pos) const
{
    assert(pos >= 0);
    assert(pos < SIZE);
    return data_[pos];
}

template <class Type>
Matrix<Type>::Matrix()
{
    for (SizeT i = 0; i < SIZE; ++i) {
        data(i, i) = 1;
    }
}

template <class Type>
Matrix<Type>::Matrix(Vector<Type> v1, Vector<Type> v2,
                     Vector<Type> v3, Vector<Type> v4)
{
    SizeT matr_pos = 0;

    for (SizeT vec_pos = 0; vec_pos < SIZE; ++vec_pos, ++matr_pos) {
        data_[matr_pos] = v1[vec_pos];
    }
    for (SizeT vec_pos = 0; vec_pos < SIZE; ++vec_pos, ++matr_pos) {
        data_[matr_pos] = v2[vec_pos];
    }
    for (SizeT vec_pos = 0; vec_pos < SIZE; ++vec_pos, ++matr_pos) {
        data_[matr_pos] = v3[vec_pos];
    }
    for (SizeT vec_pos = 0; vec_pos < SIZE; ++vec_pos, ++matr_pos) {
        data_[matr_pos] = v4[vec_pos];
    }
    assert(matr_pos == SIZE * 4);
}

template <class Type>
Matrix<Type> & Matrix<Type>::translate(Vector<Type> const & r)
{
    auto v = r.view();
    Matrix tr(
    {1,    0,    0,    0},
    {0,    1,    0,    0},
    {0,    0,    1,    0},
    {v.x,  v.y,  v.z,  1});

    *this = *this * tr;
    return *this;
}

template <class Type>
Matrix<Type> & Matrix<Type>::rotate(Vector<Type> const & r0, Type teta)
{
    Vector<Type> r = Type(1.0 / r0.norm()) * r0;
    Type c = std::cos(teta);
    Type s = std::sin(teta);
    auto a = r.view();

    Type r11 = c + a.x*a.x*(1-c);
    Type r12 = a.x*a.y*(1-c) + a.z*s;
    Type r13 = a.x*a.z*(1-c) - a.y*s;

    Type r21 = a.y*a.x*(1-c) - a.z*s;
    Type r22 = c + a.y*a.y*(1-c);
    Type r23 = a.y*a.z*(1-c) + a.x*s;

    Type r31 = a.z*a.x*(1-c) + a.y*s;
    Type r32 = a.z*a.y*(1-c) - a.x*s;
    Type r33 = c + a.z*a.z*(1-c);

    Matrix<Type> rotate(
    {r11, r12, r13},
    {r21, r22, r23},
    {r31, r32, r33});

    *this = *this * rotate;
    return *this;
}

template <class Type>
Matrix<Type> & Matrix<Type>::view(Vector<Type> const & loc,
                                  Vector<Type> const & at,
                                  Vector<Type> const & up)
{
    auto dir = at - loc;
    auto right = dir.cross(up);

    auto r = right.view();
    auto u = up.view();
    auto d = dir.view();
    Matrix m(
    { r.x,             u.x,        -d.x,          0},
    { r.y,             u.y,        -d.y,          0},
    { r.z,             u.z,        -d.z,          0},
    {-loc.dot(right), -loc.dot(up), loc.dot(dir), 1});

    *this = *this * m;
    return *this;
}

template <class Type>
Matrix<Type> & Matrix<Type>::frustum(Type l, Type r, Type b, Type t,
                                     Type n, Type f)
{
    Matrix<Type> m(
    {2*n/(r-l),   0,            0,            0},
    {0,           2*n/(t-b),    0,            0},
    {(r+l)/(r-l), (t+b)/(t-b), -(f+n)/(f-n), -1},
    {0,           0,           -2*n*f/(f-n),  0});

    *this = *this * m;
    return *this;
}

template <class T1, class T2>
auto operator+(Vector<T1> const & a, Vector<T2> const & b)
-> Vector<decltype (a[0] + b[0])>
{
    using Type = decltype (a[0] + b[0]);
    Vector<Type> rez;
    for (SizeT i = 0; i < N_COORDS(); ++i) {
        rez[i] = a[i] + b[i];
    }
    return rez;
}

template <class T1, class T2>
auto operator-(Vector<T1> const & a, Vector<T2> const & b)
-> Vector<decltype (a[0] - b[0])>
{
    using Type = decltype (a[0] - b[0]);
    Vector<Type> rez;
    for (SizeT i = 0; i < N_COORDS(); ++i) {
        rez[i] = a[i] - b[i];
    }
    return rez;
}

template <class T1, class T2>
auto operator*(T1 x, Vector<T2> const & v)
-> Vector<decltype (x * v[0])>
{
    using Type = decltype (x * v[0]);
    Vector<Type> rez;
    for (SizeT i = 0; i < N_COORDS(); ++i) {
        rez[i] = x * v[i];
    }
    return rez;
}

template <class T1, class T2>
auto operator*(Vector<T1> const & v, T2 x)
-> Vector<decltype (v[0] * x)>
{
    return x * v;
}

template <class T1, class T2>
auto operator*(Vector<T1> const & a, Vector<T2> const & b)
-> decltype (a[0] * b[0])
{
    using Type = decltype (a[0] * b[0]);
    Type sum{};
    for (SizeT i = 0; i < N_COORDS(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}


template <class Type>
Type & Matrix<Type>::operator()(SizeT row, SizeT col)
{
    assert(row >= 0 && row < SIZE);
    assert(col >= 0 && col < SIZE);
    return data_[row * SIZE + col];
}

template <class Type>
Type Matrix<Type>::operator()(SizeT row, SizeT col) const
{
    assert(row >= 0 && row < SIZE);
    assert(col >= 0 && col < SIZE);
    return data_[row * SIZE + col];
}

template <class T1, class T2>
auto operator+(Matrix<T1> const & a, Matrix<T2> const & b)
-> Matrix<decltype (a(0, 0) + b(0, 0))>
{
    using Type = decltype (a(0, 0) + b(0, 0));
    Matrix<Type> rez;
    for (SizeT row = 0; row < SIZE(); ++row) {
        for (SizeT col = 0; col < SIZE(); ++col) {
            rez(row, col) = a(row, col) + b(row, col);
        }
    }
    return rez;
}

template <class T1, class T2>
auto operator-(Matrix<T1> const & a, Matrix<T2> const & b)
-> Matrix<decltype (a(0, 0) - b(0, 0))>
{
    using Type = decltype (a(0, 0) - b(0, 0));
    Matrix<Type> rez;
    for (SizeT row = 0; row < SIZE(); ++row) {
        for (SizeT col = 0; col < SIZE(); ++col) {
            rez(row, col) = a(row, col) - b(row, col);
        }
    }
    return rez;
}

template <class T1, class T2>
auto operator*(Matrix<T1> const & a, Matrix<T2> const & b)
-> Matrix<decltype (a(0, 0) * b(0, 0))>
{
    using Type = decltype (a(0, 0) * b(0, 0));
    Matrix<Type> rez;
    for (SizeT row = 0; row < SIZE(); ++row) {
        for (SizeT col = 0; col < SIZE(); ++col) {

            Type sum{};
            for (SizeT i = 0; i < SIZE(); ++i) {
                sum = sum + a(row, i) * b(i, col);
            }
            rez(row, col) = sum;
        }
    }
    return rez;
}

template <class T1, class T2>
auto operator*(Matrix<T1> const & m, Vector<T2> const & v)
-> Vector<decltype (m(0, 0) * v[0])>
{
    using Type = decltype (m(0, 0) * v[0]);
    Vector<Type> rez;
    for (SizeT row = 0; row < SIZE(); ++row) {
        Type sum{};
        for (SizeT i = 0; i < SIZE(); ++i) {
            sum += m(row, i) * v[i];
        }
        rez[row] = sum;
    }
    return rez;
}

template <class T1, class T2>
auto operator*(Vector<T1> const & v, Matrix<T2> const & m)
-> Vector<decltype (v[0] * m(0, 0))>
{
    using Type = decltype (v[0] * m(0, 0));
    Vector<Type> rez;
    for (SizeT col = 0; col < SIZE(); ++col) {
        Type sum{};
        for (SizeT i = 0; i < SIZE(); ++i) {
            sum += v[i] * m(i, col);
        }
        rez[col] = sum;
    }
    return rez;
}

template <class T1, class T2>
auto operator*(T1 x, Matrix<T2> const & m)
-> Matrix<decltype (x * m(0, 0))>
{
    Matrix<decltype (x * m(0, 0))> rez;
    for (SizeT row = 0; row < SIZE(); ++row) {
        for (SizeT col = 0; col < SIZE(); ++col) {
            rez(row, col) = x * m(row, col);
        }
    }
    return rez;
}

template <class T1, class T2>
auto operator*(Matrix<T1> const & m, T2 x)
-> Matrix<decltype (m(0, 0) * x)>
{
    return x * m;
}

template <class Type>
std::ostream & operator<<(std::ostream & os, Matrix<Type> const & m)
{
    for (SizeT row = 0; row < SIZE(); ++row) {
        for (SizeT col = 0; col < SIZE(); ++col) {
            os << m(row, col) << ' ';
        }
        os << std::endl;
    }
    return os;
}

template <class Type>
std::ostream & operator<<(std::ostream & os, Vector<Type> const & v)
{
    for (SizeT i = 0; i < SIZE(); ++i) {
        os << v[i] << ' ';
    }
    os << std::endl;
    return os;
}

template <class Type>
Vector<Type> & Vector<Type>::operator=(Type x)
{
    for (auto & i : data_) {
        i = x;
    }
}

} // namespace GLMath

#endif // INCLUDE_MATH_GLMATH_HPP
