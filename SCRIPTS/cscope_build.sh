find . -name '*.c' > cscope.files
find . -name '*.C' >> cscope.files
find . -name '*.cc' >> cscope.files
find . -name '*.cpp' >> cscope.files
find . -name '*.CPP' >> cscope.files
find . -name '*.h' >> cscope.files
find . -name '*.H' >> cscope.files
find . -name '*.java' >> cscope.files
find . -name '*.sh' >> cscope.files
rm tags
ctags -aBF -L cscope.files
