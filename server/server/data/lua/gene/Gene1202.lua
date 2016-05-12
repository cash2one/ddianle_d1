--require("../Gene.lua")
--增加目标方人气值的基因
function OnGene1202Add(iEntity,nParam1,nParam2,szPara)
    AddRoleHot(iEntity,nParam1);
    return 1;
end

function OnGene1202Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(1202,OnGene1202Add,OnGene1202Delete,GeneType_OneTime);