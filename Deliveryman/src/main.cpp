#include<iostream>
using namespace std;
//地图资料
const char* gMapData="\
********\n\
*.o p  *\n\
*.o    *\n\
********";
const int gWidth = 8;
const int gHeight = 4;

enum Object
{
	OBJ_SPACE,
	OBJ_MAN,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN
};

bool checkclear(const Object* state,int w,int h);
void draw(const Object* s,int w,int h);
void update(char input,Object* s,int w,int h);
void init(Object* state, int w, int h, const char* gMapData);

int main()
{
	Object* state = new Object[gWidth*gHeight]; //使用完new务必使用delete！！！
	init(state,gWidth,gHeight,gMapData); //初始化,将char字符数组转换成枚举数组一边逻辑操作与修改
	while(true) //主循环
	{
		draw(state,gWidth,gHeight);
		if(checkclear(state,gWidth,gHeight))//通关检测
			break; 
		cout << "a:left,d:right,w:up,s:down,conmand?" << endl;
		char input;
		cin >> input;
		update(input,state,gWidth,gHeight);
	}
	cout << "《--------Congratulation`s! you won.---------》" << endl;
	delete[] state;
	state = 0;
	//一定要及时使用delete
}

void init(Object* state, int w, int h, const char* gMapData)
{
	int x = 0;
	int y = 0;
	while (*gMapData!=NULL)
	{
		switch (*gMapData)  //注意控制指针下标，容易遗漏尽头元素；
		{
		case'*':state[y*w + x] = OBJ_WALL, ++x; break;
		case'p':state[y*w + x] =OBJ_MAN, ++x; break;
		case' ':state[y*w + x] = OBJ_SPACE,++x; break;
		case'o':state[y*w + x] = OBJ_BLOCK, ++x; break;
		case'.':state[y*w + x] = OBJ_GOAL, ++x; break;
		case'P':state[y*w + x] = OBJ_MAN_ON_GOAL, ++x; break;
		case'O':state[y*w + x] = OBJ_BLOCK_ON_GOAL, ++x; break;
		case'\n':x = 0,++y; break;

		default:
			break;
		}
		gMapData++;
	}
}


bool checkclear(const Object* state,int w,int h)//通关条件判断，没有block存在
{
	for (int i = 0; i < w*h; i++)
	{
		if (state[i] == OBJ_BLOCK)
			return false;
	}
	return true;
}
void draw(const Object* s,int w,int h)
{
	const char font[] = { ' ','p','*','.','o','O','P' };
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			Object o = s[y*w + x];
			cout << font[o];//使用枚举值做为下标
		}
		cout << endl;
	}
}

void update(char input,Object* s,int w,int h) //更新函数
{
	int player_x, player_y;
	for (int i = 0; i < w*h; i++)
	{
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
		{
			player_x = i % w;
			player_y = i / w;                     //知道数组索引逆求x，y坐标值
			break;
		}
	}
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case'a':dx = -1; break;
	case'd':dx = 1; break;
	case'w':dy = -1; break;
	case's':dy = 1; break;
	default:cout << "error input" << endl;
		break;
	}
	int tx = player_x + dx;
	int ty = player_y + dy;

	//判断是否越界，越界则不做任何处理直接返回
	if (tx<0 || ty<0 || tx>=w || ty>=h) //注意边界值
	{
		return;
	}

	int p = player_x + player_y * w;
	int tp = tx + ty * w;

	//检验移动是否合法
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)//移动到空地或者目标点
	{
		s[tp] = (s[tp] == OBJ_SPACE) ? OBJ_MAN : OBJ_MAN_ON_GOAL;
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)//有箱子
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2<0 || tx2>w || ty2>h)//越界检查
		{
			return;
		}
		int tp2 = tx2 + ty2 * w;
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)//箱子的移动目的地为空地或者目标
		{
			s[tp2] = (s[tp2] == OBJ_SPACE) ? OBJ_BLOCK : OBJ_BLOCK_ON_GOAL;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
