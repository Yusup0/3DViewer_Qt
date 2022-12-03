#ifndef TEST_MYWIDGETOPENGL_HPP
#define TEST_MYWIDGETOPENGL_HPP

#include "mywidgetopengl.hpp"

#include <QtTest>

class TestMyWidgetOpenGL : public QObject {
  Q_OBJECT
public:
  TestMyWidgetOpenGL();
  virtual ~TestMyWidgetOpenGL();

private slots:
  void test_loadObject();
  void test_loadConfig();
  void test_writeConfig();
  void test_convertData2();

private:
  MyWidgetOPenGL m_myWidget;
};

#endif // TEST_MYWIDGETOPENGL_HPP
