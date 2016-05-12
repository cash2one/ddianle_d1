export LIBDIR=-L./mysql -L. -L../DepLib

ServerDir=server
BinDir=bin

SocketDir=socket
DataInfoDir=datainfor
LibServerFrameDir=libserverframe
AgenServerDir=server
AccountServerDir=server
GroupServerDir=server
GameServerDir=server
GMserverDir=server

# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: Socket DataInfor LibServerFrame Agent AccountServer GroupServer GameServer GMServer 

# Builds project 'Socket'...
##########################################
.PHONY: Socket
Socket: 
	@echo "================ make Socket ================"
	@make --directory="$(SocketDir)/" --file=Socket.makefile -e EXT_LIBS="-lmysqlclient -lz -lm -pthread"
	
# Builds project 'DataInfor'...
.PHONY: DataInfor
DataInfor: 
	@echo "================ make DataInfor ================"
	@make --directory="$(DataInfoDir)/" --file=DataInfor.makefile -e EXT_LIBS="-lm -pthread "

# Builds project 'LibServerFrame'...
.PHONY: LibServerFrame
LibServerFrame: 
	@echo "================ make LibServerFrame ================"
	@make --directory="$(LibServerFrameDir)/" --file=LibServerFrame.makefile -e EXT_LIBS="-lm -pthread "
	
# Builds project 'Agent'...
.PHONY: Agent
Agent: Socket LibServerFrame 
	@echo "================ make Agent ================"
	@make --directory="$(AgenServerDir)/" --file=Agent.makefile -e EXT_LIBS="-lLibServerFrame -lSocket -lmysqlclient -lz -lm -pthread -lDataInfor "
	
# Builds project 'AccountServer'...
.PHONY: AccountServer
AccountServer: Socket LibServerFrame 
	@echo "================ make AccountServer ================"
	@make --directory="$(AccountServerDir)/" --file=AccountServer.makefile -e EXT_LIBS="-lLibServerFrame -lSocket -lmysqlclient -lDataInfor -llua5.1 -lz -lm -pthread " 

# Builds project 'GroupServer'...
.PHONY: GroupServer
GroupServer: Socket LibServerFrame DataInfor 
	@echo "================ make GroupServer ================"
	@make --directory="$(GroupServerDir)/" --file=GroupServer.makefile -e EXT_LIBS="-lLibServerFrame -lSocket -lmysqlclient -lDataInfor -llua5.1 -lz -lm -pthread "

# Builds project 'GameServer'...
.PHONY: GameServer
GameServer: Socket LibServerFrame DataInfor 
	@echo "================ make GameServer ================"
	@make --directory="$(GameServerDir)/" --file=GameServer.makefile -e EXT_LIBS="-lLibServerFrame -lSocket -lmysqlclient -lDataInfor -llua5.1 -lz -lm -pthread "

# Builds project 'GMServer'...
.PHONY: GMServer
GMServer: Socket LibServerFrame DataInfor 
	@echo "================ make GMServer ================"
	@make --directory="$(GMserverDir)/" --file=GMServer.makefile -e EXT_LIBS="-lLibServerFrame -lSocket -lmysqlclient -lDataInfor -llua5.1 -lz -lm -pthread "
##########################################

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="$(LibServerFrameDir)/" --file=LibServerFrame.makefile clean
	make --directory="$(AccountServerDir)/" --file=AccountServer.makefile clean
	make --directory="$(GameServerDir)/" --file=GameServer.makefile clean
	make --directory="$(AgenServerDir)/" --file=Agent.makefile clean
	make --directory="$(SocketDir)/" --file=Socket.makefile clean
	make --directory="$(DataInfoDir)/" --file=DataInfor.makefile clean
	make --directory="$(GroupServerDir)/" --file=GroupServer.makefile clean
	make --directory="$(GMserverDir)/" --file=GMServer.makefile clean
	
#############################################################################

.PHONY: tools validclient cdkeymaker

tools:validclient cdkeymaker
	@echo -- Tools ---------------------------------------------------- OK --

validclient: $(SocketDir)/release/validclient.o $(SocketDir)/release/md5.o
	$(CXX) -o $(ServerDir)/$(BinDir)/validclient $(SocketDir)/release/validclient.o $(SocketDir)/release/md5.o
	rm -f $(SocketDir)/release/validclient.o

