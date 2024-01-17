// my_memory.h

#ifndef MY_LIBRARY_MEMORY_H
#define MY_LIBRARY_MEMORY_H

#include "my_types.h"

#include <exception>
#include <iterator>
#include <memory>
#include <type_traits>
#include <exception>
#include <concepts>

#ifdef EXCEPTION_TEST
#define MY_CALL_BAD_ALLOC throw std::bad_alloc()
#endif // EXCEPTION_TEST

namespace seq
{
	template<typename T>
	class My_unique_ptr
	{
	public:
		using element_type	= T;
		using pointer		= T*;
		using reference		= T&;

		constexpr My_unique_ptr() noexcept = default;
		explicit My_unique_ptr(pointer Ptr) noexcept
			: Current_{ Ptr }
		{}

		// ����������� ����������� ������ ����������!
		My_unique_ptr(const My_unique_ptr&)				= delete;
		My_unique_ptr& operator=(const My_unique_ptr&)	= delete;

		My_unique_ptr(My_unique_ptr&&)				= default;
		My_unique_ptr& operator=(My_unique_ptr&)	= default;

		~My_unique_ptr() noexcept
		{
			if (Current_) {
				delete Current_;
			}
		}

		// ���������� ������, ������������� *this, ������������ *get().
		reference operator*() const noexcept { return *get(); }
		
		// ���������� ��������� ������ �� ������ (������ ��� ���������),
		// ������������� *this, �.�. get().
		pointer operator->() const noexcept { return get(); }
		
		// ���������� ��������� �� ����������� ������ ��� nullptr,
		// ���� ������� �� ����������� ���������.
		pointer get() const noexcept { return Current_; }

		// �������� current_ptr, ���������, ������� �������� *this,
		// ��������� ��������� �������� � ��������� �������:
		// 1. ��������� ����� �������� ��������� Current_ � old.
		// 2. �������������� ������� ��������� � ���������� Current_ = New.
		// 3. ���� ������ ��������� (oll) ��� ��������, ������� ����� ����������� ������
		//    if (old) delete old.
		void reset(pointer New) noexcept
		{
			auto old = Current_; 
			Current_ = New;
			if (old) {
				delete old;
			}
		}

	private:
		pointer Current_{};
	};
		
	// ������� ������ �������� ���� T � �������������� ������� ��������� ���� T
	// � ���������� �������������������� ���������, �� ������� ��������� Ptr,
	// ��������� ���������� ���������� - new.
	template<typename Input_iter>
		requires Input_iterator<Input_iter>
	void my_uninitialized_fill(Input_iter First, Input_iter Last, 
		const Value_type<Input_iter>& Value)
	{
		Input_iter current{ First };
		try {
			for (; current != Last; ++current) 
			{
			#ifdef EXCEPTION_TEST
				MY_CALL_BAD_ALLOC;
			#endif // EXCEPTION_TEST
				std::construct_at(&*current, Value);
			}
		}
		catch (const std::bad_alloc&) {
			for (; First != current; ++First) {
				std::destroy_at(&*First);
			}
			throw;
		}
	}

	// ������� ������ �������� ���� T � �������������� ������ ���������� ���� T
	// ������� ��������� � ���������� �������������������� ���������, �� �������
	// ��������� Ptr, ��������� ���������� ���������� - new. 
	template<typename Input_iter, typename Output_iter>
		requires Input_iterator<Input_iter> && Output_iterator<Output_iter, Value_type<Input_iter>>
	void my_uninitialized_copy(Input_iter First, Input_iter Last, Output_iter t_First)
	{
		Output_iter current{ t_First };
		try {
			for (; First != Last; ++First, ++current)
			{
			#ifdef EXCEPTION_TEST
				MY_CALL_BAD_ALLOC;
			#endif // EXCEPTION_TEST
				std::construct_at(&*current, *First);
			}
		}
		catch (const std::bad_alloc&) {
			for (; t_First != current; ++t_First) {
				std::destroy_at(&*First);
			}
			throw;
		}
	}
}

#endif // MY_LIBRARY_MEMORY_H