/*****************************************************************************
 * Copyright (C) 2010 Jan Lepper <dehtris@yahoo.de>                          *
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

#include  "quickfilter.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>

#include <KI18n/KLocalizedString>

QuickFilter::QuickFilter(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);

    QToolButton *closeButton = new QToolButton(this);
    closeButton->setAutoRaise(true);
    closeButton->setIcon(QIcon::fromTheme("dialog-close"));
    connect(closeButton, SIGNAL(clicked()), SIGNAL(stop()));
    layout->addWidget(closeButton);

    QLabel *label = new QLabel(i18n("Filter:"), this);
    layout->addWidget(label);

    _lineEdit = new KLineEdit(this);
    _lineEdit->setClearButtonShown(true);
    layout->addWidget(_lineEdit);

    setMatch(true);
}

void QuickFilter::setMatch(bool match)
{
    Q_UNUSED(match)
    // TODO, set color of lineedit to highlight if anything matches
}
