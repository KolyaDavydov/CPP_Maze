#include "pathfinder.h"

/**
 * @brief Поиск пути в лабиринте по алгоритму волнового поиска.
 * @param rightWalls указатель на матрицу правых стенок
 * @param bottomWalls указатель на матрицу нижних стенок
 * @param start координаты начала пути
 * @param end координата конца пути
 */
std::vector<s21::PathFinder::Point> s21::PathFinder::findPath(
    const std::vector<std::vector<int>>& rightWalls,
    const std::vector<std::vector<int>>& downWalls, const Point start,
    const Point end) {
  std::queue<Point> q;  // для хранения маршрута
  q.push(start);

  // матрица лабиринта с посещенными точками
  // изначально заполняем false
  std::vector<std::vector<bool>> visited(
      rightWalls.size(), std::vector<bool>(rightWalls[0].size(), false));
  // матрица лабиринта 'волны'
  // изначально заполняем 0
  // далее запоняется правильными ходами числами попорядку
  std::vector<std::vector<int>> wave(rightWalls.size(),
                                     std::vector<int>(rightWalls[0].size(), 0));

  while (!q.empty()) {
    Point current = q.front();  // текущая координата
    q.pop();

    int x = current.x_;
    int y = current.y_;

    // если добрались до конечной точки:
    if (x == end.x_ && y == end.y_) {
      // запихиваем в вектор пути сначала координаты конечной точки
      path_.push_back(end);

      // пока не дойдем до начальной координаты
      while (!(x == start.x_ && y == start.y_)) {
        // для хранения координат текущей ячейки лабиринта
        int dx = 0;
        int dy = 0;

        // последовательно идем по матрице 'wave' от большего к меньшему
        if (x > 0 && wave[x - 1][y] == wave[x][y] - 1 &&
            downWalls[x - 1][y] == 0) {
          dx = -1;
        } else if (x < (int)rightWalls.size() - 1 &&
                   wave[x + 1][y] == wave[x][y] - 1 && downWalls[x][y] == 0) {
          dx = 1;
        } else if (y > 0 && wave[x][y - 1] == wave[x][y] - 1 &&
                   rightWalls[x][y - 1] == 0) {
          dy = -1;
        } else if (y < (int)rightWalls[0].size() - 1 &&
                   wave[x][y + 1] == wave[x][y] - 1 && rightWalls[x][y] == 0) {
          dy = 1;
        }
        x += dx;
        y += dy;

        path_.insert(path_.begin(), Point(x, y));
      }
      return path_;
    }

    if (visited[x][y]) {
      continue;
    }

    visited[x][y] = true;

    // направление движения на одну ячейку (вправо, вниз, влево, вверх)
    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    for (const auto& dir : directions) {
      int dx = dir.first;
      int dy = dir.second;

      // координаты клетки куда предполагаем ход:
      int next_x = x + dx;
      int next_y = y + dy;

      // если если предполагаемый ход выходит за пределы лабиринта - пропуск
      if (next_x < 0 || next_x >= (int)rightWalls.size() || next_y < 0 ||
          next_y >= (int)rightWalls[0].size()) {
        continue;
      }
      // если предполагаемый ход вниз, но стоит стенка - пропуск
      if (dy == 1 && rightWalls[x][y] == 1) {
        continue;
      }
      // если предполагаемый ход вправо, но стоит стенка - пропуск
      if (dx == 1 && downWalls[x][y] == 1) {
        continue;
      }
      // если предполагаемый ход вверх, но стоит стенка - пропуск
      if (dy == -1 && rightWalls[x][y - 1] == 1) {
        continue;
      }
      // если предполагаемый ход влево, но стоит стенка - пропуск
      if (dx == -1 && downWalls[x - 1][y] == 1) {
        continue;
      }
      // если предполагаемый ход в клекте, которую мы ещё не посещяли
      // то добавляем его в общий путь
      if (!visited[next_x][next_y]) {
        wave[next_x][next_y] = wave[x][y] + 1;
        q.push(Point(next_x, next_y));
      }
    }
  }
  return std::vector<s21::PathFinder::Point>();
}