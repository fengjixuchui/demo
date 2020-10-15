#include "main.h"

typedef struct {
	FILE * fp;
	curl_off_t totalSize;
	curl_off_t startPos;
	curl_off_t curPos;
} StoreData;

static HWND	_hDlgWnd = NULL;
static int isStop = 0;

static StoreData g_sd = { 0 };
static size_t DownloadCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	//�����ص���������׷�ӵķ�ʽд���ļ�(һ��Ҫ��a������ǰ��д������ݾͻᱻ������)
	FILE* fp = NULL;
	fopen_s(&fp, "test.bin", "ab+");
	size_t nWrite = fwrite(pBuffer, nSize, nMemByte, fp);
	fclose(fp);
	return nWrite;
}

static size_t DownloadCallback2(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	FILE *fp = ((StoreData*)pParam)->fp;
	return fwrite(pBuffer, nSize, nMemByte, fp)*nSize;
}

static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	int nPos = (int)((dlnow / dltotal) * 100);
	static time_t t1 = time(0) * 100;
	time_t now = time(0) * 100;
	if (now - t1 > 10 || nPos == 100){
		t1 = now;
		//֪ͨ�������������ؽ���
		::PostMessage(_hDlgWnd, WM_USER + 110, nPos, 0);
	}
	if (isStop){
		return CURLE_ABORTED_BY_CALLBACK;
	}
	return 0;
}

static int ProgressCallback2(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	static time_t t1 = time(0);
	if (dltotal > 0.0){

		g_sd.curPos = (curl_off_t)dlnow;
	
		time_t now = time(0);
		;
		if (now * 1000 - t1 * 1000 > 2){
			t1 = now;
			::PostMessage(_hDlgWnd, WM_USER + 112, 0, 0);
		}
		
		
		
		
	}
	if (isStop){
		return CURLE_ABORTED_BY_CALLBACK;
	}
	
	return 0;
}

/* ��httpͷ����ȡ�ļ�size*/
static size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *)stream) = len;

	return size * nmemb;
}



curl_off_t getDownloadFileSize(const char*  url)
{
	curl_off_t filesize = 0;
	double lensize=0;
	for (int iTry = 0; iTry < 3; iTry++)//����curl_easy_perform���ܻ���ż���Ե�CURLE_WRITE_ERROR��������������Ի���
	{
		CURL *handle = curl_easy_init();
		curl_easy_setopt(handle, CURLOPT_URL, url);
		//curl_easy_setopt(handle, CURLOPT_HEADER, 1);
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
		curl_easy_setopt(handle, CURLOPT_HEADERDATA, &filesize);
		curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
		CURLcode res = curl_easy_perform(handle), resGetInfo = CURLE_OK;
		if (res == CURLE_OK) {
			resGetInfo = curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lensize);
			if (CURLE_OK == resGetInfo){
				curl_easy_cleanup(handle);
				return (curl_off_t)lensize;
			}
		}
		curl_easy_cleanup(handle);
		Sleep(200);
	}
	return 0;
}

//static UINT timeId = 0;

static void CALLBACK _TimerProc(HWND hwnd, UINT uMsg, UINT idEvent,
	DWORD dwTime){
	::PostMessage(_hDlgWnd, WM_USER + 112, 0, 0);
}
// ���� �ļ�����
static DWORD WINAPI DownloadThread2(LPVOID lpParam)
{
	FILE *f;
	curl_off_t local_file_len = 0;
	long filesize = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	int c;
	int use_resume = 0;
	
	const char * localpath = "test2.bin";
	const char * url = "http://download.microsoft.com/download/E/4/1/E4173890-A24A-4936-9FC9-AF930FE3FA40/NDP461-KB3102436-x86-x64-AllOS-ENU.exe";

	CURL* curlhandle = curl_easy_init();

	
	

#ifndef _WIN32
	struct stat file_info;

	/* �õ������ļ���С */
	//if(access(localpath,F_OK) ==0)
	if (stat(localpath, &file_info) == 0)
	{
		local_file_len = file_info.st_size;
		use_resume = 1;
	}
#else
	HANDLE handle = CreateFile(localpath, GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{;
		DWORD tmp = 0;
		UINT64 ret=0;
		ret = GetFileSize(handle, &tmp);
		CloseHandle(handle);
		if (ret != 0)
		{
			ret +=  (UINT64)tmp * 4294967296ULL;
		}
		local_file_len = ret;
		use_resume = 1;
	}
	curl_off_t totalSize = getDownloadFileSize(url);
	if (totalSize == 0){
		MessageBox(NULL, "getDownloadFileSize failed!", "Error", 0);
		return 1;
	}
#endif
	//����׷�ӷ�ʽ���ļ�������ʵ���ļ��ϵ���������
	f = fopen(localpath, "ab+");
	if (f == NULL) {
		perror("fopen failed!");
		return 0;
	}

	//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, url);

	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, 5);  // �������ӳ�ʱ����λ��

	//����http ͷ��������
	//curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	//curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);

	// �����ļ�������λ�ø�libcurl
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0);


	g_sd.fp = f;
	g_sd.totalSize = totalSize;
	g_sd.startPos = local_file_len;
	g_sd.curPos = 0;

	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &g_sd);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, DownloadCallback2);


	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback2);
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSDATA, &g_sd);
	

	// �����ض����������
	curl_easy_setopt(curlhandle, CURLOPT_MAXREDIRS, 5);
	// ����301��302��ת����location
	curl_easy_setopt(curlhandle, CURLOPT_FOLLOWLOCATION, 1);

	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 0L);
	r = curl_easy_perform(curlhandle);

	fclose(f);
	
	//KillTimer(0, timeId);
	if (r == CURLE_OK){
		::PostMessage(_hDlgWnd, WM_USER + 112, 100, 0);
		return 1;
	}
	else {
		//fprintf(stderr, "%s\n", curl_easy_strerror(r));
		MessageBox(NULL, curl_easy_strerror(r), "Error", 0);
		return 0;
	}
	
}

