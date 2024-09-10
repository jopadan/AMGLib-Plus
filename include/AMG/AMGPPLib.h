#pragma once

#include <concepts>
#include <bit>
#include <array>
#include <cstdint>
#include <cstddef>
#include <ctime>
#include <psprtc.h>
#include <algorithm>
#include <numeric>

namespace amg
{
	template<typename T>
	concept scalar = std::integral<T> || std::floating_point<T>;

	using i64 = int64_t;
	using i32 = int32_t;
	using i16 = int16_t;
	using i8  = int8_t;
	using s64 = signed int64_t;
	using s32 = signed int32_t;
	using s16 = signed int16_t;
	using s8  = signed int8_t;
	using u64 = uint64_t;
	using u32 = uint32_t;
	using u16 = uint16_t;
	using u8  = uint8_t;

	using f32 = float;
	using f64 = double;

	namespace vec
	{
		template<scalar T, size_t ROWS = 1, size_t N = std::bit_ceil(ROWS)> requires (std::popcount(N) == 1)
		using vector_aligned __attribute__((vector_size(sizeof(T) * N))) = T;

		template<scalar T, size_t N = 3>
		struct element_aligned : std::array<T, N>
		{
			operator T*() { return (*this).data(); }
			operator vector_aligned<T, 4>()
			{
				return { (*this)[0], (*this)[1], (*this)[2], 0 };
			}
		};

		template<scalar T, size_t N = 1>
		using type = std::conditional_t<N == 1, T, std::conditional_t<std::popcount(N) == 1, vector_aligned<T, N>, element_aligned<T, N>>>;

		template<scalar T, auto tmp = 0, T w = (T)tmp>
		extern inline vector_aligned<T, 4> lda3(T src[3])
		{
			return (vector_aligned<T,4>){ src[0], src[1], src[2], w };
		}

		template<scalar T, auto tmp = 1, T w = (T)tmp>
		extern inline element_aligned<T, 3> sto3(vector_aligned<T, 4> src)
		{
			src[3] *= w;
			if((src[3] != (T)0) && (src[3] != (T)1))
				src /= src[3];
			return (element_aligned<T, 3>){ src[0], src[1], src[2] };
		}

		template<size_t N = 1>
		using u64 = type<u64, N>;
		template<size_t N = 1>
		using u32 = type<u32, N>;
		template<size_t N = 1>
		using u16 = type<u16, N>;
		template<size_t N = 1>
		using u8  = type<u8 , N>;

		template<size_t N = 1>
		using s64 = type<s64, N>;
		template<size_t N = 1>
		using s32 = type<s32, N>;
		template<size_t N = 1>
		using s16 = type<s16, N>;
		template<size_t N = 1>
		using s8  = type<s8 , N>;

		template<size_t N = 1>
		using i64 = type<i64, N>;
		template<size_t N = 1>
		using i32 = type<i32, N>;
		template<size_t N = 1>
		using i16 = type<i16, N>;
		template<size_t N = 1>
		using i8  = type<i8 , N>;

		template<size_t N = 1>
		using f32 = type<f32, N>;

                template<scalar T, size_t N>
                inline T dot(type<T,N> a, type<T,N> b) { return std::accumulate(a * b); }
		template<scalar T, size_t N>
                inline T dot_div(type<T,N> a, type<T,N> b) { return std::accumulate(a / b); }

                template<scalar T, size_t N = 2, size_t M = N>
                inline type<T, N> neg2(type<T, M> src, uint8_t mod = 2, uint8_t val = 0)
                {
                        type<T, N> dst;
                        dst[0] = (0 % mod == val) ? -src[0] : src[0];
                        dst[1] = (1 % mod == val) ? -src[1] : src[1];
                        return dst;
                }
                template<scalar T, size_t N = 2, size_t M = N>
                inline type<T,N> swp2(type<T, M> src)
                {
                        type<T,N> dst;
                        dst[0] = src[1];
                        dst[1] = src[0];
                        return dst;
                }

                template<scalar T, size_t N> requires (N >= 2)
                inline type<T,N> cross2(type<T,N> src, unsigned int winding = GU_CCW) { return neg2<T,N,2>({1,1}, 2, 1 - (winding % 2)) * swp2<T,N,N>(src); }

                template<scalar T, size_t N> requires (N >= 3)
                inline type<T,N> cross3(type<T,N> a, type<T,N> b) { return (a.rot3l(1) * b - a * b.rot3l(1)).rot3l(1); }

		template<scalar T, size_t N> requires (N >= 4)
		inline type<T,N> cross4(type<T,N> a, type<T,N> b, type<T, N> c)
		{
		}
        };

