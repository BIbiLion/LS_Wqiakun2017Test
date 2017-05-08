
#include <QtWidgets>

#include "mainwindow.h"
#include "gdal_priv.h"
#include "gdal_frmts.h"

//! [0]
MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
//! [0]

//! [1]
    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   /* infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));*/
	infoLabel = new QLabel(tr(""));
	infoLabel->setGeometry(0,0,200,200);
    /*infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);*/
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);
//! [1]

//! [2]
    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);

	m_pDataset = NULL;

	pTifData = NULL;
}
//! [2]

//! [3]
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}
//! [3]

void MainWindow::newFile()
{
	QImage image(tr("d:\\1_0_0.bmp"));//24bit彩色图像 769k
	
	//image.save(tr("d:\\1.png"));//可直接保存为其他格式的数据

	QImage image2 = image.convertToFormat(QImage::Format_Indexed8);//,Qt::ColorMode_Mask);
	image2.save(tr("d:\\1.png"));//8bit彩色图像
	//image2.save(tr("d:\\1.bmp"));

	QImage image3 = image.convertToFormat(QImage::Format_Grayscale8);//,Qt::ColorMode_Mask);
	//image3.save(tr("d:\\gray.bmp"));//258k
	image3.save(tr("d:\\gray.png"));//27k

	QImage* image4 = new QImage(image.width()/2,image.height()/2,QImage::Format_RGB888);
	for(int i=1; i<128; i++)
	{
		for(int j=0; j<128; j++)
		{
			image4->setPixel(i,j,image.pixel(i,j));
		}
	}
	image4->save(tr("d:\\cp.png"));

}

void MainWindow::open()
{
    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
	QFileDialog::Options options;
	QString selectedFilter;
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open PNG Image"),
		".",
		tr("All Files(*.*);;PNG Image (*.png)"),
		&selectedFilter,
		options);

	if (!fileName.isEmpty())
	{
		if(m_pDataset != NULL)
		{
			GDALClose((GDALDatasetH) m_pDataset);
			m_pDataset = NULL;
		}

	}
	showPng(fileName);

}

//读取文件，返回图像的rgb值，rgbrgb...rgb
uchar* MainWindow::readPng_Qt(QString filePath)
{
	QImage image(filePath);//,"PNG");

	int width = image.width();
	int height = image.height();
	int bitCount = image.bitPlaneCount();//位深
	int byteCount = image.byteCount();//字节数

	//颜色相关
	QImage::Format fmt = image.format();
	if(fmt == QImage::Format_Indexed8)
	{
		int clrcnt = image.colorCount();
		int clrsize = image.colorTable().size(); 
		QRgb rgb = image.colorTable().at(0);
		QColor clr = QColor::fromRgb(rgb);
		uchar r = clr.red();
		uchar g = clr.green();
		uchar b = clr.blue();

		uchar* pData = new uchar[width*height*3];
		uchar* p = pData;
		uchar* bits = image.bits();
		for(int i=0; i<width*height; i++)
		{
			QRgb rgb = image.color(*bits);//颜色表
			QColor clr = QColor::fromRgb(rgb);
			*p = clr.red();
			p++;
			*p = clr.green();
			p++;
			*p = clr.blue();
			p++;
			bits++;
		}
		return pData;
	}
	else 
		return NULL;
}

void MainWindow::tilesSplice()
{
	uchar* pData00 = readPng_Qt(tr("D:\\map\\1\\1_0_0.png"));//(tr("d:\\pinjie.bmp"));
	uchar* pData10 = readPng_Qt(tr("D:\\map\\1\\1_1_0.png"));
	uchar* pData01 = readPng_Qt(tr("D:\\map\\1\\1_0_1.png"));
	uchar* pData11 = readPng_Qt(tr("D:\\map\\1\\1_1_1.png"));
	uchar* pDest = new uchar[512*512*3];
	for(int i=0;i<256;i++)
	{
		memcpy(pDest+i*512*3,pData00+i*256*3,256*3);
		memcpy(pDest+i*512*3+256*3,pData10+i*256*3,256*3);
	}
	for(int i=0;i<256;i++)
	{
		memcpy(pDest+(i+256)*512*3,pData01+i*256*3,256*3);
		memcpy(pDest+(i+256)*512*3+256*3,pData11+i*256*3,256*3);
	}
	delete[] pData00;
	delete[] pData10;
	delete[] pData01;
	delete[] pData11;

	QImage destImage(pDest,512,512,QImage::Format_RGB888);
	destImage.save("d:\\pinjie_24b.png");//55K
	//destImage.save("d:\\pinjie.bmp");//769K
	//destImage.save("d:\\pinjie.jpg");//16K 图像变模糊
	QImage image = destImage.convertToFormat(QImage::Format_Indexed8);
	image.save("d:\\pinjie_8b.png");
	delete[] pDest;
}

void MainWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
    
	//QPixmap pix;
    //pix.load("d:/Data/map/4.png");//相对路径或者绝对路径
	//painter.drawPixmap(0,0,pix);    //x、y：图片绘制位置，w, h:绘制图片宽度、高度

	//QImage image("d:/Data/map/4.png");
	//painter.drawImage(0,0,image);

	/*QPixmap pixmapToShow = QPixmap::fromImage( image.scaled(size(), Qt::IgnoreAspectRatio) );
	painter.drawPixmap(0,0, pixmapToShow);*/
		
}


/*
函数名：Qt的拼接方法
*/
void MainWindow::tilesSplice(QString tilesPath)
{
	tilesPath = tr("D:/Data/map/4");
	QString destPath = tr("d:/Data/map/4.png");
		
	//遍历文件，获取文件名和文件数目
	QStringList fileList;
	FindFiles(tilesPath,fileList);
	int fileCount = fileList.count();
	
	//确定目标文件的宽度和高度
    int width = sqrt(fileCount)*256;//一张图片大小256*256    正方形的所有
	int height = width;
	QImage destImage(width,height,QImage::Format_RGB888);
	
	//逐个拼接图像
	for(int i=0; i<fileCount; i++)
	{
		QString name = fileList.at(i);
		//qDebug()<<name;
		QString temp = name.section('_',0,0);
		//qDebug()<<temp;
		int level = temp.toInt();
		temp = name.section('_',1,1);
		//qDebug()<<temp;
		int X = temp.toInt();
		temp = name.section('_',2,2);
		temp = temp.section('.',0,0);
		//qDebug()<<temp;
		int Y = temp.toInt();

		QString srcfile = tilesPath + tr("/") + name;
		qDebug()<<srcfile;
		if(QFile::exists(srcfile))
		{
			//源文件路径，目标图像，源文件在目标图像中的位置x,y(或行列)
            CombineTile(srcfile,destImage,X,Y);//
		}
	}

	//QImage spliceImage = destImage->convertToFormat(QImage::Format_Indexed8);
	if(destImage.save(destPath))
		QMessageBox::information(this,QStringLiteral("拼接结果"),QStringLiteral("拼接完成"),QMessageBox::Ok);
	else
		QMessageBox::information(this,QStringLiteral("拼接结果"),QStringLiteral("拼接失败"),QMessageBox::Ok);
	
	//infoLabel->setPixmap(QPixmap::fromImage(destImage));
}

void MainWindow::CombineTile(QString& srcfile,QImage& image,int& X,int& Y)
{
	X = X * 256;
	Y = Y * 256;
	/*QPixmap pix(srcfile);
	QImage srcImage = pix.toImage();*/
	QImage srcImage(srcfile);
	for(int i=0; i<256; i++)
	{
		for(int j=0; j<256; j++)
		{
			image.setPixel(X+i,Y+j,srcImage.pixel(i,j));
		}
	}
}

void MainWindow::FindFiles( QString& path ,QStringList& fileList)
{
	QString namefilter = tr("*.png");
	QDir dir = QDir(path);
	fileList = dir.entryList(QStringList(namefilter),QDir::Files | QDir::NoSymLinks);
}

void MainWindow::loadPng(QString filePath)
{
	QPixmap pix(filePath);
	infoLabel->setPixmap(pix);
	int depth = pix.depth();
	int clrcount = pix.colorCount();

	QImage image1 = pix.toImage();
	QImage::Format imageFromat1 = image1.format();
	int clrcnt1 = image1.colorCount();
	int clrsize1 = image1.colorTable().size();
	int depth1 = image1.depth();
	QPixelFormat pixelFromat1 = image1.pixelFormat();
	QPixelFormat::ColorModel colorModel1 = pixelFromat1.colorModel();
	int width = image1.width();
	int height = image1.height();
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			QRgb rgb = image1.pixel(i,j);
			QColor clr = QColor::fromRgba(rgb);
			//image1.setPixel(i,j,rgb);
		}
	}
	
	QImage image2(filePath);
	QImage::Format imageFromat2 = image2.format();
	int clrcnt2 = image2.colorCount();
	int clrsize2 = image2.colorTable().size();
	int depth2 = image2.depth();	
	QPixelFormat pixelFromat2 = image2.pixelFormat();
	QPixelFormat::ColorModel colorModel2 = pixelFromat2.colorModel();
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			QRgb rgb = image2.pixel(i,j);
			QColor clr = QColor::fromRgba(rgb);
			//image1.setPixel(i,j,rgb);
		}
	}

	//image2.toImageFormat(QPixelFormat);
	//image2.toPixelFormat(QImage::Format)

	QBitmap bitmap(pix);
	int clr = bitmap.colorCount();
}

