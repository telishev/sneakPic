from os import listdir, remove, devnull, system, devnull, makedirs
from os.path import isfile, join, isdir, exists, splitext
from subprocess import call, check_output, CalledProcessError, STDOUT
import shutil
import re
import sys
from optparse import OptionParser
import xml.dom.minidom as minidom
import hashlib
import time
import tempfile
import datetime
import string
from sets import Set
import math

sneakpic_path = "../build/sneakPic.exe"
inkscape_path = "inkscape"
compare_path = "compare"
get_cur_commit_num = "git rev-list --count HEAD"
test_files_directory = "./test_files"
diff_directory = "./diff_dir"
#info xml fields:
base_name = 'file_info'
md5 = 'md5'
generator_md5 = 'generator_md5'
elapsed_time = 'time_elapsed'
log_filename = 'log.html'
minimum_for_comparison = 1e-7

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

sneakpic_md5 = md5_for_file (sneakpic_path)
 
def recreate_initial_info (file_name, md5_generator):
  doc = minidom.Document ()
  base = doc.createElement (base_name)
  doc.appendChild (base)  
  md5_node = doc.createElement (md5)
  base.appendChild (md5_node)
  generator_md5_node = doc.createElement (generator_md5)
  base.appendChild (generator_md5_node)
  child = doc.createTextNode (md5_for_file (file_name))
  md5_node.appendChild (child)
  child = doc.createTextNode (md5_generator)
  generator_md5_node.appendChild (child)
  return doc

def escape_path (path):
  return "\"" + path + "\""
 
def get_output (command):
  return check_output (command, shell=True)[:-1]

def check_render_and_cache_file (renderer, file_name): # file name here without extension or dir
  md5_generator = "";
  can_rerender = True;
  if renderer == "sneakpic":
    dir_name = join (test_files_directory, get_output (get_cur_commit_num) + "n") # n means normal
    md5_generator = sneakpic_md5;
  elif renderer == "inkscape":
    dir_name = join (test_files_directory, "ink") #TODO maybe make it depending on inkscape current version
  elif renderer == "sneakpic_savedby":
    dir_name = join (test_files_directory, get_output (get_cur_commit_num) + "s") # s means saved
    md5_generator = sneakpic_md5;
  else:
    dir_name = join (test_files_directory, renderer) # s means saved
    md5_generator = ""
    can_rerender = False
  
  if not isdir (dir_name):
    if not can_rerender:
      return ""
    makedirs (dir_name)
  info_file_name =  join (dir_name, file_name + "_info.xml");
  svg_file_name = join (test_files_directory, file_name) + ".svg"
  png_file_name = join (dir_name, file_name) + ".png"
  svg_temp_file_name = join (tempfile.gettempdir(), "_tmp_.svg")
  regenerate = "";
  xml_doc = 0;
  if not exists (info_file_name):
    regenerate = True
    xml_doc = recreate_initial_info (svg_file_name, md5_generator)
  else:
    xml_doc = minidom.parse (info_file_name)
    supposed_md5 = xml_doc.getElementsByTagName(md5)[0].childNodes[0].nodeValue;
    supposed_generator_md5 = ""
    if xml_doc.getElementsByTagName(generator_md5)[0].childNodes:
      supposed_generator_md5 = xml_doc.getElementsByTagName(generator_md5)[0].childNodes[0].nodeValue;
    if supposed_md5 != md5_for_file (svg_file_name) or (md5_generator != "" and supposed_generator_md5 != md5_generator):
      xml_doc = recreate_initial_info (svg_file_name, md5_generator)
      regenerate = True;
      
  inscape_string = "inkscape" + " --export-png=\"" + png_file_name + "\" \"" + svg_file_name + "\" 1>nul 2>nul"
  sneakpic_render_string = escape_path (sneakpic_path) + " --render-png=\"" + png_file_name + "\" \"" + svg_file_name + "\""
  sneakpic_save_string = escape_path (sneakpic_path) + " --save=\"" + svg_temp_file_name + "\" \"" + svg_file_name + "\""
  sneakpic_render_tmp_string = escape_path (sneakpic_path) + " --render-png=\"" + png_file_name + "\" \"" + svg_temp_file_name + "\""
  if regenerate:
    if not can_rerender:
      return ""
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
    return png_file_name
  return png_file_name

def print_html_header (fp):
  fp.write ("<HTML><HEAD><link rel=\"stylesheet\" type=\"text/css\" href=\"log.css\" /><script src=\"sorttable.js\"></script><TITLE>Test Results</TITLE></HEAD><BODY><TABLE class=\"sortable\">\n")
  
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
  
def print_html_footer (fp):
  fp.write ("</BODY></HTML></TABLE>\n")
  
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
    x = get_output ("git rev-list --count " + x[2:]) + x[1:2]
  return x
  
