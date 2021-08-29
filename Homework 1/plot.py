# Plot support threshold vs runnning times of Apriori and FP-tree algorithm.
import matplotlib.pyplot as plt
import timeit

def main () :
    supports = [5, 10, 25, 50, 90]
    times = []
    
    for support in supports:
        cmd = "-s" + str(support)
        res = 'subprocess.run(["fpgrowth/fpgrowth/src/fpgrowth", "{0}", "retail.dat", "retail.out"])'.format(cmd)
        kl = timeit.timeit(res , setup= 'import subprocess' , number=1)
        times.append(kl)
    
    
    plt.plot(supports, times, color ='tab:orange')
    plt.show()
    
    

if __name__ == "__main__":
    main()