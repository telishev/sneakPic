from os import listdir, remove, devnull, system, devnull, makedirs
from os.path import isfile, join, isdir, exists, splitext
from subprocess import call, check_output, CalledProcessError, STDOUT
import shutil
import re
import sys
from optparse import OptionParser
import xml.dom.minidom as minidom
import copy
import hashlib
import time
import tempfile
import datetime
import string
from sets import Set
import math

sneakpic_path = "../build/sneakPic.com"
sneakpic_exe_path = "../build/sneakPic.exe"
inkscape_path = "inkscape"
compare_path = "compare"
get_cur_commit_num = "git rev-list --count HEAD"
get_cur_sha1 = "git rev-parse HEAD"
test_files_directory = "./test_files"
diff_directory = ""
#info xml fields:
base_name = 'file_info'
md5 = 'md5'
generator_md5 = 'generator_md5'
elapsed_time = 'time_elapsed'
info_version = 'xml_info_ver'
log_filename = 'log.html'
output_directory = './output'
minimum_for_comparison = 1e-7
cur_info_ver = 1
verbose = True
print_links = True
colorize_min_max = True
colorize_pos_neg = False
diff_in_pairs = False
print_mean_values = True
diff_splitter = re.compile('[() \t,]+')
metric_splitter = re.compile('[\t ,]+')
sum_row = []
sneakpic_md5 = ""

def md5_for_file(file_name, block_size=2**20):
  fp = open(file_name, "rb")
  md5 = hashlib.md5 ()
  while True:
    data = fp.read (block_size)
    if not data:
      break
    md5.update (data)
  fp.close ()
  return md5.hexdigest ()




def recreate_initial_info (file_name, md5_generator):
  doc = minidom.Document ()
  base = doc.createElement (base_name)
  doc.appendChild (base)

  md5_node = doc.createElement (md5)
  base.appendChild (md5_node)
  generator_md5_node = doc.createElement (generator_md5)
  base.appendChild (generator_md5_node)
  ver_node = doc.createElement (info_version)
  base.appendChild (ver_node)

  child = doc.createTextNode (md5_for_file (file_name))
  md5_node.appendChild (child)
  child = doc.createTextNode (md5_generator)
  generator_md5_node.appendChild (child)
  child = doc.createTextNode (str (cur_info_ver))
  ver_node.appendChild (child)

  return doc

def get_output_dir (renderer):
  if renderer == "sneakpic":
    return join (output_directory, "gn" + get_output (get_cur_sha1)) # n means normal
  elif renderer == "inkscape":
    return join (output_directory, "ink")
  elif renderer == "sneakpic_savedby":
    return join (output_directory, "gs" + get_output (get_cur_sha1)) # s means saved
  else:
    return join (output_directory, renderer)

def get_number_by_hash (sha1):
  try:
    try:
      return int (get_output ("git rev-list --count " + sha1))
    except ValueError:
      return -1
  except CalledProcessError:
    return -1



def get_version_number_by_renderer (renderer):
  version_number = 0
  try:
    version_number = int (renderer[:-1])
    return version_number
  except ValueError:
    return -1

def escape_path (path):
  return "\"" + path + "\""

def get_output (command):
  return check_output (command, shell=True, stderr=open(devnull, 'wb'))[:-1]

# remove sed usage before enabling it
'''
def get_hash_by_number (number):
  try:
    return get_output ("git rev-list --reverse HEAD | sed " + str (number) + "q;d")
  except CalledProcessError:
    return ""
'''

def get_previous_hash (sha1):
  try:
    out = get_output ("git rev-parse " + sha1 + "^^")
    return out
  except CalledProcessError:
    return ""

def get_full_sha1 (incomplete_sha1):
  try:
    sha1 = get_output ("git rev-parse " + incomplete_sha1)
    return True, sha1
  except CalledProcessError:
    return False, ""

def does_commit_exists_and_belongs_to_any_branch (sha1):
  try:
    full_sha1 = get_output ("git rev-parse " + sha1)
    answer = get_output ("git branch --contains " + full_sha1)
    if not answer:
      return False
    else:
      return True
  except CalledProcessError:
    return False

def get_ability_to_render (renderer):
  return (renderer == "sneakpic" or renderer == "inkscape" or renderer == "sneakpic_savedby")

