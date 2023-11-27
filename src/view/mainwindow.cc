#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  drawingWidget = new DrawingWidget(this);
}

MainWindow::~MainWindow() { delete ui; }

MazeData mazeData;
int flag = 0;

/**
 * @brief 'слушает' в какой ячейке лабиринта нажимается мышь
 * и если нажатия было 2 вызывает метод отрисовки пути
 * @param event слушает клик мышью
 */
void MainWindow::mousePressEvent(QMouseEvent *event) {
  int x = 0, y = 0;
  int res_x = 0, res_y = 0;
  int finish_x = 0, finish_y = 0;
  if (mazeLoaded_) {
    // размер ячейки лабиринта в высоту в пикселях
    float cellSizeBottom = 500.0 / mazeData.get_width();
    // размер ячейки лабиринта в ширину в пикселях
    float cellSizeRight = 500.0 / mazeData.get_length();
    // если нажали левой кнопкой мыши
    if (event->button() == Qt::LeftButton) {
      // записываем координаты мыши в пикселях
      x = event->pos().x();
      y = event->pos().y();

      // координаты середины ячейки
      res_x = x / cellSizeBottom + 1;
      res_y = y / cellSizeRight + 1;

      if (x < 501 && y < 501) {
        if (countClickMouse_ == 0 || countClickMouse_ == 2) {
          mazeData.set_start_point(res_x, res_y);
          countClickMouse_++;

          if (countClickMouse_ == 3) {
            finish_x = 0;
            finish_y = 0;
            mazeData.CleanPath();
            QWidget::update();
            countClickMouse_ = 1;
          }
        } else {
          finish_x = res_x;
          finish_y = res_y;
          mazeData.set_finish_point(finish_x, finish_y);
          countClickMouse_++;
        }
      }
      if (countClickMouse_ == 2) {
        findRoute();
      }
    } else {
      event->ignore();
      finish_x = 0;
      finish_y = 0;
      res_x = 0;
      res_y = 0;
      mazeData.CleanPath();
      countClickMouse_ = 0;
    }
  }
}

/**
 * @brief Записивает путь в лабиринте
 * вызывает метод для заполнения пути
 * и прорисовывает путь
 */
void MainWindow::findRoute() {
  MazeData::Point point = mazeData.get_start_point();
  // s21::PathFinder::Point start;
  s21::Controller::Point start;
  start.x_ = point.x - 1;
  start.y_ = point.y - 1;
  point = mazeData.get_finish_point();
  // s21::PathFinder::Point end;
  s21::Controller::Point end;
  end.x_ = point.x - 1;
  end.y_ = point.y - 1;

  // s21::PathFinder pathFinder(mazeData.matrixRight, mazeData.matrixBottom);
  s21::Controller controller(mazeData.matrixRight, mazeData.matrixBottom);
  // std::vector<s21::PathFinder::Point> path =
  //     pathFinder.findPath(mazeData.matrixRight, mazeData.matrixBottom, start,
  //     end);
  std::vector<s21::Controller::Point> path = controller.findPath(
      mazeData.matrixRight, mazeData.matrixBottom, start, end);
  mazeData.set_path(path);
  QWidget::update();
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  if (mazeData.path_to_file_ != "\0" || flag == 1) {
    painter.fillRect(rect(), Qt::gray);
    painter.setPen(QPen(Qt::black, 2));

    float cellSizeVertic = 500.0 / mazeData.get_length();
    float cellSizeHorizont = 500.0 / mazeData.get_width();

    painter.drawRect(0, 0, 500, 500);
    for (int i = 0; i < mazeData.matrixRight[0].size(); i++) {
      for (int j = 0; j < mazeData.matrixRight.size(); j++) {
        int x = j * cellSizeHorizont;
        int y = i * cellSizeVertic;

        if (mazeData.matrixRight[j][i]) {
          painter.drawLine(x, y + cellSizeVertic, x + cellSizeHorizont,
                           y + cellSizeVertic);
        }

        if (mazeData.matrixBottom[j][i]) {
          painter.drawLine(x + cellSizeHorizont, y, x + cellSizeHorizont,
                           y + cellSizeVertic);
        }
      }
    }
    painter.setPen(QPen(Qt::green, 2));

    if (mazeData.get_path_size() > 1) {
      // std::vector<s21::PathFinder::Point> current_path = mazeData.get_path();
      std::vector<s21::Controller::Point> current_path = mazeData.get_path();

      for (unsigned int i = 0; i < current_path.size() - 1; ++i) {
        int startX =
            current_path[i].x_ * cellSizeHorizont + cellSizeHorizont / 2;
        int startY = current_path[i].y_ * cellSizeVertic + cellSizeVertic / 2;
        int endX =
            current_path[i + 1].x_ * cellSizeHorizont + cellSizeHorizont / 2;
        int endY = current_path[i + 1].y_ * cellSizeVertic + cellSizeVertic / 2;
        painter.drawLine(startX, startY, endX, endY);
      }
    }
  }
}

