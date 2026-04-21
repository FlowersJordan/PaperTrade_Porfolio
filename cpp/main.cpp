#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
using namespace std;



struct PortfolioRow {
    std::string date;
    std::string symbol;
    int shares;
    double entry_price;
    double current_price;
    double cost_value;
    double current_value;
    double profit_loss;
    double profit_loss_percent;
    double dividend_percent;
};




// Class to parse my portfolio data from a CSV file
class FileReader {
private:
    std::string filename;

    public:
    FileReader(const std::string& name) : filename(name) {}

    std::vector<PortfolioRow> parseRows() {
        std::vector<PortfolioRow> rows;
        std::ifstream file(filename);


        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << endl;
            return rows;
        }


        string line;
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            PortfolioRow row;

            std::getline(ss, row.date, ',');
            std::getline(ss, row.symbol, ',');
            std::getline(ss, field, ','); row.shares        = std::stoi(field);
            std::getline(ss, field, ','); row.entry_price    = std::stod(field);
            std::getline(ss, field, ','); row.current_price = std::stod(field);
            std::getline(ss, field, ','); row.cost_value    = std::stod(field);
            std::getline(ss, field, ','); row.current_value = std::stod(field);
            std::getline(ss, field, ','); row.profit_loss           = std::stod(field);
            std::getline(ss, field, ','); row.profit_loss_percent    = std::stod(field);
            std::getline(ss, field, ','); row.dividend_percent = std::stod(field);


            rows.push_back(row);
        }

        return rows;
    }
    
};

class PortfolioCalculator {
    private:
        std::vector<PortfolioRow> rows;

    public:
        PortfolioCalculator(std::vector<PortfolioRow> data) : rows(data) {}

        double total_value() {
            double total = 0;

            for (const PortfolioRow& row : rows) {
                if (row.symbol != "SPY" || row.symbol != "VOO") {
                    total += row.current_value;
                }
            }
            return total;
        }


        double totalPNL() {
            double total = 0;
            for (const PortfolioRow& row : rows){
                if (row.symbol != "SPY" || row.symbol != "VOO") total+=row.profit_loss;
            }
            return total;
        }

        double totalReturn() {
            double total_cost = 0;
            for (const PortfolioRow& row : rows){
                if (row.symbol != "SPY" || row.symbol != "VOO") total_cost+=row.cost_value;
            
            }
            if (total_cost == 0) return 0;
            return (totalPNL()/ total_cost) * 100;
        }

        double spyReturn() {
            for (const PortfolioRow& row : rows) {
                if (row.symbol == "SPY") return row.profit_loss_percent;
            }
            return 0;
        }

        double vsSpy() {
            return totalReturn() - spyReturn();
        }

        PortfolioRow bestPerformer() {
            PortfolioRow best = rows[0];

            for (const PortfolioRow& row : rows) {
                if (row.symbol != "SPY" && row.profit_loss_percent > best.profit_loss_percent)
                    best = row;
            }
            return best;
        }

        PortfolioRow worstPerformer() {
            PortfolioRow worst = rows[0];
            for (const PortfolioRow& row : rows) {
                if (row.symbol != "SPY" && row.profit_loss_percent < worst.profit_loss_percent)
                    worst = row;
        }
            return worst;
    }



    //JSON OUTPUT
    void printJSON() {
        PortfolioRow best = bestPerformer();
        PortfolioRow worst = worstPerformer();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "{\n";
        std::cout << "  \"total_value\": "       << total_value()    << ",\n";
        std::cout << "  \"total_pnl\": "         << totalPNL()      << ",\n";
        std::cout << "  \"total_return\": "      << totalReturn()   << ",\n";
        std::cout << "  \"spy_return\": "        << spyReturn()     << ",\n";
        std::cout << "  \"vs_spy\": "            << vsSpy()         << ",\n";
        std::cout << "  \"best_performer\": \""  << best.symbol     << "\",\n";
        std::cout << "  \"worst_performer\": \"" << worst.symbol    << "\",\n";
        std::cout << "  \"positions\": [\n";

        int count = 0;

        for (int i = 0; i < rows.size(); i++) {
            if (rows[i].symbol == "SPY"){
                continue;
                count++;
            }
        }

        int printed = 0;
        for (int i = 0; i < rows.size(); i++){
            const PortfolioRow& row = rows[i];

            if (row.symbol == "SPY") continue;

            std::cout << "    {\n";
            std::cout << "      \"symbol\": \""        << row.symbol        << "\",\n";
            std::cout << "      \"shares\": "          << row.shares        << ",\n";
            std::cout << "      \"cost_basis\": "      << row.cost_value    << ",\n";
            std::cout << "      \"current_price\": "   << row.current_price << ",\n";
            std::cout << "      \"cost_value\": "      << row.cost_value    << ",\n";
            std::cout << "      \"current_value\": "   << row.current_value << ",\n";
            std::cout << "      \"pnl\": "             << row.profit_loss          << ",\n";
            std::cout << "      \"pct_return\": "      << row.profit_loss_percent    << "\n";
            std::cout << "    }";


            printed++;

            if(printed < count) std::cout << ",";
            std::cout << "\n";

        }

        std::cout << "   ]\n";
        std::cout << "}\n";


    }

};




int main() {

    const std::string CSV_FILE = "/Users/jordanflowers/Desktop/Python/Investment_Project/python/paper_trade_results.csv";

    FileReader reader(CSV_FILE);
    std::vector<PortfolioRow> rows = reader.parseRows();

    if(rows.empty()){
        std::cerr << "No data found in: " << CSV_FILE << std::endl;
        return 1;
    }

    PortfolioCalculator calc(rows);
    calc.printJSON();

    return 0; 
}