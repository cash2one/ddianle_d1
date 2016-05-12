--require("../Gene.lua")
--播放特效的基因
function OnGene8002Add(iEntity,nParam1,nParam2,szPara)
    return 1;
end

function OnGene8002Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(8002,OnGene8002Add,OnGene8002Delete,GeneType_Equip);