CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lm

SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

TARGET = $(BINDIR)/pa1.out

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR) $(BINDIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)

.PHONY: all clean