#pragma once

namespace SimpleRTTR
{
	inline void HashCombine([[maybe_unused]] std::size_t& seed) { }

	template <typename ClassType, typename... Rest>
	inline void HashCombine(std::size_t& seed, const ClassType& v, Rest & ... rest) {
		std::hash<ClassType> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		HashCombine(seed, rest...);
	}
}