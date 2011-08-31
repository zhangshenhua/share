#makefile - AnserCygnoides

CC=armcc
CXX=armcpp
CXXFLAGS=$(CFLAGS)
CFLAGS+= -W  -o0 -g -Ono-inline   -I .\include;SolutionDir;.\..\WindowLayer;.\..\WidgetLayer;.\..\PageLayer;.\..\Common

LD=$(CXX) $(CXXFLAGS)

LDFLAGS=

LDFLAGS+= 

LIBS+=-lodbc32 -lodbccp32 -lmrewin32 -lmsimg32

TARGET=AnserCygnoides

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

SOURCE_FILES= \

	AnserCygnoides.c \

	AnserCygnoides.def \

	./business.c \

<<<<<<< HEAD
	./mainpage.c \

	./user.c
=======
	../../../../AixGalericulata/KaiXin/AppFramework/Source/AppFramework/user.c
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc

COMMON= \

	../Common/herrorcode.c \

	../Common/herrorcode.h \

	../Common/hlist.c \

	../Common/hlist.h

RESOURCE_FILES= \

PAGELAYER= \

	../PageLayer/page.c \

	../PageLayer/page.h

WIDGETLAYER= \

	../WidgetLayer/hcheckbox.c \

	../WidgetLayer/hcheckbox.h \

	../WidgetLayer/hcontainer.c \

	../WidgetLayer/hcontainer.h \

	../WidgetLayer/himage.c \

	../WidgetLayer/himage.h \

	../WidgetLayer/hlabel.c \

	../WidgetLayer/hlabel.h \

	../WidgetLayer/hlookandfeel.c \

	../WidgetLayer/hlookandfeel.h \

	../WidgetLayer/hplane.c \

	../WidgetLayer/hplane.h \

	../WidgetLayer/hpopupmenu.c \

	../WidgetLayer/hpopupmenu.h \

	../WidgetLayer/hpushbutton.c \

	../WidgetLayer/hpushbutton.h \

	../WidgetLayer/hstatusbar.c \

	../WidgetLayer/hstatusbar.h \

	../WidgetLayer/htextarea.c \

	../WidgetLayer/htextarea.h \

	../WidgetLayer/htextinput.c \

	../WidgetLayer/htextinput.h \

	../WidgetLayer/hwidget.c \

	../WidgetLayer/hwidget.h

WINDOWLAYER= \

	../WindowLayer/hwindow.c \

	../WindowLayer/hwindow.h

INCLUDE_FILES= \

	./connectpage.h \

	./defines.h \

	./mainpage.h

SRCS=$(SOURCE_FILES) $(COMMON) $(RESOURCE_FILES) $(PAGELAYER) $(WIDGETLAYER) $(WINDOWLAYER) $(INCLUDE_FILES) 

OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx ,$(SRCS))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f  $(OBJS) $(TARGET)

