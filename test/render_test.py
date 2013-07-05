from os import listdir, remove, devnull, system, devnull
from os.path import isfile, join, isdir, exists, splitext
from subprocess import call, check_output, CalledProcessError, STDOUT
import shutil
import re

test_files_directory = "./test_files"
sneakpic_path = "../build/sneakPic.exe"
compare_path = "compare"

if not isdir (test_files_directory):
	print "FAIL: Directory for test files: '" + test_files_directory + "' doesn't exit"
	exit

if not exists (sneakpic_path):
	print "FAIL: sneakPic release version was not built"
	exit

try:
	call ("inkscape --version", stdout=open(devnull, 'wb'))
except OSError:
	print "FAIL: Inkscape is not installed or outside PATH"
	exit

try:
	call ("compare --version", stdout=open(devnull, 'wb'))
except OSError:
	print "FAIL: ImageMagick compare tool is not installed or outside PATH"
	exit


files_list = [ f for f in listdir("./test_files") if isfile(join("./test_files",f)) ]
# cleaning all pngs
for file in files_list:
	file_name, file_ext = splitext(file)
	if file_ext == ".png":
		try:
			remove (join (test_files_directory, file))
		except OSError:
			pass
fp = open("log.txt", "w")
fp.write ("File Name\tDifference\n")
fp.close ()
r = re.compile('[() ]+')
# main cycle
for file in files_list:
	full_path = join (test_files_directory, file)
	file_name, file_ext = splitext(full_path)
	if file_ext != ".svg":
		continue
	file_png = file_name + ".png"
	file_png_ink = file_name + "__ink.png"
	file_difference = file_name + "_diff.png"
	try:
		call ([sneakpic_path, full_path])
	except OSError:
		print "File wasn't processed by sneakPic"
		continue;
	inscape_string = "inkscape " + "--export-png=\"" + file_png_ink + "\" \"" + full_path + "\" 1>nul 2>nul"

	#call (["inkscape.com", "--export-png=\"" + file_png_ink + "\" \"" + file + "\""])
	#sadly inscape run only in system console
	call (inscape_string, shell=True)

	compare_string = compare_path + " -metric FUZZ \"" + file_png + "\" \"" + file_png_ink + "\" \"" + file_difference + "\""
	output = ""
	try:
		output = check_output (compare_string, stderr=STDOUT, shell=True)
	except (OSError, CalledProcessError) as e:
		output = e.output
	print "Result for file " + file + " is:"
	print output
	fp = open("log.txt", "a")
	splitted = r.split(output)
	fp.write (file + "\t")
	for piece in splitted[:-1]:
		fp.write (piece + "\t")
	fp.write(splitted[-1] + "\n")
	fp.close ();
	'''
	try:
		remove (file_difference)
	except OSError:
		pass
	try:
		remove (file_png_ink)
	except OSError:
		pass
	try:
		remove (file_png)
	except OSError:
		pass
	'''
