#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T14:08:19
#
#-------------------------------------------------

QT       += core gui sql xml

CONFIG += c++11

# BUILDDIR:     Ausgabe Ziel für das Programm
CONFIG(debug, debug|release){
    DESTDIR = $$PWD/Debug
}
CONFIG(release, debug|release):{
    DESTDIR = $$PWD/Release
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TicketSystem
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TicketModel.cpp \
    TicketItem.cpp \
    FilterObject.cpp \
    NewTicket.cpp \
    LoginWidget.cpp \
    Global.cpp \
    TicketDetails.cpp \
    CommentItem.cpp \
    TicketsWidget.cpp \
    ProjectTasksWidget.cpp \
    TreeItem.cpp \
    TreeItemData.cpp \
    TreeModel.cpp \
    AddTreeItemCommand.cpp \
    RemoveTreeItemCommand.cpp \
    ProjectItemData.cpp \
    PTaskItemData.cpp \
    TaskGanttDelegate.cpp \
    NewPTask.cpp

HEADERS  += MainWindow.h \
    TicketModel.h \
    TicketItem.h \
    FilterObject.h \
    NewTicket.h \
    LoginWidget.h \
    Global.h \
    TicketDetails.h \
    CommentItem.h \
    TicketsWidget.h \
    ProjectTasksWidget.h \
    TreeItem.h \
    TreeItemData.h \
    TreeModel.h \
    AddTreeItemCommand.h \
    RemoveTreeItemCommand.h \
    ProjectItemData.h \
    PTaskItemData.h \
    TaskGanttDelegate.h \
    NewPTask.h

FORMS    += mainwindow.ui \
    NewTicket.ui \
    LoginWidget.ui \
    TicketDetails.ui \
    TicketsWidget.ui \
    ProjectTasksWidget.ui \
    NewPTask.ui

RESOURCES += \
    rsc.qrc