def check_render_and_cache_file (renderer, file_name): # file name here without extension or dir
  global sneakpic_md5
  md5_generator = "";
  dir_name = get_output_dir (renderer);
  if renderer == "sneakpic":
    md5_generator = sneakpic_md5;
  elif renderer == "inkscape":
    pass
  elif renderer == "sneakpic_savedby":
    md5_generator = sneakpic_md5;
  else:
    md5_generator = ""

  can_rerender  = get_ability_to_render (renderer)

  if not isdir (dir_name):
    if not can_rerender:
      return
    makedirs (dir_name)
  info_file_name =  get_info_xml_path (file_name, renderer);
  svg_file_name = join (test_files_directory, file_name) + ".svg"
  png_file_name = join (dir_name, file_name) + ".png"
  svg_temp_file_name = join (tempfile.gettempdir(), "_tmp_.svg")
  regenerate = "";
  xml_doc = 0;
  if not exists (info_file_name):
    if not can_rerender:
      return
    regenerate = True
    xml_doc = recreate_initial_info (svg_file_name, md5_generator)
  else:
    supposed_md5 = get_md5 (file_name, renderer);
    supposed_generator_md5 = get_generator_md5 (file_name, renderer);
    info_version = get_info_version (file_name, renderer)
    if supposed_md5 != md5_for_file (svg_file_name)                      or \
       (md5_generator != "" and supposed_generator_md5 != md5_generator) or \
       (cur_info_ver > info_version):
      xml_doc = recreate_initial_info (svg_file_name, md5_generator)
      regenerate = True;

  inscape_string = "inkscape" + " --export-png=\"" + png_file_name + "\" \"" + svg_file_name + "\" 1>nul 2>nul"
  sneakpic_render_string = escape_path (sneakpic_path) + " --render-png=\"" + png_file_name + "\" \"" + svg_file_name + "\""
  sneakpic_save_string = escape_path (sneakpic_path) + " --save=\"" + svg_temp_file_name + "\" \"" + svg_file_name + "\""
  sneakpic_render_tmp_string = escape_path (sneakpic_path) + " --render-png=\"" + png_file_name + "\" \"" + svg_temp_file_name + "\""
  if regenerate:
    if not can_rerender:
      return
    t = time.clock ()
    if renderer == "sneakpic":
      call (sneakpic_render_string, shell=True)
    elif renderer == "inkscape":
      call (inscape_string, shell=True)
    elif renderer == "sneakpic_savedby":
      call (sneakpic_save_string, shell=True)
      call (sneakpic_render_tmp_string, shell=True)
    try:
      remove (svg_temp_file_name)
    except OSError:
      pass

    t = time.clock () - t
    base = xml_doc.childNodes[0]
    timer = xml_doc.createElement (elapsed_time)
    base.appendChild (timer)
    child = xml_doc.createTextNode (str (t))
    timer.appendChild (child)
    fp = open(info_file_name, "w")
    xml_doc.writexml (fp)
    fp.close ()
    return
  return

def print_html_header ():
  fp = open (log_filename, "a")
  fp.write ("<HTML><HEAD><link rel=\"stylesheet\" type=\"text/css\" href=\"log.css\" /><script src=\"sorttable.js\"></script><TITLE>Test Results</TITLE></HEAD><BODY><TABLE border=\"1\" class=\"sortable\">\n")
  fp.close ()

'''
def print_table_row (fp, strings):
  fp.write ("<TR>")
  emphasize_min_max = True
  min = 0.0
  max = 0.0
  try:
    min = float(strings[1])
    max = float(strings[1])
  except ValueError:
    emphasize_min_max = False

  if emphasize_min_max:
    for string in strings[1:]:
      try:
        num = float(string)
      except ValueError:
        continue
      if num < min:
        min = num
      if num > max:
        max = num

  if (math.fabs (max - min) < minimum_for_comparison):
    emphasize_min_max = False
  #print min, max
  for string in strings:
    if (emphasize_min_max):
      try:
        num = float(string)
        if (math.fabs (num - min) < minimum_for_comparison):
          fp.write ("<TD STYLE=\"background-color:LightGreen\">")
        elif (math.fabs (num - max) < minimum_for_comparison):
          fp.write ("<TD STYLE=\"background-color:LightCoral\">")
        else:
          fp.write ("<TD>")
      except ValueError:
        fp.write ("<TD>")
    else:
      fp.write ("<TD>")
    fp.write (string + "</TD>")
  fp.write ("</TR>\n")
'''

