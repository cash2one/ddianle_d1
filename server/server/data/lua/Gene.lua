GeneTyp_Duration = 1;
GeneType_Matchs = 2;
GeneType_InRole = 3;
GeneType_Equip = 4;
GeneType_OneTime = 5;
GeneType_DurationByParam = 6;
GeneType_Couple = 7;

local geneAFT = {}
local geneDFT = {}
local geneTYPE = {}

function RegisterGeneScript( ItemID,functionAdd,functionDel,nGeneType)
    if functionAdd== nil or functionDel == nil or nGeneType < 0 then
        print("注册了空的基因脚本函数GeneID:"..ItemID)
    end
    if geneAFT[ItemID]==nil and geneDFT[ItemID]==nil and geneTYPE[ItemID]==nil then 
        geneAFT[ItemID]=functionAdd
        geneDFT[ItemID]=functionDel
        geneTYPE[ItemID]=nGeneType
    else
        print("注册了重复的基因脚本函数GeneID:"..ItemID)
    end
end

function GetGeneType( GeneID )
    if geneTYPE[ GeneID ] ~= nil then
        return geneTYPE[ GeneID ];
    else
        print( "没有找到相应的Gene脚本,ID:"..GeneID )
        return 0
    end
end

function AddGene( iEntity,GeneID,nParam1,nParam2,szParam )
    if geneAFT[ GeneID ] ~= nil then
        return geneAFT[ GeneID ]( iEntity,nParam1,nParam2,szParam ),GetGeneType(GeneID);
    else
        print( "没有找到相应的Gene脚本,ID:"..GeneID )
        return 0
    end
end

function DeletGene( iEntity,GeneID,nParam1,nParam2,szParam )
    if geneDFT[GeneID] ~= nil then
        return geneDFT[ GeneID ]( iEntity,nParam1,nParam2,szParam );
    else
        print( "没有找到相应的Gene脚本,ID:"..GeneID )
        return 0;
    end
end