/**
 * @brief открывает файл .txt с описанием лабиринта
 */
void MainWindow::on_OpenMaze_clicked() {
  countClickMouse_ = 0;
  mazeData.CleanPath();
  QString desktopPath =
      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  mazeData.path_to_file_ = QFileDialog::getOpenFileName(nullptr, "Open file",
                                                        desktopPath, "(*.txt)");
  QFile file(mazeData.path_to_file_);
  if (file.exists()) {
    mazeData.GetValues();
    ui->Size_x->setValue(mazeData.get_length());
    ui->Size_y->setValue(mazeData.get_width());
    mazeData.set_start_point(0, 0);
    mazeData.set_finish_point(0, 0);
    mazeLoaded_ = true;
    QWidget::update();
  }
}

/**
 * @brief проверка параметров перед отрисовкой лабиринта
 */
void MazeData::CheckingDataBeforeFill() {
  if (get_length() != 0) {
    set_length(0);
  }
  if (get_width() != 0) {
    set_width(0);
  }
  if (!matrixRight.empty()) {
    matrixRight.clear();
  }
  if (!matrixBottom.empty()) {
    matrixBottom.clear();
  }
}

/**
 * @brief записывает два числа (ширину и высоту лабиринта)
 * в соответтсвующие поля класса MazeData
 * @param parametrs_string первая строка из .txt файла с шириной и высотой
 */
void MazeData::GetMatrixParametrs(std::string parametrs_string) {
  std::string number_one = "\0";
  std::string number_two = "\0";
  size_t i = 0;
  for (size_t it = 0; it < parametrs_string.length(); ++it) {
    if (std::isdigit(parametrs_string[it]) || parametrs_string[it] == ' ') {
      while (std::isdigit(parametrs_string[it]) &&
             parametrs_string[it] != ' ') {
        number_one[i] = parametrs_string[it];
        it++;
        i++;
      }
      i = 0;
      if (parametrs_string[it] == ' ') {
        it++;
      }
      while (std::isdigit(parametrs_string[it])) {
        number_two[i] = parametrs_string[it];
        it++;
        i++;
      }
    }
  }
  set_length(std::stod(number_one));
  set_width(std::stod(number_two));
}

/**
 * @brief преобразыет значение матриц (где стенки) в поток
 * @param matrix указатель на матрицу значений стенок
 */
void PrintMatrix(const std::vector<std::vector<int>> &matrix) {
  for (const auto &rows : matrix) {
    for (const auto &element : rows) {
      std::cout << (element ? '1' : '0') << ' ';
    }
    std::cout << std::endl;
  }
}

/**
 * @brief заносит данные строки (описывающей где нижние справа)
 * в соответствующие поля класа MazeData
 * @param data_string строка из файла описывающая где стенки справа
 */
void MazeData::FillMatrixRight(std::string data_string) {
  std::vector<int> current_vector;
  for (size_t it = 0; it < data_string.length(); ++it) {
    if (data_string[it] == '1') {
      current_vector.push_back(true);
    } else if (data_string[it] == '0') {
      current_vector.push_back(false);
    } else {
      continue;
    }
  }
  current_vector.shrink_to_fit();
  matrixRight.push_back(current_vector);
  current_vector.clear();
}

/**
 * @brief заносит данные строки (описывающей где нижние стенки)
 * в соответствующие поля класа MazeData
 * @param data_string строка из файла описывающая где стенки снизу
 */
void MazeData::FillMatrixBottom(std::string data_string) {
  std::vector<int> current_vector;
  for (size_t it = 0; it < data_string.length(); ++it) {
    if (data_string[it] == '1') {
      current_vector.push_back(true);
    } else if (data_string[it] == '0') {
      current_vector.push_back(false);
    } else {
      continue;
    }
  }
  current_vector.shrink_to_fit();
  matrixBottom.push_back(current_vector);
  current_vector.clear();
}

/**
 * @brief Переносит данные (где стенки лабиринта) из .txt
 * в соответствующие поля класса MazeData
 */
