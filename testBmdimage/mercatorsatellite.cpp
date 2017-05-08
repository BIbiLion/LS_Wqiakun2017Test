#include "mercatorsatellite.h"
#include "ltapi.h"
#include "ltplugin.h"

#define  SATELLITE_CACHE_SIZE  64      /* 对于不同大小的屏幕，需要调整合适的缓存上限 */

#ifndef PI
#define PI 3.14159265 
#endif
#define PIXEL_PER_METER 	3779
#define SCALE(z)	(1.4*2*PI*6378137*PIXEL_PER_METER/(1<<(z+8))) 

#define  __20_MASK						((1<<20)-1)
#define  GET_SIGN_20(x)                 ((x)|(0-((x)&(1<<19))))
#define  GET_SIGN_8(x)                  ((x)|(0-((x)&(1<<7))))
#define  DECODE_ID_X(id)				GET_SIGN_20((int)((id) & __20_MASK))
#define  DECODE_ID_Y(id)                GET_SIGN_20((int)(((id)>>20) & __20_MASK))
#define  DECODE_ID_LEVEL(id)			GET_SIGN_8((int)(((id)>>40)&0xFF))
#define  ENCODE_ID(x,y,level,id)		(id) = (((x)&__20_MASK)|((int64)((y)&__20_MASK)<<20)|((int64)((level)&0xFF)<<40))
#define  DECODE_ID(x,y,level,id)		(x) = DECODE_ID_X(id);(y) = DECODE_ID_Y(id);(level) = DECODE_ID_LEVEL(id)

/*-----------------------------------------------------------------------------
**                         CMercatorSatelliteLayer                           **
-----------------------------------------------------------------------------*/
CMercatorSatelliteLayer::CMercatorSatelliteLayer()
{
	osmemset(&m_ptLastCoor, 0, sizeof(m_ptLastCoor));
	osmemset(&m_ptLastOffset, 0, sizeof(m_ptLastOffset));
	m_pTileCache        = NULL;
	m_nMaxScaleIndex    = MAX_SCALE_LEVEL;
	m_nMinScaleIndex    = 1;
}

CMercatorSatelliteLayer::~CMercatorSatelliteLayer()
{
	if(m_pTileCache)
	{
		delete m_pTileCache;
		m_pTileCache = NULL;
	}
}

int CMercatorSatelliteLayer::Init(gdiGraphicsContext* pgc)
{
	m_pTileCache = new CTileCache(this);
	m_pgc = pgc;	

	return InitDbPath();	
}

