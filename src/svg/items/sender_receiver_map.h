#ifndef SENDER_RECEIVER_MAP_H
#define SENDER_RECEIVER_MAP_H

#include <string>
#include <functional>
#include <map>

template<typename T>
class sender_receiver_map
{
  typedef std::function<void (const std::string &, const T &)> function_t;
  typedef std::map<std::pair<std::string, std::string>, function_t> items_map_t;

  /// contains receivers corresponding to a sender
  items_map_t m_item_receivers;
  /// contains senders corresponding to a receiver
  items_map_t m_item_senders;

public:
  sender_receiver_map () {}
  ~sender_receiver_map () {}

  void add_watch (const std::string &sender, const std::string &receiver, const function_t &func)
  {
    m_item_receivers[std::make_pair (sender, receiver)] = func;
    m_item_senders[std::make_pair (receiver, sender)] = func;
  }

  void item_changed (const std::string &sender, const T &params)
  {
    auto it = m_item_receivers.lower_bound (std::make_pair (sender, std::string ()));
    for (; it != m_item_receivers.end () && it->first.first == sender; ++it)
      it->second (sender, params);
  }

  void remove_watch (const std::string &sender, const std::string &receiver)
  {
    m_item_receivers.erase (std::make_pair (sender, receiver));
    m_item_senders.erase (std::make_pair (receiver, sender));
  }

  void remove_item (const std::string &item)
  {
    /// 1. erase receivers of this item
    auto it = m_item_receivers.lower_bound (std::make_pair (item, std::string ())), it_begin = it;
    for (; it != m_item_receivers.end () && it->first.first == item; ++it)
      m_item_senders.erase (std::make_pair (it->first.second, it->first.first));

    m_item_receivers.erase (it_begin, it);

    /// 2. erase senders to this item
    it = m_item_senders.lower_bound (std::make_pair (item, std::string ())); it_begin = it;
    for (; it != m_item_senders.end () && it->first.first == item; ++it)
      m_item_receivers.erase (std::make_pair (it->first.second, it->first.first));

    m_item_senders.erase (it_begin, it);
  }
};

#endif // SENDER_RECEIVER_MAP_H