	namespace mat
	{
		template<scalar T, size_t COLS, size_t ROWS>
		struct type : std::array<vec::type<T, ROWS>, COLS>
		{
			type(std::initializer_list<T[ROWS]> src)
			{
				auto iter = src.begin();
				for(size_t cols = 0; (cols < COLS) && (iter != src.end()); ++cols)
					(*this)[cols] = *reinterpret_cast<const vec::type<T, ROWS>*>(iter++);
			}
			operator T*() { return &(*this)[0][0]; }
			vec::type<T, ROWS>& column(size_t i) { return (*this)[i % COLS]; }
                        vec::type<T, COLS>  row(size_t i) { vec::type<T, COLS> dst; for(size_t j = 0; j < COLS; j++) dst[j] = (*this)[j][i % ROWS];  }
                        /* matrix/scalar operators */
                        type<T, COLS, ROWS>& operator+=(const T s)
                        {
                                for(size_t i = 0; i < COLS; i++)
                                        (*this)[i] += s;
				return *this;
                        }
                        type<T, COLS, ROWS>& operator-=(const T s)
                        {
                                for(size_t i = 0; i < COLS; i++)
                                        (*this)[i] -= s;
				return *this;
                        }
                        type<T, COLS, ROWS>& operator*=(const T s)
                        {
                                for(size_t i = 0; i < COLS; i++)
                                        (*this)[i] *= s;
				return *this;
                        }
                        type<T, COLS, ROWS>& operator/=(const T s)
                        {
                                for(size_t i = 0; i < COLS; i++)
                                        (*this)[i] /= s;
				return *this;
                        }
                        /* matrix/vector operators */
                        vec::type<T, COLS> operator*(const vec::type<T, ROWS> v)
                        {
                                vec::type<T, COLS> dst;
                                for(size_t i = 0; i < COLS; i++)
                                        dst[i] = row(i) * v;
                                return dst;
                        }
                        vec::type<T, COLS> operator/(const vec::type<T, ROWS> v)
                        {
                                vec::type<T, COLS> dst;
                                for(size_t i = 0; i < COLS; i++)
                                        dst[i] = row(i) / v;
                                return dst;
                        }
                        /* matrix/matrix operator */
                        type<T, COLS, ROWS> operator+(const type<T, COLS, ROWS> rhs)
                        {
                                type<T, COLS, ROWS> dst;
                                for(size_t i = 0; i < COLS; i++)
                                        dst = (*this)[i] + rhs[i];
                        }
                        type<T, COLS, ROWS> operator-(const type<T, COLS, ROWS> rhs)
                        {
                                type<T, COLS, ROWS> dst;
                                for(size_t i = 0; i < COLS; i++)
                                        dst = (*this)[i] - rhs[i];
                        }
                        type<T, COLS, ROWS> operator*(const type<T, COLS, ROWS> rhs)
                        {
                                type<T, COLS, ROWS> dst;
                                for(size_t j = 0; j < COLS; j++)
                                        for(size_t i = 0; i < ROWS; i++)
                                                dst[j][i] = vec::dot((*this).row(i), rhs.column(j));
                                return dst;
                        }
			type<T, COLS, ROWS> operator/(const type<T, COLS, ROWS> rhs)
			{
				type<T, COLS, ROWS> dst;
				for(size_t j = 0; j < COLS; j++)
					for(size_t i =0; i < ROWS; i++)
						dst[j][i] = vec::dot_div((*this).row(i), rhs.column(j));
				return dst;
			}
		};

		template<size_t COLS, size_t ROWS>
		using u64 = type<u64, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using u32 = type<u32, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using u16 = type<u16, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using u8  = type<u8 , COLS, ROWS>;

		template<size_t COLS, size_t ROWS>
		using s64 = type<s64, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using s32 = type<s32, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using s16 = type<s16, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using s8  = type<s8 , COLS, ROWS>;

		template<size_t COLS, size_t ROWS>
		using i64 = type<i64, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using i32 = type<i32, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using i16 = type<i16, COLS, ROWS>;
		template<size_t COLS, size_t ROWS>
		using i8  = type<i8 , COLS, ROWS>;

		template<size_t COLS, size_t ROWS>
		using f32 = type<f32, COLS, ROWS>;
	};

	struct timer
	{
		u64 now;
		u64 last;
		u64 delta;
		u32 res;
		u64 frames;
		f32 curr_ms;
		f32 fps;

		timer()
		{
			now = get_tick();
			last = 0;
			delta = 0;
			res = get_res();
			frames = 0;
			curr_ms = 0.0f;
			fps = 0.0f;
		}
		u32 get_res()
		{
			res = sceRtcGetTickResolution();
			return res;
		}
		u64 get_tick()
		{
			sceRtcGetCurrentTick(&now);
			return now;
		}
		f32 update()
		{
			frames++;
			now = get_tick();
			delta = now - last;
			if(delta >= res)
			{
				fps = 1.0f / curr_ms;
				curr_ms = (delta / (float)res) / frames;
				frames = 0;
				last = now;
			}
			return delta /(float)res;
		}
	};
};

