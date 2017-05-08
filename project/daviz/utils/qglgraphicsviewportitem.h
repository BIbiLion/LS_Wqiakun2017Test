/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGLGRAPHICSVIEWPORTITEM_H
#define QGLGRAPHICSVIEWPORTITEM_H

#include "utils_global.h"
#include <QGraphicsItem>

class QGLGraphicsViewportItemPrivate;
class QGLPainter;
class QGLCamera;

class UTILS_EXPORT QGLGraphicsViewportItem : public QGraphicsItem
{
public:
    QGLGraphicsViewportItem(QGraphicsItem *parent = 0);
    QGLGraphicsViewportItem(const QRectF& rect, QGraphicsItem *parent = 0);
    QGLGraphicsViewportItem(qreal x, qreal y, qreal w, qreal h,
                            QGraphicsItem *parent = 0);
    ~QGLGraphicsViewportItem();

    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);

    QRectF boundingRect() const;

    QGLCamera *camera() const;
    void setCamera(QGLCamera *camera);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    virtual void paintGL(QGLPainter *painter) = 0;

private:
    QScopedPointer<QGLGraphicsViewportItemPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QGLGraphicsViewportItem)
    Q_DISABLE_COPY(QGLGraphicsViewportItem)
};

void QGLGraphicsViewportItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x, y, w, h));
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
