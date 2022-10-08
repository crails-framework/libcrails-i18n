#include "i18n.hpp"

using namespace std;
using namespace i18n;

I18nData::I18nData()
{
}

I18nData::I18nData(const map<string, string>& locales)
{
  for (const auto& locale : locales)
    add_locale(locale.first, locale.second);
}

void I18nData::add_locale(const string& locale, const string& asset_path)
{
  string path = Settings::directory + '/' + asset_path;
  auto it = data.find(locale);
  DataTree file_data;

  file_data.from_json_file(path);
  if (it == data.end())
    it = data.emplace(locale, DataTree()).first;
  it->second.as_data().merge(file_data.as_data());
}

static Data t_with_fallback_to_default(const std::string& key)
{
  const DataTree& data = Settings::t.data.at(Settings::get_current_locale());

  if (data[key].exists())
    return data[key];
  return Settings::t.data.at(Settings::default_locale)[key];
}

namespace i18n
{
  string t(const string& key)
  {
    return t_with_fallback_to_default(key).defaults_to<string>(key);
  }

  std::string t(const std::string& key, const std::map<std::string, std::string>& params)
  {
    string value = t(key);

    for (const auto& param : params)
      value =  _t_replace(value, param);
    return value;
  }
}
