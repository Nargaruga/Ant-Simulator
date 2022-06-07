QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ant.cpp \
    ant_sim.cpp \
    cave_gen.cpp \
    custom_graphics_scene.cpp \
    main.cpp \
    main_window.cpp

HEADERS += \
    ant.h \
    ant_sim.h \
    cave_gen.h \
    cell.h \
    colors.h \
    custom_graphics_scene.h \
    grid.h \
    main_window.h \
    sim_cell_data.h

FORMS += \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
