#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>

#define ROTATE_VALUE 720
#define GIF_FPS 10

// -------------------------------------------------------

// m_gif(new QGifImage),
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_myWidget(new MyWidgetOPenGL()), m_isPositiveHorizontal(1),
      m_isPositiveVertical(1), m_gif(new GifCreator(m_myWidget)),
      m_timerGif(new QTimer(this)), m_labelGifTime(new QLabel(m_myWidget)) {
  logging(ERROR_OK, "[BEGIN] mainWindow", 1);
  ui->setupUi(this);

  ui->radioButton_solid->setChecked(true);
  ui->radioButton_no->setChecked(true);

  m_myWidget->setParent(ui->widget);
  m_myWidget->show();
  connectsConfiguration();

  m_labelGifTime->setText("");
  m_labelGifTime->setAlignment(Qt::AlignTop);
  m_labelGifTime->setAlignment(Qt::AlignLeft);
  m_labelGifTime->move(40, 40);
  m_labelGifTime->raise();
  QFont font = m_labelGifTime->font();
  font.setPointSize(20);
  font.setBold(true);
  m_labelGifTime->setFont(font);

  logging(ERROR_OK, "[OK] mainWindow", 1);
}

// -------------------------------------------------------

MainWindow::~MainWindow() {
  delete m_myWidget;
  delete ui;
}

// -------------------------------------------------------

void MainWindow::openFileDialog() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open Object"), "./objects/", tr("Image Files (*.obj)"));
  m_myWidget->setFileNameObject(fileName);
  m_myWidget->update();
}

// -------------------------------------------------------

// TODO: remove
void MainWindow::clickCluck() {
}

// -------------------------------------------------------

void MainWindow::rotateX(int value_) {
  m_myWidget->rotateX(value_);
  changeRotateSliders();
}

// -------------------------------------------------------

void MainWindow::rotateY(int value_) {
  m_myWidget->rotateY(value_);
  changeRotateSliders();
}

// -------------------------------------------------------

void MainWindow::rotateZ(int value_) {
  m_myWidget->rotateZ(value_);
  changeRotateSliders();
}

// -------------------------------------------------------


void MainWindow::connectsConfiguration() {
  connect(ui->pb_openFIle, &QPushButton::clicked, this,
          &MainWindow::openFileDialog);

  connect(ui->pb_test1, &QPushButton::clicked, this, &MainWindow::clickCluck);

  connect(ui->widget, &QOpenGLWidget::resized, this, [&]() {
    m_myWidget->resize(ui->widget->width(), ui->widget->height());
  });

  connect(ui->radioButton_dotter, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setLineType(1); });

  connect(ui->radioButton_solid, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setLineType(0); });

  connect(ui->hSlidder_pointsSize, &QSlider::valueChanged, this,
          &MainWindow::changeSizePoint);

  connectsPointType();
  connectsRotate();
  connectsMoves();
  connectsColor();
  connectsScale();
  connectsImages();
  connectsLineWidth();
  connectPerspective();
  connectMouseRotate();
};

// -------------------------------------------------------


void MainWindow::changeRotateSliders() {
  int x = (m_myWidget->rotateBuffX() % ROTATE_VALUE);
  int y = (m_myWidget->rotateBuffY() % ROTATE_VALUE);
  int z = (m_myWidget->rotateBuffZ() % ROTATE_VALUE);
  ui->slider_x->setValue(x);
  ui->slider_y->setValue(y);
  ui->slider_z->setValue(z);
  ui->lineEdit_rotateX->setText(QString::number(x));
  ui->lineEdit_rotateY->setText(QString::number(y));
  ui->lineEdit_rotateZ->setText(QString::number(z));
  qDebug() << "on chngerotate";
}

void MainWindow::screenshot(int isJpeg) {

  long ttime = time(0);
  QString current_time = ctime(&ttime);
  QString format;
  QPixmap pix(m_myWidget->size() * 2);

  QString path = QDir::currentPath() + "/screenshots/" + current_time;
  path.chop(1);
  path.replace(" ", "_");

  if (isJpeg == 1) {
    path += ".jpeg";
    format = "JPG";
  } else {
    path += ".bmp";
    format = "BMP";
  }

  pix.setDevicePixelRatio(2);
  m_myWidget->render(&pix);
  int res = pix.save(path, "JPG", 100);
  qDebug() << "path: " << path;
  QMessageBox msgBox;

  if (res) {
    msgBox.setText("Screenshot OK (" + format + ").");
    logging(ERROR_OK, "[OK] SCREENSHOTS", 1);
    msgBox.exec();
  } else {
    msgBox.setText("Screenshot ERROR (" + format + ").");
    logging(ERROR_ANOTHER, "[ERROR] SCREENSHOTS", 1);
    msgBox.exec();
  }
}

