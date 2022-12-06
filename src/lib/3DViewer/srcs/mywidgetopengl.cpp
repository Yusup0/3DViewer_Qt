#include "../includes/mywidgetopengl.hpp"
#include "functional.h"
#ifdef __LINUX__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OPenGL/gl.h>
#endif
#include <QColor>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>

//  координаты вершин (v)
//  координаты текстуры (vt)
//  координаты нормали (vn)
//  Параметры одной поверхности объекта (f)
//  Комментарии (#)

// -------------------------------------------------------

MyWidgetOPenGL::MyWidgetOPenGL(QWidget *parent)
    : QOpenGLWidget(parent), m_isValid(0), m_rotateX(0), m_rotateY(0),
      m_rotateZ(0), m_rotateBeforeX(0), m_rotateBeforeY(0), m_rotateBeforeZ(0),
      m_moveBeforeX(0), m_moveBeforeY(0), m_moveBeforeZ(0),
      m_labelName(new QLabel(this)), m_labelVertes(new QLabel(this)),
      m_labelPolygons(new QLabel(this)), m_layoutH(new QHBoxLayout(this)),
      m_isMouse(false), m_tmpColor({152, 84, 93}) {
  logging(ERROR_OK, "[BEGIN] mywidgetopengl", 1);

  defaultConfigSimple();
  m_points.points = NULL;
  m_polygons.poligons = NULL;

  this->installEventFilter(this);
  resize(800, 600);

  m_initialized = false;
  drawInfo();
  loadConfig();

  logging(ERROR_OK, "[OK] mywidgetopengl", 1);
}

// --------------------------------------------------

MyWidgetOPenGL::~MyWidgetOPenGL() {
  free_matrix_int(&m_polygons);
  if (m_points.points)
    free(m_points.points);
  logging(ERROR_END, "", 1);
  writeToFileConfig("./config.json");
}

// --------------------------------------------------

void MyWidgetOPenGL::initializeGL() {
  logging(ERROR_OK, "[BEGIN] initializeGL", 1);

  initializeOpenGLFunctions();
  glEnable(GL_MULTISAMPLE); // smoothing the isa
  glClearColor(0.0, 0.1, 0.0, 1.0);

  glEnable(GL_DEPTH_TEST); // dissabling the buffer deep
  glShadeModel(GL_FLAT);

  m_initialized = 1;

  logging(ERROR_OK, "[OK] initializeGL", 1);
}

// --------------------------------------------------

void MyWidgetOPenGL::resizeGL(int w_, int h_) {
  logging(ERROR_OK, "[BEGIN] resizeGL", 1);
  glViewport(0, 0, w_, h_);

  logging(ERROR_OK, "[OK] resizeGL", 1);
}

// --------------------------------------------------

void MyWidgetOPenGL::paintGL() {
  if (!m_initialized || !m_isValid) {
    glClearColor(m_tmpColor.redF(), m_tmpColor.blueF(), m_tmpColor.greenF(),
                 0.1);
  } else {

    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(),
                 m_backgroundColor.blueF(), 0.0f);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (m_initialized && m_isValid) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_isValid) {
      glEnable(GL_BLEND);
      glEnable(GL_MULTISAMPLE);
      glLineWidth(m_lineWidth);

      glPointSize(m_pointSize);
      if (m_lineType) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(3, 255);
      }

      updatePerspective();
      drawObjects(e_typeDraw::TYPE_LINES);

      if (m_lineType) {
        glDisable(GL_LINE_STIPPLE);
      }

      if (m_pointType == 1) {
        glEnable(GL_POINT_SMOOTH);
        drawObjects(e_typeDraw::TYPE_POINTS);
        glDisable(GL_POINT_SMOOTH);
      } else if (m_pointType == 2) {
        drawSquare();
      }
    }
  }
}

// --------------------------------------------------

void MyWidgetOPenGL::wheelEvent(QWheelEvent *event) {
  double isDecrement = event->angleDelta().ry() < 0 ? 1 : 0;
  (isDecrement) ? decrementScale() : incrementScale();
  emit on_scaleStep();
}

