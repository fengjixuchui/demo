#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
#include <string>
#include <memory>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#if 1




#include "zlib.h"

typedef unsigned char byte;
byte* textFileRead(char* filename, long* size)
{
	byte* text;
	FILE *pf = fopen(filename, "rb");
	fseek(pf, 0, SEEK_END);
	long lSize = ftell(pf);
	*size = lSize;
	// 用完后需要将内存free掉
	text = (byte*)calloc(lSize + 1, sizeof(byte));
	rewind(pf);
	fread(text, 1, lSize, pf);
	text[lSize] = '\0';
	return text;
}

int Zip(void *out,unsigned int *outLen ,void * in,unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, -1);//Initializes the internal stream state for compression. 
	if (ret != Z_OK)
		return ret;

	strm.avail_in = inLen;
	strm.next_in = (Bytef*)in;
	do {
		strm.avail_out = *outLen;
		strm.next_out = (Bytef*)out;
		ret = deflate(&strm, Z_FINISH);    /* no bad return value */// Z_FINISH
		assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
		uInt have = allOutLen - strm.avail_out;
		*outLen = have;
	} while (false);
	(void)deflateEnd(&strm);
	return 0;
}

int unZip(void *out, unsigned int *outLen, void * in, unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = inLen;

		if (strm.avail_in == 0)
			break;
		strm.next_in = (Bytef*)in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.next_out = (Bytef*)out;
			strm.avail_out = *outLen;
			
			ret = inflate(&strm, 0);//Z_FINISH
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			uInt have = allOutLen - strm.avail_out;
			*outLen = have;
		} while (false);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


int unZip2(void *out, unsigned int *outLen, void * in, unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit2(&strm, -MAX_WBITS);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = inLen;

		if (strm.avail_in == 0)
			break;
		strm.next_in = (Bytef*)in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.next_out = (Bytef*)out;
			strm.avail_out = *outLen;

			ret = inflate(&strm, 0);//Z_FINISH
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			uInt have = allOutLen - strm.avail_out;
			*outLen = have;
		} while (false);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


void getmem(byte ** temp, unsigned int size) {
	*temp = (byte *)calloc(size, sizeof(byte));
}

#include "ZlibDecompressionStream.h"
int main() {
	std::ifstream  in("d:\\CET_Archive.dat", ios::in | ios::binary);

	int filecount = 0, filenumber = 0;
	int hasRead = in.read((char*)&filecount, sizeof(filecount)).gcount();

	

	CZlibDecompressionStream z(in,true);
	
	while (filenumber++ < filecount) {
		Bytef * temp = 0;
		unsigned int size;
		char fileName[_MAX_PATH] = {0};
		char folderName[_MAX_PATH] = { 0 };
		//get the filename
		
		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(fileName, temp, size);
		printf("filename : %s\n", temp);
		free(temp);

		//get the folder
		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(folderName, temp, size);
		printf("folder : %s\n", temp);
		free(temp);
		//fileSize

		z.Read((Bytef*)&size, sizeof(size));
		{
			fstream f1;
			Bytef *buf = new Bytef[size];
			f1.open(fileName, ios::out | ios::binary);
			z.Read(buf, size);
			f1.write((char*)buf, size);
			f1.close();
			free(buf);
		}
		if (strcmp(fileName, "CET_TRAINER.CETRAINER") == 0) {
			char * decFileName = "decCt_001.tmp";
			char * decFileName2 = "decCt_out.txt";
			int err;
			long filesize = 0;
			//byte * m = textFileRead("d:\\CET_TRAINER.CETRAINER", &filesize);
			char * m = (char*)textFileRead(fileName, &filesize);

			if (_strnicmp("<?XML", (char*)m ,5) == 0) {
				printf("find xml,break \n");
				break;
			}
			
			if (m == NULL || filesize <= 0) {
				printf("textFileRead failed" );
				break;
			}
			for (int i = 2; i <= filesize - 1; i++) {
				m[i] ^= m[i - 2];
			}
			for (int i = filesize - 2; i >= 0; i--) {
				m[i] ^= m[i + 1];
			}
			byte k = 0xca;
			for (int i = 0; i <= filesize - 1; i++) {
				m[i] ^= k;
				k = k + 5;
			}
			byte *p = (byte*)m;

			std::fstream f1(decFileName, ios::out | ios::binary |ios::trunc);
			f1.write((char*)m, filesize);
			f1.close();
			

			

			std::ifstream  in2(decFileName, ios::in | ios::binary);
			std::fstream out(decFileName2, ios::out | ios::binary | ios::trunc);
			
			Bytef* buf;
			if (memcmp(p, "CHEAT", 5) == 0) {
				CZlibDecompressionStream z_dec(in2, true);
				char p0[5];
				f1.read(p0, 5);
				size = 0;
				if (z_dec.Read((Bytef*)&size, sizeof(size)) > 0) {
					getmem(&buf, size);
					if (z_dec.Read(buf, size) > 0) {
						out.write((char*)buf, size);
					}

				}
				
			}
			else {
				CZlibDecompressionStream z_dec(in2, true);
				size = 1024;

				getmem(&buf, size);
				int ActuallyRead = 1024;

				while (ActuallyRead > 0) {
					ActuallyRead = z_dec.Read(buf, size);
					if (ActuallyRead > 0) {
						out.write((char*)buf, size);
					}
				}
				free(buf);
			}
			out.close();
			f1.close();
			in2.close();
			
			free(m);
			
			break;
		}

	}

}
int main333() {
	int err;
	int filenumber = 0;
	long size = 0;
	byte * m = textFileRead("d:\\ARCHIVE.dat", &size);
	int filecount = *(int *)m;
	m += sizeof(int);

	byte *uncompr = new byte[size];
	uInt uncomprLen = size;
	if(0)
	{
		byte txt[] = "abcd";
		byte enc[1024] ;
		byte dec[1024] = {0};
		uInt encLen = sizeof(enc);
		uInt decLen = sizeof(dec);
		err = Zip(enc, &encLen, txt, strlen((char*)txt));
		err = unZip(dec,&decLen,enc, encLen);
		dec[decLen] = 0;
		printf("err:%d ,%s\n", err,dec);

	}

	//err = uncompress(uncompr, &uncomprLen, m, 4);
	/*
	for (int i = 1;i <= 1024*10;i++) {
		uncomprLen = 100;
		err = unZip(uncompr, &uncomprLen, m, i);
		if (err == -3) {
			continue;
		}
		printf("i :%d\n", i);
		exit(0);
	}
	*/
	err = unZip2(uncompr, &uncomprLen, m, 9);
	if (err != Z_OK)
	{
		cerr << "解压错误: " << err << '\n ';
		return 0;
	}
	
	while (filenumber++ < filecount) {
		byte *temp = 0;
		byte * folder = 0;
		byte * filename = 0;

		//get the filename
		unsigned int size = *(unsigned int *)m;
		m += sizeof(unsigned int);
		getmem(&temp, size + 1);
		memcpy(temp, m, size);
		m += size;
		filename = temp;
		free(temp);
		//get the folder
		size = *(unsigned int *)m;
		m += sizeof(unsigned int);
		getmem(&temp, size + 1);
		memcpy(temp, m, size);
		m += size;
		folder = temp;
		free(temp);
		if (folder != 0) {

		}
		//get file size
		int filesize = *(unsigned int *)m;
		m += sizeof(int);
		FILE *f = fopen((char*)filename, "wb");
		fwrite(m, filesize, 1, f);
		fclose(f);
	}
	return 0;
}

