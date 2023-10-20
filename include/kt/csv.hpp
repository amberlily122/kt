#ifndef kt_csv_hpp_20231015_012404_PDT
#define kt_csv_hpp_20231015_012404_PDT
namespace kt {
  /* \brief Base class for all csv classes. 
   */
  class csv_base
  {
  public:
    static constexpr auto comma     = char { ','   }; 
    static constexpr auto quote     = char { '"'   }; 
    static constexpr auto newline   = char { '\n'  }; 
    static constexpr auto carriage  = char { '\r'  }; 
    static auto quoted(const auto& _src)
      {
        auto result = std::decay_t<decltype(_src)> {};
        result += quote;
        for(auto c : _src)
        {
          if(c == quote) result += quote;
          result += c;
        }
        result += quote;
        return result;
      };
    static auto quoted(const char* _src)
      {
        return quoted(std::string(_src));
      }
  };
    template<typename...Ts>
    class csv : public csv_base
    {
    public:
      explicit csv(auto&&..._args) : fields_(std::forward<decltype(_args)>(_args)...) {}
      template<typename _T>
      static auto send_field(auto& _os, const std::vector<_T>& _vector) -> void
        {
          send_field(_os, _vector.size());
          for(auto f : _vector)
          {
            send_field(_os, f);
          }
        }
      static auto send_field(auto& _os, const auto& _field) -> void
        {
          auto field_string = (std::stringstream {} << _field).str();
          static auto quote_trigger = std::array { quote, comma, '\r', '\n', (char)0 };
          const auto needs_quotes = field_string.find_first_of(quote_trigger.data()) != field_string.npos;
          _os << (needs_quotes? quoted(field_string) : field_string);
        }
      auto send(auto& _os) const -> auto&
        {
          auto send = [&](auto& _send, auto& _head, auto&..._tail) -> void
            {
              send_field(_os, _head);
              if constexpr(sizeof...(_tail) != 0) 
              {
                _os << comma;
                _send(_send, _tail...);
              }
            };
          std::apply([&](auto&..._args) { send(send, _args...); }, fields_);
          return _os;
        }
      auto recv(auto& _is) const -> auto&
        {
          using istream_type = std::decay_t<decltype(_is)>;
          auto parse_fields = [&]
            {
              auto fields = std::vector<std::string> {};
              auto line = std::string {};
              std::getline(_is, line);
              constexpr auto eol = std::char_traits<char>::eof();
              auto idx  = std::string::size_type(0);
              auto peek = [&] { return idx < line.size()? line[idx] : eol; };
              auto advance = [&] { ++idx; };
              auto next_field = [&] -> std::optional<std::string>
                {
                  auto end_of_field = [&]
                    {
                      return peek() == eol || peek() == comma;
                    };
                  auto scan_quoted = [&]
                    {
                      auto field = std::string{};
                      bool quoting = true;
                      advance();
                      while(quoting)
                      {
                        while(peek() != quote && peek() != eol) 
                        {
                          field += peek();
                          advance();
                        }
                        if(peek() == eol)
                        {
                          if(not _is.good())
                            throw std::runtime_error("unexpected end of file during quoted field");
                          std::getline(_is, line);
                          idx = 0; 
                        }
                        else
                        {
                          advance();
                          if(peek() == quote)
                          {
                            advance();
                            field += quote;
                          }
                          else
                          {
                            quoting = false;
                          }
                        }
                      }
                      if(not end_of_field())
                        throw std::runtime_error("unexpected chars after quoted string");
                      return field;
                    };
                  auto scan_unquoted = [&]
                    {
                      auto field = std::string {};
                      while(not end_of_field()) 
                      {
                        field += peek();
                        advance();
                      }
                      return field;
                    };
                  if(peek() == quote)
                  {
                    return scan_quoted();
                  }
                  else if(peek() == comma)
                  {
                    advance();
                    return std::nullopt;
                  }
                  else
                  {
                    return scan_unquoted();
                  }
                };
              while(peek() != eol)
              {
                auto field_maybe = next_field();
                if(field_maybe.has_value()) 
                {
                  fields.push_back(std::move(field_maybe.value()));
                }
              }
              return fields;
            };
          auto fields = parse_fields();
          auto recv = [&](auto& _recv, size_t idx, auto& _head, auto&..._tail) -> void
            {
              using field_type = std::decay_t<decltype(_head)>;
              (std::stringstream {} << fields[idx]) >> _head;
              if constexpr(sizeof...(_tail) != 0) _recv(_recv, idx + 1, _tail...);
            };
          std::apply
            ( [&](auto&..._args) 
              { 
                constexpr auto expected_count = sizeof...(_args);
                if(fields.size() != expected_count)
                  throw std::runtime_error(kt::concat("field count error; ", fields.size(), " != ", expected_count));
                recv(recv, 0, _args...); 
              }
            , fields_
            );
          return _is;
        }
    private:
      std::tuple<Ts...> fields_;
    };
    csv(auto&&..._args) -> csv<decltype(_args)...>;
  template<typename..._ArgTs>
    auto operator<<(auto& _ostream, const kt::csv<_ArgTs...>& _csv) -> decltype(_ostream)
    {
      return _csv.send(_ostream);
    }
  template<typename..._ArgTs>
    auto operator>>(auto& _istream, const kt::csv<_ArgTs...>& _csv) -> decltype(_istream)
    {
      return _csv.recv(_istream);
    }
} /* namespace kt */
#endif//kt_csv_hpp_20231015_012404_PDT
