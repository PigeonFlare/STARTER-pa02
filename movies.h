#ifndef MOVIES_H
#define MOVIES_H

#include <string>
#include <map>
#include <vector>

struct Movie {
    std::string name;
    double rating;
    Movie(const std::string& n = "", double r = 0.0) : name(n), rating(r) {}
};

class MovieDatabase {
private:
    std::map<std::string, double> movies;

public:
    void addMovie(const std::string& name, double rating);
    void printAllMovies() const;
    std::vector<Movie> getMoviesWithPrefix(const std::string& prefix) const;
    Movie getBestMovieWithPrefix(const std::string& prefix) const;
};

#endif
