import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

df = pd.read_csv('data.csv')

df['Runtime (s)'] = df['Runtime (ms)'] / 1000


scale_order = ['1k', '1.5k', '2k', '2.5k', '3k', '3.5k', '4k']
df['Scale'] = pd.Categorical(df['Scale'], categories=scale_order, ordered=True)

df_pivot = df.pivot(index="Scale", columns="Test Name", values="Runtime (s)")

plt.figure(figsize=(12, 8))
sns.heatmap(df_pivot, annot=True, cmap='viridis', fmt='.2f', cbar_kws={'label': 'Runtime (s)'})
plt.title('Heatmap of Runtime by Scale and Image Type')
plt.xlabel('Runtime (s)')
plt.ylabel('Scale')
plt.show()
