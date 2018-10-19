# IpSearch

## 文件数据
每行包含一个 IP 段跟对应的省份城市, 如：;
115.193.160.56 115.193.161.58 浙江 杭州 ;
假设文件数据经过处理，IP地址段覆盖所有的IP地址，数据已按照IP段排序，参考数据IP_DATA.txt ;

## 查询思想
采用二级索引的思想查询ip对应的地区 ;
一级索引：比较ip地址的第一段数字，缩小查询范围 ;
二次索引：精确查找，采用二分查找的方式，逼近正确值;

## 输入输出
输入：ip地址;
输出：ip对应的地区（若文件中没有IP对应的区域，则输出“null”）;
异常输入处理：若输入IP地址无效，返回空，输出“The ip address is invalid!”;
;
