#-------------------------------------------------
#
# Project created by QtCreator 2021-09-17T08:03:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH = src

#---------------------------------------------------------------------------------------------------
# Version
#---------------------------------------------------------------------------------------------------
DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = PEDumpGUI_d
DEFINES += DEBUG
} else {
TARGET = PEDumpGUI
}

#---------------------------------------------------------------------------------------------------
# Destinarion directory
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------------------------

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/pedump/coffsymboltable.cpp \
    src/pedump/common.cpp \
    src/RecentFilesMenu.cpp \
    src/items/TreeItem.cpp \
    src/items/TreeItemCOFFHeader.cpp \
    src/items/TreeItemOptionalHeader.cpp \
    src/items/TreeItemFileHeader.cpp \
    src/items/TreeItemROMOptionalHeader.cpp \
    src/items/TreeItemSectionTable.cpp \
    src/items/TreeItemBoundImportDescriptors.cpp \
    src/items/TreeItemImportsSection.cpp \
    src/items/TreeItemRelocations.cpp \
    src/items/TreeItemSymbolTable.cpp \
    src/items/TreeItemResourceSection.cpp \
    src/items/TreeItemExportsSection.cpp \
    src/items/TreeItemRuntimeFunctions.cpp \
    src/items/TreeItemBaseRelocationsSection.cpp \
    src/items/TreeItemDebugDirectory.cpp \
    src/items/TreeItemCVDebugInfo.cpp \
    src/items/TreeItemImageDbgHeader.cpp \
    src/items/TreeItemMiscDebugInfo.cpp \
    src/items/TreeItemLineNumbers.cpp \
    src/AboutDialog.cpp \
    src/items/TreeItemSection.cpp \
    src/pedump/pedump-exe.cpp \
    src/pedump/pedump-obj.cpp \
    src/pedump/pedump-dbg.cpp \
    src/pedump/pedump-rom.cpp \
    src/items/TreeItemArchiveMemberHeader.cpp \
    src/pedump/pedump-lib.cpp \
    src/pedump/pedump.cpp \
    src/items/TreeItemFirstLinkerMember.cpp \
    src/items/TreeItemSecondLinkerMember.cpp \
    src/items/TreeItemLongnamesMember.cpp

HEADERS += \
    src/MainWindow.h \
    src/pedump/coffsymboltable.h \
    src/RecentFilesMenu.h \
    src/items/TreeItem.h \
    src/items/TreeItemCOFFHeader.h \
    src/items/TreeItemOptionalHeader.h \
    src/items/TreeItemFileHeader.h \
    src/items/TreeItemROMOptionalHeader.h \
    src/items/TreeItemSectionTable.h \
    src/items/TreeItemBoundImportDescriptors.h \
    src/items/TreeItemImportsSection.h \
    src/items/TreeItemRelocations.h \
    src/items/TreeItemSymbolTable.h \
    src/items/TreeItemResourceSection.h \
    src/items/TreeItemExportsSection.h \
    src/items/TreeItemRuntimeFunctions.h \
    src/items/TreeItemBaseRelocationsSection.h \
    src/items/TreeItemDebugDirectory.h \
    src/items/TreeItemCVDebugInfo.h \
    src/items/TreeItemImageDbgHeader.h \
    src/items/TreeItemMiscDebugInfo.h \
    src/items/TreeItemLineNumbers.h \
    src/AboutDialog.h \
    src/items/TreeItemSection.h \
    src/items/TreeItemArchiveMemberHeader.h \
    src/pedump/pedump.h \
    src/items/TreeItemFirstLinkerMember.h \
    src/items/TreeItemSecondLinkerMember.h \
    src/items/TreeItemLongnamesMember.h

FORMS += \
    src/MainWindow.ui \
    src/AboutDialog.ui

#---------------------------------------------------------------------------------------------------
# Resource files
#---------------------------------------------------------------------------------------------------

RESOURCES += res/resource.qrc

win32:RC_FILE = res/resource_win32.rc
