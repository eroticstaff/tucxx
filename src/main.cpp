#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <iomanip>

enum Direction {
    Left = -1,
    Right = 1,
    None = 0
};

struct Row {
    std::string state;
    std::string symbol;
    std::string future_symbol;
    Direction direction = Direction::None;
    std::string future_state;
};

void print_row(const Row &row) {
    std::cout << row.state << " " << row.symbol << " " << row.future_symbol << " " << row.direction << " "
              << row.future_state << std::endl;
}

int main() {
    std::vector<Row> rows;
    //1100#1010
    //for inc.tuc and dec.tuc
    //    std::vector<std::string> tape ={"0", "0", "1", "0"};
    //for inc_bin.tuc and inc_digit.tuc
        std::vector<std::string> tape = {"@", "1", "1", "0", "1", "1", "#", "0", "0", "0", "1"};
    //for bracket_balance.tuc
    //    std::vector<std::string> tape = {"@", "(", "(", ")", ")", ")", "#", "0", "0", "0", "%"};

    int head = 0;
    std::string current_state = "GO_LEFT";
    std::string default_symbol = "0";


    std::ifstream file;
    file.open("../example_tucs/inc_digit.tuc");
    std::string line;
    while (std::getline(file, line)) {
        auto splitText = line | std::ranges::views::split(' ') | std::ranges::views::transform([](auto &&rng) {
            return std::string(&*rng.begin(), std::ranges::distance(rng));
        });
        std::vector<std::string> tokens;
        for (auto s: splitText) {
            tokens.push_back(s);
        }
        if (tokens.size() != 5) {
            std::cout << "ERROR: bad row" << std::endl;
            throw;
        }

        Row row;
        row.state = tokens[0];
        row.symbol = tokens[1];
        row.future_symbol = tokens[2];

        if (tokens[3] == "<-") {
            row.direction = Direction::Left;
        } else if (tokens[3] == "->") {
            row.direction = Direction::Right;
        } else {
            std::cout << "ERROR: bad row" << std::endl;
            throw;
        }
        row.future_state = tokens[4];
        rows.push_back(row);
    }

    bool loop = true;

    int cycles = 0;

    while (loop) {
        cycles++;
        std::stringstream tape_out;

        if (head == tape.size()) {
            tape.push_back(default_symbol);
        }
        tape_out << current_state << ": ";
        auto i = 0;
        int head_pos = 0;
        for (auto c: tape) {
            tape_out << c;
            if (i == head) {
                head_pos = tape_out.str().size();
            }
            i++;
        }
        std::cout << tape_out.str() << std::endl;
        std::cout << std::right << std::setw(head_pos) << "^" << std::endl;

        bool applied = false;
        for (const auto &row: rows) {
            if (row.state == current_state) {
                if (row.symbol == tape[head]) {
                    tape[head] = row.future_symbol;
                    head += row.direction;
                    current_state = row.future_state;
                    applied = true;
                    if (head < 0) {
                        loop = false;
                    }
                    break;
                }
            }
        }
        if (!applied)
            break;
    }

    std::cout << "TOTAL CYCLES: " << cycles << std::endl;

    return 0;
}
