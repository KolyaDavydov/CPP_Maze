#include "model.h"

#include <iostream>
/*
Данный модуль создает лабиринт по алгоритму Эллера.
Лабиринт в цифровом формате представляет собой два вектора веткоров (или две
матрицы): std::vector<std::vector<int>> rightWalls_;
            std::vector<std::vector<int>> bottomWalls_;
эти матрицы содержат ячейки клеток лабиринта, где
rightWalls_ - ячейка == 1, если у правой стенки ячейки стоит стенка
bottomWalls_ - ячейка == 1, если у нижней стенки ячейки стоит стенка
если == 0, то стенок нет.
*/
namespace s21 {
/**
 * @brief основной метод для генерации лабиринта в виде матриц:
 *   std::vector<std::vector<int>> rightWalls_;
 *   std::vector<std::vector<int>> bottomWalls_;
 */
void Maze::generateMaze() {
  for (int i = 0; i < rows_; i++) {
    if (i == rows_ - 1) {
      createRightWals(i);
      endLineCheck();
    } else {
      createRightWals(i);
      createBottomWals(i);
      copyCurrentLine(i);
    }
  }
}

/**
 * @brief генератор случайного булевого значения
 *
 * @return случайное значение true или false
 */
bool Maze::randBool() {
  int num = rand();
  bool b = true;
  if (num % 2 == 0) {
    b = false;
  } else {
    b = true;
  }
  return b;
}

/**
 * @brief заполняет матрицу описывающий где должна
 * быть стенка справа по алгоритму Эллера
 * @param row текущая строк в лабиринте
 */
void Maze::createRightWals(int row) {
  if (row == 0) {
    createUniqueSet();
  }
  for (int i = 0; i < cols_ - 1; ++i) {
    bool choise = randBool();
    if (choise == true || (currentLine_[i] == currentLine_[i + 1])) {
      rightWalls_[row][i] = 1;  //}
    } else if (choise == false) {
      mergeSet(currentLine_[i], currentLine_[i + 1]);
    }
  }
  rightWalls_[row][cols_ - 1] = 1;
}

/**
 * @brief заполняет вектор векторов описывающий где должна
 * быть стенка снизу по алгоритму Эллера
 * @param row текущая строка в лабиринте
 */
void Maze::createBottomWals(int row) {
  for (int i = 0; i < cols_; i++) {
    bool choise = randBool();
    if (choise == true && searchIndeticalSet(currentLine_[i])) {
      bottomWalls_[row][i] = 1;
    }
    if (searchElemWithoutBottmWals(currentLine_[i], row) == 0) {
      bottomWalls_[row][i] = 0;
    }
  }
}

/**
 * @brief создает множество элементов (целое число) и вносит это множество
 * в каждую новую строку-вектор currentLine_ согласно алгоритму Эллера
 */
void Maze::createUniqueSet() {
  for (auto &elem : currentLine_) {
    if (elem == 0) {
      elem = ++counter_;
    }
  }
}

/**
 * @brief делает равными два элемента текущего множества currentLine_
 * если того требует правило Эллера
 * @param current текущий элемент множества
 * @param currentNext слудующий элемент множества
 */
void Maze::mergeSet(int current, int currentNext) {
  for (int i = 0; i < cols_; i++) {
    if (currentLine_[i] == currentNext) {
      currentLine_[i] = current;
    }
  }
}

/**
 * @brief считает сколько текущих элементов в данном множестве
 * @param value текущий элемент множества
 *
 * @return true если value не единственный в этом множестве
 */
bool Maze::searchIndeticalSet(int value) {
  int countIndeticalSet = 0;
  for (int i = 0; i < cols_; i++) {
    if (currentLine_[i] == value) {
      countIndeticalSet++;
    }
  }
  return countIndeticalSet > 1 ? true : false;
}

/**
 * @brief считает сколько текущих элементов в данном множестве с нижней границей
 * @param value текущий элемент множества
 * @param row текущая строка в лабиринте
 *
 * @return количество элементов(value) в данном множестве без нижней границы
 */
int Maze::searchElemWithoutBottmWals(int value, int row) {
  int countElem = 0;
  for (int i = 0; i < cols_; i++) {
    if (currentLine_[i] == value && bottomWalls_[row][i] == 0) countElem++;
  }
  return countElem;
}

/**
 * @brief копирует элементы из текущий заполненой строки лабиринта в
 * currentLine_ при этом присваивая уникальные значения попорядку
 * @param numLine текущая строка массива @param buttomWalls_
 */
void Maze::copyCurrentLine(int numLine) {
  for (int i = 0; i < cols_; i++) {
    counter_++;
    if (bottomWalls_[numLine][i] == 1) {
      currentLine_[i] = counter_;
    }
  }
}

/**
 *  @brief Gроверяет и обрабатывает конец строки в лабиринте.
 * Устанавливает соответствующие значения для правых и нижних стен,
 * включая объединение множеств элементов.
 * */
void Maze::endLineCheck() {
  for (int i = 0; i < cols_ - 1; i++) {
    if (currentLine_[i] != currentLine_[i + 1]) {
      rightWalls_[rows_ - 1][i] = 0;
      mergeSet(currentLine_[i], currentLine_[i + 1]);
    }
    
  }
  for (int i = 0; i < cols_; ++i) {
    bottomWalls_[rows_ - 1][i] = 1;
  }
  rightWalls_[rows_ - 1][cols_ - 1] = 1;
};

/**
 *  @brief Геттер
 * @return массив со значением стенок справа
 * */
std::vector<std::vector<int>> Maze::getRightWalls() {
  if (rightWalls_.size() < 1) return std::vector<std::vector<int>>();
  return rightWalls_;
}

/**
 *  @brief Геттер
 * @return массив со значением стенок снизу
 * */
std::vector<std::vector<int>> Maze::getBottomWalls() {
  if (bottomWalls_.size() < 1) return std::vector<std::vector<int>>();
  return bottomWalls_;
}

/**
 *  @brief сохранение лабиринта в .txt формат
 *
 * @param path относительный путь и название файла куда сохранить
 * */
void Maze::saveMaze(std::string path) {
  std::ofstream file;
  file.open(path);
  if (file.is_open()) {
    int iterator = 0;
    file << rows_ << " " << cols_ << std::endl;
    while (iterator < rows_) {
      std::string current_string = "\0";
      current_string = CreateStringFromVectorRigth(iterator);
      file << current_string << std::endl;
      ++iterator;
    }
    file << std::endl;
    iterator = 0;
    while (iterator < rows_) {
      std::string current_string = "\0";
      current_string = CreateStringFromVectorBottom(iterator);
      file << current_string << std::endl;
      ++iterator;
    }
    file.close();
  }
}

/**
 *  @brief получает строку матрицы для правых границ
 *
 * @param line_namber номер строки матрицы
 *
 * @return строка матрицы в виде вектора
 * */
std::vector<int> Maze::get_matrixRigth_line(int line_namber) {
  std::vector<int> current;
  for (long unsigned int it = 0; it < rightWalls_[line_namber].size(); ++it) {
    current.push_back(rightWalls_[line_namber][it]);
  }
  current.shrink_to_fit();
  return current;
}

/**
 *  @brief получает строку матрицы для нижних границ
 *
 * @param line_namber номер строки матрицы
 *
 * @return строка матрицы в виде вектора
 * */
std::vector<int> Maze::get_matrixBottom_line(int line_namber) {
  std::vector<int> current;
  for (long unsigned int it = 0; it < bottomWalls_[line_namber].size(); ++it) {
    current.push_back(bottomWalls_[line_namber][it]);
  }
  current.shrink_to_fit();
  return current;
}

std::string Maze::CreateStringFromVectorRigth(int iterator) {
  std::string resultString = " ";
  std::vector<int> current_line = get_matrixRigth_line(iterator);
  for (long unsigned int it = 0; it < current_line.size(); ++it) {
    if (current_line[it] == 1) {
      resultString += "1";
    } else {
      resultString += "0";
    }
    resultString += " ";
  }
  if (resultString.back() == ' ') {
    resultString.pop_back();
  }

  return resultString;
}

std::string Maze::CreateStringFromVectorBottom(int iterator) {
  std::string resultString = " ";
  std::vector<int> current_line = get_matrixBottom_line(iterator);
  for (long unsigned int it = 0; it < current_line.size(); ++it) {
    if (current_line[it] == 1) {
      resultString += "1";
    } else {
      resultString += "0";
    }
    resultString += " ";
  }
  if (resultString.back() == ' ') {
    resultString.pop_back();
  }
  return resultString;
}

}  // namespace s21
