//03-13-26_Laboratory 1: The objetive of this laboratory is analyze how to work with similarity measures in multidimensional data
//Steps:
//1. Implemates the Eucladian distance function that works in several dimensions.
//2. Read from a csv file multidimensional data. The file has only one numeric data per line.
//3. Find which files are the most similar to 1.csv
//4. Complete de form

#include <iostream>
#include <vector> //To handle manage multidimensional data
#include <cmath> //To use the power and the square root
#include <fstream> //To write and open files  
#include <sstream> //To write and open files but you use RAM memory instead of normal memory 
#include <limits> //To know the maximum and minimum possible value of a double

using namespace std;

/*
First: we created a Eucladean fuction, this function is a double function because the distance can be a decimal number, and it receives two vectors 
of doubles as parameters, and this parameters are const because we don't want to modify the data, and they are reference because we don't want to copy the data.  
*/
double EucladianDistance(const vector<double>& a, const vector<double>& b) {
	double sum = 0.0; //Sumatory of the differences
	for(int i = 0; i < a.size(); i++) { //We iterate through the vectors and we calculate the difference between each element with the fomula
		sum += pow(a[i] - b[i], 2);
	}
	return sqrt(sum); //We return the square root of the sum to get the final distance
}

/*
In this function we read the data from a csv file, we create a vector of doubles to store the data
*/
vector<double> Reading(const string& filename ) {
	vector<double> data; //Vector to store the data
	ifstream file(filename); //We open the file with an ifstream object, this object is used to read files, and we pass the filename as a parameter
	string line; //We create a string variable to store each line of the file
	while(getline(file, line)) { //We use a while loop to read each line of the file, and we use the getline function to read the line and store it in the variable line
		if(!line.empty()) { //We check if the line is not empty, because if the line is empty we don't want to try to convert it to a double
			data.push_back(stod(line)); //If the line is not empty we convert it to a double with the stod function, and we add it to the vector with the push_back function
		} //Important: the stod function serves to convert a string to a double, and it can throw an exception if the string is not a valid number
	}
	return data; //We return the vector with the data read from the file
}

int main() {
	vector<string> archives = {"2.csv", "3.csv", "4.csv"}; //We create a vector of strings to store the names of the files we want to compare with 1.csv
	vector<double> principal = Reading("1.csv"); //We read the data from 1.csv and we store it in a vector of doubles, this vector will be our reference to compare with the other files
	string similar = ""; //We create a string variable to store the name of the most similar file, we initialize it with an empty string
	double max_cap = numeric_limits<double>::max(); //We create a doouble variable to store the maximum distance found, we initialize it with the maximum possible value of a double, this way any distance we find will be smaller than this value

	for(const string& archive : archives) { //We use a for loop to iterate through the vector of file names, and we use a reference to avoid copying the string
		vector<double> the_data = Reading(archive); //We read the data from the current file and we store it in a vector of doubles
		double dist = EucladianDistance(principal, the_data); //We calculate the distance between the data from 1.csv and the data from the current file, we store the result in a double variable
		cout << "Distance from first file to " << archive << ": " << dist << endl; // We print the distance to the console

		if(dist < max_cap) { //We check if the distance we just calculated is smaller than the maximum distance found so far, if it is smaller we update the maximum distance and we update the name of the most similar file
			max_cap = dist;
			similar = archive;
		}
	}

	cout << "\nThe file most similar to 1.csv is: " << similar << endl; //After we finish the loop we print the name of the most similar file to the console
	return 0;
}
