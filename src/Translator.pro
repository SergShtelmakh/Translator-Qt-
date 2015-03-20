#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T20:44:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Translator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        LexicalAnalyzer.cpp \
        Identifier.cpp \
        Token.cpp \
        SourceCodeInputTextEdit.cpp \
        CompileOutputTextEdit.cpp \
        LexicalAnalysisHTMLMarkupGenerator.cpp \
        HTMLColors.cpp \
        SyntacticAnalyzer.cpp \
        SyntacticSymbol.cpp \
        Production.cpp \
        FileReader.cpp

HEADERS += mainwindow.h\
        LexicalAnalyzer.h \
        Identifier.h \
        Token.h \
        SourceCodeInputTextEdit.h \
        CompileOutputTextEdit.h \
        LexicalAnalysisHTMLMarkupGenerator.h \
        HTMLColors.h \
        SyntacticAnalyzer.h \
        SyntacticSymbol.h \
        Production.h \
        FileReader.h

FORMS  += mainwindow.ui

CONFIG += mobility


