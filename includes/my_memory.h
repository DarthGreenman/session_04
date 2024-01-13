// my_memory.h

#ifndef MY_LIBRARY_MEMORY_H
#define MY_LIBRARY_MEMORY_H

#include <exception>
#include <iterator>
#include <memory>
#include <type_traits>

#ifdef EXCEPTION_TEST
#define MY_CALL_BAD_ALLOC throw std::bad_alloc()
#endif // EXCEPTION_TEST

namespace seq
{
	template<typename T>
	concept InputIterator = std::input_iterator<T> ||
		std::is_pointer<T>::value;

	template<typename I, typename T>
	concept OutputIterator = std::output_iterator<I, T>;

	template<typename Iter>
	using Value_type = typename std::iterator_traits<Iter>::value_type;

	// —оздает массив объектов типа T и инициализирует объекты значением типа T
	// в выделенном неинициализированном хранилище, на которое указывает Ptr,
	// использу€ глобальное размещение - new.
	template<typename InputIter>
		requires InputIterator<InputIter>
	void my_uninitialized_fill(InputIter First, InputIter Last, 
		const Value_type<InputIter>& Value)
	{
		InputIter current{ First };
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

	// —оздает массив объектов типа T и инициализирует массив элементами типа T
	// массива источника в выделенном неинициализированном хранилище, на которое
	// указывает Ptr, использу€ глобальное размещение - new.
	template<typename InputIter, typename OutputIter>
		requires InputIterator<InputIter> && OutputIterator<OutputIter, Value_type<InputIter>>
	void my_uninitialized_copy(InputIter First, InputIter Last, OutputIter t_First)
	{
		OutputIter current{ t_First };
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