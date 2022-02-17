#include "game.h"
#include <sstream>
#include <iostream>
using std::string;
using std::make_unique;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;
Piece::Piece(char color, char type, bool moved):color{color}, type{type}, moved{moved}{}
Game::Game(){}
int getPos(int current, int vertical, int horizontal){
	int file=current%8;
	int rank=current/8;
	rank-=vertical;
	file+=horizontal;
	if(rank<0 || rank>7 || file<0 || file>7) return -1;
	else return(rank*8+file);
}
bool Game::matchColor(int pos){ //helper function, return if color of piece on given square matches active color
	return(gameBoard[pos]->color==activeColor);
	//return false;
}
bool Game::slideMove(int newPos){
	return(newPos!=-1 && !gameBoard[newPos]);
}
bool Game::captureMove(int newPos){
	return(newPos!=-1 && (!gameBoard[newPos] || !matchColor(newPos)));
}
void Game::loadFEN(const string &FEN){
	stringstream s(FEN);
	string bp; //boardpositions
	s >> bp;
	auto it=bp.cbegin();
	for(int i=0;i<8;++i){ //loop through ranks
		for(int k=0;k<8;++k){ //square k of rank i (from top of board)
			if(isdigit(*it)) k+=(*it-'0')-1; //if FEN is a digit, skip that many squares, char to digit conversion
			else{
				char color='b'; //color 1 is black, color 0 is white
				if(isupper(*it)) color='w';
				gameBoard[k+8*i]=make_unique<Piece>(color, tolower(*it), false);
			}
			++it;
		}
		++it;
	}
	s >> activeColor >> castlingAvailability >> ep >> halfmove >> fullmove;
}
void Game::printBoard(){ //currently, run through all board positions and print them. add uppercase indication
	for(auto &i:gameBoard){
		if(i) cout << i->type << " ";
		else cout << 'X' << " ";
	}
}
vector<Move> Game::calculateMoves(){
	vector<Move> moveList;
	int vdir;
	int newPos;
	for(int i=0;i<64;++i){
		auto &curpiece=gameBoard[i];
		if(curpiece && curpiece->color==activeColor){ //if a piece exists and is of the active color
			if(curpiece->color=='w') vdir=1; else vdir=-1;
			if(curpiece->type=='p'){ //current piece is a pawn
				if(!curpiece->moved){
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
			
			else if(curpiece->type=='r'){ //current piece is a rook
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
			
			else if(curpiece->type=='b'){ //current piece is a bishop
				int yincr=1;
				int xincr=1;
				for(int rm=0;rm<4;++rm){
					if(rm==1) yincr=-1;
					else if(rm==2) xincr=-1;
					else if(rm==3) yincr=1;
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
			
		}
	}
	return moveList;
}
int main(){
	Game g;
	g.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	vector<Move> openingMoves=g.calculateMoves();
	for(auto i:openingMoves){
		cout << i.startPos << " " << i.endPos << endl;
	}
	g.printBoard();
}