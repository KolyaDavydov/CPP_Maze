#ifndef A1_MAZE_SRC_MODEL_MODEL_H_
#define A1_MAZE_SRC_MODEL_MODEL_H_

#include <stdlib.h>

#include <fstream>
#include <string>
#include <vector>

#include "pathfinder.h"

namespace s21 {
class Maze {
 private:
  int rows_ = 0;
  int cols_ = 0;
  int counter_ = 0;
  std::vector<int> currentLine_;
  std::vector<std::vector<int>> rightWalls_;
  std::vector<std::vector<int>> bottomWalls_;

  bool randBool();
  void mergeSet(int, int);
  void createUniqueSet();
  bool searchIndeticalSet(int);
  int searchElemWithoutBottmWals(int, int);
  void createRightWals(int);
  void createBottomWals(int);
  void copyCurrentLine(int);
  void endLineCheck();

  std::vector<int> get_matrixRigth_line(int line_namber);
  std::vector<int> get_matrixBottom_line(int line_namber);
  std::string CreateStringFromVectorRigth(int iterator);
  std::string CreateStringFromVectorBottom(int iterator);

 public:
  Maze() = default;
  Maze(int rows, int cols)
      : rows_(rows),
        cols_(cols),
        currentLine_(cols_, 0),
        rightWalls_(rows, std::vector<int>(cols, 0)),
        bottomWalls_(rows, std::vector<int>(cols, 0)) {}
  ~Maze() {}

  void generateMaze();
  void saveMaze(std::string path);

  std::vector<std::vector<int>> getRightWalls();
  std::vector<std::vector<int>> getBottomWalls();

  void setRightWalls(std::vector<std::vector<int>> rightWalls) {
    rightWalls_ = rightWalls;
  }
  void setBottomWalls(std::vector<std::vector<int>> bottomWalls) {
    bottomWalls_ = bottomWalls;
  }
};
}  // namespace s21

#endif  // A1_MAZE_SRC_MODEL_MODEL_H_