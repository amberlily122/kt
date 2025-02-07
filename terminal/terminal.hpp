#ifndef terminal_hpp_20250206_173322_PST
#define terminal_hpp_20250206_173322_PST
namespace kt {

class terminal
{
public:
  auto locate(int _row, int _col) -> terminal& { return do_locate(_row, _col); }
  auto move(int _x, int _y) -> terminal& { return do_move(_x, _y); }
private:
  virtual auto do_locate(int _row, int _col) -> void = 0;
  virtual auto do_move(int _x, int _y) -> void = 0;

  auto print_str(std::string_view _s) -> terminal& { return do_print_str(_s); }

  template<typename _ArgT>
  auto print(_ArgT&& _arg) -> terminal&
    {
      print_str(boost_lexical_cast<std::string>(_arg));
    }
  template<typename _ArgT, typename..._ArgTs>
  auto print(_ArgT&& _head, _ArgTs&&..._tail) -> terminal&
    {
      print(_head);
      print(_tail...);
    }
};

class ncurses_terminal
{
};
} /* namespace kt */
#endif//terminal_hpp_20250206_173322_PST
