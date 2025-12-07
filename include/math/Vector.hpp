#pragma once

#include <array>
#include <cmath>

namespace math {
    template <typename T, size_t SIZE>
    requires (SIZE >= 2 && SIZE <= 4)
    struct Vector {
    public:
        constexpr Vector() = default;

        template <typename... Args>
        requires (sizeof...(Args) == SIZE && (std::is_same_v<T, Args> && ...))
        constexpr Vector(Args... args) : array{args...} {}

        Vector& operator+=(const Vector<T, SIZE>& rhs) {
            for (size_t i = 0; i < SIZE; ++i) {
                array[i] += rhs.array[i];
            }

            return *this;
        }

        Vector& operator-=(const Vector<T, SIZE>& rhs) {
            for (size_t i = 0; i < SIZE; ++i) {
                array[i] -= rhs.array[i];
            }

            return *this;
        }

        Vector& operator*=(const T& rhs) {
            for (size_t i = 0; i < SIZE; ++i) {
                array[i] *= rhs;
            }

            return *this;
        }

        Vector& operator/=(const T& rhs) {
            for (size_t i = 0; i < SIZE; ++i) {
                array[i] /= rhs;
            }

            return *this;
        }

        constexpr bool operator==(const Vector<T, SIZE>& rhs) const {
            for (size_t i = 0; i < SIZE; ++i) {
                if (array[i] != rhs.array[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr bool operator!=(const Vector<T, SIZE>& rhs) const {
            return !(*this == rhs);
        }

        constexpr Vector operator-() const {
            Vector<T, SIZE> result = *this;
            for (size_t i = 0; i < SIZE; ++i) {
                result = -result;
            }

            return result;
        }

        constexpr Vector operator+(const Vector<T, SIZE>& rhs) const {
            Vector<T, SIZE> result = *this;
            for (size_t i = 0; i < SIZE; ++i) {
                result.array[i] += rhs.array[i];
            }

            return result;
        }

        constexpr Vector operator-(const Vector<T, SIZE>& rhs) const {
            Vector<T, SIZE> result = *this;
            for (size_t i = 0; i < SIZE; ++i) {
                result.array[i] -= rhs.array[i];
            }

            return result;
        }

        constexpr Vector operator*(const T& rhs) const {
            Vector<T, SIZE> result = *this;
            for (size_t i = 0; i < SIZE; ++i) {
                result.array[i] *= rhs;
            }

            return result;
        }

        constexpr Vector operator/(const T& rhs) const {
            Vector<T, SIZE> result = *this;
            for (size_t i = 0; i < SIZE; ++i) {
                result.array[i] /= rhs;
            }

            return result;
        }

        constexpr bool is_zero() const {
            for (size_t i = 0; i < SIZE; ++i) {
                if (array[i] != ZERO) {
                    return false;
                }
            }

            return true;
        }

        constexpr T length_squared() const {
            T sum = {};
            for (size_t i = 0; i < SIZE; ++i) {
                sum += array[i] * array[i];
            }

            return sum;
        }

        constexpr T length() const {
            return std::sqrt(length_squared());
        }

        constexpr Vector normalize() const {
            Vector<T, SIZE> result = *this;
            T length = this->length();
            for (size_t i = 0; i < SIZE; ++i) {
                result.array[i] /= length;
            }

            return result;
        }

        constexpr T& operator[](size_t index) {
            assert(index < SIZE);
            return array[index];
        }

        constexpr const T& operator[](size_t index) const {
            assert(index < SIZE);
            return array[index];
        }

        constexpr T& x() {
            return array[0];
        }

        constexpr const T& x() const {
            return array[0];
        }

        constexpr T& y() {
            return array[1];
        }

        constexpr const T& y() const {
            return array[1];
        }

        constexpr T& z() requires (SIZE >= 3) {
            return array[2];
        }

        constexpr const T& z() const requires (SIZE >= 3) {
            return array[2];
        }

        constexpr T& w() requires (SIZE >= 4) {
            return array[3];
        }

        constexpr const T& w() const requires (SIZE >= 4) {
            return array[3];
        }

    private:
        std::array<T, SIZE> array = {};
        static constexpr T ZERO = {};
    };

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;
}