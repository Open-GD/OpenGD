#pragma once
#include <fmt/format.h>

namespace GameToolbox
{
	template <typename... T>
		void log(fmt::format_string<T...> fmt, T&&... args) {
		return fmt::print("{}\n", fmt::format(fmt, std::forward<T>(args)...));
	}
}