#include<algorithm>
#include<string>
#include<vector>
#include<numeric>
#include<time.h> 
using namespace std;

bool mycomp(string a, string b) 
{ 
    if (a.size() < b.size())
    {
        return true;
    }
    if (a.size() > b.size())
    {
        return false;
    }
    if (a.size() == b.size())
    {
        return a < b;
    }
}



class Client{
public:
    vector<string> sortedkey;
    vector<int> count;
    int insert_table(string pt);
    int search_left(string pt);
    int search_right(string pt);
};


int Client::insert_table(string pt)
{
	int pos=0;
	vector<string>::iterator insert_pos;
	insert_pos = lower_bound(sortedkey.begin(), sortedkey.end(), pt,mycomp);//第一个大于等于pt的值的位置
	int temp = insert_pos - sortedkey.begin();//可能为0
	if (sortedkey.end() == insert_pos)//插入的是目前最大的
	{
		sortedkey.insert(insert_pos, pt);
		count.insert(count.begin()+temp, 1);
	}
	else if (sortedkey[temp] != pt)//没有一样值的，是新数据
	{
		sortedkey.insert(insert_pos, pt);
		count.insert(count.begin() + temp, 1);
	}
	else
	{
		count[temp] = count[temp] + 1;//重复值,不插入sortkey
	}
	srand(time(nullptr));
	pos = accumulate(count.begin(), count.begin() + temp, 0) + rand() % count[temp];
	return pos;
}

int Client::search_left(string pt)
{
	vector<string>::iterator lse_pos;
	lse_pos = lower_bound(sortedkey.begin(), sortedkey.end(), pt,mycomp);//第一个大于等于pt位置的值的
	int left_pos = 0;
	if (lse_pos == sortedkey.end())
	{
		left_pos = accumulate(count.begin(), count.end(), 0);
	}
	else
	{
		int temp = lse_pos - sortedkey.begin();
		left_pos = accumulate(count.begin(), count.begin() + temp + 1, 0);
	}
	left_pos=left_pos-1;
	return left_pos;
}

int Client::search_right(string pt)
{
	vector<string>::iterator rse_pos;
	rse_pos = lower_bound(sortedkey.begin(), sortedkey.end(), pt,mycomp);//第一个大于等于pt位置的值的
	int right_pos=0;
	if(rse_pos==sortedkey.end())
	{
		right_pos=accumulate(count.begin(),count.end(),0);
	}
	else
	{
		int temp=rse_pos-sortedkey.begin();
		right_pos=accumulate(count.begin(),count.begin()+temp+1,0);
	}
	right_pos=right_pos-1;
	return right_pos;
}