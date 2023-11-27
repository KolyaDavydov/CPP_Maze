#include "controller.h"
namespace s21 {
void Controller::generateMaze() {
  Maze maze(this->rows_, this->cols_);
  maze.generateMaze();
  this->rightWalls_ = maze.getRightWalls();
  this->bottomWalls_ = maze.getBottomWalls();
};

std::vector<s21::Controller::Point> s21::Controller::findPath(
    const std::vector<std::vector<int>>& rightWalls_,
    const std::vector<std::vector<int>>& bottomWalls_, const Point start,
    const Point end) {
  PathFinder::Point start_p;
  start_p.x_ = start.x_;
  start_p.y_ = start.y_;

  PathFinder::Point end_p;
  end_p.x_ = end.x_;
  end_p.y_ = end.y_;
  PathFinder pathFinder(this->rightWalls_, this->bottomWalls_);
  std::vector<s21::PathFinder::Point> path =
      pathFinder.findPath(rightWalls_, bottomWalls_, start_p, end_p);
  Point p;
  for (int long unsigned i = 0; i < path.size(); ++i) {
    p.x_ = path[i].x_;
    p.y_ = path[i].y_;
    this->path_.push_back(p);
  }
  return this->path_;
};

std::vector<std::vector<int>> Controller::getRightWalls() {
  if (rightWalls_.size() < 1) return std::vector<std::vector<int>>();
  return rightWalls_;
};

std::vector<std::vector<int>> Controller::getBottomWalls() {
  if (bottomWalls_.size() < 1) return std::vector<std::vector<int>>();
  return bottomWalls_;
};
void Controller::saveMaze(std::string path) {
  Maze maze(this->rows_, this->cols_);
  maze.setBottomWalls(bottomWalls_);
  maze.setRightWalls(rightWalls_);
  maze.saveMaze(path);
};
}  // namespace s21