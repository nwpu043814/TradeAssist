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
		return 388, 57
end

function getEntrust2Direction(direct)
	if direct == 1 then
		return -275, 85
	else 
		return -226, 85
	end
end

function getDirection2Price(direct)
	if direct == 1 then
		return 29,24
	else 
		return -20,24
	end
end

function getPrice2Count(direct)
	if direct == 1 then
		return -29,54
	else 
		return -29,54
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
		return 182,122 -- win7 26 , xp 31
end

function getScaleList()
		return 0, 39 --65
end

--1 is do low 方向按钮 
function getDirectionButton(direct)
	if direct == 1 then
		return -220,124
	else
		return -282,124
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

--汇丰挂单价格到启用止损 
function getHuiFengEnableStopButton(direct)
		return -244,45
end

--天通挂单价格到启用止损 
function getEnableStopButton(direct)
		return -317,83
end

--启用止损按钮到止损阈值
function getInitialStopPriceButton(direct)
		return 157,-8
end

--止损阈值到止损价格
function getAdjustStopPriceButton(direct)
		return -130,0
end

--挂单范围到挂单输入框
function getPriceRange2Price(direct)
	if direct == 1 then
		return 272,41
	else
		return 103,41
	end
end

--挂单价格到启用止损按钮距离
function getPrice2StopCheckbox()
		return -157,42
end

--汇丰确定对话框确定按钮位置
function getHFConfirDialogOK()
		return 152,129
end

--止盈按钮到止盈价格输入框距离
function getGainCheckbox2GainPriceEdit()
		return 123,0
end

--获得汇丰止盈距离
function getHuiFengStopGainDiff(direct)
	if direct == 1 then
		--做空止盈
		return 80
	else
		--做多止盈
		return 80
	end
end

--获得天通止盈距离
function getStopGainDiff(direct)
	if direct == 1 then
		--做空止盈
		return 80
	else
		--做多止盈
		return 80
	end
end

--止盈最小距离
function getStopGainThreshold()
	return 12
end

--止损汇丰最小距离
function getHuiFengStopLoseThreshold(direct)
	return 21
end

--止损天通最小距离
function getStopLoseThreshold(direct)
	return 28
end

--确定
function getConfirmButton(direct)
	return -66,65
end

-- 返回1代表海交
-- 返回2代表汇丰
-- 返回3代表天通
-- 返回4代表昆交所
-- 返回5代表柯予
-- 返回6代表更新非农
-- 返回7代表更新中新
-- 返回8代表拉取本地数据服务器
-- 返回9代表拉取百道数据
function getDoubleSideType()
	return 9;
end

function getJoblessParam()
		return "","5.6%","http://rl.fx678.com/Datadetails.aspx?tid=1552&id=32901&fid=3782";
end
--
function getNonfarmerWorkerParam()
		return "" , "23.4", "http://rl.fx678.com/Datadetails.aspx?tid=1547&id=29180&fid=3772"  
end

function getTianTongRetryTimes()
	return 1
end

--返回调试sleep间隔 单位毫秒
function getDebugSleepInterval()
	return 0;
end

function getLuoGeStopLoseDiff()
	return 15
end

function getLuoGeStopGainDiff()
	return 70
end