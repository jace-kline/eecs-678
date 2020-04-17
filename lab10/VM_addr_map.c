#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* 
	AUTHOR: Jace Kline
   	KUID: 2881618
*/

#define MAXSTR 1000

// IMPORTANT INFO:
// Pages and frames have the same fixed size, which is the page_bits.
// Frames are the REAL contiguous memory blocks that reside in the memory.
// Pages are VIRTUAL contiguous memory blocks, they do not really exist. Your program operates
// on pages (operates on virtual memory), but in reality, it operates 
// on the frames under the hood without being aware. Your program use
// virtual (or logical) addresses, and these addresses are mapped to 
// physical addresses by the OS. Instead of mapping every single byte address
// to another, the OS maps pages to frames instead to be efficient. These mappings are stored 
// in a table called PAGE TABLE. When OS map an unmapped page to a frame, it updates the
// page table like this: page_table[page_num]=frame_num. If a processes try to reach an 
// unmapped page, a page fault occurs, which is handled by the OS. It maps a frame to 
// the page causing the page fault, and give control back to the process.

// In this lab, you will manage the memory by paging. Virtual address access
// requests will come to you through an input file, and you will handle
// each request. You don't need to allocate real memory for frames, just pretend
// they exist in somewhere. Follow these steps for each memory request:

// Step 1: Check if the requested virtual address has a mapping in the page_table by
// looking at the page_mapped array (without loop, no need for that). 

// Step 2: If it doesn't have, 
// that means a page fault occurred(otherwise go to step 3). You need to find an 
// unused frame in the memory (page_mapped array stores this information), and 
// put the address of this frame to the page_table entry which caused the page fault.
// After that update, frame_mapped and page_mapped arrays, and go to step 3

// Step 3: If it does have,
// then all you need to do is use page_table to get mapped frame address
// and print it to the screen with the same page offset. Remember that page
// offsets stay same after virtual to physical address translation. Go to step 1

// At the beginning of your program, none of your pages will be mapped to the 
// frames. So you will definetely handle some page faults. At every page fault,
// map the requested page to the frame that is available and clossest to the 
// beginning. Therefore, the frames will be allocated in sequential order (0 1 2 3...).

int main(int argc, char *argv[])
{
	// A buffer for storing input strings
	char line[MAXSTR]; 
	// page_table array will represent a "single level page table".
	// The number of entries it has is dependent on logical address
	// space size and page size. It will have equal number of entries with
	// page_mapped. The page_mapped array tells whether a page_table
	// entry is valid(mapped) or not. For example, if the fifth page of
	// logical address space is mapped, page_table[5] will had the corresponding
	// frame address, and page_mapped[5] will be true (and false otherwise.)
	uint32_t *page_table; bool *page_mapped;
	// frame_mapped array will be used to store frame availability
	// for allocation at page fault. 
	bool     *frame_mapped;
	
	// each virtual address consists of n bits where n = log_bits + page_bits
	// In this lab, we will consider n as 32.
	// The most significant log_bits of a virtual address represent the page address.
	// And the least significant page_bits of a virtual address represent the offset
	// in the pointed page. With this offset, we can access individual bytes in
	// our pages.
	// each physical address consists of m bits where = phy_bits + page_bits.
	// In this lab, we will assume n=m, and log_bits=phy_bits. (In reality, these
	// can be different)
	uint32_t log_bits, phy_bits;
	uint32_t page_bits, d; // d is always 2
	
	// These numbers will hold the number of pages and frames in total.
	// They will be equal due to our assumptions for this lab.
	uint32_t num_pages, num_frames;
	
	// offset_addr | (page_num << page_bits)             -> logical_addr
	// offset_addr | (page_table[page_num] << page_bits) -> physical_addr
	uint32_t offset_addr, logical_addr, physical_addr, page_num;
	uint32_t offset_mask;

	/* Get the memory characteristics from the input file */
	fgets(line, MAXSTR, stdin);
	if((sscanf(line, "Logical address space size: %d^%d", &d, &log_bits)) != 2){
		fprintf(stderr, "Unexpected line 1. Abort.\n");
		exit(-1);
	}
	fgets(line, MAXSTR, stdin);
	if((sscanf(line, "Physical address space size: %d^%d", &d, &phy_bits)) != 2){
		fprintf(stderr, "Unexpected line 2. Abort.\n");
		exit(-1);
	}
	fgets(line, MAXSTR, stdin);
	if((sscanf(line, "Page size: %d^%d", &d, &page_bits)) != 2){
		fprintf(stderr, "Unexpected line 3. Abort.\n");
		exit(-1);
	}

	// initialize num_pages and num_frames
	// You can use pow and log2 functions of math.h library
	/* Allocate arrays to hold the page_table, page_mapped and frame_mapped
	   Initialize them to indicate that no pages are currently mapped to
	   physical memory. HINT: If you create them with calloc(), that will
	   do the initiallization for you automatically */
	num_pages    = pow(2, log_bits - page_bits);
	page_table   = (uint32_t *) calloc(num_pages, sizeof(uint32_t));
	page_mapped  = (bool *) calloc(num_pages, sizeof(uint32_t));
	num_frames   = pow(2, phy_bits - page_bits);
	frame_mapped = (bool *) calloc(num_frames, sizeof(uint32_t));
	offset_mask  = ((uint32_t)0xffffffff) >> (unsigned)log2(num_pages);

	// print number of pages and number of frames
	printf("Number of pages: %d, Number of frames: %d\n\n", num_pages, num_frames);

	/* Read each accessed address from input file. Map the logical address to
	   corresponding physical address */
	fgets(line, MAXSTR, stdin);
	while(!(feof(stdin))){
		sscanf(line, "0x%x", &logical_addr);
		fprintf(stdout, "Logical address: 0x%x\n", logical_addr);

		/* Calculate page number from the logical address (right shift needed)
			, and print it*/
		page_num = logical_addr >> page_bits;
		printf("Page Number: %d\n", page_num);

		// Here is where you need to implement Step 1 and 2

		// if the given page # is not mapped, report page fault and then map it
		if(!page_mapped[page_num]) {
			// print that there was a page fault
			printf("Page Fault!\n");
			// find the first available physical address frame (loop)
			for(int i = 0; i < num_frames; i++) {
				// if available frame, it won't be mapped yet
				if(!frame_mapped[i]) {
					// set the page # -> frame # mapping
					page_table[page_num] = i;

					// flag that the given page and frame are mapped
					frame_mapped[i] = true;
					page_mapped[page_num] = true;

					// break the loop
					break;
				}
			}
		}

		/* Form corresponding physical address (Step 3)*/
		offset_addr = logical_addr & offset_mask;
		physical_addr = (page_table[page_num] << page_bits) | offset_addr;
		// physical_addr = (page_table[page_num] << page_bits) + (logical_addr - (page_num << page_bits));

		/* Print frame number and physical address */
		printf("Frame Number: %d\nPhysical Address: 0x%x\n", page_table[page_num], physical_addr);

		/* Read next line */
		fgets(line, MAXSTR, stdin);    
	}

	// We free these arrays for you assuming that you are gonna dynamically allocate them
	free(page_table);
	free(frame_mapped);
	free(page_mapped);

	return 0;
}
