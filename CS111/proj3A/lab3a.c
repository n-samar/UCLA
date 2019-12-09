/********************************************************************
 Project 3A
 lab3a.c
------------|
 NAME: Nikola Samardzic
 EMAIL: nikola.s@ucla.edu
 ID: 904799099
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "ext2_fs.h"

#define SUPERBLOCK_SIZE 1024
#define SUPERBLOCK_OFFSET 1024
#define INODE_SIZE 128

#define ERROR_EXIT 1
#define SUCCESS_EXIT 0


__u32 block_ids_per_block;
unsigned int block_size;
int disk_fd;
struct ext2_super_block sb;
struct ext2_inode *inode_a;
struct ext2_group_desc group_desc;

void printUsage() {
    fprintf (stderr, "lab3a: Improper usage. Takes exactly one command line argument.\n");
}


char get_mode(__u16 mode) {
    switch (mode & 0xF000) {
    case 0x8000:
	return 'f';
    case 0xA000:
	return 's';
    case 0x4000:
	return 'd';
    default:
	return '?';
    }
}


void parse_block_bitmap() {
    char* bitmap = malloc(block_size);
    if (bitmap == NULL) {
	fprintf(stderr, "ERROR unsuccessful malloc.\n");
	exit(ERROR_EXIT);
    }
    if (pread(disk_fd, bitmap, block_size, block_size*(group_desc.bg_block_bitmap)) == -1) {
	fprintf(stderr, "ERROR on pread(3).\n");
	exit(ERROR_EXIT);
    }

    size_t block_count = 1;
    for (size_t i = 0;
	 i < block_size && block_count <= sb.s_blocks_per_group;
	 i++) {
	unsigned char curr_bit = bitmap[i];
	for (unsigned char sub = 0;
	     sub < 8 && block_count <= sb.s_blocks_per_group;
	     curr_bit >>= 1) {
	    if (!(curr_bit & 1u)) {
		fprintf(stdout, "BFREE,%lu\n", block_count);
	    }
	    block_count++;
	    sub++;
	}
    }
    free(bitmap);
}

void read_single_indr(size_t inode_index, __u32 block_to_read,
			     unsigned int lb_offset) {
    for (__u32 index = 0;
	 index < block_ids_per_block;
	 index++) {
	__u32 block_id;
	if (pread(disk_fd,
		  &block_id,
		  sizeof(__u32),
		  block_to_read*block_size + index * sizeof(__u32)) == -1) {
	    fprintf(stderr, "ERROR while readubg single indexed block ID.\n");
	    exit(1);
	}
	if (block_id == 0) continue;
	fprintf(stdout, "INDIRECT,%lu,1,%u,%u,%u\n",
		inode_index,      /* inode number of the owning file */
		lb_offset + index,        /* logical block offset */
		block_to_read,    /* block number of immediate indirect block */
		block_id          /* block number of referenced block */
	    );			  	
    }
}

void read_double_indr(size_t inode_index, __u32 block_to_read,
		      unsigned int lb_offset) {
    for (__u32 index = 0;
	 index < block_ids_per_block;
	 index++) {
	__u32 block_id;
	if (pread(disk_fd,
		  &block_id,
		  sizeof(__u32),
		  block_to_read*block_size + index * sizeof(__u32)) == -1) {
	    fprintf(stderr, "ERROR while readubg single indexed block ID.\n");
	    exit(1);
	}
	unsigned int curr_lb_offset = lb_offset + index * block_ids_per_block;
	if (block_id == 0) continue;
	fprintf(stdout, "INDIRECT,%lu,2,%u,%u,%u\n",
		inode_index,      /* inode number of the owning file */
		curr_lb_offset,        /* logical block offset */
		block_to_read,    /* block number of immediate indirect block */
		block_id          /* block number of referenced block */
	    );			  	
	read_single_indr(inode_index, block_id, curr_lb_offset);
    }        
}

void read_triple_indr(size_t inode_index, __u32 block_to_read,
		      unsigned int lb_offset) {		      
    for (__u32 index = 0;
	 index < block_ids_per_block;
	 index++) {
	__u32 block_id;
	if (pread(disk_fd,
		  &block_id,
		  sizeof(__u32),
		  block_to_read*block_size + index * sizeof(__u32)) == -1) {
	    fprintf(stderr, "ERROR while readubg single indexed block ID.\n");
	    exit(1);
	}
	unsigned int curr_lb_offset = lb_offset + index * block_ids_per_block * block_ids_per_block;
	if (block_id == 0) continue;
	
 	fprintf(stdout, "INDIRECT,%lu,3,%u,%u,%u\n",
		inode_index,      /* inode number of the owning file */
		curr_lb_offset,        /* logical block offset */
		block_to_read,    /* block number of immediate indirect block */
		block_id          /* block number of referenced block */
	    );		
	read_double_indr(inode_index, block_id, curr_lb_offset);
    }        
}

