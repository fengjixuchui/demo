#pragma once
#include "common.h"
#include <string>


class SM2Handler
{
private:
	// SM2��Կ64�ֽڣ�˽Կ32�ֽ�
	// SM2Ϊ�����������ֹ����Ĺ�Կ�㷨�������ǿ��Ϊ256λ
	EC_KEY *m_pubkey;
	EC_KEY *m_prikey;

	int ECDSA_SIG_get_signdataBuf(const ECDSA_SIG *sig, unsigned char *buf, int *bufLen);

	int _encrypt(std::string & data, std::string & encrypt, int isprivate);
	int _decrypt(std::string & data, std::string & decrypt, int isprivate);
public:
	SM2Handler();
	virtual ~SM2Handler();
	int generateKey();
	
	EC_KEY * getPubKey() { return m_pubkey; }
	EC_KEY * getPriKey() { return m_prikey; }
	
	void setPubKey(EC_KEY* key) {  m_pubkey= key; }
	void setPriKey(EC_KEY* key) {  m_prikey = key; }

	static EC_KEY* pubkeyToEC_KEY(unsigned char * pubkey, int len);
	static EC_KEY* prikeyTOEC_KEY(unsigned char* prikey, int len);

	static int getPublicKeyFromEC_KEY(EC_KEY* ec_key, unsigned char* buf, int * pBufLen);
	static int getPrivateKeyFromEC_KEY(EC_KEY *ec_key, unsigned char* buf, int *pBufLen);
	

	
	int public_encrypt(std::string& data, std::string& encrypt);
	int private_decrypt(std::string& data, std::string& decrypt);

	int private_encrypt(std::string& data, std::string& encrypt);
	int public_decrypt(std::string& data, std::string& encrypt);

	int private_sign(std::string& data, std::string& signData);
	int public_verify(std::string& data, std::string& signData);

};

