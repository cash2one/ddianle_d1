--require("../Gene.lua")
--变身
function OnGene1501Add(iEntity,nParam1,nParam2,szPara)
    Transform(iEntity,nParam1,nParam2);
    return 1;
end

function OnGene1501Delete(iEntity,nParam1,nParam2,szPara)
    UnTransform(iEntity);
end

RegisterGeneScript(1501,OnGene1501Add,OnGene1501Delete,GeneType_DurationByParam);