#pragma once

namespace Pun
{
	namespace Optional
	{
		struct none_t {};

		template<typename STORAGE_T>
		class T
		{
		public:
			T(const STORAGE_T& value) : value_(value), has_value_(true) {}
			T() {}
			T(none_t) {}

			const STORAGE_T& operator*() const { return value_; }
			STORAGE_T& operator*() { return value_; }

			operator bool() const { return has_value_; }

		private:
			bool has_value_{ false };
			STORAGE_T value_;
		};

		static const none_t none;
	}
}

template <typename STORAGE_T>
using pun_optional_t = Pun::Optional::T<STORAGE_T>;