def print_html_footer ():
  fp = open (log_filename, "a")
  fp.write ("</BODY></HTML></TABLE>\n")
  fp.close ()

def write_css ():
  fp = open ("log.css", "w")
  fp.write ('''
  /* Sortable tables */
table.sortable thead {
    background-color:#eee;
    color:#666666;
    font-weight: bold;
    cursor: default;
  }''')
  fp.close ()

dict = {
        'ink': 'inkscape',
        'i': 'inkscape',
        's': 'sneakpic',
        'ss': 'sneakpic_savedby',
        'sn': 'sneakpic',
       }

def strip_aliases (x):
  x = string.lower (x)
  x = dict.get (x, x)

  if x[0] == 'g':
    res,sha1 = get_full_sha1 (x[2:])
    if res:
      x = x[:1] + sha1
    else:
      return "" # means unresolved

  if x[0] == 'p': #previous version, all after 2 symbols number
    num = 1
    if x[2:]:
      try:
        num = int (x[2:])
      except ValueError:
        return ""
    result, initial_sha1 = get_full_sha1 ("HEAD")
    if not result:
      return ""
    if num == 0:
      return "g" + x[1] + initial_sha1 #another alias for current version pn0
    cur_sha1 = initial_sha1
    while True:
      cur_sha1 = get_previous_hash (cur_sha1)
      if not cur_sha1:
        v ("Not found...")
        return ""
      v ("Scanning through " + cur_sha1 + "...")
      tested_dir = "g" + x[1] + cur_sha1
      if isdir (join (output_directory, tested_dir)):
        num -= 1
      if num == 0: # then we found it
        v ("Found!")
        return tested_dir
    return "" # then there's no such version
  return x

def v (x):
  if verbose:
    print x

def get_png_path (svg_name, renderer):
  return join (get_output_dir (renderer), svg_name[:-3] + "png")

def get_diff_path (svg_name, base_renderer, second_renderer, distance_type):
  return join (diff_directory, svg_name[:-4] + "_" + base_renderer + "_" + second_renderer + "_" + distance_type) + ".png"

def get_diff_error_path (svg_name, base_renderer, second_renderer, distance_type):
  return join (diff_directory, svg_name[:-4] + "_" + base_renderer + "_" + second_renderer + "_" + distance_type) + "_error.txt"

def get_info_xml_path (svg_name, renderer):
  return join (get_output_dir (renderer), file_name + "_info.xml")

def get_value_from_xml (svg_name, renderer, value_name):
  xml_doc = minidom.parse (get_info_xml_path (svg_name, renderer))
  try:
    value = xml_doc.getElementsByTagName(value_name)[0].childNodes[0].nodeValue;
    return value
  except IndexError:
    return ""

def get_time (svg_name, renderer):
  value = get_value_from_xml (svg_name, renderer, elapsed_time)
  try:
    return float (value)
  except ValueError:
    return 0.0

def get_md5 (svg_name, renderer):
  value = get_value_from_xml (svg_name, renderer, md5)
  return value

def get_generator_md5 (svg_name, renderer):
  value = get_value_from_xml (svg_name, renderer, generator_md5)
  return value

def get_info_version (svg_name, renderer):
  value = get_value_from_xml (svg_name, renderer, info_version)
  try:
    return int (value)
  except ValueError:
    return -1

class TableCell:
  cell_class = '' # name of difference used or time for now, either text (not searching any maximums)
  value = ''      # in case of anything comparable (by cell class should be converted to number)
  format = ''     # preferable output format
  error = False
  def __init__ (self, class_arg, value_arg, format_arg = '', error_arg = False):
    self.cell_class = class_arg
    self.value = value_arg
    self.format = format_arg
    self.error = error_arg

def get_shorted (line):
  if len (line) > 20:
    return line[:17] + "..."
  else:
    return line

