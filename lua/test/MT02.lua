
--����module����ʱ���ഫ��һ�� package.seeall �Ĳ���
--�൱�� setmetatable(M, {__index = _G})

--[[
����ʹ�� module()���������� Lua ģ��
	��ʽ��ģ�鶨�巽ʽ��ͨ�� *module("filename"[,package.seeall])*����ʾ����һ������
	���ڹٷ����Ƽ���ʹ�����ַ�ʽ�����ַ�ʽ���᷵��һ���� filename ģ�麯����ɵ� table��
	���һ��ᶨ��һ�������� table ��ȫ�ֱ�����
	���ֻ�� module ����һ��������Ҳ�����ļ������Ļ���ǰ�涨���ȫ�ֱ����Ͷ��������ˣ�
	���� print �����ȣ����Ҫ��֮ǰ��ȫ�ֱ����ɼ���
	�����ڶ��� module ��ʱ����ϲ��� package.seeall ��
	������ module ����֮�� print ��Щϵͳ��������ʹ�õ�ԭ��
	�ǵ�ǰ������������ѹ��ջ�����ٿɴ
	
	
module("filename", package.seeall) ����д����Ȼ�ǲ��ᳫ�ģ��ٷ�����������ԭ��
    package.seeall ���ַ�ʽ�ƻ���ģ��ĸ��ھۣ�ԭ������"filename"ģ��ֻ��������� foobar() ������������ȴ���Զ�дȫ�����ԣ����� "filename.os" ��
    module ����ѹջ���������ĸ����ã���Ⱦ��ȫ�ֻ������������� module("filename") �ᴴ��һ�� filename �� table ��������� table ע��ȫ�ֻ��������У�����ʹ��û�����������ļ�Ҳ�ܵ��� filename ģ��ķ�����

--]]

module("MT02",package.seeall)
--module("MT02") -- ʹ����һ�лᵼ��print�޷�����

function printB()   print('B')  end

function printC()   print('C')  end  

