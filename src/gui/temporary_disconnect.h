// Macros used to correctly disconnect slot inside of block
#ifndef TEMPORARY_DISCONNECT_H
#define TEMPORARY_DISCONNECT_H

#include <QObject>

#include "common/common_utils.h"

class connector
{
  const QObject *sender;
  const char    *signal;
  const QObject *receiver;
  const char    *method;
  unsigned int connected : 1;
  unsigned int disconnect_on_free : 1;

public:
  connector (const QObject *sender_, const char *signal_, const QObject *receiver_, const char *method_, unsigned int disconnect_on_free_ = 1)
  {
    sender = sender_;
    signal = signal_;
    receiver = receiver_;
    method = method_;
    disconnect_on_free = disconnect_on_free_;
    connected = 0;
    connect ();
  }

  ~connector ()
  {
    if (disconnect_on_free)
      disconnect ();
  }

  int connect ()
  {
    if (connected)
      return -1;
    connected = (bool) QObject::connect (sender, signal, receiver, method);
    return connected ? 0 : -1;
  }

  int disconnect ()
  {
    if (!connected)
      return -1;
    connected = !(QObject::disconnect (sender, signal, receiver, method));
    return connected ? -1 : 0;
  }

  int is_connected () const { return connected; }

  friend class disconnector;

private:
  connector (const connector &other);
  connector &operator= (const connector &other);
};


class disconnector
{
  int disconnect_success;
  const QObject *sender;
  const char *signal;
  const QObject *receiver;
  const char *method;

public:
  disconnector (const QObject *sender_, const char *signal_, const QObject *receiver_, const char *method_)
  {
    init (sender_, signal_, receiver_, method_);
  }

  disconnector (connector *connector_)
  {
    init (connector_->sender, connector_->signal, connector_->receiver, connector_->method);
  }

  ~disconnector ()
  {
    if (disconnect_success)
      QObject::connect (sender, signal, receiver, method);
  }

private:
  disconnector (const disconnector &other);
  disconnector &operator= (const disconnector &other);

  void init (const QObject *sender_, const char *signal_, const QObject *receiver_, const char *method_)
  {
    sender = sender_;
    signal = signal_;
    receiver = receiver_;
    method = method_;
    disconnect_success = QObject::disconnect (sender, signal, receiver, method);
  }
};

// temporary disconnects signal and connect it again at the end of block
#define TEMPORARY_DISCONNECT(sender, signal, receiver, method) \
  disconnector CONCATENATE(disconnect_,__LINE__) (sender, signal, receiver, method)

#endif // TEMPORARY_DISCONNECT_H
