#ifndef A1_MAZE_SRC_MODEL_PATHFINDER_H_
#define A1_MAZE_SRC_MODEL_PATHFINDER_H_

#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

namespace s21 {
class PathFinder {
 public:
  PathFinder() = default;
  PathFinder(std::vector<std::vector<int>> rightWalls,
             std::vector<std::vector<int>> bottomWalls)
      : rightWalls_(rightWalls), bottomWalls_(bottomWalls){};
  ~PathFinder(){};

  struct Point {
    int x_ = 0;
    int y_ = 0;
    Point() = default;
    Point(int x, int y) : x_(x), y_(y){};
  };
  Point start;
  Point end;

  std::vector<Point> findPath(const std::vector<std::vector<int>>& rightWalls_,
                              const std::vector<std::vector<int>>& bottomWalls_,
                              const Point start, const Point end);

  const std::vector<s21::PathFinder::Point>& getPath() { return path_; }

 private:
  Point start_;
  Point end_;
  std::vector<PathFinder::Point> path_;
  std::vector<std::vector<int>> rightWalls_;
  std::vector<std::vector<int>> bottomWalls_;
};

}  // namespace s21
#endif  // A1_MAZE_SRC_MODEL_PATHFINDER_H_