bool CMercatorSatelliteLayer::Draw(gdiRect rt, SMG_Rect rect, int scale, 
								   int cenx, int ceny, int angle)
{
	Point centerPoint = {(rt.left + rt.right) / 2, (rt.top + rt.bottom) / 2};
	
	int scaleIndex = GetZoomLevel(scale);	
	if(scaleIndex > m_nMaxScaleIndex || scaleIndex < m_nMinScaleIndex)
	{
		return false;
	}

	//计算图片缩放比例
	//由于地图和卫片使用相同的投影，经度和纬度方向的放缩比相同
	//如果地图不使用墨卡托投影，经度和纬度方向的放缩比必须分别给定
	double fZoomRatio = SCALE(scaleIndex) / scale;
	double fWRatio, fHRatio;
	fWRatio = fHRatio = fZoomRatio;
	////TO DO:旋转时rect范围有时偏小，边缘可能缺图
	AdjustMapRectForRotate(rect , angle);

	Point blPixel = LatLngToPixel(rect.m_lMinLongitude, rect.m_lMinLatitude, scaleIndex);
	Point trPixel = LatLngToPixel(rect.m_lMaxLongitude, rect.m_lMaxLatitude, scaleIndex);
	int ww = (int)osabs(blPixel.x - trPixel.x);
	int hh = (int)osabs(blPixel.y - trPixel.y);

	bool bZoom = true;
	if( ((int)(fWRatio * 100.0) >= 99 && (int)(fWRatio * 100.0) <= 101)
		&& ((int)(fHRatio * 100.0)>=99 && (int)(fHRatio * 100.0)<=101) )
	{
		fWRatio = 1.0;
		fHRatio = 1.0;
		bZoom = false;
	}
	
	//计算屏幕旋转中心
	SMG_Point ptView = {cenx, ceny};
	gdiPoint ptRCenter = {0};
	ScLPtoDP(&ptView, &ptRCenter);
	
	//获取中心点位置
	Point centerPixel = LatLngToPixel(cenx, ceny, scaleIndex);

	//计算中心图块的id和偏移量
	TileID centerTile;
	Point offset;
	centerTile.x = centerPixel.x / 256;
	centerTile.y = centerPixel.y / 256;
	offset.x = centerPixel.x % 256;
	offset.y = centerPixel.y % 256;
	
	//计算id范围和第一张图片的左上角偏移量
	int xmin,xmax,ymin,ymax;
	xmin = xmax = centerTile.x;
	ymin = ymax = centerTile.y;
	Point ptLeftTop;
	ptLeftTop.x = int( centerPoint.x - offset.x * fWRatio );
	ptLeftTop.y = int( centerPoint.y - offset.y * fHRatio );
	if(ww/2 - offset.x > 0)
	{
		int nLeft = (ww/2 - offset.x) / 256 + 1;
		xmin -= nLeft;
		ptLeftTop.x -= int ( nLeft * 256 * fWRatio );
	}
	if(hh/2 - offset.y > 0)
	{
		int nTop = (hh/2 - offset.y) / 256 + 1;
		ymin -= nTop;
		ptLeftTop.y -= int ( nTop * 256 * fHRatio );
	}
	if(ww/2 - 256 + offset.x > 0)
	{
		xmax += (ww/2 - 256 + offset.x) / 256 + 1;
	}
	if(hh/2 - 256 + offset.y > 0)
	{
		ymax += (hh/2 - 256 + offset.y) / 256 + 1;
	}
	
	//边界情况
	int nWrapTiles = 1<<scaleIndex ;
	if(xmin < 0)
	{
		xmin = 0;
	}
	if(xmax >= nWrapTiles)
	{
		xmax = nWrapTiles - 1;
	}
	if(ymin < 0)
	{
		ymin = 0;
	}	
	if(ymax >= nWrapTiles)
	{
		ymax = nWrapTiles - 1;
	}
	
	angle = 360 - angle;
	if(angle == 360)
	{
		angle = 0;
	}

	int zw = int ( 256*fWRatio );
	int zh = int ( 256*fHRatio );	
	gdiPoint pt = {ptLeftTop.x, ptLeftTop.y};	

#if 1
	return m_pTileCache->Draw(m_pgc, xmin, xmax, ymin, ymax, scaleIndex, 
		                      pt, ptRCenter, angle, zw, zh, bZoom);	
#else
	// no cache version
	int x,y;
	for(y = ymin; y <= ymax; y++)
	{			
		for(x = xmin; x <= xmax; x++)		
		{
			gdiBitmap * pBmp = GetTileImage(x, y, scaleIndex);
			if(pBmp)
			{	
				if(!bZoom)
				{
					//if(angle == 0)
					//{
					//	gdiDrawBitmap(m_pgc, pBmp, &pt, NULL);	
					//}
					//else
					{
						//if(centerTile.x == x && centerTile.y == y)
						gdiRotateDrawBitmap(m_pgc, pBmp, pt, ptRCenter, angle);
					}
				}
				else
				{
					//缩放
					gdiRect rr = {0,0,zw,zh};
					gdiBitmap * pTempBmp = gdiZoomBmpByRect2(pBmp, rr);					
					if(pTempBmp)
					{		
						gdiRotateDrawBitmap(m_pgc, pTempBmp, pt, ptRCenter, angle);
						gdiDestroyBitmap(NULL, pTempBmp);
					}
				}
				gdiDestroyBitmap(NULL, pBmp);
			}

			DrawGrid(pt, zw, zh, x, y);
			pt.x += zw;
		}
		pt.y += zh;
		pt.x = ptLeftTop.x;
	}

	return true;
#endif
}

