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
	return (u.real() - v.real())*(u.real() - v.real()) + 
          (u.imag() - v.imag())*(u.imag() - v.imag());
}

struct Compare {
	Point p0; // Leftmost point of the poly
	bool operator ()(const Point &p1, const Point &p2) {
		double angle = (p1.real()-p0.real())*(p2.imag()-p0.imag())-(p1.imag()-p0.imag())*(p2.real()-p0.real());
		if (angle<0){
			//p2 before p1
			return false;
		}else if(angle==0){
			return (det(p0,p1) < det(p0,p2))? true : false;
		} 
		else{
			//p1 before p2
			return true;
		}
	}
};

bool inline salientAngle(Point &a, Point &b, Point &c) {
	// TODO
	double angle = (b.real()-a.real())*(c.imag()-a.imag())-(b.imag()-a.imag())*(c.real()-a.real());
	if (angle<0){
		return false;
	}else{
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////

Polygon convex_hull(std::vector<Point> &points) {
	Compare order;
	// TODO
	int minY = points[0].imag();
	int min = 0;
	for(int i = 1; i < points.size(); i++){
		if (points[i].imag()<=minY){
			if(points[i].imag() == minY){
				if(points[i].real() < points[min].real()){
					minY = points[i].imag();
					min = i;
				}
			}else{
				minY = points[i].imag();
				min = i;
			}
		}
	}
	order.p0 = Point(points[min].real(), points[min].imag());
	std::sort(points.begin(), points.end(), order);
	Polygon hull;
	// TODO
	int Psize = points.size();
	if (Psize <= 3){
		return points;
	}else{
		hull.push_back(points.at(0));
		hull.push_back(points.at(1));
		hull.push_back(points.at(2));
	}
	// use salientAngle(a, b, c) here
	for (int i = 3; i < Psize; i++){
		while (!salientAngle(hull.at(hull.size()-2), hull.back(), points.at(i))){
			hull.pop_back();
		}
		hull.push_back(points.at(i));
	}

	// for (it = hull.begin(); it!=hull.end(); it++){
	// 	cout << *it;
	// }
	// cout << endl;
	return hull;
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

void save_obj(const std::string &filename, Polygon &poly) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	for (const auto &v : poly) {
		out << "v " << v.real() << ' ' << v.imag() << " 0\n";
	}
	for (size_t i = 0; i < poly.size(); ++i) {
		out << "l " << i+1 << ' ' << 1+(i+1)%poly.size() << "\n";
	}
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon hull = convex_hull(points);
	save_obj(argv[2], hull);
	return 0;
}
