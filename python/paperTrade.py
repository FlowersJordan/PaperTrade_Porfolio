import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import yfinance as yf
from datetime import date
import os


positions = {
    "CRDO": {"shares": 30, "entry_price": 162.78,},
    "AMD": {"shares": 18, "entry_price": 285.14},
    "APP": {"shares": 10, "entry_price": 473.03},
    "AUPH": {"shares": 300, "entry_price": 15.95},
    "INCY": {"shares": 50, "entry_price": 98.24},
    "NBIX": {"shares": 38, "entry_price": 132.21},
    "NVDA": {"shares": 24, "entry_price": 198.74},
    "PLTR": {"shares": 35, "entry_price": 145.87},
    "SPY": {"shares": 70, "entry_price": 708.88},
    "TMDX": {"shares": 44, "entry_price": 115.44},
    "VOO": {"shares": 50, "entry_price": 651.46}
    }


rows = []

for symbol, data in positions.items():
    ticker = yf.Ticker(symbol)
    current_price = ticker.history(period="1d")["Close"].iloc[0]

    dividend_yield = ticker.info.get("dividendYield")

    shares = data["shares"]
    entry_price = data["entry_price"]
    current_value = shares * current_price
    entry_value = shares * entry_price
    profit_loss = current_value - entry_value
    profit_loss_percent = (profit_loss / entry_value) * 100

    rows.append({
        "Date": date.today(),
        "Symbol": symbol,
        "Shares": shares,
        "Entry Price": entry_price,
        "Current Price": current_price,
        "Current Value": current_value,
        "Entry Value": entry_value,
        "Profit/Loss": profit_loss,
        "Profit/Loss %": profit_loss_percent,
        "Dividend Yield": dividend_yield
    })

df = pd.DataFrame(rows)
df = df.fillna(0)
print(df)


file = "paper_trade_results.csv"
if os.path.exists(file):
    df.to_csv(file, mode='a', header=False, index=False)
else:
    df.to_csv(file, mode='w', header=True, index=False)

print(f"Results saved to {file}")



# PLOTTING TO MONITOR PERFORMANCE
colors = ['green' if x > 0 else 'red' for x in df['Profit/Loss %']]
plt.figure(figsize=(12, 6))
plt.bar(df['Symbol'], df['Profit/Loss %'], color=colors)
plt.axhline(y=0, color='black', linewidth=0.8)
plt.title('Paper Trade Performance')
plt.xlabel('Stock Symbol')
plt.ylabel('Profit/Loss %')
plt.tight_layout()
plt.savefig('paper_trade_performance.png')
plt.show()