// -------------------------------------------------------

void MainWindow::closeApp() { close(); }

// -------------------------------------------------------

void MainWindow::lineScaleChange(QString value_) {
  Q_UNUSED(value_);

  m_myWidget->lineScaleChange(value_.toInt());
}

// -------------------------------------------------------

void MainWindow::changeBackgroundColor(int value_) {
  m_myWidget->setBackgroundColor(value_);
}

// -------------------------------------------------------

void MainWindow::changeColorLines(int value_) {
  m_myWidget->setLineColor(value_);
}

// -------------------------------------------------------

void MainWindow::changeColorPoints(int value_) {
  m_myWidget->setPointColor(value_);
}

// -------------------------------------------------------

void MainWindow::changeWidthLines(int value_) {
  ui->lineEdit_widthLine->setText(QString::number(value_));
  double value = value_ / 10.0;
  m_myWidget->setLineWidth(value);
}

void MainWindow::changeSizePoint(int value_) {
  m_myWidget->setPointSize(value_);
  ui->lineEdit_pointsSize->setText(QString::number(value_));
}

// -------------------------------------------------------

void MainWindow::setPointType(int value_) { m_myWidget->setPointType(value_); }

// -------------------------------------------------------

void MainWindow::moveObject(int type_, int value_) {
  if (type_ == 1) {
    m_myWidget->moveX((value_));
    ui->lineEdit_moveX->setText(QString::number(value_));
  } else if (type_ == 2) {
    m_myWidget->moveY((value_));
    ui->lineEdit_moveY->setText(QString::number(value_));
  } else if (type_ == 3) {
    m_myWidget->moveZ((value_));
    ui->lineEdit_moveZ->setText(QString::number(value_));
  }
}

// -------------------------------------------------------

void MainWindow::screenshotJPEG() { screenshot(1); }

// -------------------------------------------------------

void MainWindow::screenshotBMP() { screenshot(0); }

// -------------------------------------------------------

void MainWindow::doGif() {
  m_startTime = 0;
  m_endTime = 1000 / GIF_FPS;
  m_timerGif->start(1000 / GIF_FPS);
  m_frameNum = 0;
}

// -------------------------------------------------------

void MainWindow::startGif() {
  if (m_startTime == m_endTime) {
    float time = 0;
    QPixmap gif(m_myWidget->size());
    m_myWidget->render(&gif);
    gif.scaled(640, 480, Qt::IgnoreAspectRatio);
    if (!gif.save(m_gif->imageFilePathMask().arg(m_frameNum))) {
        logging_line(ERROR_ANOTHER, "", __LINE__, "[ERROR] gif not save image.", 1);
    } 

    time = m_startTime / 1000;
    m_labelGifTime->setText(QString::number(time));
    m_endTime += 1000 / GIF_FPS;
    ++m_frameNum;
  }

  int gifLength = 5;
  if (m_startTime >= 1000 * gifLength) {
    if (m_gif->createGif(m_frameNum, GIF_FPS)) {
      qDebug() << "GIF OK";
    } else {
      qDebug() << "GIF FALSE";
    }

    m_timerGif->stop();
    qDebug() << "gif path: " << m_gif->gifFilePath();
    qDebug() << "END GIF";
    m_labelGifTime->setText("");
  }
  m_startTime += 1000 / GIF_FPS;
}

// -------------------------------------------------------

void MainWindow::connectsMoves() {
  connect(ui->hSlider_moveX, &QSlider::valueChanged, this, [&](int value) {
    ui->lineEdit_moveX->setText(QString::number(value));
    moveObject(1, value);
  });

  connect(ui->hSlider_moveY, &QSlider::valueChanged, this, [&](int value) {
    ui->lineEdit_moveY->setText(QString::number(value));
    moveObject(2, value);
  });

  connect(ui->hSlider_moveZ, &QSlider::valueChanged, this, [&](int value) {
    ui->lineEdit_moveZ->setText(QString::number(value));
    moveObject(3, value);
  });

  /* *****  ***** */

  connect(ui->lineEdit_moveX, &QLineEdit::textEdited, this,
          [&](QString value) { moveObject(1, value.toInt()); });

  connect(ui->lineEdit_moveY, &QLineEdit::textEdited, this,
          [&](QString value) { moveObject(2, value.toInt()); });

  connect(ui->lineEdit_moveZ, &QLineEdit::textEdited, this,
          [&](QString value) { moveObject(3, value.toInt()); });
}

// -------------------------------------------------------

