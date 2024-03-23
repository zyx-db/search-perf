import seaborn as sns
import matplotlib.pyplot as plt
import pandas
import sys

def makePlot(times, imageFile):
    sns.set_theme()

    sns.lineplot(data=times, x='size', y='binary set', label='binary search')
    sns.lineplot(data=times, x='size', y='eytzinger set', label='branchless binary search')
    sns.lineplot(data=times, x='size', y='bl binary set', label='eytzinger layout')
    sns.lineplot(data=times, x='size', y='bl eytzinger set', label='branchless eytzinger layout')

    plt.ylabel('latency (ns)')
    plt.xlabel('array size')
    plt.xscale('log', base=2)
    plt.legend()

    plt.savefig(imageFile, dpi=300)
    print("saved!")

def main():
    if len(sys.argv) != 3:
        print("INCORRECT USAGE")
        print()
        print("CORRECT USAGE:")
        print("python3 analyze.py <CSV_FILE> <IMAGE_FILE>")
    CSV_FILE = sys.argv[1]
    IMAGE_FILE = sys.argv[2]
    data = pandas.read_csv(CSV_FILE)
    makePlot(data, IMAGE_FILE)

if __name__ == "__main__":
    main()