void CMercatorSatelliteLayer::AdjustMapRectForRotate( SMG_Rect &rect , int angle )
{
	int x = ( rect.m_lMaxLongitude - rect.m_lMinLongitude ) / 3;
	int y = ( rect.m_lMaxLatitude  - rect.m_lMinLatitude  ) / 2;

#ifdef NEW_CACHE
	rect.m_lMaxLatitude  += y ;
	rect.m_lMaxLongitude += x ;
	rect.m_lMinLatitude  -= y ;
	rect.m_lMinLongitude -= x ;
#else
	if ( 0 <= angle && angle < 90 )
	{
		rect.m_lMaxLatitude  += y ;
		rect.m_lMaxLongitude += x ;
	}
	else if ( 90 <= angle && angle < 180 )
	{
		rect.m_lMinLatitude  -= y ;
		rect.m_lMaxLongitude += x ;
	}
	else if ( 180 <= angle && angle < 270 )
	{
		rect.m_lMinLatitude  -= y ;
		rect.m_lMinLongitude -= x ;
	}
	else if ( 270 <= angle && angle < 360 )
	{
		rect.m_lMaxLatitude  += y ;
		rect.m_lMinLongitude -= x ;
	}
#endif
}

Point CMercatorSatelliteLayer::LatLngToPixel(int lon, int lat, int zoom)
{
	double dlon = lon/100000.0;
	double dlat = lat/100000.0;

	int centerPoint = 1<<(zoom+7);
	int totalPixels = centerPoint*2;
	double pixelsPerLngDegree = totalPixels/360.0;
	double pixelsPerLngRadian = totalPixels/(2*PI);
	
	double siny = ossin(dlat*PI/180.0);
	if(siny < -0.9999)
		siny = -0.9999;
	else if(siny > 0.9999)
		siny = 0.9999;

	Point centerPixel;
	centerPixel.x = (int)(centerPoint + dlon * pixelsPerLngDegree + 0.5);
	centerPixel.y = (int)(centerPoint + 0.5 - 0.5 * osln((1+siny)/(1-siny)) * pixelsPerLngRadian);
	return centerPixel;
}

int CMercatorSatelliteLayer::GetZoomLevel(int scale)
{
	int smax = int ( SCALE(MAX_SCALE_LEVEL) ) ;
	int z = 0;
	for(z = MAX_SCALE_LEVEL; z >= 1; z--)
	{
		if(scale > smax)
		{
			scale >>= 1;			
		}
		else
		{
			break;
		}
	}
	return z;	 	
}

#ifdef _DEBUG
void CMercatorSatelliteLayer::DrawGrid( gdiPoint pt, int w, int h, int x, int y, 
									   gdiColor color /*= gdiRGB(255,0,0)*/ )
{
	/*gdiRect trect = {pt.x, pt.y, pt.x+w, pt.y+h};
	gdiSetPenColor(m_pgc, color);
	gdiSetPenWidth(m_pgc, 1);
	gdiSetPenStyle(m_pgc, GDI_PS_SOLID, NULL);
	gdiSetBrushStyle(m_pgc,GDI_BS_NULL);
	gdiRectangle(m_pgc, &trect);
	gdiSetFont(m_pgc, NULL, 20);
	gdiSetTextColor(m_pgc, color);
	unsigned short wcText[100] = {0};
	const unsigned short str[] = {0x25,0x64,0x2c,0x25,0x64,0};
	ossnwprintf(wcText, 100, str, x, y);
	gdiText(m_pgc, &pt, wcText, 100);*/
}
#endif

gdiBitmap * CMercatorSatelliteLayer::GetTileImage(int tiledX, int tiledY, int scaleIndex)
{	
	char realfilename[MAX_PATHLENGTH] = {0};
	GetTileFileName(tiledX, tiledY, scaleIndex, realfilename);

	unsigned short filefullname[MAX_PATHLENGTH] = {0};
	osMultiByteToWideChar(filefullname, realfilename, MAX_PATHLENGTH);

	//先从文件加载
	gdiBitmap * pbmp = gdiCreateBitmapFromFile(NULL, filefullname);
	if(pbmp)
	{
		return pbmp;
	}

#ifdef ENABLE_ONLINE
	//再从网络下载
	char url[MAX_PATHLENGTH] = {0};
	GetTileUrl(tiledX, tiledY, scaleIndex, url);

	if(DownloadTileImage(url, realfilename))
	{
		return gdiCreateBitmapFromFile(NULL, filefullname);
	}
#endif

	return NULL;	
}

