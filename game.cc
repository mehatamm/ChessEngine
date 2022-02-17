#include "game.h"
#include <sstream>
#include <iostream>
using std::string;
using std::make_unique;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;
Game::Game(){}
int getPos(int current, int vertical, int horizontal){ //calculates vertical, horizontal movements in array
	int file=current%8;
	int rank=current/8;
	rank-=vertical;
	file+=horizontal;
	if(rank<0 || rank>7 || file<0 || file>7) return -1;
	else return(rank*8+file);
}
bool Game::matchColor(int pos){ //helper function, return if color of piece on given square matches active color
	return((isupper(gameBoard[pos]) && whiteMove) || !(isupper(gameBoard[pos]) || whiteMove)); //disgusting logic because isupper is just nonzero and not 1
	//return false;
}
bool Game::slideMove(int newPos){
	return(newPos!=-1 && !gameBoard[newPos]);
}
bool Game::captureMove(int newPos){
	return(newPos!=-1 && (!gameBoard[newPos] || !matchColor(newPos)));
}
/*bool Game::inCheck(){
	for(int i=0;i<64;++i){
		if(gameBoard[i] && gameBoard[i]->type=='k' && matchColor(i)){ //idenfity king of active color
			
		}
	}
	return false;
}*/
void Game::loadFEN(const string &FEN){
	stringstream s(FEN);
	string bp; //boardpositions
	s >> bp;
	auto it=bp.cbegin();
	for(int i=0;i<8;++i){ //loop through ranks
		for(int k=0;k<8;++k){ //square k of rank i (from top of board)
			if(isdigit(*it)) k+=(*it-'0')-1; //if FEN is a digit, skip that many squares, char to digit conversion
			else{
				gameBoard[k+8*i]=*it;
			}
			++it;
		}
		++it;
	}
	char sc;
	s >> sc >> castlingAvailability >> ep >> halfmove >> fullmove;
	if(sc=='w') whiteMove=true;
	else whiteMove=false;
}
void Game::printBoard(){ //currently, run through all board positions and print them. add uppercase indication
	for(char i:gameBoard){
		if(i) cout << i << " ";
		else cout << 'X' << " ";
	}
}
vector<Move> Game::calculateMoves(){
	vector<Move> moveList;
	int newPos;
	int vdir;
	for(int i=0;i<64;++i){
		char curpiece=gameBoard[i];
		if(curpiece && matchColor(i)){ //if a piece exists and is of the active color
			if(tolower(curpiece)=='p'){ //current piece is a pawn
				if(curpiece=='P') vdir=1; else vdir=-1;
				if((vdir==1 && i/8==6) || (vdir==-1 && i/8==1)){ //if white pawn and in rank 2 or black pawn in rank 7
					newPos=getPos(i, vdir*2, 0);
					if(slideMove(newPos)){
						moveList.push_back(Move{i, newPos}); //add move to list
					}
				}
				newPos=getPos(i, vdir, 0);
				if(slideMove(newPos)){
					moveList.push_back(Move{i, newPos}); //add move to list
				}
			}
			
			else if(tolower(curpiece)=='r'){ //current piece is a rook
				int yincr=1;
				int xincr=0;
				for(int rm=0;rm<4;++rm){ //4 different directions, add moves until invalid or capture a piece
					if(rm==1) yincr=-1;
					else if(rm==2){yincr=0; xincr=1;}
					else if(rm==3) xincr=-1;
					newPos=i;
					while(true){
						newPos=getPos(newPos, yincr, xincr);
						if(!captureMove(newPos)) goto rookendloop;
					else if(gameBoard[newPos]){moveList.push_back(Move{i, newPos});goto rookendloop;}
						moveList.push_back(Move{i, newPos});
					}
					rookendloop:;
				}
			}
			
			else if(tolower(curpiece)=='b'){ //current piece is a bishop
				int yincr=1;
				int xincr=1;
				for(int bm=0;bm<4;++bm){
					if(bm==1) yincr=-1;
					else if(bm==2) xincr=-1;
					else if(bm==3) yincr=1;
					newPos=i;
					while(true){
						newPos=getPos(newPos, yincr, xincr);
						if(!captureMove(newPos)) goto bishopendloop;
						else if(gameBoard[newPos]){moveList.push_back(Move{i, newPos});goto bishopendloop;}
						moveList.push_back(Move{i, newPos});
					}
					bishopendloop:;
				}
			}
			else if(tolower(curpiece)=='q'){ //current piece is a queen
				int yincr=1;
				int xincr=0;
				for(int qm=0;qm<8;++qm){ //8 different moves
					if(qm==1) xincr=-1;
					else if(qm==2) xincr=1;
					else if(qm==3) yincr=-1;
					else if(qm==4) xincr=0;
					else if(qm==5) xincr=-1;
					else if(qm==6) yincr=0;
					else if(qm==7) xincr=1;
					newPos=i;
					while(true){
						newPos=getPos(newPos, yincr, xincr);
						if(!captureMove(newPos)) goto queenendloop;
						else if(gameBoard[newPos]){moveList.push_back(Move{i, newPos});goto queenendloop;}
						moveList.push_back(Move{i, newPos});
					}
					queenendloop:;
				}
			}
			else if(tolower(curpiece)=='n'){ //current piece is a knight
				int yincr=2;
				int xincr=1;
				for(int nm=0;nm<8;++nm){
					if(nm==1) yincr=-2;
					else if(nm==2) xincr=-1;
					else if(nm==3) yincr=2;
					else if(nm==4){yincr=1;xincr=2;}
					else if(nm==5) yincr=-1;
					else if(nm==6) xincr=-2;
					else if(nm==7) yincr=1;
					newPos=getPos(i, yincr, xincr);
					if(captureMove(newPos)){
						moveList.push_back(Move{i, newPos});
					}
				}
			}
			
		}
	}
	//see if in check
	return moveList;
}
int main(){
	Game g;
	g.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	vector<Move> openingMoves=g.calculateMoves();
	for(auto i:openingMoves){ //printing out all opening moves
		cout << i.startPos << " " << i.endPos << endl;
	}
	g.printBoard(); //printing out the state of the board
}