void MainWindow::connectsRotate() {
  connect(ui->slider_x, &QSlider::valueChanged, this, &MainWindow::rotateX);
  connect(ui->slider_y, &QSlider::valueChanged, this, &MainWindow::rotateY);
  connect(ui->slider_z, &QSlider::valueChanged, this, &MainWindow::rotateZ);

  connect(ui->lineEdit_rotateX, &QLineEdit::textEdited, this,
          [&](QString value) { rotateX(value.toInt()); });

  connect(ui->lineEdit_rotateY, &QLineEdit::textEdited, this,
          [&](QString value) { rotateY(value.toInt()); });

  connect(ui->lineEdit_rotateZ, &QLineEdit::textEdited, this,
          [&](QString value) { rotateZ(value.toInt()); });
  connect(m_myWidget, &MyWidgetOPenGL::on_changeRotate, this,
          &MainWindow::changeRotateSliders);
}

// -------------------------------------------------------

void MainWindow::connectsColor() {
  connect(ui->hSlider_colorLines, &QSlider::valueChanged, this,
          &MainWindow::changeColorLines);
  connect(ui->hSlider_colorPoints, &QSlider::valueChanged, this,
          &MainWindow::changeColorPoints);
  connect(ui->hSlider_backgroundColor, &QSlider::valueChanged, this,
          &MainWindow::changeBackgroundColor);
  connect(ui->hSlider_colorPoints, &QSlider::valueChanged, this,
          &MainWindow::changeColorPoints);
}

// -------------------------------------------------------

void MainWindow::connectsPointType() {
  connect(ui->radioButton_no, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setPointType(0); });

  connect(ui->radioButton_circle, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setPointType(1); });

  connect(ui->radioButton_squard, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setPointType(2); });

  connect(ui->lineEdit_pointsSize, &QLineEdit::textEdited, this,
          [&](QString value_) { changeSizePoint(value_.toInt()); });
}

// -------------------------------------------------------

void MainWindow::connectsScale() {
  connect(m_myWidget, &MyWidgetOPenGL::on_scaleStep, this,
          [&]() { ui->lineEdit_scale->setText(m_myWidget->scaleString()); });

  connect(ui->pb_changeScale, &QPushButton::clicked, this,
          [&]() { lineScaleChange(ui->lineEdit_scale->text()); });

  connect(ui->pb_scalePlus, &QPushButton::clicked, m_myWidget,
          &MyWidgetOPenGL::incrementScale);
  connect(ui->pb_scaleMinus, &QPushButton::clicked, m_myWidget,
          &MyWidgetOPenGL::decrementScale);
}

// -------------------------------------------------------

void MainWindow::connectsImages() {
  connect(ui->pb_jpeg, &QPushButton::clicked, this,
          &MainWindow::screenshotJPEG);
  connect(ui->pb_bmp, &QPushButton::clicked, this, &MainWindow::screenshotBMP);
  connect(ui->pb_gif, &QPushButton::clicked, this, &MainWindow::doGif);
  connect(m_timerGif, &QTimer::timeout, this, &MainWindow::startGif);
}

// -------------------------------------------------------

void MainWindow::connectPerspective() {
  connect(ui->radioButton_central, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setPerspective(0); });

  connect(ui->radioButton_parallel, &QRadioButton::pressed, this,
          [&]() { m_myWidget->setPerspective(1); });
}

// -------------------------------------------------------

void MainWindow::connectMouseRotate() {
  connect(ui->checkBox_mouseOn, &QCheckBox::stateChanged, this,
          [&](int state_) {
            if (!state_) {
              ui->slider_x->setEnabled(true);
              ui->slider_y->setEnabled(true);
              ui->slider_z->setEnabled(true);
              ui->lineEdit_rotateX->setEnabled(true);
              ui->lineEdit_rotateY->setEnabled(true);
              ui->lineEdit_rotateZ->setEnabled(true);
              m_myWidget->turnOffMouse();
            } else {
              ui->slider_x->setEnabled(false);
              ui->slider_y->setEnabled(false);
              ui->slider_z->setEnabled(false);
              ui->lineEdit_rotateX->setEnabled(false);
              ui->lineEdit_rotateY->setEnabled(false);
              ui->lineEdit_rotateZ->setEnabled(false);
              m_myWidget->turnOnMouse();
            }
          });
}

// -------------------------------------------------------

void MainWindow::connectsLineWidth() {
  connect(ui->hSlidder_widthLine, &QSlider::valueChanged, this,
          &MainWindow::changeWidthLines);
  connect(ui->lineEdit_widthLine, &QLineEdit::textEdited, this,
          [&](QString value) { changeWidthLines(value.toDouble()); });
}