bool CMercatorSatelliteLayer::DownloadTileImage(const char * url, const char * filename)
{
	char tmpfilename[MAX_PATHLENGTH] = {0};
	osstrncpy(tmpfilename, filename, MAX_PATHLENGTH-1);
	osstrncat(tmpfilename, ".tmp", MAX_PATHLENGTH-1);

#if 1
	osHandle hh = osNetCreate();
	if(hh == NULL)
		return false;

	if(osNetConnect(hh, url, "GET", NULL, NULL) < 0)
	{
		osNetDestroy(hh);
		return false;
	}

	char username[64]= {0};
	char password[64] = {0};
	sgcGetGlobalProxy(username, 64, password, 64);
	if (username[0]!='\0' && password[0]!='\0')
	{
		osNetSetProxy(hh, username, 63,  password, 63);
	}
	
	if(osNetRecvFile(hh, tmpfilename, 80000) < 0)
	{
		osNetDestroy(hh);
		return false;
	}
	osNetDestroy(hh);

#else
	osHttpHandle hh = osHttpCreate(0);
	char username[64]= {0};
	char password[64] = {0};
	sgcGetGlobalProxy(username, 64, password, 64);
	if (username[0]!='\0' && password[0]!='\0')
		osHttpSetProxy(hh, username, password);

	if(osHttpGet(hh, url) < 0)
	{
		osHttpDestroy(hh);
		return false;
	}

	if(osHttpSaveToFile(hh, 0, tmpfilename) < 0)
	{
		osHttpDestroy(hh);
		return false;
	}
	osHttpDestroy(hh);
#endif

	if(!ValidateFile((const char *)tmpfilename))
	{
		osDeleteFile((const char *)tmpfilename);
		return false;
	}

	osMoveFile((const char *)tmpfilename, (const char *)filename);
	return true;
}

/*-----------------------------------------------------------------------------
**                                CTileCache                                 **
-----------------------------------------------------------------------------*/
CTileCache::CTileCache(CMercatorSatelliteLayer *layer)
{
	m_layer             = layer;
	m_nCacheSize        = SATELLITE_CACHE_SIZE; 
	m_nodes             = new stTileNode[m_nCacheSize];
	osmemset(m_nodes, 0, sizeof(stTileNode)*m_nCacheSize);
	m_nTraversalSize    = m_nCacheSize;
	m_tnodes            = new stTraversalNode[m_nTraversalSize];

#ifdef ENABLE_MULTITHREAD
	m_pQueue            = new CDataQueue;
	m_active            = 1;
	m_datathread        = osCreateThread(CTileCache::image_thread, this, OS_PRIORITY_NORMAL);
#endif
}

CTileCache::~CTileCache()
{
	if(m_nodes)
	{
		int i;
		for(i = 0; i < m_nCacheSize; i++)
		{
			if(m_nodes[i].bmp)
			{
				gdiDestroyBitmap(NULL, m_nodes[i].bmp);
			}
		}
		delete m_nodes;
		m_nodes = NULL;
	}

	if(m_tnodes)
	{
		delete m_tnodes;
		m_tnodes = NULL;
	}
	
#ifdef ENABLE_MULTITHREAD
	if(m_datathread)
	{
		m_active = 0;
		osWaitMutex(m_threadmutex, OS_INFINITE);
		osTerminateThread(m_datathread);
		m_datathread = NULL;
		osReleaseMutex(m_threadmutex);
	}
	if(m_pQueue)
	{
		delete m_pQueue;
		m_pQueue = NULL;
	}
#endif
}

#ifdef ENABLE_MULTITHREAD
unsigned long CTileCache::image_thread(void *pParam)
{ 		
	CTileCache* cache = (CTileCache *)pParam;
	if(!cache)	
        return 0;

	cache->dataloop();	
	return 0;
}

void CTileCache::dataloop()
{
	int64 id;
	int tiledX, tiledY, scaleIndex;

	osWaitMutex(m_threadmutex, OS_INFINITE);
	while(m_active)
	{
		if(m_pQueue->FetchTask(id))
		{
			//osprintk("fetch id = %lld\n",id);
			DECODE_ID(tiledX, tiledY, scaleIndex, id);
			gdiBitmap* bmp = m_layer->GetTileImage(tiledX, tiledY, scaleIndex);
			if(bmp)
			{
				m_pQueue->AddBitmapToCandidates(id, bmp);
				sgcRefresh();
			}
		}
		//osSleep(10);
	}
	osReleaseMutex(m_threadmutex);
}
#endif

