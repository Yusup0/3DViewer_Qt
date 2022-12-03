#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "gifcreator.h"

#include <QMainWindow>
#include <mywidgetopengl.hpp>
#include <stdbool.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:

private slots:
  void openFileDialog();
  void clickCluck();

  void rotateX(int value_);
  void rotateY(int value_);
  void rotateZ(int value_);

  void screenshotJPEG();
  void screenshotBMP();
  void doGif();
  void startGif();

private slots:
  void closeApp();

  void lineScaleChange(QString value_);

  void changeBackgroundColor(int value_);
  void changeColorLines(int value_);
  void changeColorPoints(int value_);

  void changeWidthLines(int value_);
  void changeSizePoint(int value_);

  void setPointType(int value_);

  void moveObject(int type_, int value_);

private:
  Ui::MainWindow *ui;

  MyWidgetOPenGL *m_myWidget;

  bool m_isPositiveHorizontal;
  bool m_isPositiveVertical;
  // QGifImage *m_gif;
  GifCreator *m_gif;
  QTimer *m_timerGif;
  QLabel *m_labelGifTime;

  double m_startTime;
  int m_endTime;
  int m_countFrame = 0;
  int m_frameNum = 0;

private:
  void connectsConfiguration();
  void defaultConfig();
  void changeRotateSliders();

  void connectsMoves();
  void connectsRotate();
  void connectsColor();
  void connectsPointType();
  void connectsScale();
  void connectsImages();
  void connectsLineWidth();
  void connectPerspective();
  void connectMouseRotate();

  void screenshot(int isJpeg = 1);
};
#endif // MAINWINDOW_HPP
