CXX = clang
PROGRAM = svgedit
CXXFILES = main.cc svgedit.cc
OBJS = $(CXXFILES:.cc=.o)
DEPDIR = .deps
CXXFLAGS = `pkg-config --cflags gtkmm-3.0` -std=c++11 -Wall -g
LIBS = `pkg-config --libs gtkmm-3.0` -lstdc++
all: $(PROGRAM)
-include $(OBJS:%.o=$(DEPDIR)/%.Po)
%.o: %.cc
	@mkdir -p $(DEPDIR)
	$(CXX) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(CXXFLAGS) $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS) $(LIBS)
clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)
install-desktop-file:
	@mkdir -p ~/.local/share/applications
	sed -e "s#@bindir@#$$PWD#" exampleapp.desktop.in > ~/.local/share/applications/$(PROGRAM).desktop
	@mkdir -p ~/.local/share/icons/hicolor/48x48/apps
	cp exampleapp.png ~/.local/share/icons/hicolor/48x48/apps
