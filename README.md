# 📈 Portfolio Tracker

A multi-language personal investment portfolio dashboard built with **C++**, **Python**, and **Flask**. Tracks a paper trading portfolio against the SPY benchmark in real time, with daily performance history stored to CSV.

Built as a software engineering portfolio project while studying at WGU — designed from the ground up with a deliberate three-layer architecture rather than a single-language solution.

---

## 🖥️ Preview

> Clean dark fintech-style dashboard showing portfolio return vs SPY, individual position P&L, and a mini performance bar for each holding.

---

## 🏗️ Architecture

```
┌─────────────────────────────────┐
│   FRONTEND  (HTML/CSS)          │
│   Flask-rendered dashboard      │
│   Live portfolio vs SPY display │
└──────────────┬──────────────────┘
               │ Jinja2 templates
┌──────────────▼──────────────────┐
│   PYTHON LAYER  (Flask)         │
│   yfinance price fetching       │
│   CSV history logging           │
│   Calls C++ engine via subprocess│
│   Serves JSON to frontend       │
└──────────────┬──────────────────┘
               │ subprocess / JSON
┌──────────────▼──────────────────┐
│   C++ ENGINE                    │
│   Parses CSV into structs        │
│   Calculates P&L, returns       │
│   Portfolio vs SPY delta        │
│   Outputs structured JSON       │
└─────────────────────────────────┘
```

Each language does what it is genuinely best at. Python handles I/O and networking. C++ handles the computation layer. HTML/CSS handles presentation. This is an intentional architectural decision, not just "using multiple languages for the sake of it."

---

## ⚙️ Tech Stack

| Layer | Technology | Purpose |
|---|---|---|
| Data Fetching | Python + yfinance | Pull live stock prices |
| History Storage | CSV | Daily portfolio snapshots |
| Computation | C++ (C++17) | Portfolio math & analytics |
| API Layer | Python + Flask | Serve data to frontend |
| Frontend | HTML + CSS | Dashboard display |
| Templating | Jinja2 | Dynamic data rendering |

---

## 📁 Project Structure

```
portfolio-tracker/
│
├── cpp/
│   ├── main.cpp              # C++ engine — CSV parser + calculator
│   └── data                  # compiled binary (gitignored)
│
├── python/
│   ├── fetch_prices.py       # fetches live prices, appends to CSV
│   ├── app.py                # Flask server, calls C++ via subprocess
│   └── portfolio_history.csv # daily data (gitignored)
│
├── frontend/
│   └── index.html            # dashboard UI with embedded CSS
│
├── .gitignore
├── run.sh                    # single command to run everything
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- Python 3.x
- g++ with C++17 support
- pip

### Installation

```bash
# clone the repo
git clone https://github.com/yourusername/portfolio-tracker.git
cd portfolio-tracker

# install Python dependencies
pip install flask yfinance pandas matplotlib

# compile the C++ engine
g++ -std=c++17 cpp/main.cpp -o cpp/data
```

### Configuration

Open `python/fetch_prices.py` and update the positions dictionary with your own holdings:

```python
positions = {
    "AAPL": {"shares": 10, "cost_basis": 175.00},
    "NVDA": {"shares": 5,  "cost_basis": 200.00},
    "SPY":  {"shares": 70, "cost_basis": 710.00},  # benchmark
}
```

`cost_basis` is your **average cost per share**, not total dollars spent.

### Running

```bash
# fetch today's prices (run this first on each trading day)
python python/fetch_prices.py

# start the dashboard
python python/app.py

# open in browser
http://127.0.0.1:5000
```

Or use the single convenience script:

```bash
./run.sh
```

---

## 📊 What It Tracks

- **Portfolio vs SPY** — the primary metric, shown front and center
- **Total portfolio value** — sum of all stock positions
- **Unrealized P&L** — current value minus cost basis in dollars
- **Total return %** — portfolio percentage return since entry
- **Best and worst performers** — highlighted positions
- **Per-position breakdown** — price, value, P&L, and return for every holding
- **Daily history** — every run appends a new row to CSV for trend analysis over time

---

## 🔧 How the C++ Layer Works

The C++ engine is called by Python via `subprocess` and communicates through stdout JSON:

```
Python calls → ./cpp/data → C++ reads CSV → calculates analytics → prints JSON → Python parses → Flask serves
```

Inside `main.cpp`:

- `FileReader` class — opens the CSV, parses each line into a `PortfolioRow` struct
- `PortfolioCalculator` class — takes a `std::vector<PortfolioRow>` and computes all metrics
- `printJSON()` method — outputs structured JSON for Python to consume

This separation means the math layer could be replaced, upgraded, or reused independently of the web layer.

---

## 📅 Daily Workflow

1. Run `fetch_prices.py` — appends today's prices and P&L to `portfolio_history.csv`
2. Run `app.py` — starts Flask, which calls the C++ binary and serves the dashboard
3. Refresh browser to see updated data
4. Script automatically skips weekends (market closed)

Over time, the CSV builds up a complete performance history that can be loaded into Matplotlib for trend charts.

---

## 🗺️ Roadmap

- [ ] Matplotlib charts for historical performance over time
- [ ] Flask `/api/portfolio` endpoint for raw JSON access
- [ ] Database backend replacing CSV (PostgreSQL or SQLite)
- [ ] Android mobile app via Java (in progress with WGU coursework)
- [ ] Authentication so the dashboard can be hosted publicly
- [ ] Automated daily run via cron job

---

## 🎓 About This Project

This project was built as the first entry in a software engineering portfolio during a BS Software Engineering degree at WGU. The investment strategy behind the paper portfolio was developed using a custom ThinkorSwim stock screener built with thinkScript, designed to identify companies with Nvidia-style bottleneck moats — dominant market positions in infrastructure that entire industries depend on.

The screener filters for expanding gross margins, accelerating revenue growth, positive and growing free cash flow, and strong operating leverage — the same financial fingerprints that early Nvidia showed before its major run.

The paper portfolio is a live test of that thesis, tracked in real time by this application.

---

## ⚠️ Disclaimer

This project tracks a **paper trading** (simulated) portfolio. Nothing in this repository constitutes financial advice. All positions are virtual and for educational and software development purposes only.

---

## 📄 License

MIT License — free to use, modify, and build on.