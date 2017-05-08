#ifndef _MERCATORSATELLITE_H_
#define _MERCATORSATELLITE_H_

//#include "ltplugin_api.h"
#include "F:\workspace\lt_pro_0519\include\ltplugin\ltplugin_api.h"
//#define ENABLE_MULTITHREAD           /* 支持多线程，I/O部分使用独立线程 */
//#define ENABLE_ONLINE                /* 支持在线下载 */
#define NEW_CACHE                      /* 使用新的CTileCache::Draw() */

#ifdef _MSC_VER
typedef __int64   int64;
#else
typedef long long int64;
#endif

typedef gdiPoint TileID;
typedef gdiPoint Point;
#define MAX_SCALE_LEVEL 20             /* 不能超过32-8 */

class CTileCache;

/*
 * @class 墨卡托卫星影像图层
 */
class CMercatorSatelliteLayer
{
public:
    CMercatorSatelliteLayer();
    virtual ~CMercatorSatelliteLayer();

    virtual int         Init(gdiGraphicsContext* pgc);
    virtual bool        Draw(gdiRect rt, SMG_Rect rect, int scale,
                             int cenx, int ceny, int angle);

protected:
    virtual int         InitDbPath() = 0;
    virtual void        GetTileFileName(int tiledX, int tiledY, int scaleIndex, char *filename) = 0;
    virtual void        GetTileUrl(int tiledX, int tiledY, int scaleIndex, char *url) = 0;
    virtual bool        ValidateFile(const char *filename)
    {
        return true;
    }

    virtual gdiBitmap * GetTileImage(int tiledX, int tiledY, int scaleIndex);
    bool                DownloadTileImage(const char * url, const char * filename);
    Point               LatLngToPixel(int lon, int lat, int zoom);
    int                 GetZoomLevel(int scale);

#ifdef _DEBUG
    void                DrawGrid(gdiPoint pt, int w, int h, int x, int y,
                                 gdiColor color = gdiRGB(255, 0, 0));
#endif

private:
    void                AdjustMapRectForRotate( SMG_Rect &rect , int angle);

protected:
    int                 m_nMaxScaleIndex;
    int                 m_nMinScaleIndex;
    gdiGraphicsContext* m_pgc;
    char                m_dbpath[MAX_PATHLENGTH];

private:
    Point               m_ptLastCoor;  /* 上次偏移计算的中心坐标 */
    Point               m_ptLastOffset; /* 上次偏移计算的偏移量 */

    friend class CTileCache;
    CTileCache *        m_pTileCache;

};


class CDataQueue;

/*
 * @class 卫片缓存区
 */
class CTileCache
{
public:
    CTileCache(CMercatorSatelliteLayer *layer);
    virtual ~CTileCache();

    bool                Draw(gdiGraphicsContext *pgc, int xmin, int xmax,
                             int ymin, int ymax, int scaleIndex,
                             gdiPoint ptOrig, gdiPoint ptRCenter, int angle,
                             int zw, int zh, bool bZoom);

private:
    void                DrawBitmap(gdiGraphicsContext *pgc, gdiBitmap *bmp, 
		                           gdiPoint pt, gdiPoint ptRCenter, int angle, 
								   int zw, int zh, bool bZoom);

#ifdef ENABLE_MULTITHREAD
private:
    void                dataloop();
    static unsigned long image_thread(void *pParam);

private:
    volatile int        m_active;
    osHandle            m_threadmutex;
    osHandle            m_datathread;
    CDataQueue *        m_pQueue;
#endif

private:
    typedef struct _stTraversalNode
    {
        int64           id;
#		ifndef NEW_CACHE
        gdiBitmap *     bmp;
#		endif
        gdiPoint        pt;
    }stTraversalNode;

    typedef struct _stTileNode
    {
        int64           id;
        gdiBitmap *     bmp;
#		ifndef NEW_CACHE
        unsigned char   valid;
#		endif
    }stTileNode;

    stTileNode*         m_nodes;
    int                 m_nCacheSize;
    stTraversalNode*    m_tnodes;
    int                 m_nTraversalSize;
    CMercatorSatelliteLayer* m_layer;
};

#ifdef ENABLE_MULTITHREAD
class CDataQueue
{
public:
    enum
    {
        MAX_ID_NUM_SHIFT = 4,
        MAX_ID_NUM = 1 << MAX_ID_NUM_SHIFT,
        MAX_ID_NUM_MASK = MAX_ID_NUM - 1,
    };
    enum
    {
        CANDIDATE_SIZE_SHIFT = 2,
        CANDIDATE_SIZE = 1 << CANDIDATE_SIZE_SHIFT,
        CANDIDATE_SIZE_MASK = CANDIDATE_SIZE - 1,
    };

    CDataQueue();
    virtual ~CDataQueue();

    gdiBitmap *         GetBitmapFromCandidates(int64 id);
    bool                AddBitmapToCandidates(int64 id, gdiBitmap *bmp);
    bool                FetchTask(int64& id);

private:
    bool                Add(int64 id);
    bool                Remove(int64& id);
    bool                IsEmpty();
    bool                IsFull();

private:
    typedef struct _stBitmapNode
    {
        int64           id;
        gdiBitmap *     bmp;
    }stBitmapNode;

    stBitmapNode        m_candidates[CANDIDATE_SIZE];
    int                 m_nLastRemovedNode;
    int64               m_ids[MAX_ID_NUM];
    int                 m_head;
    int                 m_tail;
    osHandle            m_queuemutex;
    osHandle            m_activesemaphore;
};
#endif /*end of ENABLE_MULTITHREAD*/


#endif
