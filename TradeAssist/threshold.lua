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

--����ʱ����ֵ����λ��
function getChaseMaxTime()
		return 600;
end

--��������б� ����
function getOrigin2DropListButton()
		return 342, 69;
end

--�е��м۽��� ����
function getOrderTypeButton()
		return 0 , 28;
end

--�е�������������
function getTradeCount()
		return -159,31
end

function getScaleList()
		return 0, 39 --65
end

--1 is do low ����ť 
function getDirectionButton(direct)
	if direct == 1 then
		return -220,124
	else
		return -282,124
	end
end

--�۸����ð�ť
function getPriceAdjustButton(direct)
	if direct == 1 then
		return -36,44
	else
		return 196,44
	end
end

--�ҵ��۸�����ֹ�� 
function getEnableStopButton(direct)
		return -317,83
end

--����ֹ��ť��ֹ����ֵ
function getInitialStopPriceButton(direct)
		return 157,-8
end

--ֹ����ֵ��ֹ��۸�
function getAdjustStopPriceButton(direct)
		return -130,0
end

--�ҵ���Χ���ҵ������
function getPriceRange2Price(direct)
	if direct == 1 then
		return 272,41
	else
		return 103,41
	end
end

--�ҵ��۸�����ֹ��ť����
function getPrice2StopCheckbox()
		return -157,42
end

--���ȷ���Ի���ȷ����ťλ��
function getHFConfirDialogOK()
		return 152,129
end

--ֹӯ��ť��ֹӯ�۸���������
function getGainCheckbox2GainPriceEdit()
		return 123,0
end

--���ֹӯ����
function getStopGainDiff(direct)
	if direct == 1 then
		--����ֹӯ
		return 80
	else
		--����ֹӯ
		return 120
	end
end

--ֹӯ��С����
function getStopGainThreshold()
	return 12
end

--ֹ����С����
function getStopLoseThreshold(direct)
	return 28
end

--ȷ��
function getConfirmButton(direct)
	return -66,65
end

-- ����1������������
-- ����2������
function getDoubleSideType()
	return 2;
end

function getJoblessParam()
		return "","7.4%","http://www.fx678.com/indexs/Datadetails.aspx?tid=1552&id=9959&fid=3782";
end
--
function getNonfarmerWorkerParam()
		return "" , "+18", "http://www.fx678.com/indexs/Datadetails.aspx?tid=1547&id=9899&fid=3772"  
end