void CTileCache::DrawBitmap(gdiGraphicsContext *pgc, gdiBitmap *bmp, gdiPoint pt,
							gdiPoint ptRCenter, int angle, int zw, int zh, bool bZoom)
{
	if(bZoom)
	{
		//缩放
		gdiRect rr = {0,0,zw,zh};
		gdiBitmap * pTempBmp = gdiZoomBmpByRect2(bmp, rr);					
		if(pTempBmp)
		{		
			gdiRotateDrawBitmap(pgc, pTempBmp, pt, ptRCenter, angle);
			gdiDestroyBitmap(NULL, pTempBmp);
		}
	}
	else
	{
		gdiRotateDrawBitmap(pgc, bmp, pt, ptRCenter, angle);
	}
}



#ifdef NEW_CACHE
bool CTileCache::Draw(gdiGraphicsContext *pgc, int xmin, int xmax, int ymin, int ymax, 
					  int scaleIndex, gdiPoint ptOrig, gdiPoint ptRCenter, int angle,
					  int zw, int zh, bool bZoom)
{
	const int listSize = 1000;
	stTraversalNode nodeList[listSize] ;
	osmemset(nodeList, 0 , sizeof(stTraversalNode) * listSize);
	int pos = 0;
	gdiPoint pt = ptOrig;

	/* 计算需要加载的卫片ID */
	for ( int x = xmin ; x <= xmax ; ++x )
	{
		for ( int y = ymin ; y <= ymax ; ++y )
		{
			int64 id;
			ENCODE_ID(x , y, scaleIndex , id);
			nodeList[pos].id = id;
			nodeList[pos].pt = pt;
			pos++;

			pt.y += zh;

#			ifdef _DEBUG
			if ( 0 == id ) osprintk("Exception : Satellite image id can't be zero !\n");
#			endif
		}
		pt.x += zw;
		pt.y = ptOrig.y;
	}

	/* 销毁已经无效(不需要再显示)的卫片 */
	for ( int i = 0 ; i != m_nCacheSize ; ++i )
	{
		stTileNode *pNode = &m_nodes[i];
		if ( NULL == pNode->bmp )
			continue;

		osSleep(0);

		bool isValidNode = false;
		for ( int j = 0 ; j != listSize && nodeList[j].id !=0 ; ++j )
		{
			if ( pNode->id == nodeList[j].id )
			{
				isValidNode = true;
				break;
			}
		}

		if ( !isValidNode )
		{
			gdiDestroyBitmap( NULL, pNode->bmp );
			osmemset(pNode, 0, sizeof(stTileNode));
		}
	}

	bool hasDraw = false;
	/* 绘制卫片 */
	for ( int i = 0 ; i != listSize && nodeList[i].id != 0 ; ++i )
	{
		stTraversalNode node = nodeList[i];
		gdiBitmap * bmp = NULL;		

		for( int j = 0 ; j != m_nCacheSize ; ++j )
		{
			if ( node.id == m_nodes[j].id )
			{
				bmp = m_nodes[j].bmp;
				break;
			}
		}

		if ( NULL == bmp )
		{
			/* 缓存当中没有相应ID的卫片，从文件中读取 */
			int x = DECODE_ID_X(node.id);
			int y = DECODE_ID_Y(node.id);
			bmp = m_layer->GetTileImage(x, y, scaleIndex);

			if ( bmp )
			{
				stTileNode * pNode = NULL;
				bool hasFindSpace = false;
				for ( int k = 0 ; k != m_nCacheSize ; ++k )
				{
					pNode = &m_nodes[k];
					if ( NULL == pNode->bmp )
					{
						hasFindSpace = true;
						break;
					}
				}

				if ( !hasFindSpace ) 
				{
#					ifdef _DEBUG
					osprintk("CacheSize(%d) is too small!\n", m_nCacheSize);
#					endif

					/* 缓存不足时，直接存储在第一个位置 */
					pNode = &m_nodes[0];
					if ( pNode->bmp )
						gdiDestroyBitmap(NULL, pNode->bmp);
				}

				pNode->bmp = bmp;
				pNode->id  = node.id;
			}						
		}

		if ( bmp )/* 有些地方可能没有卫片 */
		{
			DrawBitmap(pgc, bmp, node.pt, ptRCenter, angle, zw, zh, bZoom);
			hasDraw = true;
#			ifdef _DEBUG
			m_layer->DrawGrid(node.pt, zw, zh , node.pt.x , node.pt.y);
#			endif
		}		
	}	

	return hasDraw;
}
#else
bool CTileCache::Draw(gdiGraphicsContext *pgc, int xmin, int xmax, int ymin, int ymax, 
					  int scaleIndex, gdiPoint ptOrig, gdiPoint ptRCenter, int angle,
					  int zw, int zh, bool bZoom)
{
	bool bDraw = false;
	gdiPoint pt = ptOrig;

	int totalnum = (xmax-xmin+1)*(ymax-ymin+1);
	if(totalnum > m_nTraversalSize)
	{
		do
		{
			m_nTraversalSize *= 2;
		}
		while(totalnum > m_nTraversalSize);

		delete m_tnodes;
		m_tnodes = new stTraversalNode[m_nTraversalSize];		
	}

	int pos1,pos2;
	pos1 = 0;
	pos2 = totalnum-1;

	int i,x,y;
	for(i = 0; i < m_nCacheSize; i++)
	{
		m_nodes[i].valid = 0;
	}

	for(x = xmin; x <= xmax; x++)
	{			
		for(y = ymin; y <= ymax; y++)
		{
			int64 id;
			ENCODE_ID(x, y, scaleIndex, id);		
			for(i = 0; i < m_nCacheSize; i++)
			{
				if(id == m_nodes[i].id)
				{		
					//osprintk("find in cache %d\n", i);
					break;
				}
			}

			stTraversalNode node = {id, NULL, pt};
			if(i >= m_nCacheSize)
			{
				//未找到
				m_tnodes[pos2] = node;
				pos2--;
			}
			else
			{
				//找到
				m_nodes[i].valid = 1;
				node.bmp = m_nodes[i].bmp;
				m_tnodes[pos1] = node;
				pos1++;
			}

			pt.y += zh;
		}
		pt.x += zw;
		pt.y = ptOrig.y;
	}	

	//先画一遍
	for(i = 0; i < pos1; i++)
	{
		DrawBitmap(pgc, m_tnodes[i].bmp, m_tnodes[i].pt, ptRCenter, angle, zw, zh, bZoom);
		bDraw = true;
	}

	//请求数据
	for(i = pos1; i < totalnum; i++)
	{
		gdiBitmap* bmp = NULL;
#		if defined( ENABLE_MULTITHREAD )
		bmp = m_pQueue->GetBitmapFromCandidates(m_tnodes[i].id);
#		else
		bmp = m_tnodes[i].bmp;
		if (NULL == bmp)
		{
			x = DECODE_ID_X(m_tnodes[i].id);
			y = DECODE_ID_Y(m_tnodes[i].id);
			bmp = m_layer->GetTileImage(x, y, scaleIndex);
		}		
#		endif

		if(bmp)
		{
			//绘制
			DrawBitmap(pgc, bmp, m_tnodes[i].pt, ptRCenter, angle, zw, zh, bZoom);
			bDraw = true;
			
			//送入m_nodes
			int j;
			for(j = m_nCacheSize-1; j >= 0; j--)
			{
				if(m_nodes[j].valid == 0)
				{
					break;
				}
			}

			if(j < 0)
			{
				//未找到空位，cache太小
				gdiDestroyBitmap(NULL, bmp);
				//osprintk("cache too small id = %lld\n",m_tnodes[i].id);
			}
			else
			{
				if(m_nodes[j].bmp)
				{
					gdiDestroyBitmap(NULL, m_nodes[j].bmp);
					//osprintk("cache conflict id = %lld\n",m_tnodes[i].id);
				}
				stTileNode node = {m_tnodes[i].id, bmp, 1};
				m_nodes[j] = node;	
			}
		}
	}
	return bDraw;
}
#endif

