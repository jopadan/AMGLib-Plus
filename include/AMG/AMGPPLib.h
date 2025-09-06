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

	enum class align
	{
		none     = 0 << 0,
		scalar   = 1 << 0,
		vector   = 1 << 1,
		matrix   = 1 << 2,
		adaptive = 1 << 3,
	};
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

	template<scalar T, size_t N = 1, enum align A = align::adaptive,  size_t N_POW2 = std::bit_ceil(N), size_t T_S = std::max<size_t>(alignof(T), sizeof(T))>
	struct alignas((N == N_POW2 || A == align::vector) && A != align::scalar ? N * T_S : T_S) buf : std::array<T,N>
	{
	};

	namespace vec 
	{
		template<scalar T, size_t N = 1, enum align A = align::adaptive, size_t N_POW2 = std::bit_ceil<size_t>(N), size_t T_S = std::max<size_t>(alignof(T), sizeof(T))>
		struct type : buf<T, N, A>
		{
		};

		template<size_t N = 1, enum align A = align::adaptive>
		using u64 = type<u64, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using u32 = type<u32, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using u16 = type<u16, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using u8  = type<u8 , N, A>;

		template<size_t N = 1, enum align A = align::adaptive>
		using s64 = type<s64, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using s32 = type<s32, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using s16 = type<s16, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using s8  = type<s8 , N, A>;

		template<size_t N = 1, enum align A = align::adaptive>
		using i64 = type<i64, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using i32 = type<i32, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using i16 = type<i16, N, A>;
		template<size_t N = 1, enum align A = align::adaptive>
		using i8  = type<i8 , N, A>;

		template<size_t N = 1, enum align A = align::adaptive>
		using f32 = type<f32, N, A>;
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