class Measure:
  measure_type = ''  # type of measure like 'distance' or 'time'
  specification = '' # like type of distance, otherwise empty
  def __init__ (self, measure_type_arg, specification_arg):
    self.measure_type = measure_type_arg
    self.specification = specification_arg
  def has_two_arguments (self):
    return (self.measure_type != 'time')
  def need_link (self):
    return (self.measure_type != 'time')
  def column_header (self, base_renderer, second_renderer):
    base_renderer = get_shorted (base_renderer)
    second_renderer = get_shorted (second_renderer)
    if self.measure_type == 'time':
      return "Time by <br>" + second_renderer
    elif self.measure_type == 'distance':
      return self.specification + " (<br>" + base_renderer + ",<br> " + second_renderer + "<br>)"
    else:
      return "<unsupported>"

  def value_type (self):
    return self.measure_type + "_" + self.specification

  def cell_value_and_format (self, svg_name, base_renderer, second_renderer):
    if self.measure_type == 'distance':
      base_png_path = get_png_path (svg_name, base_renderer)
      second_png_path = get_png_path (svg_name, second_renderer)
      compare_string = compare_path + " -metric " + self.specification + " \"" + base_png_path + "\" \"" + second_png_path + "\" \"" + get_diff_path (svg_name, base_renderer, second_renderer, self.specification) + "\""
      try:
        output = check_output (compare_string, stderr=STDOUT, shell=True)
      except (OSError, CalledProcessError) as e:
        output = e.output
      num = 0.0
      format = '{0:.2f}'
      splitted = diff_splitter.split(output)
      format_string = ""
      try:
        if (self.specification == 'AE'):
          num = float (splitted[0])
          format = '{0:.0f}'
        elif (self.specification == 'FUZZ' or \
              self.specification == 'MAE'  or \
              self.specification == 'MEPP' or \
              self.specification == 'MSE'  or \
              self.specification == 'PAE'  or \
              self.specification == 'RMSE'):
          num = float (splitted[1])
          format = '{0:.7f}'
        elif self.specification == 'NCC' or self.specification == 'PSNR':
          num = float (splitted[0])
          format = '{0:.5f}'
        return num, format, False
      except ValueError:
        file = open (get_diff_error_path (svg_name, base_renderer, second_renderer, self.specification), "w")
        file.write (output)
        file.close ()
        return "Comparison Error", '', True
    elif self.measure_type == 'time':
      time = get_time (svg_name, second_renderer)
      return float (time), '{0:.2f}', False
  def link_string (self, svg_name, base_renderer, second_renderer):
    final_link_string = get_html_link_string (get_png_path (svg_name, second_renderer), "pic")
    if self.measure_type == 'distance':
      diff_path = get_diff_path (svg_name, base_renderer, second_renderer, self.specification)
      diff_err_path = get_diff_error_path (svg_name, base_renderer, second_renderer, self.specification)
      final_link_string += "<BR>" + get_html_link_string (diff_path if (exists (diff_path)) else diff_err_path, "diff")
    return final_link_string

def generate_header (base_renderer, renderers, measures, print_links_arg):
  Row = [TableCell ("text", "Checked File")]
  prev_renderer = ''
  #Then for each type of measure we generate group of columns
  for measure in measures:
    if not measure.has_two_arguments ():
      column_header = measure.column_header (base_renderer, base_renderer)
      Row += [TableCell ("text", column_header)]
      if print_links_arg and measure.need_link ():
          Row += [TableCell ("text", "Link:")]
      prev_renderer = base_renderer
      i = 1
    else:
      i = 0
    for r in renderers:
      column_header = measure.column_header (base_renderer, r)
      Row += [TableCell ("text", column_header)]
      if print_links_arg and measure.need_link ():
        Row += [TableCell ("text", "Links:", '')]
      if diff_in_pairs and i > 0:
        Row += [TableCell ("text", "Diff of <br>" + get_shorted (prev_renderer) + "<br>and<br>" + get_shorted (r))]
      i += 1
      prev_renderer = r
    if measure != measures[-1]:
        Row += [TableCell ("text", "===")] # separating header
  return Row

def get_html_link_string (linkto, contents):
  return "<a href=\"" + linkto + "\">" + contents + "</a>"

def get_cell_style (style):
  if style == 'max':
    return " STYLE=\"background-color:LightPink\" "
  elif style == 'min':
    return " STYLE=\"background-color:PaleGreen\" "
  else:
    return ""

def get_positive_negative_style (number):
  if number > 0.0:
    return " STYLE=\"color:FireBrick\" "
  elif number < 0.0:
    return " STYLE=\"color:DarkBlue \" "
  else:
    return ""

