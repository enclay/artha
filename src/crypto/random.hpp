#pragma once

#include <random>
#include <string>
#include <type_traits>

namespace artha {

	std::string RandomSecretKey();

	namespace internal {
		
		template <bool IsFloatingPoint, typename T>
		struct DistributionSelector;
		
		template <typename T>
		struct DistributionSelector<false, T> {
			using type = std::uniform_int_distribution<T>;
		};
		
		template <typename T>
		struct DistributionSelector<true, T> {
			using type = std::uniform_real_distribution<T>;
		};
		
		template <typename T>
		using UniformDistribution = typename DistributionSelector<std::is_floating_point_v<T>, T>::type;
	}

	template <class T>
	T Random(T from, T to) {
		std::random_device engine;
		internal::UniformDistribution<T> dist(from, to);
		return dist(engine);
	}

}
