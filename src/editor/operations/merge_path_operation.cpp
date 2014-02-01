#include "merge_path_operation.h"

#include "svg/items/svg_item_path.h"
#include "path/svg_path_geom.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_path_data.h"

template<typename Iter>
std::pair<Iter,Iter> slide(Iter begin, Iter end, Iter target)
{
  if (target < begin) return std::make_pair( target, std::rotate(target, begin, end) );
  if (end < target) return std::make_pair( std::rotate( begin, end, target ), target );
  return std::make_pair( begin, end );
}

merge_path_operation::merge_path_operation ()
{
}

merge_path_operation::~merge_path_operation ()
{
   
}

void merge_path_operation::apply (svg_path *path_dest, svg_path_geom_iterator dest, svg_path *path_src, svg_path_geom_iterator src)
{
  if (path_dest->get_geom () == path_src->get_geom ())
    merge_inside (path_dest, dest, src);
  else
    merge_paths (path_dest, dest, path_src, src);
}

void merge_path_operation::merge_inside (svg_path *path, svg_path_geom_iterator dest, svg_path_geom_iterator src)
{
  /// if it is the same subpath, just close it
  if (&dest.subpath () == &src.subpath ())
    {
      auto &elements = dest.subpath ().elements ();
      auto &front_elem = elements.front ();
      auto &back_elem = elements.back ();
      if (elements.size () > 1 && front_elem.point == back_elem.point)
        {
          front_elem.c1 = back_elem.c1;
          
          svg_path_geom_iterator last_segment_point (*path->get_geom (), dest.get_subpath_index (), dest.subpath ().last_point ());
          size_t point_index = last_segment_point.point_index ();
          path->get_node_type ()->erase (path->get_node_type ()->begin () + point_index);
          elements.pop_back ();
        }
      dest.subpath ().set_closed (true);
      return;
    }

  /// else merge two subpaths

  // if dest is subpath begin, prepend src
  std::vector<single_path_point> &dest_subpath = dest.subpath ().elements (), &src_subpath = src.subpath ().elements ();
  if (src_subpath.size () > 0)
    {
      svg_path_geom *geom = path->get_geom ();
      auto line_segment = path->get_is_line_segment ();
      auto node_types = path->get_node_type ();
      auto line_begin = line_segment->begin ();
      auto node_type_begin = node_types->begin ();
      int dest_subpath_index = (int)dest.get_subpath_index ();
      int src_subpath_index = (int)src.get_subpath_index ();
      auto src_begin_it = geom->subpath_begin (src_subpath_index);
      auto dst_begin_it = geom->subpath_begin (dest_subpath_index);

      if (dest.get_subpath_point () == 0)
        {
          if (src.get_subpath_point () == 0)
            path->reverse_subpath ((int)src.get_subpath_index ());

          slide (line_begin + src_begin_it.segment_index (false), line_begin + src_begin_it.segment_index (false) + src.subpath ().total_segments (),
                 line_begin + dst_begin_it.segment_index (false));
          slide (node_type_begin + src_begin_it.point_index (), node_type_begin + src_begin_it.point_index () + src.subpath ().total_points (),
                 node_type_begin + dst_begin_it.point_index ());

          node_types->erase (node_type_begin + src_begin_it.point_index () + src.subpath ().total_points () - 1);
          dest_subpath.insert (dest_subpath.begin (), src_subpath.begin (), src_subpath.end () - 1);

        }
      else
        {
          /// reverse src if necessary
          if (src.get_subpath_point () != 0)
            path->reverse_subpath ((int)src.get_subpath_index ());
      
          slide (line_begin + src_begin_it.segment_index (false), line_begin + src_begin_it.segment_index (false) + src.subpath ().total_segments (),
                 line_begin + dst_begin_it.segment_index (false) + dest.subpath ().total_segments ());

          slide (node_type_begin + src_begin_it.point_index (), node_type_begin + src_begin_it.point_index () + src.subpath ().total_points (),
                 node_type_begin + dst_begin_it.point_index () + dest.subpath ().total_points ());

          node_types->erase (node_type_begin + src_begin_it.point_index ());
          dest_subpath.insert (dest_subpath.end (), src_subpath.begin () + 1, src_subpath.end ());

        }
    }

  auto & subpath = path->get_geom ()->subpath ();
  subpath.erase (subpath.begin () + src.get_subpath_index ());
}

void merge_path_operation::merge_paths (svg_path *item_path_dest, svg_path_geom_iterator dest,
                                        svg_path *item_path_src, svg_path_geom_iterator src)
{

  /// TODO: merge line and node types too
  svg_path_geom *dest_path = item_path_dest->get_geom ();
  svg_path_geom *src_path = item_path_src->get_geom ();
  src_path->apply_transform (item_path_src->transform () * item_path_dest->transform ().inverted ());

  /// TOOO: make it better
  size_t dest_path_count = dest_path->subpath ().size ();
  for (const auto &src_subpath : src_path->subpath ())
    dest_path->subpath ().push_back (src_subpath);

  item_path_dest->get_is_line_segment ()->insert (item_path_dest->get_is_line_segment ()->end (),
    item_path_src->get_is_line_segment ()->begin (), item_path_src->get_is_line_segment ()->end ());

  item_path_dest->get_node_type ()->insert (item_path_dest->get_node_type ()->end (),
    item_path_src->get_node_type ()->begin (), item_path_src->get_node_type ()->end ());

  svg_path_geom_iterator new_src_it = svg_path_geom_iterator (*dest_path, src.get_subpath_index () + dest_path_count, src.get_subpath_point ());
  merge_inside (item_path_dest, dest, new_src_it);
}

