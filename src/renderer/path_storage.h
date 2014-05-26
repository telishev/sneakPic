#ifndef PATH_STORAGE_H
#define PATH_STORAGE_H

// Simple class that loads paths from .svg files from resources directory and stores + gives access to them as QPainterPath

class QPainterPath;

enum class path_id
{
  DOUBLE_HEADED_ARROW,
  ROTATE_ARROW,

  COUNT
};

class path_storage
{
  vector <unique_ptr<QPainterPath>> m_data;

public:
  path_storage ();
  const QPainterPath &path (path_id id) const;
  static const path_storage &ref ();

private:
  QString path_to_svg (path_id idw) const;
  unique_ptr<QPainterPath> generate_path (const QString &path);
};

#endif // PATH_STORAGE_H

