#pragma once

namespace Pun
{
	template<typename T, typename E>
	class expected
	{
	public:
		expected(const T& value) : value_(value), has_value_(true) {}
		expected(const E& error) : error_(error) {}
		expected() = delete;

		const T& operator*() const { return value_; }
		T& operator*() { return value_; }

		operator bool() const { return has_value_; }

		E get_error() const { return error_; }

	private:
		bool has_value_{ false };
		T value_;
		E error_;
	};
}