cdkeymaker: $(SocketDir)/release/keyfilemaker.o $(SocketDir)/release/md5.o $(SocketDir)/release/crypt.o
	$(CXX) -o $(ServerDir)/$(BinDir)/keyfilemaker $(SocketDir)/release/keyfilemaker.o $(SocketDir)/release/md5.o $(SocketDir)/release/crypt.o
	rm -f $(SocketDir)/release/keyfilemaker.o
	
#############################################################################
#############################################################################

.PHONY:serveredition createvalidclient updateserver serverpackage

serveredition:createvalidclient updateserver serverpackage
	@echo -- ServerEdition ---------------------------------------------------- OK --
	
SVR_DIR = /opt/smartphone/lovedance/
SVR_TGZ_DIR = /opt/smartphone/server
PUBLISH_DIR = /opt/publish/Server/

DEBUG_DIR=debug

VER=`date +%m%d`

createvalidclient:
	@echo Create valid clients md5
	cd $(ServerDir)/$(BinDir); ./validclient
	cd $(ServerDir)/$(BinDir); ./keyfilemaker $(VER) hotdance
	
updateserver:
	@echo Copy server edition to $(SVR_DIR)
	chattr -i $(SVR_DIR)/etc/*
	rm $(SVR_DIR)/* -rf
	cp $(ServerDir)/data $(SVR_DIR) -rpuvf
	cp $(ServerDir)/bin $(SVR_DIR) -rupvf
	cp $(ServerDir)/etc $(SVR_DIR) -rupvf
	chattr +i $(SVR_DIR)/etc/*
	mkdir $(SVR_DIR)logs || rm $(SVR_DIR)logs/* -rf
	rm $(SVR_DIR)bin/keyfilemaker -f
	rm $(SVR_DIR)bin/vcproj -f
	rm $(SVR_DIR)bin/validclient -f
	rm $(SVR_DIR)bin/vssver.scc -f
	rm $(SVR_DIR)data/srwcli.mrp -f
	rm -rf $(SVR_DIR)bin/.svn
	rm -rf $(SVR_DIR)data/.svn
	rm -rf $(SVR_DIR)etc/.svn
	rm -rf $(SVR_DIR)logs/.svn
	@echo inner test server ok!
	
serverpackage:
	@echo Create server package for outer test server
	@cd $(SVR_DIR); tar zcvf $(SVR_TGZ_DIR)/adb_$(VER).tgz * || @echo *** Can not pack server file !!!
	md5sum $(SVR_TGZ_DIR)/adb_$(VER).tgz > $(SVR_TGZ_DIR)/adb_$(VER).tgz.md5
	cp $(SVR_TGZ_DIR)/adb_$(VER).tgz* $(PUBLISH_DIR)
	#chown nobody:nobody /opt/smartphone/server/ -R

################################################################
################################################################
# GMServer	
.PHONY:gmedition updategm gmpackage

gmedition: updategm gmpackage
	@echo --GMServerEdition---------------------------------------------------- OK --

GMDIR=/opt/gmserver/source
PACKAGEDIR=/opt/gmserver/archive
GM_PUBLISH_DIR = /opt/publish/Server/

GMVER=`date +%y%m%d`

updategm:
	@echo making diretory...
	@mkdir	-p	$(GMDIR)/GM/etc	|| rm $(GMDIR)/etc/*		-rf
	@mkdir	-p	$(GMDIR)/GM/data	|| rm $(GMDIR)/data/*	-rf
	@echo making directory OK!
	
	@echo Copy gm editon to				$(GMDIR)...
	@cp $(ServerDir)/GM/data			$(GMDIR)/GM		-rpvf
	@cp $(ServerDir)/GM/etc				$(GMDIR)/GM		-rpvf
	@cp $(ServerDir)/bin/GMServer		$(GMDIR)/		-rpvf
	@------echo update gm OK!	

gmpackage:
	@mkdir -p $(PACKAGEDIR)
	@echo Creating package...
	@cd	$(GMDIR)/; tar zcvf $(PACKAGEDIR)/adb_gm$(GMVER).tgz * || @echo *** Can not pack server file!
	md5sum $(PACKAGEDIR)/adb_gm$(GMVER).tgz > $(PACKAGEDIR)/adb_gm$(GMVER).tgz.md5
	cp $(PACKAGEDIR)/adb_gm$(GMVER).tgz* $(GM_PUBLISH_DIR)
