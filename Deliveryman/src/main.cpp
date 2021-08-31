#include<iostream>
using namespace std;
//��ͼ����
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
	Object* state = new Object[gWidth*gHeight]; //ʹ����new���ʹ��delete������
	init(state,gWidth,gHeight,gMapData); //��ʼ��,��char�ַ�����ת����ö������һ���߼��������޸�
	while(true) //��ѭ��
	{
		draw(state,gWidth,gHeight);
		if(checkclear(state,gWidth,gHeight))//ͨ�ؼ��
			break; 
		cout << "a:left,d:right,w:up,s:down,conmand?" << endl;
		char input;
		cin >> input;
		update(input,state,gWidth,gHeight);
	}
	cout << "��--------Congratulation`s! you won.---------��" << endl;
	delete[] state;
	state = 0;
	//һ��Ҫ��ʱʹ��delete
}

void init(Object* state, int w, int h, const char* gMapData)
{
	int x = 0;
	int y = 0;
	while (*gMapData!=NULL)
	{
		switch (*gMapData)  //ע�����ָ���±꣬������©��ͷԪ�أ�
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


bool checkclear(const Object* state,int w,int h)//ͨ�������жϣ�û��block����
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
			cout << font[o];//ʹ��ö��ֵ��Ϊ�±�
		}
		cout << endl;
	}
}

void update(char input,Object* s,int w,int h) //���º���
{
	int player_x, player_y;
	for (int i = 0; i < w*h; i++)
	{
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
		{
			player_x = i % w;
			player_y = i / w;                     //֪��������������x��y����ֵ
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

	//�ж��Ƿ�Խ�磬Խ�������κδ���ֱ�ӷ���
	if (tx<0 || ty<0 || tx>=w || ty>=h) //ע��߽�ֵ
	{
		return;
	}

	int p = player_x + player_y * w;
	int tp = tx + ty * w;

	//�����ƶ��Ƿ�Ϸ�
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)//�ƶ����յػ���Ŀ���
	{
		s[tp] = (s[tp] == OBJ_SPACE) ? OBJ_MAN : OBJ_MAN_ON_GOAL;
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)//������
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2<0 || tx2>w || ty2>h)//Խ����
		{
			return;
		}
		int tp2 = tx2 + ty2 * w;
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)//���ӵ��ƶ�Ŀ�ĵ�Ϊ�յػ���Ŀ��
		{
			s[tp2] = (s[tp2] == OBJ_SPACE) ? OBJ_BLOCK : OBJ_BLOCK_ON_GOAL;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
