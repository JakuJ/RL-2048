import matplotlib.pyplot as plt
import pandas as pd
import sys

if __name__ == "__main__":
    df = pd.read_csv(sys.argv[1])
    df['won'] = df['max'] >= 2048

    fig, ax = plt.subplots()

    ax = plt.subplot(2, 1, 1)
    ax.plot(df['score'].rolling(window=300).mean())
    ax.set_ylabel('Wynik')

    ax = plt.subplot(2, 1, 2)
    ax.plot(df['won'].rolling(window=300).mean())
    ax.set_ylabel('Odsetek zwycięstw')
    ax.set_xlabel('Liczba iteracji')
    ax.set_ylim((0, 1))

    plt.show()
