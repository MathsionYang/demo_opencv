#include<opencv2\opencv.hpp>   
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;
typedef struct
{
	int x;
	int y;
	string s;
}test_t;
int main(int argc,char **argv)
{
	//FileStorage fs("test.xml",FileStorage::WRITE);//д����
	//FileStorage fs("test.txt", FileStorage::WRITE);
	FileStorage fs("test.yml", FileStorage::WRITE);
	//��������
	int a1 = 4;
	char a2 = 9;
	string str = "Hello,world";
	int arr[10] = { 10,9,8,7,6,5,4,3,2,1 };
	test_t t = {3,4,"Hello"};
	map<string, int> m;
	m["Tom"] = 150;
	m["jack"] = 120;
	
	//д���ļ�����,��д��ע
	fs << "int_data" << a1;
	fs << "char_data" << a2;
	fs << "string_data" << str;
	//д������
	fs << "array_data" << "[";//���鿪ʼ
		for (int i = 0; i < 10; i++)
		{
			fs << arr[i];
		}
	fs << "]";//�������
	 //д��ṹ��
	fs << "struct_data" << "{"; //�ṹ�忪ʼ
	fs << "x" << t.x;
	fs << "y" << t.y;
	fs << "s" << t.s;
	fs << "}";  //�ṹ����
	//map��д��
	fs << "map_data" << "{";//map�Ŀ�ʼд��
	map<string, int>::iterator it = m.begin();
	for (; it != m.end(); it++)
	{
		fs << it->first << it->second;
	}
	fs << "}";  //mapд�����
	return 0;
}