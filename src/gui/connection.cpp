#include "gui/connection.h"

#include "common/debug_utils.h"

connection::connection (func_t connect_func)
{
  m_connect_func = connect_func;
  DEBUG_ASSERT (connect ());
}

connection::~connection ()
{

}

bool connection::connected () const
{
  return (bool)m_result;
}

bool connection::connect ()
{
  DEBUG_ASSERT (!connected ());
  m_result = m_connect_func ();
  return connected ();
}

bool connection::disconnect ()
{
  if (connected ())
    return QObject::disconnect (m_result);

  return false;
}

auto_connection connection::temporary_disconnect ()
{
  return std::move (auto_connection (*this));
}
