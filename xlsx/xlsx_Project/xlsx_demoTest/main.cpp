/*
 * excel 文件的操作xlsx
 * 注意行列的话，是从 1行1列开始的
 * 另外注意：cell单元格 A1 B1是大写的字母，小写的字母是写不进入 内容的。

*/
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <windows.h>

#include <QtXlsx>
int main(int argc, char *argv[])
{

    //文件已被其他应用打开，比如excel，文件就写不进入了，但是可以读取
    QFile _xlsxFile("Test.xlsx");
    if(_xlsxFile.exists())
    {
        qDebug()<<"file isor not open:"<<_xlsxFile.isOpen();
        if(_xlsxFile.isOpen())
        {
            qDebug()<<"file is open";
            _xlsxFile.close();
        }
        if(!_xlsxFile.open(QIODevice::ReadWrite))
        {
            qDebug()<<"cannot readWrite";

            return 0;
        }
    }





    //写excel文件
    QXlsx::Document xlsx;
    xlsx.write("A1", "Hello Qt!");
    xlsx.write("A1", "Hello A1!");
    xlsx.write("A2", "Hello A2!");
    xlsx.write("B1", "Hello Qtb1!");


    //超链接 Hyperlinks
    xlsx.write("D1", "http://qt-project.org");
    xlsx.write("D2", "file:///C:/A.txt");
    xlsx.write("C5", "Hello Qtc1!");


    //![1] richText 丰富的文本格式
    QXlsx::Format blue;
    blue.setFontColor(Qt::blue);
    QXlsx::Format red;
    red.setFontColor(Qt::red);
    red.setFontSize(15);
    QXlsx::Format bold;
    bold.setFontBold(true);

    QXlsx::RichString rich;
    rich.addFragment("Hello ", blue);
    rich.addFragment("Qt ", red);
    rich.addFragment("Xlsx", bold);
    xlsx.write("B2", rich);

    xlsx.workbook()->setHtmlToRichStringEnabled(true);
    xlsx.write("B4", "<b>Hello</b> <font color=\"red\">Qt</font> <i>Xlsx</i>");

    xlsx.write("B6", "<font color=\"red\"><b><u><i>Qt Xlsx</i></u></b></font>");

    //![1]

    //![2]数字格式化输出
    QXlsx::Format header;
    header.setFontBold(true);
    header.setFontSize(20);

    //Custom number formats
    QStringList numFormats;
    numFormats<<"Qt #"
             <<"yyyy-mmm-dd"
            <<"$ #,##0.00"
           <<"[red]0.00";

    //Builtin number formats
    xlsx.addSheet("numberFormat");
    xlsx.setColumnWidth(1, 4, 20.0);
    xlsx.write(11, 11, "Raw data", header);
    xlsx.write(11, 12, "Builtin Format", header);
    xlsx.write(11, 13, "Shown value", header);
    for (int i=10; i<50; ++i) {
        int row = i+2;
        int numFmt = i-10;
        xlsx.write(row, 11, 100.0);
        xlsx.write(row, 12, numFmt);
        QXlsx::Format format;
        format.setNumberFormatIndex(numFmt);
        xlsx.write(row, 13, 100.0, format);
    }

    //![2]


    xlsx.saveAs("Test.xlsx");

    //读excel,并修改，另存为，支持中文字符串
    QXlsx::Document xlsxReader("Test.xlsx");
    qDebug()<<xlsxReader.read("A1").toString()<<xlsxReader.read(1,1).toString()\
           <<xlsxReader.read(1,2).toString();//直接输出不存在的单元格，显示为空
    xlsxReader.write("A1","modify A1 你好");
    xlsxReader.save();
    xlsxReader.saveAs("TestWrite.xlsx");



    qDebug()<<"输出excel填充数据的范围："<<xlsxReader.dimension().rowCount()<<xlsxReader.dimension().columnCount();
    //read 这样更好，空的celL（单元格）就不输出了

    for (int row=1; row<=xlsxReader.dimension().rowCount(); ++row)
    {
        //if(QXlsx::Cell *cell=xlsxReader.cellAt(row, 1)) ==
        for(int colum=1;colum<=xlsxReader.dimension().columnCount();++colum)
        {
            QXlsx::Cell *cell=xlsxReader.cellAt(row, colum);
            if (cell)
            {

                qDebug()<<cell->value()<<cell->cellType()<<cell->dateTime();
            }
            else
            {
                qDebug()<<"cell is null";
            }
        }

    }


    qDebug()<<"表单名："<<xlsxReader.sheetNames()<<xlsxReader.renameSheet("Sheet1","table1");
    qDebug()<<"modify sheetsName:"<<xlsxReader.sheetNames();

    //另一个文件
    //设置excel文本的行间距 大小和字体
    QXlsx::Document _xlsxDocument("Merger_Font.xlsx");
    QXlsx::Format _format;
    _format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    _format.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    _xlsxDocument.write("A1","Caption");//写入数据到单元格
    _xlsxDocument.mergeCells("A1:K2",_format);//合并单元格

    //设置行 height
    _xlsxDocument.setRowHeight(1, 50.0);

    //设置列width
    _xlsxDocument.setColumnWidth(3, 3, 40.0);

    //设置format's的字体属性
    QXlsx::Format format1;
    format1.setFontName("幼圆");
    format1.setFontBold(true);//加粗
    format1.setFontItalic(true);//斜体
    format1.setFontColor(QColor(Qt::blue));//字体颜色
    format1.setFontSize(20);//字体大小
    format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);//下划线
    format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    format1.setTextWarp(true);
    format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);//边框式样
    format1.setPatternBackgroundColor(QColor(Qt::green));//背景色

    _xlsxDocument.write(3,1,"吴乾坤row style formataassfdfdfdggdsafasfas ");
    _xlsxDocument.write(3,2,"color");
    _xlsxDocument.setRowFormat(3,format1);
    _xlsxDocument.setRowHeight(3,3,70);
    _xlsxDocument.save();
    qDebug()<<"表单名："<<_xlsxDocument.sheetNames()<<"当前sheetName:"<<_xlsxDocument.currentSheet()->sheetName();

    //copy表格 无格式 无format
    _xlsxDocument.copySheet("Sheet1","copyFile");

    //设置当前的显示sheet,不选择就是默认第一个sheet
    _xlsxDocument.selectSheet("copyFile");
    _xlsxDocument.write(5,5,"in the copyFile");

    //修改sheets排列的顺序
    _xlsxDocument.moveSheet("copyFile",0);

    //影藏 sheet
    //_xlsxDocument.sheet("copyFile")->setVisible(false);

    //删除sheet
    //_xlsxDocument.deleteSheet("Sheet1");

    //添加sheet
    _xlsxDocument.addSheet("newAddSheet");

    //不显示 网格
    _xlsxDocument.currentWorksheet()->setGridLinesVisible(false);

    //设置xlsx文件属性
    _xlsxDocument.setDocumentProperty("title", "This is an example spreadsheet");
    _xlsxDocument.setDocumentProperty("subject", "With document properties");
    _xlsxDocument.setDocumentProperty("creator", "wqiankun");
    _xlsxDocument.setDocumentProperty("company", "LS");
    _xlsxDocument.setDocumentProperty("category", "Example about sheets");
    _xlsxDocument.setDocumentProperty("keywords", "标记");
    _xlsxDocument.setDocumentProperty("description", "Created with Qt Xlsx");
    _xlsxDocument.save();


    //![image]
    QXlsx::Document imagexlsx;
    //QImage image(40, 30, QImage::Format_RGB32);
    QImage image(":/image/dog.png");
    //image.fill(Qt::green);
    for (int i=0; i<10; ++i)
        imagexlsx.insertImage(10*i, 5, image);
    imagexlsx.saveAs("image.xlsx");
    //![image]

    // chart图表   conditionalFormating条件格式    DataValidation数字过滤（数据有效性）  defineName函数
    //formulas 公式
    //![defineName]
    /*
    Document xlsx;
    for (int i=1; i<=10; ++i) {
        xlsx.write(i, 1, i);
        xlsx.write(i, 2, i*i);
        xlsx.write(i, 3, i*i*i);
    }

    xlsx.defineName("MyCol_1", "=Sheet1!$A$1:$A$10");
    xlsx.defineName("MyCol_2", "=Sheet1!$B$1:$B$10", "This is comments");
    xlsx.defineName("MyCol_3", "=Sheet1!$C$1:$C$10", "", "Sheet1");
    xlsx.defineName("Factor", "=0.5");

    xlsx.write(11, 1, "=SUM(MyCol_1)");
    xlsx.write(11, 2, "=SUM(MyCol_2)");
    xlsx.write(11, 3, "=SUM(MyCol_3)");
    xlsx.write(12, 1, "=SUM(MyCol_1)*Factor");
    xlsx.write(12, 2, "=SUM(MyCol_2)*Factor");
    xlsx.write(12, 3, "=SUM(MyCol_3)*Factor");

    xlsx.save();
    */
    //![defineName]
    return 0;

}
