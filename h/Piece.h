#pragma once
class Piece {
public:
	static const int INVALID = -1;
	static const int   WHITE =  0;
	static const int   GRAY  =  1;

private:
	int owner;

public:
	Piece();
	Piece(int owner);

	int Owner() const;
};
