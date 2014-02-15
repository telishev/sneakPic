#ifndef ELEMENT_HANDLES_H
#define ELEMENT_HANDLES_H

#include <vector>

class abstract_handle;

class element_handles
{
public:
  virtual ~element_handles () {}

  virtual vector<abstract_handle *> handles () = 0;
};

class element_handles_vector : public element_handles
{
  vector<abstract_handle *> m_handles;
public:
  element_handles_vector (const vector<abstract_handle *> &handles);
  ~element_handles_vector ();

  virtual vector<abstract_handle *> handles () override { return m_handles; }
};

#endif // ELEMENT_HANDLES_H
