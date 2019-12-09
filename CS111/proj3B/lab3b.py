'''
Project 3B
lab3b.py
------------
NAME: Nikola Samardzic
EMAIL: nikola.s@ucla.edu
ID: 904799099
'''

import sys
import math


if len(sys.argv) != 2:
    sys.exit(1)
    
input_file = sys.argv[1]

blocks = {}   # dictionary made up of 'block ptr' : 'block object list'
              # key-value pairs. If a block is on the free list, add None to
              # block object list. All valid blocks are initialized to empty
              # lists when the Superblock object is created
inodes = {}   # same as blocks but for inodes.
superblock = None # superblock object
group = None      # group object
min_block = -1    # biggest block index accessible to user mode
max_block = -1    # smallest block index accessible to user mode
valid_inodes = [] # list of all valid inodes
dir_children = {}    # given a key that is an inode of a directory, this
                     # gives a list of all its direntries
                     
dir_entries = {}     # given a file inode, gives list of all direntries
                     # that reference it
corrupt = False      # specifies if the disk is corrupt
     

def report_inonsistent_dot(key, parent):
    global corrupt
    corrupt = True
    print('DIRECTORY INODE ' + str(key) \
          + " NAME '.' LINK TO INODE " \
          + str(parent) + " SHOULD BE " \
          + str(key))                    

def report_inconsistent_dotdot(key, file_inode, parent_dir):
    global corrupt
    corrupt = True    
    print('DIRECTORY INODE ' + str(key) \
          + " NAME '..' LINK TO INODE " \
          + str(file_inode) + " SHOULD BE " \
          + str(parent_dir))

def report_bad_linkcount(key):
    global corrupt
    corrupt = True    
    print('INODE ' + str(key) + ' HAS ' \
          + str(len(dir_entries[key])) \
          + ' LINKS BUT LINKCOUNT IS ' \
          + str(inodes[key][0].link_count))


def report_unallocated_reference(key, direntry):
    global corrupt
    corrupt = True    
    print('DIRECTORY INODE ' + str(key) + ' NAME ' \
          + direntry.name + ' UNALLOCATED INODE ' \
          + str(direntry.file_inode))
                
def report_invalid_reference(key, direntry):
    global corrupt
    corrupt = True    
    print('DIRECTORY INODE ' + str(key) + ' NAME ' \
          + direntry.name + ' INVALID INODE ' \
          + str(direntry.file_inode))    
                
def report_unallocated_inode(key):
    global corrupt
    corrupt = True    
    print('UNALLOCATED INODE ' + str(key) + ' NOT ON FREELIST')

    
class Superblock(object):
    def __init__(self, block_count, inode_count, block_size, inode_size, \
                 first_non_reserved_inode, first_inode_block):
        global single_offset, double_offset, triple_offset
        global max_block, min_block, inodes, inode_block_count
        # initialize valid inodes to empty list
        inodes[2] = []   # special case: home directory inode
        for i in range(first_non_reserved_inode, inode_count+1):
            inodes[i] = []

        inode_block_count = int(math.ceil(float(inode_count)\
                                    *inode_size/block_size))
        min_block = first_inode_block + inode_block_count
        max_block = block_count - 1
        for i in range(min_block, block_count):
            blocks[i] = []

        block_ids_per_block = block_size / 4
        double_offset = 12 + block_ids_per_block
        triple_offset = double_offset \
                        + block_ids_per_block * block_ids_per_block
        # create list of valid inodes
        valid_inodes.append(2)
        for i in range(first_inode_block, inode_count):
            valid_inodes.append(i)
            
class Block(object):

    def __init__(self, uid, referenced_from, logical_offset, indirection):
        self.uid = uid
        self.referenced_from = referenced_from
        self.logical_offset = logical_offset
        self.indirection = indirection
        if self.is_user_available():
            blocks[uid].append(self)
            
    def is_valid(self):
        if self.uid < 0 or self.uid > max_block:
            global corrupt
            corrupt = True
            print('INVALID ' + self.indirection + 'BLOCK ' \
                  + str(self.uid) + ' IN INODE ' \
                  + str(self.referenced_from) \
                  + ' AT OFFSET ' + str(self.logical_offset))
            return False
        return True

    def is_reserved(self):
        if self.uid < min_block:
            global corrupt
            corrupt = True
            print('RESERVED ' + self.indirection + 'BLOCK ' \
                  + str(self.uid) + ' IN INODE ' \
                  + str(self.referenced_from) + ' AT OFFSET '\
                  + str(self.logical_offset))
            return True
        return False

    def is_user_available(self):
        if self.uid == 0:
            return False
        if self.is_valid() and not self.is_reserved():
            return True
        return False

        
class DirectoryEntry(object):
    
    def __init__(self, parent_inode, logical_offset, file_inode, name):
        self.parent_inode = parent_inode
        self.logical_offset = logical_offset
        self.file_inode = file_inode
        self.name = name
        self.add_to_dicts()

    def add_to_dicts(self):
        if not self.file_inode in dir_entries:
            dir_entries[self.file_inode] = []
        if not self.parent_inode in dir_children:
            dir_children[self.parent_inode] = []                
        dir_entries[self.file_inode].append(self)
        dir_children[self.parent_inode].append(self)

        
