#ifndef  CRAILS_I18N_SETTINGS_HPP
# define CRAILS_I18N_SETTINGS_HPP

# include <vector>
# include <string>
# include <crails/utils/singleton.hpp>
# include "i18n.hpp"

namespace i18n
{
  class Settings
  {
    SINGLETON(Settings)
  protected:
    Settings() : t(*this) {}
  public:
    std::string                     directory;
    std::string                     default_locale;
    std::vector<std::string>        locales;
    static thread_local std::string current_locale;
    I18nData                        t;
    bool                            use_localized_strings = false;

    const std::string& get_current_locale() const { return current_locale != "" ? current_locale : default_locale; }
  };
}

#endif
