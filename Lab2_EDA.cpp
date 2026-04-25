//20-03-26_Laboratory 2: The objetive of this laboratory is analyze how space change as data dimnesionability (features) increase and how challenging it can be.
//Description -> We will carry out several experiments in C++ where we will work with data sets of diferent dimensions (10, 50, 100, 500, 1000, 2000, 5000). For each data set of a certain dimension d must be:
// - Generate 100 random points between 0 and 1 of d dimension.
// - Calculate the distance between all pairs of points (Euclidean distance) (Hint 4950 distances)
// - Generate a histogram (using Python) of the distances obtained for each dimension as in the figure shown below.
//Deriverable:
// - Report containing the graphs obtained for each dimension: 10, 50, 100, 500, 1000, 2000, 5000 and the corresponding analysis in PDF.
//   -> The report must be detailed explaining what is happening (analyze the changes in the distribution of distances, values on the axis x and y, etc), how does it affect us the course of dimensionality and you must include bibliographic citations in case you need them. 
// - Readeble code in zip file
#include <iostream>
#include <random> //for generate random numbers or random points
#include <vector> //for the Euclidian distance function and to store the points
#include <cmath> //the same
#include <string> //for the name of the file
#include <fstream> //to write the distances in a csv file

using namespace std;

//This function is used in laboratory 1
double EucladianDistance(const vector<double>& a, const vector<double>& b) {
	double sum = 0.0; 
	for(int i = 0; i < a.size(); i++) { 
		sum += pow(a[i] - b[i], 2);
	}
    
	return sqrt(sum); 
}

//This function generates the random points with help of the webpage cplusplus.com, calculates the distances and writes them in a csv file, take one dimension as parameter
void generarYCalcularDistancias(int d) {
    int numPoints = 100; //number of points to generate
    
    random_device rd; //random number generator
    mt19937 gen(rd()); //Mersenne Twister random number generator
    uniform_real_distribution<double> dist(0.0, 1.0); //uniform distribution between 0 and 1
    
    vector<vector<double>> points; //to store the generated points
    for(int i = 0; i < numPoints; i++) { //generate 100 random points
        vector<double> point; //to store the coordinates of the point
        for(int j = 0; j < d; j++) { //generate d coordinates for each point
            point.push_back(dist(gen)); //generate a random number between 0 and 1 and add it to the point
        }
        points.push_back(point); //add the point to the list of points
    }
    
    vector<double> distances; //to store the distances between all pairs of points
    for(int i = 0; i < numPoints; i++) { //calculate the distance between all pairs of points
        for(int j = i + 1; j < numPoints; j++) { //start from i + 1 because we don't want to calculate the distance between the same point and we don't want to calculate the distance twice
            double distance = EucladianDistance(points[i], points[j]); //calculate the distance between points i and j
            distances.push_back(distance); //add the distance to the list of distances
        }
    }
    
    string filename = "distancias_d" + to_string(d) + ".csv"; //name of the file to write the distances
    ofstream file(filename);//open the file for writing 
    
    for(double distance : distances) { //write the distnaces in the file, one distances per line
        file << distance << "\n";
    }

    file.close(); 
}

int main(){
    vector<int> dimen = {10, 50, 100, 500, 1000, 2000, 5000}; //dimensions to analyze
    
    for(int d : dimen) { // for each dimension, generate the points, calculate the distances and write them in a csv file
        generarYCalcularDistancias(d);
    }
    
    return 0;
}

