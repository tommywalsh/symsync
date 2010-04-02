QT_INCS:=QtCore QtGui
LDFLAGS+=$(addprefix -l, $(QT_INCS))
QT_PRE:=/usr/include/qt4/
CXXFLAGS+=-g -Wall $(addprefix -I$(QT_PRE), $(QT_INCS)) -I$(QT_PRE)

PLAIN_FILE_BASES:=main FileTreeWidgetItem
MOCABLE_FILE_BASES:=SyncTreeWidget

MOC_FILE_BASES:=$(addsuffix _moc, $(MOCABLE_FILE_BASES))
HAND_FILE_BASES:=$(PLAIN_FILE_BASES) $(MOCABLE_FILE_BASES)
GEN_FILE_BASES:=$(MOC_FILE_BASES)
ALL_FILE_BASES:=$(HAND_FILE_BASES) $(GEN_FILE_BASES)
OBJECT_FILES:=$(addsuffix .o, $(ALL_FILE_BASES))
DEPENDENCY_FILES:=$(addsuffix .d, $(HAND_FILE_BASES))


ssync: $(OBJECT_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

%_moc.cpp : %.hpp
	moc-qt4 -o $@ $^

%.d : %.cpp
	@set -e; rm -f $@; \
 $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
 rm -f $@.$$$$

include $(DEPENDENCY_FILES)

