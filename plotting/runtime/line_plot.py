import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('data.csv')

df['Runtime (s)'] = df['Runtime (ms)'] / 1000

scale_order = ['1k', '1.5k', '2k', '2.5k', '3k', '3.5k', '4k']

df['Scale'] = pd.Categorical(df['Scale'], categories=scale_order, ordered=True)

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
