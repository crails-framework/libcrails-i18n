#pragma once
#include <crails/datatree.hpp>
#include <string_view>
#include <sstream>

namespace i18n
{
  struct String : public DataTree
  {
    String() {}
    String(const std::string& base_value);
    String(const String& copy) { as_data().merge(copy); }
    String(Data item) { as_data().merge(item); }

    static bool is_localized();
    bool has_translation() const;

    void initialize(const std::string&);
    void from_data(Data);
    String& operator=(const std::string_view val);
    String& operator=(const std::string& val); 
    operator std::string() const { return to_string(); }

    std::string to_string() const;
    std::vector<std::string> available_locales() const; 
  };
}

std::ostream& operator<<(std::ostream& o, const i18n::String& d);
