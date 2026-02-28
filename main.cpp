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

Parameters: 
n = number of movies
m = number of prefixes
k = max movies starting with a prefix
l = max length of a movie title

I used a std::map (which is a balanced BST) to store all n movies, keyed by their name.

For each of the m prefixes, the algorithm does the following:
1. Find the first movie >= prefix using lower_bound. This takes O(log n) comparisons, each costing O(l), so that's O(l * log n).
2. Iterate through the map to find the k matching movies. Checking the prefix takes O(l), so O(k * l).
3. Sort these k movies by rating/name for printing. This takes O(k * log k * l).
4. Scanning for the single best movie is also linear in k, so O(k * l).

Adding that all up for m prefixes gives a total time complexity of:
O(m * (l * log n + k * l + k * log k * l))

Simplifying this big expression by factoring out l and dropping lower order terms:
O(m * l * (log n + k * log k))

Here are the actual runtimes I measured on the CSIL machines using prefix_large.txt:
- input_20_random.csv: ~11 ms
- input_100_random.csv: ~12 ms
- input_1000_random.csv: ~12 ms
- input_76920_random.csv: ~145 ms

The trend makes sense because the runtime grows logically with n (log n factor) and the output size.

Part 3b: Space Complexity Analysis

My space usage is dominated by storing the movie titles themselves.
- Storing n movies in the map takes O(n * l).
- Storing m prefixes takes O(m * l).
- The temporary vector of matches uses at most O(k * l).
- Storing the list of best movies takes O(m * l).

So the Total Space is O(n * l + m * l + k * l).
Since n (total movies) is generally larger than m (prefixes) or k (matches), this simplifies to:
O(n * l)

Part 3c: Trade-off Analysis

I primarily designed this for low time complexity.
I chose a std::map because it keeps keys sorted. This lets me use lower_bound to jump straight to the relevant movies for any prefix, avoiding a scan of the whole list (which would be O(n)). This makes it much faster when n is large.

Was I able to achieve low space complexity as well?
Yes, I think so. The space complexity is O(n * l), which is about the minimum required to store the input data. The map adds small overhead for the tree structure, but it doesn't duplicate the string data significantly like a trie might if implemented naively with full pointers. So it strikes a good balance of fast lookups without using lots of RAM.
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
