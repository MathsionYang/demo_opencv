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
	//FileStorage fs("test.xml",FileStorage::WRITE);//写操作
	//FileStorage fs("test.txt", FileStorage::WRITE);
	FileStorage fs("test.yml", FileStorage::WRITE);
	//测试数据
	int a1 = 4;
	char a2 = 9;
	string str = "Hello,world";
	int arr[10] = { 10,9,8,7,6,5,4,3,2,1 };
	test_t t = {3,4,"Hello"};
	map<string, int> m;
	m["Tom"] = 150;
	m["jack"] = 120;
	
	//写入文件操作,先写标注
	fs << "int_data" << a1;
	fs << "char_data" << a2;
	fs << "string_data" << str;
	//写入数组
	fs << "array_data" << "[";//数组开始
		for (int i = 0; i < 10; i++)
		{
			fs << arr[i];
		}
	fs << "]";//数组结束
	 //写入结构体
	fs << "struct_data" << "{"; //结构体开始
	fs << "x" << t.x;
	fs << "y" << t.y;
	fs << "s" << t.s;
	fs << "}";  //结构结束
	//map的写入
	fs << "map_data" << "{";//map的开始写入
	map<string, int>::iterator it = m.begin();
	for (; it != m.end(); it++)
	{
		fs << it->first << it->second;
	}
	fs << "}";  //map写入结束
	return 0;
}