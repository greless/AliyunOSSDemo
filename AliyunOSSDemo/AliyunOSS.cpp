#include "pch.h"
#include "AliyunOSS.h"
 
#include<iostream>
#include<fstream>


void  ProgressCallback(size_t increment, int64_t transfered, int64_t total, void* userData)
{
	std::cout << "ProgressCallback[" << userData << "] => " <<
		increment << " ," << transfered << "," << total << std::endl;
}

AliyunOSS::AliyunOSS(const std::string& endpoint, const std::string& accessKeyId, 
	const std::string& accessKeySecret,const std::string& backetname)
	:m_client(endpoint, accessKeyId, accessKeySecret,m_conf)
{
	/* ��ʼ��OSS�˺���Ϣ */
	//m_AccessKeyId = accessKeyId;
	//m_AccessKeySecret = accessKeySecret;
  //  m_Endpoint = endpoint;
	m_bucketName = backetname;

	/* ��ʼ���������Դ */
	 InitializeSdk();

	//ClientConfiguration conf;
	//m_client=OssClient(m_Endpoint, m_AccessKeyId, m_AccessKeySecret, conf);
 
}

AliyunOSS::~AliyunOSS()
{
	/* �ͷ��������Դ */
	ShutdownSdk();
}

 

int64_t AliyunOSS::GetObjectLenght(std::string strObjectName_)
{
	/* ��ȡ�ļ��Ĳ���Ԫ��Ϣ */
	auto outcome = m_client.GetObjectMeta(m_bucketName, strObjectName_);

	if (!outcome.isSuccess()) {
		/* �쳣���� */
		std::cout << "GetObjectMeta fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return -1;
	}
	else {
		auto metadata = outcome.result();
		std::cout << " get metadata success, ETag:" << metadata.ETag() << "; LastModified:"
			<< metadata.LastModified() << "; Size:" << metadata.ContentLength() << std::endl;

		return metadata.ContentLength();
	}

	return int64_t();
}

bool AliyunOSS::GetListObject(std::string strPrefix_, std::vector<std::string>& vecObject_)
{
	vecObject_.clear();

	std::string nextMarker = "";
	bool isTruncated = false;
	do {
		/* �о��ļ� */
		ListObjectsRequest request(m_bucketName);
		/* ָ��ǰ׺ */
		request.setPrefix(strPrefix_);
		request.setMarker(nextMarker);
		auto outcome = m_client.ListObjects(request);

		if (!outcome.isSuccess()) {
			/* �쳣���� */
			std::cout << "ListObjects fail" <<
				",code:" << outcome.error().Code() <<
				",message:" << outcome.error().Message() <<
				",requestId:" << outcome.error().RequestId() << std::endl;

			return false;
			break;
		}
		for (const auto& object : outcome.result().ObjectSummarys()) {
			std::cout << "object" <<
				",name:" << object.Key() <<
				",size:" << object.Size() <<
				",lastmodify time:" << object.LastModified() << std::endl;

			vecObject_.push_back(object.Key());
		}
		nextMarker = outcome.result().NextMarker();
		isTruncated = outcome.result().IsTruncated();
	} while (isTruncated);

	return true;
}

bool AliyunOSS::IsObjectExist(std::string strObjectName_)
{
	/* �ж��ļ��Ƿ���� */
	auto outcome = m_client.DoesObjectExist(m_bucketName, strObjectName_);

	if (outcome) {
		std::cout << "The Object exists!" << std::endl;
		return true;
	}
	else {
		std::cout << "The Object does not exist!" << std::endl;
		return false;
	}
	return false;
}

bool AliyunOSS::DeleteObject(std::string strObjectName_)
{
	 
	DeleteObjectRequest request(m_bucketName, strObjectName_);

	/* ɾ���ļ� */
	auto outcome = m_client.DeleteObject(request);

	if (!outcome.isSuccess()) {
		/* �쳣���� */
		std::cout << "DeleteObject fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return false;
	}
	return true;
}
bool AliyunOSS::GetObjectToFile(std::string strObjectName_, std::string strSaveName_)
{
	//auto outcome = m_client.GetObject(m_bucketName, strObjectName_, strSaveName_);

	/*��ȡ�ļ��������ļ�*/
	GetObjectRequest request(m_bucketName, strObjectName_);
	 request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(strSaveName_, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
	
	// TransferProgress progressCallback = { ProgressCallback , nullptr };
	 //request.setTransferProgress(progressCallback);

	 auto outcome = m_client.GetObject(request);

	if (outcome.isSuccess()) {
		std::cout << "GetObjectToFile success" << outcome.result().Metadata().ContentLength() << std::endl;
	}
	else {
		/*�쳣����*/
		std::cout << "GetObjectToFile fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return false;
	}
	return true;
}

bool AliyunOSS::PutObjectFile(std::string strObjectName_, std::string strFilePath_)
{
 
	//auto outcome = m_client.PutObject(m_bucketName, strObjectName_, strFilePath_); 
	
	/* �ϴ��ļ� */
	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(strFilePath_, std::ios::in | std::ios::binary);
	PutObjectRequest request(m_bucketName, strObjectName_,  content);

//	TransferProgress progressCallback = { ProgressCallback , nullptr };
//	request.setTransferProgress(progressCallback);

	auto outcome = m_client.PutObject(request);
	 
	if (!outcome.isSuccess()) {
		/* �쳣���� */
		std::cout << "PutObject fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return false;
	}

	return true;
}
