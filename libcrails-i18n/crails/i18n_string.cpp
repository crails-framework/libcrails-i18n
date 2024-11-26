#include <boost/algorithm/string.hpp>
#include "i18n_settings.hpp"
#include "i18n_string.hpp"
#include <iostream>

using namespace std;
using namespace i18n;

static const string undefined_name = "null";

static const string& get_locale_name()
{
  const auto& settings = Settings::singleton::require();
  const string& name = settings.get_current_locale();

  return name.length() ? name : undefined_name;
}

ostream& operator<<(ostream& stream, const i18n::String& value)
{
  return stream << value.to_string();
}
    
String::String(const std::string& base_value)
{
  initialize(base_value);
}

bool String::is_localized()
{
  auto* settings = Settings::singleton::get();

  return settings && settings->use_localized_strings;
}

void String::initialize(const string& base_value)
{
  Data self = as_data();
  const string val = boost::trim_left_copy(
    boost::trim_right_copy(base_value)
  );

  clear();
  if (val[0] == '{' && val.back() == '}')
    self.merge(DataTree().from_json(val).as_data());
  else
    self[get_locale_name()] = val;
}

void String::from_data(Data data)
{
  initialize(data.defaults_to<string>(""));
}

String& String::operator=(const string& val)
{
  as_data()[get_locale_name()] = val;
  return *this;
}

String& String::operator=(const string_view val)
{
  cout << "DEBUG i18n::String::operator=" << endl;
  cout << get_locale_name() << endl;
  cout << "DEBUG #1" << endl;
  cout << val << endl;
  cout << "DEBUG #2" << endl;
  cout << as_data().exists() << endl;
  cout << "DEBUG #3" << endl;
  cout << as_data()[get_locale_name()].exists() << endl;
  cout << "DEBUG #4" << endl;
  as_data()["toto"] = val;
  cout << "DEBUG #5" << endl;
  as_data()[get_locale_name()] = val;
  cout << "DEBUT should've crashed by now" << endl;
  return *this;
}

string String::to_string() const
{
  const Data data = as_data();
  const Data default_value = data[get_locale_name()];

  if (default_value.exists())
    return default_value;
  else
  {
    const auto keys = data.get_keys();

    if (keys.size())
      return data[*keys.begin()];
  }
  return "";
}

vector<string> String::available_locales() const
{
  vector<string> list = as_data().get_keys();
  vector<string>::const_iterator it = find(list.begin(), list.end(), undefined_name);

  if (it != list.end())
    list.erase(it);
  return list;
}
