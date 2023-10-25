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
} /* namespace kt */
#endif//concat_hpp_20230109_073634_PST