// --------------------------------------------------

void MyWidgetOPenGL::mouseMoveEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  m_rotateX = m_Pos.x() - event->pos().x();
  m_rotateY = m_Pos.y() - event->pos().y();
  m_Pos = event->pos();
  changeRotate();
}

// --------------------------------------------------

void MyWidgetOPenGL::mousePressEvent(QMouseEvent *event) {
  m_Pos = event->pos();
}

// -------------------------------------------------------

void MyWidgetOPenGL::mouseReleaseEvent(QMouseEvent *event) { Q_UNUSED(event); }

bool MyWidgetOPenGL::eventFilter(QObject *watched, QEvent *event) {
  Q_UNUSED(watched);
  bool res = false;

  if (!m_isMouse) {
    if (event->type() == QEvent::Wheel) {
      res = true;
    } else if (event->type() == QEvent::MouseMove) {
      res = true;
    }
  }

  return (res);
}

// -------------------------------------------------------

void MyWidgetOPenGL::setPointSize(double newPointSize) {
  if (newPointSize >= m_minPointSize && newPointSize <= m_maxPointSize) {
    m_pointSize = newPointSize;
    update();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::setPerspective(int value_) {
  if (m_perspective != value_) {
    m_perspective = value_;
    update();
  } else {
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::turnOnMouse() { m_isMouse = true; }
// -------------------------------------------------------

void MyWidgetOPenGL::turnOffMouse() { m_isMouse = false; }

// -------------------------------------------------------

void MyWidgetOPenGL::setLineType(int newLineType) {
  m_lineType = newLineType;
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::setPointType(int newPointType) {
  m_pointType = newPointType;
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::setLineWidth(double newLineWidth) {
  m_lineWidth = newLineWidth;
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::setLineColor(int value_) {
  if (value_ == 0 || value_ == 255)
    m_lineColor.setHsl(255, 255, 255);
  else
    m_lineColor.setHsl(value_, 80, 80);
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::setPointColor(int value_) {
  if (value_ == 0 || value_ == 255)
    m_pointColor.setHsl(255, 255, 255);
  else
    m_pointColor.setHsl(value_, 80, 80);
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::setBackgroundColor(int value_) {
  if (value_ == 0 || value_ == 255) {
    m_backgroundColor.setHsl(0, 0, 0);
    m_labelName->setStyleSheet("QLabel { color : white; }");
    m_labelVertes->setStyleSheet("QLabel { color : white; }");
    m_labelPolygons->setStyleSheet("QLabel { color : white; }");
    emit on_changeColorGifTime(0);
  } else {
    m_backgroundColor.setHsl(value_, 50, 50);
    m_labelName->setStyleSheet("QLabel { color : black; }");
    m_labelVertes->setStyleSheet("QLabel { color : black; }");
    m_labelPolygons->setStyleSheet("QLabel { color : black; }");
    emit on_changeColorGifTime(1);
  }
  update();
}

// -------------------------------------------------------

int MyWidgetOPenGL::rotateBuffX() const { return m_rotateX; }

// -------------------------------------------------------

int MyWidgetOPenGL::rotateBuffY() const { return m_rotateY; }

// -------------------------------------------------------

int MyWidgetOPenGL::rotateBuffZ() const { return m_rotateZ; }

// -------------------------------------------------------

int MyWidgetOPenGL::maxScale() { return (m_maxScale); }

// -------------------------------------------------------

int MyWidgetOPenGL::minScale() { return (m_minScale); }

// -------------------------------------------------------

void MyWidgetOPenGL::setRotateBuffZ(int newRotateBuffZ) {
  m_rotateZ = newRotateBuffZ;
}

// -------------------------------------------------------

void MyWidgetOPenGL::setRotateBuffY(int newRotateBuffY) {
  m_rotateY = newRotateBuffY;
}

// -------------------------------------------------------

void MyWidgetOPenGL::setRotateBuffX(int newRotateBuffX) {
  m_rotateX = newRotateBuffX;
}

// -------------------------------------------------------

void MyWidgetOPenGL::rotateY(int const value_) {
  setRotateBuffY(value_);
  moveRotation(MOVE_ROTATE_Y, value_);
  // changeRotate();
}

// -------------------------------------------------------

void MyWidgetOPenGL::rotateX(int const value_) {
  setRotateBuffX(value_);
  moveRotation(MOVE_ROTATE_X, value_);
  // changeRotate();
}

// -------------------------------------------------------

void MyWidgetOPenGL::rotateZ(int const value_) {
  setRotateBuffZ(value_);
  moveRotation(MOVE_ROTATE_Z, value_);
  // changeRotate();
}

// -------------------------------------------------------

const QString &MyWidgetOPenGL::fileNameObject() const {
  return m_fileNameObject;
}

// -------------------------------------------------------

void MyWidgetOPenGL::setFileNameObject(const QString &newFileNameObject) {
  m_fileNameObject = newFileNameObject;
  updateData();
}

// -------------------------------------------------------

void MyWidgetOPenGL::connectionsConfiguration() {}

// -------------------------------------------------------

void MyWidgetOPenGL::qColorToRGB(const QColor &c_, float &r_, float &g_,
                                 float &b_) const {
  r_ = normalize_0_1(c_.redF(), 1.0f, 255.0f);
  g_ = normalize_0_1(c_.greenF(), 1.0f, 255.0f);
  b_ = normalize_0_1(c_.blueF(), 1.0f, 255.0f);
}

// -------------------------------------------------------

int MyWidgetOPenGL::normalize_0_1(float val, float min, float max) const {
  return (val - min) / (max - min);
}

// -------------------------------------------------------

int MyWidgetOPenGL::updateData() {
  QFileInfo checkFile(m_fileNameObject);
  int is_res = 0;
  m_isValid = 0;

  logging(ERROR_OK, "[BEGIN] loadObject", 1);

  free_matrix_int(&m_polygons);
  if (m_points.points) {
    free(m_points.points);
    m_points.points = NULL;
  }

  if (checkFile.exists() && checkFile.isFile() && !m_fileNameObject.isEmpty()) {
    is_res = pars_file(m_fileNameObject.toStdString().c_str(), &m_points,
                       &m_polygons);
    if (is_res && !m_fileNameObject.isEmpty()) {
      defaultConfigSimple();
      m_isValid = is_res;
      m_sizePerspective = pow(10, countNumber(m_points.max_size));

      updateInfoObject();
      logging(ERROR_OK, "[OK] loadObject", 1);
    } else {
      clearInfo();
      logging_line(
          ERROR_ANOTHER, "", __LINE__,
          "[ERROR] loadObject (functions) data (coordinates) return NULL ", 1);
    }
  } else {
    clearInfo();
    logging_line(ERROR_ANOTHER, m_fileNameObject.toStdString().c_str(),
                 __LINE__, "FILE NOT FOUND", 1);
  }

  return (is_res);
}

// -------------------------------------------------------

void MyWidgetOPenGL::updateInfoObject() {
  if (m_isValid) {
    QFileInfo info(m_fileNameObject);

    m_labelName->setText("Name: " + info.baseName());
    m_labelVertes->setText("    Vertes: " +
                           QString::number(m_points.count_points));
    m_labelPolygons->setText(
        "    Polygons: " +
        QString::number(m_polygons.poligons->columns * m_polygons.rows));
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::changeRotate() {
  // muose rotate
  if (m_isValid) {
    if (m_rotateX) {
      turn_obj(m_rotateX, &m_points, 2);
    }
    if (m_rotateY) {
      turn_obj(m_rotateY, &m_points, 1);
    }
    update();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::lineWidth(GLfloat nWidth_) {
  m_widthLine = nWidth_ / 10;
  update();
}

// -------------------------------------------------------

void MyWidgetOPenGL::defaultConfig() {
  m_lineType = 0;
  m_perspective = 0;
  m_lineWidth = 1;
  m_pointSize = 2;
  m_pointType = 0;
  m_lineColor = {1, 1, 1};
  m_pointColor = {1, 1, 1};
}

// -------------------------------------------------------

void MyWidgetOPenGL::defaultConfigSimple() {
  m_countScale = 10;
  m_maxScale = 60;
  m_minScale = -40;
  m_maxPointSize = 25.0;
  m_minPointSize = 0.0;
  m_perspective = 0;
  m_rotateBeforeX = 0;
  m_rotateBeforeY = 0;
  m_rotateBeforeZ = 0;
  m_moveBeforeX = 0;
  m_moveBeforeY = 0;
  m_moveBeforeZ = 0;
}

// -------------------------------------------------------

bool MyWidgetOPenGL::loadConfig(QString path_) {
  logging(ERROR_OK, "[BEGIN] loadConfig", 1);
  bool is_res = 1;
  QString val;
  QString filename =
      path_.isEmpty() ? QDir::currentPath() + "/config.json" : path_;

  QFile file(filename);

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  if (file.isOpen()) {
    val = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();
    QJsonValue value = sett2.value(QString("lineType"));
    if (!value.isUndefined() && is_res) {
      m_lineType = (value.toInt());
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("perspective"));
    if (!value.isUndefined() && is_res) {
      m_perspective = (value.toInt());
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("lineWidth"));
    if (!value.isUndefined() && is_res) {
      m_lineWidth = (value.toInt());
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("pointSize"));
    if (!value.isUndefined() && is_res) {
      m_pointSize = (value.toInt());
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("pointType"));
    if (!value.isUndefined() && is_res) {
      m_pointType = (value.toInt());
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("lineColor"));
    QJsonArray arr = value.toArray();

    if (!value.isUndefined() && value.isArray() && arr.size() == 3 && is_res) {
      m_lineColor = {arr[0].toInt(), arr[1].toInt(), arr[2].toInt()};
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("pointColor"));
    arr = value.toArray();

    if (!value.isUndefined() && value.isArray() && arr.size() == 3 && is_res) {
      m_pointColor = {arr[0].toInt(), arr[1].toInt(), arr[2].toInt()};
    } else {
      is_res = 0;
    }

    value = sett2.value(QString("backColor"));
    arr = value.toArray();

    if (!value.isUndefined() && value.isArray() && arr.size() == 3 && is_res) {
      m_backgroundColor = {arr[0].toInt(), arr[1].toInt(), arr[2].toInt()};
    } else {
      is_res = 0;
    }
    logging(ERROR_OK, "[OK] loadConfig", 1);
  } else {
    logging_line(ERROR_FILE_NOT_EXISTS, filename.toStdString().c_str(),
                 __LINE__, "[FILE NOT EXISTS] load to default settings", 1);
    is_res = 0;
    defaultConfig();
  }

  return (is_res);
}

// -------------------------------------------------------

bool MyWidgetOPenGL::writeToFileConfig(QString path_) {
  bool isRes = 1;

  QString val;
  QString filename =
      path_.isEmpty() ? QDir::currentPath() + "/config.json" : path_;

  QFile file(filename);

  file.open(QIODevice::WriteOnly | QIODevice::Text);

  if (file.isOpen()) {
    QJsonObject tmp;

    tmp.insert("lineType", m_lineType);
    tmp.insert("perspective", m_perspective);
    tmp.insert("lineWidth", m_lineWidth);
    tmp.insert("pointType", m_pointType);
    tmp.insert("pointSize", m_pointSize);

    QJsonArray jsonArr;
    jsonArr.push_back(m_lineColor.red());
    jsonArr.push_back(m_lineColor.green());
    jsonArr.push_back(m_lineColor.blue());

    tmp.insert("lineColor", jsonArr);

    QJsonArray jsonArrPointColor;
    jsonArrPointColor.push_back(m_pointColor.red());
    jsonArrPointColor.push_back(m_pointColor.green());
    jsonArrPointColor.push_back(m_pointColor.blue());

    tmp.insert("pointColor", jsonArrPointColor);

    QJsonArray jsonArrBackColor;
    jsonArrBackColor.push_back(m_backgroundColor.red());
    jsonArrBackColor.push_back(m_backgroundColor.green());
    jsonArrBackColor.push_back(m_backgroundColor.blue());

    tmp.insert("backColor", jsonArrBackColor);

    QTextStream out(&file);
    QJsonDocument jsonDocument;

    jsonDocument.setObject(tmp);

    QByteArray byteArray = jsonDocument.toJson(QJsonDocument::Indented);
    out << byteArray;
    file.close();
  } else {
    logging_line(ERROR_FILE_NOT_EXISTS, filename.toStdString().c_str(),
                 __LINE__, "", 1);
  }

  return (isRes);
}

// -------------------------------------------------------

void MyWidgetOPenGL::moveX(float value_) { moveDirection(MOVE_X, value_); }

// -------------------------------------------------------

void MyWidgetOPenGL::moveY(float value_) { moveDirection(MOVE_Y, value_); }

// -------------------------------------------------------

void MyWidgetOPenGL::moveZ(float value_) { moveDirection(MOVE_Z, value_); }

// -------------------------------------------------------

void MyWidgetOPenGL::drawObjects(e_typeDraw type_) {
  auto type = type_ == 0 ? GL_LINE_LOOP : GL_POINTS;
  double x, y, z;

  for (int i = 1; i < m_polygons.rows; i++) {
    glBegin(type);
    if (type_ == TYPE_LINES)
      glColor3f(m_lineColor.redF(), m_lineColor.greenF(), m_lineColor.blueF());
    else
      glColor3f(m_pointColor.redF(), m_pointColor.greenF(),
                m_pointColor.blueF());
    for (int j = 0; j < m_polygons.poligons[i].columns; j++) {
      x = m_points.points[m_polygons.poligons[i].points[j]].x;
      y = m_points.points[m_polygons.poligons[i].points[j]].y;
      z = m_points.points[m_polygons.poligons[i].points[j]].z;
      glVertex3f(x, y, z);
    }
    glEnd();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::drawInfo() {
  m_labelName->setText("Name: ");
  m_labelVertes->setText("Vertes: ");
  m_labelPolygons->setText("Polygons: ");
  m_layoutH->setAlignment(Qt::AlignTop);
  m_layoutH->addStretch();
  m_layoutH->addWidget(m_labelName);
  m_layoutH->addWidget(m_labelVertes);
  m_layoutH->addWidget(m_labelPolygons);
}

// -------------------------------------------------------

void MyWidgetOPenGL::incrementScale() {
  if (m_isValid) {
    if (m_countScale < m_maxScale) {
      ++m_countScale;
      scale_obj(1.05, &m_points);
      emit on_scaleStep();
    }
    update();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::decrementScale() {
  if (m_isValid) {
    if (m_countScale > m_minScale) {
      --m_countScale;
      scale_obj(0.95, &m_points);
      emit on_scaleStep();
    }
    update();
  }
}

// -------------------------------------------------------

QString MyWidgetOPenGL::scaleString() {
  return (QString::number(m_countScale));
}

// -------------------------------------------------------

void MyWidgetOPenGL::lineScaleChange(int value_) {
  bool isDecrement = 0;
  isDecrement = m_countScale > value_ ? 1 : 0;

  if (isDecrement && value_ >= m_minScale) {
    while (m_countScale != value_)
      decrementScale();
  } else if (!isDecrement && value_ <= m_maxScale) {
    while (m_countScale != value_)
      incrementScale();
  }
  if (value_ >= m_minScale && value_ <= m_maxScale) {
    m_countScale = value_;
    emit on_scaleStep();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::moveDirection(e_moveType direction_, float value_) {
  if (m_isValid) {
    value_ = value_ * m_points.max_size / 99;

    point_t t;
    float tmp = 0;
    int isError = 0;

    switch (direction_) {
    case MOVE_X:
      tmp = m_moveBeforeX - value_;
      t = {tmp, 0, 0};
      m_moveBeforeX = value_;
      break;
    case MOVE_Y:
      tmp = m_moveBeforeY - value_;
      t = {0, tmp, 0};
      m_moveBeforeY = value_;
      break;
    case MOVE_Z:
      tmp = m_moveBeforeZ - value_;
      t = {0, 0, tmp};
      m_moveBeforeZ = value_;
      break;
    default:
      isError = 1;
      logging_line(ERROR_ANOTHER, "", __LINE__,
                   "[ERROR] move direction [x, y, z]", 1);
      break;
    }

    if (!isError) {
      move_obj(&t, &m_points);
    }
    update();
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::moveRotation(e_moveRotatinoType direction_, float value_) {
  float tmp = 0;
  int isError = 0;

  if (m_isValid) {
    switch (direction_) {
    case MOVE_ROTATE_X:
      tmp = m_rotateBeforeX - value_;
      m_rotateBeforeX = value_;
      break;
    case MOVE_ROTATE_Y:
      tmp = m_rotateBeforeY - value_;
      m_rotateBeforeY = value_;
      break;
    case MOVE_ROTATE_Z:
      tmp = m_rotateBeforeZ - value_;
      m_rotateBeforeZ = value_;
      break;
    default:
      isError = 1;
      logging_line(ERROR_ANOTHER, "", __LINE__,
                   "[ERROR] move rotation direction [x, y, z]", 1);
      break;
    }

    if (!isError) {
      turn_obj(tmp, &m_points, direction_);
    }
    update();
  }
}

// -------------------------------------------------------

int MyWidgetOPenGL::countNumber(int number_) {
  int result = 0;

  if (number_ > 0) {
    while (number_ > 0) {
      number_ /= 10;
      result++;
    }
  }

  return result;
}

// -------------------------------------------------------

void MyWidgetOPenGL::updatePerspective() {
  if (m_perspective == 1) {
    glLoadIdentity();
    glFrustum(-m_sizePerspective, m_sizePerspective, -m_sizePerspective,
              m_sizePerspective, m_sizePerspective / 2.5,
              m_sizePerspective * 2);
    glTranslatef(0, 0, -m_sizePerspective / 2.5 * 3);
    m_perspective = 4;
  } else if (m_perspective == 0) {
    glLoadIdentity();
    glOrtho(-m_sizePerspective, m_sizePerspective, -m_sizePerspective,
            m_sizePerspective, -m_sizePerspective, m_sizePerspective);
    m_perspective = 5;
  }
}

// -------------------------------------------------------

void MyWidgetOPenGL::clearInfo() {
  m_labelName->setText("");
  m_labelVertes->setText("");
  m_labelPolygons->setText("");
}

// -------------------------------------------------------

void MyWidgetOPenGL::drawSquare() {
  glColor3f(m_pointColor.redF(), m_pointColor.greenF(), m_pointColor.blueF());
  double x, y, z, del = m_perspective == 4 ? 9 : 23;
  del = m_points.max_size / del * m_pointSize / 20;
  for (int i = 1; i < m_polygons.rows; i++) {
    for (int j = 0; j < m_polygons.poligons[i].columns; j++) {
      glBegin(GL_POLYGON);
      x = m_points.points[m_polygons.poligons[i].points[j]].x;
      y = m_points.points[m_polygons.poligons[i].points[j]].y;
      z = m_points.points[m_polygons.poligons[i].points[j]].z;
      glVertex3f(x - del, y - del, z);
      glVertex3f(x + del, y - del, z);
      glVertex3f(x + del, y + del, z);
      glVertex3f(x - del, y + del, z);
      glEnd();
    }
  }
}

// -------------------------------------------------------
