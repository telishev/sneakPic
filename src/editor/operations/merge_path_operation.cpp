#include "merge_path_operation.h"

#include "svg/items/svg_item_path.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_path_data.h"

merge_path_operation::merge_path_operation ()
{
}

merge_path_operation::~merge_path_operation ()
{
   
}

void merge_path_operation::apply (svg_item_path *path_dest, svg_path_iterator dest, svg_item_path *path_src, svg_path_iterator src)
{
  if (path_dest == path_src)
    merge_inside (path_dest, dest, src);
  else
    merge_paths (path_dest, dest, path_src, src);
}

void merge_path_operation::merge_inside (svg_item_path *path, svg_path_iterator dest, svg_path_iterator src)
{
  /// TODO: support line and node types
  auto path_edit = path->get_attribute_for_change<svg_attribute_path_data> ();

  /// if it is the same subpath, just close it
  if (&dest.subpath () == &src.subpath ())
    {
      auto &elements = dest.subpath ().elements ();
      auto &front_elem = elements.front ();
      auto &back_elem = elements.back ();
      if (elements.size () > 1 && front_elem.point == back_elem.point)
        {
          front_elem.c1 = back_elem.c1;
          elements.pop_back ();
        }
      dest.subpath ().set_closed (true);
      path->process_after_read ();
      return;
    }

  /// else merge two subpaths

  // if dest is subpath begin, prepend src
  std::vector<single_path_point> &dest_subpath = dest.subpath ().elements (), &src_subpath = src.subpath ().elements ();
  if (src_subpath.size () > 0)
    {
      if (dest.get_subpath_point () == 0)
        {
          if (src.get_subpath_point () == 0)
            src.subpath ().reverse ();

          dest_subpath.insert (dest_subpath.begin (), src_subpath.begin (), src_subpath.end () - 1);
        }
      else
        {
          /// reverse src if necessary
          if (src.get_subpath_point () != 0)
            src.subpath ().reverse ();
      
          dest_subpath.insert (dest_subpath.end (), src_subpath.begin () + 1, src_subpath.end ());
        }
    }


  svg_path *dest_path = path_edit->path ();
  dest_path->subpath ().erase (dest_path->subpath ().begin () + src.get_subpath_index ());


  path->process_after_read ();
}

void merge_path_operation::merge_paths (svg_item_path *item_path_dest, svg_path_iterator dest,
                                        svg_item_path *item_path_src, svg_path_iterator src)
{

  /// TODO: merge line and node types too
  auto path_dest_edit = item_path_dest->get_attribute_for_change<svg_attribute_path_data> ();
  auto path_src_edit = item_path_src->get_computed_attribute <svg_attribute_path_data> ();
  svg_path *dest_path = path_dest_edit->path ();
  const svg_path *src_path = path_src_edit->path ();
  /// TOOO: make it better
  size_t dest_path_count = dest_path->subpath ().size ();
  for (const auto &src_subpath : src_path->subpath ())
    dest_path->subpath ().push_back (src_subpath);

  svg_path_iterator new_src_it = svg_path_iterator (*dest_path, src.get_subpath_index () + dest_path_count, src.get_subpath_point ());
  merge_inside (item_path_dest, dest, new_src_it);
  if (item_path_src->parent ())
    item_path_src->parent ()->remove_child (item_path_src);
}