void read_indirect (size_t inode_index) {
    block_ids_per_block = block_size / sizeof(__u32);
    unsigned int logical_off_1 = 12;
    unsigned int logical_off_2 = logical_off_1 + block_ids_per_block;
    unsigned int logical_off_3 = logical_off_2 + block_ids_per_block * block_ids_per_block;

    read_single_indr(inode_index, inode_a[inode_index].i_block[EXT2_IND_BLOCK],
		     logical_off_1);
    read_double_indr(inode_index, inode_a[inode_index].i_block[EXT2_DIND_BLOCK],
		     logical_off_2);
    read_triple_indr(inode_index, inode_a[inode_index].i_block[EXT2_TIND_BLOCK],
		     logical_off_3);
}


void read_directory (size_t inode_index) {
    struct ext2_inode directory = inode_a[inode_index];
    size_t bytes_read = 0;
    /* process direct blocks */
    for (size_t block_index = 0;
	 block_index < EXT2_NDIR_BLOCKS && block_index < directory.i_blocks;
	 block_index++) {
	__u32 block_ptr = directory.i_block[block_index] * block_size;
	/* No more blocks to process */
	if (block_ptr == 0) return;
	
	/* process all directory entries in current block */
	__u32 curr_entry_ptr = block_ptr;  /* points to current entry on disk */
	for(; curr_entry_ptr < block_ptr + block_size;) {
	    struct ext2_dir_entry curr_entry;   /* stores current entry in RAM */	    
	    if (pread(disk_fd,
		      &curr_entry,
		      sizeof(struct ext2_dir_entry),
		      curr_entry_ptr) == -1) {
		fprintf(stderr, "ERROR while reading directory entry.\n");
		exit(ERROR_EXIT);
	    }

	    if (curr_entry.inode == 0) {
		curr_entry_ptr += block_size;
		continue;
	    }
	    
	    fprintf(stdout, "DIRENT,%lu,%lu,%u,%u,%u,'",
		    inode_index,                 /* parent inode number */
		    bytes_read,                  /* logical block offset */
		    curr_entry.inode,            /* inode number of referenced file */
		    curr_entry.rec_len,          /* entry length */
		    curr_entry.name_len          /* name length */
		);

	    /* print filename */
	    for (size_t i = 0; i < curr_entry.name_len; i++)
		fprintf(stdout, "%c", curr_entry.name[i]);
	    fprintf(stdout, "'\n");
	    curr_entry_ptr += curr_entry.rec_len;
	    bytes_read += curr_entry.rec_len;
	}
    }
}


void print_used_inodes() {
    for (size_t inode_index = 2;
	 inode_index <= sb.s_inodes_count - sb.s_free_inodes_count;
	 inode_index++) {
	if (inode_a[inode_index].i_mode != 0xF000) {
		char mtime_str[26];	  
		char atime_str[26];
		char ctime_str[26];
		bzero(mtime_str, 26);
		bzero(atime_str, 26);
		bzero(ctime_str, 26);
		unsigned long mtime = inode_a[inode_index].i_mtime;
	        unsigned long atime = inode_a[inode_index].i_atime;
		unsigned long ctime = inode_a[inode_index].i_ctime;		
		struct tm lt_m;
		memcpy(&lt_m,
		       gmtime((time_t*)&mtime),
		       sizeof(struct tm));
		strftime(mtime_str,
			 26, 
			 "%m/%d/%y %H:%M:%S",
			 &lt_m);
		struct tm lt_a;
		memcpy(&lt_a,
		       gmtime((time_t*)&atime),
		       sizeof(struct tm));
		
		strftime(atime_str,
			 26,
			 "%m/%d/%y %H:%M:%S",
			 &lt_a);		

		struct tm lt_c;
		memcpy(&lt_c,
		       gmtime((time_t*)&ctime),
		       sizeof(struct tm));
		strftime(ctime_str,
			 26,
			 "%m/%d/%y %H:%M:%S",
			 &lt_c);
		fprintf(stdout, "INODE,%lu,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u,",
			inode_index,                              /* inode number */
			get_mode(inode_a[inode_index].i_mode),    /* file type */
			(inode_a[inode_index].i_mode&0X0FFF),     /* mode */
			(inode_a[inode_index].i_uid),             /* owner */
			(inode_a[inode_index].i_gid),             /* group */
			(inode_a[inode_index].i_links_count),     /* link count */
			ctime_str,                                /* change time */
			mtime_str,                                /* modification time */
			atime_str,                                /* access time */
			(inode_a[inode_index].i_size),            /* file size */
			(inode_a[inode_index].i_blocks)           /* number of blocks */
		    );

		/* block pointers */
		for (size_t cnt = 0; cnt < EXT2_NDIR_BLOCKS; cnt++)
		    fprintf(stdout, "%u,", inode_a[inode_index].i_block[cnt]);
		fprintf(stdout, "%u,", inode_a[inode_index].i_block[EXT2_IND_BLOCK]);
		fprintf(stdout, "%u,", inode_a[inode_index].i_block[EXT2_DIND_BLOCK]);
		fprintf(stdout, "%u\n", inode_a[inode_index].i_block[EXT2_TIND_BLOCK]);
		if (get_mode(inode_a[inode_index].i_mode) == 'd') read_directory(inode_index);
		read_indirect(inode_index);
	}
	
	if (inode_index == 2)
	    inode_index = sb.s_first_ino - 1;
    }
}

