%module MultiClass_Swig
class Side {
        private:
                int len;
        public:
                Side() {}
                Side(int l) : len(l) {}
                int getSideLength(void);
};

class Polygon {
        private:
                vector<Side> sides;
        public:
                int getNumberOfSides(void);
                int getPerimeter(void);
                int addSide(Side s);
};

%{
#include "MultiClass.h"
%}
