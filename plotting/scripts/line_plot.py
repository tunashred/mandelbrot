import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import pandas as pd
import sys

if len(sys.argv) != 2:
    print("Usage: python line_plot.py <data.csv>")
    sys.exit(1)

csv_file = sys.argv[1]

df = pd.read_csv(csv_file)

df['Runtime (s)'] = df['Runtime (ms)'] / 1000

df['Scale'] = pd.Categorical(df['Scale'], categories=['1k', '1.5k', '2k', '2.5k', '3k', '3.5k', '4k'], ordered=True)
df = df.sort_values(['Test Name', 'Scale'])

df = df.groupby(['Test Name', 'Scale'], as_index=False).agg({'Runtime (s)': 'mean'})

df_pivot = df.pivot(index='Scale', columns='Test Name', values='Runtime (s)')

plt.figure(figsize=(12, 8))
for test_name in df_pivot.columns:
    plt.plot(df_pivot.index, df_pivot[test_name], marker='o', linestyle='-', label=test_name)

plt.xlabel('Scale')
plt.ylabel('Runtime (s)')
plt.title('Runtime by Scale for All Images')
plt.legend(loc='upper left')
plt.xticks(rotation=45)
plt.grid(True)
plt.tight_layout()
plt.show()
