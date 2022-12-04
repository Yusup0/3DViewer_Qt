#ifndef MYWIDGETOPENGL_HPP
#define MYWIDGETOPENGL_HPP

#ifdef __MACOS__
#define GL_SILENCE_DEPRECATION
#endif

#include <QObject>
#include <QOpenGLFunctions>
#include <QtOpenGLWidgets>

#define RGB_MIN 1
#define RGB_MAX 255

extern "C" {
#include "functional.h"
#include "logging_manager.h"
}

typedef enum e_typeDraw {
  TYPE_LINES = 0,
  TYPE_POINTS,
  END_TYPE_DRAW
} e_typeDraw;

// -------------------------------------------------------

typedef enum e_moveType { MOVE_X = 0, MOVE_Y, MOVE_Z, MOVE_END } e_moveType;

// -------------------------------------------------------

typedef enum e_moveRotationType {
  MOVE_ROTATE_Y = 1,
  MOVE_ROTATE_X,
  MOVE_ROTATE_Z,
  MOVE_ROTATE_END
} e_moveRotatinoType;

// -------------------------------------------------

class MyWidgetOPenGL : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  MyWidgetOPenGL(QWidget *parent = nullptr);
  MyWidgetOPenGL(MyWidgetOPenGL const &other_) = delete;
  MyWidgetOPenGL(MyWidgetOPenGL &&other_) = delete;
  void operator=(MyWidgetOPenGL const &other_) = delete;
  void operator=(MyWidgetOPenGL &&other_) = delete;
  virtual ~MyWidgetOPenGL();

  /* **** ***** **** */

  void setFileNameObject(const QString &newFileNameObject);
  const QString &fileNameObject() const;

  void rotateY(int value_);
  void rotateX(int value_);
  void rotateZ(int value_);

  void lineWidth(GLfloat nWidth_);
  void setScale(float value_);

  void setRotateBuffX(int newRotateBuffX);
  void setRotateBuffY(int newRotateBuffY);
  void setRotateBuffZ(int newRotateBuffZ);

  int rotateBuffX() const;
  int rotateBuffY() const;
  int rotateBuffZ() const;

  int maxScale();
  int minScale();

  void setLineColor(int value_);
  void setPointColor(int value_);
  void setBackgroundColor(int value_);

  void setLineWidth(double newLineWidth);
  void setPointType(int newPointType);
  void setLineType(int newLineType);
  void setPointSize(double newPointSize);

  void setPerspective(int value_);
  void turnOnMouse();
  void turnOffMouse();

  void incrementScale();
  void decrementScale();

  QString scaleString();
  void lineScaleChange(int value_);

  void moveX(float value_);
  void moveY(float value_);
  void moveZ(float value_);
  void moveDirection(e_moveType direction_, float value_);
  void moveRotation(e_moveRotatinoType direction_, float value_);
  int countNumber(int number);

signals:
  void on_moveChange(float value_);
  void on_changeRotate();
  void on_scaleStep();
  void on_changeColorGifTime(int isBlack_);

protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

  // QWidget interface
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;

  // QObject interface
  virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
  bool m_isValid;

  int m_rotateX;
  int m_rotateY;
  int m_rotateZ;

  GLint m_rotateBeforeX;
  GLint m_rotateBeforeY;
  GLint m_rotateBeforeZ;
  GLfloat m_moveBeforeX;
  GLfloat m_moveBeforeY;
  GLfloat m_moveBeforeZ;

  QString m_fileNameObject;

  QColor m_lineColor;
  QColor m_pointColor;
  QColor m_backgroundColor;

  int m_lineType;
  int m_perspective;
  double m_lineWidth;
  double m_pointSize;
  double m_maxPointSize;
  double m_minPointSize;
  int m_pointType; // 0 - none; 1 - circle; 2 - square

  GLfloat m_widthLine;
  int m_countScale;
  int m_maxScale;
  int m_minScale;

  bool m_initialized;
  QPoint m_Pos;
  double m_sizePerspective;

  QLabel *m_labelName;
  QLabel *m_labelVertes;
  QLabel *m_labelPolygons;
  QHBoxLayout *m_layoutH;

  bool m_isMouse;

  points_a m_points;
  matrix_poligon m_polygons;

private:
  void connectionsConfiguration();
  void qColorToRGB(const QColor &c, float &r, float &g, float &b) const;
  int normalize_0_1(float val, float min, float max) const;

  int updateData();
  void updateInfoObject();

  void changeRotate();
  void updatePerspective();

  void defaultConfig();
  void defaultConfigSimple();
  bool loadConfig(QString path_ = "");
  bool writeToFileConfig(QString path_ = "");

  void drawObjects(e_typeDraw type_);
  void drawInfo();
  void clearInfo();
};

#endif // MYWIDGETOPENGL_HPP
