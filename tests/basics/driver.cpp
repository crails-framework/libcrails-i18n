#include <crails/i18n_string.hpp>
#include <crails/i18n.hpp>

#undef NODEBUG
#include <cassert>

static void initialize_translations(SingletonInstantiator<i18n::Settings>& i18n_settings)
{
  DataTree fr_translations, es_translations;

  fr_translations["basic"] = "Je veux et j'exige d'exquises excuses";
  es_translations["basic"] = "Papa pone pan para Pepin";
  fr_translations["nested"]["basic"] = "Coucou petite perruche";
  fr_translations["nested"]["vars"] = "Coucou {{name}} !";

  i18n_settings->default_locale = "fr";
  i18n_settings->t.data.emplace("fr", fr_translations);
  i18n_settings->t.data.emplace("es", es_translations);
}

int main()
{
  using namespace std;

  // Uses default locale when no current locale is defined
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;

    initialize_translations(i18n_settings);
    assert(i18n::t("basic") == "Je veux et j'exige d'exquises excuses");
    i18n_settings->default_locale = "es";
    assert(i18n::t("basic") == "Papa pone pan para Pepin");
  }

  // Uses current locale over default locale
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;

    initialize_translations(i18n_settings);
    i18n_settings->current_locale = "es";
    assert(i18n::t("basic") == "Papa pone pan para Pepin");
  }

  // Supports nested keys
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;

    initialize_translations(i18n_settings);
    assert(i18n::t("nested.basic") == "Coucou petite perruche");
  }

  // Supports variables
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    string result;

    initialize_translations(i18n_settings);
    result = i18n::t("nested.vars", {{"name", "Roger"}});
    assert(result == "Coucou Roger !");
  }
 
  return 0;
}
