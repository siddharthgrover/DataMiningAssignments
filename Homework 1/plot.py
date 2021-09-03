# Plot support threshold vs runnning times of Apriori and FP-tree algorithm.
import matplotlib.pyplot as plt
import timeit
import sys


def main () :
    
    args = sys.argv
    input_file = args[1]
        
    supports = [25, 50, 90]
    times_fp = []
    times_ap = []
    
    for support in supports:
        cmd = "-s" + str(support)
        res = 'subprocess.run(["fpgrowth/fpgrowth/src/fpgrowth", "{0}", "{1}", "webdocs.out"])'.format(cmd, input_file)
        res2 = 'subprocess.run(["./apriori", "{1}", "{0}", "webdocs_.out"])'.format(str(support), input_file)
        kl = timeit.timeit(res , setup= 'import subprocess' , number=1)
        kp = timeit.timeit(res2 , setup= 'import subprocess' , number=1)
        times_fp.append(kl)
        times_ap.append(kp)
    
    
    plt.plot(supports, times_fp, color ='tab:orange')
    plt.plot(supports, times_ap, color ='tab:blue')
    plt.show()
    
    

if __name__ == "__main__":
    main()