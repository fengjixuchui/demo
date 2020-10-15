#include "main.h"
#include "MainWnd.h"
#include <com-loader.hpp>


#ifdef _DEBUG
//#define COM_IMGDECODER  _T("imgdecoder-wicd.dll")
//#define COM_RENDER_GDI  _T("render-gdid.dll")
//#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
	#define COM_IMGDECODER  _T("imgdecoder-wic.dll")
	#define COM_RENDER_GDI  _T("render-gdi.dll")
	#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
	#define COM_IMGDECODER  _T("imgdecoder-wic.dll")
	#define COM_RENDER_GDI  _T("render-gdi.dll")
	#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
#define RES_TYPE 1

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));
    
    int nRet = 0; 

    SComLoader imgDecLoader;
    SComLoader renderLoader;
    SComLoader transLoader;
    //�����������·���޸ĵ���Ŀ����Ŀ¼���ڵ�Ŀ¼
    TCHAR szCurrentDir[MAX_PATH]={0};
    GetModuleFileName( NULL, szCurrentDir, sizeof(szCurrentDir) );
    LPTSTR lpInsertPos = _tcsrchr( szCurrentDir, _T('\\') );
    _tcscpy(lpInsertPos+1,_T("..\\demo-010"));
    SetCurrentDirectory(szCurrentDir);
        
    {

        CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
        imgDecLoader.CreateInstance(COM_IMGDECODER,(IObjRef**)&pImgDecoderFactory);
        renderLoader.CreateInstance(COM_RENDER_GDI,(IObjRef**)&pRenderFactory);

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

        SApplication *theApp=new SApplication(pRenderFactory,hInstance);

        HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
        if(hSysResource)
        {
            CAutoRefPtr<IResProvider> sysSesProvider;
            CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
            sysSesProvider->Init((WPARAM)hSysResource,0);
            theApp->LoadSystemNamedResource(sysSesProvider);
        }

		CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)//���ļ�����
        CreateResProvider(RES_FILE,(IObjRef**)&pResProvider);
        if(!pResProvider->Init((LPARAM)_T("uires"),0))
        {
            CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
            if(!pResProvider->Init((WPARAM)hInstance,0))
            {
                SASSERT(0);
                delete theApp;
                nRet = 1;
                goto exit;
            }
        }
#elif (RES_TYPE==1)//��EXE��Դ����
		/*
		����:
			..\tools\uiresbuilder.exe   --help
			not specify input file, using -i to define the input file
			usage: uiresbuilder -p uires -i uires\uires.idx -r .\uires\winres.rc2 -h .\uires\resource.h idtable
					param -i : define uires.idx path
					param -p : define path of uires folder
					param -r : define path of output .rc2 file
					param -h : define path of output resource.h file
					param idtable : define idtable is needed for resource.h. no id table for default.
		1.	������:
				..\tools\uiresbuilder.exe  -i "uires\uires.idx" -p uires -r .\res\soui_res.rc2 -h .\res\resource.h idtable
		2.  �� stdafx.h �ļ���,���:
				#include "res/resource.h"
		3.  �� *.rc �ļ�ĩβ���
				#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
				LANGUAGE 4, 2
				#pragma code_page(936)
				#include "res\soui_res.rc2" //SOUI��Դ
				#endif
		*/
        CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
        pResProvider->Init((WPARAM)hInstance,0);
#elif (RES_TYPE==2)//��ZIP������
        bLoaded=pComMgr->CreateResProvider_ZIP((IObjRef**)&pResProvider);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("resprovider_zip"));

        ZIPRES_PARAM param;
        param.ZipFile(pRenderFactory, _T("uires.zip"),"souizip");
        bLoaded = pResProvider->Init((WPARAM)&param,0);
        SASSERT(bLoaded);
#endif  
        theApp->AddResProvider(pResProvider);
        
		//2.x�汾�Ѿ�����Ҫ�������С�
        //theApp->Init(_T("XML_INIT")); 

        {
            //������������������д���
            CMainWnd wndMain;  
            wndMain.Create(GetActiveWindow(),0,0,800,600);
            wndMain.SendMessage(WM_INITDIALOG);
            wndMain.CenterWindow(wndMain.m_hWnd);
            wndMain.ShowWindow(SW_SHOWNORMAL);
            nRet=theApp->Run(wndMain.m_hWnd);
            //�������
        }

        delete theApp;
    }

    OleUninitialize();
    return nRet;
}
