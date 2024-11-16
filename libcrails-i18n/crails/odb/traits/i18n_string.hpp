#pragma once
#include <cstring>
#include "../../i18n_string.hpp"

namespace odb
{
  namespace pgsql
  {
    template<>
    class value_traits<i18n::String, id_string>
    {
    public:
      typedef i18n::String value_type;
      typedef value_type query_type;
      typedef details::buffer image_type;

      static void
      set_value (value_type& i18n_string,
                 const details::buffer& b,
                 std::size_t n,
                 bool is_null)
      {
        if (i18n_string.is_localized() && b.data()[0] == '{')
          odb::pgsql::value_traits<DataTree, id_string>::set_value(i18n_string, b, n, is_null);
        else
          i18n_string = std::string_view(b.data(), n);
      }

      static void
      set_image (details::buffer& b,
                 std::size_t& n,
                 bool& is_null,
                 const value_type& i18n_string)
      {
        if (i18n_string.is_localized())
          odb::pgsql::value_traits<DataTree, id_string>::set_image(b, n, is_null, i18n_string);
        else
        {
          std::string as_string = i18n_string.to_string();

          is_null = false;
          n = as_string.size();
          std::memcpy(b.data(), as_string.c_str(), n);
        }
      }
    };
  }
}
