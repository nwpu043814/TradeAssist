TYPE_HIGH = 0;
TYPE_LOW = 1;
TYPE_EQUAL = 2;
TYPE_UNKOWN = 3;


function getThreshold(y, delta)
	
	if y < 0 then
			y = -y;
	end;
	
	if delta <= 1 then
		delta = 4;
	end;
	
	local result = 20;
	if y <= delta then
			result = 20;
	elseif y <= delta + 9 then
			result = 1;
	elseif y <= delta + 13 then
			result = 1;
	elseif y <= delta + 20 then
			result = 4;
	elseif y <= delta + 40 then
			result = 8;
	elseif y <= delta + 60 then
			result = 12;
	elseif y <= delta + 80 then
			result = 20;
	elseif y <= delta + 100 then
			result = 20;
	elseif y<= delta + 150 then
			result = 25;
	elseif y <= delta + 200 then
			result = 32;
	else
			result = 42;
	end;
	
	return  result;
end

function delPrefix(data)
	data = string.gsub(data, "+","");
	data = string.gsub(data, "%%","");
	return data;
end

function getOriginal2DoHigh()
		return 340 ,160
end

function getOriginal2DoLow()
		return 220 ,160
end

function getOrigin2Entrust()
		return 364, 58
end

function getEntrust2Direction(direct)
	if direct == 1 then
		return -257, 90
	else 
		return -208, 90
	end
end

function getDirection2Price(direct)
	if direct == 1 then
		return 30,25
	else 
		return -19,25
	end
end

function getPrice2Count(direct)
	if direct == 1 then
		return -22,50
	else 
		return -22,50
	end
end

function getCount2Confirm()
	return 262,232
end

function getOrigin2Remove()
	return 30, 430
end

function getRemove2Confirm()
	return 170, -120
end

function getOrigin2Count()
	return 110,120
end

--1 is do low
function getCount2OrderButton(direct)
	if direct == 1 then
		return 1,2
	else
		return 3,4
	end
end

function getDailyFxUrl()
	return "http://www.dailyfx.com.hk/calendar/index.html";
end

function getChasePriceThreshold()
		return 4;
end

function getChasePriceMax()
		return 20;
end

--返回时间阈值，单位秒
function getChaseMaxTime()
		return 600;
end

--点击下拉列表 公共
function getOrigin2DropListButton()
		return 342, 69;
end

--切到市价建单 公共
function getOrderTypeButton()
		return 0 , 28;
end

--切到交易手数窗口
function getTradeCount()
		return -159,31
end

function getScaleList()
		return 0, 39 --65
end

--1 is do low 方向按钮 
function getDirectionButton(direct)
	if direct == 1 then
		return -220,134
	else
		return -282,134
	end
end

--价格设置按钮
function getPriceAdjustButton(direct)
	if direct == 1 then
		return -36,44
	else
		return 196,44
	end
end

--挂单价格到启用止损 
function getEnableStopButton(direct)
		return -244,45
end

--启用止损按钮到止损阈值
function getInitialStopPriceButton(direct)
		return 253,0
end

--止损阈值到止损价格
function getAdjustStopPriceButton(direct)
		return -130,0
end

--止盈价格输入框到确定按钮距离
function getConfirmButton(direct)
	return -30,72
end

-- 返回1代表中鑫龙祥
-- 返回2代表汇丰
function getDoubleSideType()
	return 2;
end

function getJoblessParam()
		return "美国6月失业率","7.5%","http://www.fx678.com/indexs/Datadetails.aspx?tid=1552&id=9957&fid=3782";
end
--
function getNonfarmerWorkerParam()
		return "美国6月非农就业人数变化" , "16.5", "http://www.fx678.com/indexs/Datadetails.aspx?tid=1547&id=9897&fid=3772"  
end