void MainWindow::showPng(QString filePath)
{
	QPixmap pixmap;
	GDALAllRegister();
	m_pDataset = (GDALDataset*)GDALOpen(filePath.toUtf8().data(), GA_ReadOnly);
	GDALDriver* pDriver = m_pDataset->GetDriver();
	if (m_pDataset == NULL)
	{
		QMessageBox::information(this,tr("提示"), tr("指定的文件不能打开！"));	
		return;
	}
	int bandCount = m_pDataset->GetRasterCount();
	int nWidth = m_pDataset->GetRasterXSize();
	int nHeight = m_pDataset->GetRasterYSize();
	int sz = 1;
	if(bandCount == 4)
	{   
		unsigned char* pClrData = new unsigned char[bandCount * nWidth * nHeight];
		m_pDataset->RasterIO(GF_Read,0,0,nWidth,nHeight,pClrData,nWidth,nHeight,
			GDT_Byte,bandCount,NULL,sz*bandCount,sz*bandCount*nWidth,sz,NULL);
		QImage QImg(pClrData, nWidth, nHeight, QImage::Format_RGBA8888);
		pixmap = QPixmap::fromImage(QImg);
		delete[] pClrData;
	}
	else if(bandCount == 3)
	{
		unsigned char* pClrData = new unsigned char[bandCount * nWidth * nHeight];
		m_pDataset->RasterIO(GF_Read,0,0,nWidth,nHeight,pClrData,nWidth,nHeight,GDT_Byte,bandCount,NULL,sz*bandCount,sz*bandCount*nWidth,sz,NULL);
		QImage QImg(pClrData, nWidth, nHeight, QImage::Format_RGB888);
		pixmap = QPixmap::fromImage(QImg);
		delete[] pClrData;
	}
	else
	{
		GDALRasterBand* pBand = m_pDataset->GetRasterBand(1);
		GDALDataType pDataType = pBand->GetRasterDataType();
		unsigned char* pData = new unsigned char[nWidth * nHeight];
		pBand->RasterIO(GF_Read,0,0,nWidth,nHeight,pData,nWidth,nHeight,GDT_Byte,0,0,NULL);
		GDALColorInterp ClrInterp = pBand->GetColorInterpretation();

		GDALColorTable* pClrTable = pBand->GetColorTable();
		GDALPaletteInterp ClrPalette = pClrTable->GetPaletteInterpretation();

		//GDALDriver *pDriverJPG = GetGDALDriverManager()->GetDriverByName("PNG");
		//pDriverJPG->CreateCopy("d:\\tem.png",m_pDataset,TRUE,0,0,0); //创建jpg文件 

		//提取RGB值用于显示，RGBRGB...RGB
		unsigned char* pClrData = new unsigned char[3 * nWidth * nHeight];
		unsigned char* p = pClrData;
		for(int i=0; i<nWidth * nHeight; i++)
		{
			*p = pClrTable->GetColorEntry(pData[i])->c1;
			p++;
			*p = pClrTable->GetColorEntry(pData[i])->c2;
			p++;
			*p = pClrTable->GetColorEntry(pData[i])->c3;
			p++;
		}
		QImage QImg(pClrData, nWidth, nHeight, QImage::Format_RGB888);
		QImg.save(tr("d://1222222.bmp"));
		pixmap = QPixmap::fromImage(QImg);
		delete[] pClrData;

		//提取RGB值，保存为24位TIF数据 RRR..RRRGGG..GGGBBB...BBB
		pTifData = new unsigned char[3 * nWidth * nHeight];
		unsigned char* pRed = pTifData;
		unsigned char* pGreen = pTifData + nWidth * nHeight;
		unsigned char* pBlue = pTifData + 2 * nWidth * nHeight;
		for(int i=0; i<nWidth * nHeight; i++)
		{
			*pRed = pClrTable->GetColorEntry(pData[i])->c1;
			pRed++;
			*pGreen = pClrTable->GetColorEntry(pData[i])->c2;
			pGreen++;
			*pBlue = pClrTable->GetColorEntry(pData[i])->c3;
			pBlue++;
		}
		delete[] pData;
		GDALDriver *pDriverTIF = GetGDALDriverManager()->GetDriverByName("BMP");
		GDALDataset* saveDataset = pDriverTIF->Create("d:\\24bit.bmp",3*nWidth,3*nHeight,3,GDT_Byte,NULL);
		saveDataset->RasterIO(GF_Write,0,0,nWidth,nHeight,pTifData,nWidth,nHeight,GDT_Byte,3,0,0,0,0);
		saveDataset->RasterIO(GF_Write,nWidth,0,nWidth,nHeight,pTifData,nWidth,nHeight,GDT_Byte,3,0,0,0,0);
		delete[] pTifData;
		GDALClose((GDALDatasetH)saveDataset);//delete saveDataset;
	}

	infoLabel->setPixmap(pixmap);
}