static DWORD WINAPI DownloadThread(LPVOID lpParam)
{
	//��ʼ��curl������Ǳ����
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://download.microsoft.com/download/E/4/1/E4173890-A24A-4936-9FC9-AF930FE3FA40/NDP461-KB3102436-x86-x64-AllOS-ENU.exe");
	//���ý������ݵĻص�
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadCallback);
	//curl_easy_setopt(curl, CURLOPT_INFILESIZE, lFileSize);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	//curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	//curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	// �����ض����������
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
	// ����301��302��ת����location
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	//���ý��Ȼص�����
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	//curl_easy_getinfo(curl,  CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lFileSize);
	//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, g_hDlgWnd);
	//��ʼִ������
	CURLcode retcCode = curl_easy_perform(curl);
	//�鿴�Ƿ��г�����Ϣ
	const char* pError = curl_easy_strerror(retcCode);
	//����curl����ǰ��ĳ�ʼ��ƥ��
	::PostMessage(_hDlgWnd, WM_USER + 110, 100, 0);
	curl_easy_cleanup(curl);
	return 0;
}



static INT_PTR CALLBACK DialogProcDownload(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		CenterWindow(hWnd);
		_hDlgWnd = hWnd;
		HWND hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
		SendMessage(hProgress, PBM_SETRANGE32, (WPARAM)0, (LPARAM)100);
		//�Ի����ʼ��ʱ���������߳�
		HANDLE hThread = CreateThread(NULL, 0, DownloadThread2, 0, 0, NULL);
		CloseHandle(hThread);
		::SetWindowText(hWnd, "ʹ��CURL�����ļ�ʾ����");

		//timeId = SetTimer(NULL, 0, 500, _TimerProc);

		return TRUE;
	}
	case WM_COMMAND:
	{
		WORD  msg = HIWORD(wParam);
		WORD  id = LOWORD(wParam);
		if (id == ID_BTN_START || id == IDCANCEL)
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	}
	case WM_ERASEBKGND:
		return TRUE;
	case WM_CTLCOLORSTATIC:
		return (INT_PTR)(HBRUSH)::GetStockObject(WHITE_BRUSH);
	case WM_USER + 110:
	{//���յ����ý��ȵ���Ϣ
		HWND	hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
		HWND	hStatus = GetDlgItem(hWnd, IDC_STATUS);
		if (hProgress)
			SendMessage(hProgress, PBM_SETPOS, wParam, 0L);
		if (hStatus)
		{
			char szBuffer[100] = { 0 };
			if (wParam<100)
				sprintf(szBuffer, "���������ļ������ȣ�%d%%", wParam);
			else{
				sprintf(szBuffer, "�ļ�������ϣ�");
			}
				
			::SetWindowText(hStatus, szBuffer);
		}
		return 0;
	}
	case WM_USER + 112:
	{//���յ����ý��ȵ���Ϣ
		HWND	hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
		HWND	hStatus = GetDlgItem(hWnd, IDC_STATUS);
		int pos = (g_sd.startPos + g_sd.curPos) * 100 / g_sd.totalSize;
		if (hProgress){
			
			SendMessage(hProgress, PBM_SETPOS, pos, 0L);
		}
			
		if (hStatus)
		{
			char szBuffer[100] = { 0 };
			if (pos<100 && wParam !=100)
				sprintf(szBuffer, "���������ļ������ȣ�%d%%", pos);
			else{
				sprintf(szBuffer, "�ļ�������ϣ�");
			}
			::SetWindowText(hStatus, szBuffer);
		}
		return 0;
	}
	case WM_DESTROY:
	case WM_CLOSE:{
		isStop = 1;
		break;
	}

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



DWORD WINAPI showDownloadDlg(LPVOID lpParam){
	isStop = 0;
	DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_DOWNLOAD), NULL, DialogProcDownload);
	return 0;
}