#ifndef CONNECTION_H
#define CONNECTION_H

#include "common/common_utils.h"

#include <functional>
#include <memory>
#include <QObject>

class auto_connection;

class connection
{
  typedef std::function<QMetaObject::Connection ()> func_t;
  func_t m_connect_func;
  QMetaObject::Connection m_result;
public:
  connection (func_t connect_func);
  ~connection ();

  bool connected () const;
  bool connect ();
  bool disconnect ();

  auto_connection temporary_disconnect ();
};

class auto_connection
{
  connection &m_connection;
  bool m_disconnect_success;
public:
  auto_connection (connection &connection_) : m_connection (connection_)
  {
    m_disconnect_success = m_connection.disconnect ();
  }

  auto_connection (auto_connection &&rhs)
    : m_connection (rhs.m_connection)
  {
    m_disconnect_success = rhs.m_disconnect_success;
    rhs.m_disconnect_success = false;
  }

  ~auto_connection ()
  {
    if (m_disconnect_success)
      m_connection.connect ();
  }
};

#define TEMPORARY_DISCONNECT(CONNECTION) auto_connection connection##__LINE__ (CONNECTION->temporary_disconnect ());
#define CONNECT(...) EXPAND (std::move(std::unique_ptr<connection> (new connection ([&] () { return QObject::connect (__VA_ARGS__); }))))
#endif // CONNECTION_H
