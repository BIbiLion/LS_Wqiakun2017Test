#include <QCoreApplication>

#include <QDebug>
#include <QString>
#include <string.h>
struct Node
{
    char name;
    Node * next;
};
void visitDisplay(Node ** g_headNode)
{
    Node * _curNode=*g_headNode;
    while(_curNode)
    {
        qDebug()<<"diaplay:"<<(_curNode->name);
        _curNode=_curNode->next;
    }
}

void create(int nodeLength, Node ** g_headNode)
{

    Node * g_lastNode=NULL;
    int i=0;
    while(i<nodeLength)
    {
        Node * _node=NULL;
        _node=(struct Node *)malloc(sizeof(struct Node));
        if(_node==NULL)
        {
            qDebug() << "Initialization single linked list error!";
        }
        /*
        char  _name[25];//=(char *)QString("Node"+QString("%1").arg(i)).toStdString().c_str();
        char*_result= itoa(i,_name,10);
        _node->name=_result;

        _node->next=NULL;
        */
        if(0==i)
        {
            _node->name='A'+i;
            _node->next=NULL;

            *g_headNode=g_lastNode=_node;
        }
        else
        {
            _node->name='A'+i;
            _node->next=NULL;

            g_lastNode->next=_node;
            g_lastNode=_node;
        }
        ++i;
    }
    //return *g_headNode;
}

void delNode(int iNode,Node **g_headNode)
{
    Node *_tmpNode=NULL;
    Node * _beforeNode=NULL;
    int i=1;
    Node *_curNode=*g_headNode;

    //删除头节点
    if(1==iNode)
    {
        _tmpNode=(*g_headNode);
        *g_headNode=(*g_headNode)->next;
        qDebug()<<"删除节点名"<<_tmpNode->name;
        free(_tmpNode);
        _tmpNode=NULL;

        qDebug()<<"after del headNode,display List ";
        visitDisplay(g_headNode);
        return;
    }

    qDebug()<<"删除中间节点";
    /*// 方法1：
    while(_curNode)
    {
        if(i==iNode)
        {
            _tmpNode=_curNode;

            Node *_nextNode= _beforeNode->next->next;
            _beforeNode->next=_nextNode;

            qDebug()<<"删除中间节点 显示";
            visitDisplay(g_headNode);
            return;
        }
        _beforeNode=_curNode;
        _curNode=_curNode->next;
        ++i;
    }
*/
    //方法2
    Node * node,*temp,*head;
    temp = (*g_headNode);node =temp;
    while(node)
    {
        if(++i == iNode)
        {
            temp = node->next;
            if(temp == NULL)
            {
                qDebug() << "\nOut of range length!";
                return ;
            }
            node->next = temp->next;

            qDebug()<<"删除中间节点 显示";
            visitDisplay(g_headNode);
            return ;
        }//if

        node = node->next;
    }
    qDebug()<<"out of range...";//bool
}



int searchPos(char name,Node **g_headNode)
{
    int _pos=0;
    Node * _curNode=*g_headNode;
    while(_curNode)
    {
        qDebug()<<"diaplay:"<<(_curNode->name);
        if(_curNode->name==name)
        {
            return _pos;
        }

        _curNode=_curNode->next;
        ++_pos;
    }
    return(-1);
}

//insert pos node
void addNode(Node * _node,int pos,Node **g_headNodePtr)
{

    if(pos<1)
    {
        Node * headTmp=_node;
        _node->next=(*g_headNodePtr);
        g_headNodePtr=&headTmp;
        qDebug()<<"add head";
        visitDisplay(g_headNodePtr);
        return;
    }

    Node * _curNode=*g_headNodePtr;
    int i=1;
    while(_curNode)
    {
        if(++i==pos)
        {
            Node * tempNode=_curNode->next;
            if(tempNode->next ==NULL)//结尾
            {
               _curNode->next=_node;
               _node->next=NULL;
               qDebug()<<"add end";
               visitDisplay(g_headNodePtr);
            }
            else
            {
                _curNode->next=_node;
                _node->next=tempNode;
                qDebug()<<"add middle";
                visitDisplay(g_headNodePtr);
            }
        }

        _curNode=_curNode->next;
    }


}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Node * g_headNode=NULL;
    create(10,&g_headNode);
    visitDisplay(&g_headNode);
    //delNode(1,&g_headNode);
    delNode(3,&g_headNode);

    //addNode
    Node * _node= (struct Node*)malloc(sizeof(struct Node));
    if(_node==NULL)
    {
        qDebug() << "Initialization single linked list error!";
    }

    _node->name='K';
    _node->next=NULL;

    //addNode(_node,0,&g_headNode);


    //addNode(_node,5,&g_headNode);
    addNode(_node,9,&g_headNode);


    /*//CREATE node link
    Node * _head=NULL;
    Node *_lastNode=NULL;
    Node * _node= (struct Node*)malloc(sizeof(struct Node));
    if(_node==NULL)
    {
        qDebug() << "Initialization single linked list error!";
    }

    _node->name="Node_A";
    _node->next=NULL;
    _head=_lastNode=_node;

    Node * _node2= (struct Node*)malloc(sizeof(struct Node));
    if(_node2==NULL)
    {
        qDebug() << "Initialization single linked list error!";
    }
      _node2->name="Node_2";
      _lastNode->next  =_node2;
      _node2->next=NULL;
      _lastNode=_node2;



      qDebug()<<_head->name<<_head->next->name;
      qDebug()<<_node->name<<_node->next->name;
      qDebug()<<_node2->name;
      if(_node2->next==NULL)
      {
        qDebug()<<"this is last Node2.";
      }
      //释放的话
   // free(_node);
   // free(_node2);
   // _node=NULL;
   // _node2=NULL;
     */

    return a.exec();
}
