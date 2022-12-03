#include "gifcreator.h"
#include "qnamespace.h"
#include <QDir>
#include <QImage>
#include <QProgressDialog>
#include <QStandardPaths>
#include <ctime>
#include <cwchar>
#include <gif.h>

GifCreator::GifCreator(QWidget *parent) : QWidget(parent) {
  QString tmp_dir = QDir::currentPath() + "/gifs/";

  if (tmp_dir != "") {
    QDir().mkpath(tmp_dir);
  }

  m_gifFilePath = tmp_dir;
}

// -------------------------------------------------------

QString GifCreator::imageFilePathMask() const {
  QString tmp_dir =
      QStandardPaths::writableLocation(QStandardPaths::TempLocation);

  if (tmp_dir != "") {
    QDir().mkpath(tmp_dir);
  }

  return QDir(tmp_dir).filePath("image_%1.jpg");
}

// -------------------------------------------------------

QString GifCreator::gifFullPath() const {
  time_t now = time(0);
  tm *time = localtime(&now);
  QString name = "GIF_" + QString::number(time->tm_hour) + "_" +
                 QString::number(time->tm_min) + "_" +
                 QString::number(time->tm_sec) + ".gif";

  return (m_gifFilePath + name);
}

// -------------------------------------------------------

QString GifCreator::gifFilePath() const { return (m_gifFilePath); }

// -------------------------------------------------------

bool GifCreator::createGif(int frames_count, int frame_delay) {
  QImage first_image(imageFilePathMask().arg(0));

  first_image = first_image.scaled(640, 480, Qt::IgnoreAspectRatio);
  first_image.setDotsPerMeterX(640);
  first_image.setDotsPerMeterY(480);

  // qDebug() << "imageFilePathMask: " << imageFilePathMask().arg(0);
  // qDebug() << "image width: " << first_image.width();
  // qDebug() << "image height: " << first_image.height();
  // qDebug() << "frame delay: " << frame_delay;
  // qDebug() << "frame count: " << frames_count;
  // qDebug() << "gifFilePath" << gifFilePath().toUtf8();
  bool isValid = true;
  GifWriter gif_writer;

  if (!first_image.isNull()) {

    if (GifBegin(&gif_writer, gifFullPath().toUtf8(),
                 static_cast<uint32_t>(first_image.width()),
                 static_cast<uint32_t>(first_image.height()),
                 static_cast<uint32_t>(frame_delay))) {

      QProgressDialog progress("Render gif...", "Cancel", 0, frames_count,
                               this);
      progress.setWindowModality(Qt::WindowModal);

      for (int frame = 0; frame < frames_count && isValid; frame++) {
        QImage image(imageFilePathMask().arg(frame));
        image = image.scaled(640, 480, Qt::IgnoreAspectRatio);
        image.setDotsPerMeterX(640);
        image.setDotsPerMeterY(480);

        progress.setValue(frame);

        if (progress.wasCanceled())
          break;

        if (!image.isNull()) {
          if (!GifWriteFrame(&gif_writer,
                             image.convertToFormat(QImage::Format_Indexed8)
                                 .convertToFormat(QImage::Format_RGBA8888)
                                 .constBits(),
                             static_cast<uint32_t>(image.width()),
                             static_cast<uint32_t>(image.height()),
                             static_cast<uint32_t>(frame_delay))) {
            GifEnd(&gif_writer);

            isValid = false;
          }
        } else {
          GifEnd(&gif_writer);

          isValid = false;
        }
      }
      progress.setValue(frames_count);
      isValid = true;
    } else {
      isValid = false;
    }
  } else {
    isValid = false;
  }

  qDebug() << "END GIFFFF...";
  return ((isValid == 1) ? GifEnd(&gif_writer) : false);
}
