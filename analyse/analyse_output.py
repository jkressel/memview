import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import random
import circlify as circ
import math
import sys
from adjustText import adjust_text

mappings = 0

trace = open(sys.argv[1], 'r')
trace1 = open(sys.argv[1], 'r').read()

name = sys.argv[2]

lib = []
lib_names = []
lib_starts = []
lib_ends = []
min_malloc = 0
max_malloc = 0

different_frees = [] #called free on memory not allocated in this compartment
different_accesses = [] #accesses to memory not allocated in this compartment
total_allocations = 0
total_frees = 0


allocations = {}
allocations_by_comp = {}

def find_allocator(malloc):
    for i in range(1, len(malloc)):
        line = malloc[i].split()
        source = int(line[2][:-1], base=16)
        for j in range(len(lib_starts)):
            if source >= lib_starts[j] and source < lib_ends[j]:
                return j
    return -1

def find_usage(ptr):
    if ptr in allocations:
        return [allocations[ptr][0], ptr]
    for i in range(1, 1024):
        if ptr - i in allocations:
            return [allocations[ptr-i][0], ptr-i]
    for alloc in allocations:
        if (ptr >= alloc and ptr < alloc + allocations[alloc][1]):
            return [allocations[alloc][0], alloc]
    return -1

def where_dis(ptr):
    for comp in range(len(lib_starts)):
        if ptr >= lib_starts[comp] and ptr < lib_ends[comp]:
            return comp
    return -1





for ln in trace:
    if ln.startswith("[LIB]"):
        lib.append(ln)

for i in range(len(lib)):
    lib_split = lib[i].split()
    if "valgrind" not in lib_split[1] and "libc.so" not in lib_split[1]:
        lib_names.append(lib_split[1])
        starts_and_ends = lib_split[2].split("-")
        lib_starts.append(int(starts_and_ends[0].strip(), base=16))
        lib_ends.append(int(starts_and_ends[1].strip(), base=16))
        different_frees.append(0)
        different_accesses.append(0)


lines = trace1.splitlines()
for ln in range(len(lines)):
    split_ln = lines[ln].split()
    if lines[ln].startswith("[L]:") and len(allocations) > 0:
        #where malloced?
        if int(split_ln[2].strip(), base=16) >= min_malloc and int(split_ln[2].strip(), base=16) <= max_malloc:
            dis = where_dis(int(split_ln[1].strip(), base=16))
            if dis == -1:
                current = ln+1
                alloc = []
                while lines[current].startswith("=="):
                    alloc.append(lines[current])
                    current = current+1
                    dis = find_allocator(alloc)
                    if dis == -1:
                        continue
            comp = find_usage(int(split_ln[2].strip(), base=16))
            if comp != -1 and dis != -1:
                #print("comp " + str(comp[0]) + " other comp " + str(allocations[comp[1]][0]))
                if comp[0] != dis:
                    different_accesses[dis] = different_accesses[dis] + 1


    elif lines[ln].startswith("[S]:") and len(allocations) > 0:
        if int(split_ln[2].strip(), base=16) >= min_malloc and int(split_ln[2].strip(), base=16) <= max_malloc:
            dis = where_dis(int(split_ln[1].strip(), base=16))
            if dis == -1:
                current = ln+1
                alloc = []
                while lines[current].startswith("=="):
                    alloc.append(lines[current])
                    current = current+1
                    dis = find_allocator(alloc)
                    if dis == -1:
                        continue
            comp = find_usage(int(split_ln[2].strip(), base=16))
            if comp != -1 and dis != -1:
            #print("comp " + str(comp[0]) + " other comp " + str(allocations[comp[1]][0]))
                if comp[0] != dis:
                    different_accesses[dis] = different_accesses[dis] + 1
    elif lines[ln].startswith("[MALLOC]:"):
        current = ln+1
        alloc = []
        while lines[current].startswith("=="):
            alloc.append(lines[current])
            current = current+1
        allocator = find_allocator(alloc)
        if allocator >= 0:
            a_list = []
            a_list.append(allocator)
            a_list.append(int(lines[ln].split()[2].strip()))
          #  print("alloctor is " + lib_names[allocator] + "\n")
            allocations[int(lines[ln].split()[1].strip(), base=16)] = a_list
            total_allocations = total_allocations + 1
            if min_malloc == 0 or int(lines[ln].split()[1].strip(), base=16) < min_malloc:
                min_malloc = int(lines[ln].split()[1].strip(), base=16)
            if max_malloc == 0 or int(lines[ln].split()[1].strip(), base=16) + int(lines[ln].split()[2].strip(), base=16) > max_malloc:
                max_malloc = int(lines[ln].split()[1].strip(), base=16) + int(lines[ln].split()[2].strip() , base=16)

    elif lines[ln].startswith("[FREE]:"):
        total_frees = total_frees + 1
        current = ln+1
        alloc = []
        while lines[current].startswith("=="):
            alloc.append(lines[current])
            current = current+1
        allocator = find_allocator(alloc)
        if allocator >= 0 and int(lines[ln].split()[1].strip(), base=16) in allocations:
            comp_allocated_in = allocations[int(lines[ln].split()[1].strip(), base=16)][0]
            if allocator != comp_allocated_in:
                different_frees[allocator] = different_frees[allocator] + 1

            del allocations[int(lines[ln].split()[1].strip(), base=16)]

    progress = round((ln / len(lines))*100, 2)
    #if progress % 1 == 0:
    #    for i in range(len(different_frees)):
    #        print(lib_names[i] + " freed memory it did not allocate: " + str(different_frees[i]) + " and accessed memory it did not allocate: " + str(different_accesses[i]) + "\n")
    print(str(progress) + "%", end='\r')


print("Total allocations " + str(total_allocations) + "\n")
print("Total frees " + str(total_frees) + "\n")
for i in range(len(different_frees)):
    print(lib_names[i] + " freed memory it did not allocate: " + str(different_frees[i]) + " and accessed memory it did not allocate: " + str(different_accesses[i]) + "\n") 


