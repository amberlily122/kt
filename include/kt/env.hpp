#ifndef env_hpp_20230301_144421_PST
#define env_hpp_20230301_144421_PST
#include <cstdlib>
#include <string_view>
namespace kt {
 
auto getenv(std::string_view _n) -> std::string_view
  {
    auto e = std::getenv(_n);
    return e? e : "";
  }
} /* namespace kt */
#endif//env_hpp_20230301_144421_PST
