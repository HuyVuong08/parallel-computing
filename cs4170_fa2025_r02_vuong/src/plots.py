import pandas as pd
import matplotlib.pyplot as plt
import os

# output directory for plots
os.makedirs('../plots', exist_ok=True)
output_dir = '../plots/'

# Read results
df = pd.read_csv('../data/runs.csv')

avg_df = df[df['trial'] == 'Average'].copy()

# Speedup
T1 = avg_df['seconds'].iloc[0]
avg_df['Speedup'] = T1 / avg_df['seconds']

# Efficiency
avg_df['Efficiency'] = avg_df['Speedup'] / avg_df['nodes']

# Karp-Flatt
avg_df['Karp_Flatt'] = ((1 / avg_df['Speedup']) - (1 / avg_df['nodes'])) / (1 - (1 / avg_df['nodes']))

# Speedup plot
plt.figure()
plt.plot(avg_df['nodes'].to_numpy(), avg_df['Speedup'].to_numpy(), marker='o')
plt.xlabel('Processes (p)')
plt.ylabel('Speedup (Sp)')
plt.title('Speedup vs. Processes')
plt.grid(True)
plt.savefig(f'{output_dir}speedup.png')

# Efficiency plot
plt.figure()
plt.plot(avg_df['nodes'].to_numpy(), avg_df['Efficiency'].to_numpy(), marker='o')
plt.xlabel('Processes (p)')
plt.ylabel('Efficiency (Ep)')
plt.title('Efficiency vs. Processes')
plt.grid(True)
plt.savefig(f'{output_dir}efficiency.png')

# Karp-Flatt plot
plt.figure()
plt.plot(avg_df['nodes'].to_numpy(), avg_df['Karp_Flatt'].to_numpy(), marker='o')
plt.xlabel('Processes (p)')
plt.ylabel('Karp-Flatt (ε)')
plt.title('Karp-Flatt vs. Processes')
plt.grid(True)
plt.savefig(f'{output_dir}karp_flatt.png')

print("Plots saved in 'plots/' directory.")

avg_df