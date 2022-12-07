#include "mainwindow.hpp"

#include <QApplication>
#include <signal.h>

extern "C" {
#include "logging_manager.h"
}

void handler_sigsegv(int signum_) {
  signal(signum_, SIG_DFL);
  logging(ERROR_SEGMENTATION, NULL, 1);
  exit(EXIT_FAILURE);
}

void handler_sigfpe(int signum_) {
  signal(signum_, SIG_DFL);
  logging(ERROR_SEGMENTATION, NULL, 1);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "C");
  QSurfaceFormat fmt;
  fmt.setSamples(16);
  QSurfaceFormat::setDefaultFormat(fmt);

  signal(SIGSEGV, handler_sigsegv);
  signal(SIGFPE, handler_sigfpe);

  logging(ERROR_OK, "\n\n<--- MAIN --->", 1);

  MainWindow w;
  w.show();
  return a.exec();
}