def generate_row (base_renderer, renderers, measures, file_name, print_links_arg):
  global sum_row
  Row = [TableCell ("text", get_html_link_string (join (test_files_directory, file_name), file_name) + " (" + get_html_link_string (get_png_path (file_name, base_renderer), "*") + ")")]
  #Then for each type of measure we generate group of columns
  for measure in measures:
    prev_value = 0.0
    prev_error = True
    prev_format = ''
    if not measure.has_two_arguments ():
      value, format, error = measure.cell_value_and_format (file_name, base_renderer, base_renderer)
      Row += [TableCell (measure.value_type (), value, format, error)]
      if print_links_arg and measure.need_link ():
          Row += [TableCell ("text", measure.link_string (file_name, base_renderer, base_renderer))]
      prev_value, prev_format, prev_error = value, format, error
      i = 1
    else:
      i = 0
    for r in renderers:
      value, format, error =  measure.cell_value_and_format (file_name, base_renderer, r)
      Row += [TableCell (measure.value_type (), value, format, error)]
      if print_links_arg and measure.need_link ():
        Row += [TableCell ("text", measure.link_string (file_name, base_renderer, r))]
      if diff_in_pairs and i > 0:
        diff_type = "incomparable"
        if prev_error and error:
          Row += [TableCell (diff_type, 0.0, '0:0.f', True)]
        elif prev_error:
          Row += [TableCell (diff_type, 1e10, '0:0.f', True)]
        elif error:
          Row += [TableCell (diff_type, -1e10, '0:0.f', True)]
        else:
          Row += [TableCell (diff_type, value - prev_value, format, False)]

      prev_value, prev_format, prev_error = value, format, error
      i += 1
    if measure != measures[-1]:
        Row += [TableCell ("text", "===")] # separating cell
  if not sum_row:
    sum_row = copy.deepcopy (Row)
    for table_cell in sum_row:
      if table_cell.cell_class == "text":
        table_cell.value = ""
    sum_row[0].value = "Mean Values:"
  else:
    i = 0
    for table_cell in Row:
      if table_cell.cell_class != "text" and (not table_cell.error):
        if sum_row[i].error:
          sum_row[i].error = False
          sum_row[i].value = table_cell.value
        else:
          sum_row[i].value += table_cell.value
      i += 1
  return Row

def values_eq (a, b):
  if type(a) is float and type (b) is float:
    return math.fabs(a - b) < 1e-7
  else:
    return a == b



def print_row (cells):
  fp = open (log_filename, "a")
  fp.write ("<TR>")
  minimum = {} # dictionaries
  maximum = {}
  for cell in cells:
    if (not cell.cell_class == 'text') and (not cell.cell_class == 'incomparable'):
      if (not cell.cell_class in minimum) or (minimum[cell.cell_class] > cell.value):
        minimum[cell.cell_class] = cell.value
      if (not cell.cell_class in maximum) or (maximum[cell.cell_class] < cell.value):
        maximum[cell.cell_class] = cell.value

  for cell in cells:
    style = ''
    if (not cell.cell_class == 'text') and (not cell.cell_class == 'incomparable'):
      min_val = minimum [cell.cell_class]
      max_val = maximum [cell.cell_class]

      if values_eq (min_val, max_val):
        style = ''
      elif values_eq (cell.value, min_val):
        style = get_cell_style ('min')
      elif values_eq (cell.value, max_val):
        style = get_cell_style ('max')
      else:
        style = ''

      if not colorize_min_max:
        style = ''
    if (cell.cell_class == 'incomparable' and colorize_pos_neg):
      style = get_positive_negative_style (cell.value)

    output_str = ''
    if cell.cell_class == 'text':
      output_str = cell.value
    else:
      if not cell.error:
        output_str = cell.format.format (cell.value)
      else:
        output_str = cell.value
    fp.write ("<TD" + style + ">" + output_str + "</TD>")
  fp.write ("</TR>\n")
  fp.close ()

# FUNCTIONS AND CLASS END ================================================================================================================================================================
time_spent = time.time ()
usage = "usage: %prog [options] [sha1 or program names to compare against]..."
parser = OptionParser(usage=usage)
parser = OptionParser()
write_css ()
parser.add_option("-b", "--base-renderer", dest="renderer", default="inkscape",
                  help="Version or program which will be compared against the rest")
parser.add_option("-m", "--metric", dest="metric", default="FUZZ",
                  help="Metric used by compare utility, could be one of AE, FUZZ[default], MAE, MEPP, MSE, NCC, PAE, PSNR, RMSE (could be several separated by comma)")
parser.add_option("-t", "--time", dest="time", action="store_true", default=False,
                  help="Print time of rendering in link column")
parser.add_option("-d", "--diff", dest="diff", action="store_true", default=False,
                  help="Print difference between nearest renderers in list")
