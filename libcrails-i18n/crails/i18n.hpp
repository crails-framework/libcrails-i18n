#ifndef __CHEERP_CLIENT__
#ifndef  CRAILS_I18N_HPP
# define CRAILS_I18N_HPP
# include <crails/datatree.hpp>
# include <map>
# include <vector>
# include "i18n_to_string.hpp"

namespace i18n
{
  class Settings;

  struct I18nData
  {
    I18nData(const Settings&);
    I18nData(const Settings&, const std::map<std::string, std::string>&);

    void add_locale(const std::string& locale, const std::string& asset_path);

    std::map<std::string, DataTree> data;
    const Settings& settings;
  };

  const std::string& locale_name();

  std::string t(const std::string& key);

  std::string t(const std::string& key, const std::map<std::string, std::string>& params);

  template<typename PARAM, typename ...Args>
  std::string t(const std::string& key, PARAM param, Args... args)
  {
    std::string value = _t_replace(t(key), param);

    return _t_part(value, args...);
  }

  template<typename PARAM>
  std::string t(const std::string& key, PARAM param)
  {
    return _t_replace<PARAM>(t(key), param);
  }
}

# include "i18n_settings.hpp"

#endif
#endif
