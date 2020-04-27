import sys

import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    df = pd.read_csv(sys.argv[1])
    df['won'] = df['max'] >= 2048

    fig, ax = plt.subplots()
    fig.suptitle('Average metrics over 100 games')

    ax = plt.subplot(2, 1, 1)
    ax.plot(df['score'].rolling(window=100).mean())
    ax.set_ylabel('Score')

    ax = plt.subplot(2, 1, 2)
    ax.plot(df['won'].rolling(window=100).mean())
    ax.set_ylabel('Win rate')
    ax.set_xlabel('Epoch')
    ax.set_ylim((0, 1))

    plt.show()
