#ifndef LAB3_COORDINATES_H
#define LAB3_COORDINATES_H

using namespace std;

class Coordinates {
public:
	int x;
	int y;
	Coordinates(int x, int y) : x{ x }, y{ y } {};
	Coordinates() = default;
	void setXY(int x, int y) { this->x = x; this->y = y; }
	//bool operator==(const Coordinates& r) { return (this->x == r.x) && (this->y == r.y); }
	Coordinates operator-(const Coordinates& r) { 
		Coordinates temp;
		temp.x = this->x - r.x;
		temp.y = this->y - r.y;
		return temp;
	}
	friend bool operator == (const Coordinates& r, const Coordinates& l) {
		return ((r.x == l.x) && (r.y == l.y));
	}
	friend bool operator != (const Coordinates& r, const Coordinates& l) {
		return !(r == l);
	}
	friend bool operator < (const Coordinates& r, const Coordinates& l) {
		return ((r.x < l.x) && (r.y < l.y));
	}
};

#endif