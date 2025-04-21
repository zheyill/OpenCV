QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Window平台opencv，seetaface环境
win64{
LIBS += D:\opencv4.5.4\opencv-4.5.4\build\install\x64\mingw\lib\libopencv_*.a
LIBS += D:\SeetaFace\lib\libSeeta*
INCLUDEPATH += D:\opencv4.5.4\opencv-4.5.4\build\install\include
INCLUDEPATH += D:\opencv4.5.4\opencv-4.5.4\include
INCLUDEPATH += D:\opencv4.5.4\opencv-4.5.4\include\opencv2
INCLUDEPATH += D:\SeetaFace\include
INCLUDEPATH += D:\SeetaFace\include\seeta
}
INCLUDEPATH += D:\opencv4.5.4\opencv-4.5.4\build\install\include
INCLUDEPATH += D:\opencv4.5.4\opencv-4.5.4\include\opencv2

INCLUDEPATH += D:\SeetaFace\include\seeta

LIBS += D:\opencv4.5.4\opencv-4.5.4\build\install\x64\mingw\lib\libopencv_*.a
LIBS += D:\SeetaFace\lib\libSeeta*

#Linux平台opencv，seetaface环境
unix{
LIBS +=-L/home/book/opencv-4.5.4/lib -lopencv_world \
-lSeetaFaceDetector \
-lSeetaFaceLandmarker \
-lSeetaFaceRecognizer \
-lSeetaFaceTracker \
-lSeetaNet \
-lSeetaQualityAssessor \

INCLUDEPATH += /home/book/opencv-4.5.4/build/install/include
INCLUDEPATH += /home/book/opencv-4.5.4/include/opencv4
INCLUDEPATH += /home/book/opencv-4.5.4/include/opencv4/opencv2
INCLUDEPATH += /home/book/opencv-4.5.4/include
INCLUDEPATH += /home/book/opencv-4.5.4/include/seeta
}
SOURCES += \
    main.cpp \
    faceattendance.cpp

HEADERS += \
    faceattendance.h

FORMS += \
    faceattendance.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
