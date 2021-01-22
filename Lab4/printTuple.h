#ifndef LAB4_PRINTTUPLE_H
#define LAB4_PRINTTUPLE_H

#include <ostream>

template<size_t N, class... Args>
struct tuplePrint{
    static void f(std::ostream& os, std::tuple<Args...> const& t){
        tuplePrint<N-1, Args...>::f(os,t);
        os << "[" << std::get<N>(t) << "] ";
    }
};

template<class... Args>
struct tuplePrint<0, Args...>{
    static void f(std::ostream& os, std::tuple<Args...> const& t){
        os << "[" << std::get<0>(t) << "] ";
    }
};


template <class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const& t){
    tuplePrint<std::tuple_size<std::tuple<Args...>>::value - 1, Args...>::f(os, t);
    return os;
}

#endif //LAB4_PRINTTUPLE_H
