#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlsf.h"
#include <sys/mman.h>



struct entry {
  void **addr;
  long size;
};
struct entry** maps;


void (*p[12]) (void* base, void* end);
void* (*a[12]) (size_t);
void (*f[12]) (void*);

extern  void oe_allocator_init1(void* base, void* end);
extern  void oe_allocator_init2(void* base, void* end);
extern  void oe_allocator_init3(void* base, void* end);
extern  void oe_allocator_init4(void* base, void* end);
extern  void oe_allocator_init5(void* base, void* end);
extern  void oe_allocator_init6(void* base, void* end);
extern  void oe_allocator_init7(void* base, void* end);
extern  void oe_allocator_init8(void* base, void* end);
extern  void oe_allocator_init9(void* base, void* end);
extern  void oe_allocator_init10(void* base, void* end);
extern  void oe_allocator_init11(void* base, void* end);
extern  void oe_allocator_init12(void* base, void* end);
extern  void* host_malloc(size_t);
extern  void host_free(void*);

extern  void* a_malloc(size_t);
extern  void a_free(void*);

extern  void* b_malloc(size_t);
extern  void b_free(void*);

extern  void* c_malloc(size_t);
extern  void c_free(void*);

extern  void* d_malloc(size_t);
extern  void d_free(void*);

extern  void* e_malloc(size_t);
extern  void e_free(void*);

extern  void* f_malloc(size_t);
extern  void f_free(void*);

extern  void* g_malloc(size_t);
extern  void g_free(void*);

extern  void* h_malloc(size_t);
extern  void h_free(void*);

extern  void* i_malloc(size_t);
extern  void i_free(void*);

extern  void* j_malloc(size_t);
extern  void j_free(void*);

extern  void* k_malloc(size_t);
extern  void k_free(void*);
extern  void* l_malloc(size_t);
extern  void l_free(void*);


int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
   // printf("%s \n", line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}