parser.add_option("-v", "--verbose", dest="verbose", action="store_true", default=False,
                  help="Allow detailed output")

(options, args) = parser.parse_args()
base_renderer = options.renderer;
diff_in_pairs = options.diff
verbose = options.verbose
metric = options.metric;
metric_list = metric_splitter.split (metric)
metric_set = Set (['AE', 'FUZZ', 'MAE', 'MEPP', 'MSE', 'NCC', 'PAE', 'PSNR', 'RMSE'])
i = 0
for m in metric_list:
  if (not m) or (not m in metric_set):
    del metric_list[i]
  else:
    i += 1

if not metric_list:
  metric_list = ['FUZZ']
measure_list = []
for m in metric_list:
  measure_list += [Measure ('distance', m)]

print_time = options.time

if print_time:
  measure_list += [Measure ('time', '')]

compare_against = []

base_renderer = strip_aliases (base_renderer)
if not base_renderer:
  sys.exit ("Base renderer version cannot be resolved")

for arg in args:
  stripped = strip_aliases (arg)
  if (not stripped) or (not get_ability_to_render(stripped) and not isdir (get_output_dir (stripped))):
    print "Version " + arg + " you specified couldn't be resolved"
  else:
    compare_against += [stripped]

if not compare_against:
  compare_against += ["sneakpic"]

if not isdir (test_files_directory):
  sys.exit ("FAIL: Directory for test files: '" + test_files_directory + "' doesn't exit")

if not isdir (output_directory):
  makedirs (output_directory)

diff_directory = join (output_directory, "diff_dir")

if isdir (diff_directory):
  shutil.rmtree (diff_directory)

if not isdir (diff_directory):
  makedirs (diff_directory)

try:
  call (sneakpic_path + " --version", stdout=open(devnull, 'wb'))
except OSError:
  sys.exit ("FAIL: sneakPic.com haven't been found, please build it in utils/console_wrapper and place it to build dir")

sneakpic_md5 = md5_for_file (sneakpic_exe_path)

try:
  call ("git --version", stdout=open(devnull, 'wb'))
except OSError:
  sys.exit ("FAIL: Git is not installed or outside PATH")

try:
  call ("inkscape --version", stdout=open(devnull, 'wb'))
except OSError:
  sys.exit ("FAIL: Inkscape is not installed or outside PATH")

try:
  call ("compare --version", stdout=open(devnull, 'wb'))
except OSError:
  sys.exit ("FAIL: ImageMagick compare tool is not installed or outside PATH")

files_list = [ f for f in listdir(test_files_directory) if isfile(join(test_files_directory, f)) ]
dirs_list = [ d for d in listdir(output_directory) if isdir(join(output_directory, d)) ]

v ("Clearing diff directory...")

v ("Removing directories of non-existent commits...")
for d in dirs_list:
  if d[0] == "g":
    if not does_commit_exists_and_belongs_to_any_branch (d[2:]):
      shutil.rmtree (join (output_directory, d))
      v ("Removing directory of non-existent or out-of-any-branch commit " + d[2:])

fp = open (log_filename, "w")
fp.close ()
print_html_header ()
header_row = generate_header (base_renderer, compare_against, measure_list, print_links)
print_row (header_row)
r = re.compile('[() ,]+')
i = 0
num = 0
for file in files_list:
  file_name, file_ext = splitext(file)
  if file_ext == ".svg":
    num+=1
# main cycle
for file in files_list:
  full_path = join (test_files_directory, file)
  file_name, file_ext = splitext(file)
  if file_ext != ".svg":
    continue
  # at first we generated every picture for every renderer
  check_render_and_cache_file (base_renderer, file_name)
  for renderer in compare_against:
    check_render_and_cache_file (renderer, file_name)

  # now we should measure all stuff needed and arrange them in proper columns (for time even base should be put in separate class)
  row = generate_row (base_renderer, compare_against, measure_list, file, print_links)
  print_row (row)
  i += 1
  print "File " +  file + " has been fully processed (" + str (i) + "/" + str (num) + ")"
for table_cell in sum_row:
  if table_cell.cell_class != "text" and (not table_cell.error):
    table_cell.value /= num
if print_mean_values:
  print_row (sum_row)
print_html_footer ()
fp.close ()
time_spent = time.time () - time_spent
print "--------"
print str (datetime.timedelta(seconds=time_spent)) + " was spent on comparison"