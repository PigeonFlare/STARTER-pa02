// Winter'24
// Instructor: Diba Mirza
// Student name: Henry Li
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    MovieDatabase db;

    string line, movieName;
    double movieRating;
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        db.addMovie(movieName, movieRating);
    }

    movieFile.close();

    if (argc == 2){
        db.printAllMovies();
        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    vector<pair<string, Movie>> bestMovies;

    for (const string& prefix : prefixes) {
        vector<Movie> matches = db.getMoviesWithPrefix(prefix);
        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        } else {
            for (const Movie& m : matches) {
                cout << m.name << ", " << fixed << setprecision(1) << m.rating << endl;
            }
            cout << endl;
        }
        Movie best = db.getBestMovieWithPrefix(prefix);
        if (best.rating >= 0) {
            bestMovies.push_back({prefix, best});
        }
    }

    for (const auto& p : bestMovies) {
        cout << "Best movie with prefix " << p.first << " is: " << p.second.name << " with rating " << fixed << setprecision(1) << p.second.rating << endl;
    }

    return 0;
}

/*
Part 3a: Time Complexity Analysis
My time complexity can be affected by all of the following parameters: n (movies), m (prefixes), k (max movie matches per prefix), and l (max title length). Using a sorted map, each prefix search involves a binary search O(l·log n), a scan of its k matches   O(k·l), and a sort of those matches O(k·log k·l). The latter sort dominates time complexity, so one prefix is reflected by O(l·log n + k·log k·l), while all m prefixes are reflected by O(m·l·(log n + k·log k)). 

Running times measured with prefix_large.txt and the following input files:
input_20_random.csv: 0.011 s
input_100_random.csv: 0.011 s
input_1000_random.csv: 0.013 s
input_76920_random.csv: 0.140 s
The upward trend in my measured runtimes aligns with the upward trend in my proposed time complexity. As n and k increase, log n and k * log k inflate, driving an overall increase in time complexity.

Part 3b: Space Complexity Analysis
All movie titles, ratings, and prefixes in the map are stored, as well as a temporary match list. This gives an overall space complexity of O(n·l), where m = prefixes and k = max movie matches per prefix are negligible add-ons. 

Part 3c: Explore trade-offs
I designed my algorithm for a low time complexity since there isn't really a space constraint inherent to or evaluated by the problem. I achieved a lower time complexity using a sorted map so that each prefix is jumped to instead of all n being scanned. I was able to achieve a somewhat low space complexity as well using by storing my data in keyed maps, efficiently storing data so that the space stays close to the input size O(n*l).
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