void MainWindow::ReadData_GDAL(QString filePath)
{
	GDALAllRegister();
	
	m_pDataset = (GDALDataset*)GDALOpen(filePath.toUtf8().data(), GA_ReadOnly);
	GDALDriver* pDriver = m_pDataset->GetDriver();
	if (m_pDataset == NULL)
	{
		QMessageBox::information(this,tr("提示"), tr("指定的文件不能打开！"));	
		return;
	}
	int bandCount = m_pDataset->GetRasterCount();
	int nWidth = m_pDataset->GetRasterXSize();
	int nHeight = m_pDataset->GetRasterYSize();

	GDALRasterBand* pBand = m_pDataset->GetRasterBand(1);
	GDALDataType pDataType = pBand->GetRasterDataType();
	unsigned char* pData = new unsigned char[nWidth * nHeight];
	pBand->RasterIO(GF_Read,0,0,nWidth,nHeight,pData,nWidth,nHeight,GDT_Byte,0,0,NULL);
	GDALColorInterp ClrInterp = pBand->GetColorInterpretation();

	GDALColorTable* pClrTable = pBand->GetColorTable();
	GDALPaletteInterp ClrPalette = pClrTable->GetPaletteInterpretation();

	//提取RGB值，保存为24位TIF数据 RRR..RRRGGG..GGGBBB...BBB
	if(pTifData!=NULL)
	{
		delete[] pTifData;
		pTifData = NULL;
	}
	pTifData = new unsigned char[3 * nWidth * nHeight];
	unsigned char* pRed = pTifData;
	unsigned char* pGreen = pTifData + nWidth * nHeight;
	unsigned char* pBlue = pTifData + 2 * nWidth * nHeight;
	for(int i=0; i<nWidth * nHeight; i++)
	{
		*pRed = pClrTable->GetColorEntry(pData[i])->c1;
		pRed++;
		*pGreen = pClrTable->GetColorEntry(pData[i])->c2;
		pGreen++;
		*pBlue = pClrTable->GetColorEntry(pData[i])->c3;
		pBlue++;
	}
	delete[] pData;
	GDALClose((GDALDatasetH)m_pDataset);
}

void MainWindow::PinJie()
{
	GDALAllRegister();
	int nWidth = 256;
	int nHeight = 256;
	GDALDriver *pDriverTIF = GetGDALDriverManager()->GetDriverByName("BMP");
	GDALDataset* saveDataset = pDriverTIF->Create("d:\\24bit.bmp",2*nWidth,nHeight,3,GDT_Byte,NULL);
	ReadData_GDAL(tr("D:\\map\\1\\1_0_0.png"));
	saveDataset->RasterIO(GF_Write,0,0,nWidth,nHeight,pTifData,nWidth,nHeight,GDT_Byte,3,0,0,0,0);
	ReadData_GDAL(tr("D:\\map\\1\\1_1_0.png"));
	saveDataset->RasterIO(GF_Write,nWidth,0,nWidth,nHeight,pTifData,nWidth,nHeight,GDT_Byte,3,0,0,0,0);
	delete saveDataset;
}

void MainWindow::save()
{
	tilesSplice(tr(""));
    //infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::print()
{
	PinJie();
   // infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::undo()
{
	loadPng("D:\\Data\\map\\1\\1_0_0.png");
}

void MainWindow::redo()
{
    //infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
	infoLabel->setText(tr("NI"));
}

void MainWindow::center()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

//! [4]
void MainWindow::createActions()
{
//! [5]
    newAct = new QAction(QStringLiteral("&图像Format转换"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
//! [4]

    openAct = new QAction(QStringLiteral("&读取PNG-GDAL"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//! [5]
    saveAct = new QAction(QStringLiteral("&拼接-Qt"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QStringLiteral("&拼接-GDAL"), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(QStringLiteral("&加载png"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

//! [6] //! [7]
    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
//! [6]
}
//! [7]

//! [8]
void MainWindow::createMenus()
{
//! [9] //! [10]
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
//! [9]
    fileMenu->addAction(openAct);
//! [10]
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
//! [11]
    fileMenu->addSeparator();
//! [11]
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
//! [8]

//! [12]
    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}
//! [12]