class Inode(object):
    
    def __init__(self, uid, link_count, my_blocks):
        self.uid = uid
        self.link_count = link_count
        self.my_blocks = my_blocks
        self.add_blocks()

    def add_blocks(self):
        log_offset = 0
        for block in self.my_blocks[:12]:
             Block(block, self.uid, log_offset, "")
             log_offset+=1             
        Block(self.my_blocks[12], self.uid, 12, "INDIRECT ")
        Block(self.my_blocks[13], self.uid, double_offset,
              "DOUBLE INDIRECT ")
        Block(self.my_blocks[14], self.uid, triple_offset, \
              "TRIPLE INDIRECT ")
        inodes[self.uid].append(self)
        dir_entries[self.uid] = dir_entries.get(self.uid, [])

def process_csv():
    # read input
    with open(input_file, 'r') as fd:
        sb_params = []
        for line in fd:
            while line[-1] == '\n':
                line = line[:-1]
            
            tokens = line.split(',')            
            line_id = tokens[0]
            if line_id == 'SUPERBLOCK':
                sb_params = [int(tokens[1]), int(tokens[2]),
                             int(tokens[3]), int(tokens[4]),
                             int(tokens[7])]
            elif line_id == 'GROUP':
                superblock = Superblock(sb_params[0], sb_params[1], \
                                    sb_params[2], sb_params[3], \
                                    sb_params[4], int(tokens[8]))
            elif line_id == 'BFREE':
                block_id = int(tokens[1])
                blocks[block_id].append(None)
            elif line_id == 'IFREE':
                inode_id = int(tokens[1])
                inodes[inode_id].append(None)
            elif line_id == 'INODE':
                Inode(int(tokens[1]),
                      int(tokens[6]),
                      map(lambda x: int(x), tokens[12:12+15]))
            elif line_id == 'DIRENT':
                DirectoryEntry(int(tokens[1]), int(tokens[2]),
                               int(tokens[3]), tokens[6])
            elif line_id == 'INDIRECT':
                indirection = ''
                indir = int(tokens[2])
                if indir == 1:
                    indirection = 'INDIRECT '
                elif indir == 2:
                    indirection = 'DOUBLE INDIRECT '
                else:
                    indirection = 'TRIPLE INDIRECT '                    
                Block(int(tokens[5]), int(tokens[2]),
                      int(tokens[3]), indirection)

                
def check_blocks():
    global corrupt
    for key in blocks.keys():
        # print unreferenced valid blocks        
        if blocks[key] == []:
            corrupt = True
            print('UNREFERENCED BLOCK ' + str(key))
        # print duplicate / allocated nodes on free list
        elif len(blocks[key]) > 1:
            # if on free list and allocated somewhere
            if None in blocks[key]:
                corrupt = True
                print('ALLOCATED BLOCK ' + str(key) + ' ON FREELIST')
            else:
                # check if the parent of all the nodes is the same
                parent_list = map(lambda x: x.referenced_from,
                                  blocks[key])
                matching_parents = filter(lambda x: x == parent_list[0],
                                          parent_list)
                # if the block is referenced from only one inode
                if len(matching_parents) == len(parent_list):
                    continue
                # duplicate
                for elem in blocks[key]:
                    corrupt = True
                    print('DUPLICATE ' + elem.indirection + 'BLOCK ' \
                          + str(key) + ' IN INODE ' \
                          + str(elem.referenced_from) + ' AT OFFSET ' \
                          + str(elem.logical_offset))

    
def check_dir_entries():
    for key in dir_entries.keys():
        # if invalid inode key, don't check
        if key not in valid_inodes or None in inodes[key] \
           or len(inodes[key]) != 1:
            continue
        if len(dir_entries[key]) != inodes[key][0].link_count:
            report_bad_linkcount(key)

    for key in dir_children.keys():
        for direntry in dir_children[key]:
            if direntry.name == "'.'":
                if direntry.file_inode != key:
                    report_inconsistent_dot(key, direntry.parent_inode)
            if direntry.name == "'..'":
                parent_dir = dir_entries[direntry.parent_inode][0]\
                             .parent_inode
                if direntry.file_inode != parent_dir:
                    report_inconsistent_dotdot(key, direntry.file_inode, \
                                               parent_dir)
    for key in dir_children.keys():
        for direntry in dir_children[key]:
            # inode associated with direntry is invalid
            if direntry.file_inode not in valid_inodes:
                report_invalid_reference(key, direntry)
            elif inodes[direntry.file_inode] == [None]:
                report_unallocated_reference(key, direntry)
                
def check_inodes():
    global corrupt
    for key in inodes.keys():
        # print unreferenced valid inodes        
        if inodes[key] == []:
            report_unallocated_inode(key)

        elif len(inodes[key]) > 1:
            corrupt = True
            print('ALLOCATED INODE ' + str(key) + ' ON FREELIST')


def main():
    try:
        open(input_file, 'r').close()
    except IOError:
        print >> sys.stderr, "ERROR while opening file"
        sys.exit(1)
    process_csv()
    check_blocks()
    check_inodes()
    check_dir_entries()
    if corrupt:
        sys.exit(2)


if __name__ == "__main__":
    main()
