#ifndef ELEMENT_HANDLES_H
#define ELEMENT_HANDLES_H

#include <vector>

class abstract_handle;

class element_handles
{
public:
  virtual ~element_handles () {}

  virtual std::vector<abstract_handle *> handles () = 0;
};

class element_handles_vector : public element_handles
{
  std::vector<abstract_handle *> m_handles;
public:
  element_handles_vector (const std::vector<abstract_handle *> &handles);
  ~element_handles_vector ();

  virtual std::vector<abstract_handle *> handles () override { return m_handles; }
};

#endif // ELEMENT_HANDLES_H
