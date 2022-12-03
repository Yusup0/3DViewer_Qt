#ifndef GIFCREATOR_H
#define GIFCREATOR_H

#include <QWidget>

class GifCreator : public QWidget {
  Q_OBJECT

public:
  explicit GifCreator(QWidget *parent = nullptr);
  virtual ~GifCreator() = default;

public slots:
  QString imageFilePathMask() const;
  QString gifFilePath() const;
  QString gifFullPath() const;

  Q_INVOKABLE bool createGif(int frames_count, int frame_delay);

private:
  QString m_gifFilePath;
};

#endif // GIFCREATOR_H
