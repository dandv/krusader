/*****************************************************************************
 * Copyright (C) 2008-2009 Csaba Karai <cskarai@freemail.hu>                 *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This package is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this package; if not, write to the Free Software               *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA *
 *****************************************************************************/

#ifndef QUEUEDIALOG_H
#define QUEUEDIALOG_H

#include <QtWidgets/QDialog>

class QToolButton;
class QueueWidget;
class QLabel;
class QProgressBar;
class Queue;

class QueueDialog : public QDialog
{
    Q_OBJECT

private:
    QueueDialog();

public:
    virtual ~QueueDialog();

    static void showDialog(bool autoHide = true);
    static void deleteDialog();
    static void everyQueueIsEmpty();

public slots:
    void reject() Q_DECL_OVERRIDE;

    void slotUpdateToolbar();
    void slotPauseClicked();
    void slotScheduleClicked();
    void slotNewTab();
    void slotNextTab();
    void slotPrevTab();
    void slotDeleteCurrentTab();
    void slotPercentChanged(Queue *, int);

protected:
    void saveSettings();

private:
    static QueueDialog * _queueDialog;

    QToolButton        * _newTabButton;
    QToolButton        * _closeTabButton;
    QToolButton        * _pauseButton;
    QToolButton        * _scheduleButton;

    QProgressBar       * _progressBar;
    QueueWidget        * _queueWidget;
    QLabel             * _statusLabel;

    bool                 _autoHide;
};

#endif // __QUEUEDIALOG_H__

