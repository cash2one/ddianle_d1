--require("../Gene.lua")
--婚戒基因
function OnGene7007Add(iEntity,nParam1,nParam2,szPara)
    AddRingIntimacy(iEntity,nParam1,nParam2);
    return 1;
end

function OnGene7007Delete(iEntity,nParam1,nParam2,szPara)
	DelRingIntimacy(iEntity,nParam1*-1,nParam2);
end

RegisterGeneScript(7007,OnGene7007Add,OnGene7007Delete,GeneType_Couple);