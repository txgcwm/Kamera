#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <string>

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QImage>

#include "RtspSession.h"



class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(const char* progName, QWidget* parent = 0);
    virtual ~AppWindow(){};
private slots:
    void onStart();
    void onStop();
    void updateFrame(const QImage& image);
private:
    void setupUi();
    void bindEvents();
private:
    std::string _progName;
    QLabel* _cameraScreen;
    QPushButton* _startCaptureButton;
    QPushButton* _stopCaptureButton;
    RtspSession* _rtspSession;
};


#endif // APP_WINDOW_H
