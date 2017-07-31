#ifndef QCHARTVIEWER_H
#define QCHARTVIEWER_H

#include <QLabel>
#include "chartdir.h"

//
// Mouse usage mode constants
//
namespace Chart
{
    enum
    {
        MouseUsageDefault = 0,
        MouseUsageScroll = 1,
        MouseUsageZoomIn = 3,
        MouseUsageZoomOut = 4
    };
}

class QChartViewer : public QLabel, public ViewPortManager
{
Q_OBJECT
public:
    explicit QChartViewer(QWidget *parent = 0);
    ~QChartViewer();

    //
    // QChartViewer properties
    //

    virtual void setChart(BaseChart *c);
    virtual BaseChart *getChart();

    virtual void setImageMap(const char *imageMap);
    virtual ImageMapHandler *getImageMapHandler();

    virtual void setDefaultToolTip(QString text);

    virtual void setSelectionBorderWidth(int width);
    virtual int getSelectionBorderWidth();

    virtual void setSelectionBorderColor(QColor c);
    virtual QColor getSelectionBorderColor();

    virtual void setMouseUsage(int mouseUsage);
    virtual int getMouseUsage();

    virtual void setZoomDirection(int direction);
    virtual int getZoomDirection();

    virtual void setScrollDirection(int direction);
    virtual int getScrollDirection();

    virtual void setZoomInRatio(double ratio);
    virtual double getZoomInRatio();

    virtual void setZoomOutRatio(double ratio);
    virtual double getZoomOutRatio();

    virtual void setMinimumDrag(int offset);
    virtual int getMinimumDrag();

    virtual void setUpdateInterval(int interval);
    virtual int getUpdateInterval();

    virtual bool needUpdateChart();
    virtual bool needUpdateImageMap();

    //
    // QChartViewer methods
    //

    // Trigger the ViewPortChanged event
    virtual void updateViewPort(bool needUpdateChart, bool needUpdateImageMap);

protected:
    //
    // QChartViewer event handlers
    //

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void timerEvent (QTimerEvent *event);
    virtual void onSetCursor();

private:
    //
    // QChartViewer configurable properties
    //

    BaseChart *m_currentChart;          // Current BaseChart object
    ImageMapHandler *m_hotSpotTester;   // ImageMapHander representing the image map
    QString m_defaultToolTip;           // Default tool tip text
    QColor m_selectBoxLineColor;      // Selectiom box border color
    int m_selectBoxLineWidth;           // Selectiom box border width
    int m_mouseUsage;                   // Mouse usage mode
    int m_zoomDirection;                // Zoom direction
    int m_scrollDirection;              // Scroll direction
    double m_zoomInRatio;               // Zoom in ratio
    double m_zoomOutRatio;              // Zoom out ratio
    int m_minDragAmount;                // Minimum drag amount
    int m_updateInterval;               // Minimum interval between chart updates
    bool m_needUpdateChart;             // Has pending chart update request
    bool m_needUpdateImageMap;          // Has pending image map udpate request

    //
    // Keep track of mouse states
    //

    int m_currentHotSpot;               // The hot spot under the mouse cursor.
    bool m_isClickable;                 // Mouse is over a clickable hot spot.
    bool m_isOnPlotArea;                // Mouse is over the plot area.
    bool m_isPlotAreaMouseDown;         // Mouse left button is down in the plot area.
    bool m_isDragScrolling;             // Is current dragging scrolling the chart.

    //
    // Dragging support
    //

    int m_plotAreaMouseDownXPos;        // The starting x coordinate of the mouse drag.
    int m_plotAreaMouseDownYPos;        // The starting y coordinate of the mouse drag.
    bool isDrag(int direction, QMouseEvent *event);           // Check if mouse is dragging
    void onPlotAreaMouseDrag(QMouseEvent *point);             // Process mouse dragging

    //
    // Selection rectangle
    //

    QLabel *m_LeftLine;                 // Left edge of selection rectangle
    QLabel *m_RightLine;                // Right edge of selection rectangle
    QLabel *m_TopLine;                  // Top edge of selection rectangle
    QLabel *m_BottomLine;               // Bottom edge of selection rectangle

    void initRect();                    // Initialize selection rectangle edges
    void drawRect(int x, int y, int width, int height);     // Draw selection rectangle
    void setRectVisible(bool b);        // Show/hide selection rectangle

    //
    // Chart update rate control
    //

    bool m_holdTimerActive;             // Chart is in on-hold state.
    int m_holdTimerId;                  // The id of the hold timer.
    bool m_isInViewPortChanged;          // Flag to avoid infinite update loops

signals:
    void viewPortChanged();
    void clicked(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);

public slots:

};

#endif // QCHARTVIEWER_H
