#include <QCoreApplication>
#include <QDebug>

class TwoWayLinkedList
{
public:
    struct Node
    {
        struct Node * lastNode;
        char name;
        struct Node * nextNode;
    }*head,*tail;//头 尾

    TwoWayLinkedList()
    {
        head=tail=NULL;
    }


    void vistNode()
    {
        qDebug()<<"vistNode diaplay The NodeName:";
        qDebug()<<"from head to tail---";
        Node * _curNode=head;
        while(_curNode)
        {
            qDebug()<<_curNode->name;
            _curNode=_curNode->nextNode;
        }
        /*
        qDebug()<<"\n\nfrom Tail to head---";
        _curNode=tail;
        while(_curNode)
        {
            qDebug()<<_curNode->name;
            _curNode=_curNode->lastNode;
        }
       */
        return;
    }

    void createLinkList(int lengthNode)
    {

        head=tail;

        int i=0;
        Node * _node=NULL;
        Node * _tmpNode=NULL;
        while(i< lengthNode)
        {

            _node=(struct Node *)malloc(sizeof(struct Node));
            _node->lastNode=NULL;
            _node->name='A'+i;
            _node->nextNode=NULL;
            if(0==i)
            {
                head=tail=_tmpNode=_node;
            }
            else
            {
                //添加到队尾
                {
                    _tmpNode->nextNode=_node;
                    _node->lastNode=_tmpNode;

                    _tmpNode=_node;
                }
                /*
                //添加到队首
                {
                    _node->nextNode=_tmpNode;
                    _tmpNode->lastNode=_node;
                    head=_node;

                    _tmpNode=_node;
                }
                 */
            }

            ++i;
        }
        tail=_tmpNode;//添加到队尾
    }

    void addNode(int _posNode)//插入位置
    {
        qDebug()<<"addNode------";
        Node * _curNode= head;
        int i=1;
        Node*  _node=NULL;
        _node=(struct Node *)malloc(sizeof(struct Node));
        _node->lastNode=NULL;
        _node->name='K';
        _node->nextNode=NULL;
        if(_posNode<1)//插入到头部
        {
            _node->nextNode=head;
            head->lastNode=_node;

            head=_node;
            qDebug()<<"add node head";
            vistNode();
            return;
        }

        while(i<_posNode)
        {
            //qDebug()<<_curNode->name;
            ++i;
            _curNode=_curNode->nextNode;
        }

        if(_curNode)//不是队尾
        {
            /*
             _node=(struct Node *)malloc(sizeof(struct Node));
            _node->lastNode=NULL;
            _node->name='K';
            _node->nextNode=NULL;
            */

            //插入方法
            //_curNode->nextNode;//插入后的节点

            _node->nextNode=_curNode->nextNode;
            _curNode->nextNode->lastNode=_node;

            _node->lastNode=_curNode;
            _curNode->nextNode=_node;



        }
        else//队尾
        {
            tail->nextNode=_node;
            _node->lastNode=tail;
            tail=_node;
            qDebug()<<"insert tail";
        }
        vistNode();

    }

    void delNode(int _posNode)
    {
        //删除头节点
        if(_posNode<1)
        {
            qDebug()<<"del node pos head";
            head=head->nextNode;
            vistNode();
            return;
        }

        //删除中间节点
        int i=1;
        Node * _delNode=head;
        while(i<_posNode)
        {

            _delNode=_delNode->nextNode;

            ++i;

        }
        if(_delNode)
        {
            qDebug()<<"del node pos middle";
          _delNode->nextNode=_delNode->nextNode->nextNode;
          _delNode->nextNode->lastNode=_delNode;
          vistNode();
        }
        else
        {
            //删除尾节点
            //if(_posNode>10)
            {  qDebug()<<"del node pos tail";
                tail=tail->lastNode;
                tail->nextNode=NULL;
                vistNode();
            }
        }
    }

};



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TwoWayLinkedList * twoLinkeWay= new TwoWayLinkedList();
    twoLinkeWay->createLinkList(10);
    twoLinkeWay->vistNode();
    //twoLinkeWay->addNode(3);
    //twoLinkeWay->addNode(0);
    twoLinkeWay->addNode(11);
    // twoLinkeWay->delNode(0);
    //twoLinkeWay->delNode(11);
    twoLinkeWay->delNode(5);

    return a.exec();
}
