#ifndef concat_hpp_20230109_073634_PST
#define concat_hpp_20230109_073634_PST
#include <string>
#include <valarray>
#include <sstream>
namespace kt {
template<typename _T>
  auto to_string(_T&& _var)
  {
    using var_type = std::decay_t<_T>;
    if constexpr( std::is_same_v<var_type, std::string>
               || std::is_same_v<var_type, std::string_view>
               || std::is_same_v<var_type, const char*>
                )
    {
      return _var;
    }
    else if constexpr(std::is_same_v<var_type, char>)
    {
      return std::string(1, _var);
    }
    else if constexpr(std::is_convertible_v<var_type, std::string>)
    {
      return static_cast<std::string>(_var);
    }
    else
    {
      auto ss = std::stringstream {};
      ss << _var;
      return std::move(ss.str());
    }
  };
template<typename..._Ts>
  auto concat(_Ts&&..._vars) -> std::string
  {
    auto msg = std::string {};
    ((msg.append(kt::to_string(std::forward<_Ts>(_vars)))), ...);
    return msg;
  }
template<typename T>
struct is_valarray : std::false_type {};

template<typename T>
struct is_valarray<std::valarray<T>> : std::true_type {};

template<typename _T> constexpr auto is_valarray_v = is_valarray<_T>::value;

template<typename _T, typename..._U>
  auto concat_arr(const _T& _arr, auto&& _delim)
  {
    auto result = std::string {};
    auto iter = std::begin(_arr);
    const auto end_iter  = std::end(_arr);
    if(iter == end_iter) 
      { return result; }
    else if(_arr.size() == 1) 
      { return result += _arr[0]; }
    else if(_arr.size() == 2)
      { return kt::concat(_arr[0], " or ", _arr[1]); }
    else
    {
      while(iter != end_iter)
      {
        result += *iter;
        ++iter;
        if(iter != end_iter) result += _delim;
        if(iter + 1 == end_iter) { result += "or "; };
      }
    }
    return result;
  }
} /* namespace kt */
#endif//concat_hpp_20230109_073634_PST
