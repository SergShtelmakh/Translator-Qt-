#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T20:44:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Translator
TEMPLATE = app

INCLUDEPATH += gui
INCLUDEPATH += translator

SOURCES += main.cpp\
        gui/mainwindow.cpp\
        gui/SourceCodeInputTextEdit.cpp \
        gui/CompileOutputTextEdit.cpp \
        gui/HTMLColors.cpp \
        gui/HTMLMarkupGenerator.cpp \
        gui/ProgramBlockTreeWidget.cpp \
        translator/LexicalAnalyzer.cpp \
        translator/Identifier.cpp \
        translator/Token.cpp \
        translator/SyntacticAnalyzer.cpp \
        translator/SyntacticSymbol.cpp \
        translator/Production.cpp \
        translator/Block.cpp \
        translator/SemanticAnalyzer.cpp \
        translator/ThreeAddressCodeGenerator.cpp \
        translator/Expression.cpp \
        translator/Translator.cpp \
        FileReader.cpp \
        BackusNaurFormParser.cpp \
        BackusNaurFormRule.cpp \
        ErrorGenerator.cpp

HEADERS += gui/mainwindow.h\
        gui/SourceCodeInputTextEdit.h \
        gui/CompileOutputTextEdit.h \
        gui/HTMLColors.h \
        gui/HTMLMarkupGenerator.h \
        gui/ProgramBlockTreeWidget.h \
        translator/LexicalAnalyzer.h \
        translator/Identifier.h \
        translator/Token.h \
        translator/SyntacticAnalyzer.h \
        translator/SyntacticSymbol.h \
        translator/Production.h \
        translator/Block.h \
        translator/SemanticAnalyzer.h \
        translator/ThreeAddressCodeGenerator.h \
        translator/Expression.h \
        translator/Translator.h \
        FileReader.h \
        BackusNaurFormParser.h \
        BackusNaurFormRule.h \
        ErrorGenerator.h

FORMS  += gui/mainwindow.ui

CONFIG += mobility


