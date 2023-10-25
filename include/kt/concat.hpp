#ifndef concat_hpp_20230109_073634_PST
#define concat_hpp_20230109_073634_PST
#include <string>
#include <string_view>
#include <sstream>
namespace kt {
/* \brief Convert a string to a string; this function exists for overloading purposes.
*/
auto to_string(const std::string& _src) -> std::string { return _src; }
/* \brief Convert a string_view to a string; this function exists for overloading purposes.
*/
auto to_string(std::string_view _src)   -> std::string { return std::string(_src); }
/* \brief Convert a c-string to a string; this function exists for overloading purposes. */
auto to_string(const char* _src)        -> std::string { return _src; }
/* \brief Convert a variable of type `_T` to a string as if it were sent to the 
          formatted output operator of a std::stringstream object.
*/
template<typename _T>
  auto to_string(_T&& _var)             -> std::string
  {
    return (std::stringstream {} << _var).str();
  }
/* \brief Convert each argument into a string, concatenate them, and return the result
          as a new string.
*/
template<typename..._Ts>
  auto concat(_Ts&&..._vars) -> std::string
  {
    auto msg = std::string {};
    ((msg.append(kt::to_string(std::forward<_Ts>(_vars)))), ...);
    return msg;
  }
} /* namespace kt */
#endif//concat_hpp_20230109_073634_PST
