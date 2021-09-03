## preprocessing 
fin = open('./paths_finished.tsv','r')
fout = open('./paths_finished.dat','w')
line = fin.readline()
while(len(line)):
    if(len(line) > 1 and line[0] != '#'):
        path = line.split('\t')[3].split(';')
        parents = [None]
        for i in range(1,len(path)):
            if(path[i] == '<'):
                path[i] = path[parents[-1]]
                parents.append(parents[parents[-1]])
            else:
                parents.append(i-1)
        fout.write(';'.join(path)+'\n')
    line = fin.readline()
fin.close()
fout.close()