#ifndef currency_hpp_20250227_172431_PST
#define currency_hpp_20250227_172431_PST
namespace kt {
class currency
{
  friend auto operator<<(std::ostream&, const currency&) -> std::ostream&;
  friend auto operator+(const currency&, const currency&) -> currency;
  friend auto operator-(const currency&, const currency&) -> currency;
  friend auto operator*(const currency&, const currency&) -> currency;
  friend auto operator/(const currency&, const currency&) -> currency;
public:
  using value_type = int64_t;
  static constexpr auto scale_factor = 100;
  explicit currency(int _value) : currency(_value, 0) {}
  currency(const int _whole, const int _fraction)
    {
      value_ = _whole * scale_factor + _fraction;
    }
  explicit currency(double _d)
    {
      value_ = _d * scale_factor;
    }
  explicit currency(float _f)
    {
      value_ = _f * scale_factor;
    }
  
private:
  enum direct_init_tag { direct_init };
  currency(direct_init_tag, value_type _v)
      : value_(_v) 
    {}
  auto get_value() const -> value_type 
    { 
      return value_; 
    }
  value_type    value_;
};
auto operator+(const currency& _lhs, const currency& _rhs) -> currency
  {
    return currency(currency::direct_init, _lhs.get_value() + _rhs.get_value());
  }
auto operator-(const currency& _lhs, const currency& _rhs) -> currency
  {
    return currency(currency::direct_init, _lhs.get_value() - _rhs.get_value());
  }
auto operator*(const currency& _lhs, const currency& _rhs) -> currency
  {
    return currency(currency::direct_init, _lhs.get_value() * _rhs.get_value() / currency::scale_factor);
  }
auto operator/(const currency& _lhs, const currency& _rhs) -> currency
  {
    return currency(currency::direct_init, _lhs.get_value() / _rhs.get_value() * currency::scale_factor);
  }
auto operator<<(std::ostream& _os, const currency& _c) -> std::ostream&
  {
    using namespace std;
    _os << (stringstream() << fixed << setprecision(2) << (double(_c.get_value()) / currency::scale_factor)).str();
    return _os;
  }
} /* namespace kt */
#endif//currency_hpp_20250227_172431_PST
