#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QStandardPaths>
#include <QTextStream>
#include <iostream>
#include <queue>
#include <vector>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class DrawingWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void GenerateMaze();
  void OpenMaze();
  void SaveMaze();
  void findRoute();

  void on_Size_x_valueChanged(int arg1);
  void on_Size_y_valueChanged(int arg1);

 private:
  void mousePressEvent(QMouseEvent *event);
  void SetupButtons();
  int countClickMouse_ = 0;  // подсчет количества кликов мыши
  bool mazeLoaded_ = false;
  Ui::MainWindow *ui;
  DrawingWidget *drawingWidget;
};

class DrawingWidget : public QWidget {
 public:
  DrawingWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setFixedSize(500, 500);
  }

 protected:
  void paintEvent(QPaintEvent *event) override;
  QSize sizeHint() const override { return QSize(500, 500); }
};

class MazeData {
 public:
  struct Point {
    int x = 0;
    int y = 0;
    Point() = default;
    Point(int _x, int _y) : x(_x), y(_y) {}
  };
  // Сеттеры
  void set_width(int value);
  void set_length(int value);
  void set_path(std::vector<s21::Controller::Point> current_path);
  void set_start_point(int x, int y);
  void set_finish_point(int x, int y);

  // геттеры
  size_t get_width();
  size_t get_length();
  MazeData::Point get_start_point();
  MazeData::Point get_finish_point();
  int get_path_size();
  std::vector<s21::Controller::Point> get_path();

  // методы
  int GetValues();
  void CheckingDataBeforeFill();
  int GetMatrixParametrs(std::string parametrs_string);
  void FillMatrixRight(std::string data_string);
  void FillMatrixBottom(std::string data_string);
  void CleanPath();

  // матрицы границ
  std::vector<std::vector<int>> matrixRight;
  std::vector<std::vector<int>> matrixBottom;

  QString path_to_file_ = "\0";

 private:
  size_t width_ = 10;
  size_t length_ = 10;
  Point start_;
  Point finish_;
  std::vector<s21::Controller::Point> path_;
};
};      // namespace s21
#endif  // MAINWINDOW_H