# FUNCTIONS END ================================================================================================================================================================
if not isdir (diff_directory):
  makedirs (diff_directory)
time_spent = time.time ()
usage = "usage: %prog [options] [version or program names to compare against]..."
parser = OptionParser(usage=usage)
parser = OptionParser()
write_css ()
parser.add_option("-b", "--base-renderer", dest="renderer", default="sneakPic",
                  help="Version or program which will be compared against the rest")
parser.add_option("-m", "--metric", dest="metric", default="FUZZ",
                  help="Metric used by compare util, could be one of AE, FUZZ[default], MAE, MEPP, MSE, NCC, PAE, PSNR, RMSE")
(options, args) = parser.parse_args()
base_renderer = options.renderer;
metric = options.metric;
metric_set = Set (['AE', 'FUZZ', 'MAE', 'MEPP', 'MSE', 'NCC', 'PAE', 'PSNR', 'RMSE'])
if not metric in metric_set:
  metric = 'FUZZ'
  
compare_against = [];

base_renderer = strip_aliases (base_renderer)
for arg in args:
  compare_against.append (strip_aliases (arg))

if not compare_against:
  compare_against = ["inkscape"]

if not isdir (test_files_directory):
  sys.exit ("FAIL: Directory for test files: '" + test_files_directory + "' doesn't exit")

try:
  call (sneakpic_path + " --version", stdout=open(devnull, 'wb'))
except OSError:
  sys.exit ("FAIL: sneakPic release version was not built")
  
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


files_list = [ f for f in listdir("./test_files") if isfile(join("./test_files",f)) ]
# cleaning all pngs
fp = open (log_filename, "w")
print_html_header (fp)
column_names = [""]
for r in compare_against:
  column_names.append (metric + " (" +base_renderer + ", " + r + ")")
  column_names.append ("")
print_table_row (fp, column_names)
r = re.compile('[() ,]+')
diff_sum = [0.0] * len (compare_against)
diff_count = [0] * len (compare_against)
# main cycle
for file in files_list:
  full_path = join (test_files_directory, file)
  file_name, file_ext = splitext(file)
  if file_ext != ".svg":
    continue
  base_png_name = check_render_and_cache_file (base_renderer, file_name)
  png_names = []
  column_strings = ["<a href=\""+full_path+"\">"+ file +"</a>"]
  if not base_png_name == "":
    column_strings[0] += "(<a href=\"" + base_png_name + "\">*</a>)"
  i = -1
  for renderer in compare_against:
    i +=  1
    link_text = "-"
    png_name = check_render_and_cache_file (renderer, file_name)
    if base_png_name == "":
      column_strings += ["Base File Generation Error"]
      column_strings += [link_text]
      continue
    if png_name == "":
      column_strings += ["Target File Generation Error"]
      column_strings += [link_text]
      continue
    file_difference = join (diff_directory, file_name) + ".png"
    compare_string = compare_path + " -metric " + metric + " \"" + base_png_name + "\" \"" + png_name + "\" \"" + file_difference + "\""
    output = ""
    try:
      output = check_output (compare_string, stderr=STDOUT, shell=True)
    except (OSError, CalledProcessError) as e:
      output = e.output
      
    # print output
    link_text = "<a href=\"" + png_name + "\">Link</a>"
    splitted = r.split(output)
    if (len (splitted) == 0):
      column_strings += ["Comparison error"]
      column_strings += [link_text]
      continue;
      
    try:
        num = float(splitted[0])
        format_string = '{0:.2f}'
        if (metric == 'AE'):
          format_string = '{0:.0f}'
        elif (metric == 'FUZZ' or metric == 'MAE' or metric == 'MEPP' or metric == 'MSE' or metric == 'PAE' or metric == 'RMSE'):
          format_string = '{0:.7f}'
          num = float(splitted[1])
        elif (metric == 'NCC' or metric == 'PSNR'):
          format_string = '{0:.5f}'
          
        diff_sum[i] += num
        diff_count[i] += 1
        
        column_strings += [format_string.format (num)]
        column_strings += [link_text]
    except ValueError:
        column_strings += ["Comparison error"]
        column_strings += [link_text]
  print_table_row (fp, column_strings)
  fp.close ()
  fp = open (log_filename, "a")
  print file + " comparison finished"
column_strings = ["Mean value (excluding errors)"]
i = 0
for sum in diff_sum:
  column_strings += [sum / diff_count[i]]
  i += 1
print_table_row (fp, column_strings)
print_html_footer (fp)
fp.close ()
time_spent = time.time () - time_spent
print "--------"
print str (datetime.timedelta(seconds=time_spent)) + " was spent on comparison"