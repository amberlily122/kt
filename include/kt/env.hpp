#ifndef env_hpp_20230301_144421_PST
#define env_hpp_20230301_144421_PST
#include <cstdlib>
#include <string_view>
#include <optional>
namespace kt {
/* \brief Get the value of an environment variable as a string_view.
*/ 
inline auto getenv(const char* _n) -> std::string_view
  {
    auto e = std::getenv(_n);
    return e? e : "";
  }
/* \brief Get the value of an environment variable as an optional string_view.
*/
inline auto optenv(const char* _n) -> std::optional<std::string_view>
  {
    auto e = std::getenv(_n);
    return e? e : std::nullopt;
  }
} /* namespace kt */
#endif//env_hpp_20230301_144421_PST
