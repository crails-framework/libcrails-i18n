#include "i18n.hpp"
#include "i18n_settings.hpp"

using namespace std;
using namespace i18n;

I18nData::I18nData(const Settings& settings) : settings(settings)
{
}

I18nData::I18nData(const Settings& settings, const map<string, string>& locales) : settings(settings)
{
  for (const auto& locale : locales)
    add_locale(locale.first, locale.second);
}

void I18nData::add_locale(const string& locale, const string& asset_path)
{
  string path = settings.directory + '/' + asset_path;
  auto it = data.find(locale);
  DataTree file_data;

  try
  {
    file_data.from_json_file(path);
    if (it == data.end())
      it = data.emplace(locale, DataTree()).first;
    it->second.as_data().merge(file_data.as_data());
  }
  catch (const exception& error)
  {
    string message("i18n: failed to load `" + path + '`');
    throw runtime_error(message + ": " + error.what());
  }
}

static Data t_with_fallback_to_default(const std::string& key)
{
  const Settings& settings = Settings::singleton::require();
  const DataTree& data = settings.t.data.at(settings.get_current_locale());

  if (data[key].exists())
    return data[key];
  return settings.t.data.at(settings.default_locale)[key];
}

namespace i18n
{
  const string& locale_name()
  {
    const Settings& settings = Settings::singleton::require();

    return settings.get_current_locale();
  }

  string t(const string& key)
  {
    return t_with_fallback_to_default(key).defaults_to<string>(key);
  }

  string t(const string& key, const map<string, string>& params)
  {
    string value = t(key);

    for (const auto& param : params)
      value =  _t_replace(value, param);
    return value;
  }
}
