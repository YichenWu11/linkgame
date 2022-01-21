#include<stdio.h>
#include <graphics.h>
#include<time.h>
#define MAP_SIZE 10 //每一行有多少张图片
#define IMG_SIZE 39 //每张图片的宽度
#define WIDTH (MAP_SIZE +2) * IMG_SIZE //窗口的宽度
#define ANIMAL_NUM 42 //图片的总个数

int map[MAP_SIZE + 2][MAP_SIZE + 2];
int way[MAP_SIZE + 2][MAP_SIZE + 2];
IMAGE img_bk;
IMAGE img_Animal[ANIMAL_NUM][2];
struct Index
{
    int row;
    int col;
}begin = { 1, -1 }, end = { -1, -1 };
enum Cur
{
    BEGIN,
    END
};
int step = 0;
enum Cur state = BEGIN;
void gameInit()
{
    //设置随机数种子
    srand((unsigned)time(NULL));
    //用不同的数据表示不同的图片，每种图片来10个，一共需要10种图片
    for (int i = 1; i <= MAP_SIZE; i++)
    {
        for (int k = 1; k <= MAP_SIZE; k++)
        {
            map[i][k] = i; //10个i
        }
    }
    //加载图片
    loadimage(&img_bk,"./ref/bk.jpg",WIDTH+80,WIDTH+50);   //后两个参数用来调整背景图片的位置
    IMAGE animal;
    loadimage(&animal, "./ref/animal.bmp"); //bmp
    //如何切割图片
    SetWorkingImage(&animal);  //设置工作区
    for (int i = 0; i < ANIMAL_NUM; i++)
    {
        for (int k = 0; k < 2; k++)
        {
            getimage(&img_Animal[i][k], k * IMG_SIZE, i * IMG_SIZE, IMG_SIZE, IMG_SIZE);
        }
    }
    SetWorkingImage();  //恢复默认工作区
    //打乱数组排列，让图片也随机排列
    for (int i = 1; i <= MAP_SIZE; i++)
    {
        for (int k = 1; k <= MAP_SIZE; k++)
        {
            //随即找到两个游戏区域的下标位置，然后交换值 随机函数 rand()
            int x = rand() % MAP_SIZE + 1;
            int y=  rand() % MAP_SIZE + 1;
            int _swap = map[i][k];
            map[i][k] = map[x][y];
            map[x][y] = _swap;
        }
    }
}

//游戏的图形绘制
void gameDraw()
{
    putimage(0, 0, &img_bk);
    for (int i = 1; i <= MAP_SIZE; i++)
    {
        for (int k = 1; k <= MAP_SIZE; k++)
        {
            if (map[i][k] != 0)
            {
                putimage(k * IMG_SIZE, i * IMG_SIZE, &img_Animal[map[i][k]][1], SRCAND);    //原图
                putimage(k * IMG_SIZE, i * IMG_SIZE, &img_Animal[map[i][k]][0], SRCPAINT);  //掩码图
            }
        }
    }
}
//获取鼠标消息，获取点击的两个位置的下标
void mouseEvent()
{
    if (MouseHit())
    {
        MOUSEMSG msg = GetMouseMsg();
        //判断鼠标点击的位置(begin,end)
        if (msg.uMsg == WM_LBUTTONDOWN && state == BEGIN)
        {
            //把鼠标的坐标转成对应的数组的下标
            begin.col = msg.x / IMG_SIZE;
            begin.row = msg.y / IMG_SIZE;
            end.col = -1;
            end.row = -1;
            state = END;  //改变状态
        }
        else if (msg.uMsg == WM_LBUTTONDOWN && state == END)
        {
            end.col = msg.x / IMG_SIZE;
            end.row = msg.y / IMG_SIZE;
            state = BEGIN; //改变状态
        }
        if (msg.uMsg == WM_LBUTTONDOWN)
        {
            printf("begin(%d,%d) end(%d,%d)\n", begin.col, begin.row, end.col, end.row);
        }
    }
}

