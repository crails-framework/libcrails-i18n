#ifndef  CRAILS_I18N_CONTROLLER_HPP
# define CRAILS_I18N_CONTROLLER_HPP

# include <crails/controller.hpp>

namespace Crails
{
  class I18nController : public Crails::Controller
  {
    typedef Crails::Controller Super;
  public:
    I18nController(Crails::Context& context) : Super(context)
    {
    }

    void initialize();
    void set_current_locale(const std::string& name);
    void json_locale();

  private:
    std::string find_request_locale() const;
    std::string find_current_locale_from_header() const;
  };
}

#endif
