// my_allocator.h

#ifndef MY_LIBRARY_ALLOCATOR_H
#define MY_LIBRARY_ALLOCATOR_H

#include <exception>
#include <memory>

#define _EXCEPTION_TEST

#ifdef EXCEPTION_TEST
#define MY_CALL_BAD_ARRAY_NEW_LENGTH throw std::bad_array_new_length()
#endif // EXCEPTION_TEST

namespace seq
{
	template<typename T>
	struct My_allocator
	{
		using element_type = T;
		using size_type = typename std::size_t;
		using pointer = T*;

		My_allocator() = default;
		
		My_allocator(const My_allocator&)				= default;
		My_allocator& operator=(const My_allocator&)	= delete;
		My_allocator(My_allocator&&)					= delete;
		My_allocator& operator=(My_allocator&&)			= delete;

		~My_allocator() = default;

		// �������� Number*sizeof(T) ���� �������������������� ������ ����� ������
		// ::operator new(std::size_t). ����� ��� ������� ������� � ��������� ������ ���� T[Number]
		// � ���������� ��� ����� �����, �� �� ���������� ����� ����� �� ������ �� ��� ���������.
		// ���������� ��������� �� ������ ������� ������� �� Number �������� ���� T, ��������
		// �������� ��� �� ���������.
		pointer allocate(size_type Number) 
		{
			const size_t size{ sizeof(element_type) * Number };
			void* ptr{};
			try {
			#ifdef EXCEPTION_TEST
				MY_CALL_BAD_ARRAY_NEW_LENGTH;
			#endif /* EXCEPTION_TEST */
				ptr = reinterpret_cast<void*>(::operator new[](size));
			}
			catch (const std::bad_array_new_length&) {
				throw;
			}
			return static_cast<element_type*>(ptr);
		}

		// ������� ������, �� ������� ��������� ��������� Ptr, ������� ������ ���� ����������,
		// ���������� � ���������� ����������� ������ allocate(). �������� Number ������ ���� �����
		// ������� ��������� ������ allocate(), ������� ������������� ������ Ptr.
		// �������� ::operator delete(void*).
		void deallocate(pointer Ptr, size_type Number) 
		{
			::operator delete[](Ptr, Number);
		}
	};
}
#endif // MY_LIBRARY_ALLOCATOR_H