/***************************************************************************
                                 packguibase.h
                             -------------------
    copyright            : (C) 2000 by Shie Erlich & Rafi Yanai
    email                : krusader@users.sourceforge.net
    web site             : http://krusader.sourceforge.net
 ---------------------------------------------------------------------------
  Description
 ***************************************************************************

  A

     db   dD d8888b. db    db .d8888.  .d8b.  d8888b. d88888b d8888b.
     88 ,8P' 88  `8D 88    88 88'  YP d8' `8b 88  `8D 88'     88  `8D
     88,8P   88oobY' 88    88 `8bo.   88ooo88 88   88 88ooooo 88oobY'
     88`8b   88`8b   88    88   `Y8b. 88~~~88 88   88 88~~~~~ 88`8b
     88 `88. 88 `88. 88b  d88 db   8D 88   88 88  .8D 88.     88 `88.
     YP   YD 88   YD ~Y8888P' `8888Y' YP   YP Y8888D' Y88888P 88   YD

                                                     H e a d e r    F i l e

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PACKGUIBASE_H
#define PACKGUIBASE_H

#include <klocale.h>
#include <qdialog.h>


class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class PackGUIBase : public QDialog
{ 
    Q_OBJECT

public:
    PackGUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~PackGUIBase();

    QLabel* TextLabel3;
    QLineEdit* nameData;
    QComboBox* typeData;
    QLabel* TextLabel5;
    QLineEdit* dirData;
    QToolButton* browseButton;
    QLabel* PixmapLabel1;
    QLabel* TextLabel1;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QCheckBox* moveCheckbox;

public slots:
    virtual void browse();

protected:
    QHBoxLayout* hbox;
    QHBoxLayout* hbox_2;
    QHBoxLayout* hbox_3;
    QHBoxLayout* hbox_4;
    QHBoxLayout* hbox_5;
    QGridLayout* grid;
};

#endif // PACKGUIBASE_H
