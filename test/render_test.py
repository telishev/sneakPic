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
  if renderer == "sneakPic":
    dir_name = join (test_files_directory, get_output (get_cur_commit_num) + "n") # n means normal
    md5_generator = md5_for_file (sneakpic_path);
  elif renderer == "Inkscape":
    dir_name = join (test_files_directory, "ink") #TODO maybe make it depending on inkscape current version
  elif renderer == "sneakPic_savedby":
    dir_name = join (test_files_directory, get_output (get_cur_commit_num) + "s") # s means saved
    md5_generator = md5_for_file (sneakpic_path);
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
    if renderer == "sneakPic":
      call (sneakpic_render_string, shell=True)
    elif renderer == "Inkscape":
      call (inscape_string, shell=True)
    elif renderer == "sneakPic_savedby":
      call (sneakpic_save_string, shell=True)
      call (sneakpic_render_tmp_string, shell=True)
      remove (svg_temp_file_name)
      
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
  for string in strings:
    fp.write ("<TD>" + string + "</TD>")
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
(options, args) = parser.parse_args()
base_renderer = options.renderer;
compare_against = args;
if not compare_against:
  compare_against = ["Inkscape"]

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
  column_names.append ("FUZZ (" +base_renderer + ", " + r + ")")
print_table_row (fp, column_names)
r = re.compile('[() ]+')
# main cycle
for file in files_list:
  full_path = join (test_files_directory, file)
  file_name, file_ext = splitext(file)
  if file_ext != ".svg":
    continue
  base_png_name = check_render_and_cache_file (base_renderer, file_name)
  png_names = []
  if not base_png_name == "":
    column_strings = ["<a href=\""+full_path+"\">"+ file +"</a>" + "(<a href=\"" + base_png_name + "\">*</a>)"]
  else:
    column_strings = [""]
  for renderer in compare_against:
    png_name = check_render_and_cache_file (renderer, file_name)
    if base_png_name == "":
      column_strings += ["Base File Generation Error"]
    if png_name == "":
      column_strings += ["Target File Generation Error"]
    file_difference = join (diff_directory, file_name) + ".png"
    compare_string = compare_path + " -metric FUZZ \"" + base_png_name + "\" \"" + png_name + "\" \"" + file_difference + "\""
    output = ""
    try:
      output = check_output (compare_string, stderr=STDOUT, shell=True)
    except (OSError, CalledProcessError) as e:
      output = e.output
    splitted = r.split(output)
    try:
        float(splitted[1])
        column_strings += [splitted[1] + "(<a href=\"" + png_name + "\">*</a>)"]
    except ValueError:
        column_strings += ["Comparison error"]
  print_table_row (fp, column_strings)
  fp.close ()
  fp = open (log_filename, "a")
  print file + " comparison finished"
print_html_footer (fp)
fp.close ()
time_spent = time.time () - time_spent
print "--------"
print str (datetime.timedelta(seconds=time_spent)) + " was spent on comparison"