//判断某个位置是否有动物
bool isBlock(int row, int col)
{
    if (row < 0 || col < 0 || row >= MAP_SIZE + 2 || col >= MAP_SIZE + 2)
        return 1;
    return map[row][col] + way[row][col];
}

void showMap()
{
    for (int i = 0; i < MAP_SIZE + 2; i++)
    {
        for (int k = 0; k < MAP_SIZE + 2; k++)
        {
            printf("%-2d ", map[i][k]);
        }
        printf("\n");
    }
}

bool same(int begin_x, int begin_y, int end_x, int end_y)
{
    if (begin_x < 0 || begin_y < 0 || end_x < 0 || end_y < 0 || end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2)
        return false;
    if (begin_x == end_x && begin_y == end_y)
        return true;
    return false;
}

bool can_pass(int begin_x, int begin_y, int end_x, int end_y)
{
    if (state != BEGIN || begin_x < 0 || begin_y < 0 || end_x < 0 || end_y < 0 || end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2)
        return false;
    if (begin_x == end_x && begin_y == end_y)
        return false;
    if (map[begin.row][begin.col] != map[end.row][end.col])
        return false;
    return true;
}

bool can(int begin_x, int begin_y, int end_x, int end_y)
{
    printf("step:%d (%d,%d)->(%d,%d)\n", step, begin_x, begin_y, end_x, end_y);
    if (step++ > 2000) //当找路太多了
        return false;
    if (begin_x < 0 || begin_y < 0 || end_x < 0 || end_y < 0 || end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2)
        return false;
    if (same(begin_x, begin_y, end_x, end_y) || same(begin_x - 1, begin_y, end_x, end_y) || same(begin_x + 1, begin_y, end_x, end_y) || same(begin_x, begin_y - 1, end_x, end_y) || same(begin_x, begin_y + 1, end_x, end_y))
        return true;
    if (!isBlock(begin_x - 1, begin_y)) //1
    {
        way[begin_x - 1][begin_y] = 1;
        if (can(begin_x - 1, begin_y, end_x, end_y))
            return true;
        way[begin_x - 1][begin_y] = 0;
    }
    if (!isBlock(begin_x, begin_y - 1)) //3
    {
        way[begin_x][begin_y - 1] = 1;
        if (can(begin_x, begin_y - 1, end_x, end_y))
            return true;
        way[begin_x][begin_y - 1] = 0;
    }
    if (!isBlock(begin_x + 1, begin_y)) //2
    {
        way[begin_x + 1][begin_y] = 1;
        if (can(begin_x + 1, begin_y, end_x, end_y))
            return true;
        way[begin_x + 1][begin_y] = 0;
    }
    if (!isBlock(begin_x, begin_y + 1)) //4
    {
        way[begin_x][begin_y + 1] = 1;
        if (can(begin_x, begin_y + 1, end_x, end_y))
            return true;
        way[begin_x][begin_y + 1] = 0;
    }
    return false;
}

int main()
{
    //首先需要我们自己创建一个图形窗口
    initgraph(WIDTH, WIDTH, SHOWCONSOLE);
    gameInit();
    showMap();
    while (1)
    {
        //开始双缓冲绘图
        BeginBatchDraw();
        gameDraw();
        //结束双缓冲绘图
        EndBatchDraw();
        if (can_pass(begin.row, begin.col, end.row, end.col))
        {
            memset(way, 0, sizeof(way));
            way[begin.row][begin.col] = 1;
            step = 0;
            printf("inininininin");
            if (can(begin.row, begin.col, end.row, end.col))
            {
                map[begin.row][begin.col] = 0;
                map[end.row][end.col] = 0;
                begin.row = -1;
                begin.col = -1;
                end.row = -1;
                end.col = -1;
            }
        }
        mouseEvent();
    }
    getchar();
    return 0;
}