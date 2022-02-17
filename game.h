#include <memory>
#include <string>
#include <vector>
using std::string;
struct Move{
	int startPos;
	int endPos;
};
class Game{
	char gameBoard[64]={};
	bool whiteMove;
	string castlingAvailability;
	string ep;
	int halfmove;
	int fullmove;
	bool matchColor(int pos);
	bool slideMove(int newPos);
	bool captureMove(int newPos);
	//bool inCheck();
	public:
	Game();
	void loadFEN(const string &FEN);
	void printBoard();
	std::vector<Move> calculateMoves();
};