// homework_04_07_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/my_vector.h"
#include <algorithm>
#include <iostream>
#include <iterator>

namespace seq
{
    template<typename InputIt>
    using My_unique_vector = 
        My_vector<typename std::iterator_traits<InputIt>::value_type>;

    template<typename InputIt>
    My_unique_vector<InputIt> make_unique_vector(InputIt First, InputIt Last);
}

int main()
{
    seq::My_vector<int> vec_i{ 1,1,2,5,6,1,2,4,1,3,4,5,2,5,6,12,145,-23,0,7,-23 };

    std::cout << "[IN]:\t";
    std::for_each(std::cbegin(vec_i), std::cend(vec_i), 
        [](int Elem) ->void { std::cout << Elem << ' '; });
    std::cout << "\nsize: " << vec_i.size() << '\n' << "capacity: " << vec_i.capacity();

    // Удаляем дубликаты при создании нового вектора (т. к. функции erase пока нет), 
    // но прежде сортируем
    std::sort(std::begin(vec_i), std::end(vec_i));
    const auto vec_o{seq::make_unique_vector(std::begin(vec_i), std::end(vec_i))};
    
    std::cout << "\n\n[OUT]:\t";
    std::for_each(std::begin(vec_o), std::end(vec_o), 
        [](int Elem) ->void { std::cout << Elem << ' '; });
    std::cout << "\nsize: " << vec_o.size() << '\n' << "capacity: " << vec_o.capacity();

    std::cout << "\n\n";
    std::system("pause");
    return 0;
}

namespace seq
{
    template<typename InputIt>
    My_unique_vector<InputIt> make_unique_vector(InputIt First, InputIt Last)
    {
        My_vector<typename std::iterator_traits<InputIt>::value_type> vec;
        vec.reserve(Last - First);
        
        for (; First != Last; ++First) {
            auto current = std::next(First);
            for (; current != Last; ++current) {
                if (*First == *current) {
                    continue;
                }
                else {
                    break;
                }
            }
            vec.push_back(*First);
            First = std::prev(current);
        }
        return vec;
    }
}
