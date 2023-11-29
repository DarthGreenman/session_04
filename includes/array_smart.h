/* array_smart.h */

#ifndef MY_LIBRARY_ARRAY_SMART_H
#define MY_LIBRARY_ARRAY_SMART_H

#include "test.h"

#include <cstddef>
#include <stdexcept>

#define NO_EXCEPTION_TEST

namespace seq
{
	template<typename T>
	class Array;

	template<typename T>
	class Array_smart
	{
	public:
		template<typename T> friend class Array;
		using value_type	= T;
		using size_type		= typename std::size_t;
		using pointer		= value_type*;

	private:
		Array_smart() = default;
		Array_smart(size_type Count)
			: Array_smart()
		{
			try {
				allocate(Count, Count / 2);
#ifdef EXCEPTION_TEST
				test::test_bad_alloc();
#endif /* EXCEPTION_TEST */
			}
			catch (const std::bad_alloc&) {
				throw;
			}
		}
		/*****************************************************************************************/
		Array_smart(const Array_smart& Copied) = delete;
		Array_smart& operator=(const Array_smart& Copied) = delete;
		/*****************************************************************************************/
		Array_smart(Array_smart&& Movable) noexcept
			: Array_smart()
		{
			swaps_contents(Movable);
		}
		Array_smart& operator=(Array_smart&& Movable) noexcept
		{
			auto seq(std::move(Movable));
			swaps_contents(seq);
			return *this;
		}
		/*****************************************************************************************/
		~Array_smart()
		{
			deallocate();
		}
		/*****************************************************************************************/
		void resize(size_type Count)
		{
			deallocate();
			try {
				allocate(Count, Count / 2);
#ifdef EXCEPTION_TEST
				test::test_bad_alloc();
#endif /* EXCEPTION_TEST */
			}
			catch (const std::bad_alloc&) {
				throw;
			}
		}
		void swaps_contents(Array_smart& Movable) noexcept
		{
			std::swap(first_,	Movable.first_);
			std::swap(last_,	Movable.last_);
			std::swap(end_,		Movable.end_);
		}
		void allocate(size_type Count, size_t Reserve)
		{
			first_	= new value_type[Count + Reserve]{};
			last_	= first_ + Count;
			end_	= last_ + Reserve;
		}
		void deallocate()
		{
			if (first_ != end_) {
				delete[] first_;
			}
		}

	private:
		pointer	first_{};	/* Указатель на первый элемент */
		pointer	last_{};	/* Указатель на элемент следующий за последним */
		pointer	end_{};		/* Указатель на адрес памяти следующий за последним выделенным */
	};
}
#endif /* MY_LIBRARY_ARRAY_SMART_H */