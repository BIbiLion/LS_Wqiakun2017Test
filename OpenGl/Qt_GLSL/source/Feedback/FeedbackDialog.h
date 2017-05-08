/* ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 * ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 * ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 * ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 * ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 * ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 * ┃ This software is provided 'as-is', without any express or implied      ┃
 * ┃ warranty. In no event will the authors be held liable for any damages  ┃
 * ┃ arising from the use of this software.                                 ┃
 * ┃                                                                        ┃
 * ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 * ┃ including commercial applications, and to alter it and redistribute it ┃
 * ┃ freely, subject to the following restrictions:                         ┃
 * ┃                                                                        ┃
 * ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 * ┃    not claim that you wrote the original software. If you use this     ┃
 * ┃    software in a product, an acknowledgment in the product             ┃
 * ┃    documentation would be appreciated but is not required.             ┃
 * ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 * ┃    not be misrepresented as being the original software.               ┃
 * ┃ 3. This notice may not be removed or altered from any source           ┃
 * ┃    distribution.                                                       ┃
 * ┃                                                                        ┃
 * ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃ file name: FeedbackDialog.h                                            ┃
 * ┃ create date: 2013年8月24日星期六 13时25分29秒                          ┃
 * ┃ last modified date: 2013年8月23日星期五 22时22分1秒                    ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include <QDialog>
#include <QTimer>
/*---------------------------------------------------------------------------*/
namespace Ui {
class FeedbackDialog;
}

class EnginioClient;
class EnginioReply;
class FeedbackDialog;
/*---------------------------------------------------------------------------*/
class FeedbackDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FeedbackDialog( const QString& title,
                             QWidget *parent = 0);
    ~FeedbackDialog();
private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_pressed();
    void UploadFinished( void );
    void ResetButtonText( void );
    void DownloadText( EnginioReply* pReply );
private:
    void SendToBackend( void );

    QTimer              m_LocalTimer;
    Ui::FeedbackDialog *ui;
    EnginioClient*      m_pClient;
    bool                m_SendLock;
};
/*---------------------------------------------------------------------------*/
#endif // FEEDBACKDIALOG_H
