#ifndef  CRAILS_I18N_SETTINGS_HPP
# define CRAILS_I18N_SETTINGS_HPP

# include <vector>
# include <string>
# include "i18n.hpp"

namespace i18n
{
  struct Settings
  {
    static const std::string              directory;
    static const std::string              default_locale;
    static const std::vector<std::string> locales;
    static thread_local std::string       current_locale;

    static const std::string& get_current_locale() { return current_locale != "" ? current_locale : default_locale; }
    static const I18nData t;
  };
}

#endif