/* store valid inodes in inode_a 
 * print free inodes */
void parse_inodes() {
    char* bitmap = malloc((sb.s_inodes_count)/8 + 1);
    if (pread(disk_fd,
	      bitmap,
	      (sb.s_inodes_count)/8 + 1,
	      group_desc.bg_inode_bitmap * block_size) == -1) {
	fprintf(stderr, "ERROR while reading block bitmap.\n");
    }
    
    inode_a = (struct ext2_inode*) malloc(sb.s_inode_size*(sb.s_inodes_count + 1));
    if (bitmap == NULL) {
	fprintf(stderr, "ERROR unsuccessful malloc.\n");
	exit(ERROR_EXIT);
    }
    size_t inode_count = 1;
    for (size_t i = 0;
	 inode_count <= sb.s_inodes_count;
	 i++) {
	unsigned char curr_bit = bitmap[i];
	for (unsigned char sub = 0;
	     sub < 8 && inode_count <= sb.s_inodes_count;
	     curr_bit >>= 1) {
	    /* if inode is used save it */
	    if (curr_bit & 1u) {
		if (pread(disk_fd,
			  inode_a + inode_count,
			  sb.s_inode_size,
			  block_size*group_desc.bg_inode_table + (inode_count - 1) * sb.s_inode_size) == -1) {
		    fprintf(stdout, "ERROR on pread(2).\n");
		    exit(ERROR_EXIT);
		}
	    } else {
		fprintf(stdout, "IFREE,%lu\n", inode_count);   /* if free, print */
		inode_a[inode_count].i_mode = 0xF000;         /* flag as free */
	    }
	    inode_count++;
	    sub++;
	}
    }

    print_used_inodes();
}

void parse_group() {
    unsigned int BGDT_block_offset;
    /* if first block fits both boot record and superblock */
    if (block_size >= 2 * 1024) BGDT_block_offset = 1;
    else BGDT_block_offset = 2;
    /* Block Group Descriptor Table starts on the first block
     * after the SUPERBLOCK */
    unsigned int BGDT_offset = BGDT_block_offset*block_size;
    if (pread(disk_fd, &group_desc,
	      sizeof(struct ext2_group_desc), BGDT_offset) == -1) {
	fprintf (stderr, "ERROR while reading Block Group Description Table.\n");
	exit(1);
	}
    fprintf(stdout, "GROUP,0,%u,%u,%u,%u,%u,%u,%u\n",
	    sb.s_blocks_count,                     /* total number of blocks in group */
	    sb.s_inodes_count,                     /* total number of inodes in group */
	    group_desc.bg_free_blocks_count,       /* number of free blocks in group */
	    group_desc.bg_free_inodes_count,       /* number of free inodes in group */
	    group_desc.bg_block_bitmap,            /* block number of free block bitmap */
	    group_desc.bg_inode_bitmap,            /* block number of free inode bitmap */
	    group_desc.bg_inode_table              /* block number of first inodes */
	);
    parse_block_bitmap();
    parse_inodes();
}

void parse() {
    /* get superblock data */
    if (pread(disk_fd, &sb, SUPERBLOCK_SIZE, SUPERBLOCK_OFFSET)	== -1) {
	fprintf (stderr, "ERROR while reading superblock.\n");
	exit(1);
    }

    block_size = 1024 << sb.s_log_block_size;
    fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n",
	    sb.s_blocks_count,       /* total number of blocks */
	    sb.s_inodes_count,       /* total number of inodes */
	    block_size,              /* block size */
	    sb.s_inode_size,         /* inode size */
	    sb.s_blocks_per_group,   /* blockes per group */
	    sb.s_inodes_per_group,   /* inodes per group */
	    sb.s_first_ino           /* first non-reserved inode */
	);
    
    parse_group();
}



int main (int argc, char *argv[]) {    
    /* Read command-line arguments */
    if (argc != 2) {
	printUsage();
	exit(ERROR_EXIT);
    }
    char* disk = argv[1];
    if ((disk_fd = open(disk, O_RDONLY)) == -1) {
	fprintf (stderr, "ERORR while opening disk image.\n");
	exit(ERROR_EXIT);
    }
    
    parse();
    exit(SUCCESS_EXIT);
}
