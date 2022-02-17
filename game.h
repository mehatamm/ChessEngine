#include <memory>
#include <string>
#include <vector>
using std::string;
class Piece{
	public:
	char color;
	char type;
	bool moved;
	Piece(char color, char type, bool moved);
};
struct Move{
	int startPos;
	int endPos;
};
class Game{
	std::unique_ptr<Piece> gameBoard[64];
	char activeColor;
	string castlingAvailability;
	string ep;
	int halfmove;
	int fullmove;
	bool matchColor(int pos);
	bool slideMove(int newPos);
	bool captureMove(int newPos);
	public:
	Game();
	void loadFEN(const string &FEN);
	void printBoard();
	std::vector<Move> calculateMoves();
};