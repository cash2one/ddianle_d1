--0 һ�����
--3
--2
--1 GM
--0xffffffff Admin,����Ա�����Ȩ��


local commands = 
{
--���GMȨ��
	GrantGMCmd			= {0xffffffff},						-- Ȩ����Ȩ	
--ɾ��GMȨ��
	RemoveGMCmd			= {0xffffffff},						-- Ȩ��ɾ��	
--���淢��
	Announce			= {1,0xffffffff},					-- ���͹���
--�ʼ�����
	SendSysMail			= {0xffffffff},						-- ����ϵͳ�ʼ�
--�һ���
	GenGiftCode			= {1,0xffffffff},					-- �����һ���
	QueryGiftCode		= {1,0xffffffff},					-- ��ѯ������
--����ϵͳ����
   AdminChat			= {1, 0xffffffff},
}

function GetPrivsInfo(commands, value)
	if commands ~= nil then	
		local count = table.getn(value);
		for i = 1, count do
			AddPriv(commands, value[i]);
		end
	end
end

table.foreach(commands, GetPrivsInfo);


