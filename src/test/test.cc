#include <gtest/gtest.h>

#include "../controller/controller.h"

TEST(Maze, generation) {
  s21::Controller controller(20, 20);
  controller.generateMaze();
  EXPECT_EQ(controller.getRightWalls().size(), 20);
  EXPECT_EQ(controller.getBottomWalls().size(), 20);
}

TEST(Maze, save) {
  s21::Controller maze(3, 3);
  std::vector<std::vector<int>> matrixRight = {{1, 0, 1}, {0, 0, 1}, {0, 0, 1}};
  maze.setRightWalls(matrixRight);
  std::vector<std::vector<int>> matrixBottom = {
      {0, 1, 0}, {1, 0, 1}, {1, 1, 1}};
  maze.setBottomWalls(matrixBottom);
  maze.saveMaze("test/saved_maze.txt");

  std::ifstream file("test/saved_maze.txt");
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  EXPECT_EQ(lines[0], "3 3");
  EXPECT_EQ(lines[1], " 1 0 1");
  EXPECT_EQ(lines[2], " 0 0 1");
  EXPECT_EQ(lines[3], " 0 0 1");
  EXPECT_EQ(lines[4], "");
  EXPECT_EQ(lines[5], " 0 1 0");
  EXPECT_EQ(lines[6], " 1 0 1");
  EXPECT_EQ(lines[7], " 1 1 1");
}

TEST(Maze, open) {
  s21::Controller maze(3, 3);
  std::vector<std::vector<int>> matrixRight = {{1, 0, 1}, {0, 0, 1}, {0, 0, 1}};
  maze.setRightWalls(matrixRight);
  std::vector<std::vector<int>> matrixBottom = {
      {0, 1, 0}, {1, 0, 1}, {1, 1, 1}};

  s21::Controller::Point start = {0, 0};
  s21::Controller::Point end = {2, 0};

  std::vector<s21::Controller::Point> route;

  route = maze.findPath(matrixRight, matrixBottom, start, end);
  EXPECT_EQ(route.size(), 5);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}