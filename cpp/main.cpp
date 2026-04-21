#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>



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
};




// Class to parse my portfolio data from a CSV file
class FileReader {
private:
    std::string filename;

    public:
    FileReader(const std::string& name) : filename(name) {}

    std::vector<std::string> readLines() {
        std::vector<std::string> lines;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return lines;
        }
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    
};






int main() {

    const std::string CSV_FILE = "/Users/jordanflowers/Desktop/Python/Investment_Project/python/paper_trade_results.csv";

    FileReader reader(CSV_FILE);

    std::vector<std::string> content = reader.readLines();


    for (const std::string& line: content){
        std::cout << line << std::endl;
    }

    return 0; 
}