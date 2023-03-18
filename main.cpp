#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

enum Game {chess, checkers};

enum Color {white = 0, black = 1, nocolor = -1};

class Cell
{
    public:
    int x;
    int y;
    Color color;
    bool is_figure_placed;
    Cell(): x(-1), y(-1), color(nocolor),is_figure_placed(false){}
    Cell (int a, int b)
      : x(a)
      , y(b)
      , color(nocolor)
      , is_figure_placed(false)
    {}
    friend std::ostream& operator << (std::ostream& out, const Cell& cell);
};

std::ostream& operator << (std::ostream& out, const Cell& cell)
{
    if (cell.is_figure_placed == 1)
            {
                out << "<Q>";
            }
            else
            {
                out << " " << cell.color << " ";
            }
  return out;
}

class Board
{
    public:
    int size;
    Cell** field;
    Board(int size);
    friend std::ostream& operator << (std::ostream& out, const Board& board);
    Cell* operator [](int index) {return field[index];}
    const Cell* operator [](int index) const {return field[index];} 
    void place_figure(Cell cell)
    {
    field[size - cell.y][cell.x - 1].is_figure_placed = 1;
    }
    void delete_figure(Cell cell)
    {
    field[size - cell.y][cell.x - 1].is_figure_placed = 0;
    }
};

std::ostream& operator << (std::ostream& out, const Board& board)
{
    out << "\n";
    for (int i = 0; i < board.size; i++)
    {
        out << std::setw(3) << board.size - i << " ";
        for (int j = 0; j < board.size; j++)
        {
	        out << board.field[i][j];
        }
        out << "\n";
    }
    out << "  ";
    const char* letters[8] = { "A","B","C","D","E","F","G","H" };
    out << "  ";
    for (int i = 0; i < board.size; i++)
    {
        out << " " << letters[i % 8] << " ";
    }
    out << "\n\n";
  return out;
}

class Figure
{
  public:
  Color color;
};

class Queen : public Figure 
{
  std::string mark;
};

Board::Board(int size)
{
  this->size = size;
  field = new Cell*[size];
  int color_count = size % 2;
  for (int i = 0; i < size; i++)
    {
      field[i] = new Cell[size];  
      for (int j = 0; j < size; j++)
        {
          field[i][j] = Cell(j + 1, size - i);
          field[i][j].color = (color_count % 2 ? black : white);
          color_count++;
        } 
        if (size % 2 == 0)
        {
          color_count++;
        }    
    }
}

bool is_attacked_horizontally(const Cell& current_position, const Board& board)
{
    bool queen_count = false;
    for (int i = 0; i < board.size; i++)
    {
        if (board[board.size - current_position.y][i].is_figure_placed == 1)
        {
            queen_count = true;
        }
    }
    return (queen_count);
}

bool is_attacked_vertically(const Cell& current_position, const Board& board)
{
    bool queen_count = false;
    for (int i = 0; i < board.size; i++)
    {
        if (board[i][current_position.x - 1].is_figure_placed == 1)
        {
            queen_count = true;
        }
    }
    return (queen_count);
}

bool is_attacked_diagonally(const Cell& current_position, const Board& board)
{
    bool queen_count = false;
    for (int i = 0; i < board.size; i++)
    {
        for (int j = 0; j < board.size; j++)
        {
            if (board[i][j].is_figure_placed == 1 && abs(i - board.size + current_position.y) == abs(j - current_position.x + 1))
            {
                queen_count = true;
            }
        }
    }
    return (queen_count);
}

bool is_under_attack(const Cell& current_position, const Board& board)
{
    return is_attacked_horizontally(current_position, board) || is_attacked_vertically(current_position, board) || is_attacked_diagonally(current_position, board);
}

//false, если все клетки в столбце под атакой
//true, если в каждом столбце стоит ферзь
bool place_in_column(int column, Board& board)
{
    if (column == board.size)
    {
        return true;
    }
    for (int j = 0; j < board.size; j++)
    {
        Cell current_cell(column + 1, j + 1);
        if (!is_under_attack(current_cell, board))
        {
            board.place_figure(current_cell);
            if (place_in_column(column + 1, board))
            {
                return true;
            }
        }
        board.delete_figure(current_cell);
    }  
    return false;
}

Board solve_queens_problem(int board_size)
{
    Board board = Board(board_size);
    place_in_column(0, board);
    return board;
}


int main()
{
    Board main_board = solve_queens_problem(10);
    std::cout << main_board;
}
