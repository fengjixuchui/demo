
local function testDataType()
    a = nil
    b = true
    c = 1.0
    d = '123'
    e = function()
        printf("this is e function!")
    end
    -- f = userdata;
    -- g = thread
    h = {"abc",1}
    print(type("Hello world"))      --> string
    print(type(10.4*3))             --> number
    print(type(print))              --> function
    print(type(type))               --> function
    print(type(true))               --> boolean
    print(type(nil))                --> nil
    print(type(type(X)))            --> string


    -- boolean
    print(type(true))
    print(type(false))
    print(type(nil))

    if false or nil then
        print("������һ���� true")
    else
        print("false �� nil ��Ϊ false")
    end

    if 0 then
        print("���� 0 �� true")
    else
        print("���� 0 Ϊ false")
    end


    --[[   string   --]]
    html = [[
<html>
<head></head>
<body>
    <a href="http://www.runoob.com/">����</a>
</body>
</html>
]]
    print(html)


    --�ڶ�һ�������ַ����Ͻ�����������ʱ��Lua �᳢�Խ���������ַ���ת��һ������
    print("2" + 6)


    -- Lua �table �Ĵ�����ͨ��"������ʽ"����ɣ���򵥹�����ʽ��{}����������һ���ձ�Ҳ�����ڱ������һЩ���ݣ�ֱ�ӳ�ʼ����
    a = {}
    a["key"] = "value"
    key = 10
    a[key] = 22
    a[key] = a[key] + 11
    for k, v in pairs(a) do
        print(k .. " : " .. v)
    end




    --  Lua �У������Ǳ�������"��һ��ֵ��First-Class Value��"���������Դ��ڱ�����
    function factorial1(n)
        if n == 0 then
            return 1
        else
            return n * factorial1(n - 1)
        end
    end
    print(factorial1(5))
    factorial2 = factorial1
    print(factorial2(5))




    a = "Hello "
    b = "World"

    print("�����ַ��� a �� b ", a..b )

    print("b �ַ������� ",#b )

    print("�ַ��� Test ���� ",#"Test" )

    a = 20
    b = 10
    c = 15
    d = 5

    e = (a + b) * c / d;-- ( 30 * 15 ) / 5
    print("(a + b) * c / d ����ֵΪ  :",e )

    e = ((a + b) * c) / d; -- (30 * 15 ) / 5
    print("((a + b) * c) / d ����ֵΪ :",e )

    e = (a + b) * (c / d);-- (30) * (15/5)
    print("(a + b) * (c / d) ����ֵΪ :",e )

    e = a + (b * c) / d;  -- 20 + (150/5)
    print("a + (b * c) / d ����ֵΪ   :",e )

end

local function testPairsAndIpairs()
    local tabFiles = {
        [1] = "test2",
        [6] = "test3",
        [4] = "test1"
    }
    print("ipairs : \n")
    for k, v in ipairs(tabFiles) do    --���"test2",��key����2���Ͽ�
        print(k, v)
    end
    print("pairs : \n")
    for k, v in pairs(tabFiles) do  --[[ʲô��û�����Ϊʲô����Ϊ���Ʊ�����ʼֵ�ǰ������������ģ���keyΪ1ʱ��valueΪnil����ʱ��ֹͣ�˱����� ����ʲô�����û���]]--
        print(k, v)
    end
end



local function testCycle()

    -- while
    i = 1
    while(i<3)
    do
        print("while i = " .. i)
        i = i + 1

    end



    -- for

    for i=3,1,-1 do
        print(i)
    end

    a = {"one", "two", "three"}
    for i, v in ipairs(a) do
        print(i, v)
    end

    --[ �������� --]
    a = 0
    --[ ִ��ѭ�� --]
    repeat
        print("a��ֵΪ:", a)
        a = a + 1
    until( a > 3 )

    --goto Label
    --print("not to be shown")
	--:: Label ::
    --print("goto here");

end

local function testControl()
    --[ ������� --]
    a = 100

    --[ ��鲼������ --]
    if( a == 10 )
    then
        --[ �������Ϊ true ��ӡ������Ϣ --]
        print("a ��ֵΪ 10" )
    elseif( a == 20 )
    then
        --[ if else if ����Ϊ true ʱ��ӡ������Ϣ --]
        print("a ��ֵΪ 20" )
    elseif( a == 30 )
    then
        --[ if else if condition ����Ϊ true ʱ��ӡ������Ϣ --]
        print("a ��ֵΪ 30" )
    else
        --[ �����������û��һ��Ϊ true ʱ��ӡ������Ϣ --]
        print("û��ƥ�� a ��ֵ" )
    end
    print("a ����ʵֵΪ: ", a )


end


local function testSelect()

    function average(...)
        result = 0
        local arg={...}    --> arg Ϊһ�����ֲ�����
        for i,v in ipairs(arg) do
            result = result + v
        end
        print("�ܹ����� " .. #arg .. " ����")
        return result/#arg
    end

    function average_select(...)
        --[[
        select('#', ��) ���ؿɱ�����ĳ���
        select(n, ��)
         --]]
        result = 0
        local arg={...}
        for i,v in ipairs(arg) do
            result = result + v
        end
        print("�ܹ����� " .. select("#",...) .. " ����")
        return result/select("#",...)
    end

    print("averageƽ��ֵΪ",average(10,5,3,4,5,6))
    print("average_selectƽ��ֵΪ",average_select(10,5,3,4,5,6))


    do
        function foo(...)
            for i = 1, select('#', ...) do  -->��ȡ��������
                local arg = select(i, ...); -->��ȡ����
                print("arg", arg);
            end
        end

        foo(1, 2, 3, 4);
    end

end



local function testLogic()
    a = true
    b = true

    if ( a and b )
    then
        print("a and b - ����Ϊ true" )
    end

    if ( a or b )
    then
        print("a or b - ����Ϊ true" )
    end

    print("---------�ָ���---------" )

    -- �޸� a �� b ��ֵ
    a = false
    b = true

    if ( a and b )
    then
        print("a and b - ����Ϊ true" )
    else
        print("a and b - ����Ϊ false" )
    end

    if ( not( a and b) )
    then
        print("not( a and b) - ����Ϊ true" )
    else
        print("not( a and b) - ����Ϊ false" )
    end
end


local function testLocal()
    function test0001()
        test000001 = "local__test000001"
        local test000002 = "local__test000002"

    end

    test0001()
    print(test000001)
    print(test000002)
end

local function testFunction()
    local shape = {side = 4}
    function shape.set_side(shape, side)
        shape.side = side
    end

    function shape.print_area(shape)
        print(shape.side * shape.side)
    end

    shape:set_side(5)
    print(shape.side)
    shape:print_area()

    shape.set_side(shape,10)
    print(shape.side)
    shape.print_area(shape)
end

local function testFunction2()
    local shape = {side = 4}
    function shape:set_side(side)
        self.side = side
    end

    function shape:print_area()
        print(self.side * self.side)
    end

    print(shape.side)

    shape:set_side(5)
    print(shape.side)
    shape:print_area()


end
require("io")
require("string")

local function testPrint()
    function eraseTerminal()
        io.write("\27[2J")
    end
    -- writes an '*' at column 'x' , 'row y'
    function mark (x,y)
        local s = string.format("\27[%d;%dH*",y,x)
        io.write(s)
    end
    -- Terminal size
    TermSize = {w = 80, h = 24}
    -- plot a function
    -- (assume that domain and image are in the range [-1,1])
    function plot (f)
        eraseTerminal()
        for i=1,TermSize.w do
            local x = (i/TermSize.w)*2 - 1
            local y = (f(x) + 1)/2 * TermSize.h
            mark(i, y)
        end
        io.read() -- wait before spoiling the screen
    end
    plot(function (x) return math.sin(x*2*math.pi) end)
end

local function testIter()
    function list_iter (t)
        local i = 0
        return function ()
            i = i + 1
            if i <= #t  then return t[i] end
        end
    end
    t = {10, 20, 30}
    iter = list_iter(t) -- creates the iterator
    while true do
        local element = iter() -- calls the iterator
        if element == nil then break end
        print(element)

    end
    t = {10, 20, 30}
    for element in list_iter(t) do
        print(element)
    end
end

local function testIO()
	print "enter a number:"
	n = io.read("*number")
	if not n then error("invalid inputxxxx") end
	print("read number:", n)
end

--[[
multiple Lua VMs detected: ��Ҫʹ��dll lua�����ʽ 
--]]
local function testLoadLib()
	local path = 'D:\\systemp\\t123456789\\lua\\LTest\\Debug\\luaDll.dll'
	--local f = package.loadlib(path, "luaopen_mylib")
	--print("f:",type(f)); f()
	local f = package.loadlib(path, "luaopen_ldll")
	print("f:",type(f)); f()
	
	local ldll = require("ldll")
	print("ldll:", ldll.version())
	ldll.show()
end



local function testCall()
	local  status = xpcall(testLoadLib, function() print(debug.traceback()) end)
	if(status)
	then
		print("testCall ok")
	else
		print("testCall failed")
	end
end


local function testCoroutine()
	local co = coroutine.create(function ()
		for i = 1, 2 do
			print("co",i)
			coroutine.yield(co)
		end
	end)
	print('---------------------------------1')
	print(coroutine.resume(co))--1
	print(coroutine.status(co))
	
	print('---------------------------------2')
	print(coroutine.resume(co))--2
	print(coroutine.status(co))
	
	print('---------------------------------3')
	print(coroutine.resume(co))
	print(coroutine.status(co))
	
	print('---------------------------------4')
	print(coroutine.resume(co))
	print(coroutine.status(co))
	
	print('---------------------------------')
	co = coroutine.create(function (a,b,c)
		print("co2", a,b,c)
	end)
	coroutine.resume(co, 1, 2, 3)
	
	
	print('---------------------------------')
	co = coroutine.create(function (a,b)
		coroutine.yield(a + b, a - b)
	end)
	print(coroutine.resume(co, 20, 10)) --> true 30 10
	print('---------------------------------')
	co = coroutine.create(function ()
		return 6,7
	end)	
	print(coroutine.resume(co))
	
	print('---------------------------------')
	function receive (prod)
		local status, value = coroutine.resume(prod)
		return value
	end
	

	
	function send (x)
		coroutine.yield(x)
	end
	
	function producer ()
		return coroutine.create( function ()
			while true do
				local x = io.read() -- produce new value
				send(x)
			end
		end)
	end	
	
	function filter (prod)
		return coroutine.create(function ()
				local line = 1
				while true do
					local x = receive(prod) -- get new value
					x = string.format("%5d %s", line, x)
					send(x) -- send it to consumer
					line = line + 1
				end
		end)
	end
	
	
	function consumer (prod)
		while true do
			local x = receive(prod) -- get new value
			--io.write(x, "\n") -- consume new value
			print("x:",x)
		end
	end
	--consumer(filter(producer()))
	
	
	
	
	function permgen (a, n)
		if n == 0 then
			coroutine.yield(a)
		else
			for i=1,n do
				-- put i-th element as the last one
				a[n], a[i] = a[i], a[n]
				-- generate all permutations of the other elements
				permgen(a, n - 1)
				-- restore i-th element
				a[n], a[i] = a[i], a[n]
			end
		end
	end
	function printResult (a)
		for i,v in ipairs(a) do
			io.write(v, " ")
		end
		io.write("\n")
	end
	function perm (a)
		local n = #a
		--local co = coroutine.create(function () permgen(a, n) end)
		--return function () -- iterator
		--	local code, res = coroutine.resume(co)
		--	return res
		--end
		--create��ͬ���ǣ�wrap����һ��Эͬ���򣻲�ͬ����wrap������Эͬ�������Ƿ���һ�����������������������ʱ��resumeЭͬ
		--wrap����������ʱ��resumeЭͬ��wrap��resumeЭͬ��ʱ�򲻻᷵�ش��������Ϊ��һ�����ؽ����һ���д����������׳�����
		return coroutine.wrap(function () permgen(a, n) end)
	end
	
	for p in perm{"1", "2", "3"} do
		printResult(p)
	end

	
	
	
end
	
	

--[[
require����ʹ��ģ�飬module(5.1�汾)���ڴ���ģ��
��һ���� lua �� module ģ�������Ҫע��ĵ��ǣ��� lua_code_cache on ����ʱ�� require ���ص�ģ���ǻᱻ���������ģ��������ǵ�ģ��ͻ������Ч�ķ�ʽ���У�ֱ������ʽ�ص���������䣺
package.loaded["square"] = nil


ð�ź͵�ŵ�����:
	ð�Ų��������һ��self���������������Լ��������Ų�����ֻ�����ݵ�չ����
	ð�ŵĲ�����ֻ�е������������ʱ����Ҫ���й����ʹ�� Lua �����࣬��ҿɲο�����½ڡ�
--]]
local function testModule()
	print("-------------------------------")
	local jctest = require("MT01")
	jctest:Func1()
	jctest.num = 100;local jctest2 = require("MT01");print(jctest2.num)
	jctest.Func2();
	print(_G.JCTest.Func1())
	print(jctest.print)
	print("-------------------------------")
	
	
	--module
	if(_VERSION == "Lua 5.2" or _VERSION == "Lua 5.1")
	then
		
		local mt02 = require("MT02")
		mt02.printB()
		mt02.print("mt02.print(...)")
		print("-------------------------------")
	else
		print("[warning] not test module function",_VERSION)
	end
	
	
	local square = require "square" 
	
	local s1 = square:new(1, 2)
	print(s1:get_square())          --output: 2
	print(s1:get_circumference())   --output: 6
end

--[[
setmetatable(table, metatable)
Lua�е��������ֻ�������������Ԫ�����Ҫ��ı��������͵�Ԫ��ֻ����C������ʵ�֣�����ͨ��lua�������ã�����ڶ�������matetableΪnil����table��Ԫ������Ϊ�ա����ԭ��table��Ԫ���а���һ��"__metatable"�ֶΣ���ô���������׳�һ������

--]]
local function testMetalable()
	mytable = setmetatable({key1 = "value1"}, {
	  __index = function(mytable, key)
	    if key == "key2" then
	      return "metatablevalue"
	    else
	      return nil
	    end
	  end
	})
	
	--print(mytable.key1,mytable.key2)
	--local a={}
	--print(getmetatable(a), getmetatable(mytable))
	
	
	
	Set = {}
	Set.mt = {}
	function Set.new (t)
		local set = {}
		setmetatable(set, Set.mt)
		for _, l in ipairs(t) do set[l] = true end
		return set
	end
	
	function Set.union (a,b)
		local res = Set.new{}
		for k in pairs(a) do res[k] = true end
		for k in pairs(b) do res[k] = true end
		return res
	end
	Set.mt.__add = Set.union
	
	function Set.intersection (a,b)
		local res = Set.new{}
		for k in pairs(a) do
			res[k] = b[k]
		end
		return res
	end
	Set.mt.__mul = Set.intersection
	
	
	Set.mt.__le = function (a,b) -- set containment
		for k in pairs(a) do
			if not b[k] then return false end
		end
		return true
	end
	Set.mt.__lt = function (a,b)
		return a <= b and not (b <= a)
	end
	Set.mt.__eq = function (a,b)
		return a <= b and b <= a
	end
	
	function Set.tostring (set)
		local s = "{"
		local sep = ""
		for e in pairs(set) do
			s = s .. sep .. e
			sep = ", "
		end
		return s .. "}"
	end
	Set.mt.__tostring = Set.tostring
		

	s1 = Set.new{10, 20, 30, 50}
	s2 = Set.new{30, 1}
	--print(getmetatable(s1)) --> table: 00672B60
	--print(getmetatable(s2))	
	
	s3 = s1 + s2
	print(s3)
	
	print((s1 + s2)*s1)
	
	s1 = Set.new{2, 4}
	s2 = Set.new{4, 10, 2}
	print("s1 <= s2", s1 <= s2) --> true
	print("s1 < s2", s1 < s2) --> true
	print("s1 >= s1", s1 >= s1) --> true
	print("s1 > s1", s1 > s1) --> false
	print("s1 == s2", s1 == s2) --> true
	print("s1 == s2 * s1", s1 == s2 * s1) --> true
	
	
	local Window = {}
	-- create the prototype with default values
	Window.prototype = {x=0, y=0, width=100, height=100, }
	-- create a metatable
	Window.mt = {}
	Window.mt.__index = Window.prototype
	-- declare the constructor function
	function Window.new (o)
		setmetatable(o, Window.mt)
		return o
	end
	local w = Window.new{x = 20, y = 199}
	print(w["y"])
	
	-- __newindex metamethod�����Ա���£�__index�������Ա����
	-- ��������һ��ȱ�ٵ���ֵ���������ͻ����__newindex metamethod����������������������������и�ֵ����
	-- ��__indexһ�������metamethod��һ������������ָ�����Ǹ���������ԭʼ�ı���и�ֵ������
	-- ��һ��raw���������ƹ�metamethod������rawset(t,k,v)�������κ�metamethod�Ա�t��k��ֵΪv
	
	
	local t = {} -- original table (created somewhere)
	-- keep a private access to original table
	local _t = t
	-- create proxy
	local t = {}
	-- create metatable
	local mt = {
		__index = function (t,k)
			print("*access to element " .. tostring(k))
			return _t[k] -- access the original table
			end,
			__newindex = function (t,k,v)
			print("*update of element " .. tostring(k) ..
			" to " .. tostring(v))
			_t[k] = v -- update original table
		end
	}
	setmetatable(t, mt)
	
	t["aabbcc"] = "1"
	t["aabbcc"] = "12"
	print(t["aabbcc"])
	
	
	-- create private index
	local index = {}
	-- create metatable
	local mt = {
		__index = function (t,k)
			print("*access to element " .. tostring(k))
			return t[index][k] -- access the original table
		end,
		__newindex = function (t,k,v)
			print("*update of element " .. tostring(k) .. " to " .. tostring(v))
			t[index][k] = v -- update original table
		end
	}
	function track (t)
		local proxy = {}
		proxy[index] = t
		setmetatable(proxy, mt)
		return proxy
	end
	 
	b = track({})
	b["ssdd"] = "2"
	b["ssddff"] = "23"
	print(b["ssddff"])
	
	
	--ֻ����
	local function readOnly (t)
		local proxy = {}
		local mt = { -- create metatable
			__index = t,
			__newindex = function (t,k,v)
				error("attempt to update a read-only table", 2)
			end
			}
		setmetatable(proxy, mt)
		return proxy
	end
	
	local days = readOnly{"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"}
	print(days[1]) --> Sunday
	--days[2] = "Noday" -- error
	
	
	
end

local function testString()
	varname = "[[" .. type(os.execute('dir')) .. "]]"
	print(varname)
end

local function testEnv()
	--for n in pairs(_G) do print(n) end
	
	
	if false
	then
		function declare (name, initval)
			rawset(_G, name, initval or false)
		end
		setmetatable(_G, {
			__newindex = function (_, n)
			error("attempt to write to undeclared variable "..n, 2)
			end,
			__index = function (_, n)
			error("attempt to read undeclared variable "..n, 2)
			end,
		})
	
		declare("a")
		a=1
		print(a)
	end
	
	
	if(_VERSION == "Lua 5.1")
	then
		a = 1 -- create a global variable
		
		--setfenv(1, {_G = _G}) -- change current environment
		--_G.print(a) --> nil
		--_G.print(_G.a) --> 1
		
		
		local newgt = {} -- create new environment
		setmetatable(newgt, {__index = _G})
		setfenv(1, newgt) -- set it
		print(a) --> 1
		
		print("-----------------------------")
		a = 10
		print(a) --> 10
		print(_G.a) --> 1
		_G.a = 20
		print(_G.a) --> 20
		
	end
end

local function testPackage()
	-- ����1
	local function p1()
		local complex = {}
		function complex.new (r, i) return {r=r, i=i} end
		
		-- defines a constant `i'
		complex.i = complex.new(0, 1)
		
		function complex.add (c1, c2)
			return complex.new(c1.r + c2.r, c1.i + c2.i)
		end
		function complex.sub (c1, c2)
			return complex.new(c1.r - c2.r, c1.i - c2.i)
		end
		function complex.mul (c1, c2)
			return complex.new(c1.r*c2.r - c1.i*c2.i,c1.r*c2.i + c1.i*c2.r)
		end
		function complex.inv (c)
			local n = c.r^2 + c.i^2
			return complex.new(c.r/n, -c.i/n)
		end
		return complex
	end

	
	local function p2()
		local function checkComplex (c)
			if not ((type(c) == "table")
				and tonumber(c.r) and tonumber(c.i)) then
				error("bad complex number", 3)
			end
		end
		
		local function new (r, i) return {r=r, i=i} end
		
		
		local function add (c1, c2)
			checkComplex(c1);
			checkComplex(c2);
			return new(c1.r + c2.r, c1.i + c2.i)
		end

		local function sub (c1, c2)
			return new(c1.r - c2.r, c1.i - c2.i)
		end
		local function mul (c1, c2)
			return new(c1.r*c2.r - c1.i*c2.i,c1.r*c2.i + c1.i*c2.r)
		end
		local function div (c)
			local n = c.r^2 + c.i^2
			return new(c.r/n, -c.i/n)
		end
		
		local complex = {
			new = new,
			add = add,
			sub = sub,
			mul = mul,
			div = div,
		}
		complex.i = complex.new(0, 2)
		return complex
	end
	c = p2()
	print(c.r, c.i , c.i.r, c.i.i)
	c = c.add(c.i, c.new(10, 20))
	print(c.r, c.i)
	
	function openpackage (ns)
		for n,v in pairs(ns) do
			if _G[n] ~= nil then
				error("name clash: " .. n .. " is already defined")
			end
			_G[n] = v
		end
	end
	
	local ABC = {}
	ABC.show = function()
		print("ABC.show()") 
	end
	
	--openpackage(ABC)
	--openpackage(ABC)  -- �ڶ��ζ����׳�����
	local function p3()
		local location = {
			foo = "/usr/local/lua/lib/pack1_1.lua",
			goo = "/usr/local/lua/lib/pack1_1.lua",
			foo1 = "/usr/local/lua/lib/pack1_2.lua",
			goo1 = "/usr/local/lua/lib/pack1_3.lua",
		}
		pack1 = {}
		
		setmetatable(pack1, {
			__index = function (t, funcname)
				local file = location[funcname]
				if not file then
					error("package pack1 does not define " .. funcname)
				end
				assert(loadfile(file))() -- load and run definition
				return t[funcname] -- return the function
			end
		})
		return pack1
	end
end


--[[
Lua����һ����Ԫ�ص�3������
	������ܽ�һ��Lua����һ����Ԫ��ʱ�Ĺ�����ʵ��������3������:
	1.�ڱ��в��ң�����ҵ������ظ�Ԫ�أ��Ҳ��������
	2.�жϸñ��Ƿ���Ԫ������ָ�ϣ������û��Ԫ������nil����Ԫ�������
	3.�ж�Ԫ������ָ�ϣ�����û�й�������ʧ�ܵ�ָ�ϣ���__index�����������û�У���__index����Ϊnil�����򷵻�nil�����__index������һ�������ظ�1��2��3�����__index������һ���������򷵻ظú����ķ���ֵ
--]]
local function testOBJ()

		
	father = {
		house=1
	}
	--father.__index = father
	son = {
		car=1
	}
	setmetatable(son, father) --��son��metatable����Ϊfather
	print(son.house)
	--[[
	�����������У�����son.houseʱ��son��û��house�����Ա����Lua���ŷ���son��Ԫ��father��
	ע�⣺��ʱ��Lua������ֱ����father������Ϊhouse�ĳ�Ա�����ǵ���father��__index���������__index����Ϊnil���򷵻�nil�������һ����������father��__index���������Լ��������������������ô�͵�__index������ָ��������в�����Ϊhouse�ĳ�Ա�����ǣ������ҵ���house��Ա��
	ע��__index�������˿�����һ������������һ�������������һ��������__index����������ʱ�����ظú����ķ���ֵ��	
	--]]
		
	local Account = {balance = 0}
	function Account:new (o)
		o = o or {}
		setmetatable(o, self)
		print("Account:", Account)     --            table: 0000000019C8E180
		print("self", self) -- ��ǰselfΪAccount table: 0000000019C8E180
		self.__index = self
		return o
	end
	function Account:deposit (v)
		self.balance = self.balance + v
	end
	function Account:withdraw (v)
		if v > self.balance then error"insufficient funds" end
			self.balance = self.balance - v
	end
	
	SpecialAccount = Account:new()
	print(SpecialAccount) --table: 0000000019C8DF00
	--s = SpecialAccount:new{limit=1000.00}
	--s:deposit(100.00)
	
	-- ���ؼ̳�
	Named = {}
	function Named:getname ()
		return self.name
	end
	
	function Named:setname (n)
		self.name = n
	end
	
	
	-- look up for `k' in list of tables 'plist'
	local function search (k, plist)
		for i=1, #plist do
			local v = plist[i][k] -- try 'i'-th superclass
			if v then return v end
		end
	end
	function createClass (...)
		local c = {} -- new class
		-- class will search for each method in the list of its
		-- parents (`arg' is the list of parents)
		setmetatable(c, {__index = function (t, k)
			local v = search(k, arg)
			t[k] = v -- save for next access
			return v
		end})
		--Ӧ��������ɣ����ʼ̳еķ����ͷ��ʾֲ�����һ����.ȱ����ϵͳ����֮�󣬺��Ѹı䷽���Ķ��壬��Ϊ���ָı䲻��Ӱ��̳������¶ˡ�
		-- prepare `c' to be the metatable of its instances
		c.__index = c
		-- define a new constructor for this new class
		function c:new (o)
			o = o or {}
			setmetatable(o, c)
			return o
		end
		-- return new class
		return c
	end
	
	
	NamedAccount = createClass(Account, Named)
	account = NamedAccount:new{name = "Paul"}
	print(account:getname()) --> Paul
	
	
	
	function newAccount (initialBalance)
		local self = {balance = initialBalance}
		local withdraw = function (v)
			self.balance = self.balance - v
		end
		local deposit = function (v)
			self.balance = self.balance + v
		end
		local getBalance = function () 
			return self.balance 
		end
		
		return {
			withdraw = withdraw,
			deposit = deposit,
			getBalance = getBalance
		}
	end


	acc1 = newAccount(100.00)
	acc1.withdraw(40.00)
	--acc1:withdraw(40.00) -- error
	print(acc1.getBalance()) --> 60
		
end



local function testWeak()
	a = {}
	b = {}
	setmetatable(a, b)
	b.__mode = "v" -- now 'a' has weak keys
	key = {} -- creates first key
	a[key] = 1
	key = {} -- creates second key
	a[key] = 2
	collectgarbage() -- forces a garbage collection cycle
	for k, v in pairs(a) do print(v) end
end

local function testTable()
	print(#{10,2,4}) --> 3
	print(#{10,2,nil}) --> 2
	print(#{10,2,nil; n=3}) --> 2
	print(#{n=1000})  --> 0
	print(#{n=1000,n2=2000})  --> 0
end

local function testString()
	print(string.lower("a?b??c"))
	print(string.upper("A?B??C"))
	s = "[in brackets]"
	print(string.sub(s, 2, -2))
	
	print(string.char(97)) --> a
	i = 99; print(string.char(i, i+1, i+2)) --> cde
	print(string.byte("abc")) --> 97
	print(string.byte("abc", 2)) --> 98
	print(string.byte("abc", -1)) --> 99
	
	
	print(string.format("pi = %.4f", math.pi))
	--> pi = 3.1416
	d = 5; m = 11; y = 1990
	print(string.format("%02d/%02d/%04d", d, m, y))
	--> 05/11/1990
	tag, title = "h1", "a title"
	print(string.format("<%s>%s</%s>", tag, title, tag))
	--> <h1>a title</h1>
	
	
	s = "hello world"
	i, j = string.find(s, "hello")
	print(i, j) --> 1 5
	print(string.sub(s, i, j)) --> hello
	print(string.find(s, "world")) --> 7 11
	i, j = string.find(s, "l")
	print(i, j) --> 3 3
	print(string.find(s, "lll")) --> nil
	
	local t = {} -- table to store the indices
	local i = 0
	while true do
		i = string.find(s, "\n", i+1) -- find 'next' newline
		if i == nil then break end
		table.insert(t, i)
	end
	
	--string.gsub����������������Ŀ�괮��ģʽ�����滻��
	s = string.gsub("Lua is cute", "cute", "great")
	print(s) --> Lua is great
	s = string.gsub("all lii", "l", "x")
	print(s) --> axx xii
	s = string.gsub("Lua is great", "perl", "tcl")
	print(s) --> Lua is great
	
	
	s = "Deadline is 30/05/1999, firm"
	date = "%d%d/%d%d/%d%d%d%d"
	print(string.sub(s, string.find(s, date))) --> 30/05/1999
	
	-- %������ת���ʶ
	--'.*' ���е����ƥ��
	--'.-' ���е������ƥ��
	--'%d' ��ʾ '[0-9]'
	--'%x' ��ʾ'[0-9a-fA-F]'
	--'-' �� '*' һ������ƥ��һ���ַ���0�λ��γ���
	test = "int x; /* x */ int y; /* y */"
	print(string.gsub(test, "/%*.*%*/", "<COMMENT>"))
	--> int x; <COMMENT>
	test = "int x; /* x */ int y; /* y */"
	print(string.gsub(test, "/%*.-%*/", "<COMMENT>"))
	--> int x; <COMMENT> int y; <COMMENT>
	
	-- %b' ����ƥ��ԳƵ��ַ�����дΪ '%bxy' ��x��y������������ͬ���ַ���x��Ϊƥ��Ŀ�ʼ��y��Ϊƥ��Ľ��������磬'%b()' ƥ���� '(' ��ʼ���� ')' �������ַ�����
	print(string.gsub("a (enclosed (in) parentheses) line", "%b()", ""))
	
	pair = "name = Anna"
	_, _, key, value = string.find(pair, "(%a+)%s*=%s*(%a+)")
	print(key, value) --> name Anna
	
	
	date = "17/7/1990"
	_, _, d, m, y = string.find(date, "(%d+)/(%d+)/(%d+)")
	print(d, m, y) --> 17 7 1990
	
	s = [[then he said: "it's all right"!]]
	a, b, c, quotedPart = string.find(s, "([\"'])(.-)%1")
	print(a,b,c,quotedPart) --> it's all right
	print(c) --> "
	
	print(string.gsub("hello Lua!", "(%a)", "%1-%1"))
	--> h-he-el-ll-lo-o L-Lu-ua-a!
	
	print(string.gsub("hello Lua", "(.)(.)", "%2%1"))
	--> ehll ouLa
	
	function trim (s)
		return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
	end
	print(trim("   [123 xx  ff]  "))
	
	
	function expand (s)
		s = string.gsub(s, "$(%w+)", function (n) return _G[n] end)
		return s
	end
	name = "Lua"; status = "great"
	print(expand("$name is $status, isn't it?"))
		
	
	function expand (s)
		return (string.gsub(s, "$(%w+)", function (n)
			return tostring(_G[n])
		end))
	end
	print(expand("print = $print; a = $a"))
		
	
	s = "sin(3) = $[math.sin(3)]; 2^5 = $[2^5]"
	print((string.gsub(s, "$(%b[])", function (x)
		x = "return " .. string.sub(x, 2, -2)
		local f = loadstring(x)
		return f()
	end)))
	
	
	words = {}
	for w in string.gfind(s, "%a") do
		table.insert(words, w)
	end
	
	--TODO more...
	
end

local function testIO()
	io.write(string.format("sin (3) = %.4f\n", math.sin(3)))
	--t = io.read("*line") -- read the whole file
	--t = string.gsub(t, "abc","123") -- do the job
	--io.write(t) -- write the file
	function fsize (file)
		local current = file:seek() -- get current position
		local size = file:seek("end") -- get file size
		file:seek("set", current) -- restore position
		return size
	end
end

local function testFile()
	local f = assert(io.open("./src/MT01.lua", "rb"))
	local block = 10
	while true do
		local bytes = f:read(block)
		if not bytes then break end
		-- .(��): ���κ��ַ����
		-- string.gmatch (s, pattern)
		-- ����һ��������������ÿ�ε������������������� pattern ���μ�����6.4.1���� s ��ƥ�䣬���������в��񵽵�ֵ����� pattern ��û��ָ��������ÿ�β������� pattern
		for b in string.gfind(bytes, ".") do
			io.write(string.format("%02X ", string.byte(b)))
		end
		-- string.rep (s, n [, sep])
		-- ���� n ���ַ��� s ���ַ��� sepΪ�ָ������һ����ַ�����Ĭ�ϵ� sep ֵΪ���ַ�������û�зָ��������� n ���������򷵻ؿմ���
		io.write(string.rep(" ", (block - string.len(bytes))*3 + 1))
		
		--string.gsub (s, pattern, repl [, n])
		--���ַ��� s �У����еģ������� n ����ʱ��ǰ n ����pattern ���μ� ��6.4.1�����滻�� repl ���������丱����
		--%c: ���κο��Ʒ����(����\n)
		io.write(string.gsub(bytes, "%c", "."), "\n")
	end

end

local function testOS()
	print(os.time{year=1970, month=1, day=1, hour=0})
	--> 10800
	print(os.time{year=1970, month=1, day=1, hour=0, sec=1})
	--> 10801
	print(os.time{year=1970, month=1, day=1})
	
	temp = os.date("*t", 906000490)
	for i, v in pairs(temp)
	do
		print(i,v)
	end
	
	print(os.date("today is %A, in %B"))
	--> today is Tuesday, in May
	print(os.date("%x", 906000490))
	--> 09/17/98
	
	local x = os.clock()
	local s = 0
	for i=1,10000000 do s = s + i end
	print(string.format("elapsed time: %.2f\n", os.clock() - x))
	
	print(os.getenv("HOME"))
	
	local function createDir (dirname)
		os.execute("mkdir " .. dirname)
	end
	
	print(os.setlocale("ISO-8859-1", "collate"))
	
	-- ��������Ϊ��������ϵ����
	print(os.setlocale('pt_BR')) --> pt_BR
	print(3,4) --> 3 4
	print(3.4) --> stdin:1: malformed number near `3.4'
	
end

local function testDebug()
	--[[
		Debug���е�һ����Ҫ��˼����ջ����(stack level)��
		һ��ջ�������һ��ָ���ڵ�ǰʱ�����ڻ�����⺯�������֣�
		Ҳ����˵������������ڱ����õ���û�з��ء�����debug��ĺ�������Ϊ1��
		������(��ָ����debug��ĺ���)�ĺ�������Ϊ2���Դ����ơ�
	--]]
	local function traceback ()
		local level = 1
		while true do
			--S  ���� source short_src linedefined lastlinedefined what
			--l  ����currentline
			local info = debug.getinfo(level, "Sl")
			if not info then break end
			
			--if info.what == "C" then -- is a C function?
			--	print(level, "C function")
			--else -- a Lua function
			--	print(string.format("[%s]:%d",info.short_src, info.currentline))
			--end
			for i,v in pairs(info)
			do
				print(i,v)
			end
			print('\n')
			level = level + 1
		end
	end
	traceback()
	
	
	function foo (a,b)
		local x
		do local c = a - b end
		local a = 1
		while true do
			local name, value = debug.getlocal(1, a)
			if not name then break end
			print(name, value)
			a = a + 1
		end
	end
	foo(10, 20)
	
	local abc='123456'
	function getvarvalue (name)
		local value, found
		-- try local variables
		local i = 1
		while true do
			local n, v = debug.getlocal(2, i)
			if not n then break end
				if n == name then
					value = v
					found = true
				end
				i = i + 1
			end
			if found then return value end
			-- try upvalues
			local func = debug.getinfo(2).func
			i = 1
			while true do
				local n, v = debug.getupvalue(func, i)
				if not n then break end
				if n == name then return v end
				i = i + 1
			end
		-- not found; get global
		print(func,name)
		return getfenv(func)[name]
	end
	print(getvarvalue("abc"))
	
	
	--[[
	debug.sethook ([thread,] hook, mask [, count])
		��һ��������Ϊ���Ӻ������롣�ַ��� mask �Լ����� count �����˹��ӽ��ں�ʱ���á��������������ַ���ϳɵ��ַ�����ÿ���ַ����京�壺
		    'c': ÿ�� Lua ����һ������ʱ�����ù��ӣ�
		    'r': ÿ�� Lua ��һ�������ڷ���ʱ�����ù��ӣ�
		    'l': ÿ�� Lua �����µ�һ��ʱ�����ù��ӡ� ���⣬����һ����Ϊ��� count �����ӽ���ÿ���� count ��ָ��ʱ���á�
		��������������debug.sethook �رչ��ӡ�
	--]]
	function trace (event, line)
		--[[
		debug.getinfo ([thread,] f [, what])
			���ع���һ��������Ϣ�ı������ֱ���ṩ�ú�����Ҳ������һ������ f ��ʾ�ú��������� f ��ʾ������ָ���̵߳ĵ���ջ��Ӧ����ϵĺ�����0 ���ʾ��ǰ������getinfo ������1 ���ʾ���� getinfo �ĺ�����������β���ã��������������ջ�����ȵȡ���� f ��һ���Ȼ����������������֣�getinfo ���� nil��
			ֻ���ַ��� what ��������Ҫ�����Щ����صı���԰��� lua_getinfo �ܷ��ص������what Ĭ���Ƿ����ṩ�ĳ��Ϸ��кű����������Ϣ������ѡ�� 'f' �����ڿ��ܵ�����£�����func �򱣴溯����������ѡ�� 'L' �����ڿ��ܵ�����£�����activelines �򱣴�Ϸ��кű�
			����,���ʽ debug.getinfo(1,"n")���ش��е�ǰ����������Ϣ�ı�����ҵĵ����ֵĻ��������ʽ debug.getinfo(print) ���ع��� print �����İ������������ṩ��Ϣ�ı�
		--]]
		local s = debug.getinfo(2).short_src
		print(s .. ":" .. line)
	end
	--debug.sethook(trace, "l")
	--print("hooka")
	
	
	-- ���Ժ������ô���
	local Counters = {}
	local Names = {}
	
	local function hook ()
		local f = debug.getinfo(2, "f").func
		if Counters[f] == nil then -- first time `f' is called?
			Counters[f] = 1
			--[[
				  if (strchr(options, 'S')) {
				    settabss(L, "source", ar.source);
				    settabss(L, "short_src", ar.short_src);
				    settabsi(L, "linedefined", ar.linedefined);
				    settabsi(L, "lastlinedefined", ar.lastlinedefined);
				    settabss(L, "what", ar.what);
				  }
				  if (strchr(options, 'l'))
				    settabsi(L, "currentline", ar.currentline);
				  if (strchr(options, 'u')) {
				    settabsi(L, "nups", ar.nups);
				    settabsi(L, "nparams", ar.nparams);
				    settabsb(L, "isvararg", ar.isvararg);
				  }
				  if (strchr(options, 'n')) {
				    settabss(L, "name", ar.name);
				    settabss(L, "namewhat", ar.namewhat);
				  }
				  if (strchr(options, 't'))
				    settabsb(L, "istailcall", ar.istailcall);
				  if (strchr(options, 'L'))
				    treatstackoption(L, L1, "activelines");
				  if (strchr(options, 'f'))
				    treatstackoption(L, L1, "func");
			--]]
			Names[f] = debug.getinfo(2, "Sn")
		else -- only increment the counter
			Counters[f] = Counters[f] + 1
		end
	end
	debug.sethook(hook, "c")
	testFunction()
	debug.sethook() ---- turn off the hook
	
	
	function getname (func)
		local n = Names[func]
		if n.what == "C" then
			return n.name
		end
		local loc = string.format("[%s]:%s", n.short_src, n.linedefined)
		if n.namewhat ~= "" then
			return string.format("%s (%s)", loc, n.name)
		else
			return string.format("%s", loc)
		end
	end
	for func, count in pairs(Counters) do
		print(getname(func), count)
	end
	
end


--C��Lua�м������ֽ�����ʽ��
--��һ�֣�C��ΪӦ�ó������ԣ�Lua��Ϊһ����ʹ�ã�
--�ڶ��֣���������Lua��Ϊ�������ԣ�C��Ϊ��ʹ�á������ַ�ʽ��C���Զ�ʹ����ͬ��API��Luaͨ��
--���C��Lua�����ⲿ�ֳ�ΪC API��
local function testCAPI()
--[[
ѹ��Ԫ��
	void lua_pushnil (lua_State *L);
	void lua_pushboolean (lua_State *L, int bool);
	void lua_pushnumber (lua_State *L, double n);
	void lua_pushlstring (lua_State *L, const char *s,
	size_t length);
	void lua_pushstring (lua_State *L, const char *s);
	
���ջ���Ƿ����㹻����Ҫ�Ŀռ�
	int lua_checkstack (lua_State *L, int sz);
	
	
��ѯԪ��
	API������������ջ�е�Ԫ�ء���ջ�еĵ�һ��Ԫ�أ�Ҳ���ǵ�һ����ѹ��ջ�ģ�������1����һ��������2���Դ����ơ�����Ҳ������ջ����Ϊ��������ȡԪ�أ����ø�����������������£�-1ָ��ջ��Ԫ�أ�Ҳ�������ѹ��ģ���-2ָ������ǰһ��Ԫ�أ��Դ����ơ����磬����lua_tostring(L, -1)���ַ�������ʽ����ջ����ֵ���������潫��������ĳЩ����ʹ������������ջ�ȽϷ��㣬����һЩ����£�ʹ�ø���������ջ�����㡣
	lua_isnumber��lua_isstring������������ֵ�Ƿ���ָ�������ͣ����ǿ����Ƿ��ܱ�ת����ָ�����������͡����磬�κ��������Ͷ�����lua_isstring��

	int lua_toboolean (lua_State *L, int index);
	double lua_tonumber (lua_State *L, int index);
	const char * lua_tostring (lua_State *L, int index);
	size_t lua_strlen (lua_State *L, int index);
	

������ջ����
	int lua_gettop (lua_State *L);
		����lua_gettop���ض�ջ�е�Ԫ�ظ�������Ҳ��ջ��Ԫ�ص�������ע��һ����������-x��Ӧ����������gettop-x+1
	void lua_settop (lua_State *L, int index);
	void lua_pushvalue (lua_State *L, int index);
	void lua_remove (lua_State *L, int index);
	void lua_insert (lua_State *L, int index);
	void lua_replace (lua_State *L, int index);

�Ӷ�ջ�е���n��Ԫ�أ�
	#define lua_pop(L,n) lua_settop(L, -(n)-1)
	
	
����lua_pushvalueѹ���ջ��ָ��������һ���ұ���ջ����
	lua_remove�Ƴ�ָ������λ�õ�Ԫ�أ��������������е�Ԫ������������λ�õĿհף�
	lua_insert�ƶ�ջ��Ԫ�ص�ָ��������λ�ã������������λ�������Ԫ��ȫ��������ջ�����ƶ����µĿո���
	lua_replace��ջ������Ԫ��ֵ���������õ�ָ������λ�ã�û���κ��ƶ�������ע�⵽����Ĳ����Զ�ջû���κ�Ӱ��
		lua_settop(L, -1); /* set top to its current value */
		lua_insert(L, -1); /* move top element to the top */
	
	
API�ṩ�����������������������
	int lua_rawgeti (lua_State *L, int index, lua_Integer n);
		�� t[n] ��ֵѹջ������� t ��ָ�����������ı�����һ��ֱ�ӷ��ʣ�����˵�������ᴥ��Ԫ������������ջֵ������
	void lua_rawseti (lua_State *L, int index, lua_Integer i);
		�ȼ��� t[i] = v ������� t ��ָ�����������ı��� v ��ջ����ֵ��
		��������Ὣֵ����ջ����ֵ��ֱ�ӵģ������ᴥ��Ԫ������
--]]


--[[
User-Defined Types in C
	Lua API�ṩ������ĺ�����������һ��userdatum��
		void *lua_newuserdata (lua_State *L, size_t size);


--]]
end


--testLocal()
--testFunction()
--testFunction2()
--testDataType()
--testSelect()
--testPairsAndIpairs()
--testCycle()
--testControl()
--testLogic()
--testPrint()
--testIter()
--testLoadLib()
--testIO()
--testCall()
--testCoroutine()
--testModule()
--testString()
--testMetalable()
--testEnv()
--testPackage()
--testOBJ()
--testWeak()
--testTable()
--testString()
--testIO()
--testFile()
--testOS()
--testDebug()
testCAPI()






