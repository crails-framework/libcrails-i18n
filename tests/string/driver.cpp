#include <crails/i18n_string.hpp>
#include <crails/i18n.hpp>

#undef NODEBUG
#include <cassert>

#include <iostream>

void cleanup()
{
  i18n::Settings::current_locale = "";
}

int main()
{
  // Localized strings are disabled by default
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;

    assert(i18n::String::is_localized() == false);
  }

  // Default constructor works
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    assert(string.to_string().length() == 0);
    assert(string.available_locales().size() == 0);
  }

  // When localized string are disabled, behaves as a regular string
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string("Coucou petite perruche");

    assert(string.to_string() == "Coucou petite perruche");
    string = std::string_view("Hejo spann den wagen an");
    assert(string.to_string() == "Hejo spann den wagen an");
  }

  // Store translations according to the current locale
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    i18n_settings->use_localized_strings = true;

    i18n_settings->default_locale = "fr";
    string = std::string_view("Suis-je bien chez ce cher Serge ?");

    i18n_settings->current_locale = "es";
    string = std::string("Que triste estas, Tristan, con tan tetrica trama teatral!");

    assert(string["fr"].as<std::string>() == "Suis-je bien chez ce cher Serge ?");
    assert(string["es"].as<std::string>() == "Que triste estas, Tristan, con tan tetrica trama teatral!");
    cleanup();
  }

  // Properly report existing translation in the current locale
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    i18n_settings->use_localized_strings = true;

    i18n_settings->default_locale = "fr";
    assert(string.has_translation() == false);
    string = std::string_view("Suis-je bien chez ce cher Serge ?");
    assert(string.has_translation() == true);

    i18n_settings->current_locale = "es";
    assert(string.has_translation() == false);
    string = std::string("Que triste estas, Tristan, con tan tetrica trama teatral!");
    assert(string.has_translation() == true);
    cleanup();
  }

  // Renders as string according to the current locale
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    i18n_settings->use_localized_strings = true;
    i18n_settings->default_locale = "fr";
    string["fr"] = "Suis-je bien chez ce cher Serge ?";
    string["es"] = "Que triste estas, Tristan, con tan tetrica trama teatral!";

    {
      std::ostringstream stream;
      stream << string;
      assert(stream.str() == "Suis-je bien chez ce cher Serge ?");
    }

    {
      std::ostringstream stream;
      i18n_settings->current_locale = "es";
      stream << string;
      assert(stream.str() == "Que triste estas, Tristan, con tan tetrica trama teatral!");
    }
    cleanup();
  }

  // Loads JSON from a raw string
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    i18n_settings->use_localized_strings = true;
    i18n_settings->default_locale = "fr";

    string.initialize("{\"fr\":\"Coucou petite perruche\",\"en\":\"Hello little birdie\"}");
    assert(string.to_string() == "Coucou petite perruche");

    i18n_settings->current_locale = "en";
    assert(string.to_string() == "Hello little birdie");
    cleanup();
  }

  // Loads data from a raw string
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;

    i18n_settings->use_localized_strings = true;
    i18n_settings->default_locale = "fr";

    string.initialize("Coucou petite perruche");
    assert(string.to_string() == "Coucou petite perruche");
    assert(string["fr"].as<std::string>() == "Coucou petite perruche");
    cleanup();
  }

  // Lists available locales
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String string;
    std::vector<std::string> list;

    i18n_settings->use_localized_strings = true;
    i18n_settings->default_locale = "fr";
    string["fr"] = "Suis-je bien chez ce cher Serge ?";
    string["es"] = "Que triste estas, Tristan, con tan tetrica trama teatral!";
    string["null"] = "Defined when locales were disabled";
    list = string.available_locales();
    assert(std::find(list.begin(), list.end(), "fr") != list.end());
    assert(std::find(list.begin(), list.end(), "es") != list.end());
    assert(std::find(list.begin(), list.end(), "null") == list.end());
  }

  // Copy constructor
  {
    SingletonInstantiator<i18n::Settings> i18n_settings;
    i18n::String source, target;
    std::vector<std::string> list;

    i18n_settings->use_localized_strings = true;
    i18n_settings->default_locale = "fr";
    source["fr"] = "Suis-je bien chez ce cher Serge ?";
    target = source;
    assert(target.to_string() == "Suis-je bien chez ce cher Serge ?");
  }
 
  return 0;
}

