#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Minesweeper {
    int width, height, mines;
    std::vector<std::vector<int>> board; // -1 for mine, else count
    std::vector<std::vector<bool>> revealed;
public:
    Minesweeper(int w, int h, int m) : width(w), height(h), mines(m) {
        board.assign(height, std::vector<int>(width, 0));
        revealed.assign(height, std::vector<bool>(width, false));
        placeMines();
        calcNumbers();
    }

    void placeMines() {
        srand((unsigned)time(nullptr));
        int placed = 0;
        while (placed < mines) {
            int r = rand() % height;
            int c = rand() % width;
            if (board[r][c] == -1) continue;
            board[r][c] = -1;
            ++placed;
        }
    }

    void calcNumbers() {
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                if (board[r][c] == -1) continue;
                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < height && nc >= 0 && nc < width && board[nr][nc] == -1)
                            ++count;
                    }
                }
                board[r][c] = count;
            }
        }
    }

    bool reveal(int r, int c) {
        if (r < 0 || r >= height || c < 0 || c >= width) return true;
        if (revealed[r][c]) return true;
        revealed[r][c] = true;
        if (board[r][c] == -1) return false;
        if (board[r][c] == 0) {
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc)
                    if (dr || dc) reveal(r+dr, c+dc);
        }
        return true;
    }

    bool isWon() const {
        int cells = width * height - mines;
        int revealedCount = 0;
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                if (revealed[r][c] && board[r][c] != -1) ++revealedCount;
        return revealedCount == cells;
    }

    void display(bool showMines = false) const {
        std::cout << "   ";
        for (int c = 0; c < width; ++c) std::cout << c % 10;
        std::cout << '\n';
        for (int r = 0; r < height; ++r) {
            std::cout << r % 10 << " |";
            for (int c = 0; c < width; ++c) {
                if (revealed[r][c]) {
                    if (board[r][c] == -1) std::cout << '*';
                    else if (board[r][c] == 0) std::cout << ' ';
                    else std::cout << board[r][c];
                } else if (showMines && board[r][c] == -1) {
                    std::cout << '*';
                } else {
                    std::cout << '#';
                }
            }
            std::cout << "|\n";
        }
    }
};

int main() {
    int width = 10, height = 10, mines = 10;
    Minesweeper game(width, height, mines);
    while (true) {
        game.display();
        std::cout << "Enter row and column to reveal: ";
        int r, c;
        if (!(std::cin >> r >> c)) break;
        if (!game.reveal(r, c)) {
            std::cout << "BOOM! You hit a mine.\n";
            game.display(true);
            break;
        }
        if (game.isWon()) {
            std::cout << "Congratulations! You cleared the board.\n";
            game.display(true);
            break;
        }
    }
}
