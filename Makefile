CXX = clang
PROGRAM = svgedit
CXXFILES = main.cc svgedit.cc application.cc svgdisplay.cc svgrender.cc
OBJS = $(CXXFILES:.cc=.o)
DEPDIR = .deps
CXXFLAGS = `pkg-config --cflags gtkmm-3.0 glibmm-2.4 librsvg-2.0` -I/usr/include/rapidxml/ -Iantlr/ -I/usr/include/antlr4-runtime -std=c++14 -Wall -g
LIBS = `pkg-config --libs gtkmm-3.0 glibmm-2.4 librsvg-2.0` -lstdc++ -lantlr4-runtime

ANTLR_CLASSPATH=/usr/share/java/antlr4-runtime.jar
ANTLR_DIR = antlr
ANTLR_SRCS = pathListener.cpp pathParser.cpp pathBaseListener.cpp pathLexer.cpp
ANTLR_OUTS = $(ANTLR_SRCS:%=$(ANTLR_DIR)/%)
ANTLR_HEADERS = $(ANTLR_OUTS:%.cpp=%.h)
ANTLR_OBJS = $(ANTLR_OUTS:%.cpp=%.o)
GRUN=/usr/share/antlr4/grun

RED=$(shell tput setaf 1)
YELLOW=$(shell tput setaf 3)
RESET=$(shell tput sgr0)

all: $(PROGRAM)
-include $(OBJS:%.o=$(DEPDIR)/%.Po)

$(OBJS) $(ANTLR_OBJS): Makefile

%.o: %.cc
	@echo "$(YELLOW)[$(RED)cpp:$(RESET)$@$(YELLOW)]$(RESET) "
	@mkdir -p $(DEPDIR)
	@$(CXX) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(CXXFLAGS) $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

# antlr generates .cpp files, so we duplicate that rule:
%.o: %.cpp
	@echo "$(YELLOW)[$(RED)cpp:$(RESET)$@$(YELLOW)]$(RESET) "
	@mkdir -p $(DEPDIR)
	@$(CXX) -c -o $@ $(CXXFLAGS) $<


$(PROGRAM): $(OBJS) $(ANTLR_OBJS)
	$(CXX) -o $(PROGRAM) $^ $(LIBS)

clean:
	rm -f $(OBJS) $(ANTLR_OBJS)
	rm -f $(PROGRAM)
	rm -rf $(ANTLR_DIR)

install-desktop-file:
	@mkdir -p ~/.local/share/applications
	sed -e "s#@bindir@#$$PWD#" exampleapp.desktop.in > ~/.local/share/applications/$(PROGRAM).desktop
	@mkdir -p ~/.local/share/icons/hicolor/48x48/apps
	cp exampleapp.png ~/.local/share/icons/hicolor/48x48/apps

$(ANTLR_OUTS) $(ANTLR_HEADERS)&: path.g4
	@echo "$(YELLOW)[$(RED)antlr:$(RESET)$@$(YELLOW)]$(RESET) "
	@mkdir -p $(ANTLR_DIR)
	@antlr4 -o $(ANTLR_DIR)/ -Dlanguage=Cpp -visitor $^
	@antlr4 -o $(ANTLR_DIR)/ -Dlanguage=Java -visitor $^

.PHONY: grun
grun: $(ANTLR_OUTS)
	@cd $(ANTLR_DIR) && CLASSPATH=$(ANTLR_CLASSPATH) javac *.java && $(GRUN) path path -tokens

svgedit.o: $(ANTLR_HEADERS)

