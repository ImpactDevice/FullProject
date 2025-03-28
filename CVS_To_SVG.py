import pandas as pd
import matplotlib.pyplot as plt

# List of CSV files
csv_files = ['FakeData1.csv']

# Generate graphs from CSV files
for csv_file in csv_files:
    df = pd.read_csv(csv_file)
    plt.figure()
    df.plot(x='Time', y='Out')
    plt.xlabel('Time')
    plt.ylabel('Distance') #This whole file will have to get fix
    graph_file = f"{csv_file.split('.')[0]}.svg"
    plt.savefig(graph_file, format='svg')