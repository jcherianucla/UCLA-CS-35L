#!/usr/bin/python
""""
This file was created by Jahan Kuruvilla Cherian on 01/21/16.
This file runs the POSIX command comm to compare two files and output
the differences and similarities in the files. This modified version also
supports unsorted comparisons.
"""
import sys, string, locale
from optparse import OptionParser

class compare:
	def __init__(self,filename1, filename2):
		try:
			if filename1 == "-":
				f1 = sys.stdin
				f2 = open(filename2, 'r')
			elif filename2 == "-":
				f2 = sys.stdin
				f1 = open(filename1, 'r')
			elif filename1 == "-" and filename2 == "-":
				print("Error! Can't read both files from STDIN")
				exit()
			else:
				f1 = open(filename1, 'r')
				f2 = open(filename2, 'r')
			self.lines1 = f1.read().split('\n')
			self.lines2 = f2.read().split('\n')
			del self.lines1[len(self.lines1)-1]
			del self.lines2[len(self.lines2)-1]
			self.list1 = []
			self.list2 = []
			self.list3 = []
			f1.close()
			f2.close()
		except IOError as e:
			errno = e.errno
			strerror = e.strerror
			parser.error("I/O error({0}): {1}".
						  format(errno, strerror))

	def add_Newline(self, file):
		for i in range(len(file)):
			if file[i] == '':
				file[i] = '\n'
			if file[i].count(' ') >= 1 and file[i].isspace():
			  	file[i] = '\n' * file[i].count(' ')

	def modify(self):
		self.add_Newline(self.lines1)
		self.add_Newline(self.lines2)

	def u_comparison(self):
		self.modify()
		for i in range(len(self.lines1)):
			for j in range(len(self.lines2)):
				if self.lines1[i] == self.lines2[j]:
					self.list3.append(self.lines1[i])
					self.list1.append("        ")
					self.list2.append("        ")
					del self.lines2[j]
					similar = True
					break
				else:
				  similar = False
			if similar == False:
				self.list1.append(self.lines1[i])
				self.list2.append("")
				self.list3.append("")
		extra_spaces = ["        "] * len(self.lines2)
		extra_null = [""] * len(self.lines2)
		self.list1 += extra_spaces
		self.list3 += extra_null
		self.list2 += self.lines2

	def is_Sorted(self, file, file_num):
		for i in range(len(file) - 1):
			if locale.strcoll(file[i],file[i+1]) > 0:
				print ("File %s is not sorted") % file
				exit()
		return True

	def s_comparison(self):
		if self.is_Sorted(self.lines1, 1) and self.is_Sorted(self.lines2, 2):
			self.modify()
			i = j = 0
			while i < len(self.lines1) and j < len(self.lines2):
				if self.lines1[i] == self.lines2[j]:
					self.list3.append(self.lines1[i])
					self.list1.append("        ")
					self.list2.append("        ")
					self.lines1[i] = ''
					self.lines2[j] = ''
					i += 1
					j += 1
				elif self.lines1[i] > self.lines2[j]:
					self.list2.append(self.lines2[j])
					self.list1.append("        ")
					self.list3.append("")
					self.lines2[j] = ''
					j += 1
				elif self.lines1[i] < self.lines2[j]:
					self.list1.append(self.lines1[i])
					self.list2.append("")
					self.list3.append("")
					self.lines1[i] = ''
					i +=1
			if i > j: 
			  extra_spaces = ["        "] * len(self.lines2)
			  extra_null = [""] * len(self.lines2)
			  self.list2 += self.lines2
			  self.list1 += extra_spaces
			  self.list3 += extra_null
			elif i < j:
			  extra_spaces = [""] * len(self.lines1)
			  self.list1 += self.lines1
			  self.list2 += extra_spaces
			  self.list3 += extra_spaces


	def printer(self, option1, option2, option3 ):
		if option1 == True:
			self.list1 = [''] * len(self.list1)
		if option2 == True:
			self.list2 = [''] * len(self.list2)
		if option3 == True:
			self.list3 = [''] * len(self.list3)
		final_list = []
		for i in range(len(self.list1)):
			final_list.append(self.list1[i] + self.list2[i] + self.list3[i])
		for j in range(len(final_list)):
			for k in range(len(final_list[j])):
				if final_list[j][k] != " ":
				  if final_list[j][k] == "\n":
					  str = final_list[j]
					  for i in range(len(str)):
						  if str[i] == '\n':
							  sys.stdout.write(" ")
						  else:
							  sys.stdout.write(str[i])
					  print('')
					  break
				  else:
					  print(final_list[j])
					  break

def main():
	locale.setlocale(locale.LC_ALL, 'C')
	version_msg = "%prog 1.0"
	usage_msg = """%prog [OPTION]... FILE1 FILE2

	Output the comparison results between FILE1 and FILE2 in three columns."""

	parser = OptionParser(version=version_msg, usage=usage_msg)
	parser.add_option("-1", action="store_true", dest="sup1",
					  default = False,
					  help="suppress column 1 (lines unique to FILE1)")
	parser.add_option("-2", action="store_true", dest="sup2",
					  default = False,
					  help="suppress column 2 (lines unique to FILE2)")
	parser.add_option("-3", action="store_true", dest="sup3",
					  default = False,
					  help="suppress column 3 (lines that appear in both files)")
	parser.add_option("-u", action="store_true", dest="unsort",
					  default = False,
					  help="run comparison on unsorted files line by line." )
	options, args = parser.parse_args(sys.argv[1:])
	try:
		sup1 = bool(options.sup1)
		sup2 = bool(options.sup2)
		sup3 = bool(options.sup3)
		unsort = bool(options.unsort)
	except:
		parser.error("invalid option type: {0}".
					format(options.sup1))
	if len(args) != 2:
		parser.error("missing option arguments")
	input_file1 = args[0]
	input_file2 = args[1]
	try:
		comparator = compare(input_file1, input_file2)
		if unsort:
			comparator.u_comparison()
		else:
			comparator.s_comparison()
		comparator.printer(sup1, sup2, sup3)
	except IOError as e:
		errno = e.errno
		strerror = e.strerror
		parser.error("I/O error({0}): {1}".
					  format(errno, strerror))

if __name__ == "__main__":
	main()
