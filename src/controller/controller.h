#ifndef A1_MAZE_SRC_CONTROLLER_CONTROLLER_H_
#define A1_MAZE_SRC_CONTROLLER_CONTROLLER_H_

#include "../model/model.h"
#include "../model/pathfinder.h"
namespace s21 {

class Controller {
 public:
  Controller(){};
  Controller(int rows, int cols)
      : rows_(rows),
        cols_(cols),
        rightWalls_(rows, std::vector<int>(cols, 0)),
        bottomWalls_(rows, std::vector<int>(cols, 0)){};
  Controller(std::vector<std::vector<int>> rightWalls,
             std::vector<std::vector<int>> bottomWalls)
      : rightWalls_(rightWalls), bottomWalls_(bottomWalls){};
  ~Controller(){};

  struct Point {
    int x_ = 0;
    int y_ = 0;
    Point() = default;
    Point(int x, int y) : x_(x), y_(y){};
  };

  std::vector<Point> findPath(const std::vector<std::vector<int>>& rightWalls_,
                              const std::vector<std::vector<int>>& bottomWalls_,
                              const Point start, const Point end);
  std::vector<std::vector<int>> getRightWalls();
  std::vector<std::vector<int>> getBottomWalls();

  const std::vector<s21::Controller::Point>& getPath() { return path_; }

  void setRightWalls(std::vector<std::vector<int>> rightWalls) {
    rightWalls_ = rightWalls;
  }
  void setBottomWalls(std::vector<std::vector<int>> bottomWalls) {
    bottomWalls_ = bottomWalls;
  }
  void saveMaze(std::string path);
  void generateMaze();

 private:
  Maze maze_;
  PathFinder pathFinder_;
  int rows_ = 0;
  int cols_ = 0;
  std::vector<std::vector<int>> rightWalls_;
  std::vector<std::vector<int>> bottomWalls_;
  Point start_;
  Point end_;
  std::vector<Controller::Point> path_;
};
}  // namespace s21

#endif  // A1_MAZE_SRC_CONTROLLER_CONTROLLER_H_