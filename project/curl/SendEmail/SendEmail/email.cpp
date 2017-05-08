
#include "email.h"
#include "third_party/mimetic/mimetic.h"
#include "utils/utils.h"

struct UserData  
{  
	std::stringstream ss;  
	size_t total;  
	UserData() : total(0),
		         ss()
	{}
};

Email::Email(QObject *parent)
	: QThread(parent)
{
	m_receiverList.clear();
	m_attachsList.clear();
}

Email::~Email()
{

}

void Email::run()
{
	mimetic::MultipartMixed head;  
	head.header().from(utils::QStringToUtf8String(m_userName));  
	head.header().subject(utils::QStringToUtf8String(m_subject)); 
	head.header().push_back(mimetic::Field("Mime-Version","1.0"));
	struct curl_slist *slist = NULL;
	for (int i = 0; i < m_receiverList.size(); ++i)
	{
		slist = curl_slist_append(slist, utils::QStringToUtf8String(m_receiverList.at(i)).c_str());
		head.header().to(utils::QStringToUtf8String(m_receiverList.at(i)).c_str()); 
	}

	//添加邮件内容
	mimetic::MimeEntity* pMeContent = new mimetic::MimeEntity;
	pMeContent->body().assign(utils::QStringToUtf8String(m_content + tr("\nContact Info:") + m_contact));
	head.body().parts().push_back(pMeContent);

	//如果有附件添加附件
	for (int i = 0; i < m_attachsList.size(); ++i)
	{
		mimetic::MimeEntity* pMe = new mimetic::MimeEntity;
		pMe->header().push_back(mimetic::Field("Content-Transfer-Encoding","base64"));
		FILE *pfile = fopen(utils::QStringToUtf8String(m_attachsList.at(i)).c_str(), "rb");  
		char buffer[4096];  
		uint32_t totalreadbytes = 0;  
		while (!feof(pfile))  
		{  
			uint32_t readbytes = fread(buffer, 1, 4096, pfile);  
			if (ferror(pfile) || readbytes == 0)
				break;  

			totalreadbytes += readbytes;  
			mimetic::Base64::Encoder b64;  
			std::stringstream temp;  
			std::ostreambuf_iterator<char> out(temp);  
			//转为BASE64编码，目标存放至std::stringstream中  
			mimetic::code(buffer,  buffer + readbytes, b64, out);  
			std::string str = temp.str();  
			std::cout<<str;
			pMe->load(str.begin(), str.end(), mimetic::imNone);  
		}
		fclose(pfile);
		QString fileName = utils::PathFindFileName(m_attachsList.at(i));
		pMe->header().push_back(mimetic::Field(
			utils::QStringToUtf8String("Content-Type: application/octet-stream; name=" + 
			fileName)));
		pMe->header().push_back(mimetic::Field(utils::QStringToUtf8String("Content-Disposition : attachment; filename=" +
			fileName)));
		head.body().parts().push_back(pMe);
	}

	struct UserData ud;
	ud.ss<<head;
	ud.ss.seekg(0, std::ios::end);  
	ud.total = ud.ss.tellg();  
	ud.ss.seekg(0, std::ios::beg);

	CURL *curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, utils::QStringToUtf8String(m_smtpServer).c_str());
		curl_easy_setopt(curl, CURLOPT_USERNAME, utils::QStringToUtf8String(m_userName).c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, utils::QStringToUtf8String(m_passWord).c_str());
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, utils::QStringToUtf8String(m_userName).c_str());    //发送者
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, slist);
		curl_easy_setopt(curl, CURLOPT_READDATA, &ud);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadData);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}

	m_res = curl_easy_perform(curl);
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
}

void Email::SetSend(const QString& subject, const QString& content, const QString& contact)
{
	m_subject = subject;
	m_contact = contact;
	m_content = content;
}

void Email::Reset()
{
	m_userName.clear();
	m_passWord.clear();
	m_smtpServer.clear();
	m_receiverList.clear();
	m_attachsList.clear();
}

void Email::SetUserName(const QString& name)
{
	m_userName = name;
}

void Email::SetPassword(const QString& password)
{
	m_passWord = password;
}

void Email::SetUserInfo(const QString& name, const QString& password)
{
	m_userName = name;
	m_passWord = password;
}

int Email::AddReceiver(const QString& receiver)
{
	m_receiverList.append(receiver);
	return m_receiverList.size();
}

int Email::AddAttach(const QString& attachPath)
{
	m_attachsList.append(attachPath);
	return m_attachsList.size();
}

void Email::RemoveAttach(int index)
{
	m_attachsList.removeAt(index);
}

int Email::ReadData(void* ptr, size_t size, size_t nmemb, void* userp)
{
	struct UserData * pstream = static_cast<struct UserData *>(userp);  
	if (pstream->ss.eof())  
		return 0;  

	size_t before = pstream->ss.tellg();  
	pstream->ss.read((char*)ptr, size*nmemb);  
	size_t after = pstream->ss.tellg();  
	if (pstream->ss.eof())  
		return pstream->total - before;  
	return after - before;  
}