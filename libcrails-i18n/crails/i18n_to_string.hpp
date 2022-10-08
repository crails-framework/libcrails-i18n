#ifndef  CRAILS_I18N_TO_STRING_HPP
# define CRAILS_I18N_TO_STRING_HPP

# include <string>
# include <sstream>

namespace i18n
{
  template<typename PARAM>
  std::string _t_replace(const std::string& base, const PARAM param)
  {
    const std::string prefix("{{");
    const std::string suffix("}}");
    std::stringstream result;
    std::string pattern   = prefix + param.first + suffix;
    auto        begin_pos = base.find(pattern);
    auto        end_pos   = begin_pos + pattern.size();

    if (begin_pos != std::string::npos)
    {
      result << base.substr(0, begin_pos)
             << param.second
             << base.substr(end_pos);
    }
    return result.str();
  }

  template<typename PARAM>
  std::string _t_part(const std::string& base, PARAM param)
  {
    return _t_replace<PARAM>(base, param);
  }

  template<typename PARAM, typename ...Args>
  std::string _t_part(const std::string& base, PARAM param, Args... args)
  {
    return _t_part<Args...>(_t_replace<PARAM>(base, param), args...);
  }
}

#endif