void MazeData::GetValues() {
  QFile file(path_to_file_);

  CheckingDataBeforeFill();

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    std::string checking_line;
    checking_line = in.readLine().toStdString();
    GetMatrixParametrs(checking_line);

    for (size_t it = 0; it < get_length(); ++it) {
      checking_line = in.readLine().toStdString();
      FillMatrixRight(checking_line);
    }

    checking_line = in.readLine().toStdString();

    for (size_t it = 0; it < get_length(); ++it) {
      checking_line = in.readLine().toStdString();
      FillMatrixBottom(checking_line);
    }
    file.close();
  }
}

/**
 * @brief удаление нарисованного маршрута с виджета
 */
void MazeData::CleanPath() { this->path_.clear(); }

/**
 * @brief сохранение текущего лабиринта в .txt формат
 * при нажатии кнопки 'Сохранить'
 */
void MainWindow::on_SaveMaze_clicked() {
  if (mazeData.matrixBottom.size() != 0 && mazeData.matrixRight.size() != 0) {
    QString saveFilePath = QFileDialog::getSaveFileName(
        nullptr, "Save as", "", "Text file (*.txt);;All file (*.*)");
    if (!saveFilePath.isEmpty()) {
      // Maze maze(mazeData.get_length(), mazeData.get_width());
      Controller controller(mazeData.get_length(), mazeData.get_width());
      // maze.setRightWalls(mazeData.matrixRight);
      // maze.setBottomWalls(mazeData.matrixBottom);
      // maze.saveMaze(saveFilePath.toStdString());
      controller.setRightWalls(mazeData.matrixRight);
      controller.setBottomWalls(mazeData.matrixBottom);
      controller.saveMaze(saveFilePath.toStdString());
    }
  }
}

/**
 * @brief создание и прорисовка лабиринта
 * при нажатии кнопки 'Создать'
 */
void MainWindow::on_Generate_clicked() {
  countClickMouse_ = 0;
  mazeData.CleanPath();
  mazeData.set_length(ui->Size_x->value());
  mazeData.set_width(ui->Size_y->value());
  // s21::Maze maze(mazeData.get_length(), mazeData.get_width());
  s21::Controller controller(mazeData.get_length(), mazeData.get_width());
  // maze.generateMaze();
  controller.generateMaze();

  // mazeData.matrixRight = maze.getRightWalls();
  // mazeData.matrixBottom = maze.getBottomWalls();
  mazeData.matrixRight = controller.getRightWalls();
  mazeData.matrixBottom = controller.getBottomWalls();

  mazeData.set_length(mazeData.matrixRight[0].size());
  mazeData.set_width(mazeData.matrixRight.size());
  mazeData.set_start_point(0, 0);
  mazeData.set_finish_point(0, 0);
  flag = 1;
  if (mazeData.get_length() > 0 && mazeData.get_width() > 0) {
    countClickMouse_ = 0;
    mazeLoaded_ = true;
    QWidget::update();
  }
}

void MainWindow::on_Size_x_valueChanged(int arg1) { mazeData.set_width(arg1); }

void MainWindow::on_Size_y_valueChanged(int arg1) { mazeData.set_length(arg1); }

// === геттеры и сеттеры класа MazeData

// void MazeData::set_path(
//     std::vector<s21::PathFinder::Point> current_path) {
//   mazeData.path_ = current_path;
// }
void MazeData::set_path(std::vector<s21::Controller::Point> current_path) {
  mazeData.path_ = current_path;
}

void MazeData::set_width(int value) { this->width_ = value; }

int MazeData::get_path_size() { return path_.size(); }

void MazeData::set_length(int value) { this->length_ = value; }

size_t MazeData::get_width() { return this->width_; }

size_t MazeData::get_length() { return this->length_; }

// std::vector<s21::PathFinder::Point> MazeData::get_path() {
//   return path_;
// }
std::vector<s21::Controller::Point> MazeData::get_path() { return path_; }

void MazeData::set_start_point(int x, int y) {
  if (x > 0) {
    start_.x = x;
  }
  if (y > 0) {
    start_.y = y;
  }
}

void MazeData::set_finish_point(int x, int y) {
  if (x > 0) {
    finish_.x = x;
  }
  if (y > 0) {
    finish_.y = y;
  }
}

MazeData::Point MazeData::get_start_point() { return start_; }

MazeData::Point MazeData::get_finish_point() { return finish_; }
};  // namespace s21