void main(int argc, char* argv[])
{
	char* name = NULL;

	/*
	 * 0 is default, just allocate with the default shared allocator
	 * 1 is tlsf in private heaps
	 * 2 is snmalloc in private heaps
	 *
	 */
	int mode = 0;
	if (argc > 1)
	{
		name = argv[1];
		
	}
	char* e;
	if (argc > 2)
		mode = strtol(argv[2], &e, 10);

	printf("Name %s\n", name);

	FILE* trace = fopen(name, "r");

	char line[256];

	p[0] = oe_allocator_init1;
	p[1] = oe_allocator_init2;
	p[2] = oe_allocator_init3;
	p[3] = oe_allocator_init4;
	p[4] = oe_allocator_init5;
	p[5] = oe_allocator_init6;
	p[6] = oe_allocator_init7;
	p[7] = oe_allocator_init8;
	p[8] = oe_allocator_init9;
	p[9] = oe_allocator_init10;
	p[10] = oe_allocator_init11;
	p[11] = oe_allocator_init12;

	a[0] = a_malloc;
	a[1] = b_malloc;
	a[2] = c_malloc;
	a[3] = d_malloc;
	a[4] = e_malloc;
	a[5] = f_malloc;
	a[6] = g_malloc;
	a[7] = h_malloc;
	a[8] = i_malloc;
	a[9] = j_malloc;
	a[10] = k_malloc;
	a[11] = l_malloc;

	f[0] = a_free;
	f[1] = b_free;
	f[2] = c_free;
	f[3] = d_free;
	f[4] = e_free;
	f[5] = f_free;
	f[6] = g_free;
	f[7] = h_free;
	f[8] = i_free;
	f[9] = j_free;
	f[10] = k_free;
	f[11] = l_free;
;

	fgets(line, 255, trace);
	char* end;
	const long quantity = strtol(line, &end, 10);
	fgets(line, 255, trace);
	const long comps = strtol(line, &end, 10);
	fgets(line, 255, trace);
        const long total = strtol(line, &end, 10);

       	maps = mmap(0, quantity*sizeof(struct entry*), PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);
   	for (int i = 0; i < quantity; i++)
	{
		maps[i] = mmap(0, sizeof(struct entry), PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);

	}

	void* heaps[comps];
	tlsf_t allocs[comps];
	if (mode == 0) {
		heaps[0] = mmap(0, 0x100000000, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);
		allocs[0] = tlsf_create_with_pool(heaps[0], 0x100000000);
	}
	if (mode == 1)	{
		for (int i = 0; i < comps; i++)
		{
			heaps[i] = mmap(0, 0x10000000, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);
			allocs[i] = tlsf_create_with_pool(heaps[i], 0x10000000);

		}

	}
	if (mode == 2) {
		for (int i = 0; i < comps; i++)
        	{
			void* base;
			long size;
				base = mmap(0, 0x10000000, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);
				size = 0x10000000;
                	if (!base)
                        	printf("Got null %d\n", i);

                	p[i](base, base +(sizeof(char)*size));

        	}
	}
	if (mode == 3) {
		void* base = mmap(0, 0x10000000, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANON, -1, 0);
		p[0](base, base+ (sizeof(char)*0x10000000));

	}

	for (int q = 0; q < total; q++)
	{
		fgets(line, 255, trace);
		int line_max = 255;
		char *next;
		char *mapping = strtok_r(line, " ", &next);
		printf("%s\n", mapping);
		

		if (strstr(mapping, "MALLOC")) {
			int comp = atoi(strtok_r(NULL, " ", &next));
			int alloc_no = atoi(strtok_r(NULL, " ", &next));
			int size = atoi(strtok_r(NULL, " ", &next));
			void* alloc;
                	if (mode == 0)
                            	//alloc = malloc(size);
                          	alloc = tlsf_malloc(allocs[0], size);
                      	else if (mode == 1)
                              alloc = tlsf_malloc(allocs[comp], size);
                      	else if (mode == 2)
                              	alloc = a[comp](size);
                      	else if (mode == 3)
                              	alloc = a[0](size);
			printf("Malloc comp %d, id %d, size %d, alloc %p\n", comp, alloc_no, size, alloc);
                      	struct entry* current_entry = (struct entry*)maps[alloc_no];
                      	current_entry->size = size;
                      	current_entry->addr = alloc;

		} else if(strstr(mapping, "FREE")) {
			int alloc_no = atoi(strtok_r(NULL, " ", &next));
			int comp = atoi(strtok_r(NULL, " ", &next));
			struct entry* current_entry = (struct entry*)maps[alloc_no];
			printf("free comp %d, id %d, addr %p\n", comp, alloc_no, current_entry->addr);
			if (current_entry->addr == NULL)
				printf("Invalid free\n");
			if (mode == 0)
                           	//free(current_entry->addr);
                                tlsf_free(allocs[0], current_entry->addr);
                        else if (mode == 1)
                                tlsf_free(allocs[comp], current_entry->addr);
                        else if (mode == 2)
                                f[comp](current_entry->addr);
                        else if (mode == 3)
                                f[0](current_entry->addr);
                                current_entry->addr = NULL;
                                current_entry->size = 0;

		} 
		else if (strstr(mapping, "[S]")) {
			int alloc_no = atoi(strtok_r(NULL, " ", &next));
			int offset = atoi(strtok_r(NULL, " ", &next));
                        int size = atoi(strtok_r(NULL, " ", &next));
			struct entry* current_entry = (struct entry*)maps[alloc_no];
			switch(size) {
				case 1: {
					*((char*)current_entry->addr +offset) = (char) 1;
					break;
				}
				case 2: {
					*((char*)current_entry->addr + offset) = (short) 1;
                                        break;
				}
				case 4: {
                                        *((char*)current_entry->addr + offset) = (int) 1;
                                        break;
				}
				case 8: {
                                        *((char*)current_entry->addr + offset) = (long) 1;
                                        break;
				}
				case 16: {
                                        *((char*)current_entry->addr + offset) = (long) 1;
					*((char*)current_entry->addr + offset+sizeof(long)) = (long) 1;
                                        break;
				}
				case 32: {
			//		printf("addr %p, offset %x, addr+offset %p\n", current_entry->addr, offset, (char*)current_entry->addr + offset+(2*sizeof(long)));
                                        *((char*)current_entry->addr + offset) = (long) 1;
                                        *((char*)current_entry->addr +offset+sizeof(long)) = (long) 1;
					*((char*)current_entry->addr +offset+(2*sizeof(long))) = (long) 1;
				*((char*)current_entry->addr+offset+(3*sizeof(long))) = (long) 1;
                                        break;
				}
				default: {
                                        printf("Unhandled store size: %d\n", size);
                                        break;
				}


			}

		} 
		else if (strstr(mapping, "[L]")) {
			int alloc_no = atoi(strtok_r(NULL, " ", &next));
                        int offset = atoi(strtok_r(NULL, " ", &next));
                        int size = atoi(strtok_r(NULL, " ", &next));
			struct entry* current_entry = (struct entry*)maps[alloc_no];
			switch(size) {
                                case 1: {
                                        char one = (char)current_entry->addr[offset];
                                        break;
				}
                                case 2: {
                                        short two = (short)current_entry->addr[offset];
                                        break;
				}
                                case 4: {
                                        int four = (int)current_entry->addr[offset];
                                        break;
				}
                                case 8: {
                                        long eight = (long)current_entry->addr[offset];
                                        break;
				}
                                case 16: {
                                        long sixteen_one = (long)current_entry->addr[offset];
                                        long sixteen_two = (long)current_entry->addr[offset+sizeof(long)];
                                        break;
				}
                                case 32: {
                                        long thirtytwo_one = (long)current_entry->addr[offset];
                                        long thirtytwo_two = (long)current_entry->addr[offset+sizeof(long)];
                                        long thirtytwo_three = (long)current_entry->addr[offset+(2*sizeof(long))];
                                        long thirtytwo_four = (long)current_entry->addr[offset+(3*sizeof(long))];
                                        break;
				}
                                default: {
                                        printf("Unhandled store size: %d\n", size);
                                        break;
				}


                        }

		}	
		//int map_index = strtol(mapping, &end, 10);
		//mapping = strtok_r(NULL, " ", &next);
		//if (afsize > 0) 
		//{
		//	void* alloc;
		//	if (mode == 0)
		//	//	alloc = malloc(afsize);
		//		alloc = tlsf_malloc(allocs[0], afsize);
		//	else if (mode == 1)
		//		alloc = tlsf_malloc(allocs[map_index], afsize);
		//	else if (mode == 2)
		//		alloc = a[map_index](afsize);
		//	else if (mode == 3)
		//		alloc = a[0](afsize);
		//	struct entry* current_entry = (struct entry*)maps[map_index][used[map_index]];
		//	current_entry->size = afsize;
		//	current_entry->addr = alloc;
		//	used[map_index]++;
		//	long running = 0;
		//	for (int i = 0; i < quantity; i++)
		//	{
		//		running += sizes[i];
		//	}
		//	if (maxima[quantity] == running)
		//	{
		//	//	printf("Reached absolute maximum\n");
		//		actual_usage[quantity] = getValue();
		//	//	getchar();
		//	}


	//	} 
	//	else if (afsize < 0)
	//	{
	//		for (int i = 0; i < used[map_index]; i++)
	//		{
	//			struct entry* current_entry = (struct entry*)maps[map_index][i];
	//			if (current_entry->size == abs(afsize)) {
	//				if (mode == 0)
	//				//	free(current_entry->addr);
	//					tlsf_free(allocs[0], current_entry->addr);
	//				else if (mode == 1)
	//					tlsf_free(allocs[map_index], current_entry->addr);
	//				else if (mode == 2)
	//					f[map_index](current_entry->addr);
	//				else if (mode == 3)
	//					f[0](current_entry->addr);
	//				current_entry->addr = NULL;
	//				current_entry->size = 0;
	//				sizes[map_index] += afsize;
	//			}
	//		}
	//		rss[q] = getValue();
	//		vm[q] = getVMSize();
//
//		}
	}
}
