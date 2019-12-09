#!/usr/bin/python
# comm.py

"""
This program behaves like the UNIX shell command comm. It has an added option '-u'.
This option allows for comparisons between unsorted files. This is done by firstly
going through all the lines in the first file and comparing them to lines of the
second file. These lines are then printed appropriately. Lastly the remaining lines
unique to the second file are printed in the second column.


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

"""

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

def getTabs(num):
    s = ""
    for x in range(0, num):
        s = s + "\t"
    return s

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Equivalent to comm UNIX command. Has added -u option. Read on for further info."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", "--del1uniq", default=False, action="store_true", 
                      dest="opt1",
                      help="Suppress output unique to file1")
    parser.add_option("-2", "--del2uniq", default=False, action="store_true", 
                      dest="opt2",
                      help="Suppress output unique to file2")
    parser.add_option("-3", "--delcomm", default=False, action="store_true", 
                      dest="common",
                      help="Suppress output common to both files")
    parser.add_option("-u", "--unsorted", default=False, action="store_true", 
                      dest="unsorted",
                      help="This option allows for comparisons between unsorted files. This is done by firstly going through all the lines in the first file and comparing them to lines of the second file. These lines are then printed appropriately. Lastly the remaining lines unique to the second file are printed in the second column.")

    options, args = parser.parse_args(sys.argv[1:])
    try:
        opt1     = options.opt1
        opt2     = options.opt2
        common   = options.common
        unsorted = options.unsorted
    except:
        parser.error("Invalid option format.")
    
    try:    
        file1 = args[0]
        file2 = args[1]
    except:
        parser.error("Must specify 2 file names (or - for STDIN). Only one file can be redirected to STDIN.")

    # try to read file content
    try:
        # if they both expect input from STDIN raise exception
        if file1 == "-" and file2 == "-":
            raise Exception()

        # if file1 is not STDIN, read content
        if file1 != "-":
            with open(file1) as open_file1:
                list1 = open_file1.read().splitlines();
        else:
            list1 = sys.stdin.read().splitlines();

        # if file2 is not STDIN, read content
        if file2 != "-":
            with open(file2) as open_file2:
                list2 = open_file2.read().splitlines();
        else:
            list2 = sys.stdin.read().splitlines();

    except IOError:
        parser.error("Problem reading file content.")

    tabList = [0, 0, 0, 0]  # specifies number of tabs for printing each column
    tabList[0] = -1;        # 0 and 1 index will not be used
    tabList[1] = -1;

    # sort out content of tabList
    if opt1:
        tabList[2] = 0;
    else:
        tabList[2] = 1;

    if opt2:
        tabList[3] = tabList[2];
    else:
        tabList[3] = tabList[2] + 1;

    # if '-u' is not active
    if not unsorted:

        line2Count = 0;
        for line1 in list1:

            # go through file2 as long as lines are smaller than curr file1 line
            while line2Count < len(list2) and list2[line2Count] < line1:
                # print files unique to file2
                if not opt2:
                    sys.stdout.write("%s%s\n" % (getTabs(tabList[2]), list2[line2Count]))
                line2Count = line2Count + 1

            if line2Count < len(list2):
                # if current lines are the same
                if list2[line2Count] == line1:
                    if not common:
                        sys.stdout.write("%s%s\n" % (getTabs(tabList[3]), line1))
                    line2Count = line2Count + 1;
                # if not the same, print line unique to file1
                elif list2[line2Count] != line1 and (not opt1):
                    sys.stdout.write("%s\n" % (line1))
            # if all list2 lines are exhausted, print list1 lines
            elif (not opt1):
                    sys.stdout.write("%s\n" % (line1))

        # print all lines left over in list2, if any
        while line2Count < len(list2):
            if not opt2:
                sys.stdout.write("%s%s\n" % (getTabs(tabList[2]), list2[line2Count]))
            line2Count = line2Count + 1


    # if '-u' is active
    else:
        for line1 in list1:
            for line2Count in range(0, len(list2)):
                # if match found, print appropriately, delete that line from list2
                # exit inner for loop and iterate line1
                if line1 == list2[line2Count]:
                    if not common:
                        sys.stdout.write("%s%s\n" % (getTabs(tabList[3]), line1))
                    list2.pop(line2Count)
                    break
            # if break is not called in for, then line1 is unique to file1
            # print accordingly
            else:
                if not opt1:
                    sys.stdout.write("%s\n" % (line1))

        # print everything left in list2
        for line2Count in range(0, len(list2)):
            if not opt2:
                sys.stdout.write("%s%s\n" % (getTabs(tabList[2]), list2[line2Count]))

if __name__ == "__main__":
    main()
