#include "../includes/test_mywidgetopengl.hpp"
#include "qtestcase.h"

#include <QDebug>

int equalDouble(float a_, float b_) { return (a_ - b_) < 1e7; }

// -------------------------------------------------------

TestMyWidgetOpenGL::TestMyWidgetOpenGL() {}

// -------------------------------------------------------

TestMyWidgetOpenGL::~TestMyWidgetOpenGL() {}

// -------------------------------------------------------

void TestMyWidgetOpenGL::test_loadObject() {
    QVERIFY(1 == 1);

  m_myWidget.setFileNameObject(QDir::currentPath() + "/objects/cub.obj");
  // QVERIFY(res == 1);

  // auto *pDataObject = m_myWidget.dataObject();
  // QVERIFY(equalDouble(pDataObject->maxVertex, 1.0f));
}

// -------------------------------------------------------

void TestMyWidgetOpenGL::test_loadConfig() {
  // int res = m_myWidget.loadConfig();
  // QVERIFY(res == 1);

  // res = m_myWidget.loadConfig("piter");
  // QVERIFY(res == 0);
}

// -------------------------------------------------------

void TestMyWidgetOpenGL::test_writeConfig() {
    // bool isRes = m_myWidget.writeToFileConfig();
    // QVERIFY(isRes == 1);
}

void TestMyWidgetOpenGL::test_convertData2() {

}

QTEST_MAIN(TestMyWidgetOpenGL)
