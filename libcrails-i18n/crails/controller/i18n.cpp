#include "i18n.hpp"
#include "../i18n.hpp"
#include <crails/context.hpp>
#include <crails/utils/split.hpp>
#include <string>

using namespace std;
using namespace Crails;

struct AcceptLocaleOption
{
  string name;
  double quality_value = 1;

  bool operator>(const AcceptLocaleOption& compare) const { return quality_value > compare.quality_value; }
  bool operator<(const AcceptLocaleOption& compare) const { return quality_value < compare.quality_value; }
};

static vector<AcceptLocaleOption> parse_language_header(const string_view header)
{
  auto declarations = Crails::split(header, ',');
  vector<AcceptLocaleOption> options;

  options.reserve(declarations.size());
  for (const auto& declaration : declarations)
  {
    AcceptLocaleOption option;
    auto parts = Crails::split(declaration, ';');

    option.name = *parts.begin();
    if (parts.size() > 1 && parts.rbegin()->find("q=") == 0)
      option.quality_value = stod(parts.rbegin()->substr(2).data());
    options.push_back(option);
  }
  sort(options.begin(), options.end());
  return options;
}

void I18nController::initialize()
{
  Super::initialize();
  set_current_locale(find_request_locale());
}

void I18nController::set_current_locale(const std::string& name)
{
  i18n_settings.current_locale = name;
  response.set_header(HttpHeader::content_language, name);
}

string I18nController::find_request_locale() const
{
  if (session["lang"].exists())
    return session["lang"].as<std::string>();
  return find_current_locale_from_header();
}

string I18nController::find_current_locale_from_header() const
{
  auto header = request.find(Crails::HttpHeader::accept_language);

  if (header != request.end())
  {
    const auto& locales = i18n_settings.locales;
    const auto  options = parse_language_header(header->value().data());

    for (const AcceptLocaleOption& option : options)
    {
      auto locale = find(locales.begin(), locales.end(), option.name);

      if (locale != locales.end())
        return *locale;
    }
  }
  return i18n_settings.default_locale;
}

void I18nController::json_locale()
{
  const auto& locales = i18n_settings.locales;
  const auto  locale  = params["lang"].defaults_to<string>(i18n_settings.default_locale);
  auto it = find(locales.begin(), locales.end(), locale);

  if (it != locales.end())
    render(JSON, i18n_settings.t.data.at(locale).as_data());
  else
    respond_with(HttpStatus::not_found);
}
