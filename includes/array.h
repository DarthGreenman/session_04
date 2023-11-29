/* array.h */

#ifndef MY_LIBRARY_ARRAY_H
#define MY_LIBRARY_ARRAY_H

#include "array_smart.h"
#include "array_iterator.h"

#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace seq
{
	template<typename T>
	class Array
	{
	public:
		using value_type		= T;
		using size_type			= typename std::size_t;
		using difference_type	= typename std::ptrdiff_t;
		using reference			= value_type&;
		using const_reference	= const value_type&;
		using iterator			= Array_iterator<value_type>;
		using const_iterator	= Array_iterator<const value_type>;
	
	public:
		Array() = default;
		Array(size_type Count, const_reference& Value = T()) try
			: sequence_{ Count }
		{
			std::fill(begin(), end(), Value);
		}
		catch (...) {
			throw;
		}
		/*****************************************************************************************/
		Array(const Array& Copied) 
			: sequence_{ Copied.size() }
		{
			std::copy(Copied.begin(), Copied.end(), begin());
		}
		Array& operator=(const Array& Copied)
		{
			if (this != &Copied) {
				Array<value_type> temp{ Copied };
				sequence_.swaps_contents(temp.sequence_);
			}
			return *this;
		}
		/*****************************************************************************************/
		Array(Array&& Movable) noexcept
			: sequence_{ std::move(Movable.sequence_) }
		{
		}
		Array& operator=(Array&& Movable) noexcept
		{
			sequence_ = std::move(Movable.sequence_);
			return *this;
		}
		/*****************************************************************************************/
		Array(const std::initializer_list<value_type>& Value_list)
			: sequence_{ Value_list.size() }
		{
			std::copy(Value_list.begin(), Value_list.end(), begin());
		}
		~Array() = default;
		/*****************************************************************************************/
		void push_back(const value_type& New)
		{
			if (sequence_.last_ == sequence_.end_) 
			{
				Array<value_type> temp(*this);
				sequence_.resize(size() + 1);
				std::copy(temp.cbegin(), temp.cend(), sequence_.first_);
				--sequence_.last_;
			}
			*(sequence_.last_) = New;
			++sequence_.last_;
		}
		/*****************************************************************************************/
		reference operator[](size_type Index) 
		{
			if (!(Index >= 0 && Index < size())) {
				throw std::out_of_range("Index of an element outside the array.");
			}
			const auto it = begin();
			return it[Index];
		}
		/*****************************************************************************************/
		iterator begin() 
		{
			return iterator(sequence_.first_);
		}
		iterator end() 
		{
			return iterator(sequence_.last_);
		}
		const_iterator begin() const 
		{
			return const_iterator(sequence_.first_);
		}
		const_iterator end() const 
		{
			return const_iterator(sequence_.last_);
		}
		const_iterator cbegin() const 
		{
			return const_iterator(sequence_.first_);
		}
		const_iterator cend() const 
		{
			return const_iterator(sequence_.last_);
		}
		/*****************************************************************************************/
		size_type size() const 
		{
			return cend() - cbegin();
		}
		size_type capacity() const 
		{
			return sequence_.end_ - sequence_.first_;
		}

	private:
		Array_smart<value_type> sequence_;
	};
}
#endif /* MY_LIBRARY_ARRAY_H */