#pragma once

namespace Pun
{
	// Implemented 'Expected' to avoid dependency on boost or recent C++
	namespace Expected
	{
		template<typename STORAGE_T, typename ERROR_T>
		class T
		{
		public:
			T(const STORAGE_T& value) : value_(value), has_value_(true) {}
			T(const ERROR_T& error) : error_(error) {}
			T() = delete;

			const STORAGE_T& operator*() const { return value_; }
			STORAGE_T& operator*() { return value_; }

			operator bool() const { return has_value_; }

			ERROR_T get_error() const { return error_; }

		private:
			bool has_value_{ false };
			STORAGE_T value_;
			ERROR_T error_;
		};
	}
}

template<typename STORAGE_T, typename ERROR_T>
using pun_expected_t = Pun::Expected::T<STORAGE_T, ERROR_T>;

