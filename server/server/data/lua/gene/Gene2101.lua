--require("../Gene.lua")
--增加舞团贡献值
function OnGene2101Add(iEntity,nParam1,nParam2,szPara)
    AddDanceGroupContribution(iEntity,nParam1);
    return 1;
end

function OnGene2101Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(2101,OnGene2101Add,OnGene2101Delete,GeneType_OneTime);