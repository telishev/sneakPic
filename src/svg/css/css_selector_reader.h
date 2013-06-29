#ifndef CSS_SELECTOR_READER_H
#define CSS_SELECTOR_READER_H

class abstract_css_selector;

class css_selector_reader
{
public:
  static abstract_css_selector *create_selector (const char *str);

private:
  static abstract_css_selector *create_single_selector (const char *str);
};

#endif // CSS_SELECTOR_READER_H