/*-----------------------------------------------------------------------------
**                                CDataQueue                                 **
-----------------------------------------------------------------------------*/

#ifdef ENABLE_MULTITHREAD
CDataQueue::CDataQueue()
{
	m_queuemutex        = osCreateMutex();
	m_activesemaphore   = osCreateSemaphore(0);
	m_tail              = 0;
	m_head              = 0;
	osmemset(m_ids, 0, sizeof(m_ids));
	
	osmemset(m_candidates, 0, sizeof(m_candidates));
	m_nLastRemovedNode  = 0;
}

CDataQueue::~CDataQueue()
{
	int i;
	for(i = 0; i < CANDIDATE_SIZE; i++)
	{
		if(m_candidates[i].bmp)
		{
			gdiDestroyBitmap(NULL, m_candidates[i].bmp);
		}
	}
	osmemset(m_candidates, 0, sizeof(m_candidates));

	osCloseSemaphore(m_activesemaphore);
	osCloseMutex(m_queuemutex);
}

bool CDataQueue::Add(int64 id)
{
	if(IsFull())
	{
		return false;
	}
	
	//判断是否与队列中的id重复
	int next = m_head;
	while(next != m_tail)
	{
		if(id == m_ids[next])
		{
			return false;
		}
		next = (next+1)&MAX_ID_NUM_MASK;
	}
	
	m_ids[m_tail] = id;
	m_tail = (m_tail+1)&MAX_ID_NUM_MASK;	
	//osprintk("queue add head=%d,tail=%d,id=%lld\n", m_head, m_tail,id);
	return true;
}

