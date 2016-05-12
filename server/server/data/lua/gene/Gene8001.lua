--require("../Gene.lua")
--播放特效的基因
function OnGene8001Add(iEntity,nParam1,nParam2,szPara)
    return 1;
end

function OnGene8001Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(8001,OnGene8001Add,OnGene8001Delete,GeneType_Equip);