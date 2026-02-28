#include "movies.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void MovieDatabase::addMovie(const std::string& name, double rating) {
    movies[name] = rating;
}

void MovieDatabase::printAllMovies() const {
    for (const auto& pair : movies) {
        std::cout << pair.first << ", " << std::fixed << std::setprecision(1) << pair.second << std::endl;
    }
}

std::vector<Movie> MovieDatabase::getMoviesWithPrefix(const std::string& prefix) const {
    std::vector<Movie> result;
    auto it = movies.lower_bound(prefix);
    while (it != movies.end() && it->first.substr(0, prefix.size()) == prefix) {
        result.emplace_back(it->first, it->second);
        ++it;
    }
    std::sort(result.begin(), result.end(), [](const Movie& a, const Movie& b) {
        if (a.rating != b.rating) return a.rating > b.rating;
        return a.name < b.name;
    });
    return result;
}

Movie MovieDatabase::getBestMovieWithPrefix(const std::string& prefix) const {
    Movie best("", -1.0);
    auto it = movies.lower_bound(prefix);
    while (it != movies.end() && it->first.substr(0, prefix.size()) == prefix) {
        if (it->second > best.rating || (it->second == best.rating && it->first < best.name)) {
            best = Movie(it->first, it->second);
        }
        ++it;
    }
    return best;
}
