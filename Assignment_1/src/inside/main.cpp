////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;
std::vector<Point>::iterator it;

double inline det(const Point &u, const Point &v) {
	// TODO
	return 0;
}

int inline salientAngle(const Point &a,const Point &b,const Point &c) {
	// TODO
	double angle = (b.real()-a.real())*(c.imag()-a.imag())-(b.imag()-a.imag())*(c.real()-a.real());
	if (angle<0){
		return -1;
	}else if(angle==0){
		return 0;
	}else{
		return 1;
	}
}

bool inline online(const Point &a,const Point &b,const Point &c){
	if (c.real() <= max(a.real(), b.real()) && c.real() >= min(a.real(), b.real()) && 
        c.imag() <= max(a.imag(), b.imag()) && c.imag() >= min(a.imag(), b.imag())){
		return true; 
	}else{
    	return false; 
	}

}

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	int ab1 = salientAngle(a,b,c);
	int ab2 = salientAngle(a,b,d);
	int cd1 = salientAngle(c,d,a);
	int cd2 = salientAngle(c,d,b);

	if (ab1!=ab2 && cd1!=cd2){
		return true;
	}

	if(ab1 == 0 && online(a,b,c)){
		return true;
	}

	if(ab2 == 0 && online(a,b,d)){
		return true;
	}

	if(cd1 == 0 && online(c,d,a)){
		return true;
	}

	if(cd2 == 0 && online(c,d,b)){
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
	// 1. Compute bounding box and set coordinate of a point outside the polygon
	// TODO
	double maxX = poly[0].real();
	double maxY = poly[0].imag();
	for (int i = 1; i < poly.size(); i++){
		if (poly[i].real() > maxX){
			maxX = poly[i].real();
		}
		if (poly[i].imag() > maxY){
			maxY = poly[i].imag();
		}
	}
	Point outside(maxX*1000, maxY*1000);
	// 2. Cast a ray from the query point to the 'outside' point, count number of intersections
	// TODO
	int interCount = 0;
	int Psize = poly.size();
	Point ans;
	for (int i = 0; i < Psize; i++){
		if(intersect_segment(poly[i%Psize],poly[(i+1)%Psize],query,outside,ans)){
			interCount++;
		}
	}
	if (interCount%2 == 0){
		return false;
	}else{
		return true;
	}
	
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);
	// TODO
	std::string line1;
	std::getline(in,line1);
	int dotNum = std::stoi(line1);
	for (int i = 0; i < dotNum; i++){
		double a , b , c;
		in >> a >> b >> c;
		points.push_back(Point(a,b));
	}
	return points;
}

Polygon load_obj(const std::string &filename) {
	std::ifstream in(filename);
	// TODO
	Polygon poly;
	std::string line;
	int polyPoints=0;
	std::vector<Point> xy;
	while (std::getline(in, line)){
		double a, b;
   		std::string token;
   		std::istringstream tokenStream(line);
		std::getline(tokenStream, token, ' ');
	    if (!token.compare("v")){
	    	std::getline(tokenStream, token, ' ');
	    	a = std::stod(token);
	    	std::getline(tokenStream, token, ' ');
	    	b = std::stod(token);
	    	xy.push_back(Point(a,b));
	    	polyPoints++;
	    }else if(!token.compare("f")){
	    	for (int i = 0; i < polyPoints; i++){
	    		std::getline(tokenStream, token, ' ');
	    		int index = std::stoi(token);
	    		poly.push_back(xy[index-1]);
	    	}
	    }else{
			cout << "Error!" << endl;
	    	break;
	    }
	}
	// for (it = poly.begin(); it!=poly.end(); it++){
	// 	cout << *it;
	// }
	// cout << endl;
	return poly;
}

void save_xyz(const std::string &filename, const std::vector<Point> &points) {
	// TODO
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << points.size() << endl;
	for (int i = 0; i < points.size(); i++){
		out << points[i].real() << " " <<points[i].imag() << " 0" << endl;
	}
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
