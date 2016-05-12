--require("../Gene.lua")
--宠物耐力喂养物品

function OnGene2003Add(iEntity, nParam1, nParam2, szPara)
    return 1;
end

function OnGene2003Delete(iEntity, nParam1, nParam2, szPara)
end

RegisterGeneScript(2003, OnGene2003Add, OnGene2003Delete, GeneType_OneTime);