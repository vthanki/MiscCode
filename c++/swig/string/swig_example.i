%module StringEx_Swig
class StringEx {
        private:
                vector<const char *> names;
        public:
                void addName(const char *n);
                const char *getNameAt(int i);
                void printAll(void);
                int getSize();
};
%inline %{
#include "StringEx.h"
%}
