from collections import defaultdict

frequent_patterns = []

def find_frequent_items(database, support):
    freq = defaultdict(int)
    for sequence in database:
        found = set()
        for item in sequence:
            if item in found:
                continue
            else:
                freq[item] += 1
                found.add(item)
    
    freq_items = []
    for item in freq.keys():
        if freq[item] >= support:
            freq_items.append(item)
    
    return freq_items

def create_projected_database(database, item):
    projected_database = []
    
    for sequence in database:
        for i in range(len(sequence)):
            if sequence[i] == item and i != len(sequence)-1:
                projected_database.append(sequence[i+1:])
                break
    
    return projected_database

def min_sequence(projected_database, support=2, prefix_list=list()):
    
    # find the frequent items in projected database
    frequent_items = find_frequent_items(projected_database, support)
    
    for item in frequent_items:
        
        # try to create a projected database of it
        new_projected_database = create_projected_database(projected_database, item)
        
        # create new prefix
        new_prefix_list = prefix_list + [item]
        
        # push the new found frequent pattern to answer
        frequent_patterns.append(new_prefix_list)
        
        # now recurse on this
        min_sequence(new_projected_database, support, new_prefix_list)


# MAIN #########
import sys
import os
args = sys.argv

input_processed_file, support_percent, output_file_name = args[1], float(args[2]), args[3]

assert os.path.isfile(input_processed_file), "Input file does not exists!!"

## full data in memory
fin = open(input_processed_file,'r')
data = fin.readlines()
def strip(l):
    return l[:-1].split(';')
data = list(map(strip,data))

min_sequence(data, support=int(support_percent*0.01*len(data) + 0.5) )

f = open(output_file_name + ".txt", "w")

# spit in file
for pattern in frequent_patterns:
    for item in pattern:
        print(item, file=f, end=' ')
    print(file=f)

f.close()