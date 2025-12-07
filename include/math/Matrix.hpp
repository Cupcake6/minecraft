#pragma once

#include <cstddef>
#include <cmath>
#include <span>
#include <cassert>
#include <array>
#include <initializer_list>
#include <ranges>

#include "math/pi.hpp"
#include "math/Vector.hpp"

namespace math {
    template <typename T, size_t COLUMN_COUNT, size_t ROW_COUNT>
    requires (COLUMN_COUNT >= 2 && COLUMN_COUNT <= 4 && ROW_COUNT >= 2 && ROW_COUNT <= 4)
    struct Matrix {
    public:
        static constexpr size_t SIZE = COLUMN_COUNT * ROW_COUNT;

        constexpr Matrix() = default;

        template <typename... Args>
        requires (sizeof...(Args) == SIZE && (std::is_same_v<T, Args> && ...))
        constexpr Matrix(Args... args) : array{args...} {}

        constexpr std::span<T, ROW_COUNT> operator[](size_t column_index) {
            assert(column_index < COLUMN_COUNT);
            return std::span<T, ROW_COUNT>(array.data() + column_index * ROW_COUNT, ROW_COUNT);
        }

        constexpr std::span<const T, ROW_COUNT> operator[](size_t column_index) const {
            assert(column_index < COLUMN_COUNT);
            return std::span<const T, ROW_COUNT>(array.data() + column_index * ROW_COUNT, ROW_COUNT);
        }

        template <size_t RHS_COLUMN_COUNT>
        constexpr Matrix<T, RHS_COLUMN_COUNT, ROW_COUNT> operator*(const Matrix<T, RHS_COLUMN_COUNT, COLUMN_COUNT>& rhs) const {
            Matrix<T, RHS_COLUMN_COUNT, ROW_COUNT> result;
            for (size_t rhs_column = 0; rhs_column < RHS_COLUMN_COUNT; ++rhs_column) {
                for (size_t row = 0; row < ROW_COUNT; ++row) {
                    for (size_t column = 0; column < COLUMN_COUNT; ++column) {
                        result[rhs_column][row] += (*this)[column][row] * rhs[rhs_column][column];
                    }
                }
            }

            return result;
        }

        constexpr Vector<T, ROW_COUNT> operator*(const Vector<T, COLUMN_COUNT>& rhs) const {
            Vector<T, ROW_COUNT> result;
            for (size_t row = 0; row < ROW_COUNT; ++row) {
                for (size_t column = 0; column < COLUMN_COUNT; ++column) {
                    result[row] += (*this)[column][row] * rhs[column];
                }
            }

            return result;
        }

        constexpr bool operator==(const Matrix<T, COLUMN_COUNT, ROW_COUNT>& rhs) const {
            for (size_t i = 0; i < SIZE; ++i) {
                if (array[i] != rhs.array[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr std::array<T, SIZE> get_flat_data() const {
            return array;
        }

    protected:
        std::array<T, SIZE> array = {};
    };

    using Matrix2f = Matrix<float, 2, 2>;
    using Matrix3f = Matrix<float, 3, 3>;
    using Matrix4f = Matrix<float, 4, 4>;

    constexpr Matrix4f perspective_projection(float FOV, float aspect_ratio, float near, float far) {
        float d = 1.0f / std::tanf(FOV * 0.5f);

        return Matrix4f {
            d / aspect_ratio, 0.0f, 0.0f, 0.0f,
            0.0f, d, 0.0f, 0.0f,
            0.0f, 0.0f, (far + near) / (far - near), 1.0f,
            0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f
        };
    }

    constexpr Matrix4f translation(float x, float y, float z) {
        return Matrix4f {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 1.0f
        };
    }

    constexpr Matrix4f rotation_y(float angle) {
        return Matrix4f {
            std::cosf(angle), 0.0f, -std::sinf(angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            std::sinf(angle), 0.0f, std::cosf(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    constexpr Matrix4f rotation_x(float angle) {
        return Matrix4f {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, std::cos(angle), -std::sin(angle), 0.0f,
            0.0f, std::sin(angle), std::cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
}