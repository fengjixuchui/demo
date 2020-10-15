#include "RSAHandler.h"

#include "../crypto/rsa/rsa_locl.h"
RSAHandler::RSAHandler()
{
	m_padding = RSA_PKCS1_PADDING;
}


RSAHandler::~RSAHandler()
{
}

int RSAHandler::setPubKey(RSA* rsa) {
	if (rsa) {
		m_pubkey = rsa;
		return 0;
	}

	return -1;
}

int RSAHandler::setPriKey(RSA* rsa) {
	if (rsa) {
		m_prikey = rsa;
		return 0;
	}
	
	return -1;
}

int RSAHandler::generateRSA() {
	RSA *rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);
	if (rsa == NULL)
	{
		printf("RSA_generate_key error!\n");
		return -1;
	}
	m_prikey = m_pubkey = rsa;
	return 0;
}

int RSAHandler::public_encrypt(std::string& data, std::string &encrypted){

	return _encrypt(data, encrypted, 0);
}
int RSAHandler::private_encrypt(std::string& data, std::string &encrypted) {

	return _encrypt(data, encrypted, 1);
}

int RSAHandler::public_decrypt(std::string& data, std::string & decrypted)
{
	return _decrypt(data, decrypted, 0);
}

int RSAHandler::private_decrypt(std::string& data, std::string & decrypted)
{
	return _decrypt(data, decrypted, 1);
}

int RSAHandler::_encrypt(std::string&  data, std::string & encrypted,int isprivate)
{
	int result = 0;
	if (m_pubkey == 0) {
		printf("m_pubkey can not be null\n");
		return -1;
	}
	RSA * rsa = m_pubkey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int rsasize = 0;//��ȡrsa->n ��С
		int result = 0;
		int pdBlock;
		
		char*  indata =(char*) data.data();
		int data_len = data.length();

		rsasize = RSA_size(rsa);
		pdBlock = rsasize - 11;

		//.net Framework���ṩ��RSA�㷨�涨��ÿ�μ��ܵ��ֽ��������ܳ�����Կ�ĳ���ֵ��ȥ11,
		//��ÿ�μ��ܵõ������ĳ��ȣ�ȴǡǡ����Կ�ĳ��ȡ�
		/*
		1.RSA_PKCS1_PADDING ���ģʽ����õ�ģʽ

		Ҫ��:
		���룺���� �� RSA Կģ��(modulus) ������11���ֽ�, Ҳ���ǡ�RSA_size(rsa) �C 11
		�����������Ĺ����������и��Ȼ�����

		�������modulusһ����

		�������Ҫ�󣬶���512bit����Կ����block length = 512/8 �C 11 = 53 �ֽ�
		*/
		int eCount;
		if (data_len % (pdBlock) == 0) {
			eCount = data_len / pdBlock;
		}
		else {
			eCount = (data_len / pdBlock) + 1;
		}

		char* encryptData = (char*)malloc(rsasize);
		if (encryptData == 0) { printf("malloc failed\n"); return -1; }
		encrypted.clear();
		result = 0;
		for (int i = 0; i < eCount; i++)//exppadding ÿ�ε���Ҫ�������ݳ���
		{
			// exppadding : ���������ݳ���
			// smldata    : ����������
			// smlencrypted : �����������
			//  rsa      : �����Կ
			// 
			int ret = 0;
			int inLen = data_len > pdBlock ? pdBlock : data_len;
			if (isprivate == 1) {
				ret = RSA_private_encrypt(inLen, (const unsigned char*)indata, (unsigned char*)encryptData, rsa, RSA_PKCS1_PADDING);
			}
			else {
				ret = RSA_public_encrypt(inLen, (const unsigned char*)indata, (unsigned char*)encryptData, rsa, RSA_PKCS1_PADDING);
			}
			
			if (ret>0)
			{
				data_len -= inLen;
				indata += inLen;
				encrypted.append(encryptData, ret);
				result += ret;
			}
			else {
				if (encryptData) { free(encryptData); }
				printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
				return -1;

			}

			
		}
		if (encryptData) { free(encryptData); }
	}

	return result;
}

int RSAHandler::_decrypt(std::string& data, std::string & decrypted,int isprivate)
{
	int result = 0;
	if (m_prikey == 0) {
		printf("m_prikey can not be null\n");
		return -1;
	}
	RSA * rsa = m_prikey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int data_len = data.length();
		char * encdata = (char*)data.data();

		int pdBlock = RSA_size(rsa);
		int dCount = data_len / pdBlock;

		decrypted.clear();
		char * decryptData = (char*)malloc(pdBlock);
		if (decryptData == 0) { printf("malloc failed\n"); return -1; }
		for (int i = 0;i < dCount;i++)
		{
			int ret;
			if (isprivate == 1) {
				ret = RSA_private_decrypt(pdBlock, (unsigned char *)encdata, (unsigned char*)decryptData, rsa, RSA_PKCS1_PADDING);
			}
			else {
				ret = RSA_public_decrypt(pdBlock, (unsigned char *)encdata, (unsigned char*)decryptData, rsa, RSA_PKCS1_PADDING);
			}
			
			
			if (ret>0)
			{
				encdata += pdBlock;
				decrypted.append(decryptData, ret);
				result += ret;
			}
			else {
				if (decryptData) { free(decryptData); }
				printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
				return -1;

			}
		}
		if (decryptData) free(decryptData);
	}

	return result;
}










int RSAHandler::private_sign(std::string& data, std::string & signData)
{
	RSA* rsa = m_prikey;
	char outret[1024] = { 0 };
	unsigned int outlen = sizeof(outret);
	//data //// ��󳤶�96
	
	int result = RSA_sign(NID_sha1, (unsigned char*)data.data(), data.length(), (unsigned char*)outret, &outlen, rsa);
	if (result != 1)
	{
		printf("sign error\n");
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		return -1;
	}
	signData.append(outret, outlen);
	return 0;
}

int RSAHandler::public_verify(std::string& data, std::string & signData)
{
	RSA* rsa = m_pubkey;
	int result = RSA_verify(NID_sha1, (unsigned char*)data.data(), data.length(), (unsigned char*)signData.data(), signData.length(), rsa);
	if (result != 1)
	{
		printf("verify error\n");
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		return -1;
	}
	return result;
}

RSA * RSAHandler::createRSA(unsigned char * key, int ispubkey)
{
	RSA *rsa = NULL;
	BIO *keybio;
	keybio = BIO_new_mem_buf(key, -1);
	if (keybio == NULL)
	{
		return 0;
	}

	rsa = RSA_new();
	if (ispubkey)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}

	if (rsa == NULL)
	{
		return 0;
	}

	return rsa;
}