int main11() {
	int err;
	long size = 0;
	byte * m = textFileRead("d:\\CET_TRAINER.CETRAINER", &size);
	for (int i = 2; i <= size - 1; i++) {
		m[i] ^= m[i - 2];
	}
	for (int i = size - 2; i >= 0; i--) {
		m[i] ^= m[i + 1];
	}
	byte k = 0xce;
	for (int i = 0; i <= size - 1; i++) {
		m[i] ^= k;
		k++;
	}
	byte *p = m;



	if (memcmp(p, "CHEAT", 5) == 0) {
		m += 5;
		uLong comprLen = *(int*)m;
		m += sizeof(int);
		uLong destLen = comprLen * 3;
		std::shared_ptr<byte> out_ptr = std::make_shared<byte>(destLen);
		Bytef * out = (Bytef*)out_ptr.get();

		err = uncompress((Bytef*)out, &destLen, (Bytef*)m, comprLen);
		if (err != Z_OK)
		{
			cerr << "解压错误: " << err << '\n ';
			return 1;
		}
	}
	else {
		uLong comprLen = size;
		uLong destLen = comprLen * 3;
		std::shared_ptr<byte> out_ptr = std::make_shared<byte>(destLen);
		Bytef * out = (Bytef*)out_ptr.get();
		//Z_DATA_ERROR
		err = unZip((Bytef*)out, (uInt*)&destLen, (Bytef*)m, comprLen);
		//err = uncompress((Bytef*)out, &destLen, (Bytef*)m, comprLen);
		if (err != Z_OK)
		{
			cerr << "解压错误: " << err << '\n ';
			return 1;
		}
	}
}

int case002() {
	int err;
	Byte compr[200], uncompr[200]; //足够大
	uLong comprLen = 0, uncomprLen = 0;
	const char* hello = "1234567890"; //原文

	uLong len = strlen(hello);// + 1;
	comprLen = sizeof(compr) / sizeof(compr[0]);

	err = compress(compr, &comprLen, (const Bytef*)hello, len);

	if (err != Z_OK)
	{
		cerr << "压缩错误: " << err << '\n ';
	}

	cout << "原文长度: " << len << "压缩后长度:" << comprLen << std::endl;
	printf("原文长度: %d: 压缩后长度: %ld\n", len, comprLen);
	strcpy((char*)uncompr, "garbage ");
	uncomprLen = sizeof(uncompr) / sizeof(uncompr[0]);
	err = uncompress(uncompr, &uncomprLen, compr, comprLen);

	if (err != Z_OK)
	{
		cerr << "解压错误: " << err << '\n ';
	}

	printf("原文长度: %d: 解压后长度: %ld\n" , len,uncomprLen );

	cout << "原文长度:" << len << "解压后长度:" << uncomprLen << endl;

	if (strncmp((char*)uncompr, hello,len))
	{
		cerr << "\n解压失败!!!\n ";
	}
	else
	{
		cout << "\n解压成功:" << (char*)uncompr;
	}
	/*
	FILE *f = fopen("d:\\b1.bin", "wb");
	fwrite(compr, comprLen, 1, f);
	fclose(f);
	*/
	getchar();
	return 0;
	return 0;
}



#endif




