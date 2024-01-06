/* my_vector_iterator.h */

#ifndef MY_LIBRARY_VECTOR_ITERATOR_H
#define MY_LIBRARY_VECTOR_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace seq
{
	template<typename T, typename A>
	class My_vector;
	
	template<typename T>
	struct My_allocator;

	template<typename T>
	class My_vector_iterator
	{
	public:
		template<typename T, typename A> 
		friend class My_vector;
		template<typename T>
		friend struct My_allocator;

		using iterator_category	= typename std::random_access_iterator_tag;
		using value_type		= T;
		using difference_type	= typename std::ptrdiff_t;
		using pointer			= T*;
		using const_pointer		= const T*;
		using reference			= T&;
		using const_reference	= const T&;
	
	private:
		My_vector_iterator() = default;
		explicit My_vector_iterator(pointer Iter)
			: Ptr_{ Iter } {}
	
	public:
		My_vector_iterator(const My_vector_iterator&)				= default;
		My_vector_iterator& operator=(const My_vector_iterator&)	= default;

		My_vector_iterator(My_vector_iterator&&)					= default;
		My_vector_iterator& operator=(My_vector_iterator&&)			= default;

		~My_vector_iterator() = default;
		
		const_reference operator*() const	{ return *Ptr_; }
		reference operator*()				
		{ 
			return
				const_cast<reference>(
					static_cast<const My_vector_iterator&>(*this).operator*()
					);
		}
		
		const_pointer operator->() const	{ return Ptr_; }
		pointer operator->()				
		{
			return 
				const_cast<pointer>(
					static_cast<const My_vector_iterator*>(*this)->operator->()
					);
		}

		My_vector_iterator& operator++()
		{
			++Ptr_;
			return *this;
		}
		My_vector_iterator operator++(int)
		{
			const My_vector_iterator iter{ *this };
			++(*this);
			return iter;
		}
		My_vector_iterator& operator--()
		{
			--Ptr_;
			return *this;
		}
		My_vector_iterator operator--(int)
		{
			const My_vector_iterator iter{ *this };
			--(*this);
			return iter;
		}

		My_vector_iterator& operator+=(const difference_type& Offset)
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			Ptr_ += Offset;
			return *this;
		}
		My_vector_iterator operator+(const difference_type& Offset) const
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			My_vector_iterator iter{ *this };
			iter += Offset;
			return iter;
		}
		My_vector_iterator& operator-=(const difference_type& Offset) 
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			return *this += -Offset;
		}
		My_vector_iterator operator-(const difference_type& Offset) const
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			My_vector_iterator iter{ *this };
			iter -= Offset;
			return iter;
		}
		
		const_reference operator[](const difference_type& Offset) const
		{
			return *(*this + Offset);
		}
		
		reference operator[](const difference_type& Offset)
		{
			return 
				const_cast<reference>(
					static_cast<const My_vector_iterator&>(*this)[Offset]
					);
		}
		
	private:
		pointer Ptr_{};
	};

	template<typename T>
	My_vector_iterator<T> operator+(
		const typename My_vector_iterator<T>::difference_type& Offset,
		const My_vector_iterator<T>& Iter
		)
	{
		/* Значение n должно находиться в диапазоне представимых
		 * значений типа difference_type */
		return Iter + Offset;
	}	
	
	template<typename T>
	typename My_vector_iterator<T>::difference_type operator-(
		const My_vector_iterator<T>& Left, 
		const My_vector_iterator<T>& Right
		)
	{
		/* Существует значение n типа difference_type такое, что first + n == last */
		const typename My_vector_iterator<T>::const_pointer last	= &*Left;
		const typename My_vector_iterator<T>::const_pointer first	= &*Right;
		return std::distance(first, last);
	}
	
	template<typename T>
	bool operator==(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		const typename My_vector_iterator<T>::const_pointer lhs	= &*Lhs;
		const typename My_vector_iterator<T>::const_pointer rhs	= &*Rhs;
		return lhs == rhs;
	}

	template<typename T>
	bool operator!=(const My_vector_iterator<T>& Lhs,
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs == Rhs);
	}

	template<typename T>
	bool operator<(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		const typename My_vector_iterator<T>::const_pointer lhs	= &*Lhs;
		const typename My_vector_iterator<T>::const_pointer rhs	= &*Rhs;
		return lhs < rhs;
	}

	template<typename T>
	bool operator>(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		const typename My_vector_iterator<T>::const_pointer lhs	= &*Lhs;
		const typename My_vector_iterator<T>::const_pointer rhs	= &*Rhs;
		return lhs > rhs;
	}

	template<typename T>
	bool operator<=(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs < Rhs);
	}

	template<typename T>
	bool operator>=(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs > Rhs);
	}
}

#endif /* MY_LIBRARY_VECTOR_ITERATOR_H */