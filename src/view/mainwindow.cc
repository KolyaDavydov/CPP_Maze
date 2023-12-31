#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SetupButtons();
  drawingWidget = new DrawingWidget(this);
}

MainWindow::~MainWindow() { delete ui; }

MazeData mazeData;
int flag = 0;

/**
 * @brief назначение кнопок интерфейса
 */
void MainWindow::SetupButtons() {
  connect(ui->Generate, SIGNAL(clicked()), this, SLOT(GenerateMaze()));
  connect(ui->OpenMaze, SIGNAL(clicked()), this, SLOT(OpenMaze()));
  connect(ui->SaveMaze, SIGNAL(clicked()), this, SLOT(SaveMaze()));
}

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
        countClickMouse_++;
        if (countClickMouse_ == 1) {
          mazeData.set_start_point(res_x, res_y);
        }
        if (countClickMouse_ == 2) {
          mazeData.set_finish_point(res_x, res_y);
          findRoute();
          countClickMouse_ = 0;
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
}

/**
 * @brief Записивает путь в лабиринте
 * вызывает метод для заполнения пути
 * и прорисовывает путь
 */
void MainWindow::findRoute() {
  MazeData::Point point = mazeData.get_start_point();
  s21::Controller::Point start;
  start.x_ = point.x - 1;
  start.y_ = point.y - 1;
  point = mazeData.get_finish_point();

  s21::Controller::Point end;
  end.x_ = point.x - 1;
  end.y_ = point.y - 1;

  s21::Controller controller(mazeData.matrixRight, mazeData.matrixBottom);

  std::vector<s21::Controller::Point> path = controller.findPath(
      mazeData.matrixRight, mazeData.matrixBottom, start, end);
  mazeData.set_path(path);
  QWidget::update();
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  if (mazeData.path_to_file_ != "\0" || flag == 1) {
    painter.fillRect(rect(), Qt::black);
    painter.setPen(QPen(Qt::white, 2));

    float cellSizeVertic = 500.0 / mazeData.get_length();
    float cellSizeHorizont = 500.0 / mazeData.get_width();

    painter.drawRect(0, 0, 500, 500);
    for (int row = 0; row < mazeData.get_length(); ++row) {
      for (int col = 0; col < mazeData.get_width(); ++col) {
        if (mazeData.matrixRight[row][col] == 1) {
          painter.drawLine((col + 1) * cellSizeHorizont, row * cellSizeVertic,
                           (col + 1) * cellSizeHorizont,
                           (row + 1) * cellSizeVertic);
        }
        if (mazeData.matrixBottom[row][col] == 1) {
          painter.drawLine(col * cellSizeHorizont, (row + 1) * cellSizeVertic,
                           (col + 1) * cellSizeHorizont,
                           (row + 1) * cellSizeVertic);
        }
      }
    }
    painter.setPen(QPen(Qt::green, 2));

    if (mazeData.get_path_size() > 1) {
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
void MainWindow::OpenMaze() {
  countClickMouse_ = 0;
  mazeData.CleanPath();
  QString desktopPath =
      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  mazeData.path_to_file_ = QFileDialog::getOpenFileName(nullptr, "Open file",
                                                        desktopPath, "(*.txt)");
  QFile file(mazeData.path_to_file_);
  if (file.exists()) {
    int error = mazeData.GetValues();
    if (error == 0) {
      ui->Size_x->setValue(mazeData.get_width());
      ui->Size_y->setValue(mazeData.get_length());
      mazeData.set_start_point(0, 0);
      mazeData.set_finish_point(0, 0);
      mazeLoaded_ = true;
      QWidget::update();
    } else {
      mazeLoaded_ = false;
      QMessageBox::warning(this, "Предупреждение",
                           "Некорректный файл! Пересоздайте лабиринт или "
                           "откройти корректный файл");
    }
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
int MazeData::GetMatrixParametrs(std::string parametrs_string) {
  int error = 0;
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
  if (get_length() <= 1 || get_length() > 50 || get_width() <= 1 ||
      get_width() > 50) {
    error = 1;
  }
  return error;
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
int MazeData::GetValues() {
  int error = 0;
  QFile file(path_to_file_);

  CheckingDataBeforeFill();

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    std::string checking_line;
    checking_line = in.readLine().toStdString();
    error = GetMatrixParametrs(checking_line);

    for (size_t it = 0; it < get_length(); ++it) {
      checking_line = in.readLine().toStdString();
      if (checking_line.size() != get_width() * 2) {
        error = 1;
        file.close();
        return error;
      }
      FillMatrixRight(checking_line);
    }

    checking_line = in.readLine().toStdString();

    for (size_t it = 0; it < get_length(); ++it) {
      checking_line = in.readLine().toStdString();
      if (checking_line.size() != get_width() * 2) {
        error = 1;
        file.close();
        return error;
      }
      FillMatrixBottom(checking_line);
    }
    file.close();
  }
  return error;
}

/**
 * @brief удаление нарисованного маршрута с виджета
 */
void MazeData::CleanPath() { this->path_.clear(); }

/**
 * @brief сохранение текущего лабиринта в .txt формат
 * при нажатии кнопки 'Сохранить'
 */
void MainWindow::SaveMaze() {
  if (mazeData.matrixBottom.size() != 0 && mazeData.matrixRight.size() != 0) {
    QString saveFilePath = QFileDialog::getSaveFileName(
        nullptr, "Save as", "", "Text file (*.txt);;All file (*.*)");
    if (!saveFilePath.isEmpty()) {
      Controller controller(mazeData.get_length(), mazeData.get_width());
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
void MainWindow::GenerateMaze() {
  countClickMouse_ = 0;
  mazeData.CleanPath();
  mazeData.set_length(ui->Size_y->value());
  mazeData.set_width(ui->Size_x->value());
  s21::Controller controller(mazeData.get_length(), mazeData.get_width());
  controller.generateMaze();

  mazeData.matrixRight = controller.getRightWalls();
  mazeData.matrixBottom = controller.getBottomWalls();

  mazeData.set_length(mazeData.matrixRight.size());
  mazeData.set_width(mazeData.matrixRight[0].size());
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

void MazeData::set_path(std::vector<s21::Controller::Point> current_path) {
  mazeData.path_ = current_path;
}

void MazeData::set_width(int value) { this->width_ = value; }

int MazeData::get_path_size() { return path_.size(); }

void MazeData::set_length(int value) { this->length_ = value; }

size_t MazeData::get_width() { return this->width_; }

size_t MazeData::get_length() { return this->length_; }

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
