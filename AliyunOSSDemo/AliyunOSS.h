#pragma once
/****************************** OSS ******************************/
/*
用法：
	 AliyunOSS _AliyunOSS(Endpoint, AccessKeyId, AccessKeySecret, BucketName);	 
	 _AliyunOSS.PutObjectFile(ObjectName, FileNametoSave);
*/
#include <alibabacloud/oss/OssClient.h>
using namespace AlibabaCloud::OSS;

#ifdef _DEBUG
#pragma comment(lib, ".\\alibabacloud-oss-cpp-sdk\\lib\\alibabacloud-oss-cpp-sdk_d.lib")
#else
#pragma comment(lib, ".\\alibabacloud-oss-cpp-sdk\\lib\\alibabacloud-oss-cpp-sdk.lib")
#endif

#pragma comment(lib, ".\\alibabacloud-oss-cpp-sdk\\third_party\\lib\\Win32\\libcurl.lib")
#pragma comment(lib, ".\\alibabacloud-oss-cpp-sdk\\third_party\\lib\\Win32\\libeay32.lib")
#pragma comment(lib, ".\\alibabacloud-oss-cpp-sdk\\third_party\\lib\\Win32\\ssleay32.lib")

// 进度条回调
void  ProgressCallback(size_t increment, int64_t transfered, int64_t total, void* userData);

class AliyunOSS 
{
public:
	 
	AliyunOSS(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret, 
		const std::string& backetname );
	~AliyunOSS();

private:
	 
	std::string m_bucketName;
	ClientConfiguration m_conf;
	 OssClient m_client;
public:
	 
	int64_t GetObjectLenght(std::string strObjectName_);// 获得文件长度
	bool GetListObject(std::string strPrefix_, std::vector<std::string>& vecObject_);// 举例指定目录下的文件和子目录
	bool IsObjectExist(std::string strObjectName_);// 判断文件是否存在
	bool DeleteObject(std::string strObjectName_);// 删除文件
	bool GetObjectToFile(std::string strObjectName_, std::string strSaveName_);// 下载文件到本地
	bool PutObjectFile(  std::string strObjectName_, std::string strFilePath_);// 上传文件
	//void GetProgress( size_t& nIncrement_,   int64_t& nTransfered_,  int64_t& nTotal_);// 获得进度
};

