--require("../Gene.lua")
--跟随称号的基因
function OnGene7002Add(iEntity,nParam1,nParam2,szPara)
    ModifyTitle(iEntity,1,nParam1);
    return 1;
end

function OnGene7002Delete(iEntity,nParam1,nParam2,szPara)
    ModifyTitle(iEntity,0,nParam1);
end

RegisterGeneScript(7002,OnGene7002Add,OnGene7002Delete,GeneType_InRole);