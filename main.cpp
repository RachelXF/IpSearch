//
//  main.cpp
//  IpSearch
//
//  采用二级索引的思想查询ip对应的地区
//  一级索引：比较ip地址的第一段数字，缩小查询范围
//  二次索引：精确查找，采用二分查找的方式，逼近正确值
//  输入：ip地址
//  输出：ip对应的地区（null代表没有）
//
//  Created by rachelwu on 2018/10/13.
//  Copyright © 2018年 rachelwu. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

//定义三个全局变量
vector<pair<int,int>> firstMap(256);  //firstMap[i]代表ip地址第一段数字为i时，index范围[start,end]
vector<long> endIpArr;    //存放文件每行的endIp值
vector<pair<string,string>> addrArr;   //存放每行的地域信息<省份，城市>

//字符串分割，str待分割的字符串，pattern分割标志，返回vector数组为分割后的字符串数组
vector<string> split(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;   //扩展字符串以方便操作
    int size=(int)str.size();
    for(int i=0; i<size; i++)
    {
        //从下标i开始查找pattern出现的位置
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            //分割的子字符串为空，则不保存
            if(s!="")
                result.push_back(s);
            i=(int)pos+(int)pattern.size()-1;
        }
    }
    return result;
}

//ip地址的stirng转为long型，返回long值，并保存ip第一段的数字
//若输入的ip地址无效，则返回-1
long IpToLong(string ipStr,int &firstIpNum){
    //根据“.”分割ip字符串
    vector<string> ips = split(ipStr,".");
    firstIpNum = atoi(ips[0].c_str());
    long ipLong = 0;
    if(ips.size()!=4){
        return -1;
    }
    for(int i=0;i<4;i++){
        long ipNum = atoi(ips[i].c_str());
        if(ipNum<0 || ipNum>255){
            ipLong=-1;
            break;
        }
        ipLong = 256*ipLong + ipNum;
    }
    return ipLong;
}

//加载数据，遍历文件的每一行，分别赋值firstMap，endIpArr，addrArr全局变量
//输入file文件路径
void LoadData(string file){
    ifstream infile;
    infile.open(file.data());
    //断言文件是否打开，若失败，则输出错误消息，并终止程序
    assert(infile.is_open());
    string str;
    int index = 0;
    int firstIp = -1;
    //逐行读取文件内容
    while(getline(infile,str)){
        int firstIpNum;
        //" "分割一行数据，返回的数组，第一个为ip段的开始值，第二个值为ip段的结束值，剩下是地区信息
        vector<string> strLines = split(str, " ");
        endIpArr.push_back(IpToLong(strLines[1],firstIpNum));
        if(strLines.size()>3)
            addrArr.push_back(make_pair(strLines[2],strLines[3]));
        else
            addrArr.push_back(make_pair(strLines[2],""));
        
        if(firstIp==firstIpNum){
            firstMap[firstIp].second = index;
        }
        else {
            firstIp=firstIpNum;
            firstMap[firstIp].first = index;
        }
        index++;
    }
    infile.close();
}

//二分逼近，下标范围[low,high]，ipVal待查询的ip值（long型）
int BinarySearch(int low ,int high ,long ipVal){
    int res=0;
    int mid=0;
    while (low <= high)
    {
        mid = (low + high) / 2;
        long endIpNum = endIpArr[mid];
        //若ipVal小于等于endIp的值，缩小范围为[low,mid-1]
        if (endIpNum >= ipVal)
        {
            res = mid;
            if (mid == 0)
            {
                break;   //防止溢出
            }
            high = mid - 1;
        }
        //否则范围缩小为[mid+1,high]
        else
            low = mid + 1;
    }
    return res;
}

//根据ip地址查询，返回ip对应的地区
string Query(string ip){
    int firstIp;
    long ipVal = IpToLong(ip,firstIp);
    //若ipVal为-1，代表输入ip地址无效，直接返回空串
    if(ipVal==-1){
        cout<<"The ip address is invalid!"<<endl;
        return "";
    }
    
    //一级索引，根据ip地址的第一段数字，缩小查询范围[low,high]
    int low = firstMap[firstIp].first;
    int high = firstMap[firstIp].second;
    //二级索引，精确查找
    int cur = low==high ? low : BinarySearch(low, high, ipVal);
    
    return addrArr[cur].first + " " + addrArr[cur].second;
}

int main(int argc, const char * argv[]) {  
    //加载文件
    LoadData("IP_DATA.txt");
    string res = Query("196.2.1.3");
    cout<<res<<endl;
    
    return 0;
}