bool CDataQueue::Remove(int64& id)
{
	if(IsEmpty())
	{		
		return false;
	}
	
	id = m_ids[m_head];
	m_head = (m_head+1)&MAX_ID_NUM_MASK;
	//osprintk("queue remove head=%d,tail=%d,id=%lld\n",m_head, m_tail, id);
	return true;
}

bool CDataQueue::IsFull()
{
	if(m_head == ((m_tail+1)&MAX_ID_NUM_MASK))
		return true;
	return false;
}

bool CDataQueue::IsEmpty()
{
	if(m_head == m_tail)
		return true;
	return false;
}

gdiBitmap * CDataQueue::GetBitmapFromCandidates(int64 id)
{
	osWaitMutex(m_queuemutex, OS_INFINITE);

	int i;
	for(i = 0; i < CANDIDATE_SIZE; i++)
	{
		if(id == m_candidates[i].id)
		{
			gdiBitmap * bmp = m_candidates[i].bmp;
			osmemset(&m_candidates[i], 0, sizeof(stBitmapNode));
			m_nLastRemovedNode = i;
			osReleaseMutex(m_queuemutex);
			//osprintk("remove bmp pos = %d, id = %lld\n",i,id);
			return bmp;
		}
	}

	//osprintk("add id = %lld\n",id);
	bool ret = Add(id);
	osReleaseMutex(m_queuemutex);
	if(ret)
	{
		osReleaseSemaphore(m_activesemaphore);
	}
	return NULL;
}

bool CDataQueue::AddBitmapToCandidates(int64 id, gdiBitmap *bmp)
{
	//osprintk("new bmp id = %lld\n",id);

	stBitmapNode node = {id, bmp};
	osWaitMutex(m_queuemutex, OS_INFINITE);

	int insert = m_nLastRemovedNode;
	if(m_candidates[insert].bmp)
	{
		//可能是最老的候选
		gdiDestroyBitmap(NULL, m_candidates[insert].bmp);
		//osprintk("new bmp destroy old bmp pos = %d, id = %lld\n", insert, m_candidates[insert].id);
	}
	m_candidates[insert] = node;	
	m_nLastRemovedNode = (m_nLastRemovedNode+1)&CANDIDATE_SIZE_MASK;
	osReleaseMutex(m_queuemutex);
	return true;
}

bool CDataQueue::FetchTask(int64& id)
{	
	bool ret = false;
	osWaitSemaphore(m_activesemaphore, OS_INFINITE);
	osWaitMutex(m_queuemutex, OS_INFINITE);
	ret = Remove(id);
	osReleaseMutex(m_queuemutex);
	return ret;
}
#endif /*end of ENABLE_MULTITHREAD*/