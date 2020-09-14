// AliyunOSSDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "AliyunOSS.h"

//#ifdef _DEBUG
//#pragma comment(lib, "..\\debug\\AliyunOSS.lib")
//#else
//#pragma comment(lib, "..\\Release\\AliyunOSS.lib")
//#endif

int main()
{
	/* 初始化OSS账号信息 */
	std::string AccessKeyId = "你的用户AccessKeyId";
	std::string AccessKeySecret = "你的用户AccessKeySecret";
	std::string Endpoint = "oss-cn-shenzhen.aliyuncs.com";
	std::string BucketName = "greless-temp";


	/*设置下载文件的文件名*/
	std::string ObjectName = "1.png";
	std::string FileNametoSave = "c:\\1.png";

	AliyunOSS _AliyunOSS(Endpoint, AccessKeyId, AccessKeySecret, BucketName);
	_AliyunOSS.PutObjectFile(ObjectName, FileNametoSave);

	return 0;
}

 